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

caseNode::caseNode(exprNode * expr, stmtNode * stmt, switchNode * container,
		   const Coord coord)
  : targetNode(Case, stmt, coord),
    _expr(expr),
    _container(container)
{}

caseNode::caseNode(exprNode * expr, stmtNode * stmt,
		   const Coord the_coord)
  : targetNode(Case, stmt, the_coord),
    _expr(expr),
    _container(0)
{}

// ------------------------------------------------------------
//  Walker
// ------------------------------------------------------------

void caseNode::visit(Visitor * the_visitor) 
{
  the_visitor->at_case(this);
}

void caseNode::walk(Walker & the_walker)
{
  Walker::Order ord = the_walker.order(); 

  if (ord == Walker::Preorder || ord == Walker::Both)
    the_walker.at_case(this, Walker::Preorder);

  if (the_walker.depth() == Walker::Subtree) {
    // -- Visit the children 

    if (expr())
      expr()->walk(the_walker);

    if (stmt())
      stmt()->walk(the_walker);
  }

  if (ord == Walker::Postorder || ord == Walker::Both)
    the_walker.at_case(this, Walker::Postorder);
}

// ------------------------------------------------------------
//  Dataflow
// ------------------------------------------------------------

void caseNode::dataflow(FlowVal * v, FlowProblem & fp)
{
  if (fp.forward()) {

    // -- Merge in values from the top of the switch

    if (container())
      v->meet(container()->at_top());

    fp.flow_case(v, this, FlowProblem::Entry);

    if (expr())
      if (fp.basicblocks()) {
	fp.flow_basicblock(v, expr(), FlowProblem::Entry);
	fp.flow_basicblock(v, expr(), FlowProblem::Exit);
      }
      else
	expr()->dataflow(v, fp);

    if (stmt())
      stmt()->dataflow(v, fp);

    fp.flow_case(v, this, FlowProblem::Exit);
  }
  else {
      fp.flow_case(v, this, FlowProblem::Exit);

      if (stmt())
	stmt()->dataflow(v, fp);

      if (expr())
	if (fp.basicblocks()) {
	  fp.flow_basicblock(v, expr(), FlowProblem::Exit);
	  fp.flow_basicblock(v, expr(), FlowProblem::Entry);
	}
	else
	  expr()->dataflow(v, fp);

      fp.flow_case(v, this, FlowProblem::Entry);

      // -- Send values to the top of the switch

      if (container())
	v->meet(container()->at_top());
  }
}

// ------------------------------------------------------------
// Output
// ------------------------------------------------------------

void caseNode::output_stmt(output_context & ct, Node * parent)
{

  if (expr()) {
    ct << "case ";
    expr()->output(ct, this);
  }
  else
    ct << "default";

  ct << ':';

    if (stmt())
      stmt()->output(ct, this);
  else
    ct << ';';
}

// ------------------------------------------------------------
//  Changer
// ------------------------------------------------------------

Node * caseNode::change(Changer & the_changer, bool redispatch)
{
  Changer::Order ord = the_changer.order(); 
  caseNode * the_case = this;

  if ((ord == Changer::Preorder || ord == Changer::Both) && ! redispatch)
    the_case = (caseNode *) the_changer.at_case(the_case, Changer::Preorder);

  if (the_case) {

    if (the_case != this)
      return the_case->change(the_changer, true);

    exprNode * old_expr = the_case->expr();
    if (old_expr) {
      exprNode * new_expr = (exprNode *) old_expr->change(the_changer);
      if (old_expr != new_expr) {
	//if (the_changer.delete_old())
	 // delete old_expr;
        the_case->expr(new_expr);
      }
    }

    blockNode * old_stmt = the_case->stmt();
    if (old_stmt) {
      blockNode * new_stmt = (blockNode *) old_stmt->change(the_changer);
      if (old_stmt != new_stmt) {
	//if (the_changer.delete_old())
	 // delete old_stmt;
        the_case->stmt(new_stmt);
      }
    }

  }

  if ((ord == Changer::Postorder || ord == Changer::Both) && ! redispatch)
    the_case = (caseNode *) the_changer.at_case(the_case, Changer::Postorder);

  return the_case;
}


// ------------------------------------------------------------
// Destructor
// ------------------------------------------------------------

caseNode::~caseNode()
{
  //delete _expr;
}
