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

unionNode::unionNode(const Coord coord)
  : sueNode(Union, coord)
{}

// ------------------------------------------------------------
//  Walker
// ------------------------------------------------------------

void unionNode::visit(Visitor * the_visitor) 
{
  the_visitor->at_union(this);
}

void unionNode::walk(Walker & the_walker)
{
  Walker::Order ord = the_walker.order(); 

  if (ord == Walker::Preorder || ord == Walker::Both)
    the_walker.at_union(this, Walker::Preorder);

  if (the_walker.depth() == Walker::Subtree) {
    // -- Visit the children 

    //if (elaborated() && spec())
    //  spec()->walk(the_walker);
  }

  if (ord == Walker::Postorder || ord == Walker::Both)
    the_walker.at_union(this, Walker::Postorder);
}

// ------------------------------------------------------------
//  Changer
// ------------------------------------------------------------

Node * unionNode::change(Changer & the_changer, bool redispatch)
{
  Changer::Order ord = the_changer.order(); 
  unionNode * the_union = this;

  if ((ord == Changer::Preorder || ord == Changer::Both) && ! redispatch)
    the_union = (unionNode *) the_changer.at_union(the_union, Changer::Preorder);

  if (the_union) {

    if (the_union != this)
      return the_union->change(the_changer, true);

    //    suespecNode * old_spec = the_union->spec();
    //    if (the_union->elaborated() && old_spec) {
    //      suespecNode * new_spec = (suespecNode *) old_spec->change(the_changer);
    //      if (old_spec != new_spec) {
    //    if (the_changer.delete_old())
    //	   delete old_spec;
    //        the_union->spec(new_spec);
    //      }
    //    }

  }

  if ((ord == Changer::Postorder || ord == Changer::Both) && ! redispatch)
    the_union = (unionNode *) the_changer.at_union(the_union, Changer::Postorder);

  return the_union;
}


// ------------------------------------------------------------
// Destructor
// ------------------------------------------------------------

unionNode::~unionNode()
{
}
