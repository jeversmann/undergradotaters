/*	$Id: briggs_reg_alloc.cc,v 1.11 2002/11/25 07:00:57 cnevill Exp $	*/

#include <c_breeze.h>
#include "briggs_reg_alloc.h"
#include <limits>
#include "LIR.h"
#include "math.h"
#include "instruction.h"
#include "LIR.h"
#include "hash_set_ex.h"
#include "lir_flow_walker.h"

const int ListRecord::color_none = INT_MIN;

bool Symbol::operator ==( const Symbol & rhs ) const
{
	// must have same type
	if ( _type != rhs._type )
		return false;

	// compare the right thing
	switch ( _type )
	{
	case sym_id:	return (rhs._id == _id); // FIXME: can we use pointer equivalence here?
	case sym_reg:	return (rhs._reg == _reg);
	case sym_const:	return _const.is_equal_to(rhs._const);
	default:		assert( false ); return false;
	}
}

lir_var_type Symbol::getLirVt() const
{
	// compare the right thing
	switch ( _type )
	{
	case sym_id:	
		assert( _id ); 
		return LirVt::getVarType( _id );

	case sym_reg:	

		// what kind of register is it?
		if ( _reg.type() == reg_gpr || 
			_reg.type() == reg_frame_ptr ||
			_reg.type() == reg_stack_ptr )
		{
			// it's a gpr
			return CBZ::ArchInfo.get_reg_data_type_gpr();
		}
		else if ( _reg.type() == reg_fpr )
		{
			// it's an fpr
			return CBZ::ArchInfo.get_reg_data_type_fpr();
		}
		else
		{
			// this is an invalid register.
			assert( false );
			return vt_unknown;
		}

	case sym_const:	
		return LirVt::getVarType( _const.basic() );

	default:		
		assert( false ); 
		return vt_unknown;
	}
}

bool WebRecord::intersectUses( DUChainUses & u )
{
	for (DUChainUses_p p = u.begin(); p != u.end(); ++p) {
		for (DUChainUses_p q = uses.begin(); q != uses.end(); ++q) {
			if (*p == *q)
				return true;
		}
	}

		//	pnav -- use below if pointer equivalence fails from hash_multiset.find()
		//					I'm assuming that the def-use chain method doesn't clone nodes before
		//					putting them in chains.
//			for (DUChainUses_p q = uses->begin(); q != uses->end(); ++q) {
//				if (**p == **q) { // pnav -- get to the actual node object to test equality
//		return true;
//				}
//			}

	return false;
}

void WebRecord::unionDefs( DUChainDefs & d ) 
{
	for (DUChainUses_p p = d.begin(); p != d.end(); ++p) {
		defs.insert(defs.end(), *p);
	}
}

void WebRecord::unionUses( DUChainUses & u )
{
	for (DUChainUses_p p = u.begin(); p != u.end(); ++p) {
		uses.insert(uses.end(), *p);
	}
}

void ListRecord::addAdjacentNode( int reg ) 
{
	assert( reg != webNum );

	// add to our list
	adjNodes.insert(reg); 
	num_ints++;
}

void ListRecord::removeAdjacentNode( int reg ) 
{
	bool found = false;

	// remove any instances of reg from our adjacent list
	int_set_p it = adjNodes.find( reg );
	assert( it != adjNodes.end() );

	// remove from our set
	adjNodes.erase( it );
	num_ints--;

	// remember that we were adjacent to this
	removeAdj.insert( reg );
}
	
void ListRecord::removeAllAdjacencies()
{
	// move all remaining into removed set
	removeAdj.insert( adjNodes.begin(), adjNodes.end() );
	adjNodes.clear();
	num_ints = 0;
}

const float briggs_reg_alloc::useWt = 1.0;
const float briggs_reg_alloc::copyWt = 1.0;
const float briggs_reg_alloc::defWt = 1.0;

template< class T >
set< T > * briggs_reg_alloc::copySet( set<T> * s )
{
	set<T> *retval = new set<T>();
	for (set<T>::iterator p = s->begin(); p != s->end(); ++p) {
		retval->insert(*p);
	}

	return retval;
}


