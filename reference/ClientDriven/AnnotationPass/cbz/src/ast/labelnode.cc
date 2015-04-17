// $Id: labelnode.cc,v 1.3 2003/08/07 23:13:08 pnav Exp $
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

labelNode::labelNode(const char * name, stmtNode * stmt, const Coord coord)
  : targetNode(Label, stmt, coord),
    _name(string(name)),
    _references()
{}

// -- Used in BuildLabel and ResolveGoto in procedure.c

labelNode::labelNode(idNode * ident, stmtNode * the_stmt)
  : targetNode(Label, the_stmt, ident->coord()), 
    _name(ident->name()),
    _references()
{
  /* -- NOTE: This is all made obsolete by goto_label_walker....

  labelNode * orig = Symbols::Labels->insert(ident->text(), this);

  if (orig) {

    // -- Conflict

    if ( ! is_undeclared() )
      if ( ! orig->is_undeclared() )
	CBZ::SyntaxError(coord(),
			 string("multiple definitions of label `") +
			 orig->name() + string("'"));
      else {
	// -- Original is undeclared
	// Loop over all the references to the old one and point them here.

	goto_list & olist = orig->references();
	goto_list_p p;

	for (p = olist.begin(); p != olist.end(); ++p)
	  (*p)->label(this);

	// --  Prevent EndOfScope checker from whining
	orig->stmt((blockNode *)0);
	_references = orig->references();
      }
  }

  */
}

// -- Build an undeclared label (used in ResolveGoto when we reach
// a goto before seeing the label.
// Now obsolete.

labelNode::labelNode(idNode * ident)
  : targetNode(Label, (stmtNode *) 0, ident->coord()), 
    _name(ident->name()),
    _references()
{
  // labelNode * orig = Symbols::Labels->insert(ident->text(), this);
}

// ------------------------------------------------------------
// Undeclared
// ------------------------------------------------------------
// A label is undeclared if it has a stmt(), but the statement
// is of type "Undeclared".

bool labelNode::is_undeclared() const
{
  return (stmt() && stmt()->typ() == Undeclared);
}

// ------------------------------------------------------------
//  Walker
// ------------------------------------------------------------

void labelNode::visit(Visitor * the_visitor) 
{
  the_visitor->at_label(this);
}

void labelNode::walk(Walker & the_walker)
{
  Walker::Order ord = the_walker.order(); 

  if (ord == Walker::Preorder || ord == Walker::Both)
    the_walker.at_label(this, Walker::Preorder);

  if (the_walker.depth() == Walker::Subtree) {
    // -- Visit the children 

    if (stmt())
      stmt()->walk(the_walker);
  }

  if (ord == Walker::Postorder || ord == Walker::Both)
    the_walker.at_label(this, Walker::Postorder);
}

// ------------------------------------------------------------
//  Dataflow
// ------------------------------------------------------------

void labelNode::dataflow(FlowVal * v, FlowProblem & fp)
{
  if (fp.forward()) {
    v->meet(at_entry());

    fp.flow_label(v, this, FlowProblem::Entry);

    if (stmt())
      stmt()->dataflow(v, fp);

    fp.flow_label(v, this, FlowProblem::Exit);
  }
  else {
    fp.flow_label(v, this, FlowProblem::Exit);

    if (stmt())
      stmt()->dataflow(v, fp);

    fp.flow_label(v, this, FlowProblem::Entry);

    v->meet(at_entry());
  }
}

// ------------------------------------------------------------
// Output
// ------------------------------------------------------------

void labelNode::output_stmt(output_context & ct, Node * parent)
{
  ct << name() << ':';

  if (stmt())
    stmt()->output(ct, this);
  else
    ct << ';';
}

// ------------------------------------------------------------
//  Changer
// ------------------------------------------------------------

Node * labelNode::change(Changer & the_changer, bool redispatch)
{
  Changer::Order ord = the_changer.order(); 
  labelNode * the_label = this;

  if ((ord == Changer::Preorder || ord == Changer::Both) && ! redispatch)
    the_label = (labelNode *) the_changer.at_label(the_label, Changer::Preorder);

  if (the_label) {

    if (the_label != this)
      return the_label->change(the_changer, true);

    blockNode * old_stmt = the_label->stmt();
    if (old_stmt) {
      blockNode * new_stmt = (blockNode *) old_stmt->change(the_changer);
      if (old_stmt != new_stmt) {
	//if (the_changer.delete_old())
	  //delete old_stmt;
        the_label->stmt(new_stmt);
      }
    }

  }

  if ((ord == Changer::Postorder || ord == Changer::Both) && ! redispatch)
    the_label = (labelNode *) the_changer.at_label(the_label, Changer::Postorder);

  return the_label;
}


// ------------------------------------------------------------
// Destructor
// ------------------------------------------------------------

labelNode::~labelNode()
{
}
