// ------------- 
//  Chuck Tsen
// June 24, 2002
//  Lir_Class.cc
// -------------
//
// ********************************************************
// *							  *
// * this is the LirInst* set class that describes the *
// * psuedo risc instructions as described in the writeup *
// * Lir_Class by Chuck Tsen.  thisInst idea was adapted from a *
// * similar concept implemented by the cmcc compiler, as *
// * described in the paper "Code-Reuse in an Optimizing  *
// * compiler."  					  *
// *							  *
// ********************************************************

#include "c_breeze.h"
#include "LIR.h"

LirInst* Lir_Class::ConvertType(lir_var_type vtSource, Register source, declNode* pDeclSrc, lir_var_type vtDest, Register& dest, declNode* pDeclDest) 
{
	dest.setType( vtDest );

	// new instruction
	LirInst* inst = new LirInst( mn_ConvertType );
	inst->opnd1 = source;
	inst->opnd1_contents = pDeclSrc;
	inst->dest = dest;
	inst->dest_contents = pDeclDest;
	inst->vtPrimary = vtSource;
	inst->vtConvert = vtDest;

	return inst;
}

LirInst* Lir_Class::Load(lir_var_type typeToLoad, Register& dest, declNode* pDeclDest, Register base_reg, declNode* pDeclBase, constant offset, declNode* pDeclOffset) 
{
	dest.setType( typeToLoad );

	// new instruction
	LirInst* inst = new LirInst( mn_Load );
	inst->dest = dest;
	inst->dest_contents = pDeclDest;
	inst->memBase = base_reg;
	inst->memBase_contents = pDeclBase;
	inst->memOffset = offset;
	inst->memOffset_contents = pDeclOffset;
	inst->vtPrimary = typeToLoad;

	return inst;
}

LirInst* Lir_Class::LoadImmediate(lir_var_type typeToLoad, Register& dest, declNode* pDeclDest, constant value) 
{
	dest.setType( typeToLoad );

	// new instruction
	LirInst* inst = new LirInst( mn_LoadImmediate );
	inst->opnd2 = value;
	inst->dest = dest;
	inst->dest_contents = pDeclDest;

	return inst;
}

LirInst* Lir_Class::Store(lir_var_type typeToStore, Register source, declNode* pDeclSrc, Register base_reg, declNode* pDeclBase, constant offset, declNode* pDeclOffset) 
{
	// new instruction
	LirInst* inst = new LirInst( mn_Store );
	inst->opnd1 = source;
	inst->opnd1_contents = pDeclSrc;
	inst->memBase = base_reg;
	inst->memBase_contents = pDeclBase;
	inst->memOffset = offset;
	inst->memOffset_contents = pDeclOffset;
	inst->vtPrimary = typeToStore;

	return inst;
}

LirInst* Lir_Class::GetEffectiveAddress(Register& dest, declNode* pDeclDest, Register base_reg, declNode* pDeclBase, constant offset, declNode* pDeclOffset) 
{
	dest.makeGPR();

	// new instruction
	LirInst* inst = new LirInst( mn_GetEffectiveAddr );
	inst->dest = dest;
	inst->dest_contents = pDeclDest;
	inst->memBase = base_reg;
	inst->memBase_contents = pDeclBase;
	inst->memOffset = offset;
	inst->memOffset_contents = pDeclOffset;
	inst->vtPrimary = vt_pointer;

	return inst;
}

LirInst* Lir_Class::Move(Register dest, declNode* pDeclDest, Register source, declNode* pDeclSrc)
{
	// new instruction
	LirInst* inst = new LirInst( mn_Move );
	inst->dest = dest;
	inst->dest_contents = pDeclDest;
	inst->opnd1 = source;
	inst->opnd1_contents = pDeclSrc;

	return inst;
}

LirInst* Lir_Class::LoadStatic(Register& dest, declNode* pDeclDest, string staticLabel, declNode * declStatic)
{
	dest.setType( LirVt::getVarType( declStatic ) );

	// new instruction
	LirInst* inst = new LirInst( mn_LoadStatic );
	inst->dest = dest;
	inst->dest_contents = pDeclDest;
	inst->target = staticLabel;
	inst->vtPrimary = LirVt::getVarType( declStatic );
	inst->nodeExtra = declStatic;

	return inst;
}