bool briggs_reg_alloc::interfere( WebRecord	*wr, int reg) 
{
	// everything conflicts with stack ptr and frame ptr - these can never be
	//	allocated for any other use
	if ( CBZ::ArchInfo.get_reg_fp()->_id == reg || 
		CBZ::ArchInfo.get_reg_sp()->_id == reg )
		return true;

	// this had better be a real register - if not, it interferes
	const arch_info::register_info_list & allregs = CBZ::ArchInfo.get_all_regs();
	if ( reg < 0 || reg >= (int)allregs.size() )
		return true;
	const arch_info::register_info * regInfo = allregs[reg];
	if ( ! regInfo )
		return true;

	// find out the type of this symbol
	const Symbol & sym = wr->getSym();
	lir_var_type vt = sym.getLirVt();

	// if the types don't match, we have interference 
	// i.e. if it's a float, it can't go in gpr, etc.
	if ( regInfo->_type == reg_fpr && ! LirVt::isTypeFloat( vt ) )
		return true;
	if ( regInfo->_type == reg_gpr && ! LirVt::isTypeFixed( vt ) )
		return true;

	///////////////////////////////////////////////////////////////////////
	// here's the nasty part - it conflicts with a return value register
	//	if it's live across a function call instruction that writes that 
	//	register.  for this we will use liveness info computed in makeWebs

	unsigned int rvFixedId = (unsigned int)-1;
	if ( CBZ::ArchInfo.get_reg_retval_fixed() )
		rvFixedId = CBZ::ArchInfo.get_reg_retval_fixed()->_id;
	unsigned int rvFloatId = (unsigned int)-1;
	if ( CBZ::ArchInfo.get_reg_retval_float() )
		rvFloatId = CBZ::ArchInfo.get_reg_retval_float()->_id;

	if ( rvFixedId == reg || rvFloatId == reg )
	{
		instruction_list & insts = proc->instructions();

		// for all call instructions, find out if this web's symreg is live
		unsigned int symRegNum = wr->getReg().num();
		instruction_list_p it = insts.begin();
		LirInst * pInst;
		for ( ; it != insts.end(); ++it )
		{
			// get instrution and see if it's a call
			pInst = *it;
			if ( pInst->instruction != mn_Call )
				continue;

			// see what the actual return register will be - if return value 
			//	will not be in this register, there is no confict here
			lir_var_type vt = LirVt::getVarType( pInst->nodeExtra );
			if ( LirVt::isTypeFixed( vt ) && rvFixedId != reg )
				continue;
			if ( LirVt::isTypeFloat( vt ) && rvFloatId != reg )
				continue;

			// is symreg live at this call?  if so it cannot go in return value register
			reg_id_set live = liveRegs[ pInst ];
			if ( (live & symRegNum).size() != 0 )
				return true;
		}
	}

	return false;
}

bool briggs_reg_alloc::liveAt( WebRecord *wr, DUChainDef def ) 
{
	// just see if the symbolic register for this web is live at
	//	the definition point in question.
	const reg_id_set &live = liveRegs[ def ];
	return (live.find( wr->getReg().num() ) != live.end());
}

bool briggs_reg_alloc::nonStore( int reg_k, int reg_l, instruction_list_p instr )
{
	// look at all instructions after the starting instruction to end of block
	instruction_list & insts = proc->instructions();
	for (++instr; instr != insts.end(); ++instr) {

		// instruction of interest
		LirInst * inst = *instr;

		// does it have a destination?
		if ( ! inst->has_dest( true ) )
			continue;

		// does it write to k or l?
		if ( inst->dest.num() == reg_k || inst->dest.num() == reg_l )
			return false;
	}

	// apparently neither was stored to...
	return true;
}

void briggs_reg_alloc::deleteInstr( instruction_list_p it ) 
{
	// just nuke this from the instruction list (and from its block)
	assert( proc );
	LirInst * inst = *it;
	if ( inst->block )
		inst->block->remove_inst( inst );
	proc->instructions().erase( it );
}

int briggs_reg_alloc::depth( instruction_list_p it )
{
	// see if we have a block for it
	LirInst * inst = *it;
	if ( ! inst->block )
	{
		cerr << "WARNING: LIR block flow info is missing - loop depth defaulted to 0." << endl;
		return 0;
	}

	// return depth of block
	return inst->block->_depth;
}

