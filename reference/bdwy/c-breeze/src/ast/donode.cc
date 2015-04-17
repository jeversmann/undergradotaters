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

doNode::doNode(stmtNode * body, exprNode * cond, const Coord coord,
	       const Coord while_coord)
  : loopNode(Do, cond, body, coord),
    _while_coord(while_coord)
{}

// ------------------------------------------------------------
//  Walker
// ------------------------------------------------------------

void doNode::visit(Visitor * the_visitor) 
{
  the_visitor->at_do(this);
}

void doNode::walk(Walker & the_walker)
{
  Walker::Order ord = the_walker.order(); 

  if (ord == Walker::Preorder || ord == Walker::Both)
    the_walker.at_do(this, Walker::Preorder);

  if (the_walker.depth() == Walker::Subtree) {
    // -- Visit the children 

    if (body())
      body()->walk(the_walker);

    if (cond())
      cond()->walk(the_walker);
  }

  if (ord == Walker::Postorder || ord == Walker::Both)
    the_walker.at_do(this, Walker::Postorder);
}

// ------------------------------------------------------------
//  Dataflow
// ------------------------------------------------------------

void doNode::dataflow(FlowVal * v, FlowProblem & fp)
{
  if (fp.forward()) {
    fp.flow_do(v, this, FlowProblem::Entry);

    // -- Get the values from the previous loop iteration

    v->meet(at_loop_head());

    // -- Dataflow the loop body (collect break values in at_exit and
    // continue values in at_loop_tail)

    if (body())
      body()->dataflow(v, fp);

    // -- Get the collected continue values

    v->meet(at_loop_tail());

    if (cond())
      if (fp.basicblocks()) {
	fp.flow_basicblock(v, cond(), FlowProblem::Entry);
	fp.flow_basicblock(v, cond(), FlowProblem::Exit);
      }
      else
	cond()->dataflow(v, fp);
    
    // -- Transfer values at the end of the loop to the loop head

    at_loop_head()->meet_and_diff(v, fp);

    // -- If there is no condition, then flow cannot go directly from
    // the cond to the exit.

    if (! cond())
      v->to_top();

    // -- Get the collected break values

    v->meet(at_exit());

    fp.flow_do(v, this, FlowProblem::Exit);
  }
  else {
    fp.flow_do(v, this, FlowProblem::Exit);

    // -- at_exit holds the values need by breaks

    at_exit()->meet_and_diff(v, fp);

    // -- Transfer values at loop head to the loop end

    at_loop_tail()->meet_and_diff(at_loop_head(), fp);

    // -- If there is no condition, then flow cannot go directly from
    // the cond to the exit.

    if (! cond())
      v->to_top();

    // -- Merge in the loop values

    v->meet(at_loop_tail());

    if (cond())
      if (fp.basicblocks()) {
	fp.flow_basicblock(v, cond(), FlowProblem::Exit);
	fp.flow_basicblock(v, cond(), FlowProblem::Entry);
      }
      else
	cond()->dataflow(v, fp);

    // -- Dataflow the body (breaks and continues read values from
    // at_exit and at_loop_end, respectively).

    if (body())
      body()->dataflow(v, fp);

    // -- Save the values for the next loop iteration

    at_loop_head()->meet_and_diff(v, fp);

    fp.flow_do(v, this, FlowProblem::Entry);
  }
}

// ------------------------------------------------------------
// Output
// ------------------------------------------------------------

void doNode::output_stmt(output_context & ct, Node * parent)
{
  ct << "do";
  ct.space();

  if (body())
    body()->output(ct, this);
  else
    ct << ';';

  ct.space();
  ct << "while " << '(' ;

  if (cond())
    cond()->output(ct, this);

  ct << ')' << ';' ;
}

// ------------------------------------------------------------
//  Changer
// ------------------------------------------------------------

Node * doNode::change(Changer & the_changer, bool redispatch)
{
  Changer::Order ord = the_changer.order(); 
  doNode * the_do = this;

  if ((ord == Changer::Preorder || ord == Changer::Both) && ! redispatch)
    the_do = (doNode *) the_changer.at_do(the_do, Changer::Preorder);

  if (the_do) {

    if (the_do != this)
      return the_do->change(the_changer, true);

    blockNode * old_body = the_do->body();
    if (old_body) {
      blockNode * new_body = (blockNode *) old_body->change(the_changer);
      if (old_body != new_body) {
	//if (the_changer.delete_old())
	  //delete old_body;
        the_do->body(new_body);
      }
    }

    exprNode * old_cond = the_do->cond();
    if (old_cond) {
      exprNode * new_cond = (exprNode *) old_cond->change(the_changer);
      if (old_cond != new_cond) {
	//if (the_changer.delete_old())
	  //delete old_cond;
        the_do->cond(new_cond);
      }
    }

  }

  if ((ord == Changer::Postorder || ord == Changer::Both) && ! redispatch)
    the_do = (doNode *) the_changer.at_do(the_do, Changer::Postorder);

  return the_do;
}


// ------------------------------------------------------------
// Destructor
// ------------------------------------------------------------

doNode::~doNode()
{
}
