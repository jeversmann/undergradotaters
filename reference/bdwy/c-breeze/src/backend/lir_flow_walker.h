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

#ifndef __LIR_FLOW_WALKER_H
#define __LIR_FLOW_WALKER_H

#include "c_breeze.h"
#include "instruction.h"
#include "register.h"

class lir_flow_walker : public Walker
{
public:

	// constructor
	lir_flow_walker();

	// destructor
	~lir_flow_walker();


public:
////////////////////////////
// walker interface

	// what to do at a procedure
	void at_proc( procNode * the_proc, Order ord );


public:
////////////////////////////
// block analysis helpers

	// structure defining edge
	struct edge
	{
		LirBlock * source;
		LirBlock * target;
	};

	// a list of edges
	typedef list<edge> edge_list;
	typedef edge_list::iterator edge_list_p;

	// build basic blocks for LIR in a procedure
	static void build_lir_blocks( procNode * the_proc );

	// recursively generate depth-first ordering of blocks
	static void get_block_dfo( LirBlock * entryblock, LirBlockList & blocks );

	// do dominator analysis of blocks in a procedure
	static void find_block_doms( procNode * the_proc );

	// find all the loops in a procedure, also sets _depth member of all lir blocks
	// need_recompute_flow - whether or not to recompute flow info 
	//	before looking for loops
	static void find_loops( procNode * proc, bool need_recompute_flow, vector<LirBlockSet> & loops );

public:
///////////////////////////////
// instruction-level analysis

	// compute register liveness at each instruction
	static void computeRegisterLiveness( instruction_list & insts, inst_to_reg_id_map & liveRegs );

	// recursively visit instructions and build up postorder ordering
	static void getInstructionsPostorder( instruction_list & insts, instruction_list_p it,
		instruction_list & ordering );

	// generate predecessor/successor info for an instruction
	static void getInstructionFlowInfo( instruction_list & insts );

	// find an instruction in a list
	static instruction_list_p findInstruction( instruction_list & insts, const LirInst * inst );

protected:

	// print out block info for debugging purposes (disabled in non-debug mode)
	static void debug_print_block_info( procNode * proc );

};

#endif //  __CODEGENPHASE_H__

