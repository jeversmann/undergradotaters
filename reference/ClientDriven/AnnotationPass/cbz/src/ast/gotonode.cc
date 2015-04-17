// $Id: gotonode.cc,v 1.7 2003/08/07 23:13:05 pnav Exp $
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

gotoNode::gotoNode(labelNode * label, const Coord coord, NodeType typ)
  : jumpNode(typ, coord),
    _name(),
    _label(NULL)
{
  if (label) {
    this->label(label);
  }
}

// -- Was ResolveGoto in procedure.c

gotoNode::gotoNode(idNode * ident, const Coord coord)
  : jumpNode(Goto, coord),
    _label(0),
    _name()
{
  /* --- NOTE: This is all made obsolete by goto_label_walker...

  // -- Try to find a matching label

  labelNode * the_label = Symbols::Labels->lookup(ident->text());

  // -- If not there, create an undeclared label. It will be
  // found later when we actually encounter the label.

  if (! the_label)
    the_label = new labelNode(ident);

  assert(the_label);

  // -- Set the label field and add this "goto" to it's references

  label(the_label);
  the_label->add_reference(this);

  */

  name(ident->name());
  //delete ident;
}

void gotoNode::label(labelNode * label) {
  labelNode * old_label = _label;
  _label = label;
  if ( old_label )
    old_label->references().remove(this);
  if ( label ) {
    label->references().push_back(this);
    label->references().sort();
    label->references().unique();
    _name = label->name();
  }
}

// ------------------------------------------------------------
//  Walker
// ------------------------------------------------------------

void gotoNode::visit(Visitor * the_visitor) 
{
  the_visitor->at_goto(this);
}

void gotoNode::walk(Walker & the_walker)
{
  Walker::Order ord = the_walker.order(); 

  if (ord == Walker::Preorder || ord == Walker::Both)
    the_walker.at_goto(this, Walker::Preorder);

  if (ord == Walker::Postorder || ord == Walker::Both)
    the_walker.at_goto(this, Walker::Postorder);
}

// ------------------------------------------------------------
//  Dataflow
// ------------------------------------------------------------

void gotoNode::dataflow(FlowVal * v, FlowProblem & fp)
{
  if (fp.forward()) {
    fp.flow_goto(v, this, FlowProblem::Entry);

    // -- Push current values to the label and then reset to "top"

    if (label())
      label()->at_entry()->meet_and_diff(v, fp);

    v->to_top();

    fp.flow_goto(v, this, FlowProblem::Exit);
  }
  else {
    fp.flow_goto(v, this, FlowProblem::Exit);

    // -- Get the current values from the label

    v->to_top();

    if (label())
      v->meet(label()->at_entry());

    fp.flow_goto(v, this, FlowProblem::Entry);
  }
}

// ------------------------------------------------------------
// Output
// ------------------------------------------------------------

void gotoNode::output_stmt(output_context & ct, Node * parent)
{
  ct << "goto " << name() << ';' ;
}

// ------------------------------------------------------------
//  Changer
// ------------------------------------------------------------

Node * gotoNode::change(Changer & the_changer, bool redispatch)
{
  Changer::Order ord = the_changer.order(); 
  gotoNode * the_goto = this;

  if ((ord == Changer::Preorder || ord == Changer::Both) && ! redispatch)
    the_goto = (gotoNode *) the_changer.at_goto(the_goto, Changer::Preorder);

  if (the_goto) {

    if (the_goto != this)
      return the_goto->change(the_changer, true);

  }

  if ((ord == Changer::Postorder || ord == Changer::Both) && ! redispatch)
    the_goto = (gotoNode *) the_changer.at_goto(the_goto, Changer::Postorder);

  return the_goto;
}


// ------------------------------------------------------------
// Destructor
// ------------------------------------------------------------

gotoNode::~gotoNode()
{
}
