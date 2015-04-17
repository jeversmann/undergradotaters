// $Id: conditiongotonode.cc,v 1.3 2003/08/07 23:13:02 pnav Exp $
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

conditiongotoNode::conditiongotoNode(labelNode * label, indexNode * left, 
				     unsigned int op_id, indexNode * right, 
				     const Coord coord):
  gotoNode(label, coord, Condition),
  _oper1(left),
  _oper2(right)
{
  Operator * new_op = Operators::table[op_id];
  op(new_op);
}

// ------------------------------------------------------------
// Data access & modification
// ------------------------------------------------------------

void conditiongotoNode::op(Operator * op) {
  if ( op->is_comparison() )
    _op = op;
  else
    // TODO: throw an exception?
    ;
}

// ------------------------------------------------------------
// Walker
// ------------------------------------------------------------

void conditiongotoNode::visit(Visitor * the_visitor)
{
  the_visitor->at_conditiongoto(this);
}

void conditiongotoNode::walk(Walker & the_walker)
{
  Walker::Order ord = the_walker.order();

  if ( ord == Walker::Preorder || ord == Walker::Both )
    the_walker.at_conditiongoto(this, Walker::Preorder);

  if ( the_walker.depth() == Walker::Subtree ) {
    // -- Visit the children

    left()->walk(the_walker);
    right()->walk(the_walker);
  }

  if ( ord == Walker::Postorder || ord == Walker::Both )
    the_walker.at_conditiongoto(this, Walker::Postorder);
}

// ------------------------------------------------------------
// Changer
// ------------------------------------------------------------

Node * conditiongotoNode::change(Changer & the_changer, bool redispatch)
{
  Changer::Order ord = the_changer.order();
  conditiongotoNode * the_condgoto = this;

  if ( ( ord == Changer::Preorder || ord == Changer::Both ) && !redispatch )
    the_condgoto = 
      (conditiongotoNode *) the_changer.at_conditiongoto(the_condgoto,
							 Changer::Preorder);
  
  if ( the_condgoto ) {
    if ( the_condgoto != this )
      return the_condgoto->change(the_changer, true);
    
    indexNode * old_left = the_condgoto->left();
    indexNode * new_left = (indexNode *) old_left->change(the_changer);
    if ( old_left != new_left )
      the_condgoto->left(new_left);
    
    indexNode * old_right = the_condgoto->right();
    indexNode * new_right = (indexNode *) old_right->change(the_changer);
    if ( old_right != new_right )
      the_condgoto->right(new_right);
  }

  if ( ( ord == Changer::Postorder || ord == Changer::Both ) && !redispatch)
    the_condgoto = 
      (conditiongotoNode *) the_changer.at_conditiongoto(the_condgoto,
							 Changer::Postorder);

  return the_condgoto;
}

// ------------------------------------------------------------
// Output
// ------------------------------------------------------------

void conditiongotoNode::output_stmt(output_context & ct, Node * parent) 
{
  ct << "if";
  ct.space();
  ct << '(' ;
  ct.space();

  left()->output(ct, this);
  ct.space();
  ct << op()->print();
  ct.space();
  right()->output(ct, this);

  ct << ')';
  ct.space();

  gotoNode::output_stmt(ct, parent);
}

// ------------------------------------------------------------
// Dataflow
// ------------------------------------------------------------

void conditiongotoNode::dataflow(FlowVal * v, FlowProblem & fp)
{
  // TODO:  uhhh, sam?
}
