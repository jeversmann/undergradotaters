// $Id: structnode.cc,v 1.3 2003/08/07 23:13:12 pnav Exp $
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

structNode::structNode(const Coord coord)
  : sueNode(Struct, coord)
{}

// ------------------------------------------------------------
//  Walker
// ------------------------------------------------------------

void structNode::visit(Visitor * the_visitor) 
{
  the_visitor->at_struct(this);
}

void structNode::walk(Walker & the_walker)
{
  Walker::Order ord = the_walker.order(); 

  if (ord == Walker::Preorder || ord == Walker::Both)
    the_walker.at_struct(this, Walker::Preorder);

  if (the_walker.depth() == Walker::Subtree) {
    // -- Visit the children 

    //    if (elaborated() && spec())
    //      spec()->walk(the_walker);
  }

  if (ord == Walker::Postorder || ord == Walker::Both)
    the_walker.at_struct(this, Walker::Postorder);
}

// ------------------------------------------------------------
//  Changer
// ------------------------------------------------------------

Node * structNode::change(Changer & the_changer, bool redispatch)
{
  Changer::Order ord = the_changer.order(); 
  structNode * the_struct = this;

  if ((ord == Changer::Preorder || ord == Changer::Both) && ! redispatch)
    the_struct = (structNode *) the_changer.at_struct(the_struct, Changer::Preorder);

  if (the_struct) {

    if (the_struct != this)
      return the_struct->change(the_changer, true);

    //    suespecNode * old_spec = the_struct->spec();
    //    if (the_struct->elaborated() && old_spec) {
    //      suespecNode * new_spec = (suespecNode *) old_spec->change(the_changer);
    //      if (old_spec != new_spec) {
    //	if (the_changer.delete_old())
    //	  delete old_spec;
    //        the_struct->spec(new_spec);
    //      }
    //    }

  }

  if ((ord == Changer::Postorder || ord == Changer::Both) && ! redispatch)
    the_struct = (structNode *) the_changer.at_struct(the_struct, Changer::Postorder);

  return the_struct;
}


// ------------------------------------------------------------
// Destructor
// ------------------------------------------------------------

structNode::~structNode()
{
}
