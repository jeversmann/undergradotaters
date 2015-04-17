// ----------------------
//     Chuck Tsen
// Started June 25th 2002
// 	match.cc
// Updated June 27th 2002
// ----------------------

// ********************************************************
// *							  *
// * This class performs simple pattern matching to	  *
// * generate the LIR "psuedo-RISC" linear instructions.  *
// * 							  *
// * It takes general information about the tree and the  *
// * operations trying to be performed and updates the	  *
// * STL GLOBAL List<LirInst> instList.		  *
// * 							  *
// * We must find a better way to encapsulate the list.	  *
// *							  *
// ********************************************************

#ifndef __MATCHER_H__
#define __MATCHER_H__

#include "c_breeze.h"
#include "instruction.h"
#include "struct_info.h"
#include "storage_alloc.h"


// structure containing info we need when generating instructions and such
struct lir_gen_info
{
	// set the proc we're currently in
	void set_proc( procNode * proc )
	{
		// setup these things
		_proc = proc;
		_alloc->_proc = proc;
		if ( _proc )
            _insts = &_proc->instructions();
		else
			_insts = &_unit->instructions();
	}

	// storage allocator
	storage_alloc * _alloc;

	// translation unit being processed
	unitNode * _unit;

	// procedure being processed
	procNode * _proc;

	// output instruction list 
	instruction_list * _insts;

	// structure information if needed
	Enc_Data * _structs;
};

// class that handles matching various MIR constructs and generating instructions for them
class Matcher{
public:

	// match various things
	static void match_cast(lir_gen_info & gen_info, binaryNode *the_assign);
	static void match_direct_asgmt(lir_gen_info & gen_info, binaryNode *the_assign);
	static void match_unop_asgmt(lir_gen_info & gen_info, binaryNode *the_assign);
	static void match_binop_asgmt(lir_gen_info & gen_info, binaryNode *the_assign);
	static void match_cond_branch(lir_gen_info & gen_info, Operator::OperatorID cmpOp, Node * pCmp1, 
		Node * pCmp2, string label);
	static void match_convert(lir_gen_info & gen_info, lir_var_type sourceType, Register sourceReg, 
		declNode * pDeclSrc, lir_var_type destType, Register& destReg, declNode * pDeclDest);
	static void match_declaration(lir_gen_info & gen_info, declNode * the_decl);
	static void match_arith(lir_gen_info & gen_info, binaryNode* the_binary, Register& dest, 
		declNode * pDeclDest, Register source1, declNode * pDeclSrc1, reg_or_const source2, declNode * pDeclSrc2);
	static void match_goto(lir_gen_info & gen_info, gotoNode* the_goto);
	static void match_label(lir_gen_info & gen_info, labelNode* the_label);
	static void match_simple_load(lir_gen_info & gen_info, Node * thing_to_load, Register& dest_reg, 
		declNode *& pDeclDest, lir_var_type *var);
	static void match_simple_store(lir_gen_info & gen_info, Node * store_target, Register store_value_reg, 
		declNode *& pDeclStoreValue, lir_var_type type);
	static void match_move(lir_gen_info & gen_info, Register src_reg, declNode * pDeclSrc, Register dest_reg, 
		declNode * pDeclDest);
	static void match_unary(lir_gen_info & gen_info, unaryNode* the_unary, Register& dest_reg, 
		declNode * pDeclDest, Register source, declNode * pDeclSrc);
	static void match_call(lir_gen_info & gen_info, callNode* the_call);
	static void match_return(lir_gen_info & gen_info, returnNode *the_return);

private:

	// helper functions for converting the contents of a register to a new type
	static void convert_helper(lir_gen_info & gen_info, typeNode * targetType, 
		Register & reg, declNode *& decl, lir_var_type vtSrc, lir_var_type vtDest);

	// load something into a register, including doing all structure/pointer/etc. management required
	static bool load_thing(lir_gen_info & gen_info, Node* thing_to_load, Register& dest_register, 
		declNode ** dest_decl = NULL, lir_var_type * type_loaded = NULL);
	static bool load_thing(lir_gen_info & gen_info, Node* thing_to_load, reg_or_const& result, 
		mnemonic targetInstruction, declNode ** dest_decl = NULL, lir_var_type * type_loaded = NULL);

	// load a constant into a register, including doing whatever declaration of static data that might be required
	static void load_constant(lir_gen_info & gen_info, Register & dest_register, declNode *dest_decl, 
		constNode * pConst, lir_var_type & type_loaded);

	// store a register into something, including doing all structure/pointer/etc. management required
	// allowFixupLastInstruction: if true, and if the result of store_thing ends up being a simple 
	//	register-to-register move, this method will try to change the last generated instruction's 
	//	destination to the right register instead of moving from value_reg to the target register.
	static bool store_thing(lir_gen_info & gen_info, bool allowFixupLastInstruction, Node* thing_to_store, 
		Register value_reg, declNode * value_decl, lir_var_type value_type);

	// try to change the given instruction so that it writes its result into the given register
	static bool patch_instruction_for_store(LirInst * pInst, Register desired_dest, 
		declNode * pDesiredDestDecl, Register expected_dest, declNode * pExpectedDestDecl);

	// get the address of some thing into a register
	static bool get_thing_address(lir_gen_info & gen_info, Node* thing, Register& dest_register, 
		declNode *& pDeclDest);

	// find the type of some node, if it is not already defined
	static typeNode * get_thing_type( Node * thing );

	// get the instruction mnemonic for a particular operator
	static mnemonic get_inst_for_operator(Operator::OperatorID id);

};

#endif	//__MATCHER_H__