void briggs_reg_alloc::makeDuChains( sduList & DuChains )
{
	instruction_list & insts = proc->instructions();
	instruction_list_p ilp;

	// handy typedefs
	typedef hash_set_ex<DUChainDef> def_set;
	typedef map<Register, def_set> def_map;
	typedef hash_set_ex<DuChainUse> use_set;
	typedef map<Register, use_set> use_map;

	// map to hold current def and uses for each register
	reg_set current_regs;
	def_map defs_all;
	use_map uses_all;

	// generate postorder ordering of instructions, reverse to get reverse postorder
	instruction_list worklist = insts;
	lir_flow_walker::getInstructionFlowInfo( insts );
	lir_flow_walker::getInstructionsPostorder( insts, insts.begin(), worklist );
	worklist.reverse();

	// for debugging - trace out instructions in reverse postorder
	// change 0 to 1 to add this code
#if 0
	{
		cout << "DEBUG:" << endl;
		cout << "Reverse Postorder:" << endl;
		instruction_list_p it = rpo.begin();
		for ( ; it != rpo.end(); ++it )
			cout << **it << endl;
		cout << "END DEBUG" << endl;
	}
#endif

	// go through and find defs and uses
	for (ilp = worklist.begin(); ilp != worklist.end(); ++ilp)
	{
		LirInst * inst = *ilp;

		// add uses for this instruction
		if ( inst->has_opnd1() )
			uses_all[ inst->opnd1 ].insert( inst );
		if ( inst->has_opnd2() )
			uses_all[ inst->opnd2._reg ].insert( inst );
		if ( inst->has_base() )
			uses_all[ inst->memBase ].insert( inst );
		if ( inst->has_offset() )
			uses_all[ inst->memOffset._reg ].insert( inst );

		// do we define anything?
		if ( inst->has_dest( true ) )
			defs_all[ inst->get_dest() ].insert( inst );
	}

	// we don't care about defs or uses of any real registers - they are not candidates for allocation
	for ( int i = 0; i < num_regs; ++i )
	{
		Register reg;
		CBZ::ArchInfo.get_reg_by_index( i, reg );
		defs_all.erase( reg );
		uses_all.erase( reg );
	}

	// map from register to instructions that def that register
	typedef map<unsigned int, hash_set_ex<DUChainDef> > reg_to_def_map;

	// map from instruction to defs that reach that instruction
	typedef map<LirInst*, reg_to_def_map> reach_map;

	// do reaching definitions analysis over all instructions, in reverse postorder
	reach_map reach;
	while ( worklist.size() != 0 )
	{
		LirInst * inst = worklist.front();
		worklist.pop_front();

		// figure out what reaches from predecessors
		reg_to_def_map thisReach;
		instruction_set_p pit = inst->preds.begin();
		for ( ; pit != inst->preds.end(); ++pit )
		{
			// what reaches this predecessor?
			reg_to_def_map & pdefs = reach[ *pit ];
			reg_to_def_map::iterator regit = pdefs.begin();
			for ( ; regit != pdefs.end(); ++regit )
				thisReach[ regit->first ] |= regit->second;
		}

		// adjust for what we define
		Register defReg;
		if ( inst->has_dest( true ) )
		{
			hash_set_ex<DUChainDef> & defs = thisReach[ inst->get_dest().num() ];

			// kill any previous and set ours as the only one
			defs.clear();
			defs.insert( inst );
		}

		// has this changed?
		if ( thisReach != reach[ inst ] )
		{
			// update new reaching info for this instruction
			reach[ inst ] = thisReach;

			// add all successors back to worklist
			instruction_set_p succit = inst->succs.begin();
			for ( ; succit != inst->succs.end(); ++succit )
				if ( ! cbz_util::list_find( worklist, *succit ) )
					worklist.push_back( *succit );
		}
	}

	// now build def-use chains by finding which defs reach which uses
	def_map::iterator dmit = defs_all.begin();
	for ( ; dmit != defs_all.end(); ++dmit )
	{
		Register reg = dmit->first;

		// iterate over each def of this register
		def_set & defs = dmit->second;
		def_set::iterator def = defs.begin();
		for ( ; def != defs.end(); ++def )
		{
			// a new sdu for this thing
			Symbol * sym = NULL;
			if ( (*def)->dest_contents )
				sym = new Symbol( (*def)->dest_contents );
			else 
				sym = new Symbol( reg );
			sdu * defuse = new sdu( *sym, *def );
			DuChains.push_back( defuse );
			delete sym;
			DUChainUses & uses = defuse->getUses();

			// find all uses that this reaches
			use_set & possibles = uses_all[ reg ];
			use_set::iterator pit = possibles.begin();
			for ( ; pit != possibles.end(); ++pit )
			{
				// is this def live at this use?
				hash_set_ex<DUChainDef> & reach_defs = reach[ *pit ][ reg.num() ];
				if ( reach_defs.find( *def ) != reach_defs.end() )
					uses.push_back( *pit );
			}
		}
	}

	// kill defs from decl nodes that were unused
	sduList::iterator it = DuChains.begin();
	for ( ; it != DuChains.end(); ++it )
	{
		// if this def is a decl node, and it does not have any uses, throw it away
		sdu * du = *it;
		if ( du->getDef()->instruction == mn_DeclLocal && du->getUses().size() == 0 )
			DuChains.erase( it-- );
	}

	// debug - print out def-use chains
#if 0
	{
		sduList::iterator it = DuChains.begin();
		for ( ; it != DuChains.end(); ++it )
		{
			sdu * du = *it;
			cout << "DefUse:" << endl;
			cout << "  Def: " << *(du->getDef()) << endl;
			cout << "  Uses: " << endl;
			DUChainUses::iterator uses = du->getUses().begin();
			for ( ; uses != du->getUses().end(); ++uses )
				cout << "    " << **uses << endl;
			cout << endl;
		}
	}
#endif

}

