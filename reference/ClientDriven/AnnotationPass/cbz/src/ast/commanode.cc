// $Id: commanode.cc,v 1.3 2003/08/07 23:13:01 pnav Exp $
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

commaNode::commaNode(expr_list * exprs, const Coord coord)
  : exprNode(Comma, 0, coord),
    _exprs()
{
  if (exprs) {
    _exprs.swap(* exprs);
    //delete exprs;
  }
}

commaNode::commaNode(const Coord coord)
  : exprNode(Comma, 0, coord),
    _exprs()
{}

// ------------------------------------------------------------
// Data type base
// ------------------------------------------------------------

typeNode * commaNode::base_type(bool TdefIndir) const
{
  const expr_list & the_exprs = exprs();
 
  // -- The type of a comma expr is the type of the last expr in the
  // list.

 if (! the_exprs.empty())
    return the_exprs.back()->base_type(TdefIndir);
 else
   return (typeNode *)0;
}

// ------------------------------------------------------------
// Operator precedence
// ------------------------------------------------------------

int commaNode::precedence(Assoc & assoc)
{
  assoc = Left;
  return 1;
}

// ------------------------------------------------------------
// Expression evaluator
// ------------------------------------------------------------

void commaNode::eval()
{
  // -- Evaluate all the subexpressions

  for (expr_list_p p = exprs().begin();
       p != exprs().end();
       ++p)
    {
      (*p)->eval();
    }

  // -- The value of the comma is the value of the last expr

  value(exprs().back()->value());
}

// ------------------------------------------------------------
//  Walker
// ------------------------------------------------------------

void commaNode::visit(Visitor * the_visitor) 
{
  the_visitor->at_comma(this);
}

void commaNode::walk(Walker & the_walker)
{
  Walker::Order ord = the_walker.order(); 

  if (ord == Walker::Preorder || ord == Walker::Both)
    the_walker.at_comma(this, Walker::Preorder);

  if (the_walker.depth() == Walker::Subtree) {
    // -- Visit the children 

    list_walker(exprs(), the_walker);

    if (type())
      type()->walk(the_walker);
  }

  if (ord == Walker::Postorder || ord == Walker::Both)
    the_walker.at_comma(this, Walker::Postorder);
}

// ------------------------------------------------------------
//  Dataflow
// ------------------------------------------------------------

void commaNode::dataflow(FlowVal * v, FlowProblem & fp)
{
  if (fp.forward()) {
    fp.flow_comma(v, this, FlowProblem::Entry);

    dataflow_forward_list(exprs(), v, fp);

    fp.flow_comma(v, this, FlowProblem::Exit);
  }
  else {
    fp.flow_comma(v, this, FlowProblem::Exit);

    dataflow_reverse_list(exprs(), v, fp);

    fp.flow_comma(v, this, FlowProblem::Entry);
  }
}

// ------------------------------------------------------------
// Output
// ------------------------------------------------------------

void commaNode::output_expr(output_context & ct, Node * parent, int prec, Assoc assoc)
{
  bool par = parens(prec, assoc);
  int myprec;
  Assoc myassoc;

  myprec = precedence(myassoc);

  if (par)
    ct << '(';

  output_delim_list(exprs(), ct, this, ',');

  if (par)
    ct << ')';
}

// ------------------------------------------------------------
//  Changer
// ------------------------------------------------------------

Node * commaNode::change(Changer & the_changer, bool redispatch)
{
  Changer::Order ord = the_changer.order(); 
  commaNode * the_comma = this;

  if ((ord == Changer::Preorder || ord == Changer::Both) && ! redispatch)
    the_comma = (commaNode *) the_changer.at_comma(the_comma, Changer::Preorder);

  if (the_comma) {

    if (the_comma != this)
      return the_comma->change(the_changer, true);

    change_list(the_comma->exprs(), the_changer);

    typeNode * old_type = the_comma->type();
    if (old_type) {
      typeNode * new_type = (typeNode *) old_type->change(the_changer);
      if (old_type != new_type) {
	//if (the_changer.delete_old())
	 // delete old_type;
        the_comma->type(new_type);
      }
    }

  }

  if ((ord == Changer::Postorder || ord == Changer::Both) && ! redispatch)
    the_comma = (commaNode *) the_changer.at_comma(the_comma, Changer::Postorder);

  return the_comma;
}


// ------------------------------------------------------------
// Destructor
// ------------------------------------------------------------

commaNode::~commaNode()
{
  //delete_list(_exprs);
}
