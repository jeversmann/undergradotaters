// $Id: tdefnode.cc,v 1.3 2003/08/07 23:13:14 pnav Exp $
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

tdefNode::tdefNode(Type_qualifiers tq, const char * name, const Coord coord)
  : typeNode(Tdef, tq, 0, coord),
    _name(string(name)),
    _def(0)
{}

// ------------------------------------------------------------
// ConvertIdToTdef in ast.c
// ------------------------------------------------------------

tdefNode::tdefNode(idNode * the_id, Type_qualifiers tq, typeNode * the_type)
  : typeNode(Tdef, tq, 0, the_id->coord()),
    _name(the_id->name()),
    _def(the_type)
{
  //delete the_id;
}

// ------------------------------------------------------------
// Data type base
// ------------------------------------------------------------

typeNode * tdefNode::base_type(bool TdefIndir) const
{
  if (TdefIndir && def())
    return def()->base_type(TdefIndir);
  else
    return (typeNode *) this;
}

// ------------------------------------------------------------
//  Walker
// ------------------------------------------------------------

void tdefNode::visit(Visitor * the_visitor) 
{
  the_visitor->at_tdef(this);
}

void tdefNode::walk(Walker & the_walker)
{
  Walker::Order ord = the_walker.order(); 

  if (ord == Walker::Preorder || ord == Walker::Both)
    the_walker.at_tdef(this, Walker::Preorder);

  if (ord == Walker::Postorder || ord == Walker::Both)
    the_walker.at_tdef(this, Walker::Postorder);
}

// ------------------------------------------------------------
// Output
// ------------------------------------------------------------

void tdefNode::output_type(output_context & ct, Node * parent, Assoc context, Type_qualifiers q)
{
  if (context == Left) {
    const string & qs = type_qualifiers_name(q);
    const string & qs1 = type_qualifiers_name();

    if (! qs.empty())
      ct.space();
    ct << qs;

    if (! qs1.empty())
      ct.space();

    ct << qs1;

    if (! name().empty())
      ct.space();

    ct << name();
  }
}

// ------------------------------------------------------------
//  Changer
// ------------------------------------------------------------

Node * tdefNode::change(Changer & the_changer, bool redispatch)
{
  Changer::Order ord = the_changer.order(); 
  tdefNode * the_tdef = this;

  if ((ord == Changer::Preorder || ord == Changer::Both) && ! redispatch)
    the_tdef = (tdefNode *) the_changer.at_tdef(the_tdef, Changer::Preorder);

  if (the_tdef) {

    if (the_tdef != this)
      return the_tdef->change(the_changer, true);

  }

  if ((ord == Changer::Postorder || ord == Changer::Both) && ! redispatch)
    the_tdef = (tdefNode *) the_changer.at_tdef(the_tdef, Changer::Postorder);

  return the_tdef;
}


// ------------------------------------------------------------
// Destructor
// ------------------------------------------------------------

tdefNode::~tdefNode()
{
}
