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

#ifndef _LIR_VAR_TYPE_H
#define _LIR_VAR_TYPE_H

// defines a variable type for our low-level representation
enum lir_var_type {	

	vt_unknown, 
	vt_void,
	vt_char,	vt_uchar, 
	vt_short,	vt_ushort, 
	vt_int,		vt_uint, 
	vt_long,	vt_ulong, 
	vt_float,
	vt_double,
	vt_func_decl,
	vt_pointer,
	vt_aggr,
	vt_ellipsis, // type of ... arg for varadic function
};

namespace LirVt
{

	// a handy function for getting a variable type from a type node
	lir_var_type	getVarType( Node * thing );

	// another handy functio, but it works with a basic_type object
	lir_var_type	getVarType( basic_type bt );

	// find out if a type is float/double
	bool			isTypeFloat( lir_var_type vt );

	// find out if a type is one of the fixed-point types (char,int,short,long,pointer,etc.)
	bool			isTypeFixed( lir_var_type vt );

	// stringize a variable type
	const char *	getVarTypeString( lir_var_type vt );

}

#endif
