// ----------------------
//     Chuck Tsen
// Started June 25th 2002
//   match.cc
// Updated June 27th 2002
// ----------------------

// ********************************************************
// *							  *
// * This class performs simple pattern matching to	  *
// * generate the LIR "psuedo-RISC" linear instructions.  *
// * 							  *
// * It takes general information about the tree and the  *
// * operations trying to be performed and returns a	  *
// * pointer to an instruction.  (LirInst *), which	  *
// * can be used to update the list of instructions with  *
// * instList.push_back(<fcn call>)			  *
// *							  *
// ********************************************************

#include "c_breeze.h"
#include "instruction.h"
#include "match.h"
#include "LIR.h"
#include "operators.h"

#define ALLOC (gen_info._alloc)
#define INSTS (gen_info._insts)
#define STRUCTS (gen_info._structs)

// because I'm lazy
typedef declNode::Storage_location Storage_location;

void Matcher::match_cast(lir_gen_info & gen_info, binaryNode *the_assign)
{
	castNode *the_cast = (castNode *)the_assign->right();

	LirInst* before = INSTS->back();

	// load the thing to be cast
	lir_var_type typeLoaded = vt_unknown;
	Register tempReg;
	declNode * pTempDecl = NULL;
	if ( ! load_thing( gen_info, the_cast->expr(), tempReg, &pTempDecl, &typeLoaded ) )
		return;

	// what to convert to?
	lir_var_type typeDesired = LirVt::getVarType( the_cast );

	// convert types?
	if ( typeLoaded != typeDesired )
	{
		// convert it
		convert_helper( gen_info, the_cast->type(), tempReg, pTempDecl, typeLoaded, typeDesired );
	}

	// store the result
	// NOTE: only allow fixup if we actually added an instruction here
	store_thing( gen_info, (INSTS->back() != before), the_assign->left(), tempReg, pTempDecl, typeLoaded );
}

// FOR DIRECT ASSIGNMENTS
void Matcher::match_direct_asgmt(lir_gen_info & gen_info, binaryNode *the_assign)
{
	Node * pRhsThing = the_assign->right();

	LirInst* before = INSTS->back();

	// load the thing to be stored
	lir_var_type typeLoaded = vt_unknown;
	Register tempReg;
	declNode * pTempDecl = NULL;
	if ( ! load_thing( gen_info, pRhsThing, tempReg, &pTempDecl, &typeLoaded ) )
		return;

	// store it
	// NOTE: only allow fixup if we actually added an instruction here
	store_thing( gen_info, (INSTS->back() != before), the_assign->left(), tempReg, pTempDecl, typeLoaded );
}

// FOR SINGLE OPERAND ASSIGNMENTS (FOR UNARY OPERATORS)
void Matcher::match_unop_asgmt(lir_gen_info & gen_info, binaryNode *the_assign)
{
	unaryNode * pUnOp = (unaryNode *)the_assign->right();

	// load the thing to be operated on
	lir_var_type typeLoaded = vt_unknown;
	Register opndReg;
	declNode * pTempDecl = NULL;
	if ( ! load_thing( gen_info, pUnOp->expr(), opndReg, &pTempDecl, &typeLoaded ) )
		return;

	LirInst* before = INSTS->back();

	// do the operator
	Register resultReg;
	declNode * pTempDecl2 = NULL;
	ALLOC->get_temp_var( get_thing_type( the_assign->left() ), resultReg, pTempDecl2 );
	match_unary( gen_info, pUnOp, resultReg, pTempDecl2, opndReg, pTempDecl );

	// store the result
	store_thing( gen_info, (INSTS->back() != before), the_assign->left(), 
		resultReg, pTempDecl2, LirVt::getVarType( pUnOp ) );
}


// FOR TWO OPERAND ASSIGNMENTS
void Matcher::match_binop_asgmt(lir_gen_info & gen_info, binaryNode *the_assign)
{
	binaryNode * pBinOp = (binaryNode *)the_assign->right();
	mnemonic op = get_inst_for_operator( (Operator::OperatorID)pBinOp->op()->id() );

	// get types for each side
	lir_var_type lhsType = LirVt::getVarType( pBinOp->left() );
	lir_var_type rhsType = LirVt::getVarType( pBinOp->right() );
	lir_var_type vtTemp;

	// load operand 1
	Register lhsReg;
	declNode * pLhsDecl = NULL;
	vtTemp = vt_unknown;
	if ( ! load_thing( gen_info, pBinOp->left(), lhsReg, &pLhsDecl, &vtTemp ) )
		return;
	assert( vtTemp == lhsType );

	// load operand 2 (try to avoid reload if same as operand 1)
	reg_or_const rhsReg;
	declNode * pRhsDecl = NULL;
	if ( pBinOp->left()->typ() == Id && pBinOp->right()->typ() == Id &&
		((idNode*)pBinOp->left())->decl() == ((idNode*)pBinOp->right())->decl() )
	{
		// just copy over - they are the same
		rhsReg = lhsReg;
		pRhsDecl = pLhsDecl;
		rhsType = lhsType;
	}
	else
	{
		// actually load this thing
		vtTemp = vt_unknown;
		if ( ! load_thing( gen_info, pBinOp->right(), rhsReg, op, &pRhsDecl, &vtTemp ) )
			return;
		assert( vtTemp == rhsType );
	}

	// conversion if necessary
	if (lhsType > rhsType && CBZ::ArchInfo.types_need_conversion( rhsType, lhsType ))
	{
		// force rhs into a register so we can convert it, if not already in one
		if ( rhsReg._is_const )
		{
			load_thing( gen_info, pBinOp->right(), rhsReg._reg, &pRhsDecl, &rhsType );
			rhsReg._is_const = false;
		}

		// convert the values
		convert_helper( gen_info, pBinOp->left()->type(), rhsReg._reg, pRhsDecl, rhsType, lhsType );
	}
	else if (rhsType > lhsType && CBZ::ArchInfo.types_need_conversion( lhsType, rhsType ))
	{
		// convert lhs to rhs type
		convert_helper( gen_info, pBinOp->right()->type(), lhsReg, pLhsDecl, lhsType, rhsType );
	}

	LirInst* before = INSTS->back();

	// do the actual operation
	Register result;
	declNode * pDeclRes = NULL;
	ALLOC->get_temp_var( get_thing_type( the_assign->left() ), result, pDeclRes );
	match_arith( gen_info, pBinOp, result, pDeclRes, lhsReg, pLhsDecl, rhsReg, pRhsDecl );

	// store the result
	store_thing( gen_info, (INSTS->back() != before), the_assign->left(),
		result, pDeclRes, LirVt::getVarType( pDeclRes ) );
}

