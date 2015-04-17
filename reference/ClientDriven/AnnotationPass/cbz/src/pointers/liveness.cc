// $Id: liveness.cc,v 1.14 2005/06/03 02:00:48 akkartik Exp $
// ----------------------------------------------------------------------
//
//  C-Breeze
//  C Compiler Framework
// 
//  Copyright (c) 2003 University of Texas at Austin
// 
//  Samuel Z. Guyer
//  Adam Brown
//  Teck Bok Tok
//  Paul Arthur Navratil
//  Calvin Lin
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

#include "c_breeze.h"
#include "liveness.h"

livenessAnalyzer::livenessAnalyzer()
  : analysisProblem(Backward),
    _live_stmts(),
    _live_stmtlocations(),
    _live_mergepoints(),
    _defs(),
    _change(false),
    _change_stack(),
    _visited_procedures()
{}

void livenessAnalyzer::clear()
{
  _live_stmts.clear();
  _live_stmtlocations.clear();
  _live_mergepoints.clear();
  _defs.clear();
  _change = false;
  _change_stack.clear();
}

bool livenessAnalyzer::isLive(stmtNode * stmt)
{
  stmt_liveness_set_p p = _live_stmts.find(stmt);
  return (p != _live_stmts.end());
}

void livenessAnalyzer::collectDefs(pointerValue & pointer)
{
  for (memoryblock_set_p p = pointer.blocks.begin();
       p != pointer.blocks.end();
       ++p)
    {
      memoryBlock * block = *p;
      if ( ! block->write_protected()) {
	memoryDef * def = block->current_def();
	_defs.insert(def);
      }
    }
}

void livenessAnalyzer::addDef(memoryDef * def)
{
  _defs.insert(def);
}

bool livenessAnalyzer::determineLiveness()
{
  if (pointerOptions::Verbose_liveness)
    cout << "  + Determine liveness..." << endl;

  // -- For all defs at this statement...

  for (memorydef_set_p p = _defs.begin();
       p != _defs.end();
       ++p)
    {
      memoryDef * def = *p;
      memoryBlock * owner = def->owner();

      if (pointerOptions::Verbose_liveness)
	cout << "   - Def of " << owner->name() << " at " << * (def->where()) << endl;

      // -- For all uses of this def...

      memoryuse_list uses;
      owner->def_uses(def, uses);

      for (memoryuse_list_p q = uses.begin();
	   q != uses.end();
	   ++q)
	{
	  memoryUse * use = *q;

	  // -- If any one of them comes from a live statement...

	  if (isLive(use, owner)) {

	    if (pointerOptions::Verbose_liveness) {
	      cout << "      + Use at " << * (use->where()) << " is live." << endl;
	      cout << "  => Live" << endl;
	    }

	    // -- Then this statement is live.

	    return true;
	  }

	  if (pointerOptions::Verbose_liveness)
	    cout << "      + Use at " << * (use->where()) << " is dead." << endl;
	}
    }

  // -- Fall through, the statement is dead

  if (pointerOptions::Verbose_liveness)
    cout << "  => Dead" << endl;

  return false;
}

bool livenessAnalyzer::isLive(memoryUse * use, memoryBlock * owner)
{
  Location * where = use->where();
  bool is_live = true;

  switch (where->kind()) {

  case Location::Procedure:
    {
      // -- Procedure locations are always live

      is_live = true;
    }
    break;

  case Location::BasicBlock:
    {
      // -- Basic block locations represent merge points

      basicblockLocation * bb_loc = (basicblockLocation *) where;
      mergepoint_pair merge_point(bb_loc, owner);

      mergepoint_liveness_set_p p = _live_mergepoints.find(merge_point);
      is_live = (p != _live_mergepoints.end());
    }
    break;

  case Location::Statement:
    {
      // -- Statement locations are regular computations

      stmtLocation * stmt_loc = (stmtLocation *) where;

      stmtlocation_liveness_set_p p = _live_stmtlocations.find(stmt_loc);
      is_live = (p != _live_stmtlocations.end());
    }
    break;
  }

  /*
  cout << "isLive(" << owner->name() << ", " << * (use->where()) << ") = ";
  if (is_live)
    cout << "true" << endl;
  else
    cout << "false" << endl;
  */

  return is_live;
}

void livenessAnalyzer::setLive(Location * where, memoryBlock * block)
{
  pair< mergepoint_liveness_set_p, bool > result = _live_mergepoints.insert(mergepoint_pair(where, block));

  // -- Note any changes in liveness status

  if (result.second)
    _change = true;
}


void livenessAnalyzer::setLive(stmtLocation * stmt_loc)
{
  pair< stmtlocation_liveness_set_p, bool > result = _live_stmtlocations.insert(stmt_loc);

  // -- Note any changes in liveness status

  if (result.second) {
    _change = true;

    if (pointerOptions::Verbose_liveness)
      cout << "  + Live statement at " << * stmt_loc << endl;
  }

  // -- Also, record context insensitive

  _live_stmts.insert(stmt_loc->stmt());
}

