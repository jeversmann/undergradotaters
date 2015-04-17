// $Id: enumnode.cc,v 1.4 2003/09/19 17:04:39 toktb Exp $
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

enumNode::enumNode(const Coord coord)
  : sueNode(Enum, coord)
{}

// --- Was BuildEnum from sue.c

enumNode::enumNode(idNode *id, decl_list * values, 
		   const Coord enum_coord, const Coord left_coord,
		   const Coord the_right_coord)
  : sueNode(Enum, enum_coord)
{
  // -- NOTE: This no longer tests for incomplete enums
  // (which are illegal).

  set_name_fields(id, values, values!=NULL, left_coord, the_right_coord);
}

// ------------------------------------------------------------
//  Walker
// ------------------------------------------------------------

void enumNode::visit(Visitor * the_visitor) 
{
  the_visitor->at_enum(this);
}

void enumNode::walk(Walker & the_walker)
{
  Walker::Order ord = the_walker.order(); 

  if (ord == Walker::Preorder || ord == Walker::Both)
    the_walker.at_enum(this, Walker::Preorder);

  if (the_walker.depth() == Walker::Subtree) {
    // -- Visit the children 

    //if (elaborated() && spec())
    //  spec()->walk(the_walker);
  }

  if (ord == Walker::Postorder || ord == Walker::Both)
    the_walker.at_enum(this, Walker::Postorder);
}

// ------------------------------------------------------------
//  Changer
// ------------------------------------------------------------

Node * enumNode::change(Changer & the_changer, bool redispatch)
{
  Changer::Order ord = the_changer.order(); 
  enumNode * the_enum = this;

  if ((ord == Changer::Preorder || ord == Changer::Both) && ! redispatch)
    the_enum = (enumNode *) the_changer.at_enum(the_enum, Changer::Preorder);

  if (the_enum) {

    if (the_enum != this)
      return the_enum->change(the_changer, true);

    // suespecNode * old_spec = the_enum->spec();
    // if (the_enum->elaborated() && old_spec) {
    //  suespecNode * new_spec = (suespecNode *) old_spec->change(the_changer);
    //  if (old_spec != new_spec) {
    //	if (the_changer.delete_old())
    //	  delete old_spec;
    //        the_enum->spec(new_spec);
    //      }
    // }

  }

  if ((ord == Changer::Postorder || ord == Changer::Both) && ! redispatch)
    the_enum = (enumNode *) the_changer.at_enum(the_enum, Changer::Postorder);

  return the_enum;
}


// ------------------------------------------------------------
// Destructor
// ------------------------------------------------------------

enumNode::~enumNode()
{
}
