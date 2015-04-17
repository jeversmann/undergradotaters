// ----------------------------------------------------------------------
//
//  C-Breeze
//  C Compiler Framework
// 
//  Copyright (c) 2000 University of Texas at Austin
// 
//  Samuel Z. Guyer
//  Daniel A. Jimenez
//  Calvin Lin
//  Paul Navratil
//  Charles Nevill
// 
//  Permission is hereby granted, free of charge, to any person
//  obtaining a copy of this software and associated documentation
//  files (the "Software"), to deal in the Software without
//  restriction, including without limitation the rights to use, copy,
//  modify, merge, publish, distribute, sublicense, and/or sell copies
//  of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//  
//  The above copyright notice and this permission notice shall be
//  included in all copies or substantial portions of the Software.
//  
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
//  NONINFRINGEMENT.  IN NO EVENT SHALL THE UNIVERSITY OF TEXAS AT
//  AUSTIN BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
//  IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
//  OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//
//  We acknowledge the C-to-C Translator from MIT Laboratory for
//  Computer Science for inspiring parts of the C-Breeze design.
//
// ----------------------------------------------------------------------

#include "c_breeze.h"
#include "lir_flow_walker.h"
#include "LIR.h"


lir_flow_walker::lir_flow_walker() : 
	Walker ( Both, Subtree )
{
}

lir_flow_walker::~lir_flow_walker(){
}

void lir_flow_walker::build_lir_blocks(procNode * the_proc)
{
	// the list of blocks we're building
	LirBlockList & blocks = the_proc->lir_blocks();
	blocks.clear();

	// a map from label name to the block which that lable heads
	typedef map<string, LirBlock*> HeaderMap;
	HeaderMap headerMap;

	// build the actual blocks
	int next = 1;
	const instruction_list & insts = the_proc->instructions();
	instruction_list::const_iterator instIt = insts.begin();
	LirBlock * block = new LirBlock( next++ );
	bool nextNew = false;
	for ( ; instIt != insts.end(); ++instIt ) 
	{
		LirInst * inst = *instIt;

		// are we starting a new block? (happens at label and after jump/branch)
		mnemonic mn = inst->instruction;
		if ( mn == mn_Label || nextNew )
		{
			// add the old block to the list and make a new one
			if ( block->insts().size() > 0 ) {
				blocks.push_back( block );
				LirBlock * tempblock = new LirBlock( next++ );
				block->_next = tempblock;
				block = tempblock;
			}

			// remember that this label heads this block
			headerMap[ inst->target ] = block;
		}

		// reset this
		nextNew = false;

		// which instruction is this?
		switch ( mn )
		{
		case mn_BranchEQ:
		case mn_BranchNE:
		case mn_BranchLT:
		case mn_BranchLE:
		case mn_BranchGT:
		case mn_BranchGE:
		case mn_Jmp:

			// a new block next time around
			nextNew = true;
			break;

		default:

			// nothing special
			break;
		}

		// add this instruction to the block
		block->add_inst( inst );
	}

	// add the last one
	if ( block->insts().size() > 0 )
		blocks.push_back( block );

	// build succs/preds arrays
	LirBlock *s1, *s2;
	HeaderMap::iterator findIt;
	LirBlockList_p b;
	for ( b = blocks.begin(); b != blocks.end(); ++b ) {
		LirBlock* block = *b;

		// assume no successors
		s1 = NULL;
		s2 = NULL;

		// get last instruction in this block
		LirInst * last = block->insts().back();
		switch ( last->instruction )
		{
		case mn_Jmp:

			// find jump target
			s1 = headerMap[ last->target ];
			assert( s1 );
			break;

		case mn_BranchEQ:
		case mn_BranchNE:
		case mn_BranchLT:
		case mn_BranchLE:
		case mn_BranchGT:
		case mn_BranchGE:

			// find branch target and fall-through case
			s1 = headerMap[ last->target ];
			assert( s1 );
			s2 = block->_next;
			break;

		case mn_Return:

			// no successors
			break;

		default:

			// only target is successor block
			s1 = block->_next;
			break;
		}

		// setup linkages between target and source
		if ( s1 ) {
			block->_succs.insert( s1 );
			s1->_preds.insert( block );
		}
		if ( s2 ) {
			block->_succs.insert( s2 );
			s2->_preds.insert( block );
		}
	}

}

