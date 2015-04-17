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

switchNode::switchNode(exprNode * expr, stmtNode * stmt, const Coord coord)
  : selectionNode(Switch, expr, stmt, coord),
    _cases(),
    _has_default(false)
{}

// ------------------------------------------------------------
//  Walker
// ------------------------------------------------------------

void switchNode::visit(Visitor * the_visitor) 
{
  the_visitor->at_switch(this);
}

void switchNode::walk(Walker & the_walker)
{
  Walker::Order ord = the_walker.order(); 

  if (ord == Walker::Preorder || ord == Walker::Both)
    the_walker.at_switch(this, Walker::Preorder);

  if (the_walker.depth() == Walker::Subtree) {
    // -- Visit the children 

    if (expr())
      expr()->walk(the_walker);

    if (stmt())
      stmt()->walk(the_walker);
  }

  if (ord == Walker::Postorder || ord == Walker::Both)
    the_walker.at_switch(this, Walker::Postorder);
}

// ------------------------------------------------------------
//  Dataflow
// ------------------------------------------------------------

void switchNode::dataflow(FlowVal * v, FlowProblem & fp)
{
  if (fp.forward()) {
    fp.flow_switch(v, this, FlowProblem::Entry);

    if (expr())
      if (fp.basicblocks()) {
	fp.flow_basicblock(v, expr(), FlowProblem::Entry);
	fp.flow_basicblock(v, expr(), FlowProblem::Exit);
      }
      else
	expr()->dataflow(v, fp);
    
    // -- at_top holds flow values needed at cases

    at_top()->meet_and_diff(v, fp);

    // -- If no default, then flow can fall through

    if (! has_default())
      at_exit()->meet_and_diff(v, fp);

    // -- Dataflow the cases (pushes break values into at_exit)
    // Note: control does not flow directly into switch body

    v->to_top();

    if (stmt())
      stmt()->dataflow(v, fp);

    // -- Get the flow values collected from breaks

    v->meet(at_exit());

    fp.flow_switch(v, this, FlowProblem::Exit);
  }
  else {
    fp.flow_switch(v, this, FlowProblem::Exit);

    // -- at_exit holds values needed by breaks

    at_exit()->meet_and_diff(v, fp);

    // -- If no default, then flow can fall through

    if (! has_default())
      at_top()->meet_and_diff(at_exit(), fp);

    // -- Dataflow the cases (push values in to at_top)

    if (stmt())
      stmt()->dataflow(v, fp);

    // -- Get the flow values collected from the cases
    // Note: control does not flow directly into switch body

    v->to_top();
    v->meet(at_top());

    if (expr())
      if (fp.basicblocks()) {
	fp.flow_basicblock(v, expr(), FlowProblem::Exit);
	fp.flow_basicblock(v, expr(), FlowProblem::Entry);
      }
      else
	expr()->dataflow(v, fp);

    fp.flow_switch(v, this, FlowProblem::Entry);
  }
}

// ------------------------------------------------------------
// Output
// ------------------------------------------------------------

void switchNode::output_stmt(output_context & ct, Node * parent)
{
  ct << "switch " << '(';

  if (expr())
    expr()->output(ct, this);

  ct << ')';
  ct.space();

  if (stmt())
    stmt()->output(ct, this);
  else
    ct << ';';
}

// ------------------------------------------------------------
//  Changer
// ------------------------------------------------------------

Node * switchNode::change(Changer & the_changer, bool redispatch)
{
  Changer::Order ord = the_changer.order(); 
  switchNode * the_switch = this;

  if ((ord == Changer::Preorder || ord == Changer::Both) && ! redispatch)
    the_switch = (switchNode *) the_changer.at_switch(the_switch, Changer::Preorder);

  if (the_switch) {

    if (the_switch != this)
      return the_switch->change(the_changer, true);

    exprNode * old_expr = the_switch->expr();
    if (old_expr) {
      exprNode * new_expr = (exprNode *) old_expr->change(the_changer);
      if (old_expr != new_expr) {
	//if (the_changer.delete_old())
	  //delete old_expr;
        the_switch->expr(new_expr);
      }
    }

    blockNode * old_stmt = the_switch->stmt();
    if (old_stmt) {
      blockNode * new_stmt = (blockNode *) old_stmt->change(the_changer);
      if (old_stmt != new_stmt) {
	//if (the_changer.delete_old())
	  //delete old_stmt;
        the_switch->stmt(new_stmt);
      }
    }

  }

  if ((ord == Changer::Postorder || ord == Changer::Both) && ! redispatch)
    the_switch = (switchNode *) the_changer.at_switch(the_switch, Changer::Postorder);

  return the_switch;
}


// ------------------------------------------------------------
// Destructor
// ------------------------------------------------------------

switchNode::~switchNode()
{
}
