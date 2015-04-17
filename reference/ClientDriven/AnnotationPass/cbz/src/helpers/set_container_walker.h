// $Id: set_container_walker.h,v 1.6 2003/08/07 23:13:53 pnav Exp $
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

#ifndef CBZ_SET_CONTAINER_WALKER_H
#define CBZ_SET_CONTAINER_WALKER_H


/** @brief visits switches and loops, marking continue, return, and break statements.
    @ingroup Phases
*/
class set_container_walker : public Walker
{

public:

  static void fixup(Node * n);

private:

  stmt_list _stack;

  procNode * _current_proc;

  stmtNode * nearest();
  switchNode * nearest_switch();
  loopNode * nearest_loop();

  set_container_walker()
    : Walker(Both, Subtree), 
      _stack()
  {}

public:

  virtual void at_node(Node * the_node, Order ord)
  { }

  // -- These are the containers; push or pop them...

  virtual void at_switch(switchNode * the_switch, Order ord)
  {
    if (ord == Preorder) {
      _stack.push_front(the_switch);
      the_switch->cases().clear();
    }
    else _stack.pop_front();
  }

  virtual void at_loop(loopNode * the_loop, Order ord)
  {
    if (ord == Preorder) _stack.push_front(the_loop);
    else _stack.pop_front();
  }

  // -- These are the containees; find their containers...

  virtual void at_case(caseNode * the_case, Order ord);

  virtual void at_continue(continueNode * the_continue, Order ord);

  virtual void at_break(breakNode * the_break, Order ord);

  // -- Also handle return statements...

  void at_proc(procNode * the_proc, Order ord)
  {
    if (ord == Preorder)
      _current_proc = the_proc;
    else
      _current_proc = (procNode *)0;
  }

  void at_return(returnNode * the_return, Order ord)
  {
    if (ord == Preorder)
      the_return->proc(_current_proc);
  }
};



#endif // CBZ_SET_CONTAINER_WALKER_H
