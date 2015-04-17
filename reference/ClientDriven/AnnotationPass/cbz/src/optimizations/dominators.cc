// $Id: dominators.cc,v 1.11 2005/06/16 09:05:01 akkartik Exp $
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
#include "dominators.h"

basicblockNode * Dominators::None = 0;

Dominators::Dominators(procNode * proc, bool forward)
  : _proc(proc),
    _root(0),
    _forward(forward),
    _index(0)
{
  // --- Make sure there aren't too many basic blocks for the bitset

  if (proc->body()->stmts().size() > BASICBLOCK_BITSET_SIZE) {
    cerr << "INTERNAL ERROR: trying to compute dominators for \"" << proc->decl()->name() << "\"" << endl;
    cerr << "   Too many basic blocks (" << proc->body()->stmts().size() << "); limit is " << BASICBLOCK_BITSET_SIZE << endl;
    return;
  }

  // --- Make sure the "None" node has been created

  if (1 || None == 0) { // Stuff inside None can become stale, so 
			// we always re-make it -- djimenez
    None = new basicblockNode(0,0);
    None->info(new dominator_info(None, 0));
  }

  if (forward)
    _root = proc->entry();
  else
    _root = proc->exit();

  // --- Compute

  dominator_tree();

  // print();

  int size = df_vec.size();

  // --- Remove all the dominator_info structures

  for (int i = 1; i < size; i++) {
    basicblockNode * curblock = df_vec[i];

    if (curblock->info())
      delete curblock->info();

    curblock->info(0);
  }
}

// ------------------------------------------------------------
// Fast version (Muchnick)

#define Label(n)    ((dominator_info *)(n)->info())->label
#define Parent(n)   ((dominator_info *)(n)->info())->parent
#define Ancestor(n) ((dominator_info *)(n)->info())->ancestor
#define Child(n)    ((dominator_info *)(n)->info())->child
#define Sdno(n)     ((dominator_info *)(n)->info())->sdno
#define Size(n)     ((dominator_info *)(n)->info())->size
#define Bucket(n)   ((dominator_info *)(n)->info())->bucket

dominator_info::dominator_info(basicblockNode * node, int depth_first_num)
  : label(node),
    parent(Dominators::None),
    ancestor(Dominators::None),
    child(Dominators::None),
    sdno(depth_first_num),
    size(1),
    bucket()
{}

void Dominators::dominator_tree()
{
  int num_nodes;

  df_vec.push_back(None);

  // --- Initialize the data structures
  
  _index = 0;
  depth_first_search(_root, None);

  for (stmt_list_p sp = _proc->body()->stmts().begin();
        sp != _proc->body()->stmts().end();
        ++sp) {
    basicblockNode * bb = static_cast<basicblockNode*>(*sp);
    bb->parent(None);
  }

  // --- Main loop

  num_nodes = df_vec.size() - 1;

  for (int i = num_nodes; i > 1; i--) {
    // cout << "[" << i << "] ---------- " << endl;

    // --- Retrieve the current node

    basicblockNode * w = df_vec[i];

    // cout << "Dfn = " << w->dfn() << " parent = " << Parent(w)->dfn() << endl;

    // --- For all predecessors, 

    basicblock_list & bbl = (_forward) ? w->preds() : w->succs();

    for (basicblock_list_p p = bbl.begin();
	 p != bbl.end();
	 ++p)
      {
	basicblockNode * v = *p;

	// cout << "Eval(" << v->dfn() << ") = ";
	basicblockNode * u = eval(v);
	// cout << u->dfn() << endl;
	
	// cout << "Test Sdno(" << Dfn(u) << ") = " << Sdno(u) << " < "
	//  "Test Sdno(" << Dfn(w) << ") = " << Sdno(w) << endl;

	if (Sdno(u) < Sdno(w))
	  Sdno(w) = Sdno(u);
      }

    basicblockNode * semi_dom = df_vec[Sdno(w)];
    Bucket(semi_dom).set(i);
    // cout << "Add " << i << " to bucket of " << Dfn(semi_dom) << endl;
    link(Parent(w), w);

    // print();

    for (int j = 1; j <= num_nodes; j++) {
      if (Bucket(Parent(w)).test(j) != 0) {
	basicblockNode * v = df_vec[j];
	basicblockNode * u = eval(v);

	//	cout << " in bucket : " << Dfn(v) << endl;

	// cout << "Test Sdno(" << Dfn(u) << ") = " << Sdno(u) << " < "
	//  "Test Sdno(" << Dfn(v) << ") = " << Sdno(v) << endl;

	if (Sdno(u) < Sdno(v))
	  v->parent(u);
	else
	  v->parent(Parent(w));
      }
    }
  }

  for (int i = 2; i <= num_nodes; i++) {
    basicblockNode * w = df_vec[i];
    basicblockNode * semi_dom = df_vec[Sdno(w)];

    if (w->parent() != semi_dom)
      w->parent(w->parent()->parent());

    // --- Add to the children pointer

    w->parent()->children().push_back(w);
  }
}