void briggs_reg_alloc::makeWebs(sduList & DuChains)
{
	// make a semi-web for each du chain
	// also keep track of largest register # currently in use
	num_webs = 0;
	WRSet *Webs = new WRSet();
	num_webs = num_regs;
	for (sduList_p p = DuChains.begin(); p != DuChains.end(); ++p) {

		sdu * chain = *p;

		// figure out which virtual register is being used for this duchain
		Register webreg = chain->getDef()->get_dest();
		if ( ! webreg.isValid() && chain->getSymbol()._type == Symbol::sym_reg )
			webreg = chain->getSymbol()._reg;
		assert( webreg.isValid() );
		if ( num_webs < (int)webreg.num() )
			num_webs = webreg.num();

		// make a web for this du chain
		WebRecord *wr = new WebRecord(chain->getSymbol(), webreg);
		wr->getDefs().push_back( chain->getDef() );
		wr->getUses() = chain->getUses();
		Webs->insert(wr);
	}

	// merge semi-webs into webs
	WRSet *Tmp1, *Tmp2;
	WebRecord *web1, *web2;
	bool changed;
	do {
		changed = false;
		Tmp1 = copySet(Webs);

		while (!Tmp1->empty()) {
			web1 = *(Tmp1->begin());	
			Tmp1->erase(web1);
			Tmp2 = copySet(Webs);
			Tmp2->erase(web1);
			
			while (!Tmp2->empty()) {
				web2 = *(Tmp2->begin());	
				Tmp2->erase(web2);

				// do these two intersect?
				if ((web1->getSym() == web2->getSym()) && web1->intersectUses(web2->getUses())) {
					assert( web1 != web2 );
					assert( web1->getReg() == web2->getReg() );

					// union web2 into web1
					web1->unionDefs( web2->getDefs() );
					web1->unionUses( web2->getUses() );

					// kill this web
					Webs->erase(web2);
					Tmp1->erase(web2);
					Tmp2->erase(web2);
					changed = true;
				}
			}
		}

	} while (changed);

	// give each web a unique symbolic register
	set<unsigned int> usedRegs;
	unsigned int nextreg = num_regs + 1;
	for (WRSet_p w = Webs->begin(); w != Webs->end(); ++w) {

		// this web's new register 
		Register newReg( (*w)->getReg() );
		newReg = nextreg++;

		// fix it to match
		changeWebRegister( *w, newReg );
	}

	// compute total number of webs
	num_webs = nextreg;

	// kill old symbolic register info
	Symreg.clear();
	Symreg.resize( num_webs );

	// setup info for symbolic registers
	for (int i = 0; i < num_regs; i++) {
		Register reg;
		CBZ::ArchInfo.get_reg_by_index(i, reg);
		WebRecord * wr = new WebRecord(reg, reg);
		Symreg[ i ] = wr;
	}
	for (WRSet_p p = Webs->begin(); p != Webs->end(); ++p) {
		// add to symregs
		int num = (*p)->getReg().num();
		assert( num >= 0 && num < (int)Symreg.size() );
		Symreg[ num ] = *p;
	}

	// print out all of the instructions
#if 0
	{
	cout << "Instruction listing after makeWebs:" << endl;
	instruction_list::const_iterator it = proc->instructions().begin();
	int num = 0;
	while ( it != proc->instructions().end() )
		cout << num++ << " " << **(it++) << endl;
	cout << endl << endl;
	}
#endif

#if 0
	{
	cout << "Color assignments:" << endl;
	for (WRSet_p p = Webs->begin(); p != Webs->end(); ++p) {
		cout << "  Color: " << (*p)->getReg().to_string() << endl;
		DUChainDefs & defs = (*p)->getDefs();
		DUChainDefs::iterator it = defs.begin();
		for ( ; it != defs.end(); ++it )
			cout << "    Def At: " << **it << endl;
	}
	cout << endl << endl;
	}
#endif


}