void lir_flow_walker::get_block_dfo(LirBlock * block, LirBlockList & blocks) 
{
	// skip if invalid block or already visited
	if ( ! block || block->_visited )
		return;

	// we have visted this now
	block->_visited = true;

	// process all successors first
	LirBlockSet_p it;
	for ( it = block->_succs.begin(); it != block->_succs.end(); ++it )
		get_block_dfo( *it, blocks );

	// finally put us into the ordering
	blocks.push_back( block );
}

void lir_flow_walker::find_block_doms(procNode * the_proc) 
{
	LirBlockList & blocks = the_proc->lir_blocks();
	LirBlockList_p it;

	//
	// this is basically a transcription of the code from Muchnick ACDI,
	// page 182, figure 7.14
	//

	// any to do at all?
	if ( blocks.size() < 1 )
		return;

	// generate a depth-first ordering on the blocks
	LirBlockList dfoBlocks;
	LirBlock * root = *(blocks.begin());
	root->reset_visited();
	get_block_dfo( root, dfoBlocks );

	// build set of all blocks
	LirBlockSet allBlocks;
	it = dfoBlocks.begin();
	while ( it != dfoBlocks.end() )
		allBlocks.insert( *(it++) );

	// set all blocks dominated by everything to start with
	it = blocks.begin();
	while ( it != blocks.end() )
		(*it++)->_doms = allBlocks;

	// root dominates itself only
	root->_doms.clear();
	root->_doms.insert( root );

	// take root out of allBlocks to get depth-first ordered list minus root
	LirBlockSet allNoRoot = allBlocks - root;

	// figure out dominance relationship
	bool changed;
	LirBlockSet t, d;
	do
	{
		// no change yet
		changed = false;

		// process all except root, in depth-first order
		LirBlockSet_p itn;
		for ( itn = allNoRoot.begin(); itn != allNoRoot.end(); ++itn )
		{
			LirBlock * n = *itn;

			// init this set to all blocks
			t = allBlocks;

			// modify according to this guy's predecessors
			LirBlockSet_p pred;
			for ( pred = n->_preds.begin(); pred != n->_preds.end(); ++pred )
				t &= (*pred)->_doms;

			// did the set change?
			d = t | n;
			if ( d != n->_doms )
			{
				n->_doms = d;
				changed = true;
			}
		}
	} 
	while ( changed );
}

void lir_flow_walker::find_loops( procNode * proc, bool need_recompute_flow, 
	vector<LirBlockSet> & loops ) 
{
	// do we have to recompute things?
	if ( need_recompute_flow )
	{
		build_lir_blocks( proc );
		find_block_doms( proc );
		debug_print_block_info( proc );
	}

	// reset loop depth and generate list of back edges
	edge_list backedges;
	LirBlockList & blocks = proc->lir_blocks();
	LirBlockList_p bl = blocks.begin();
	for ( ; bl != blocks.end(); ++bl )
	{
		LirBlock * block = *bl;

		// reset depth to zero
		block->_depth = 0;

		// look at successors other than next sequential instruction
		LirBlockSet & succs = block->_succs;
		LirBlockSet::iterator suc = succs.begin();
		for ( ; suc != succs.end(); ++suc )
		{
			LirBlock * succ = *suc;

			// ignore immediate successor
			if ( succ == block->_next )
				continue;

			// does successor dominate us?
			if ( block->_doms.find( succ ) != block->_doms.end() )
			{
				// add a backedge
				edge e = { block, succ };
				backedges.push_back( e );
			}
		}
	}

	// use muchnick's algorithm from ACDI, figure 7.21 to find loops
	loops.clear();
	edge_list_p edg = backedges.begin();
	LirBlock *p, *q;
	for ( ; edg != backedges.end(); ++edg )
	{
		LirBlockList stack;

		edge & e = *edg;
		LirBlock *m = e.source, *n = e.target;

		// add a new loop
		int sz = loops.size() + 1;
		loops.resize( sz );
		LirBlockSet & loop = loops[sz-1];
		loop.insert( m );
		loop.insert( n );

		// setup stack and run till we're done
		if ( m != n )
			stack.push_back( m );
		while ( stack.size() > 0 )
		{
			// get end of stack
			p = stack.back();
			stack.pop_back();

			// iterate over preds of p
			LirBlockSet_p pr = p->_preds.begin();
			for ( ; pr != p->_preds.end(); ++pr )
			{
				q = *pr;
				if ( loop.find( q ) == loop.end() )
				{
					loop.insert( q );
					stack.push_back( q );
				}
			}
		}
	}

	// setup loop depth of blocks
	vector<LirBlockSet>::iterator lp = loops.begin();
	for ( ; lp != loops.end(); ++lp )
	{
		// increment depth of each block
		LirBlockSet & loop = *lp;
		LirBlockSet_p bl = loop.begin();
		for ( ; bl != loop.end(); ++bl )
			(*bl)->_depth ++;
	}

}

