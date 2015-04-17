// $Id: ptrnode.cc,v 1.4 2003/08/07 23:13:11 pnav Exp $
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

ptrNode::ptrNode(Type_qualifiers tq, typeNode * the_type, const Coord coord)
  : typeNode(Ptr, tq, the_type, coord)
{}

// ------------------------------------------------------------
//  Type Equal
// ------------------------------------------------------------

bool ptrNode::qualified_equal_to(typeNode * node2, bool strict_toplevel, bool strict_recursive)
{
  // pnav
  // don't cast to ptrNode, since node2 may be an Array
  // just check underlying type
  return equal_to(type(), node2->type(),
		  strict_recursive, strict_recursive);
}

// ------------------------------------------------------------
//  Walker
// ------------------------------------------------------------

void ptrNode::visit(Visitor * the_visitor) 
{
  the_visitor->at_ptr(this);
}

void ptrNode::walk(Walker & the_walker)
{
  Walker::Order ord = the_walker.order(); 

  if (ord == Walker::Preorder || ord == Walker::Both)
    the_walker.at_ptr(this, Walker::Preorder);

  if (the_walker.depth() == Walker::Subtree) {
    // -- Visit the children 
    if (type())
      type()->walk(the_walker);
  }

  if (ord == Walker::Postorder || ord == Walker::Both)
    the_walker.at_ptr(this, Walker::Postorder);
}

// ------------------------------------------------------------
// Output
// ------------------------------------------------------------

void ptrNode::output_type(output_context & ct, Node * parent,
			  Assoc context, Type_qualifiers q)
{
  typeNode * t = type();

  bool f_or_a = (t->typ() == Array) || (t->typ() == Func);

  if (context == Left) {

    t->output_type(ct, this, Left, q);

    ct.space();

    if (f_or_a) {
      ct << '(';
    }

    ct << '*';

    ct << type_qualifiers_name();
  }
  else {
    if (f_or_a)
      ct << ')';

    t->output_type(ct, this, Right, q);
  }    
}

// ------------------------------------------------------------
//  Changer
// ------------------------------------------------------------

Node * ptrNode::change(Changer & the_changer, bool redispatch)
{
  Changer::Order ord = the_changer.order(); 
  ptrNode * the_ptr = this;

  if ((ord == Changer::Preorder || ord == Changer::Both) && ! redispatch)
    the_ptr = (ptrNode *) the_changer.at_ptr(the_ptr, Changer::Preorder);

  if (the_ptr) {

    if (the_ptr != this)
      return the_ptr->change(the_changer, true);

    typeNode * old_type = the_ptr->type();
    if (old_type) {
      typeNode * new_type = (typeNode *) old_type->change(the_changer);
      if (old_type != new_type) {
	//if (the_changer.delete_old())
	  //delete old_type;
        the_ptr->type(new_type);
      }
    }

  }

  if ((ord == Changer::Postorder || ord == Changer::Both) && ! redispatch)
    the_ptr = (ptrNode *) the_changer.at_ptr(the_ptr, Changer::Postorder);

  return the_ptr;
}


// ------------------------------------------------------------
// Destructor
// ------------------------------------------------------------

ptrNode::~ptrNode()
{
}
