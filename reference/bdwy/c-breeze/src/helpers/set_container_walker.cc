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
#include "set_container_walker.h"

// ------------------------------------------------------------
// Entry point
// ------------------------------------------------------------

void set_container_walker::fixup(Node * n)
{
  set_container_walker s;
  n->walk(s);
}

// ------------------------------------------------------------
// Search the stack of containers...
// ------------------------------------------------------------

stmtNode * set_container_walker::nearest()
{
  return _stack.front();
}

switchNode * set_container_walker::nearest_switch()
{
  stmt_list_p p;

  p = _stack.begin();
  while (p != _stack.end()) {
    if ((*p)->typ() == Switch)
      return (switchNode *)(*p);
    ++p;
  }

  return (switchNode *) 0;
}

loopNode * set_container_walker::nearest_loop()
{
  stmt_list_p p;

  p = _stack.begin();
  while (p != _stack.end()) {
    NodeType t = (*p)->typ();
    if ((t == For) || (t == While) || (t = Do))
      return (loopNode *)(*p);
    ++p;
  }

  return (loopNode *) 0;
}

// ------------------------------------------------------------
// Use the container stack to set the appropriate
// container data member
// ------------------------------------------------------------

void set_container_walker::at_case(caseNode * the_case, Order ord)
{
  if (ord == Postorder) return;

  // -- Find the nearest enclosing switch statement
  switchNode * sw = nearest_switch();

  if (sw) {
    // -- If found, set the container and add to the case list

    the_case->container(sw);
    sw->cases().push_back(the_case);

    // -- If necessary, indicate that this switch has a default case

    if ( ! the_case->expr())
      sw->has_default(true);

  } else
    // -- Othewise, issue an error
    CBZ::SyntaxError(the_case->coord(), "no enclosing switch statement");
}

void set_container_walker::at_continue(continueNode * the_continue, Order ord)
{
  if (ord == Postorder) return;

  // -- Find the nearest enclosing loop

  loopNode * lp = nearest_loop();

  if (lp)
    the_continue->container(lp);
  else
    CBZ::SyntaxError(the_continue->coord(), "no enclosing loop");
}

void set_container_walker::at_break(breakNode * the_break, Order ord)
{
  if (ord == Postorder) return;

  // -- Find the nearest enclosing loop or switch

  stmtNode * st = nearest();

  if (st)
    the_break->container(st);
  else
    CBZ::SyntaxError(the_break->coord(),
		     "no enclosing loop or switch statement found");
}
