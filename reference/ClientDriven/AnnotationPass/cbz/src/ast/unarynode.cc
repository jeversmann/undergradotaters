// $Id: unarynode.cc,v 1.5 2003/08/07 23:13:15 pnav Exp $
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

unaryNode::unaryNode(unsigned int op_id, exprNode * expr, const Coord coord)
  : exprNode(Unary, 0, coord),
    _op(0),
    _expr(expr),
    _sizeof_type(0)
{
  switch (op_id) {
  case '*':
    _op = Operators::table[Operator::INDIR];
    break;
  case '&':
    _op = Operators::table[Operator::ADDRESS];
    break;
  case '-':
    _op = Operators::table[Operator::UMINUS];
    break;
  case '+':
    _op = Operators::table[Operator::UPLUS];
    break;
  default:
    _op = Operators::table[op_id];
    break;
  }
}

//    Special constructor for sizeof operator

unaryNode::unaryNode(unsigned int op_id, typeNode * the_type, const Coord coord)
  : exprNode(Unary, 0, coord),
    _op(Operators::table[op_id]),
    _expr(0),
    _sizeof_type(the_type)
{}

// ------------------------------------------------------------
// Symbol lookup
// ------------------------------------------------------------

/*
void unaryNode::lookup()
{
  if (op()->id() == Operator::POSTINC  ||  op()->id() == Operator::POSTDEC)
    expr()->lookup();
}
*/

// ------------------------------------------------------------
// Expression evaluator
// ------------------------------------------------------------

void unaryNode::eval()
{
  // Look out for sizeof

  if (expr()) {

    // -- Evaluate the argument

    expr()->eval();

    // -- Evaluate the operator

    value(constant::eval(op(),
			 expr()->value()));
  }
  else
    value(constant());
}

// ------------------------------------------------------------
// Operator precedence
// ------------------------------------------------------------

int unaryNode::precedence(Assoc & assoc)
{
  if (op()->is_left_assoc())
    assoc = Left;
  else
    assoc = Right;

  return op()->unary_prec();
}

// ------------------------------------------------------------
//  Walker
// ------------------------------------------------------------

void unaryNode::visit(Visitor * the_visitor) 
{
  the_visitor->at_unary(this);
}

void unaryNode::walk(Walker & the_walker)
{
  Walker::Order ord = the_walker.order(); 

  if (ord == Walker::Preorder || ord == Walker::Both)
    the_walker.at_unary(this, Walker::Preorder);

  if (the_walker.depth() == Walker::Subtree) {
    // -- Visit the children 

    if (expr())
      expr()->walk(the_walker);

    if (type())
      type()->walk(the_walker);

    if (sizeof_type())
      sizeof_type()->walk(the_walker);
  }

  if (ord == Walker::Postorder || ord == Walker::Both)
    the_walker.at_unary(this, Walker::Postorder);
}

// ------------------------------------------------------------
//  Dataflow
// ------------------------------------------------------------

void unaryNode::dataflow(FlowVal * v, FlowProblem & fp)
{
  if (fp.forward()) {
    fp.flow_unary(v, this, FlowProblem::Entry);

    // -- sizeof operation is not executable...

    if (expr() && (op()->id() != Operator::SIZEOF))
      expr()->dataflow(v, fp);

    fp.flow_unary(v, this, FlowProblem::Exit);
  }
  else {
    fp.flow_unary(v, this, FlowProblem::Exit);

    if (expr() && (op()->id() != Operator::SIZEOF))
      expr()->dataflow(v, fp);

    fp.flow_unary(v, this, FlowProblem::Entry);
  }
}

// ------------------------------------------------------------
// Output
// ------------------------------------------------------------

void unaryNode::output_expr(output_context & ct, Node * parent, int prec, Assoc assoc)
{
  bool par = parens(prec, assoc);

  if (par)
    ct << '(';

  switch (op()->id()) {
  case Operator::SIZEOF:

    // djimenez - added the space after sizeof so things like
    // sizeof foo wouldn't show up as sizeoffoo

    ct << "sizeof ";

    if (sizeof_type()) {
      ct  << '(';
      sizeof_type()->output(ct, this);
      ct << ')';
    }
    else
      expr()->output_expr(ct, this, op()->unary_prec(), Right);

    break;

  case Operator::PREINC:
  case Operator::PREDEC:

    ct << op()->print();
    ct.space();
    expr()->output_expr(ct, this, op()->unary_prec(), Right);

    break;

  case Operator::POSTINC:
  case Operator::POSTDEC:

    expr()->output_expr(ct, this, op()->unary_prec(), Left);
    ct << op()->print();

    break;

  default:

    ct << op()->print();
    ct.space();
    expr()->output_expr(ct, this, op()->unary_prec(), Right);

    break;
  }

  if (par)
    ct << ')';
}

// ------------------------------------------------------------
//  Changer
// ------------------------------------------------------------

Node * unaryNode::change(Changer & the_changer, bool redispatch)
{
  Changer::Order ord = the_changer.order(); 
  unaryNode * the_unary = this;

  if ((ord == Changer::Preorder || ord == Changer::Both) && ! redispatch)
    the_unary = (unaryNode *) the_changer.at_unary(the_unary, Changer::Preorder);

  if (the_unary) {

    if (the_unary != this)
      return the_unary->change(the_changer, true);

    exprNode * old_expr = the_unary->expr();
    if (old_expr) {
      exprNode * new_expr = (exprNode *) old_expr->change(the_changer);
      if (old_expr != new_expr) {
	//if (the_changer.delete_old())
	  //delete old_expr;
        the_unary->expr(new_expr);
      }
    }

    typeNode * old_type = the_unary->type();
    if (old_type) {
      typeNode * new_type = (typeNode *) old_type->change(the_changer);
      if (old_type != new_type) {
	//if (the_changer.delete_old())
	  //delete old_type;
        the_unary->type(new_type);
      }
    }

    typeNode * old_sizeof_type = the_unary->sizeof_type();
    if (old_sizeof_type) {
      typeNode * new_sizeof_type = (typeNode *) old_sizeof_type->change(the_changer);
      if (old_sizeof_type != new_sizeof_type) {
	//if (the_changer.delete_old())
	  //delete old_sizeof_type;
        the_unary->sizeof_type(new_sizeof_type);
      }
    }

  }

  if ((ord == Changer::Postorder || ord == Changer::Both) && ! redispatch)
    the_unary = (unaryNode *) the_changer.at_unary(the_unary, Changer::Postorder);

  return the_unary;
}


// ------------------------------------------------------------
// Destructor
// ------------------------------------------------------------

unaryNode::~unaryNode()
{
  //delete _expr;
}
