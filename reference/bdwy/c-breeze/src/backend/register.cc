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
#include "register.h"

void Register::setType( lir_var_type t ) 
{
	switch ( t ) {
	case vt_char:
	case vt_uchar:
	case vt_short:
	case vt_ushort:
	case vt_int:
	case vt_uint:
	case vt_long:
	case vt_ulong:
	case vt_pointer:
		makeGPR();
		break;
	case vt_float:
	case vt_double:
		makeFPR();
		break;
	default:
		CBZFAIL(("don't know how to put low-level type %d into a register.", t));
	}
}

string Register::to_string(bool wantPrefixAdd, bool wantPrefixRemove) const
{
	// are we a real register? if so return our name with the right prefix
	string name;
	if ( CBZ::ArchInfo.get_reg_name( *this, name, wantPrefixAdd, wantPrefixRemove ) )
		return name;

	// we are a virtual register - format our name properly
	char * pBuf = new char[20];
	switch( _type )
	{
	case reg_gpr:		return cbz_util::string_format( "gpr_%d", _num );
	case reg_fpr:		return cbz_util::string_format( "fpr_%d", _num );
	case reg_stack_ptr:	return cbz_util::string_format( "sp" );
	case reg_frame_ptr:	return cbz_util::string_format( "fp" );
	default:			return cbz_util::string_format( "R_%d", _num );
	}
}

string reg_or_const::to_string( bool quote_strings, bool addAsmConstPrefix ) const
{
	// if we are a string and we do not want to quote strings, do things specially
	if ( _is_const && _const.is_str() && ! quote_strings )
		return _const.Str();

	// are we a register?
	if ( ! _is_const )
		return _reg.to_string();

	// we are a constant - we may need to add some prefix to this name
	string str = _const.to_string( false );
	cbz_util::string_replace( str, "UL", "" );
	cbz_util::string_replace( str, "U", "" );
	if ( addAsmConstPrefix && ! _const.is_str() )
		str = CBZ::ArchInfo.get_asm_const_prefix() + str;
	return str;
}

// handy print operator for reg_or_const
ostream & operator << ( ostream& os, const reg_or_const & rc )
{
	// stringize the thing
	os << rc.to_string( true );
	return os;
}

Register Register::getRegSp() 
{
	assert( CBZ::ArchInfo.is_valid() );
	return *(CBZ::ArchInfo.get_reg_sp());
}

Register Register::getRegFp() 
{
	assert( CBZ::ArchInfo.is_valid() );
	return *(CBZ::ArchInfo.get_reg_fp());
}

Register Register::getRegRetValFixed() 
{
	assert( CBZ::ArchInfo.is_valid() );
	const arch_info::register_info* info = CBZ::ArchInfo.get_reg_retval_fixed();
	Register reg;
	if ( info )
		reg = *info;
	return reg;
}

Register Register::getRegRetValFloat() 
{
	assert( CBZ::ArchInfo.is_valid() );
	const arch_info::register_info* info = CBZ::ArchInfo.get_reg_retval_float();
	Register reg;
	if ( info )
		reg = *info;
	return reg;
}

bool Register::getRegRetVal( lir_var_type vt, Register & reg )
{
	// where is the return value?
	Register regRet;
	switch ( vt )
	{
	case vt_void:
		// no return value
		return false;

	case vt_char:
	case vt_uchar:
	case vt_short:
	case vt_ushort:
	case vt_int:
	case vt_uint:
	case vt_long:
	case vt_ulong:
	case vt_pointer:
		// fixed-point return value
		reg = Register::getRegRetValFixed();
		break;

	case vt_float:
	case vt_double:
		// floating-point return value
		reg = Register::getRegRetValFloat();
		break;

	default:
		// this can't go in a register
		CBZFAIL(("low-level type %d can't be returned in a register.", vt));
	}

	return true;
}
