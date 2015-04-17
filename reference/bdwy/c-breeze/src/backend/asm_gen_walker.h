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

#ifndef _ASM_GEN_WALKER_H
#define _ASM_GEN_WALKER_H


#include "c_breeze.h"
#include "instruction.h"

class asm_gen_walker : public Walker
{
  friend class reg_alloc;

public:

	// constructor
	asm_gen_walker();

	// destructor
	~asm_gen_walker();


public:
////////////////////////////
// walker interface

	// what to do at a translation unit
	void at_unit( unitNode * the_unit, Order ord );

	// what to do at a procedure
	void at_proc( procNode * the_proc, Order ord );

protected:
///////////////////////////
// helpers

	// emit an instruction to the output file
	void emit( const LirInst * inst );
	void emit( const instruction_list & insts );

	// compute the total size of the stack frame 
	void computeStackFrameSize();

	// inject code for saving/restoring callee-save registers
	void doCalleeSave();

	// inject code for saving/restoring caller-save registers
	void doCallerSave();

	// create a temporary for saving a register
	void createTempForRegSave( unsigned int id, Register & reg, lir_var_type & vt, declNode *& tempDecl );


protected:
///////////////////////////
// variables

	// current translation unit
	unitNode * _unit;

	// current procedure
	procNode * _proc;

	// current output file
	FILE * _pOutFile;

};

#endif
