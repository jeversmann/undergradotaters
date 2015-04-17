// $Id: init_flowproblem_walker.cc,v 1.3 2003/08/07 23:14:14 pnav Exp $
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
#include "init_flowproblem_walker.h"

// ------------------------------------------------------------
// Entry points
// ------------------------------------------------------------

void init_flowproblem_walker::init(Node * n, FlowProblem * f)
{
  init_flowproblem_walker fpw(f, true);
  n->walk(fpw);
}

void init_flowproblem_walker::cleanup(Node * n, FlowProblem * f)
{
  init_flowproblem_walker fpw(f, false);
  n->walk(fpw);
}
// ------------------------------------------------------------
// Node types
// ------------------------------------------------------------

void init_flowproblem_walker::at_proc(procNode * the_proc, Order ord)
{
  if (_init)
    the_proc->at_exit(_fp->top()->clone());
  else
    delete the_proc->at_exit();
}

void init_flowproblem_walker::at_stmt(stmtNode * the_stmt, Order ord)
{
  if (_init)
    the_stmt->at_exit(_fp->top()->clone());
  else
    delete the_stmt->at_exit();
}

void init_flowproblem_walker::at_target(targetNode * the_target, Order ord)
{
  at_stmt(the_target, ord);
  if (_init)
    the_target->at_entry(_fp->top()->clone());
  else
    delete the_target->at_entry();
}

void init_flowproblem_walker::at_switch(switchNode * the_switch, Order ord)
{
  at_stmt(the_switch, ord);
  if (_init)
    the_switch->at_top(_fp->top()->clone());
  else
    delete the_switch->at_top();
}

void init_flowproblem_walker::at_loop(loopNode * the_loop, Order ord)
{
  at_stmt(the_loop, ord);
  if (_init) {
    the_loop->at_loop_head(_fp->top()->clone());
    the_loop->at_loop_tail(_fp->top()->clone());
  } else {
    delete the_loop->at_loop_head();
    delete the_loop->at_loop_tail();
  }
}
