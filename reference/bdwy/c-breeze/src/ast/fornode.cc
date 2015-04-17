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

forNode::forNode(exprNode * init, exprNode * cond, exprNode * next,
		 stmtNode * body, const Coord coord)
  : loopNode(For, cond, body, coord),
    _init(init),
    _next(next)
{}

// ------------------------------------------------------------
//  Walker
// ------------------------------------------------------------

void forNode::visit(Visitor * the_visitor) 
{
  the_visitor->at_for(this);
}

void forNode::walk(Walker & the_walker)
{
  Walker::Order ord = the_walker.order(); 

  if (ord == Walker::Preorder || ord == Walker::Both)
    the_walker.at_for(this, Walker::Preorder);

  if (the_walker.depth() == Walker::Subtree) {
    // -- Visit the children 

    if (init())
      init()->walk(the_walker);

    if (cond())
      cond()->walk(the_walker);

    if (next())
      next()->walk(the_walker);

    if (body())
      body()->walk(the_walker);
  }

  if (ord == Walker::Postorder || ord == Walker::Both)
    the_walker.at_for(this, Walker::Postorder);
}

// ------------------------------------------------------------
// Dataflow
// ------------------------------------------------------------

void forNode::dataflow(FlowVal * v, FlowProblem & fp)
{
  if (fp.forward()) {
    fp.flow_for(v, this, FlowProblem::Entry);

    if (init())
      if (fp.basicblocks()) {
	fp.flow_basicblock(v, init(), FlowProblem::Entry);
	fp.flow_basicblock(v, init(), FlowProblem::Exit);
      }
      else
	init()->dataflow(v, fp);
    
    // -- Get the values from the previous loop iteration

    v->meet(at_loop_head());

    if (cond())
      if (fp.basicblocks()) {
	fp.flow_basicblock(v, cond(), FlowProblem::Entry);
	fp.flow_basicblock(v, cond(), FlowProblem::Exit);
      }
      else
	cond()->dataflow(v, fp);
    
    // -- Dataflow the loop body (collect break values in at_exit and
    // continue values in at_loop_tail)

    FlowVal * bv = v->clone();

    if (body())
      body()->dataflow(bv, fp);

    // -- Get the collected continue values

    bv->meet(at_loop_tail());

    if (next())
      if (fp.basicblocks()) {
	fp.flow_basicblock(v, next(), FlowProblem::Entry);
	fp.flow_basicblock(v, next(), FlowProblem::Exit);
      }
      else
	next()->dataflow(v, fp);
    
    // -- Transfer values at the end of the loop to the loop head

    at_loop_head()->meet_and_diff(bv, fp);
    delete bv;

    // -- If there is no condition, then flow cannot go directly from
    // the cond to the exit.

    if (! cond())
      v->to_top();

    // -- Get the collected break values

    v->meet(at_exit());

    fp.flow_for(v, this, FlowProblem::Exit);
  }
  else {
    fp.flow_for(v, this, FlowProblem::Exit);

    // -- at_exit holds the values need by breaks

    at_exit()->meet_and_diff(v, fp);

    // -- Dataflow the loop

    FlowVal * bv = at_loop_head()->clone();

    if (next())
      if (fp.basicblocks()) {
	fp.flow_basicblock(v, next(), FlowProblem::Exit);
	fp.flow_basicblock(v, next(), FlowProblem::Entry);
      }
      else
	next()->dataflow(v, fp);

    // -- Save the loop_tail values needed by continue and break

    at_loop_tail()->meet_and_diff(bv, fp);

    // -- Dataflow the body (breaks and continues read values from
    // at_exit and at_loop_end, respectively).

    if (body())
      body()->dataflow(bv, fp);

    // -- If there is no condition, then flow cannot go directly from
    // the cond to the exit.

    if (! cond())
      v->to_top();

    // -- Merge in the loop body values

    v->meet(bv);
    delete bv;

    if (cond())
      if (fp.basicblocks()) {
	fp.flow_basicblock(v, cond(), FlowProblem::Exit);
	fp.flow_basicblock(v, cond(), FlowProblem::Entry);
      }
      else
	cond()->dataflow(v, fp);

    // -- Save the values for the next loop iteration

    at_loop_head()->meet_and_diff(v, fp);

    if (init())
      if (fp.basicblocks()) {
	fp.flow_basicblock(v, init(), FlowProblem::Exit);
	fp.flow_basicblock(v, init(), FlowProblem::Entry);
      }
      else
	init()->dataflow(v, fp);

    fp.flow_for(v, this, FlowProblem::Entry);
  }
}

// ------------------------------------------------------------
// Output
// ------------------------------------------------------------

void forNode::output_stmt(output_context & ct, Node * parent)
{
  ct << "for " << '(';

  if (init())
    init()->output(ct, this);

  ct.space();
  ct << ';';
  ct.space();

  if (cond())
    cond()->output(ct, this);

  ct.space();
  ct << ';';
  ct.space();

  if (next())
    next()->output(ct, this);

  ct.space();
  ct << ')';
  ct.space();

  if (body())
    body()->output(ct, this);
  else
    ct << ';';
}

// ------------------------------------------------------------
//  Changer
// ------------------------------------------------------------

Node * forNode::change(Changer & the_changer, bool redispatch)
{
  Changer::Order ord = the_changer.order(); 
  forNode * the_for = this;

  if ((ord == Changer::Preorder || ord == Changer::Both) && ! redispatch)
    the_for = (forNode *) the_changer.at_for(the_for, Changer::Preorder);

  if (the_for) {

    if (the_for != this)
      return the_for->change(the_changer, true);

    exprNode * old_init = the_for->init();
    if (old_init) {
      exprNode * new_init = (exprNode *) old_init->change(the_changer);
      if (old_init != new_init) {
	//if (the_changer.delete_old())
	  //delete old_init;
        the_for->init(new_init);
      }
    }

    exprNode * old_cond = the_for->cond();
    if (old_cond) {
      exprNode * new_cond = (exprNode *) old_cond->change(the_changer);
      if (old_cond != new_cond) {
	//if (the_changer.delete_old())
	  //delete old_cond;
        the_for->cond(new_cond);
      }
    }

    exprNode * old_next = the_for->next();
    if (old_next) {
      exprNode * new_next = (exprNode *) old_next->change(the_changer);
      if (old_next != new_next) {
	//if (the_changer.delete_old())
	  //delete old_next;
        the_for->next(new_next);
      }
    }

    blockNode * old_body = the_for->body();
    if (old_body) {
      blockNode * new_body = (blockNode *) old_body->change(the_changer);
      if (old_body != new_body) {
	//if (the_changer.delete_old())
	  //delete old_body;
        the_for->body(new_body);
      }
    }

  }

  if ((ord == Changer::Postorder || ord == Changer::Both) && ! redispatch)
    the_for = (forNode *) the_changer.at_for(the_for, Changer::Postorder);

  return the_for;
}


// ------------------------------------------------------------
// Destructor
// ------------------------------------------------------------

forNode::~forNode()
{
  //delete _init;
  //delete _next;
}