LirInst* Lir_Class::Add(lir_var_type vtDest, Register &dest, declNode* pDeclDest, Register opnd1, declNode* pDeclOp1, reg_or_const opnd2, declNode* pDeclOp2) 
{
	return make_simple(mn_Add, vtDest, dest, pDeclDest, opnd1, pDeclOp1, opnd2, pDeclOp2);
}

LirInst* Lir_Class::Sub(lir_var_type vtDest, Register &dest, declNode* pDeclDest, Register opnd1, declNode* pDeclOp1, reg_or_const opnd2, declNode* pDeclOp2) 
{
	return make_simple(mn_Sub, vtDest, dest, pDeclDest, opnd1, pDeclOp1, opnd2, pDeclOp2);
}

LirInst* Lir_Class::Mul(lir_var_type vtDest, Register &dest, declNode* pDeclDest, Register opnd1, declNode* pDeclOp1, reg_or_const opnd2, declNode* pDeclOp2) 
{
	return make_simple(mn_Mul, vtDest, dest, pDeclDest, opnd1, pDeclOp1, opnd2, pDeclOp2);
}

LirInst* Lir_Class::Div(lir_var_type vtDest, Register &dest, declNode* pDeclDest, Register opnd1, declNode* pDeclOp1, reg_or_const opnd2, declNode* pDeclOp2) 
{
	return make_simple(mn_Div, vtDest, dest, pDeclDest, opnd1, pDeclOp1, opnd2, pDeclOp2);
}

LirInst* Lir_Class::Mod(lir_var_type vtDest, Register &dest, declNode* pDeclDest, Register opnd1, declNode* pDeclOp1, reg_or_const opnd2, declNode* pDeclOp2) 
{
	assert( vtDest != vt_float && vtDest != vt_double );
	return make_simple(mn_Mod, vtDest, dest, pDeclDest, opnd1, pDeclOp1, opnd2, pDeclOp2);
}

LirInst* Lir_Class::Neg(lir_var_type vtDest, Register &dest, declNode* pDeclDest, Register opnd1, declNode* pDeclOp1) 
{
	// result depends on operand type
	dest.setType( vtDest );

	// new instruction
	LirInst* inst = new LirInst( mn_Neg );
	inst->opnd1 = opnd1;
	inst->opnd1_contents = pDeclOp1;
	inst->dest = dest;
	inst->dest_contents = pDeclDest;

	return inst;
}

LirInst* Lir_Class::BitwiseOR(lir_var_type vtDest, Register &dest, declNode* pDeclDest, Register opnd1, declNode* pDeclOp1, reg_or_const opnd2, declNode* pDeclOp2) 
{
	assert( vtDest != vt_float && vtDest != vt_double );
	return make_simple(mn_BitwiseOR, vtDest, dest, pDeclDest, opnd1, pDeclOp1, opnd2, pDeclOp2);
}

LirInst* Lir_Class::BitwiseAND(lir_var_type vtDest, Register &dest, declNode* pDeclDest, Register opnd1, declNode* pDeclOp1, reg_or_const opnd2, declNode* pDeclOp2) 
{
	assert( vtDest != vt_float && vtDest != vt_double );
	return make_simple(mn_BitwiseAND, vtDest, dest, pDeclDest, opnd1, pDeclOp1, opnd2, pDeclOp2);
}

LirInst* Lir_Class::BitwiseXOR(lir_var_type vtDest, Register &dest, declNode* pDeclDest, Register opnd1, declNode* pDeclOp1, reg_or_const opnd2, declNode* pDeclOp2) 
{
	assert( vtDest != vt_float && vtDest != vt_double );
	return make_simple(mn_BitwiseXOR, vtDest, dest, pDeclDest, opnd1, pDeclOp1, opnd2, pDeclOp2);
}

