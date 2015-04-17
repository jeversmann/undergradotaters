// $Id: binarynode.cc,v 1.5 2003/08/07 23:12:59 pnav Exp $
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

binaryNode::binaryNode(unsigned int op_id, exprNode * left, exprNode * right,
		       const Coord coord)
  : exprNode(Binary, 0, coord),
    _op(Operators::table[op_id]),
    _left(left),
    _right(right)
{
}

// ------------------------------------------------------------
// Symbol lookup
// ------------------------------------------------------------

/*
void binaryNode::lookup()
{
  if (op()->id() == '.' || op()->id() == Operator::ARROW)
    left()->lookup();
}
*/

// ------------------------------------------------------------
// Operator precedence
// ------------------------------------------------------------

int binaryNode::precedence(Assoc & my_assoc)
{
  if (op()->is_left_assoc())
    my_assoc = Left;
  else
    my_assoc = Right;

  return op()->binary_prec();
}

// ------------------------------------------------------------
// Expression evaluator
// ------------------------------------------------------------

void binaryNode::eval()
{
  // -- Evaluate the two arguments...

  left()->eval();
  right()->eval();

  // -- Evaluate the operator

  value(constant::eval(op(),
		       left()->value(),
		       right()->value()));
}

// ------------------------------------------------------------
//  Walker
// ------------------------------------------------------------

void binaryNode::visit(Visitor * the_visitor) 
{
  the_visitor->at_binary(this);
}

void binaryNode::walk(Walker & the_walker)
{
  Walker::Order ord = the_walker.order(); 

  if (ord == Walker::Preorder || ord == Walker::Both)
    the_walker.at_binary(this, Walker::Preorder);

  if (the_walker.depth() == Walker::Subtree) {
    // -- Visit the children 

    if (op()->id()=='.' || op()->id()==Operator::ARROW) {
	// djimenez
	// BUG: we aren't handling the rhs of . or -> properly;
	// it should be looked up and something done with it.
	// the above if/else is a hack so that C-Breeze will at least
	// not bail with an error.
	if (right()->typ() == Id) {
		idNode *n = (idNode *) right();
		// do something here...
	}
    } else {
	    if (right())
	      right()->walk(the_walker);
    }

    if (left())
      left()->walk(the_walker);

    if (type())
      type()->walk(the_walker);
  }

  if (ord == Walker::Postorder || ord == Walker::Both)
    the_walker.at_binary(this, Walker::Postorder);
}

// ------------------------------------------------------------
//  Dataflow
// ------------------------------------------------------------

void binaryNode::dataflow(FlowVal * v, FlowProblem & fp)
{
  Operator * o = op();

  // -- The "&&" and "||" operators require us to consider the binary
  // operation as a branch that always executes it's left part, but
  // may not execute the right part.

  bool short_circuit = ((o->id() == Operator::ANDAND) ||
			(o->id() == Operator::OROR));

  if (fp.forward()) {
    fp.flow_binary(v, this, FlowProblem::Entry);

    if (short_circuit) {
      FlowVal * fv = v->clone();

      if (left())
	left()->dataflow(v, fp);

      if (right())
	right()->dataflow(fv, fp);

      v->meet(fv);
      delete fv;
    }
    else {
      if (left())
	left()->dataflow(v, fp);

      if (right())
	right()->dataflow(v, fp);
    }

    fp.flow_binary(v, this, FlowProblem::Exit);
  }
  else {
    fp.flow_binary(v, this, FlowProblem::Exit);

    if (short_circuit) {
      FlowVal * fv = v->clone();

      if (right())
	right()->dataflow(fv, fp);

      if (left())
	left()->dataflow(v, fp);

      v->meet(fv);
      delete fv;
    }
    else {
      if (right())
	right()->dataflow(v, fp);

      if (left())
	left()->dataflow(v, fp);
    }

    fp.flow_binary(v, this, FlowProblem::Entry);
  }
}

// ------------------------------------------------------------
// Output
// ------------------------------------------------------------

void binaryNode::output_expr(output_context & ct, Node * parent, int prec, Assoc assoc)
{
  bool par = parens(prec, assoc);

  if (par)
    ct << '(';

  left()->output_expr(ct, this, op()->binary_prec(), Left);

  if (op()->id() == Operator::Index) {

    exprNode * d = right();
    if (d) {
      ct << '[';
      d->output(ct, this);
      ct << ']';
    }
  }
  else {

    ct.space();
    ct << op()->print();
    ct.space();

    ct.continue_line();

    right()->output_expr(ct, this, op()->binary_prec(), Right);
  }

  if (par)
    ct << ')';
}

// ------------------------------------------------------------
//  Changer
// ------------------------------------------------------------

Node * binaryNode::change(Changer & the_changer, bool redispatch)
{
  Changer::Order ord = the_changer.order(); 
  binaryNode * the_binary = this;

  if ((ord == Changer::Preorder || ord == Changer::Both) && ! redispatch)
    the_binary = (binaryNode *) the_changer.at_binary(the_binary, Changer::Preorder);

  if (the_binary) {

    if (the_binary != this)
      return the_binary->change(the_changer, true);

    exprNode * old_left = the_binary->left();
    if (old_left) {
      exprNode * new_left = (exprNode *) old_left->change(the_changer);
      if (old_left != new_left) {
	//if (the_changer.delete_old())
	 // delete old_left;
        the_binary->left(new_left);
      }
    }

    exprNode * old_right = the_binary->right();
    if (old_right) {
      exprNode * new_right = (exprNode *) old_right->change(the_changer);
      if (old_right != new_right) {
	//if (the_changer.delete_old())
	 // delete old_right;
        the_binary->right(new_right);
      }
    }

    typeNode * old_type = the_binary->type();
    if (old_type) {
      typeNode * new_type = (typeNode *) old_type->change(the_changer);
      if (old_type != new_type) {
	//if (the_changer.delete_old())
	  //delete old_type;
        the_binary->type(new_type);
      }
    }

  }

  if ((ord == Changer::Postorder || ord == Changer::Both) && ! redispatch)
    the_binary = (binaryNode *) the_changer.at_binary(the_binary, Changer::Postorder);

  return the_binary;
}


// ------------------------------------------------------------
// Destructor
// ------------------------------------------------------------

binaryNode::~binaryNode()
{
  //delete _left;
  //delete _right;
}
