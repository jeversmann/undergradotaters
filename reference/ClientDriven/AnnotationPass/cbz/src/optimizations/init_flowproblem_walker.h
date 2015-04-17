// $Id: init_flowproblem_walker.h,v 1.5 2003/08/07 23:14:15 pnav Exp $
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

#ifndef CBZ_INIT_FLOWPROBLEM_WALKER_H
#define CBZ_INIT_FLOWPROBLEM_WALKER_H


/** @brief Creates copies of nodes in order to work a flow problem.
    @ingroup Phases
 */
class init_flowproblem_walker : public Walker
{

public:

  static void init(Node * n, FlowProblem * f);
  static void cleanup(Node * n, FlowProblem * f);

private:

  FlowProblem * _fp;
  bool _init;

  init_flowproblem_walker(FlowProblem * f, bool init)
    : Walker(Preorder, Subtree),
      _fp(f),
      _init(init)
  {}

public:

  virtual void at_proc(procNode * the_proc, Order ord);
  virtual void at_stmt(stmtNode * the_stmt, Order ord);
  virtual void at_target(targetNode * the_target, Order ord);
  virtual void at_switch(switchNode * the_switch, Order ord);
  virtual void at_loop(loopNode * the_loop, Order ord);
};


#endif // CBZ_INIT_FLOWPROBLEM_WALKER_H
