// $Id: exprnode.cc,v 1.7 2003/09/24 15:25:52 abrown Exp $
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

exprNode::exprNode(NodeType typ, typeNode * type, const Coord coord)
  : Node(typ, coord),
    _type(type),
    _value(0)
{}

// --------------------------------------------------------------------
// Const value
// --------------------------------------------------------------------

/* Obsolete
constNode * exprNode::const_value() const
{
  Node * val = value();
  if (val && val->typ() == Const)
    return (constNode *) val;
  else
    return (constNode *) 0;
}
*/

// ------------------------------------------------------------
// Data type base
// ------------------------------------------------------------

typeNode * exprNode::base_type(bool TdefIndir) const
{
  return type()->base_type(TdefIndir);
}

// ------------------------------------------------------------
// Operator precedence
// ------------------------------------------------------------

int exprNode::precedence(Assoc & assoc)
{
  // -- Default: highest precedence, left assoc

  assoc = Left;
  return 20;
}

// ------------------------------------------------------------
// Conversions
// ------------------------------------------------------------

exprNode * exprNode::integral_promotions(exprNode * old_expr)
{
  typeNode * new_type = typeNode::integral_promotions(old_expr->type());
  if (new_type)
    if ( old_expr->typ() == Operand )
      ((operandNode *) old_expr)->cast(new_type);
    else
      return new castNode(new_type, old_expr, true, old_expr->coord());
  else
    return old_expr;
}

pair<exprNode *, exprNode *>
exprNode::usual_arithmetic_conversions(exprNode * left,
				       exprNode * right)
{
  pair<typeNode *, typeNode *> types =
    typeNode::usual_arithmetic_conversions(left->no_tdef_type(), 
					   right->no_tdef_type());

  pair<exprNode *, exprNode *> out;

  if (types.first)
    if ( left->typ() == Operand ) {
      ((operandNode *) left)->cast(types.first);
      out.first = left;
    } else
      out.first = new castNode(types.first, left, true, left->coord());
  else
    out.first = left;

  if (types.second)
    if ( right->typ() == Operand ) {
      ((operandNode *) right)->cast(types.second);
      out.second = right;
    } else
      out.second = new castNode(types.second, right, true, right->coord());
  else
    out.second = right;

  return out;
}

// ------------------------------------------------------------
// Parens
// ------------------------------------------------------------

bool exprNode::parens(int outer_prec,
		      Assoc outer_assoc)
{
  Assoc my_assoc;
  int my_prec;

  my_prec = precedence(my_assoc);

  if (parenthesized())
    return true;
  else if (my_prec < outer_prec)
    return true;
  else if (my_prec > outer_prec)
    return false;
  else
    return my_assoc != outer_assoc;
}

// ------------------------------------------------------------
// Output
// ------------------------------------------------------------

void exprNode::output(output_context & ct, Node * parent)
{
  output_expr(ct, parent, 0, Left);
}

// ------------------------------------------------------------
// Destructor
// ------------------------------------------------------------

exprNode::~exprNode()
{
  //delete _type;
}
