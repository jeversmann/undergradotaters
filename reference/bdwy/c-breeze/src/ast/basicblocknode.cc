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

basicblockNode::basicblockNode(decl_list * decls, stmt_list * stmts,
		     const Coord left_coord, const Coord right_brace)
  : blockNode (decls, stmts, left_coord, right_brace),
    _parent(0),
    _info(0),
    _dfn(0),
    _at_entry(0),
    _at_exit(0)
{}

// ------------------------------------------------------------
// Destructor
// ------------------------------------------------------------

// nothing to do; if we delete_list the preds and succs, we might
// blow away other basic blocks.

basicblockNode::~basicblockNode() { }

void basicblockNode::visit(Visitor * the_visitor)
{
  the_visitor->at_basicblock(this);
}

void basicblockNode::walk(Walker & the_walker)
{
  Walker::Order ord = the_walker.order();

  if (ord == Walker::Preorder || ord == Walker::Both)
    the_walker.at_basicblock(this, Walker::Preorder);

  if (the_walker.depth() == Walker::Subtree) {
    // -- Visit the children

    list_walker(decls(), the_walker);
    list_walker(stmts(), the_walker);

  }

  if (ord == Walker::Postorder || ord == Walker::Both)
    the_walker.at_basicblock(this, Walker::Postorder);
}

Node * basicblockNode::change(Changer & the_changer, bool redispatch)
{
  Changer::Order ord = the_changer.order();
  basicblockNode * the_basicblock = this;

  if ((ord == Changer::Preorder || ord == Changer::Both) && ! redispatch)
    the_basicblock = (basicblockNode *) the_changer.at_basicblock(the_basicblock, Changer::Preorder)
;

  if (the_basicblock) {

    if (the_basicblock != this)
      return the_basicblock->change(the_changer, true);

    change_list(the_basicblock->decls(), the_changer);

    change_list(the_basicblock->stmts(), the_changer);
  }

  if ((ord == Changer::Postorder || ord == Changer::Both) && ! redispatch)
    the_basicblock = (basicblockNode *) the_changer.at_basicblock(the_basicblock, Changer::Postorder
);
  return the_basicblock;
}