void briggs_reg_alloc::buildAdjMatrix() 
{
	int size = num_webs;
	int i, j;

	adjMatrix.clear();

	// recompute liveness info
	lir_flow_walker::computeRegisterLiveness(proc->instructions(), liveRegs);

	// pnav -- this matrix is oversized.	Needs only be a triangluar matrix
	//				 alg will only use bottom half, but we're wasting half the bits.
	adjMatrix.resize( size );
	for (i = 0; i < size; i++)
		adjMatrix[i].resize( size );

	for (i = 0; i < size; i++) {
		for (j =0; j < size; j++) {
			adjMatrix[i][j] = false;
		}
	}

	for (i = 1; i < num_regs; i++) {
		for (j = 0; j < i; j++) {
			adjMatrix[i][j] = true;
		}
	}

	for (i = num_regs; i < num_webs; i++) {
		if (!Symreg[i])
			continue;

		// determine if web in Symreg[i] interferes with real reg j
		for (j = 0; j < num_regs; j++) {
			if (interfere(Symreg[i], j))
				adjMatrix[i][j] = true;
		}

		// determine whether or not 2 symregs interfere
		for (j = num_regs; j < num_webs; j++) {
			if (!Symreg[j] || i == j)
				continue;

			// determine which webs interfere
			DUChainDefs & defs = Symreg[i]->getDefs();
			for (DUChainDefs_p p = defs.begin(); p != defs.end(); ++p) {
				if (liveAt(Symreg[j], *p))
					adjMatrix[max(i,j)][min(i,j)] = true;
			}
		}
	}

#if 0
	{
	unsigned int rfp = CBZ::ArchInfo.get_reg_fp()->_id;
	unsigned int rsp = CBZ::ArchInfo.get_reg_sp()->_id;

	// print out adjacency lists
	cout << "Interference graph:" << endl;
	for (i = num_regs; i < num_webs; ++i)
		for (j = 0; j < i; ++j)
		{
			if ( i == rfp || i == rsp || j == rfp || j == rsp )
				continue;

			Register r1( i, reg_unknown );
			Register r2( j, reg_unknown );
			cout << r1.to_string() << "<->" << r2.to_string() << " : ";
			cout << (adjMatrix[i][j] ? "bad" : "ok") << endl;
		}
	}
#endif

}


bool briggs_reg_alloc::coalesceRegisters()
{
	int k,l,p;

	instruction_list & insts = proc->instructions();
	instruction_list_p it = insts.begin();
	bool changed = false;
	for (; it != insts.end(); ++it) {

		// instruction of interest
		LirInst * inst = *it;

		// if this instruction is a coalescable copy (K <- L)
		// adjust assignments to its source to assign to its target
		if (inst->instruction == mn_Move && isSymReg(inst->opnd1)) {
			k = inst->dest.num();
			l = inst->opnd1.num();
#ifdef _DEBUG
			cout << "Coalescing " << l << " into " << k << endl;
#endif
			if ( !adjMatrix[max(k,l)][min(k,l)] || nonStore(k, l, it) ) {

				// remove the copy instruction (also adjust loop var)
				Symreg[l]->getDefs().remove( *it );
				deleteInstr( it-- ); 

				// change the L web to match the K web
				changeWebRegister( Symreg[l], Symreg[k]->getReg() );

				// combine Symregs
				Symreg[k]->unionDefs(Symreg[l]->getDefs());
				Symreg[k]->unionUses(Symreg[l]->getUses());

				// adjust adjacency matrix to reflect removal of the copy
				Symreg[l] = NULL;
				for (p = 0; p < num_webs; p++)
					if ( adjMatrix[max(p,l)][min(p,l)] )
						adjMatrix[max(p,k)][min(p,k)] = true;

				// we changed things
				changed = true;
			}
		}
	}

	return changed;
}


void briggs_reg_alloc::buildAdjLists()
{
	int i, j;

	adjLists.clear();

	// initialize adjacency lists
	// infinite spill cost for 'real' registers
	for (i = 0; i < num_regs; i++) {
		adjLists.push_back(new ListRecord(i, std::numeric_limits<float>::infinity()));
	}
	// zero spill cost for 'symbolic' registers
	for (i = num_regs; i < num_webs; i++) {
		adjLists.push_back(new ListRecord(i, 0.0));
	}

	// build lists based on adjacency matrix
	for (i = 1; i < num_webs; i++) {
		for (j = 0; j < (num_webs - 1); j++) {
			if (adjMatrix[i][j]) {
	adjLists[i]->addAdjacentNode( j ); // num_ints incremented in func
	adjLists[j]->addAdjacentNode( i );
			}
		}
	}
	
}