void livenessAnalyzer::at_index(stmtLocation * current,
				operandNode * operand,
				pointerValue & left,
				pointerValue & right,
				pointerValue & result)
{
  setLive(current);
}

// -- Procedure calls

void livenessAnalyzer::at_call(stmtLocation * current, operandNode * call,
			       pointerValue & call_target,
			       procNode * callee,
			       pointervalue_list & arguments,
			       pointerValue & return_val)
{
  // -- Special case: procedure calls are always live

  setLive(current);
}

// -- Assignments

void livenessAnalyzer::at_assignment(stmtLocation * current,
				     pointerValue & left,
				     pointerValue & right,
				     pointerValue & result,
				     memoryblock_set & changes)
{
  collectDefs(left);
}

// -- Statement types

void livenessAnalyzer::at_return(stmtLocation * stmt,
				 returnNode * ret,
				 pointerValue & result,
				 pointerValue & return_val)
{
  // -- Return statements are always live

  setLive(stmt);
}

void livenessAnalyzer::at_threeAddr(stmtLocation * stmt,
                                    threeAddrNode *threeaddr,
				    pointerValue & result)
{
  // -- Regular expression statement: test the defs to see if the uses that
  // they reach are live.

  if (determineLiveness())
    setLive(stmt);
}

void livenessAnalyzer::at_conditiongoto(stmtLocation * stmt,
			                conditiongotoNode * c,
                                        pointerValue & left,
                                        pointerValue & right,
			                pointerValue & result)
{
  // -- If statements are always live

  setLive(stmt);
}

// -- Memory allocation and deallocation

void livenessAnalyzer::at_allocation(stmtLocation * stmt,
				     pointervalue_list & arguments,
				     memoryBlock * block,
				     memoryblock_set & changes)
{
  setLive(stmt);
}

void livenessAnalyzer::at_deallocation(stmtLocation * stmt,
				       pointerValue & to_deallocate,
				       memoryblock_set & changes)
{
  setLive(stmt);
}

// -- Process a merge point

void livenessAnalyzer::at_merge(Location * where,
				memoryBlock * block,
				memoryuse_list & phi_uses,
				pointerValue & result,
				memoryblock_set & changes)
{
  // -- Test the one def at the merge-point for liveness

  _defs.clear();
  _defs.insert(block->current_def());

  if (determineLiveness())
    setLive(where, block);
}

// -- Control-flow options

void livenessAnalyzer::at_stmt_exit(stmtLocation * stmt,
				    pointerValue & result)
{
  // -- Make sure the _defs set is clear

  _defs.clear();
}

void livenessAnalyzer::at_basicblock_entry(basicblockLocation * blockloc,
					   procedureInfo * info,
					   pointerValue & initial)
{
  // -- If any changes occured, add all the blocks in the backward
  // direction.

  if (_change) {
    info->add_reachable_blocks(blockloc->block(), Backward);
    _change = false;
  }
}

  // -- Procedure boundaries

void livenessAnalyzer::at_procedure_entry(procLocation * proc,
					  procedureInfo * info,
					  pointerValue & return_val)
{
  // -- Pop the state of the change boolean

  _change = _change_stack.front();
  _change_stack.pop_front();

  // -- Record that this procedure is called

  _visited_procedures.insert(info->proc());
}

void livenessAnalyzer::at_procedure_exit(procLocation * proc,
					 procedureInfo * info,
					 pointerValue & return_val)
{
  // -- Push the state of the change boolean

  _change_stack.push_front(_change);
  _change = false;
}

// ----------------------------------------------------------------------

void deadcodeChanger::optimize(unitNode * u,
			       livenessAnalyzer * liveness)
{
  deadcodeChanger ipcc(liveness);

  // -- Remove dead code only in the procedures that the liveness analyzer
  // actually visited

  const procedure_set & procs = liveness->visited_procedures();

  for (procedure_set_cp p = procs.begin();
       p != procs.end();
       ++p)
    {
      procNode * proc = *p;

      proc->change(ipcc);
    }
}

deadcodeChanger::deadcodeChanger(livenessAnalyzer * liveness)
  : Changer( Preorder, Subtree, true),
    _liveness(liveness)
{}

Node * deadcodeChanger::at_threeAddr(threeAddrNode * stmt, Order ord)
{
    // -- If the statement is live, then leave it alone

  if (_liveness->isLive(stmt))
    return stmt;
  else {

    // -- Otherwise, get rid of it.

    ostringstream ostr;
    output_context oc(ostr);
    stmt->output(oc, 0);

    exprstmtNode * repl = new exprstmtNode((exprNode *)0);
    repl->comment() = string("--- DEAD :") + ostr.str();


    if (pointerOptions::Verbose_liveness) {
      cout << "--- DEAD CODE -------------------------------------" << endl;

      output_context oc2(cout);
      stmt->output(oc2, 0);
      cout << endl;
    }

    return repl;
  }

  return stmt;
}

