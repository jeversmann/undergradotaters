// $Id: arraynode.cc,v 1.5 2003/08/07 23:12:55 pnav Exp $
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
#include "semcheck.h"

// --------------------------------------------------------------------
// Constructors
// --------------------------------------------------------------------

arrayNode::arrayNode(Type_qualifiers tq, typeNode * the_type, exprNode * the_dim,
		   const Coord coord)
  : typeNode(Array, tq, the_type, coord),
    _dim(the_dim),
    _size(0) { }

// ------------------------------------------------------------
//  Data type predicates
// ------------------------------------------------------------

bool arrayNode::is_string() const
{
  return type()->is_char();
}

// ------------------------------------------------------------
//  Type Equal
// ------------------------------------------------------------

bool arrayNode::qualified_equal_to(typeNode * node2,
				  bool strict_toplevel, bool strict_recursive)
{
  // pnav
  // this comparison of types should work for array-array and array-ptr comparisons
  if (! equal_to(type(), node2->type(),
		 strict_recursive, strict_recursive))
    return false;

  // pnav
  // if node2 is a ptrNode, and the base types are equal (checked above) 
  // then return true
  if (node2->typ() == Ptr)
    return true;

  // Either both dims are specified and the same or neither is...
  arrayNode * n2 = (arrayNode *) node2;
  if (dim())
    if (n2->dim()) {
/* djimenez replaced this:
      exprNode * dim1 = get_dim();
      exprNode * dim2 = n2->get_dim();
   with this:
*/
      exprNode * dim1 = dim();
      exprNode * dim2 = n2->dim();
/* because it was causing array second dimensions to be squished
   when they were in the same parameter of two prototypes being
   compared.
*/

      semcheck_expr_visitor::check(dim1);
      dim1->eval();

      semcheck_expr_visitor::check(dim2);
      dim2->eval();

      if ( ! dim1->value().no_val() &&
	   ! dim2->value().no_val() ) {

	unsigned long val1 = dim1->value().Integer();
	unsigned long val2 = dim2->value().Integer();

	// -- Get the actual dimensions..must be non-zero and equal

	if (val1 && val2)
	  return val1 == val2;
	else
	  return false;
     
      } else
	return true; // -- TBD: What if constants have not been computed yet?

    } else
      // return false; // pnav
      // OpenGL code, while poor style, has arrays where one dimension is not specified
      return true;

  else {
    // -- Set the dimension and type...
    dim(n2->dim());
    type(n2->type());
  }

  return true;
}

// ------------------------------------------------------------
//  Walker
// ------------------------------------------------------------

void arrayNode::visit(Visitor * the_visitor) 
{
  the_visitor->at_array(this);
}

void arrayNode::walk(Walker & the_walker)
{
  Walker::Order ord = the_walker.order(); 

  if (ord == Walker::Preorder || ord == Walker::Both)
    the_walker.at_array(this, Walker::Preorder);

  if (the_walker.depth() == Walker::Subtree) {
    // -- Visit the children 

    if (type())
      type()->walk(the_walker);

    if (dim())
      dim()->walk(the_walker);
  }

  if (ord == Walker::Postorder || ord == Walker::Both)
    the_walker.at_array(this, Walker::Postorder);
}

// ------------------------------------------------------------
// Output
// ------------------------------------------------------------

void arrayNode::output_type(output_context & ct, Node * parent, Assoc context, Type_qualifiers q)
{
  if (context == Left)
    type()->output_type(ct, this, Left, q);
  else {
    ct << '[';
    // djimenez
    // dim() is null sometimes, like when initializing an array e.g.
    // int foo[] = { 1, 2, 3 };

    if (dim()) dim()->output(ct, this);
    ct << ']';

    type()->output_type(ct, this, Right, q);
  }
}

// ------------------------------------------------------------
//  Changer
// ------------------------------------------------------------

Node * arrayNode::change(Changer & the_changer, bool redispatch)
{
  Changer::Order ord = the_changer.order(); 
  arrayNode * the_array = this;

  if ((ord == Changer::Preorder || ord == Changer::Both) && ! redispatch)
    the_array = (arrayNode *) the_changer.at_array(the_array, Changer::Preorder);

  if (the_array) {

    if (the_array != this)
      return the_array->change(the_changer, true);

    typeNode * old_type = the_array->type();
    if (old_type) {
      typeNode * new_type = (typeNode *) old_type->change(the_changer);
      if (old_type != new_type) {
	//if (the_changer.delete_old())
	  //delete old_type;
        the_array->type(new_type);
      }
    }

    exprNode * old_dim = the_array->dim();
    if (old_dim) {
      exprNode * new_dim = (exprNode *) old_dim->change(the_changer);
      if (old_dim != new_dim) {
	//if (the_changer.delete_old())
	 // delete old_dim;
        the_array->dim(new_dim);
      }
    }

  }

  if ((ord == Changer::Postorder || ord == Changer::Both) && ! redispatch)
    the_array = (arrayNode *) the_changer.at_array(the_array, Changer::Postorder);

  return the_array;
}


// ------------------------------------------------------------
// Destructor
// ------------------------------------------------------------

arrayNode::~arrayNode()
{
  //delete _dim;
}
