// $Id: breaknode.cc,v 1.3 2003/08/07 23:13:00 pnav Exp $
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

// --------------------------------------------------------------------
// Constructors
// --------------------------------------------------------------------

breakNode::breakNode(stmtNode * container, const Coord coord)
  : jumpNode(Break, coord),
    _container(container)
{}

breakNode::breakNode(const Coord the_coord)
  : jumpNode(Break, the_coord),
    _container(0)
{}

// ------------------------------------------------------------
//  Walker
// ------------------------------------------------------------

void breakNode::visit(Visitor * the_visitor) 
{
  the_visitor->at_break(this);
}

void breakNode::walk(Walker & the_walker)
{
  Walker::Order ord = the_walker.order(); 

  if (ord == Walker::Preorder || ord == Walker::Both)
    the_walker.at_break(this, Walker::Preorder);

  if (ord == Walker::Postorder || ord == Walker::Both)
    the_walker.at_break(this, Walker::Postorder);
}

// ------------------------------------------------------------
//  Dataflow
// ------------------------------------------------------------

void breakNode::dataflow(FlowVal * v, FlowProblem & fp)
{
  if (fp.forward()) {
    fp.flow_break(v, this, FlowProblem::Entry);

    // -- Push values to the exit and set to "top"

    if (container())
      container()->at_exit()->meet_and_diff(v, fp);

    v->to_top();

    fp.flow_break(v, this, FlowProblem::Exit);
  }
  else {
    fp.flow_break(v, this, FlowProblem::Exit);

    // -- Get values from the exit

    v->to_top();

    if (container())
      v->meet(container()->at_exit());

    fp.flow_break(v, this, FlowProblem::Entry);
  }
}

// ------------------------------------------------------------
// Output
// ------------------------------------------------------------

void breakNode::output_stmt(output_context & ct, Node * parent)
{
  ct << "break" << ';' ;
}

// ------------------------------------------------------------
//  Changer
// ------------------------------------------------------------

Node * breakNode::change(Changer & the_changer, bool redispatch)
{
  Changer::Order ord = the_changer.order(); 
  breakNode * the_break = this;

  if ((ord == Changer::Preorder || ord == Changer::Both) && ! redispatch)
    the_break = (breakNode *) the_changer.at_break(the_break, Changer::Preorder);

  if (the_break) {

    if (the_break != this)
      return the_break->change(the_changer, true);

  }

  if ((ord == Changer::Postorder || ord == Changer::Both) && ! redispatch)
    the_break = (breakNode *) the_changer.at_break(the_break, Changer::Postorder);

  return the_break;
}


// ------------------------------------------------------------
// Destructor
// ------------------------------------------------------------

breakNode::~breakNode()
{
}
