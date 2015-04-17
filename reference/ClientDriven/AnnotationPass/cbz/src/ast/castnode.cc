// $Id: castnode.cc,v 1.4 2003/08/07 23:13:01 pnav Exp $
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

castNode::castNode(typeNode * type, exprNode * expr, bool implicit,
		   const Coord coord)
  : exprNode(Cast, type, coord),
    _expr(expr),
    _implicit(implicit)
{}

// ------------------------------------------------------------
// Operator precedence
// ------------------------------------------------------------

int castNode::precedence(Assoc & assoc)
{
  assoc = Right;
  return 14;
}

// ------------------------------------------------------------
// Expression evaluator
// ------------------------------------------------------------

void castNode::eval()
{
  // -- Evaluate the sub-expression

  expr()->eval();

  // -- The cast type must be primitive

  if (type()->typ() == Prim) {

    // -- Get the target basic type

    primNode * p = (primNode *) type();
    const basic_type & bt = p->basic();

    // -- Evaluate the cast

    value(constant::cast(bt, expr()->value()));
  }
  else
    value(constant());
}

// ------------------------------------------------------------
//  Walker
// ------------------------------------------------------------

void castNode::visit(Visitor * the_visitor) 
{
  the_visitor->at_cast(this);
}

void castNode::walk(Walker & the_walker)
{
  Walker::Order ord = the_walker.order(); 

  if (ord == Walker::Preorder || ord == Walker::Both)
    the_walker.at_cast(this, Walker::Preorder);

  if (the_walker.depth() == Walker::Subtree) {
    // -- Visit the children 

    if (type())
      type()->walk(the_walker);

    if (expr())
      expr()->walk(the_walker);
  }

  if (ord == Walker::Postorder || ord == Walker::Both)
    the_walker.at_cast(this, Walker::Postorder);
}

// ------------------------------------------------------------
//  Dataflow
// ------------------------------------------------------------

void castNode::dataflow(FlowVal * v, FlowProblem & fp)
{
  if (fp.forward()) {
    fp.flow_cast(v, this, FlowProblem::Entry);

    if (expr())
      expr()->dataflow(v, fp);

    fp.flow_cast(v, this, FlowProblem::Exit);
  }
  else {
    fp.flow_cast(v, this, FlowProblem::Exit);

    if (expr())
      expr()->dataflow(v, fp);

    fp.flow_cast(v, this, FlowProblem::Entry);
  }
}

// ------------------------------------------------------------
// Output
// ------------------------------------------------------------

void castNode::output_expr(output_context & ct, Node * parent, int prec, Assoc assoc)
{
  // For implicit casts, just pass through
  if (is_implicit())
    expr()->output_expr(ct, this, prec, assoc);
  else {

    bool par = parens(prec, assoc);
    int myprec;
    Assoc myassoc;

    myprec = precedence(myassoc);

    if (par)
      ct << '(';

    ct << '(';
    type()->output(ct, this);
    ct << ')';
    ct.space();
    expr()->output_expr(ct, this, myprec, Right);
    
    if (par)
      ct << ')';
  }
}

// ------------------------------------------------------------
//  Changer
// ------------------------------------------------------------

Node * castNode::change(Changer & the_changer, bool redispatch)
{
  Changer::Order ord = the_changer.order(); 
  castNode * the_cast = this;

  if ((ord == Changer::Preorder || ord == Changer::Both) && ! redispatch)
    the_cast = (castNode *) the_changer.at_cast(the_cast, Changer::Preorder);

  if (the_cast) {

    if (the_cast != this)
      return the_cast->change(the_changer, true);

    typeNode * old_type = the_cast->type();
    if (old_type) {
      typeNode * new_type = (typeNode *) old_type->change(the_changer);
      if (old_type != new_type) {
	//if (the_changer.delete_old())
	 // delete old_type;
        the_cast->type(new_type);
      }
    }

    exprNode * old_expr = the_cast->expr();
    if (old_expr) {
      exprNode * new_expr = (exprNode *) old_expr->change(the_changer);
      if (old_expr != new_expr) {
	//if (the_changer.delete_old())
	 // delete old_expr;
        the_cast->expr(new_expr);
      }
    }

  }

  if ((ord == Changer::Postorder || ord == Changer::Both) && ! redispatch)
    the_cast = (castNode *) the_changer.at_cast(the_cast, Changer::Postorder);

  return the_cast;
}


// ------------------------------------------------------------
// Destructor
// ------------------------------------------------------------

castNode::~castNode()
{
  //delete _expr;
}
