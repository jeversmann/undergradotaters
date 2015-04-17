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

#include <c_breeze.h>
#include "LIR.h"
#include "storage_alloc.h"

// because I'm lazy
typedef declNode::Storage_location Storage_location;

storage_alloc::storage_alloc( Enc_Data & structData ) : 
	_structData( structData )
{
	// reset us to init vars
	reset();
}

storage_alloc::~storage_alloc() 
{
}

void 
storage_alloc::reset() 
{
	// start registers after maximum valid real register number
	_next_reg = CBZ::ArchInfo.get_all_regs().size();

	// start with input register zero
	_next_reg_formal_fixed = 0;
	_next_reg_formal_float = 0;

	// stack formals start at an arch-specific offset from frame pointer
	_next_stack_formal = CBZ::ArchInfo.get_stack_formals_offset();

	// allow more formals
	_allow_more_formals = true;
}

void 
storage_alloc::get_temp_var( typeNode * intendedType, Register & reg, declNode *& pDecl ) 
{
	// allocate a new register
	reg = _next_reg++;

	// need to make up type?
	if ( intendedType == TEMPALLOC_TYPE_POINTER )
	{
		// make it pointer to void
		typeNode * pVoidType = new primNode( basic_type::Void );
		intendedType = new ptrNode( typeNode::NONE, pVoidType );
	}

	// make a declaration for it
	pDecl = new declNode( CBZ::get_temp_id_str().c_str(), declNode::AUTO, intendedType, NULL, NULL );
}

void 
storage_alloc::alloc_decl( declNode * pDecl, bool forceToMemory ) 
{
	declNode::Storage_location & loc = pDecl->storage_location();

	// size and alignment requirements
	unsigned int size = 0, align = 0;

	// what are we trying to allocate?
	bool allowRegister = false;
	bool lastFormal = false;
	lir_var_type vt = LirVt::getVarType( pDecl->type() );
	switch ( vt )
	{
	case vt_char:
	case vt_uchar:
	case vt_short:
	case vt_ushort:
	case vt_int:
	case vt_uint:
	case vt_long:
	case vt_ulong:
	case vt_float:
	case vt_double:
	case vt_pointer:

		// this thing can hopefully live in a register....
		allowRegister = true;

		// figure out how big it is
		size = CBZ::ArchInfo.get_data_size( vt );
		align = CBZ::ArchInfo.get_data_align( vt );
		break;

	case vt_ellipsis:

		// this will have to live in memory and it will be the last allowable function arg
		allowRegister = false;
		lastFormal = false;

		// give it a bogus size and alignment so it at least has something
		size = align = 17;

		break;

	case vt_aggr:
	{
		// this thing has to live in memory
		allowRegister = false;

		// align is max possible
		align = CBZ::ArchInfo.get_max_data_align();

		// size is computed in struct info thing
		suespecNode * structType = (suespecNode*)((sueNode*)pDecl->type())->spec();
		Struct_Data * structInfo = _structData.find_struct( structType->name() );
		assert( structInfo );
		size = structInfo->size();

		break;
	}
	case vt_func_decl:
	case vt_void:

		// this needs no allocation
		return;

	default:

		assert( false );
		break;
	}

	// where is this thing allocated?
	switch ( pDecl->decl_location() )
	{
	case declNode::TOP:		// global variable or function declaration
	{
		// FIXME : not yet working
		assert( false );
		break;
	}
	case declNode::BLOCK:	// simple local variable
	{
		// no registers if we are forcing to mem or forcing no registers
		if ( forceToMemory || CBZ::NoRegAlloc )
			allowRegister = false;

		// return value can always go in register 
		if ( pDecl->is_return_value() )
			allowRegister = true;

		// can we put it in a register?
		if ( allowRegister )
		{
			// give it a register if it doesn't have one
			if ( loc._type == Storage_location::storageloc_unknown )
			{
				loc._type = Storage_location::storageloc_register;

				// is this the function return value?
				if ( pDecl->is_return_value() )
				{
					// allocate to proper return value register
					Register::getRegRetVal( LirVt::getVarType( pDecl ), loc._register );
				}
				else
				{
					// allocate to virtual register
					loc._register = _next_reg++;
					loc._register.setType( LirVt::getVarType( pDecl ) );
				}
			}
		}
		else
		{
			// force it into the stack if not already there
			if ( loc._type != Storage_location::storageloc_stack )
			{
				// have procedure allocate this into the stack
				_proc->alloc_stack_local( pDecl );
			}
		}

		break;
	}
	case declNode::FORMAL:
	{
		// make sure this is allowed
		if ( ! _allow_more_formals )
			CBZ::Fail( __FILE__, __LINE__, "Can't handle additional formals after ellipsis in varadic function." );

		// is it allocated at all? (do this even if we'll later force to memory - 
		//	we may have to know what register it will be in)
		if ( loc._type == Storage_location::storageloc_unknown )
		{
			// parameter-passing registers
			const arch_info::register_info_list & inRegsFixed = CBZ::ArchInfo.get_regs_param_fixed();
			const arch_info::register_info_list & inRegsFloat = CBZ::ArchInfo.get_regs_param_float();

			// where will this thing show up?
			bool isFloat = vt == vt_float || vt == vt_double;
			if ( isFloat && _next_reg_formal_float < inRegsFloat.size() )
			{
				// this parameter will show up in a floating-point register
				loc._type = Storage_location::storageloc_register;
				loc._register = *(inRegsFloat[_next_reg_formal_float]);
				loc._register.type( reg_fpr );
				_next_reg_formal_float++;
			}
			else if ( ! isFloat && _next_reg_formal_fixed < inRegsFixed.size() )
			{
				// this parameter will show up in a fixed-point register
				loc._type = Storage_location::storageloc_register;
				loc._register = *(inRegsFixed[_next_reg_formal_fixed]);
				loc._register.type( reg_gpr );
				_next_reg_formal_fixed++;
			}
			else
			{
				// this parameter will be on the stack

				// align this thing's offset
				_next_stack_formal = ((_next_stack_formal + align - 1) / align) * align;

				// save the location for this thing and update for next thing
				int thisLoc = _next_stack_formal;
				_next_stack_formal += size;
				loc._type = declNode::Storage_location::storageloc_stack;
				loc._stack_offset = thisLoc;
			}
		}

		// if it has to be in memory and it's not already, copy it into the stack
		if ( forceToMemory && loc._type != Storage_location::storageloc_stack )
		{
			// probably ought to be in a register at this point
			assert( loc._type == Storage_location::storageloc_register );

			// make a temporary stack variable and allocate stack space for it
			declNode * pTempDecl = new declNode( "temp", declNode::AUTO, pDecl->type(), NULL, NULL );
			alloc_decl( pTempDecl, true );

			// change this thing so it's in the stack at the location we just allocated
			declNode::Storage_location oldLoc = loc;
			pDecl->storage_location() = pTempDecl->storage_location();
			delete pTempDecl;

			// move register into temp at beginning of proc
			_proc->instructions().insert( _proc->instructions().begin(), 
				Lir_Class::Store( vt, oldLoc._register, pDecl, Register::getRegFp(), 
				DATA_CONTENTS_FRAMEP, loc._stack_offset, NULL ) );
		}

		break;
	}

	case declNode::SU:
	case declNode::ENUM:
	default:

		assert( false );
		break;

	}

	// was this the last formal?
	if ( lastFormal )
		_allow_more_formals = false;
}


