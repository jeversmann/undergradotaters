// ----------------------
//     Chuck Tsen
// Started June 25th 2002
//   code_gen_phase.h
// Updated June 27th 2002
// ----------------------

// ********************************************************
// *							  *
// * This walker should generate straight line code in	  *
// * the form of a linked list as it traverses the tree.  *
// * I'm starting out simple, trying to spit out psuedo	  *
// * RISC assembly code for simple arithmetic operations  *
// * 							  *
// * Next step is to perform coercion (automatic type	  *
// * conversion) for operations where the type does not   *
// * match.						  *
// *							  *
// ********************************************************

#ifndef __LIR_GEN_WALKER_H
#define __LIR_GEN_WALKER_H

#include "c_breeze.h"
#include "instruction.h"
#include "match.h"
#include "struct_info.h"
#include "arch_info.h"
#include "storage_alloc.h"

class lir_gen_walker : public Walker
{
public:

	// constructor
	lir_gen_walker();

	// destructor
	~lir_gen_walker();


private:
////////////////////////////
// data members

	// thing we use to pass around stuff that is needed when generating LIR instructions
	lir_gen_info _info;

	// used to maintain information about structures
	Enc_Data _struct_data;
	
	// storage allocator for initial LIR generation phase
	storage_alloc _alloc;

	// used to turn off at_decl() if in sue_spec
	bool _suespec_visit;

	// flag that is set by if statements and reset at goto statements
	// whose function is to eliminate superfluous jmp statements from the nested gotoNode
	// under an ifNode.  (because at_if takes care of work that at_goto normally wants to do)
	int _skip_next_goto;

	// flag indicating that we are processing a procedure call
	bool _is_call;

	// keeps us from reading extra decls
	bool _ignore_decl;

	// the return type of the last function call
	lir_var_type _call_res_type;

	// a statement that was evaluating a conditional - we expect that this will 
	//	immeditaely precede an 'if' statement
	binaryNode * _condition_stmt;

public:
////////////////////////////
// helper methods

	// find out if the operation is pointer indirection or address-of
	static bool is_indir_or_deref(unaryNode *the_unary);

	// get a string for the given type
	static string type_string(typeNode *the_type);

public:
////////////////////////////
// walker interface

	void at_unit(unitNode * the_unit, Order ord);
	void at_block(blockNode *block, Order ord);
	void at_proc(procNode * the_proc, Order ord);
	void at_decl(declNode *the_decl, Order ord);
	void at_binary(binaryNode *the_binary, Order ord );
	void at_goto(gotoNode *the_goto, Order ord);
	void at_label(labelNode *the_label, Order ord);
	void at_if(ifNode *the_if, Order ord);
	void at_call(callNode *the_call, Order ord);
	void at_return(returnNode *the_return, Walker::Order ord);

};

// helper walker for gathering info on structures
class suespec_walker : public Walker
{
public:
	// ctor
	suespec_walker( Enc_Data & encData );

	// so we can track namespace
	void		at_block( blockNode * block, Order ord );

	// our sole purpose in life - to walk suespec declarations
	void		at_suespec( suespecNode * suespec, Order ord );

private:

	// the thing we're updating
	Enc_Data &	_encData;

};

// helper walker for setting up pointer arithmetic
class ptr_arith_walker : public Walker
{
public:
	// ctor
	ptr_arith_walker();

	// to keep track of what block/statement we're at
	void		at_block(blockNode *block, Order ord);
	void		at_stmt(stmtNode *block, Order ord);

	// we only care about addition and subtraction
	void		at_binary(binaryNode *binary, Order ord);

private:

	// current statement
	stmtNode * _stmt;

	// stack of blocks
	list<blockNode*> _blocks;
};

// helper walker for handling args to a function call
class call_arg_walker : public Walker
{
public:
	// ctor - sets up default decl and register
	call_arg_walker( lir_gen_info & info, declNode * temp_var_decl, Register temp_var_reg );

	// walker fns
	void 		at_expr(exprNode *the_expr, Order ord);
	void 		at_unary(unaryNode *the_unary, Order ord);
	void 		at_binary(binaryNode *the_binary, Order ord );

	// find out where our results went
	void		get_results( declNode *& arg_decl, Register & arg_reg );

private:

	// lir generation info
	lir_gen_info & _info;

	// where the argument went
	declNode *	_arg_decl;
	Register	_arg_reg;

};

#endif //  __LIR_GEN_WALKER_H