LirInst* Lir_Class::BitwiseNOT(lir_var_type vtDest, Register &dest, declNode* pDeclDest, Register opnd1, declNode* pDeclOp1) 
{
	assert( vtDest != vt_float && vtDest != vt_double );

	// result has to be integer
	dest.makeGPR();

	// new instruction
	LirInst* inst = new LirInst( mn_BitwiseNOT );
	inst->opnd1 = opnd1;
	inst->opnd1_contents = pDeclOp1;
	inst->dest = dest;
	inst->dest_contents = pDeclDest;

	return inst;
}

LirInst* Lir_Class::BitwiseShiftLeft(lir_var_type vtDest, Register &dest, declNode* pDeclDest, Register opnd1, declNode* pDeclOp1, reg_or_const bitCnt, declNode* pDeclBitCnt) 
{
	assert( vtDest != vt_float && vtDest != vt_double );
	return make_simple(mn_BitwiseShiftLeft, vtDest, dest, pDeclDest, opnd1, pDeclOp1, bitCnt, pDeclBitCnt);
}

LirInst* Lir_Class::BitwiseShiftRight(lir_var_type vtDest, Register &dest, declNode* pDeclDest, Register opnd1, declNode* pDeclOp1, reg_or_const bitCnt, declNode* pDeclBitCnt) 
{
	assert( vtDest != vt_float && vtDest != vt_double );
	return make_simple(mn_BitwiseShiftRight, vtDest, dest, pDeclDest, opnd1, pDeclOp1, bitCnt, pDeclBitCnt);
}	

LirInst* Lir_Class::BitwiseRotateRight(lir_var_type vtDest, Register &dest, declNode* pDeclDest, Register opnd1, declNode* pDeclOp1, reg_or_const bitCnt, declNode* pDeclBitCnt) 
{
	assert( vtDest != vt_float && vtDest != vt_double );
	return make_simple(mn_BitwiseRotateRight, vtDest, dest, pDeclDest, opnd1, pDeclOp1, bitCnt, pDeclBitCnt);
}

LirInst* Lir_Class::BitwiseRotateLeft(lir_var_type vtDest, Register &dest, declNode* pDeclDest, Register opnd1, declNode* pDeclOp1, reg_or_const bitCnt, declNode* pDeclBitCnt) 
{
	assert( vtDest != vt_float && vtDest != vt_double );
	return make_simple(mn_BitwiseRotateLeft, vtDest, dest, pDeclDest, opnd1, pDeclOp1, bitCnt, pDeclBitCnt);
}

LirInst* Lir_Class::Compare(Register opnd1, declNode* pDeclOp1, reg_or_const opnd2, declNode* pDeclOp2)
{
	// new instruction
	LirInst* inst = new LirInst( mn_Compare );
	inst->opnd1 = opnd1;
	inst->opnd1_contents = pDeclOp1;
	inst->opnd2 = opnd2;
	inst->opnd2_contents = pDeclOp2;

	return inst;
}

LirInst* Lir_Class::Branch(Compare_type cmp_type, string label) 
{
	// new instruction
	LirInst* inst = new LirInst;
	inst->target = label;

	// pick branch type
	switch ( cmp_type )
	{
	case cmp_EQ: inst->instruction = mn_BranchEQ; break;
	case cmp_NE: inst->instruction = mn_BranchNE; break;
	case cmp_LT: inst->instruction = mn_BranchLT; break;
	case cmp_LE: inst->instruction = mn_BranchLE; break;
	case cmp_GT: inst->instruction = mn_BranchGT; break;
	case cmp_GE: inst->instruction = mn_BranchGE; break;
	}

	return inst;
}

LirInst* Lir_Class::Jmp(string label) 
{
	// new instruction
	LirInst* inst = new LirInst( mn_Jmp );
	inst->target = label;

	return inst;
}