// this function matches conditional branches for if statements
void Matcher::match_cond_branch(lir_gen_info & gen_info, Operator::OperatorID cmpOp, 
	Node * pCmp1, Node * pCmp2, string label)
{
	// load thing 1
	declNode * pDeclCmp1 = NULL, * pDeclCmp2 = NULL;
	Register cmp1;
	reg_or_const cmp2;
	lir_var_type vt1 = vt_unknown, vt2 = vt_unknown;
	if ( ! load_thing( gen_info, pCmp1, cmp1, &pDeclCmp1, &vt1 ) )
		return;

	// do we have a thing 2?
	if ( pCmp2 )
	{
		// is it a constant?
		if ( pCmp2->typ() == Const &&
			CBZ::ArchInfo.instruction_supports_immediate( get_inst_for_operator( cmpOp ), 
				LirVt::getVarType( pCmp2 ), ((constNode*)pCmp2)->value() ) )
		{
			// handle it as an immediate
            cmp2 = ((constNode*)pCmp2)->value();
		}
		else
		{
			// try to load thing 2
			cmp2._is_const = false;
			if ( ! load_thing( gen_info, pCmp2, cmp2._reg, &pDeclCmp2, &vt2 ) )
				return;

			// check compare types and convert if necessary
			assert( vt1 != vt_unknown && vt2 != vt_unknown );
			if ( vt1 != vt2 )
			{
				if ( vt1 < vt2 && CBZ::ArchInfo.types_need_conversion( vt1, vt2 ) )
				{
					// convert 1 to 2
					convert_helper( gen_info, get_thing_type( pCmp2 ), cmp1, pDeclCmp1, vt1, vt2 );
				}
				else if ( vt1 > vt2 && CBZ::ArchInfo.types_need_conversion( vt2, vt1 ) )
				{
					// force rhs into a register so we can convert it, if not already in one
					if ( cmp2._is_const )
					{
						load_thing( gen_info, pCmp2, cmp2._reg, &pDeclCmp2 );
						cmp2._is_const = false;
					}

					// convert 2 to 1
					convert_helper( gen_info, get_thing_type( pCmp1 ), cmp2._reg, pDeclCmp2, vt2, vt1 );
				}
			}
		}
	}
	else
	{
		// comparison is against zero
		cmp2._is_const = true;
		cmp2._const.make_zero();
		cmpOp = Operator::NE;
	}

	// do the compare and then a branch
	INSTS->push_back( Lir_Class::Compare(cmp1, pDeclCmp1, cmp2, pDeclCmp2) );
	LirInst * pInst = NULL;
		 if (cmpOp == Operator::EQ) { pInst = Lir_Class::Branch(cmp_EQ, label); }
	else if (cmpOp == Operator::NE) { pInst = Lir_Class::Branch(cmp_NE, label); }
	else if (cmpOp == '<')          { pInst = Lir_Class::Branch(cmp_LT, label); }
	else if (cmpOp == Operator::LE) { pInst = Lir_Class::Branch(cmp_LE, label); }
	else if (cmpOp == '>')          { pInst = Lir_Class::Branch(cmp_GT, label); }
	else if (cmpOp == Operator::GE) { pInst = Lir_Class::Branch(cmp_GE, label); }
	else { assert( false ); pInst = NULL; }
	INSTS->push_back( pInst );
}

// this function matches binary assignments and performs automatic type casting, where there was none before.
void Matcher::match_convert(lir_gen_info & gen_info, lir_var_type sourceType, Register sourceReg, 
	declNode * pDeclSrc, lir_var_type destType, Register& destReg, declNode * pDeclDest)
{
	INSTS->push_back( Lir_Class::ConvertType(sourceType, sourceReg, pDeclSrc, destType, destReg, pDeclDest) );
}


