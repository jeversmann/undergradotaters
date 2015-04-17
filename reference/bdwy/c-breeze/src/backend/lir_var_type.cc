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
#include "lir_var_type.h"

namespace LirVt
{

// a handy function for getting a variable type from a type node
lir_var_type getVarType(Node * thing)
{
	// get this thing's actual type
	typeNode * pType = NULL;
	switch ( thing->typ() )
	{
	case Binary:
	{
		// which operator?
		binaryNode* pBin = (binaryNode*)thing;
		Operator * pOp = pBin->op();

		switch ( pOp->id() )
		{
		case Operator::ARROW:
		{
			// left should be sue (struct or union)
			assert( pBin->left()->typ() == Id );
			idNode * structNode = (idNode *)pBin->left();
			assert( structNode->decl()->type()->typ() == Ptr );
			typeNode * structType = structNode->decl()->type()->type()->base_type( true );
			NodeType lhsTyp = structType->typ();
			assert( lhsTyp == Struct || lhsTyp == Union );
			suespecNode * suespec = ((sueNode*)structType)->spec();

			// right should be id that is member of sue
			assert( pBin->right()->typ() == Id );
			idNode * memberNode = (idNode *)pBin->right();
			string name = memberNode->name();
			decl_list & fields = suespec->fields();
			decl_list_p it = fields.begin();
			for ( ; it != fields.end(); ++it )
				if ( (*it)->name() == name )
					return getVarType( *it );

			// if we get here then this name is not a member of this sue thingy
			CBZFAIL(("error getting type of arrow operator : '%s' is not a member of sue '%s'.",
				name.c_str(), suespec->name().c_str()));
		}
		default:
			CBZFAIL(("don't know how to find primitive type of binary operator '%s'.",
				pOp->name().c_str()));
		}

		break;
	}

	case Unary:
	{
		// which operator?
		unaryNode* pUn = (unaryNode*)thing;
		Operator * pOp = pUn->op();
		switch ( pOp->id() )
		{
		case Operator::INDIR:

			// pointer indirection - result is whatever is pointed to
			assert( pUn->expr()->type()->typ() == Ptr );
			return getVarType( pUn->expr()->type()->type() );

		case Operator::ADDRESS:

			// the address of anything is a pointer
			return vt_pointer;

		case Operator::UPLUS:
		case Operator::UMINUS:
		case Operator::POSTINC:
		case Operator::POSTDEC:
		case Operator::PREINC:
		case Operator::PREDEC:

			// does not change type of operand
			return getVarType( pUn->expr()->type() );

		default:

			// ??? guess we'll just hope for the best....
			break;
		}

		break;
	}

	case Prim:
	case Tdef:
	case Ptr:
	case Array:
	case Func:
	case Struct:
	case Union:
	case Enum:
	case sueSpec:

		// they gave us the type node directly
		pType = (typeNode*)thing;
		break;

	case Call:

		// find out its return type
		pType = ((idNode*)((callNode*)thing)->name())->decl()->type()->type();
		break;

	default:

		// try just getting its base type
		pType = thing->base_type( true );
		break;
	}

	if ( ! pType )
		pType = thing->type();

	assert( pType );

	// what kind of type node is this?
	switch ( pType->typ() )
	{
	case Prim:
	{
		// the basic type of this thing if it has one
		basic_type bt = ((primNode*)pType)->basic();

		// get basic type
		return getVarType( bt );

		// if we are here we have no idea what this is
		assert( false );
		return vt_unknown;
	}
	case Enum:

		// call it an unsigned integer
		return vt_uint;

	case Struct:
	case Union:

		// call it an aggregate
		return vt_aggr;

	case Ptr:
	case Array:

		// call it a pointer
		return vt_pointer;

	case Func:

		// call it a function pointer
		return vt_func_decl;

	default:

		// if we are here we have no idea what this is
		assert( false );
		return vt_unknown;
	}

}

bool isTypeFloat( lir_var_type vt ) 
{
	return (vt == vt_float || vt == vt_double);
}

bool isTypeFixed( lir_var_type vt ) 
{
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
	case vt_pointer:
		return true;
	default:
		return false;
	}
}

lir_var_type getVarType( basic_type bt )
{
	// I agree that this is a stupid way to do this but we have
	// no choice given how basic_type is structured.  If only it
	// were an enumeration.....

	if (bt == basic_type::Void)
		return vt_void;
	if (bt == basic_type::SChar)
		return vt_char;
	if (bt == basic_type::UChar)
		return vt_uchar;
	if (bt == basic_type::SShort)
		return vt_short;
	if (bt == basic_type::UShort)
		return vt_ushort;
	if (bt == basic_type::SInt)
		return vt_int;
	if (bt == basic_type::UInt)
		return vt_uint;
	if (bt == basic_type::SLong)
		return vt_long;
	if (bt == basic_type::ULong)
		return vt_ulong;
	if (bt == basic_type::Float)
		return vt_float;
	if (bt == basic_type::Double)
		return vt_double;
	if (bt == basic_type::Ellipsis)
		return vt_ellipsis;

	assert( false );
	return vt_unknown;
}

const char * getVarTypeString( lir_var_type vt )
{
	// find a string for it
	switch ( vt )
	{
	case vt_void:		return "void"; 
	case vt_char:		return "char"; 
	case vt_uchar:		return "uchar"; 
	case vt_short:		return "short"; 
	case vt_ushort:		return "ushort"; 
	case vt_int:		return "int"; 
	case vt_uint:		return "uint"; 
	case vt_long:		return "long"; 
	case vt_ulong:		return "ulong"; 
	case vt_float:		return "float"; 
	case vt_double:		return "double";
	case vt_func_decl:	return "func_decl";
	case vt_pointer:	return "ptr";
	case vt_aggr:		return "aggr";
	case vt_ellipsis:	return "ellipsis";

	default:
	case vt_unknown:	
		return "unknown"; 
	}
}

}