LirInst* Lir_Class::Call(callNode* the_call) 
{
	// new instruction
	LirInst* inst = new LirInst( mn_Call );
	inst->target = ((idNode*)the_call->name())->name();
	inst->nodeExtra = the_call;

	// figure out destination register
	lir_var_type vt = LirVt::getVarType( the_call );

	// is it a fixed type?
	if ( LirVt::isTypeFixed( vt ) )
	{
		const arch_info::register_info * info = CBZ::ArchInfo.get_reg_retval_fixed();
		if ( ! info )
			CBZFAIL(("Call to function %s cannot be handled because it requires a return value "
				"register type that is not available.", ((idNode*)the_call->name())->name().c_str()));

		// save destination type
		inst->dest = *info;
		inst->dest_contents = DATA_CONTENTS_RETVAL;
	}

	// is it a float type?
	if ( LirVt::isTypeFloat( vt ) )
	{
		const arch_info::register_info * info = CBZ::ArchInfo.get_reg_retval_float();
		if ( ! info )
			CBZFAIL(("Call to function %s cannot be handled because it requires a return value "
				"register type that is not available.", ((idNode*)the_call->name())->name().c_str()));

		// save destination type
		inst->dest = *info;
		inst->dest_contents = DATA_CONTENTS_RETVAL;
	}

	return inst;
}

LirInst* Lir_Class::Return(procNode* current_proc)
{
	// new instruction
	LirInst* inst = new LirInst( mn_Return );
	inst->nodeExtra = current_proc;

	return inst;
}

LirInst* Lir_Class::Label(string label) 
{
	// new instruction
	LirInst* inst = new LirInst( mn_Label );
	inst->target = label;

	return inst;
}

LirInst* Lir_Class::StaticDataString(string str)
{
	// fixup escape chars
	constant c( str.c_str() );
	str = c.to_string();

	// remove quotes that were added by to_string
	str.erase( 0, 1 );
	str.erase( str.size() - 1, 1 );

	// new instruction
	LirInst* inst = new LirInst( mn_StaticDataString );
	inst->dataString = str;

	return inst;
}

LirInst* Lir_Class::StaticDataBinary(lir_var_type vt, char * bytes, int byteCount)
{
	assert( byteCount <= MAX_LIR_DATA_BYTES );

	// new instruction
	LirInst* inst = new LirInst( mn_StaticDataBinary );

	// setup the actual data
	memcpy( inst->dataBinary, bytes, byteCount );
	memset( inst->dataBinary + byteCount, 0, MAX_LIR_DATA_BYTES - byteCount );

	return inst;
}

LirInst* Lir_Class::DeclareLocal(declNode * the_decl) 
{
	// new instruction
	LirInst* inst = new LirInst( mn_DeclLocal );
	inst->target = the_decl->name();
	inst->nodeExtra = the_decl;

	return inst;
}

LirInst* Lir_Class::BeginProc(procNode* the_proc) 
{
	// new instruction
	LirInst* inst = new LirInst( mn_BeginProc );
	inst->target = the_proc->decl()->name();
	inst->nodeExtra = the_proc;

	return inst;
}

LirInst* Lir_Class::EndProc(procNode* the_proc) 
{
	// new instruction
	LirInst* inst = new LirInst( mn_EndProc );
	inst->target = the_proc->decl()->name();
	inst->nodeExtra = the_proc;

	return inst;
}

LirInst* Lir_Class::BeginUnit(unitNode* the_unit)
{
	// new instruction
	LirInst* inst = new LirInst( mn_BeginUnit );

	return inst;
}

LirInst* Lir_Class::EndUnit(unitNode* the_unit)
{
	// new instruction
	LirInst* inst = new LirInst( mn_EndUnit );

	return inst;
}

LirInst* Lir_Class::make_simple(mnemonic instruction, lir_var_type vtDest, Register &dest, declNode* pDeclDest, Register opnd1, declNode* pDeclOp1, reg_or_const opnd2, declNode* pDeclOp2)
{
	dest.setType( vtDest );

	// new instruction
	LirInst* inst = new LirInst( instruction );
	inst->opnd1 = opnd1;
	inst->opnd1_contents = pDeclOp1;
	inst->opnd2 = opnd2;
	inst->opnd2_contents = pDeclOp2;
	inst->dest = dest;
	inst->dest_contents = pDeclDest;
	inst->vtPrimary = vtDest;

	return inst;
}