void Matcher::match_declaration(lir_gen_info & gen_info, declNode * the_decl)
{
	// ignore typedefs and externs
	if ( the_decl->storage_class() == declNode::TYPEDEF || the_decl->storage_class() == declNode::EXTERN )
		return;

	// figure out what kind of thing it is
	switch ( the_decl->decl_location() )
	{
	case declNode::TOP:
	{
		// emit data for it
		switch ( the_decl->type()->typ() )
		{
		case Func:
			// no decls for functions - we don't care about them
			return;

		//case Prim:
		//	// emit a label for it
		//	INSTS->push_back( Lir_Class::Label( the_decl->name() ) );
		//
		//	// it had better point to something simple
		//	assert( the_decl->init()->typ() == Const );
		//	constant value = ((constNode*)the_decl->init())->value();
		//	char bytes[8];
		//	int count = 0;
		//	if ( value.basic() == basic_type::SChar )
		//
		//	break;

		case Ptr:
		{
			// is it a pointer to a character?
			if ( the_decl->type()->type()->typ() != Prim || 
					! (((primNode*)the_decl->type()->type())->basic() == basic_type::SChar) )
				CBZFAIL(("Don't understand top-level declaration of static pointer variable."));

			// emit a label for it
			INSTS->push_back( Lir_Class::Label( the_decl->name() ) );

			// get the string data and declare it
			assert( the_decl->init()->typ() == Const );
			string str = ((constNode*)the_decl->init())->value().Str();
			INSTS->push_back( Lir_Class::StaticDataString( str ) );

			break;
		}
		default:
			CBZFAIL(("Don't understand top-level declaration with node type %d", the_decl->typ()));
		}
	}
	case declNode::BLOCK:
	case declNode::FORMAL:
	{
		// do local declaration
		INSTS->push_back( Lir_Class::DeclareLocal( the_decl ) );
		break;
	}
	case declNode::SU:
	case declNode::ENUM:
	case declNode::UNKNOWN:
	case declNode::PROC:

		// ignore these things - they need no special treatment
		break;

	default:
		CBZFAIL(("Don't understand declaration with decl_location %d", the_decl->decl_location()));
	}
}


// matches a binary assignment statement and emits LIR
// thisComp is a ptr so that on comparison instructions code_gen_phase::cmp will be set 
// so that the subsequent ifNode will know the comparison type
void Matcher::match_arith(lir_gen_info & gen_info, binaryNode* the_binary, Register& dest, 
	declNode * pDeclDest, Register source1, declNode * pDeclSrc1, reg_or_const source2, declNode * pDeclSrc2 )
{
	// compute result type - should not be an aggregate or a function definition
	lir_var_type resType = LirVt::getVarType( pDeclDest );
	assert( resType != vt_unknown && resType != vt_aggr && resType != vt_func_decl );

	// these work for any type
	if (the_binary->op()->id() == '+')	{ INSTS->push_back( Lir_Class::Add(resType, dest, pDeclDest, source1, pDeclSrc1, source2, pDeclSrc2) ); return;}
	if (the_binary->op()->id() == '*')  { INSTS->push_back( Lir_Class::Mul(resType, dest, pDeclDest, source1, pDeclSrc1, source2, pDeclSrc2) ); return;}
	if (the_binary->op()->id() == '/')	{ INSTS->push_back( Lir_Class::Div(resType, dest, pDeclDest, source1, pDeclSrc1, source2, pDeclSrc2) ); return;}
	if (the_binary->op()->id() == '-')	{ INSTS->push_back( Lir_Class::Sub(resType, dest, pDeclDest, source1, pDeclSrc1, source2, pDeclSrc2) ); return;}

	// remaining operators are only for fixed-point data types
	assert( resType != vt_float && resType != vt_double );
	if (the_binary->op()->id() == '%')			{ INSTS->push_back( Lir_Class::Mod				(resType, dest, pDeclDest, source1, pDeclSrc1, source2, pDeclSrc2) ); return;}
	if (the_binary->op()->id() == '&')			{ INSTS->push_back( Lir_Class::BitwiseAND		(resType, dest, pDeclDest, source1, pDeclSrc1, source2, pDeclSrc2) ); return;}
	if (the_binary->op()->id() == '|')			{ INSTS->push_back( Lir_Class::BitwiseOR		(resType, dest, pDeclDest, source1, pDeclSrc1, source2, pDeclSrc2) ); return;}
	if (the_binary->op()->id() == '^')			{ INSTS->push_back( Lir_Class::BitwiseXOR		(resType, dest, pDeclDest, source1, pDeclSrc1, source2, pDeclSrc2) ); return;}
	if (the_binary->op()->id() == Operator::LS) { INSTS->push_back( Lir_Class::BitwiseShiftLeft	(resType, dest, pDeclDest, source1, pDeclSrc1, source2, pDeclSrc2) ); return;}
	if (the_binary->op()->id() == Operator::RS) { INSTS->push_back( Lir_Class::BitwiseShiftRight(resType, dest, pDeclDest, source1, pDeclSrc1, source2, pDeclSrc2) ); return;}

	// we have no idea what kind of operator this is
	assert( false ); 
}

void Matcher::match_goto(lir_gen_info & gen_info, gotoNode* the_goto)
{
	INSTS->push_back( Lir_Class::Jmp(the_goto->name()) );
}

void Matcher::match_label(lir_gen_info & gen_info, labelNode* the_label)
{
	INSTS->push_back( Lir_Class::Label(the_label->name()) );
}