storage_alloc_walker::storage_alloc_walker( storage_alloc & alloc ) :
	Walker( Preorder, Walker::Subtree ), _alloc( alloc )
{
}

void 
storage_alloc_walker::at_proc( procNode * the_proc, Order ord ) 
{
	// currently nothing interesting here
}

void 
storage_alloc_walker::at_decl( declNode *the_decl, Order ord ) 
{
	// allocate the thing using normal semantics
	_alloc.alloc_decl( the_decl );
}

void 
storage_alloc_walker::at_unary( unaryNode *the_unary, Order ord ) 
{
	// is this instruction taking the address of a scalar?
	if ( the_unary->op()->id() == Operator::ADDRESS && the_unary->expr()->typ() == Id )
	{
		idNode * pId = (idNode*)the_unary->expr();

		// what is this thing's type?
		lir_var_type vt = LirVt::getVarType( pId );
		switch ( vt )
		{
		case vt_char:
		case vt_uchar:
		case vt_short:
		case vt_ushort:
		case vt_int:
		case vt_uint:
		case vt_long:
		case vt_ulong:
		case vt_float:
		case vt_double:
		case vt_pointer:

			// this thing has to live in memory if it does not already
			_alloc.alloc_decl( pId->decl(), true );
			break;

		case vt_func_decl:
		case vt_aggr:
		case vt_unknown:
		default:

			// this thing should be fine - probably already has memory
			break;
		}
	}
}

void 
storage_alloc_walker::at_binary( binaryNode *binary, Order ord ) 
{
	// if this is (id = funcall()) then we need to flag the id as a return value id
	if ( binary->op()->id() == '=' && 
		binary->left()->typ() == Id && 
		binary->right()->typ() == Call )
	{
		// flag this decl as a return value so we know where it will be later
		((idNode*)binary->left())->decl()->is_return_value( true );
	}
}