void 
lir_flow_walker::at_proc(procNode * the_proc, Order ord) 
{
	if ( ord != Preorder )
		return;

	// build basic blocks and find dominators
	build_lir_blocks(the_proc);
	find_block_doms(the_proc);

	// print this out
	debug_print_block_info(the_proc);
}



void 
lir_flow_walker::computeRegisterLiveness( instruction_list & insts, inst_to_reg_id_map & liveRegs )
{
	// generate flow info for instructions
	getInstructionFlowInfo( insts );

	// mark all not visited, and clear set of live regs for each instruction
	instruction_list_p it = insts.begin();
	for ( ; it != insts.end(); ++it )
	{
		LirInst * inst = *it;
		inst->visited = false;
		liveRegs[ inst ].clear();
	}

	// generate postorder ordering of instructions
	instruction_list rpo;
	getInstructionsPostorder( insts, insts.begin(), rpo );

	// for debugging - trace out instructions in postorder
	// change 0 to 1 to add this code
#if 0
	{
		cout << "DEBUG:" << endl;
		cout << "Postorder:" << endl;
		instruction_list_p it = rpo.begin();
		for ( ; it != rpo.end(); ++it )
			cout << **it << endl;
		cout << "END DEBUG" << endl;
	}
#endif

	// do iterative dataflow analysis to find live registers at each instruction
	instruction_list worklist = rpo;
	unsigned int idFp = CBZ::ArchInfo.get_reg_fp()->_id;
	unsigned int idSp = CBZ::ArchInfo.get_reg_sp()->_id;
	unsigned int idRvFixed = (unsigned int)-1;
	if ( CBZ::ArchInfo.get_reg_retval_fixed() )
		idRvFixed = CBZ::ArchInfo.get_reg_retval_fixed()->_id;
	unsigned int idRvFloat = (unsigned int)-1;
	if ( CBZ::ArchInfo.get_reg_retval_float() )
		idRvFloat = CBZ::ArchInfo.get_reg_retval_float()->_id;
	while ( worklist.size() != 0 )
	{
		// get front of worklist
		LirInst * inst = worklist.front();
		worklist.pop_front();

		// what is used by this?
		reg_id_set used;
		if ( inst->has_opnd1() )
			used.insert( inst->opnd1.num() );
		if ( inst->has_opnd2() )
			used.insert( inst->opnd2._reg.num() );
		if ( inst->has_base() )
			used.insert( inst->memBase.num() );
		if ( inst->has_offset() )
			used.insert( inst->memOffset._reg.num() );

		// compute our successors' info
		reg_id_set succLive;
		instruction_set_p sit = inst->succs.begin();
		for ( ; sit != inst->succs.end(); ++sit )
			succLive |= liveRegs[ *sit ];

		// what do we generate/kill?
		reg_id_set killRegs;
		if ( inst->has_dest() )
		{
			// we kill destination register
			killRegs |= inst->dest.num();
		}

		// new liveness for us
		reg_id_set live = used | (succLive - killRegs);

		// we never care about liveness for stack ptr, frame ptr, or return value registers
		//	because these registers are never saved by us.
		live -= idFp;
		live -= idSp;
		live -= idRvFixed;
		live -= idRvFloat;

		// changed?
		if ( liveRegs[ inst ] != live )
		{
			// new liveness for us
			liveRegs[ inst ] = live;

			// our predecessors have changed
			instruction_set_p pit = inst->preds.begin();
			for ( ; pit != inst->preds.end(); ++pit )
				if ( ! cbz_util::list_find( worklist, *pit ) )
					worklist.push_back( *pit );
		}
	}
}

