// ------------- 
//  Chuck Tsen
// June 24, 2002
//  Lir_Class.h
// -------------
//
// ********************************************************
// *							  *
// * This is the instruction set class that describes the *
// * psuedo risc instructions as described in the writeup *
// * Lir_Class by Chuck Tsen.  This idea was adapted from a	  *
// * similar concept implemented by the cmcc compiler, as *
// * described in the paper "Code-Reuse in an Optimizing  *
// * compiler."  					  *
// *							  *
// ********************************************************

#ifndef __LIR_CLASS_H__
#define __LIR_CLASS_H__

#include "instruction.h"
using namespace std;

class Lir_Class
{
public:

	// simple no-op instruction
	static LirInst* NOP();

	// handle type conversions
	static LirInst* ConvertType(lir_var_type vtSource, Register source, declNode* pDeclSource, lir_var_type vtDest, Register &dest, declNode* pDeclDest);

	// handle load/store/move/etc
	static LirInst* Load(lir_var_type typeToLoad, Register &dest_register, declNode* pDeclDest, Register base_reg, declNode* pDeclBase, constant offset, declNode* pDeclOffset);
	static LirInst* LoadImmediate(lir_var_type typeToLoad, Register &dest_register, declNode* pDeclDest, constant con);
	static LirInst* Store(lir_var_type typeToStore, Register source_register, declNode* pDeclSrc, Register base_reg, declNode* pDeclBase, constant offset, declNode* pDeclOffset);
	static LirInst* GetEffectiveAddress(Register &dest_register, declNode* pDeclDest, Register base_reg, declNode* pDeclBase, constant offset, declNode* pDeclOffset);
	static LirInst* Move(Register dest, declNode* pDeclDest, Register source, declNode* pDeclSrc);
	static LirInst* LoadStatic(Register& dest, declNode* pDeclDest, string staticLabel, declNode * declStatic);

	// handle arithmetic
	static LirInst* Add(lir_var_type vtDest, Register &dest_reg, declNode* pDeclDest, Register opnd1, declNode* pDeclOp1, reg_or_const opnd2, declNode* pDeclOp2);
	static LirInst* Sub(lir_var_type vtDest, Register &dest_reg, declNode* pDeclDest, Register opnd1, declNode* pDeclOp1, reg_or_const opnd2, declNode* pDeclOp2);
	static LirInst* Mul(lir_var_type vtDest, Register &dest_reg, declNode* pDeclDest, Register opnd1, declNode* pDeclOp1, reg_or_const opnd2, declNode* pDeclOp2);
	static LirInst* Div(lir_var_type vtDest, Register &dest_reg, declNode* pDeclDest, Register opnd1, declNode* pDeclOp1, reg_or_const opnd2, declNode* pDeclOp2);
	static LirInst* Mod(lir_var_type vtDest, Register &dest_reg, declNode* pDeclDest, Register opnd1, declNode* pDeclOp1, reg_or_const opnd2, declNode* pDeclOp2);
	static LirInst* Neg(lir_var_type vtDest, Register &dest_reg, declNode* pDeclDest, Register opnd1, declNode* pDeclOp1);

	// boolean operations
	static LirInst* BitwiseOR			(lir_var_type vtDest, Register &dest_reg, declNode* pDeclDest, Register opnd1, declNode* pDeclOp1, reg_or_const opnd2, declNode* pDeclOp2);
	static LirInst* BitwiseAND			(lir_var_type vtDest, Register &dest_reg, declNode* pDeclDest, Register opnd1, declNode* pDeclOp1, reg_or_const opnd2, declNode* pDeclOp2);
	static LirInst* BitwiseXOR			(lir_var_type vtDest, Register &dest_reg, declNode* pDeclDest, Register opnd1, declNode* pDeclOp1, reg_or_const opnd2, declNode* pDeclOp2);
	static LirInst* BitwiseNOT			(lir_var_type vtDest, Register &dest_reg, declNode* pDeclDest, Register opnd1, declNode* pDeclOp1);
	static LirInst* BitwiseShiftLeft	(lir_var_type vtDest, Register &dest_reg, declNode* pDeclDest, Register opnd1, declNode* pDeclOp1, reg_or_const bitCnt, declNode* pDeclBitCnt);
	static LirInst* BitwiseShiftRight	(lir_var_type vtDest, Register &dest_reg, declNode* pDeclDest, Register opnd1, declNode* pDeclOp1, reg_or_const bitCnt, declNode* pDeclBitCnt);	
	static LirInst* BitwiseRotateRight	(lir_var_type vtDest, Register &dest_reg, declNode* pDeclDest, Register opnd1, declNode* pDeclOp1, reg_or_const bitCnt, declNode* pDeclBitCnt);
	static LirInst* BitwiseRotateLeft	(lir_var_type vtDest, Register &dest_reg, declNode* pDeclDest, Register opnd1, declNode* pDeclOp1, reg_or_const bitCnt, declNode* pDeclBitCnt);

	// control flow stuff
	static LirInst* Compare(Register opnd1, declNode* pDeclOp1, reg_or_const opnd2, declNode* pDeclOp2);
	static LirInst* Branch(Compare_type cmp_type, string label);
	static LirInst* Jmp(string label);
	static LirInst* Call(callNode* the_call);
	static LirInst* Return(procNode* current_proc);
	static LirInst* Label(string label);

	// static data stuff
	static LirInst* StaticDataString(string str);
	static LirInst* StaticDataBinary(lir_var_type vt, char * bytes, int byteCount);

	// other stuff
	static LirInst* DeclareLocal(declNode* the_decl);
	static LirInst* BeginProc(procNode* the_proc);
	static LirInst* EndProc(procNode* the_proc);
	static LirInst* BeginUnit(unitNode* the_unit);
	static LirInst* EndUnit(unitNode* the_unit);

private:

	// do a simple 2-operand instruction (add, subtract, bitwise stuff, etc.)
	static LirInst* make_simple(mnemonic inst, lir_var_type vtDest, Register &dest_reg, declNode* pDeclDest, Register opnd1, declNode* pDeclOp1, reg_or_const opnd2, declNode* pDeclOp2);

};
#endif	//__LIR_CLASS_H__
