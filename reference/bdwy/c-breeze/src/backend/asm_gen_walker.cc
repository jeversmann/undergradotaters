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
#include "asm_gen_walker.h"
#include "lir_flow_walker.h"
#include "LIR.h"
#include "hash_set_ex.h"


asm_gen_walker::asm_gen_walker() : Walker( Both, NodeOnly )
{
	_unit = NULL;
	_pOutFile = NULL;
}

asm_gen_walker::~asm_gen_walker() 
{
}

void 
asm_gen_walker::at_unit( unitNode * the_unit, Order ord ) 
{
	if ( ord == Preorder )
	{
		// save this
		_unit = the_unit;

		// open output file
		string file = _unit->output_file();
		_pOutFile = fopen( file.c_str(), "wt" );
		if ( ! _pOutFile )
		{
			fprintf( stderr, "Can't open output file '%s' for writing.", file.c_str() );
			assert( false );
		}

		// write a header
		fprintf( _pOutFile, ".file \"%s\"\n\n", _unit->input_file().c_str() );

		// throw out a begin unit instruction
		LirInst * begin = Lir_Class::BeginUnit( _unit );
		emit( begin );
		delete begin;

		// add all other unit instructions
		emit( _unit->instructions() );

		// process all defs
		const def_list & defs = _unit->defs();
		def_list::const_iterator it = defs.begin();
		while ( it != defs.end() )
			(*(it++))->walk( *this );
	}
	else
	{
		// throw out an end unit instruction
		LirInst * end = Lir_Class::EndUnit( _unit );
		emit( end );
		delete end;

		// close the output file
		fprintf( _pOutFile, "\n\n" );
		fclose( _pOutFile );
		_pOutFile = NULL;

		// no unit anymore
		_unit = NULL;
	}
}

void 
asm_gen_walker::at_proc( procNode * the_proc, Order ord ) 
{
	// ignore second pass
	if ( ord != Preorder )
	{
		_proc = NULL;
		return;
	}

	// save this
	_proc = the_proc;

#define CHECK_REG( reg ) \
do { \
	if ( ! CBZ::ArchInfo.get_reg_by_index( (reg).num(), dummy ) ) \
		CBZFAIL(("Instruction '%s' uses virtual register.\n" \
			"Did you forget to specify a register allocation phase or -noreg on the command line?", inst->to_string().c_str())); \
} while (0)

	// sanity check - make sure there are no vitual registers in here
	instruction_list & insts = the_proc->instructions();
	instruction_list_p it = insts.begin();
	for ( ; it != insts.end(); ++it )
	{
		LirInst * inst = *it;
		Register dummy;
		if ( inst->has_opnd1() )
			CHECK_REG( inst->opnd1 );
		if ( inst->has_opnd2() )
			CHECK_REG( inst->opnd2._reg );
		if ( inst->has_base() )		
			CHECK_REG( inst->memBase );
		if ( inst->has_offset() )
			CHECK_REG( inst->memOffset._reg );
		if ( inst->has_dest() )
			CHECK_REG( inst->dest );
	}

#undef CHECK_REG

	// add code for handling callee-save registers
	doCalleeSave();

	// add code for caller-save registers
	doCallerSave();

	// finally, compute the total size of the stack frame
	computeStackFrameSize();

	// emit all instructions
	emit( insts );

}

void 
asm_gen_walker::computeStackFrameSize() 
{
	assert( _proc );

	// decide how big we think the stack frame has to be.
	//	this is the sum of any extra space specified in the
	//	arch spec, plus the size of stack locals, plus space
	//	for arguments to called procedures.

	// start with the size of locals
	unsigned int stackSize = _proc->get_locals_size();

	// find out max size of function arguments
	unsigned int procArgSize = 0;
	instruction_list & insts = _proc->instructions();
	instruction_list_p it = insts.begin();
	for ( ; it != insts.end(); ++it )
	{
		LirInst & inst = **it;
		if ( inst.instruction == mn_Call )
		{
			// we need the call node here
			assert( inst.nodeExtra && inst.nodeExtra->typ() == Call );
			callNode * pCall = (callNode*)inst.nodeExtra;

			// see how many bytes this call takes
			if ( procArgSize < pCall->stack_arg_bytes() )
				procArgSize = pCall->stack_arg_bytes();
		}
	}

	// add in size of function args and bottom buffer and compute the total size
	stackSize += procArgSize;
	stackSize += CBZ::ArchInfo.get_stack_extra_bottom();
	if ( stackSize < CBZ::ArchInfo.get_stack_frame_min_size() )
		stackSize = CBZ::ArchInfo.get_stack_frame_min_size();

	// align the size as necessary
	unsigned int align = CBZ::ArchInfo.get_stack_align();
	stackSize = ((stackSize + align - 1) / align) * align;

	// save this in the proc for later use
	_proc->stack_frame_size( stackSize );
}

