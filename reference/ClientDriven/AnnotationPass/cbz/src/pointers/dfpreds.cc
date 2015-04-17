// $Id: dfpreds.cc,v 1.4 2003/08/07 23:14:22 pnav Exp $
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
#include "dfpreds.h"

// ------------------------------------------------------------
// Constructor
// ------------------------------------------------------------

DFPreds::DFPreds(procNode * proc)
  : basicblock_set_map_map(),
    _proc(proc),
    _root(0),
    df_vec(),
    _index(0)
{
  _root = proc->entry();
  assert(_root->typ() == Block);

  // cout << "Compute DF for " << proc->decl()->name() << endl;

  Dominators dom(proc, true);

  depth_first_search(_root);

  compute_dominance_frontiers();
}

// --- Compute a depth-first ordering of the basic blocks. Store that
// information on each basic block in "dfn".

void DFPreds::depth_first_search(basicblockNode * node)
{
  if (std::find(df_vec.begin(), df_vec.end(), node) == df_vec.end()) {
    
    node->dfn(_index);
    df_vec.push_back(node);

    ++_index;

    // --- Check the ordering:
    //       _forward = true  --> successors   --> dominators
    //       _forward = false --> predecessors --> post-dominators

    basicblock_list & bbl = node->succs();

    for (basicblock_list_p p = bbl.begin();
	 p != bbl.end();
	 ++p)
      {
	basicblockNode * curblock = *p;
	depth_first_search(curblock);
      }
  }
}

// ------------------------------------------------------------
// Build dominance frontiers
// ------------------------------------------------------------

void DFPreds::compute_dominance_frontiers()
{
  basicblock_set_map_map & DF = *this;

  int num_nodes = df_vec.size();

  for (int i = num_nodes - 1; i >= 0; i--) {
    basicblockNode * X = df_vec[i];

    DF[X] = basicblock_set_map();

    basicblock_list & bbl = X->succs();

    // --- Compute DF_local: any immediate successors Y that are not
    // dominated by X, or if Y and X are the same node.

    // cout << "  Compute DF_local for BB " << X->dfn() << endl;

    for (basicblock_list_p p = bbl.begin();
	 p != bbl.end();
	 ++p)
      {
	basicblockNode * Y = *p;

	if ((X == Y) ||
	    ( ! Dominators::dominates(X, Y))) {
	  DF[X][Y] = basicblock_set();
	  DF[X][Y].insert(X);
	  // cout << "    add " << Y->dfn() << endl;
	}
      }

    // --- Compute DF_up: any nodes in the dominance frontier of Xs
    // children, unless X happens to dominate them.

    // For all children of X
    //cout << "  Compute DF_up..." << endl;

    for (basicblock_list_p p = X->children().begin();
	 p != X->children().end();
	 ++p)
      {
	basicblockNode * childX = *p;

	// For all elements of DF child

	for (basicblock_set_map_p q = DF[childX].begin();
	     q != DF[childX].end();
	     ++q)
	  {
	    basicblockNode * dfchildX = (*q).first;
	    basicblock_set & preds_of_dfchildX = (*q).second;

	    if ((X == dfchildX) ||
		(! Dominators::dominates(X, dfchildX))) {

	      // Found a DF element for X -- see if it's already there
	      // or not. This affects how we update the predecessor
	      // set.

	      // cout << "    add " << dfchildX->dfn() << endl;

	      basicblock_set_map_p w = DF[X].find(dfchildX);
	      if (w == DF[X].end())
		DF[X][dfchildX] = preds_of_dfchildX;
	      else {

		// Merge in the preds from the new DF element

		for (basicblock_set_p z = preds_of_dfchildX.begin();
		     z != preds_of_dfchildX.end();
		     z++)
		  DF[X][dfchildX].insert(*z);
	      }
	    }
	  }
      }

    // ostringstream ost;
    // ost << "DF[" << X->dfn() << "] = ";
    // output_context oc(cout);
    // if (! X)
    //  cout << "X is null" << endl;
    // else
    //  X->output(oc,0);
    // for (int j = 0; j < num_nodes; j++)
    //  if (DF[X].test(j))
    //   ost << j << " ";
    // X->comment() = ost.str();

    /*
    {
      char buf1[2000];
      char buf2[300];
      

      sprintf(buf1, " %d : DF = { ", X->dfn());
      for (basicblock_set_map_p p = DF[X].begin();
	   p != DF[X].end();
	   ++p)
	{
	  sprintf(buf2, " %d ", (*p).first->dfn());
	  strcat(buf1, buf2);
	}
      strcat(buf1, " } \n");

      sprintf(buf2, " DOM = { ");
      strcat(buf1, buf2);
      for (basicblock_list_p p = X->children().begin();
	   p != X->children().end();
	   ++p)
	{
	  basicblockNode * childX = *p;
	  sprintf(buf2, " %d ", childX->dfn());
	  strcat(buf1, buf2);
	}
      strcat(buf1, " } \n");

      sprintf(buf2, " PRED = { ");
      strcat(buf1, buf2);
      for (basicblock_list_p p = X->preds().begin();
	   p != X->preds().end();
	   ++p)
	{
	  basicblockNode * predX = *p;
	  sprintf(buf2, " %d ", predX->dfn());
	  strcat(buf1, buf2);
	}
      strcat(buf1, " } \n");

      sprintf(buf2, " SUCC = { ");
      strcat(buf1, buf2);
      for (basicblock_list_p p = X->succs().begin();
	   p != X->succs().end();
	   ++p)
	{
	  basicblockNode * succX = *p;
	  sprintf(buf2, " %d ", succX->dfn());
	  strcat(buf1, buf2);
	}
      strcat(buf1, " } \n");

      X->comment() = string(buf1);
    }
    */
  }


  /* NOT NEEDED
  // --- Convert bitset representation into the list-oriented
  // representation.

  for (int i = 0; i < df_vec.size(); ++i) {

    // --- Get the current basic block

    basicblockNode * cur = df_vec[i];

    // --- Get the bit-vector representation of its DF

    basicblock_set & DFblock = DF[cur];

    // --- Add an empty list into "this" for the block

    pair<basicblockNode *, basicblock_list> el(cur, basicblock_list());
    pair<basicblock_map_p, bool> result = insert(el);

    // --- Get a reference to that list

    basicblock_list & bbl = ( * (result.first)).second;

    // --- Translate each bit into a basicblockNode and insert in the
    // list.

    for (int j = 0; j < num_nodes; j++)
      if (DFblock.test(j)) {
	basicblockNode * Y = df_vec[j];
	bbl.push_back(Y);
      }
  }
  */
}