void Matcher::match_unary(lir_gen_info & gen_info, unaryNode* the_unary, Register& dest_reg, 
	declNode * pDeclDest, Register source, declNode * pDeclSrc)
{
	// decide which operator it is
	switch (the_unary->op()->id())
	{
	case '~':

		// do a bitwise not on the thing
		INSTS->push_back( Lir_Class::BitwiseNOT(LirVt::getVarType(the_unary->expr()), dest_reg, 
			pDeclDest, source, pDeclSrc) );
		break;

	case Operator::UMINUS:

		// negate this bad boy
		INSTS->push_back( Lir_Class::Neg(LirVt::getVarType(the_unary->expr()), dest_reg,
			pDeclDest, source, pDeclSrc) );
		break;

	default:
		// we don't know what to do here
		cout << "unrecognized unary operator: " << the_unary->op()->id() << endl;
		assert( false );
		break;
	}

}

void Matcher::match_simple_load(lir_gen_info & gen_info, Node * thing_to_load, 
	Register& dest_reg, declNode *& pDeclDest, lir_var_type *var)
{
	// load this thing
	load_thing( gen_info, thing_to_load, dest_reg, &pDeclDest, var );
}

void Matcher::match_simple_store(lir_gen_info & gen_info, Node * store_target, 
	Register store_value_reg, declNode *& pDeclStoreValue, lir_var_type type) 
{
	// store this thing
	store_thing( gen_info, false, store_target, store_value_reg, pDeclStoreValue, type );
}

void Matcher::match_move(lir_gen_info & gen_info, Register src_reg, declNode * pDeclSrc, 
	Register dest_reg, declNode * pDeclDest) 
{
	// can we patch it?  if so, we don't need a move!
	if ( patch_instruction_for_store( INSTS->back(), dest_reg, pDeclDest, src_reg, pDeclSrc ) )
		return;

	// ok, move it then
	INSTS->push_back( Lir_Class::Move( dest_reg, pDeclDest, src_reg, pDeclSrc ) );
}

void Matcher::match_call(lir_gen_info & gen_info, callNode* the_call)
{
	// call the function (currently we don't support function pointers and such)
	assert( the_call->name()->typ() == Id );
	INSTS->push_back( Lir_Class::Call( the_call ) );
}

void Matcher::match_return(lir_gen_info & gen_info, returnNode *the_return)
{
	// are we returning anything?
	if ( the_return->expr() )
	{
		// had better be an id
		assert( the_return->expr()->typ() == Id );
		idNode * pId = (idNode*)the_return->expr();

		// load this thing
		Register reg;
		if ( ! Register::getRegRetVal( LirVt::getVarType( pId ), reg ) )
			return;
		lir_var_type whoCares;
		declNode * pDeclNode = DATA_CONTENTS_RETVAL;
		match_simple_load( gen_info, pId, reg, pDeclNode, &whoCares );
	}

	// return
	INSTS->push_back( Lir_Class::Return( gen_info._proc ) );
}

void Matcher::convert_helper(lir_gen_info & gen_info, typeNode * targetType, 
	Register & reg, declNode *& decl, lir_var_type vtSrc, lir_var_type vtDest)
{
	if ( ! CBZ::ArchInfo.types_need_conversion( vtSrc, vtDest ) )
		return;

	// convert src to dest
	Register temp;
	declNode * pTempDecl = NULL;
	ALLOC->get_temp_var( targetType, temp, pTempDecl );
	match_convert( gen_info, vtSrc, reg, decl, vtDest, temp, pTempDecl );
	reg = temp;
	decl = pTempDecl;
}

bool Matcher::load_thing(lir_gen_info & gen_info, Node* thing_to_load, Register& dest_register, 
	declNode ** dest_decl, lir_var_type * type_loaded)
{
	// load but don't allow constant result
	reg_or_const result;
	if ( ! load_thing( gen_info, thing_to_load, result, mn_undefined, dest_decl, type_loaded ) )
		return false;

	// result had better be in a register
	assert( ! result._is_const );
	dest_register = result._reg;
	return true;
}

