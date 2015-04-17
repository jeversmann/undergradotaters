// $Id: textnode.cc,v 1.4 2003/09/07 19:48:37 abrown Exp $
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

textNode::textNode(const char * text, bool start_new_line, const Coord coord)
  : Node(Text, coord),
    _text(string(text)),
    _start_new_line(start_new_line),
    _suppress_output(false)
{}

// ------------------------------------------------------------
//  Walker
// ------------------------------------------------------------

void textNode::visit(Visitor * the_visitor) 
{
  the_visitor->at_text(this);
}

void textNode::walk(Walker & the_walker)
{
  Walker::Order ord = the_walker.order(); 

  if (ord == Walker::Preorder || ord == Walker::Both)
    the_walker.at_text(this, Walker::Preorder);

  if (ord == Walker::Postorder || ord == Walker::Both)
    the_walker.at_text(this, Walker::Postorder);
}

// ------------------------------------------------------------
// Output
// ------------------------------------------------------------

void textNode::output(output_context & ct, Node * parent)
{
  if ( _suppress_output )
    return;

  if (_start_new_line)
    ct.new_line();

  ct << _text;
}

// ------------------------------------------------------------
//  Changer
// ------------------------------------------------------------

Node * textNode::change(Changer & the_changer, bool redispatch)
{
  Changer::Order ord = the_changer.order(); 
  textNode * the_text = this;

  if ((ord == Changer::Preorder || ord == Changer::Both) && ! redispatch)
    the_text = (textNode *) the_changer.at_text(the_text, Changer::Preorder);

  if ((ord == Changer::Postorder || ord == Changer::Both) && ! redispatch)
    the_text = (textNode *) the_changer.at_text(the_text, Changer::Postorder);

  return the_text;
}


// ------------------------------------------------------------
// Destructor
// ------------------------------------------------------------

textNode::~textNode()
{
}