void Dominators::depth_first_search(basicblockNode * node,
				    basicblockNode * par)
{
  if (node->info() == 0) {
    ++_index;

    node->info(new dominator_info(node, _index));
    node->dfn(_index);
    node_index[node] = _index;
    df_vec.push_back(node);

    Parent(node) = par;

    // --- Check the ordering:
    //       _forward = true  --> successors   --> dominators
    //       _forward = false --> predecessors --> post-dominators

    basicblock_list & bbl = (_forward) ? node->succs() : node->preds();

    for (basicblock_list_p p = bbl.begin();
	 p != bbl.end();
	 ++p)
      {
	basicblockNode * curblock = *p;
	depth_first_search(curblock, node);
      }
  }
}
  
void Dominators::compress(basicblockNode * v)
{
  basicblockNode * anc = Ancestor(v);

  if (Ancestor(Ancestor(v)) != None) {
    compress(Ancestor(v));

    if (Sdno(Label(Ancestor(v))) < Sdno(Label(v)))
      Label(v) = Label(Ancestor(v));

    Ancestor(v) = Ancestor(Ancestor(v));
  }
}

basicblockNode * Dominators::eval(basicblockNode * v)
{
  if (Ancestor(v) == None)
    return Label(v);
  else {
    compress(v);

    if (Sdno(Label(Ancestor(v))) >= Sdno(Label(v)))
      return Label(v);
    else
      return Label(Ancestor(v));
  }
}

void Dominators::link(basicblockNode * v, basicblockNode * w)
{
  basicblockNode * s = w;

  while (Sdno(Label(w)) < Sdno(Label(Child(s)))) {

    if (((Size(s) + Size(Child(Child(s))))) >=
	2 * (Size(Child(s)))) {
      Ancestor(Child(s)) = s;
      Child(s) = Child(Child(s));
    }
    else {
      Size(Child(s)) = Size(s);
      s = Ancestor(s) = Child(s);
    }
  }

  Label(s) = Label(w);
  Size(v) += Size(w);

  if (Size(v) < 2 * (Size(w))) {
    basicblockNode * tmp = s;
    s = Child(v);
    Child(v) = tmp;
  }

  while (s != None) {
    Ancestor(s) = v;
    s = Child(s);
  }
}

void Dominators::print()
{
  output_context out(cout);
  int size = df_vec.size();

  for (int i = 1; i < size; i++) {
    basicblockNode * curblock = df_vec[i];
    basicblockNode * parent = curblock->parent();

    ostringstream com;

    com << "BB " << curblock->dfn() << " idom = ";
    if (parent)
      com << curblock->parent()->dfn();
    else
      com << "(no idom)";
    // com << " Sdno = " << Sdno(curblock);

    // Also show the CFG

    com << "\tPreds: ";
    if (curblock->preds().empty())
      com << "(none)";

    for (basicblock_list_p l = curblock->preds().begin();
	 l != curblock->preds().end(); l++)
      com << (*l)->dfn() << " ";

    com << "\tSuccs: ";
    if (curblock->succs().empty())
      com << "(none)";

    for (basicblock_list_p l = curblock->succs().begin();
	 l != curblock->succs().end(); l++)
      com << (*l)->dfn() << " ";

    curblock->comment() = com.str();

    cout << com.str() << endl;
  }
}

Dominators::~Dominators()
{
}

// --- Test to see if A dominates B : return true if A is in the chain
// of immediate dominators starting at B.

bool Dominators::dominates(basicblockNode * A, basicblockNode * B)
{
  basicblockNode * par = B;

  if ((A == 0) || (B == 0))
    return false;

  if (A == B)
    return true;

  do
    par = par->parent();
  while (par && (par != A));

  return (par == A);
}


