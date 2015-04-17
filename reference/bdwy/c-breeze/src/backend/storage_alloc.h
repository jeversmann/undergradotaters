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

#ifndef _STORAGE_ALLOC_H
#define _STORAGE_ALLOC_H

#include "c_breeze.h"
#include "register.h"
#include "struct_info.h"

//
// helper class used in initial LIR generation phase to allocate 
//	registers and stack locations for variables
//
class storage_alloc
{
public:

	// ctor/dtor
	storage_alloc( Enc_Data & structData );
	~storage_alloc();

	// reset this thing (should be called before processing a new procedure)
	void				reset();

	// the current procedure of interest
	procNode *			_proc;

public:

	// allocate a temporary variable and a register for it
#define TEMPALLOC_TYPE_POINTER ((typeNode*)2)
	void				get_temp_var( typeNode * intendedType, Register & reg, declNode *& pDecl );

	// allocate storage for the specified declaration
	void				alloc_decl( declNode * pDecl, bool forceToMemory = false );

private:

	// the next local register to be allocated
	unsigned int		_next_reg;

	// the next input register to be allocated
	unsigned int		_next_reg_formal_fixed;
	unsigned int		_next_reg_formal_float;

	// the next stack location to be allocated to a formal
	unsigned int		_next_stack_formal;

	// whether or not we can handle any more formals
	// this is set to false when we encounter the ellipsis in a varadic function
	bool				_allow_more_formals;

	// our structure data (controlled by whoever created us)
	Enc_Data &			_structData;


};

//
// helper class used to walk a procedure before it is transformed to LIR
//
class storage_alloc_walker : public Walker
{
public:

	// ctor
	storage_alloc_walker( storage_alloc & alloc );

public:

	// the various things we care about when walking
	void				at_proc( procNode * the_proc, Order ord );
	void				at_decl( declNode *the_decl, Order ord );
	void				at_unary( unaryNode *the_unary, Order ord );
	void				at_binary( binaryNode *the_binary, Order ord );

private:

	// our allocator (controlled by whoever created us)
	storage_alloc &		_alloc;

};


#endif

