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

exprstmtNode::exprstmtNode(exprNode * expr)
  : stmtNode(Expr, Coord::Unknown),
    _expr(expr)
{
  if (expr)
    coord(expr->coord());
}

// ------------------------------------------------------------
// Data type base
// ------------------------------------------------------------

typeNode * exprstmtNode::base_type(bool TdefIndir) const
{
  return expr()->base_type(TdefIndir);
}

// ------------------------------------------------------------
//  Walker
// ------------------------------------------------------------

void exprstmtNode::visit(Visitor * the_visitor) 
{
  the_visitor->at_exprstmt(this);
}

void exprstmtNode::walk(Walker & the_walker)
{
  Walker::Order ord = the_walker.order(); 

  if (ord == Walker::Preorder || ord == Walker::Both)
    the_walker.at_exprstmt(this, Walker::Preorder);

  if (the_walker.depth() == Walker::Subtree) {
    // -- Visit the children 

    if (expr())
      expr()->walk(the_walker);
  }

  if (ord == Walker::Postorder || ord == Walker::Both)
    the_walker.at_exprstmt(this, Walker::Postorder);
}

// ------------------------------------------------------------
//  Dataflow
// ------------------------------------------------------------

void exprstmtNode::dataflow(FlowVal * v, FlowProblem & fp)
{
  if (fp.forward()) {
    fp.flow_exprstmt(v, this, FlowProblem::Entry);

    if (expr())
      if (fp.basicblocks()) {
	fp.flow_basicblock(v, expr(), FlowProblem::Entry);
	fp.flow_basicblock(v, expr(), FlowProblem::Exit);
      }
      else
	expr()->dataflow(v, fp);
    
    fp.flow_exprstmt(v, this, FlowProblem::Exit);
  }
  else {
    fp.flow_exprstmt(v, this, FlowProblem::Exit);

    if (expr())
      if (fp.basicblocks()) {
	fp.flow_basicblock(v, expr(), FlowProblem::Exit);
	fp.flow_basicblock(v, expr(), FlowProblem::Entry);
      }
      else
	expr()->dataflow(v, fp);

    fp.flow_exprstmt(v, this, FlowProblem::Entry);
  }
}

// ------------------------------------------------------------
// Output
// ------------------------------------------------------------

void exprstmtNode::output_stmt(output_context & ct, Node * parent)
{
  if (expr())
    expr()->output(ct, this);

  ct << ';' ;
}

// ------------------------------------------------------------
//  Changer
// ------------------------------------------------------------

Node * exprstmtNode::change(Changer & the_changer, bool redispatch)
{
  Changer::Order ord = the_changer.order(); 
  exprstmtNode * the_exprstmt = this;

  if ((ord == Changer::Preorder || ord == Changer::Both) && ! redispatch)
    the_exprstmt = (exprstmtNode *) the_changer.at_exprstmt(the_exprstmt, Changer::Preorder);

  if (the_exprstmt) {

    if (the_exprstmt != this)
      return the_exprstmt->change(the_changer, true);

    exprNode * old_expr = the_exprstmt->expr();
    if (old_expr) {
      exprNode * new_expr = (exprNode *) old_expr->change(the_changer);
      if (old_expr != new_expr) {
	//if (the_changer.delete_old())
	  //delete old_expr;
        the_exprstmt->expr(new_expr);
      }
    }

  }

  if ((ord == Changer::Postorder || ord == Changer::Both) && ! redispatch)
    the_exprstmt = (exprstmtNode *) the_changer.at_exprstmt(the_exprstmt, Changer::Postorder);

  return the_exprstmt;
}


// ------------------------------------------------------------
// Destructor
// ------------------------------------------------------------

exprstmtNode::~exprstmtNode()
{
  //delete _expr;
}