void briggs_reg_alloc::computeSpillCosts()
{
	instruction_list & insts = proc->instructions();
	instruction_list_p it = insts.begin();
	for (; it != insts.end(); ++it) {

		LirInst * instr = *it;
		if ( instr->instruction == mn_Move ) {

			// decrease weight by one copy
			adjLists[ instr->opnd1.num() ]->incCost( copyWt * (float)pow( 10, depth(it) ) );
		}
		else {

			// has destination?
			if ( instr->has_dest() )
				adjLists[ instr->dest.num() ]->incCost( defWt * (float)pow( 10, depth(it) ) );

			// has opnd1?
			if ( instr->has_opnd1() )
				adjLists[ instr->opnd1.num() ]->incCost( defWt * (float)pow( 10, depth(it) ) );

			// has opnd2?	(not constant)
			if ( instr->has_opnd2() )
				adjLists[ instr->opnd2._reg.num() ]->incCost( defWt * (float)pow( 10, depth(it) ) );

			// has base?
			if ( instr->has_base() )
				adjLists[ instr->memBase.num() ]->incCost( defWt * (float)pow( 10, depth(it) ) );

			// has offset?	(not constant)
			if ( instr->has_offset() )
				adjLists[ instr->memOffset._reg.num() ]->incCost( defWt * (float)pow( 10, depth(it) ) );
		}
	}

	// replace spill cost by rematerialization cost if it is less
	// pnav -- do this if we have time...
//		for (i = num_regs; i < num_webs; i++) {
		
//		}
}

void briggs_reg_alloc::pruneGraph() 
{
	bool finished;
	int i, nodes = num_webs;
	nodeStack.clear();

	// keeps track of which nodes have already been pulled from graph
	int_set okNodes;

	do {
		// apply (degree < R) rule and push nodes onto stack
		do {
			finished = true;
			for (i = 0; i < num_webs; i++) {

				// can it be trivially colored?
				if (adjLists[i]->numInts() > 0 && (i < num_regs || adjLists[i]->numInts() < usable_regs)) {
					finished = false;
					adjustNeighbors(i);
				}

				// if there is a web for it and there are no interferences, put it on the stack
				if (adjLists[i]->numInts() == 0 && okNodes.find(i) == okNodes.end()) {
					okNodes.insert(i);
					if (Symreg[i]) 
						nodeStack.push_back(i);
					nodes--;
					assert( nodes >= 0 );
				}
			}
		} while (!finished);
		if (nodes > 0) {
			// find node with minimal spill cost divided by its degree
			// push that node onto the stack
			int spillnode = -1;
			float spillcost = std::numeric_limits<float>::max();
			for (i = 0; i < num_webs; i++) {

				// ignore nodes with no interferences or infinte cost
				if (adjLists[i]->numInts() == 0 || adjLists[i]->getSpillCost() == numeric_limits<float>::infinity())
					continue;

				// evaluate spill cost
				float tmpcost = adjLists[i]->getSpillCost() / adjLists[i]->numInts();
				if (adjLists[i]->numInts() > 0 && tmpcost < spillcost) {
					spillnode = i;
					spillcost = tmpcost;
				}
			}
			assert(spillnode != -1);
			adjustNeighbors(spillnode);
		}
	} while (nodes > 0);
}

void briggs_reg_alloc::adjustNeighbors( int i ) 
{
	// fixup all of i's neighbors
	const int_set & adjNodes = adjLists[i]->getAdjNodes();
	int_set::const_iterator it = adjNodes.begin();
	for ( ; it != adjNodes.end(); ++it )
	{
		assert( *it != i );
		adjLists[ *it ]->removeAdjacentNode( i );
	}

	// remove all entries from i
	adjLists[i]->removeAllAdjacencies();
}

bool briggs_reg_alloc::assignRegisters() 
{
	int i;

	// make list of available colors
	color_set colors;
	const arch_info::register_info_list & regsGpr = CBZ::ArchInfo.get_regs_gpr();
	for ( i = 0; i < (int)regsGpr.size(); ++i )
		colors.insert( regsGpr[i]->_id );
	const arch_info::register_info_list & regsFpr = CBZ::ArchInfo.get_regs_fpr();
	for ( i = 0; i < (int)regsFpr.size(); ++i )
		colors.insert( regsFpr[i]->_id );
	int rvFixed = -1;
	if ( CBZ::ArchInfo.get_reg_retval_fixed() )
		rvFixed = CBZ::ArchInfo.get_reg_retval_fixed()->_id;
	int rvFloat = -1;
	if ( CBZ::ArchInfo.get_reg_retval_float() )
		rvFloat = CBZ::ArchInfo.get_reg_retval_float()->_id;

	// reset this thing - we will be changing its info
	colorToRealReg.clear();
	colorToRealReg.resize( num_regs );

	// pop webs off the stack and assign colors if possible
	bool success = true;
	while ( ! nodeStack.empty() )
	{
		// pop one off
		int reg = nodeStack.front();
		nodeStack.pop_front();
		ListRecord * adj = adjLists[reg];
		WebRecord * web = Symreg[reg];

		// real registers get their own number as their color
		if ( reg < num_regs )
		{
			adj->getColor() = reg;
			colorToRealReg[ reg ] = reg;
			continue;
		}

		// get this node's info and available colors
		color_set okColors = colors;
		removeUnavailableColors( adj, okColors );

		// any colors that we can use?
		if ( okColors.empty() )
		{
			// this has to be spilled
#ifdef _DEBUG
			cout << "Don't know how to color register " << reg << " : spilling." << endl;
#endif
			web->getSpill() = true;
			success = false;
		}

		// pick a color to use (this should be improved to try to pick one that 
		//	cannot be used by an adjacent node)
		int color = *(okColors.begin());
		adj->getColor() = color;
	}

	return success;
}

