// $Id: initializernode.cc,v 1.3 2003/08/07 23:13:07 pnav Exp $
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

initializerNode::initializerNode(expr_list * exprs, const Coord coord)
  : exprNode(Initializer, 0, coord),
    _exprs()
{
  if (exprs) {
    _exprs.swap(* exprs);
    //delete exprs;
  }
}

// ------------------------------------------------------------
// Expression evaluator
// ------------------------------------------------------------

void initializerNode::eval()
{
  // -- Evaluate all the subexpressions

  for (expr_list_p p = exprs().begin();
       p != exprs().end();
       ++p)
    {
      (*p)->eval();
    }

  // -- This really shouldn't have a value

  value(constant());
}

// ------------------------------------------------------------
//  Walker
// ------------------------------------------------------------

void initializerNode::visit(Visitor * the_visitor) 
{
  the_visitor->at_initializer(this);
}

void initializerNode::walk(Walker & the_walker)
{
  Walker::Order ord = the_walker.order(); 

  if (ord == Walker::Preorder || ord == Walker::Both)
    the_walker.at_initializer(this, Walker::Preorder);

  if (the_walker.depth() == Walker::Subtree) {
    // -- Visit the children 

    list_walker(exprs(), the_walker);

    if (type())
      type()->walk(the_walker);
  }

  if (ord == Walker::Postorder || ord == Walker::Both)
    the_walker.at_initializer(this, Walker::Postorder);
}

// ------------------------------------------------------------
//  Dataflow
// ------------------------------------------------------------

void initializerNode::dataflow(FlowVal * v, FlowProblem & fp)
{
  if (fp.forward()) {
    fp.flow_initializer(v, this, FlowProblem::Entry);

    dataflow_forward_list(exprs(), v, fp);

    fp.flow_initializer(v, this, FlowProblem::Exit);
  }
  else {
    fp.flow_initializer(v, this, FlowProblem::Exit);

    dataflow_reverse_list(exprs(), v, fp);

    fp.flow_initializer(v, this, FlowProblem::Entry);
  }
}

// ------------------------------------------------------------
// Output
// ------------------------------------------------------------

void initializerNode::output_expr(output_context & ct, Node * parent, int prec, Assoc assoc)
{
  bool par = parens(prec, assoc);

  if (par)
    ct << '(';

  ct.indent_in();
  ct.new_line();
  ct << '{';
  ct.space();

  output_delim_list(exprs(), ct, this, ',');

  ct.indent_out();
  ct.space();
  ct << '}';

  if (par)
    ct << ')';
}

// ------------------------------------------------------------
//  Changer
// ------------------------------------------------------------

Node * initializerNode::change(Changer & the_changer, bool redispatch)
{
  Changer::Order ord = the_changer.order(); 
  initializerNode * the_initializer = this;

  if ((ord == Changer::Preorder || ord == Changer::Both) && ! redispatch)
    the_initializer = (initializerNode *) the_changer.at_initializer(the_initializer, Changer::Preorder);

  if (the_initializer) {

    if (the_initializer != this)
      return the_initializer->change(the_changer, true);

    change_list(the_initializer->exprs(), the_changer);

    typeNode * old_type = the_initializer->type();
    if (old_type) {
      typeNode * new_type = (typeNode *) old_type->change(the_changer);
      if (old_type != new_type) {
	//if (the_changer.delete_old())
	  //delete old_type;
        the_initializer->type(new_type);
      }
    }

  }

  if ((ord == Changer::Postorder || ord == Changer::Both) && ! redispatch)
    the_initializer = (initializerNode *) the_changer.at_initializer(the_initializer, Changer::Postorder);

  return the_initializer;
}


// ------------------------------------------------------------
// Destructor
// ------------------------------------------------------------

initializerNode::~initializerNode()
{
  //delete_list(_exprs);
}
