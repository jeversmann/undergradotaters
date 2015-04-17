// $Id: blocknode.cc,v 1.6 2003/08/07 23:12:59 pnav Exp $
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

blockNode::blockNode(decl_list * decls, stmt_list * stmts,
		     const Coord left_coord, const Coord right_brace)
  : stmtNode(Block, left_coord),
    _decls(),
    _stmts(),
    _right_brace(right_brace)
{
  if (decls) {
    _decls.swap(* decls);
    //delete decls;
  }

  if (stmts) {
    _stmts.swap(* stmts);
    //delete stmts;
  }
}

// ------------------------------------------------------------
// toBlock is used in the parser to ensure that statement blocks
// with only one statement (and thus without braces) are properly
// converted to blocks.
// ------------------------------------------------------------

blockNode * blockNode::toBlock(stmtNode *stmt, Coord coord)
{
  if (stmt && (stmt->typ() != Block)) {
    stmt_list * stmts = new stmt_list();
    stmts->push_front(stmt);
    return new blockNode((decl_list *)0, stmts, coord);
  }
  else
    return (blockNode *)stmt;
}

// ------------------------------------------------------------
// Data type base
// ------------------------------------------------------------

typeNode * blockNode::base_type(bool TdefIndir) const
{
  return (typeNode *) 0;
}

// ------------------------------------------------------------
//  Walker
// ------------------------------------------------------------

void blockNode::visit(Visitor * the_visitor) 
{
  the_visitor->at_block(this);
}

void blockNode::walk(Walker & the_walker)
{
  Walker::Order ord = the_walker.order(); 

  if (ord == Walker::Preorder || ord == Walker::Both)
    the_walker.at_block(this, Walker::Preorder);

  if (the_walker.depth() == Walker::Subtree) {
    // -- Visit the children 

    list_walker(decls(), the_walker);
    list_walker(stmts(), the_walker);

  }

  if (ord == Walker::Postorder || ord == Walker::Both)
    the_walker.at_block(this, Walker::Postorder);
}

// ------------------------------------------------------------
//  Dataflow
// ------------------------------------------------------------

void blockNode::dataflow(FlowVal * v, FlowProblem & fp)
{
  if (fp.forward()) {
    fp.flow_block(v, this, FlowProblem::Entry);

    dataflow_forward_list(decls(), v, fp);
    dataflow_forward_list(stmts(), v, fp);

    fp.flow_block(v, this, FlowProblem::Exit);
  }
  else {
    fp.flow_block(v, this, FlowProblem::Exit);

    dataflow_reverse_list(stmts(), v, fp);
    dataflow_reverse_list(decls(), v, fp);

    fp.flow_block(v, this, FlowProblem::Entry);
  }
}

// ------------------------------------------------------------
// Output
// ------------------------------------------------------------

void blockNode::output_stmt(output_context & ct, Node * parent)
{
  int ds = decls().size();
  int ss = stmts().size();
  int l = ds + ss;
  NodeType parent_type;

  if (parent)
    parent_type = parent->typ();
  else
    parent_type = Proc;

  if ((parent_type == Block) ||
      (parent_type == Proc))
    ct.new_line();

  if ((l != 1) || (parent_type == Proc) || (parent_type == If)) {
    ct << '{';
    CBZ::current_unit->enter_scope();
  }

  ct.indent_in();

  output_comment(ct);

  output_list(decls(), ct, this);

  if ((ds > 0) && (ss > 0))
    ct.new_line();

  output_list(stmts(), ct, this);

  ct.indent_out();

  if ((l != 1) || (parent_type == Proc) || (parent_type == If)) {

    // an suespec that was output at this scope level should
    // be marked as having not been output, because it has gone
    // out of scope. -- djimenez

#ifdef FOOO
    // oops.  this causes us to get an error in spec vortex.
    for (suespec_list_p p=CBZ::current_unit->suespecs().begin();
	    p!=CBZ::current_unit->suespecs().end(); p++) {
		if ((*p)->scope_output() >= CBZ::current_unit->symbol_level()) {
			(*p)->already_output(false);
		}
    }
#endif
    CBZ::current_unit->exit_scope();
    ct.new_line();
    ct << '}';
  }
}

// ------------------------------------------------------------
//  Changer
// ------------------------------------------------------------

Node * blockNode::change(Changer & the_changer, bool redispatch)
{
  Changer::Order ord = the_changer.order(); 
  blockNode * the_block = this;

  if ((ord == Changer::Preorder || ord == Changer::Both) && ! redispatch)
    the_block = (blockNode *) the_changer.at_block(the_block, Changer::Preorder);

  if (the_block) {

    if (the_block != this)
      return the_block->change(the_changer, true);

    change_list(the_block->decls(), the_changer);

    change_list(the_block->stmts(), the_changer);
  }

  if ((ord == Changer::Postorder || ord == Changer::Both) && ! redispatch)
    the_block = (blockNode *) the_changer.at_block(the_block, Changer::Postorder);

  return the_block;
}


// ------------------------------------------------------------
// Destructor
// ------------------------------------------------------------

blockNode::~blockNode()
{
  //delete_list(_decls);
  //delete_list(_stmts);
}