void briggs_reg_alloc::removeUnavailableColors( ListRecord * adj, color_set & colors ) 
{
	// take out unavailable colors (used by adjacent nodes)
	const int_set & adjNodes = adj->getAdjNodes(), & rmadjNodes = adj->getRmAdjNodes();
	int_set::const_iterator it;
	for ( it = adjNodes.begin(); it != adjNodes.end(); ++it )
	{
		int color = adjLists[ *it ]->getColor();
		if ( color != ListRecord::color_none )
			colors -= color;
	}
	for ( it = rmadjNodes.begin(); it != rmadjNodes.end(); ++it )
	{
		int color = adjLists[ *it ]->getColor();
		if ( color != ListRecord::color_none )
			colors -= color;
	}
}

void briggs_reg_alloc::modifyCode() 
{
	// fixup each web as necessary
	for ( int i = num_regs; i < num_webs; ++i )
	{
		// get the web, skip empties
		WebRecord * web = Symreg[i];
		if ( ! web )
			continue;

		// fixup this web
		Register reg;
		CBZ::ArchInfo.get_reg_by_index( colorToRealReg[ adjLists[i]->getColor() ], reg );
		changeWebRegister( web, reg );
	}
}

void briggs_reg_alloc::changeWebRegister( WebRecord * web, const Register & newReg ) 
{
	// this web's new register 
	Register oldReg( web->getReg() ); 
	web->getReg() = newReg;

	// fixup all defs to match
	DUChainDefs & defs = web->getDefs();
	DUChainDefs::iterator def = defs.begin();
	for ( ; def != defs.end(); ++def ) {
		LirInst * pdef = *def;

		// if it has a dest it should be the old register
		assert( ! pdef->has_dest( false ) || pdef->dest == oldReg );

		// does it have an obvious destination?
		if ( pdef->has_dest( false ) )
			pdef->dest = newReg;

		// is it a register variable?
		else if ( pdef->has_dest( true ) )
			((declNode*)pdef->nodeExtra)->storage_location()._register = newReg;
	}

	// fixup all uses to match
	DUChainDefs & uses = web->getUses();
	DUChainDefs::iterator use = uses.begin();
	for ( ; use != uses.end(); ++use ) {
		LirInst * pdef = *use;

		// fixup used registers
		if ( pdef->has_opnd1() && pdef->opnd1 == oldReg )
			pdef->opnd1 = newReg;
		if ( pdef->has_opnd2() && pdef->opnd2._reg == oldReg )
			pdef->opnd2._reg = newReg;
		if ( pdef->has_base() && pdef->memBase == oldReg )
			pdef->memBase = newReg;
		if ( pdef->has_offset() && pdef->memOffset._reg == oldReg )
			pdef->memOffset._reg = newReg;
	}
}

void briggs_reg_alloc::loadSymReg( Register & reg, declNode * contents, 
	instruction_list_p currentInstruction, instruction_list& insts )
{
	LirInst * current = *currentInstruction;

	// make sure contents are valid
	assert( contents > DATA_CONTENTS__SPECIAL );

	// make sure it has stack storage 
	// NOTE: we currently assume that register input params do not have to get spilled, thus the assert()
	assert( contents->decl_location() == declNode::BLOCK );
	proc->alloc_stack_local( contents );

	// reload it
	LirInst * load = Lir_Class::Load( LirVt::getVarType( contents ), 
		reg, contents, 
		Register::getRegFp(), DATA_CONTENTS_FRAMEP, 
		contents->storage_location()._stack_offset, NULL );

	// put this instruction in the right place (before the current instruction)
	insts.insert( currentInstruction, load );
	if ( current->block )
		current->block->add_inst_after( load, current );
}