void 
asm_gen_walker::doCalleeSave() 
{
	assert( _proc );
	instruction_list & insts = _proc->instructions();

	// what does callee save?
	const arch_info::register_info_list & calleeSaveRegs = CBZ::ArchInfo.get_regs_callee_save();
	if ( calleeSaveRegs.size() == 0 )
		return;

	typedef hash_set_ex<unsigned int> regSet;

	// make a set of this.
	regSet calleeSaveRegSet;
	arch_info::register_info_list::const_iterator csrit = calleeSaveRegs.begin();
	while ( csrit != calleeSaveRegs.end() )
		calleeSaveRegSet.insert( (*(csrit++))->_id );

	// make a single forward pass over instructions to see which registers
	//	are used and to make a list of all return instructions that need to 
	//	get restore code added
	vector<instruction_list_p> returns;
	regSet changedRegSet;
	instruction_list_p it = insts.begin();
	for ( ; it != insts.end(); ++it )
	{
		LirInst & inst = **it;

		// if it's a return we'll need to patch it later
		if ( inst.instruction == mn_Return )
			returns.push_back( it );

		// if it has a destination add that register to changed registers
		if ( inst.has_dest() )
			changedRegSet.insert( inst.dest.num() );

		// also add in any kill registers for this instruction
		arch_info::register_info_list killRegs;
		CBZ::ArchInfo.get_instruction_kill_regs( &inst, killRegs );
		arch_info::register_info_list_p kit = killRegs.begin();
		for ( ; kit != killRegs.end(); ++kit )
			changedRegSet.insert( (*kit)->_id );
	}

	// intersect these sets
	changedRegSet &= calleeSaveRegSet;

	// did we change anything?  if not we have no work to do
	if ( changedRegSet.size() == 0 )
		return;

	// find begin proc - should be beginning
	instruction_list_p firstRealInst = insts.begin();
	while ( (*firstRealInst)->instruction != mn_BeginProc && firstRealInst != insts.end() )
		firstRealInst++;
	assert( firstRealInst != insts.end() );
	LirInst * pBeginProc = *firstRealInst;
	firstRealInst++;

	// add save code after begin proc, and restore code before each return
	regSet::iterator toSave = changedRegSet.begin();
	for ( ; toSave != changedRegSet.end(); ++toSave )
	{
		// allocate a temporary to save this register
		Register reg;
		lir_var_type vt;
		declNode * decl;
		createTempForRegSave( *toSave, reg, vt, decl );

		// make instruction to store this register into the stack
		LirInst * pStore = Lir_Class::Store( vt, reg, decl, Register::getRegFp(), DATA_CONTENTS_FRAMEP, 
			decl->storage_location()._stack_offset, NULL );

		// add this after the previous thing we added, before the first real instruction
		insts.insert( firstRealInst, pStore );

		// add instructions to restore it before each return
		vector<instruction_list_p>::iterator rtIt = returns.begin();
		for ( ; rtIt != returns.end(); ++rtIt )
		{
			// make the load instruction
			LirInst * pLoad = Lir_Class::Load( vt, reg, decl, Register::getRegFp(), DATA_CONTENTS_FRAMEP, 
				decl->storage_location()._stack_offset, NULL );

			// add this before the return instruction
			insts.insert( *rtIt, pLoad );
			assert( (**rtIt)->instruction == mn_Return );
		}
	}

}