void 
lir_flow_walker::getInstructionsPostorder( instruction_list & insts, instruction_list_p it, instruction_list & ordering )
{
	LirInst * inst = *it;

	// already in list?
	if ( inst->visited )
		return;

	// set visited
	inst->visited = true;

	// add all successors first
	switch ( inst->instruction )
	{
	case mn_Return:

		// no successors
		break;

	case mn_Jmp:

		// just has the one target
		assert( inst->targetInst );
		if ( ! inst->targetInst->visited )
			getInstructionsPostorder( insts, findInstruction( insts, inst->targetInst ), ordering );
		break;

	case mn_BranchEQ:
	case mn_BranchNE:
	case mn_BranchLT:
	case mn_BranchLE:
	case mn_BranchGT:
	case mn_BranchGE:

		// has a branch target
		assert( inst->targetInst );
		if ( ! inst->targetInst->visited )
			getInstructionsPostorder( insts, findInstruction( insts, inst->targetInst ), ordering );

		// also has not-taken - fall through for this

	default:

		// add next instruction at least
		assert( it != insts.end() );
		instruction_list_p next = it;
		next++;
		getInstructionsPostorder( insts, next, ordering );
		break;
	}

	// add us to ordering
	ordering.push_back( inst );
}

void 
lir_flow_walker::getInstructionFlowInfo( instruction_list & insts ) 
{
	instruction_list_p it;

	// make forward pass to setup list of branch targets
	typedef map<string, LirInst*> target_map;
	target_map targets;
	for ( it = insts.begin(); it != insts.end(); ++it )
	{
		LirInst * inst = *it;

		// clear out successors/predecessors arrays
		inst->succs.clear();
		inst->preds.clear();

		// label?
		if ( inst->instruction == mn_Label )
			targets[ inst->target ] = inst;
	}

	// setup successors/predecessors
	LirInst *s1, *s2;
	for ( it = insts.begin(); it != insts.end(); ++it )
	{
		LirInst * inst = *it;

		// assume no successors
		s1 = NULL;
		s2 = NULL;

		switch ( inst->instruction )
		{
		case mn_Return:
		case mn_EndProc:

			// no successors, pred is setup by previous instruction
			break;

		case mn_Jmp:
			{
			// just has the one successor
			inst->targetInst = targets[ inst->target ];
			assert( inst->targetInst );
			s1 = inst->targetInst;
			break;
			}
		case mn_BranchEQ:
		case mn_BranchNE:
		case mn_BranchLT:
		case mn_BranchLE:
		case mn_BranchGT:
		case mn_BranchGE:
			{
			// has a branch target
			inst->targetInst = targets[ inst->target ];
			assert( inst->targetInst );
			s2 = inst->targetInst;

			// also has not-taken - fall through for this
			}
		default:

			// next instruction is successor
			instruction_list_p next = it;
			++next;
			assert( next != insts.end() );
			s1 = *next;
			break;
		}

		// setup linkages between target and source
		if ( s1 ) {
			inst->succs.insert( s1 );
			s1->preds.insert( inst );
		}
		if ( s2 ) {
			inst->succs.insert( s2 );
			s2->preds.insert( inst );
		}
	}
}

instruction_list_p 
lir_flow_walker::findInstruction( instruction_list & insts, const LirInst * inst ) 
{
	instruction_list_p it = insts.begin();
	for ( ; it != insts.end(); ++it )
		if ( *it == inst )
			return it;
	return it;
}

void 
lir_flow_walker::debug_print_block_info( procNode * proc ) 
{
#if 0
	// print out info
	LirBlockList & blocks = proc->lir_blocks();
	LirBlockList_p it = blocks.begin();
	int inst = 0;
	while ( it != blocks.end() )
	{
		printf( "\n" );
		printf( "\n" );
		printf( "block %d (instructions %d through %d) ", 
			(*it)->_number, inst, inst + (*it)->insts().size() - 1 );
		printf( "\n" );
		printf( "dominated by: " );
		LirBlockSet_p doms = (*it)->_doms.begin();
		while ( doms != (*it)->_doms.end() )
			printf( "%d ", (*doms++)->_number );
		printf( "\n" );
		printf( "predecessors: " );
		LirBlockSet_p preds = (*it)->_preds.begin();
		while ( preds != (*it)->_preds.end() )
			printf( "%d ", (*preds++)->_number );
		printf( "\n" );
		printf( "successors: " );
		LirBlockSet_p succs = (*it)->_succs.begin();
		while ( succs != (*it)->_succs.end() )
			printf( "%d ", (*succs++)->_number );
		printf( "\n" );
		inst += (*it)->insts().size();
		it++;
	}
#endif
}