bool Matcher::load_thing(lir_gen_info & gen_info, Node* thing_to_load, reg_or_const& result, 
	mnemonic targetInstruction, declNode ** dest_decl, lir_var_type * type_loaded )
{
	// to make things easier - always have a valid pointer here
	lir_var_type typeJunk;
	if ( ! type_loaded )
		type_loaded = &typeJunk;

	// to make things easier - always have a valid pointer here
	declNode * pDeclJunk = NULL;
	if ( ! dest_decl )
		dest_decl = &pDeclJunk;

	// what kind of thing is this?
	switch (thing_to_load->typ())
	{
	case Unary:
	{
		unaryNode * pOpNode = (unaryNode*)thing_to_load;
		Node * operand = pOpNode->expr();
		switch ( pOpNode->op()->id() )
		{
		case Operator::INDIR:
			{
				// result is a register
				result._is_const = false;
				Register & dest_register = result._reg;

				// load the thing we're indirecting
				Register tempReg;
				lir_var_type typeLoaded;
				declNode * pDeclPtr = NULL;
				if (! load_thing( gen_info, operand, tempReg, &pDeclPtr, &typeLoaded ))
					return false;
				assert( typeLoaded == vt_pointer );

				// now indirect it (load base+offset with base == pointer, offset == 0)
				*type_loaded = LirVt::getVarType( operand->type()->type() );
				if ( ! dest_register.isValid() )
					ALLOC->get_temp_var( operand->type()->type(), dest_register, *dest_decl );
				INSTS->push_back( Lir_Class::Load( *type_loaded, dest_register, *dest_decl, tempReg, pDeclPtr, 0, NULL ) );

				break;
			}
		case Operator::ADDRESS:
			{
				// result is a register
				result._is_const = false;
				Register & dest_register = result._reg;

				// just get this thing's address into a register
				*type_loaded = vt_pointer;
				if (! get_thing_address(gen_info, operand, dest_register, *dest_decl))
					return false;

				break;
			}
		default:
			CBZFAIL(( "load_thing: can't load thing with unary operator %d.", pOpNode->op()->id() ));
		}

		break;
	}
	case Binary:
	{
		binaryNode * pOpNode = (binaryNode*)thing_to_load;
		switch ( pOpNode->op()->id() )
		{
		case Operator::ARROW:
			{
				// result is a register
				result._is_const = false;
				Register & dest_register = result._reg;

				// get the value of the pointer into a register
				Register tempReg;
				declNode * pPtrDecl = NULL;
				lir_var_type vtPtr;
				if (! load_thing( gen_info, pOpNode->left(), tempReg, &pPtrDecl, &vtPtr ))
					return false;
				assert( vtPtr == vt_pointer );

				// find the name the structure being accessed
				assert( pOpNode->left()->typ() == Id );
				assert( pOpNode->left()->base_type(true)->typ() == Ptr );
				typeNode * pStructNode = pOpNode->left()->type()->type()->base_type(true);
				assert( pStructNode->typ() == Struct );
				string structName = ((sueNode*)pStructNode)->spec()->name();

				// find the name of the field being accessed
				assert( pOpNode->right()->typ() == Id );
				idNode * pIdNode = (idNode*)pOpNode->right();
				string fieldName = pIdNode->name();

				// does it have a decl?  if not, find the right one
				if ( ! pIdNode->decl() )
					pIdNode->decl( ((sueNode*)pStructNode)->spec()->find_field( fieldName ) );

				// get the offset of the thing on the right
				unsigned int offset = STRUCTS->find_struct( structName )->find_member( fieldName )->getOffset();

				// now load it (load base+offset with base == pointer, offset == field offset)
				*type_loaded = LirVt::getVarType( pIdNode );
				if ( ! dest_register.isValid() )
					ALLOC->get_temp_var( pIdNode->type(), dest_register, *dest_decl );
				INSTS->push_back( Lir_Class::Load( *type_loaded, dest_register, *dest_decl, tempReg, pPtrDecl, (int)offset, NULL ) );

				break;
			}
		default:
			CBZFAIL(( "load_thing: can't load thing with unary operator %d.", pOpNode->op()->id() ));
		}

		break;
	}
	case Id:
	{
		idNode * pIdNode = (idNode*)thing_to_load;
		*type_loaded = LirVt::getVarType( pIdNode );

		// result is a register
		result._is_const = false;
		Register & dest_register = result._reg;

		// is it a return value?
		if ( pIdNode->decl()->is_return_value() )
		{
			// it lives in a register - we already know this
			dest_register = (*type_loaded == vt_float || *type_loaded == vt_double) ? 
				Register::getRegRetValFloat() : Register::getRegRetValFixed();
			*dest_decl = DATA_CONTENTS_RETVAL;
			break;
		}

		// where does it live?
		Storage_location loc = pIdNode->decl()->storage_location();
		assert( loc._type != Storage_location::storageloc_unknown );
		if ( loc._type == Storage_location::storageloc_register )
		{
			// it is already in a register
			*dest_decl = pIdNode->decl();
			dest_register = loc._register;
		}
		else if ( loc._type == Storage_location::storageloc_stack )
		{
			// load it into a register
			if ( ! dest_register.isValid() )
				ALLOC->get_temp_var( pIdNode->type(), dest_register, *dest_decl );
			INSTS->push_back( Lir_Class::Load( LirVt::getVarType( pIdNode ), dest_register, *dest_decl, 
				Register::getRegFp(), DATA_CONTENTS_FRAMEP, loc._stack_offset, NULL ) );
		}

		break;
	}
	case Call:
	{
		// result is a register
		result._is_const = false;
		Register & dest_register = result._reg;

		// the result is in one of 2 registers - figure out which one and return that
		lir_var_type vt = LirVt::getVarType( thing_to_load );
		*type_loaded = vt;
		bool flt = (vt == vt_float || vt == vt_double);
		dest_register = flt ? Register::getRegRetValFloat() : Register::getRegRetValFixed();
		*dest_decl = DATA_CONTENTS_RETVAL;
		break;
	}
	case Const:
	{
		constNode * pConst = (constNode*)thing_to_load;

		// can it be supported as an immediate?
		if ( targetInstruction != mn_undefined && 
			CBZ::ArchInfo.instruction_supports_immediate( targetInstruction, *type_loaded, pConst->value() ) )
		{
			// do it as an immediate in the actual instruction that needs it
			*dest_decl = NULL;
			*type_loaded = LirVt::getVarType( pConst );
			result = pConst->value();
			break;
		}
		else
		{
			// we will have to load this into a register and use it from there
			result._is_const = false;
			Register & dest_register = result._reg;
			if ( ! dest_register.isValid() )
				ALLOC->get_temp_var( pConst->type(), dest_register, *dest_decl );
			load_constant( gen_info, dest_register, *dest_decl, pConst, *type_loaded );
		}

		break;
	}
	default:
		CBZFAIL(( "load_thing: can't load thing node type %d.", thing_to_load->typ() ));
	}

	return true;
}