void 
asm_gen_walker::doCallerSave() 
{
	assert( _proc );
	instruction_list & insts = _proc->instructions();

	// make set of caller-save regs
	reg_id_set caller_save;
	const arch_info::register_info_list & csave = CBZ::ArchInfo.get_regs_caller_save();
	arch_info::register_info_list::const_iterator cs = csave.begin();
	for ( ; cs != csave.end(); ++cs )
		caller_save |= (*cs)->_id;

	// compute liveness
	inst_to_reg_id_map liveRegs;
	lir_flow_walker::computeRegisterLiveness( insts, liveRegs );

	// for debugging - trace out liveness for each instruction
#if 0
	{
		cout << "DEBUG:" << endl;
		cout << "Liveness:" << endl;
		instruction_list_p it = insts.begin();
		for ( ; it != insts.end(); ++it )
		{
			cout << "    Live: ";
			reg_id_set_p live = liveRegs[*it].begin();
			for ( ; live != found->second.end(); ++live )
			{
				arch_info::register_info * info = CBZ::ArchInfo.get_all_regs()[ *live ];
				cout << info->_name << " ";
			}
			cout << endl;
			cout << "Inst: " << **it << endl;
		}
		cout << "END DEBUG" << endl;
	}
#endif

	// now do save/load for each call instruction - start by finding mn_CallPre instructions
	instruction_list_p it = insts.begin();
	Register regRetFixed = Register::getRegRetValFixed();
	Register regRetFloat = Register::getRegRetValFloat();
	for ( ; it != insts.end(); ++it )
	{
		// grab this instruction
		LirInst * inst = *it;

		// skip to the next pre-call instruction
		if ( inst->instruction != mn_CallPre )
			continue;

		// any live caller-save regs?
        reg_id_set live = liveRegs[ inst ];
		live &= caller_save;
		if ( live.size() == 0 )
			continue;

		// get an iterator pointing right after sequencer
		instruction_list_p sequence = it;
		sequence++;

		// find the actual call
		LirInst * call = NULL;
		while ( it != insts.end() && (*it)->instruction != mn_Call )
			it++;
		assert( it != insts.end() );
		call = *it;
		instruction_list_p postCall = it;
		postCall++;
		assert( postCall != insts.end() );

		// decide on the expected return value register
		lir_var_type vt = LirVt::getVarType( call->nodeExtra );
		Register * pRegRet = NULL;
		if ( LirVt::isTypeFloat( vt ) )
			pRegRet = &regRetFloat;
		else if ( LirVt::isTypeFixed( vt ) )
			pRegRet = &regRetFixed;

		// save anything that's live here
		reg_id_set_p toSave = live.begin();
		for ( ; toSave != live.end(); ++toSave )
		{
			// we never save return value register
			assert( *toSave != pRegRet->num() );

			// allocate a temporary to save this register
			Register reg;
			lir_var_type vt;
			declNode * decl;
			createTempForRegSave( *toSave, reg, vt, decl );

			// make instruction to store this register into the stack
			LirInst * pStore = Lir_Class::Store( vt, reg, decl, Register::getRegFp(), DATA_CONTENTS_FRAMEP, 
				decl->storage_location()._stack_offset, NULL );

			// add this right after the sequencing instruction
			insts.insert( sequence, pStore );

			// make the load instruction to reload this from the stack
			LirInst * pLoad = Lir_Class::Load( vt, reg, decl, Register::getRegFp(), DATA_CONTENTS_FRAMEP, 
				decl->storage_location()._stack_offset, NULL );

			// add this after the call instruction
			insts.insert( postCall, pLoad );
		}
	}


}
	
void 
asm_gen_walker::createTempForRegSave( unsigned int id, Register & reg, lir_var_type & vt, declNode *& decl )
{
	// get the register
	CBZ::ArchInfo.get_reg_by_index( id, reg );
	assert( reg.type() == reg_gpr || reg.type() == reg_fpr );
	bool isgpr = (reg.type() == reg_gpr);
	vt = isgpr ? CBZ::ArchInfo.get_reg_data_type_gpr() : CBZ::ArchInfo.get_reg_data_type_fpr();

	// make a temp decl and allocate stack storage
	decl = new declNode( reg.to_string().c_str(), declNode::AUTO, NULL, NULL, NULL );
	_proc->alloc_stack_local( decl, CBZ::ArchInfo.get_data_size( vt ) );
}

void 
asm_gen_walker::emit( const LirInst * inst ) 
{
	assert( _pOutFile );
	if ( ! _pOutFile )
		return;
	if ( ! inst->should_emit() )
		return;

	// generate code
	vector<string> code;
	if ( ! CBZ::ArchInfo.get_code_for_instruction( inst, code ) )
		CBZFAIL(("Can't emit instruction '%s'  because there is no code template for "
			"such an instruction in the architecture spec.", inst->to_string().c_str()));

	// emit each line to the file
	int i, sz = (int)code.size();
	for ( i = 0; i < sz; ++i )
		fprintf( _pOutFile, "%s\n", code[i].c_str() );
}

void 
asm_gen_walker::emit( const instruction_list & insts ) 
{
	assert( _pOutFile );
	if ( ! _pOutFile )
		return;

	// do each one
	instruction_list::const_iterator it = insts.begin();
	for ( ; it != insts.end(); ++it )
		emit( *it );
}

