// $Id: ternarynode.cc,v 1.3 2003/08/07 23:13:14 pnav Exp $
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

ternaryNode::ternaryNode(exprNode * cond,
			 exprNode * true_br, exprNode * false_br,
			 const Coord qmark_coord, const Coord colon_coord)
  : exprNode(Ternary, 0, qmark_coord),
    _cond(cond),
    _true_br(true_br),
    _false_br(false_br),
    _colon_coord(colon_coord)
{}

// ------------------------------------------------------------
// Operator precedence
// ------------------------------------------------------------

int ternaryNode::precedence(Assoc & assoc)
{
  assoc = Right;
  return 3;
}

// ------------------------------------------------------------
// Expression evaluator
// ------------------------------------------------------------

void ternaryNode::eval()
{
  // -- Eval the subexpressions

  cond()->eval();
  true_br()->eval();
  false_br()->eval();

  // -- Test if the condition can be evaluated

  if ( ! cond()->value().no_val() )
    if (cond()->value().Boolean())
      value(true_br()->value());
    else
      value(false_br()->value());
  else
    value(constant());
}

// ------------------------------------------------------------
//  Walker
// ------------------------------------------------------------

void ternaryNode::visit(Visitor * the_visitor) 
{
  the_visitor->at_ternary(this);
}

void ternaryNode::walk(Walker & the_walker)
{
  Walker::Order ord = the_walker.order(); 

  if (ord == Walker::Preorder || ord == Walker::Both)
    the_walker.at_ternary(this, Walker::Preorder);

  if (the_walker.depth() == Walker::Subtree) {
    // -- Visit the children 

    if (cond())
      cond()->walk(the_walker);

    if (true_br())
      true_br()->walk(the_walker);

    if (false_br())
      false_br()->walk(the_walker);

    if (type())
      type()->walk(the_walker);
  }

  if (ord == Walker::Postorder || ord == Walker::Both)
    the_walker.at_ternary(this, Walker::Postorder);
}

// ------------------------------------------------------------
//  Dataflow
// ------------------------------------------------------------

void ternaryNode::dataflow(FlowVal * v, FlowProblem & fp)
{
  if (fp.forward()) {
    fp.flow_ternary(v, this, FlowProblem::Entry);

    if (cond())
      cond()->dataflow(v, fp);

    FlowVal * fv = v->clone();

    if (true_br())
      true_br()->dataflow(v, fp);

    if (false_br())
      false_br()->dataflow(fv, fp);

    v->meet(fv);
    delete fv;

    fp.flow_ternary(v, this, FlowProblem::Exit);
  }
  else {
    fp.flow_ternary(v, this, FlowProblem::Exit);

    FlowVal * fv = v->clone();

    if (true_br())
      true_br()->dataflow(v, fp);

    if (false_br())
      false_br()->dataflow(fv, fp);

    v->meet(fv);
    delete fv;

    if (cond())
      cond()->dataflow(v, fp);

    fp.flow_ternary(v, this, FlowProblem::Entry);
  }
}

// ------------------------------------------------------------
// Output
// ------------------------------------------------------------

void ternaryNode::output_expr(output_context & ct, Node * parent, int prec, Assoc assoc)
{
  bool par = parens(prec, assoc);
  int myprec;
  Assoc myassoc;

  myprec = precedence(myassoc);

  if (par)
    ct << '(';

  cond()->output_expr(ct, this, myprec, Left);

  ct << " ? ";

  true_br()->output_expr(ct, this, myprec, Right);

  ct << " : ";

  false_br()->output_expr(ct, this, myprec, Right);

  if (par)
    ct << ')';
}

// ------------------------------------------------------------
//  Changer
// ------------------------------------------------------------

Node * ternaryNode::change(Changer & the_changer, bool redispatch)
{
  Changer::Order ord = the_changer.order(); 
  ternaryNode * the_ternary = this;

  if ((ord == Changer::Preorder || ord == Changer::Both) && ! redispatch)
    the_ternary = (ternaryNode *) the_changer.at_ternary(the_ternary, Changer::Preorder);

  if (the_ternary) {

    if (the_ternary != this)
      return the_ternary->change(the_changer, true);

    exprNode * old_cond = the_ternary->cond();
    if (old_cond) {
      exprNode * new_cond = (exprNode *) old_cond->change(the_changer);
      if (old_cond != new_cond) {
	//if (the_changer.delete_old())
	  //delete old_cond;
        the_ternary->cond(new_cond);
      }
    }

    exprNode * old_true_br = the_ternary->true_br();
    if (old_true_br) {
      exprNode * new_true_br = (exprNode *) old_true_br->change(the_changer);
      if (old_true_br != new_true_br) {
	//if (the_changer.delete_old())
	  //delete old_true_br;
        the_ternary->true_br(new_true_br);
      }
    }

    exprNode * old_false_br = the_ternary->false_br();
    if (old_false_br) {
      exprNode * new_false_br = (exprNode *) old_false_br->change(the_changer);
      if (old_false_br != new_false_br) {
	//if (the_changer.delete_old())
	  //delete old_false_br;
        the_ternary->false_br(new_false_br);
      }
    }

    typeNode * old_type = the_ternary->type();
    if (old_type) {
      typeNode * new_type = (typeNode *) old_type->change(the_changer);
      if (old_type != new_type) {
	//if (the_changer.delete_old())
	  //delete old_type;
        the_ternary->type(new_type);
      }
    }

  }

  if ((ord == Changer::Postorder || ord == Changer::Both) && ! redispatch)
    the_ternary = (ternaryNode *) the_changer.at_ternary(the_ternary, Changer::Postorder);

  return the_ternary;
}


// ------------------------------------------------------------
// Destructor
// ------------------------------------------------------------

ternaryNode::~ternaryNode()
{
  //delete _cond;
  //delete _true_br;
  //delete _false_br;
}
