// $Id: ifnode.cc,v 1.4 2003/08/07 23:13:06 pnav Exp $
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

ifNode::ifNode(exprNode * expr, stmtNode * true_br, stmtNode * false_br,
	       const Coord if_coord, 
	       const Coord else_coord)
  : selectionNode(If, expr, true_br, if_coord),
    _false_br(blockNode::toBlock(false_br, if_coord)),
    _else_coord(else_coord)
{}

// ------------------------------------------------------------
//  Walker
// ------------------------------------------------------------

void ifNode::visit(Visitor * the_visitor) 
{
  the_visitor->at_if(this);
}

void ifNode::walk(Walker & the_walker)
{
  Walker::Order ord = the_walker.order(); 

  if (ord == Walker::Preorder || ord == Walker::Both)
    the_walker.at_if(this, Walker::Preorder);

  if (the_walker.depth() == Walker::Subtree) {
    // -- Visit the children

    if (expr())
      expr()->walk(the_walker);

    if (true_br())
      true_br()->walk(the_walker);

    if (false_br())
      false_br()->walk(the_walker);
  }

  if (ord == Walker::Postorder || ord == Walker::Both)
    the_walker.at_if(this, Walker::Postorder);
}

// ------------------------------------------------------------
//  Dataflow
// ------------------------------------------------------------

void ifNode::dataflow(FlowVal * v, FlowProblem & fp)
{
  if (fp.forward()) {
    fp.flow_if(v, this, FlowProblem::Entry);

    if (expr())
      if (fp.basicblocks()) {
	fp.flow_basicblock(v, expr(), FlowProblem::Entry);
	fp.flow_basicblock(v, expr(), FlowProblem::Exit);
      }
      else
	expr()->dataflow(v, fp);
    
    FlowVal * fv = v->clone();

    if (true_br())
      true_br()->dataflow(v, fp);

    if (false_br())
      false_br()->dataflow(fv, fp);

    v->meet(fv);
    delete fv;

    fp.flow_if(v, this, FlowProblem::Exit);
  }
  else {
    fp.flow_if(v, this, FlowProblem::Exit);

    FlowVal * fv = v->clone();

    if (true_br())
      true_br()->dataflow(v, fp);

    if (false_br())
      false_br()->dataflow(fv, fp);

    v->meet(fv);
    delete fv;

    if (expr())
      if (fp.basicblocks()) {
	fp.flow_basicblock(v, expr(), FlowProblem::Exit);
	fp.flow_basicblock(v, expr(), FlowProblem::Entry);
      }
      else
	expr()->dataflow(v, fp);

    fp.flow_if(v, this, FlowProblem::Entry);
  }
}

// ------------------------------------------------------------
// Output
// ------------------------------------------------------------

void ifNode::output_stmt(output_context & ct, Node * parent)
{
  ct << "if " << '(';

  if (expr())
    expr()->output(ct, this);

  ct << ')';
  ct.space();

  if (true_br())
    true_br()->output(ct, this);
  else
    ct << ';';

  blockNode * fb = false_br();
  if (fb) {
    ct.new_line();

    ct << "else";
    ct.space();

    false_br()->output(ct, this);
  }
}

// ------------------------------------------------------------
//  Changer
// ------------------------------------------------------------

Node * ifNode::change(Changer & the_changer, bool redispatch)
{
  Changer::Order ord = the_changer.order(); 
  ifNode * the_if = this;

  if ((ord == Changer::Preorder || ord == Changer::Both) && ! redispatch)
    the_if = (ifNode *) the_changer.at_if(the_if, Changer::Preorder);

  if (the_if) {

    if (the_if != this)
      return the_if->change(the_changer, true);

    exprNode * old_expr = the_if->expr();
    if (old_expr) {
      exprNode * new_expr = (exprNode *) old_expr->change(the_changer);
      if (old_expr != new_expr) {
	//if (the_changer.delete_old())
	  //delete old_expr;
        the_if->expr(new_expr);
      }
    }

    blockNode * old_true_br = the_if->true_br();
    if (old_true_br) {
      blockNode * new_true_br = (blockNode *) old_true_br->change(the_changer);
      if (old_true_br != new_true_br) {
	//if (the_changer.delete_old())
	  // delete old_true_br;
        the_if->true_br(new_true_br);
      }
    }

    blockNode * old_false_br = the_if->false_br();
    if (old_false_br) {
      blockNode * new_false_br = (blockNode *) old_false_br->change(the_changer);
      if (old_false_br != new_false_br) {
	//if (the_changer.delete_old())
	  //delete old_false_br;
        the_if->false_br(new_false_br);
      }
    }

  }

  if ((ord == Changer::Postorder || ord == Changer::Both) && ! redispatch)
    the_if = (ifNode *) the_changer.at_if(the_if, Changer::Postorder);

  return the_if;
}


// ------------------------------------------------------------
// Destructor
// ------------------------------------------------------------

ifNode::~ifNode()
{
  //delete _false_br;
}
