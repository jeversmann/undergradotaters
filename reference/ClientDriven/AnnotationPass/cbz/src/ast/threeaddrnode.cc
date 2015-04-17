// $Id: threeaddrnode.cc,v 1.5 2003/09/24 12:52:33 abrown Exp $
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
//  Adam Brown
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

// ------------------------------------------------------------
// Constructors
// ------------------------------------------------------------

threeAddrNode::threeAddrNode(operandNode * lhs, operandNode * rhs,
			     const Coord coord):
  stmtNode(ThreeAddr, coord),
  _lhs(lhs),
  _rhs1(rhs),
  _op(NULL),
  _rhs2(NULL),
  _sizeof_type(NULL)
{
}

threeAddrNode::threeAddrNode(operandNode * lhs, unsigned int op_id, 
			     operandNode * rhs, const Coord coord):
  stmtNode(ThreeAddr, coord),
  _lhs(lhs),
  _rhs1(rhs),
  _op(Operators::table[op_id]),
  _rhs2(NULL),
  _sizeof_type(NULL)
{
  // TODO: ensure that _op is a unary operator (!, ~, +, -)
}

threeAddrNode::threeAddrNode(operandNode * lhs, typeNode * type,
			     const Coord coord):
  stmtNode(ThreeAddr, coord),
  _lhs(lhs),
  _rhs1(NULL),
  _op(Operators::table[Operator::SIZEOF]),
  _rhs2(NULL),
  _sizeof_type(type)
{
}

threeAddrNode::threeAddrNode(operandNode * lhs, operandNode * rhs1, 
			     unsigned int op_id, operandNode * rhs2, 
			     const Coord coord):
  stmtNode(ThreeAddr, coord),
  _lhs(lhs),
  _rhs1(rhs1),
  _op(Operators::table[op_id]),
  _rhs2(rhs2),
  _sizeof_type(NULL)
{
  // TODO: ensure that _op is a non-control flow binary.  only arithmetic?
}

threeAddrNode::threeAddrNode(operandNode * lhs, operandNode * func, 
			     operand_list * arg_list, const Coord coord):
  stmtNode(ThreeAddr, coord),
  _lhs(lhs),
  _rhs1(func),
  _op(Operators::table[Operator::FUNC_CALL]),
  _rhs2(NULL),
  _sizeof_type(NULL)
{
  if ( arg_list )
    _arg_list.swap(*arg_list);
}

threeAddrNode::threeAddrNode(operandNode * func, operand_list * arg_list, 
			     const Coord coord):
  stmtNode(ThreeAddr, coord),
  _lhs(NULL),
  _rhs1(func),
  _op(Operators::table[Operator::FUNC_CALL]),
  _rhs2(NULL),
  _sizeof_type(NULL)
{
  if ( arg_list )
    _arg_list.swap(*arg_list);
}

// ------------------------------------------------------------
// Walker
// ------------------------------------------------------------

void threeAddrNode::visit(Visitor * the_visitor)
{
  the_visitor->at_threeAddr(this);
}

void threeAddrNode::walk(Walker & the_walker)
{
  Walker::Order ord = the_walker.order();

  if ( ord == Walker::Preorder || ord == Walker::Both )
    the_walker.at_threeAddr(this, Walker::Preorder);

  if ( the_walker.depth() == Walker::Subtree ) {
    // -- Visit the children
    if ( lhs() )
      lhs()->walk(the_walker);

    if ( rhs1() )
      rhs1()->walk(the_walker);

    if ( rhs2() )
      rhs2()->walk(the_walker);

    if ( sizeof_type() )
      sizeof_type()->walk(the_walker);

    list_walker(arg_list(), the_walker);
  }

  if ( ord == Walker::Postorder || ord == Walker::Both )
    the_walker.at_threeAddr(this, Walker::Postorder);
}

// ------------------------------------------------------------
// Changer
// ------------------------------------------------------------

Node * threeAddrNode::change(Changer & the_changer, bool redispatch)
{
  Changer::Order ord = the_changer.order();
  threeAddrNode * the_3addr = this;

  if ( ( ord == Changer::Preorder || ord == Changer::Both ) && !redispatch )
    the_3addr = (threeAddrNode *) the_changer.at_threeAddr(the_3addr,
							   Changer::Preorder);

  if ( the_3addr ) {
    if ( the_3addr != this )
      return the_3addr->change(the_changer, true);

    operandNode * old_lhs = the_3addr->lhs();
    if ( old_lhs ) {
      operandNode * new_lhs = (operandNode *) old_lhs->change(the_changer);
      if ( old_lhs != new_lhs )
	the_3addr->lhs(new_lhs);
    }

    operandNode * old_rhs1 = the_3addr->rhs1();
    if ( old_rhs1 ) {
      operandNode * new_rhs1 = (operandNode *) old_rhs1->change(the_changer);
      if ( old_rhs1 != new_rhs1 )
	the_3addr->rhs1(new_rhs1);
    }

    operandNode * old_rhs2 = the_3addr->rhs2();
    if ( old_rhs2 ) {
      operandNode * new_rhs2 = (operandNode *) old_rhs2->change(the_changer);
      if ( old_rhs2 != new_rhs2 )
	the_3addr->rhs2(new_rhs2);
    }

    typeNode * old_sizeof_type = the_3addr->sizeof_type();
    if ( old_sizeof_type ) {
      typeNode * new_sizeof_type = 
	(typeNode *) old_sizeof_type->change(the_changer);
      if ( new_sizeof_type != old_sizeof_type )
	the_3addr->sizeof_type(new_sizeof_type);
    }

    change_list(the_3addr->arg_list(), the_changer);
  }

  if ( ( ord == Changer::Postorder || ord == Changer::Both ) && !redispatch )
    the_3addr = (threeAddrNode *) the_changer.at_threeAddr(the_3addr,
							   Changer::Postorder);

  return the_3addr;
}

// ------------------------------------------------------------
// Output
// ------------------------------------------------------------

void threeAddrNode::output_stmt(output_context & ct, Node * parent)
{
  if ( lhs() ) {
    lhs()->output(ct, this);
    ct.space();
    ct << '=' ;
    ct.space();
  }

  if ( op() ) {
    if ( op()->id() == Operator::FUNC_CALL ) {
      rhs1()->output(ct, this);
      ct << '(' ;
      output_delim_list(arg_list(), ct, this, ',');
      ct << ')' ;
    } else if ( op()->id() == Operator::SIZEOF
		&& sizeof_type() ) {
      ct << op()->print();
      ct << '(';
      sizeof_type()->output(ct, this);
      ct << ')';
    } else if ( op()->is_unary() ) {
      ct << op()->print();
      ct.space();
      rhs1()->output(ct, this);
    } else {
      rhs1()->output(ct, this);
      ct.space();
      ct << op()->print();
      ct.space();
      rhs2()->output(ct, this);
    }
  } else 
    rhs1()->output(ct, this);
  
  ct << ';' ;
}

// ------------------------------------------------------------
// Dataflow
// ------------------------------------------------------------

void threeAddrNode::dataflow(FlowVal * v, FlowProblem & fp)
{
  // TODO:  uhhh, sam?
}