void Matcher::load_constant(lir_gen_info & gen_info, Register & dest_register, declNode *dest_decl, 
	constNode * pConst, lir_var_type & type_loaded) 
{
	// setup load type
	type_loaded = LirVt::getVarType( pConst );
	const constant & con = pConst->value();
	assert( ! con.no_val() );

	// strings are always static data
	if ( con.is_str() )
	{
		string id = CBZ::get_temp_id_str();

		// declare a global variable for this string
		declNode * pDeclStr = new declNode( id.c_str(), declNode::STATIC, pConst->type(), pConst, NULL );
		gen_info._unit->defs().push_back( pDeclStr );

		// declare a string global data thingy that we can load
		instruction_list & instsGlobal = gen_info._unit->instructions();
		instsGlobal.push_back( Lir_Class::Label( id ) );
		instsGlobal.push_back( Lir_Class::StaticDataString( con.Str() ) );

		// load this into the register
		INSTS->push_back( Lir_Class::LoadStatic( dest_register, dest_decl, id, pDeclStr ) );
	}

	// can we handle it with a loadimmediate?
	else if ( CBZ::ArchInfo.instruction_supports_immediate( mn_LoadImmediate, type_loaded, con ) )
	{
		// load as an immediate and we're done
		INSTS->push_back( Lir_Class::LoadImmediate( type_loaded, dest_register, dest_decl, pConst->value() ) );
	}

	// declare static data and load it....
	else
	{
		string id = CBZ::get_temp_id_str();

		// are we big-endian or little-endian?
		static enum { UNKNOWN, BIG, LITTLE } endian = UNKNOWN;
		if ( endian == UNKNOWN )
		{
			// if this fires then this test won't work - we are assuming 32-bit integers here
			assert( sizeof(unsigned int) == 4 );

			// see which end a pointer points to
			unsigned int foo = 0x12345678;
			if ( *((unsigned char*)&foo) == 0x12 )
				endian = BIG;
			else if ( *((unsigned char*)&foo) == 0x78 )
				endian = LITTLE;

			// make sure our test succeeded
			assert( endian != UNKNOWN );
		}

		// declare a global variable for this number
		declNode * pDeclStr = new declNode( id.c_str(), declNode::STATIC, pConst->type(), pConst, NULL );
		gen_info._unit->defs().push_back( pDeclStr );

		// grab the data
		int size = 0;
		char * pdat = NULL;
		char c;		unsigned char uc;
		short s;	unsigned short us;
		int i;		unsigned int ui;
		long l;		unsigned long ul;
		float f;
		double d;

		lir_var_type vt = LirVt::getVarType( con.basic() );
#define DAT( sz, var, value ) { size = sz; var = con.value(); pdat = (char*)&var; break; }
		switch ( vt )
		{
		case vt_char:	DAT( sizeof(char),			c,	SChar )
		case vt_uchar:	DAT( sizeof(unsigned char), uc, UChar )
		case vt_short:	DAT( sizeof(short),			s,	SShort )
		case vt_ushort:	DAT( sizeof(unsigned short),us, UShort )
		case vt_int:	DAT( sizeof(int),			i,	SInt )
		case vt_uint:	DAT( sizeof(unsigned int),	ui, UInt )
		case vt_long:	DAT( sizeof(long),			l,	SLong )
		case vt_ulong:	DAT( sizeof(unsigned long), ul, ULong )
		case vt_float:	DAT( sizeof(float),			f,	Float )
		case vt_double:	DAT( sizeof(double),		d,	Double )
		}
#undef DAT

		// put the data into a byte array, most significant byte first
		char data[8];
		if ( endian == LITTLE )
			for ( int i = 0; i < size; ++i )
				data[size-1-i] = pdat[i];
		else if ( endian == BIG )
			for ( int i = 0; i < size; ++i )
				data[i] = pdat[i];

		// declare a global binary data thingy that we can load
		instruction_list & instsGlobal = gen_info._unit->instructions();
		instsGlobal.push_back( Lir_Class::Label( id ) );
		instsGlobal.push_back( Lir_Class::StaticDataBinary( vt, data, size ) );

		// load this into the register
		INSTS->push_back( Lir_Class::LoadStatic( dest_register, dest_decl, id, pDeclStr ) );
	}
}

