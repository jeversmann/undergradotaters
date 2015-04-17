// $Id: operandnode.cc,v 1.6 2003/08/11 17:17:30 abrown Exp $
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

// --------------------------------------------------------------------
// Constructors
// --------------------------------------------------------------------

operandNode::operandNode(indexNode * the_var, const Coord coord):
  exprNode(Operand, NULL, coord),
  _var(the_var),
  _star(false),
  _addr(false),
  _cast(NULL),
  _fields(),
  _index(NULL)
{}

operandNode::operandNode(indexNode * the_var, bool star, bool addr,
			 const Coord coord):
  exprNode(Operand, NULL, coord),
  _var(the_var),
  _star(star),
  _addr(addr),
  _cast(NULL),
  _fields(),
  _index(NULL)
{}

operandNode::operandNode(indexNode * the_var, bool star, bool addr, 
			 id_list * fields, indexNode * array_index, 
			 const Coord coord):
  exprNode(Operand, NULL, coord),
  _var(the_var),
  _star(star),
  _addr(addr),
  _cast(NULL),
  _fields(),
  _index(array_index)
{
  if ( fields )
    _fields.swap(*fields);
}

typeNode * operandNode::type() const {
  // TODO: document & explain this
  if ( cast() )
    return (typeNode *) cast();
  return noncast_type();
}

typeNode * operandNode::noncast_type(bool convertArrays) const {
  typeNode * base_type = NULL;
  int deref = 0;
  if ( !fields().empty() )
    base_type = fields().back()->type();
  else if ( var()->typ() == Id ) {
    base_type = ((idNode *) var())->type();
    if(star()) deref++;
  } else if ( var()->typ() == Const ) {
    base_type = ((constNode *) var())->type();
    if(star()) deref++;
  }
  if ( index() ) deref++;
  if ( addr() ) deref--;
  if( base_type )
    base_type = base_type->follow_tdefs();
  if (deref < 0) {
    return new ptrNode(typeNode::NONE, base_type);
  } else {
    while(deref-- > 0) {
      typeNode * subtype;
      if ( base_type->typ() == Array )
        subtype = ((arrayNode *) base_type)->type();
      else if ( base_type->typ() == Ptr )
        subtype = ((ptrNode *) base_type)->type();
      else assert(false);
      if ( ( subtype->typ() == Array )
	   && convertArrays )
        base_type= new ptrNode(typeNode::NONE, ((arrayNode*) subtype)->type());
      else
        base_type= subtype->follow_tdefs();
    }
    // incorrectly handled arrayNode for &(a[1]), should be ptrNode
    if ( arrayNode * arrayType = dynamic_cast<arrayNode *>(base_type) )
      if ( convertArrays )
	return new ptrNode(typeNode::NONE, arrayType->type());
      else
	return base_type;
    else
      return base_type;
  }
}

void operandNode::type(typeNode * type) {
  // do nothing, because an operandNode has no type of its own, just
  // the type of its most specific constituent
  // TODO: consider making this just call "cast(type);"
}

// TODO:  does this need to be anything else?
void operandNode::eval() {
  value(constant());
}

// ------------------------------------------------------------
//  Walker
// ------------------------------------------------------------

void operandNode::visit(Visitor * the_visitor) 
{
  the_visitor->at_operand(this);
}

void operandNode::walk(Walker & the_walker)
{
  Walker::Order ord = the_walker.order(); 

  if (ord == Walker::Preorder || ord == Walker::Both)
    the_walker.at_operand(this, Walker::Preorder);

  if (the_walker.depth() == Walker::Subtree) {
    // -- Visit the children 
    // don't want to do this b/c operandNode doesn't have a type of its
    // own (see doc on type()).  also makes the AST look non-tree
//     if ( type() )
//       type()->walk(the_walker);

    var()->walk(the_walker);

    list_walker(fields(), the_walker);

    if ( index() )
      index()->walk(the_walker);
  }

  if (ord == Walker::Postorder || ord == Walker::Both)
    the_walker.at_operand(this, Walker::Postorder);
}

// ------------------------------------------------------------
//  Changer
// ------------------------------------------------------------

Node * operandNode::change(Changer & the_changer, bool redispatch)
{
  Changer::Order ord = the_changer.order(); 
  operandNode * the_oper = this;

  if ((ord == Changer::Preorder || ord == Changer::Both) && ! redispatch)
    the_oper = (operandNode *) the_changer.at_operand(the_oper, 
						      Changer::Preorder);

  if (the_oper) {
    if (the_oper != this)
      return the_oper->change(the_changer, true);

    indexNode * old_var = the_oper->var();
    indexNode * new_var = (indexNode *) old_var->change(the_changer);
    if ( old_var != new_var )
      the_oper->var(new_var);

    change_list(the_oper->fields(), the_changer);

    // don't want to do this b/c operandNode doesn't have a type of its
    // own (see doc on type()).  also makes the AST look non-tree
//     typeNode * old_type = the_oper->type();
//     if (old_type) {
//       typeNode * new_type = (typeNode *) old_type->change(the_changer);
//       if (old_type != new_type) {
//         the_oper->type(new_type);
//       }
//     }

    indexNode * old_index = the_oper->index();
    if ( old_index ) {
      indexNode * new_index = (indexNode *) old_index->change(the_changer);
      if ( old_index != new_index )
	the_oper->index(new_index);
    }
  }

  if ((ord == Changer::Postorder || ord == Changer::Both) && ! redispatch)
    the_oper = (operandNode *) the_changer.at_operand(the_oper, 
						      Changer::Postorder);

  return the_oper;
}

// ------------------------------------------------------------
// Output
// ------------------------------------------------------------

void operandNode::output_expr(output_context & ct, Node * parent, int prec, 
			 Assoc assoc)
{
  // precedence of operators: star, field, index, addr, cast

  // Not sure if we should do this, b/c the operandNode might be on the
  // right-hand side of an assignment => invalid C code.
  if ( cast() && parent && parent->typ()==ThreeAddr &&
       ((threeAddrNode*)parent)->lhs() != this ) {
    ct << '(';
    cast()->output(ct, this);
    ct << ')';
    ct.space();
  }

  if ( addr() ) {
    ct << '&';
    ct.space();
  }

  if ( star() ) {
    ct << '(';
    ct << '*';
  }

  var()->output(ct, this);

  if ( star() )
    ct << ')';

  for ( id_list_p p = fields().begin(); p != fields().end(); p++ ) {
    ct << '.';
    (*p)->output(ct, this);
  }

  if ( index() ) {
    ct << '[';
    index()->output(ct, this);
    ct << ']';
  }
}

// ------------------------------------------------------------
//  Dataflow
// ------------------------------------------------------------

void operandNode::dataflow(FlowVal * v, FlowProblem & fp)
{
  // TODO:  uhhh, sam?
}
