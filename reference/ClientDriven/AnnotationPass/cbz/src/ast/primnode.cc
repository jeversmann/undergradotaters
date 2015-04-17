// $Id: primnode.cc,v 1.3 2003/08/07 23:13:11 pnav Exp $
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

primNode::primNode(Type_qualifiers tq, basic_type basic, const Coord coord)
  : typeNode(Prim, tq, (typeNode *)0, coord), _basic(basic)
{}

//    MakeDefaultPrimType in type.c

primNode::primNode(Type_qualifiers tq, const Coord coord)
  : typeNode(Prim, tq, 0, coord),
    _basic()
{
  finish();
}

primNode::primNode(const Coord coord)
  : typeNode(Prim, NONE, 0, coord),
    _basic()
{
  finish();
}

//    StartPrimType in type.c

primNode::primNode(basic_type basic, const Coord coord)
  : typeNode(Prim, NONE, 0, coord),
    _basic(basic)
{}

// ------------------------------------------------------------
// FinishPrimType from type.c
// ------------------------------------------------------------

// This function fills in defaults (e.g., if not specified
// then an int is signed).

void primNode::finish()
{
  // -- Fill in the blanks on the basic type...

  basic().finish();
}

primNode * primNode::finish_and()
{
  finish();
  return this;
}

// ------------------------------------------------------------
// MergePrimTypes from type.c
// (the "other" object is deleted
// ------------------------------------------------------------

void primNode::merge_in(primNode * other)
{
  basic_type & bt1 = basic();
  basic_type & bt2 = other->basic();

  bt1.merge_in(bt2, coord());

  //delete other;
}

primNode * primNode::merge_in_and(primNode * other)
{
  merge_in(other);
  return this;
}

// ------------------------------------------------------------
//  Type Equal
// ------------------------------------------------------------

bool primNode::qualified_equal_to(typeNode * node2,
				  bool strict_toplevel, bool strict_recursive)
{
  primNode * n2 = (primNode *) node2;
  return basic() == n2->basic();
}

// ------------------------------------------------------------
//  Conversions
// ------------------------------------------------------------

#define UCHAR_TO_INT_OVF    0
#define USHORT_TO_INT_OVF   0

// -- UsualUnaryConversionType is used by parser to check that
//   old-style function decls contain only "usual unary conversions"
//   types -- in particular, float and char won't be allowed as
//   parameter types.

/* REPLACED by integral promotions

typeNode * primNode::usual_unary_conversion_type()
{
  basic_type & b = basic();

  if (b.is_char()) {
    if (b.is_unsigned())
      return (typeNode *) new primNode(basic_type::UInt);
    else
      return (typeNode *) new primNode(basic_type::SInt);
  }

  if (b.is_int() && b.is_short()) {
    if (b.is_unsigned())
      return (typeNode *) new primNode(basic_type::UInt);
    else
      return (typeNode *) new primNode(basic_type::SInt);
  }

  if (b.is_float())
    return (typeNode *) new primNode(basic_type::Double);

  return (typeNode *) this;
}

*/

// ------------------------------------------------------------
//  Walker
// ------------------------------------------------------------

void primNode::visit(Visitor * the_visitor) 
{
  the_visitor->at_prim(this);
}

void primNode::walk(Walker & the_walker)
{
  Walker::Order ord = the_walker.order(); 

  if (ord == Walker::Preorder || ord == Walker::Both)
    the_walker.at_prim(this, Walker::Preorder);

  if (ord == Walker::Postorder || ord == Walker::Both)
    the_walker.at_prim(this, Walker::Postorder);
}

// ------------------------------------------------------------
// Output
// ------------------------------------------------------------

void primNode::output_type(output_context & ct, Node * parent, Assoc context, Type_qualifiers q)
{
  if (context == Left) {
    const string & qs = type_qualifiers_name();
    const string & bs = basic().to_string();

    ct << qs;

    if (! bs.empty())
      ct.space();

    ct << bs;
  }
}

// ------------------------------------------------------------
//  Changer
// ------------------------------------------------------------

Node * primNode::change(Changer & the_changer, bool redispatch)
{
  Changer::Order ord = the_changer.order(); 
  primNode * the_prim = this;

  if ((ord == Changer::Preorder || ord == Changer::Both) && ! redispatch)
    the_prim = (primNode *) the_changer.at_prim(the_prim, Changer::Preorder);

  if (the_prim) {

    if (the_prim != this)
      return the_prim->change(the_changer, true);

  }

  if ((ord == Changer::Postorder || ord == Changer::Both) && ! redispatch)
    the_prim = (primNode *) the_changer.at_prim(the_prim, Changer::Postorder);

  return the_prim;
}


// ------------------------------------------------------------
// Destructor
// ------------------------------------------------------------

primNode::~primNode()
{
}