bool Matcher::store_thing(lir_gen_info & gen_info, bool allowFixupLastInstruction, 
	Node* thing_to_store, Register value_reg, declNode * value_decl, lir_var_type value_type)
{
	// what is the destination type?
	lir_var_type vt = LirVt::getVarType( thing_to_store );
	if ( vt != value_type && CBZ::ArchInfo.types_need_conversion( value_type, vt ) )
	{
		// convert the value we're writing to the proper type
		convert_helper( gen_info, thing_to_store->type(), value_reg, value_decl, value_type, vt );
	}

	// what kind of thing is this?
	switch (thing_to_store->typ())
	{
	case Unary:
	{
		unaryNode * pOpNode = (unaryNode*)thing_to_store;
		Node * operand = pOpNode->expr();
		switch ( pOpNode->op()->id() )
		{
		case Operator::INDIR:
			{
				// load the thing we're indirecting
				Register tempReg;
				declNode * pBaseDecl = NULL;
				lir_var_type typeLoaded;
				if (! load_thing( gen_info, operand, tempReg, &pBaseDecl, &typeLoaded ))
					return false;
				assert( typeLoaded == vt_pointer );

				// now store to whatever it points to (store base+offset with base == pointer, offset == 0)
				INSTS->push_back( Lir_Class::Store( typeLoaded, value_reg, value_decl, tempReg, pBaseDecl, 0, NULL ) );

				break;
			}
		default:
			CBZFAIL(( "store_thing: can't store thing with unary operator %d.", pOpNode->op()->id() ));
		}

		break;
	}
	case Binary:
	{
		binaryNode * pOpNode = (binaryNode*)thing_to_store;
		switch ( pOpNode->op()->id() )
		{
		case Operator::ARROW:
			{
				// get the value of the pointer into a register
				Register tempReg;
				declNode * pPtrDecl = NULL;
				lir_var_type vtPtr;
				if (! load_thing( gen_info, pOpNode->left(), tempReg, &pPtrDecl, &vtPtr ))
					return false;
				assert( vtPtr == vt_pointer );

				// find the name the structure being accessed
				assert( pOpNode->left()->typ() == Id );
				assert( pOpNode->left()->base_type(true)->typ() == Ptr );
				typeNode * pStructNode = pOpNode->left()->type()->type()->base_type(true);
				assert( pStructNode->typ() == Struct );
				string structName = ((sueNode*)pStructNode)->spec()->name();

				// find the name of the field being accessed
				assert( pOpNode->right()->typ() == Id );
				idNode * pIdNode = (idNode*)pOpNode->right();
				string fieldName = pIdNode->name();

				// does it have a decl?  if not, find the right one
				if ( ! pIdNode->decl() )
					pIdNode->decl( ((sueNode*)pStructNode)->spec()->find_field( fieldName ) );

				// get the offset of the thing on the right
				unsigned int offset = STRUCTS->find_struct( structName )->find_member( fieldName )->getOffset();

				// now store it (store base+offset with base == pointer, offset == field offset)
				lir_var_type typeToStore = LirVt::getVarType( pIdNode );
				INSTS->push_back( Lir_Class::Store( typeToStore, value_reg, value_decl, tempReg, pPtrDecl, (int)offset, NULL ) );

				break;
			}
		default:
			CBZFAIL(( "store_thing: can't store thing with unary operator %d.", pOpNode->op()->id() ));
		}

		break;
	}
	case Id:
	{
		idNode * pIdNode = (idNode*)thing_to_store;

		// where does it live?
		Storage_location loc = pIdNode->decl()->storage_location();
		assert( loc._type != Storage_location::storageloc_unknown );
		if ( loc._type == Storage_location::storageloc_register )
		{
			// are we allowed to tweak a previous instruction?
			if ( ! allowFixupLastInstruction || INSTS->size() < 1 || 
				! patch_instruction_for_store( INSTS->back(), loc._register, pIdNode->decl(), value_reg, value_decl ) )
			{
				// just move their value into appropriate register
				INSTS->push_back( Lir_Class::Move( loc._register, pIdNode->decl(), value_reg, value_decl ) );
			}
		}
		else if ( loc._type == Storage_location::storageloc_stack )
		{
			// load it into a register
			lir_var_type typeToStore = LirVt::getVarType( pIdNode );
			INSTS->push_back( Lir_Class::Store( typeToStore, value_reg, value_decl, Register::getRegFp(), 
				DATA_CONTENTS_FRAMEP, loc._stack_offset, NULL ) );
		}

		break;
	}
	default:
		CBZFAIL(( "store_thing: can't store thing with node type %d.", thing_to_store->typ() ));
	}

	return true;
}

bool Matcher::patch_instruction_for_store(LirInst * pInst, Register desired_dest,
	declNode * pDesiredDestDecl, Register expected_dest, declNode * pExpectedDestDecl) 
{
	if ( ! pInst )
		return false;

	// do the expected registers match?
	if ( pInst->dest != expected_dest || pInst->dest_contents != pExpectedDestDecl )
		return false;

	// what *is* the last instruction?
	switch ( pInst->instruction )
	{
	case mn_Store:
	case mn_BranchEQ:
	case mn_BranchNE:
	case mn_BranchLT:
	case mn_BranchLE:
	case mn_BranchGT:
	case mn_BranchGE:
	case mn_Jmp:
	case mn_Call:
	case mn_Return:
	case mn_Label:
	case mn_DeclLocal:
	case mn_StaticDataString:
	case mn_StaticDataBinary:

		// these instructions cannot be changed to accomplish our goal
		return false;

	default:

		// change the last instruction so that its result goes into the desired register
		pInst->dest = desired_dest;
		pInst->dest_contents = pDesiredDestDecl;
		break;
	}

	return true;
}