void briggs_reg_alloc::genSpillCode() 
{
	// everyone that's supposed to be spilled must be spilled....
	instruction_list & insts = proc->instructions();
	instruction_list_p it = insts.begin();
	for ( ; it != insts.end(); ++it ) {
		LirInst * inst = *it;

		// if we already loaded something this time around, this is its register
		// NOTE: we use this for the case where we have (R1 = R2 op R2), and R2 
		//	was spilled, so we don't reload R2 twice
		Register alreadyLoaded;

		// reload opnd1?
		if ( inst->has_opnd1() && isSymReg( inst->opnd1 ) && 
			Symreg[inst->opnd1.num()]->getSpill() ) {

			// load it back into its register
			loadSymReg( inst->opnd1, inst->opnd1_contents, it, insts );

			// remember that we already loaded this in case opnd2 uses it as well
			alreadyLoaded = inst->opnd1;
		}

		// reload opnd2 register?
		if ( inst->has_opnd2() && isSymReg( inst->opnd2._reg ) && 
			Symreg[inst->opnd2._reg.num()]->getSpill() && inst->opnd2._reg != alreadyLoaded ) {

			// load it back into its register
			loadSymReg( inst->opnd2._reg, inst->opnd2_contents, it, insts );
		}

		// reload base register?
		if ( inst->has_base() && isSymReg( inst->memBase ) && 
			Symreg[inst->memBase.num()]->getSpill() ) {

			// load it back into its register
			loadSymReg( inst->memBase, inst->memBase_contents, it, insts );
		}

		// reload offset register?
		if ( inst->has_offset() && isSymReg( inst->memOffset._reg ) && 
			Symreg[inst->memOffset._reg.num()]->getSpill() ) {

			// load it back into its register
			loadSymReg( inst->memOffset._reg, inst->memOffset_contents, it, insts );
		}

		// spill destination?
		if ( inst->has_dest( true ) && isSymReg( inst->dest ) && Symreg[inst->dest.num()]->getSpill() ) {

			// this needs to have valid contents
			assert( inst->dest_contents > DATA_CONTENTS__SPECIAL );

			// make sure it has stack storage 
			// NOTE: we currently assume that register input params do not have to get spilled, thus the assert()
			assert( inst->dest_contents->decl_location() == declNode::BLOCK );
			proc->alloc_stack_local( inst->dest_contents );

			// store it
			LirInst * store = Lir_Class::Store( LirVt::getVarType( inst->dest_contents ), 
				inst->dest, inst->dest_contents,
				Register::getRegFp(), DATA_CONTENTS_FRAMEP,
				inst->dest_contents->storage_location()._stack_offset, NULL );

			// put this instruction in the right place (after us)
			instruction_list_p insert = it;
			insert++;
			insts.insert( insert, store );
			if ( inst->block )
				inst->block->add_inst_after( store, inst );
			it++;
		}
	}

	// now flag everything as non-spilled, so we don't try to spill it again next time around
	for ( int i = 0; i < (int)Symreg.size(); ++i )
		if ( Symreg[i] )
			Symreg[i]->getSpill() = false;
}

inline bool briggs_reg_alloc::isSymReg( const Register & reg )
{
	// check out its number 
	assert( reg.num() >= 0 );
	return ( (int)reg.num() >= num_regs && (int)reg.num() < num_webs ) ? true : false;
}

void briggs_reg_alloc::allocate(procNode * _proc) 
{
	bool success, coalesce = false;
	proc = _proc;

	// find out how many registers we have
	num_regs = CBZ::ArchInfo.get_all_regs().size();
	usable_regs = CBZ::ArchInfo.get_regs_gpr().size() + CBZ::ArchInfo.get_regs_fpr().size();

	// do flow analysis on LIR to get loop info - this sets up _depth member of
	//	each block, which we'll use later.
	vector<LirBlockSet> loops;
	lir_flow_walker::find_loops( _proc, true, loops );

	sduList duChains;
	do {
		do {
			duChains.clear();
			makeDuChains(duChains);
			makeWebs(duChains);
			buildAdjMatrix();
			coalesce = coalesceRegisters();
		} while (coalesce);
	
		buildAdjLists();
		computeSpillCosts();
		pruneGraph();
		success = assignRegisters();
		if (success) {
			modifyCode();
		}
		else {
			genSpillCode();
		}
	} while (!success);

	// print out all of the instructions
#if 1
	cout << "Post-allocation instruction listing:" << endl;
	instruction_list::const_iterator it = _proc->instructions().begin();
	int num = 0;
	while ( it != _proc->instructions().end() )
		cout << num++ << " " << **(it++) << endl;
	cout << endl << endl;
#endif

}

RegAllocWalker::RegAllocWalker() : Walker( Preorder, NodeOnly )
{
}

void RegAllocWalker::at_unit(unitNode *the_unit, Order ord)
{
	// process all defs
	const def_list & defs = the_unit->defs();
	def_list::const_iterator it = defs.begin();
	while ( it != defs.end() )
		(*(it++))->walk( *this );
}

void RegAllocWalker::at_proc(procNode *the_proc, Order ord)
{
	// allocate regs for this guy
	briggs_reg_alloc ra;
	ra.allocate( the_proc );
}