bool Matcher::get_thing_address(lir_gen_info & gen_info, Node* thing, Register& dest_register,
	declNode *& pDeclDest)
{
	// what kind of thing is this?
	switch (thing->typ())
	{
	case Unary:
	{
		unaryNode * pOpNode = (unaryNode*)thing;
		Node * operand = pOpNode->expr();
		switch ( pOpNode->op()->id() )
		{
		case Operator::INDIR:
			{
				// the address of a dereferenced pointer is just the pointer itself (although this somewhat confuses me)
				lir_var_type typeLoaded;
				if (! load_thing( gen_info, operand, dest_register, &pDeclDest, &typeLoaded ))
					return false;
				assert( typeLoaded == vt_pointer );

				break;
			}
		default:
			CBZFAIL(( "get_thing_address: can't take address of result of unary operator %d.", pOpNode->op()->id() ));
		}

		break;
	}
	case Binary:
	{
		binaryNode * pOpNode = (binaryNode*)thing;
		switch ( pOpNode->op()->id() )
		{
		case Operator::ARROW:
			{
				// get the address of the thing on the left
				Register structAddrReg;
				declNode * pDeclStruct;
				if (! get_thing_address( gen_info, pOpNode->left(), structAddrReg, pDeclStruct ))
					return false;

				// find the name the structure being accessed
				assert( pOpNode->left()->typ() == Ptr );
				typeNode * pStructNode = pOpNode->left()->type()->type();
				assert( pStructNode->typ() == Struct );
				string structName = ((sueNode*)pStructNode)->spec()->name();

				// find the name of the field being accessed
				assert( pOpNode->right()->typ() == Id );
				idNode * pIdNode = (idNode*)pOpNode->right();
				string fieldName = pIdNode->name();

				// does it have a decl?  if not, find the right one
				if ( ! pIdNode->decl() )
					pIdNode->decl( ((sueNode*)pStructNode)->spec()->find_field( fieldName ) );

				// get the offset of the thing on the right
				unsigned int offset = STRUCTS->find_struct( structName )->find_member( fieldName )->getOffset();

				// now compute the address of it
				if ( ! dest_register.isValid() )
					ALLOC->get_temp_var( TEMPALLOC_TYPE_POINTER, dest_register, pDeclDest );
				INSTS->push_back( Lir_Class::GetEffectiveAddress( dest_register, pDeclDest, 
					structAddrReg, pDeclStruct, (int)offset, NULL ) );

				break;
			}
		default:
			CBZFAIL(( "get_thing_address: can't take address of result of binary operator %d.", pOpNode->op()->id() ));
		}

		break;
	}
	case Id:
	{
		idNode * pIdNode = (idNode*)thing;

		// this thing has to live on the stack if we are going to take its address
		declNode * pDecl = pIdNode->decl();
		Storage_location loc = pDecl->storage_location();
		assert( loc._type != Storage_location::storageloc_register );

		// get its effective address
		if ( ! dest_register.isValid() )
			ALLOC->get_temp_var( TEMPALLOC_TYPE_POINTER, dest_register, pDeclDest );
		INSTS->push_back( Lir_Class::GetEffectiveAddress( dest_register, pDeclDest, Register::getRegFp(), 
			DATA_CONTENTS_FRAMEP, loc._stack_offset, NULL ) );

		break;
	}
	default:
		CBZFAIL(( "get_thing_address: can't get address of thing with node type %d.", thing->typ() ));
	}

	return true;
}

typeNode * Matcher::get_thing_type( Node * thing )
{
	// does it have a type?
	if ( thing->type() )
		return thing->type();

	// what kind of thing is it?
	switch ( thing->typ() )
	{
	case Binary:
	{
		binaryNode * bin = (binaryNode*)thing;
		switch ( bin->op()->id() )
		{
		case Operator::ARROW:
		{
			// find the name the structure being accessed
			assert( bin->left()->typ() == Id );
			assert( bin->left()->base_type(true)->typ() == Ptr );
			typeNode * pStructNode = bin->left()->type()->type()->base_type(true);
			assert( pStructNode->typ() == Struct );

			// find the name of the field being accessed
			assert( bin->right()->typ() == Id );
			idNode * pIdNode = (idNode*)bin->right();
			string fieldName = pIdNode->name();

			// find the decl of the field being accessed and fixup type information
			declNode * decl = ((sueNode*)pStructNode)->spec()->find_field( fieldName );
			assert( decl->type() );
			bin->type( decl->type() );
			return decl->type();
		}
		default:
			CBZFAIL(( "Can't figure out type of binary node with operator %d.\n", bin->op()->id() ));
		}
		break;
	}
	default:
		CBZFAIL(( "Can't figure out type of node with type %d.\n", thing->typ() ));
	}

	return NULL;
}

mnemonic Matcher::get_inst_for_operator(Operator::OperatorID id) 
{
	switch ( id )
	{
	case '+': 			return mn_Add;
	case '-': 			return mn_Sub;
	case '*': 			return mn_Mul;
	case '/': 			return mn_Div;
	case '%': 			return mn_Mod;
	case '&': 			return mn_BitwiseAND;
	case '|': 			return mn_BitwiseOR;
	case '^': 			return mn_BitwiseXOR;
	case '~': 			return mn_BitwiseNOT;
	case Operator::LS:	return mn_BitwiseShiftLeft;
	case Operator::RS:	return mn_BitwiseShiftRight;
	case Operator::EQ:
	case Operator::NE:
	case '<':
	case Operator::LE:
	case '>':
	case Operator::GE:
		return mn_Compare;
	default:
		assert( false );
		return mn_undefined;
	}
}


