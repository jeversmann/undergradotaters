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

#ifndef CBZ_DFPREDS_H
#define CBZ_DFPREDS_H

#include <set>
#include "pointeroptions.h"
#include "ref_clone_changer.h"
#include "dominators.h"

// The DominanceFrontier class compute the dominance frontier for the
// procedure given in its constructor. This version of the algorithm
// also computes the particular set of predecessors of each element of
// the dominance frontier. For example:

//        _____3_______
//       /             \
//  1 __/               >--6
//      \       __5____/
//       \__2__/      /
//             \__4__/

// In the above graph, if we have a def at (2), the dominance frontier
// is {6}. However, we also want to record the fact that the
// particular predecessors of (6) that (2) dominates are {4,5}. This
// will allow us to put the reaching definition directly into the
// appropriate entry of the phi function at (6).

// Store the dominance frontier entries (with predecessor sets)

typedef map< basicblockNode *, basicblock_set > basicblock_set_map;
typedef basicblock_set_map::iterator basicblock_set_map_p;

// Store all the dominance frontiers

typedef map< basicblockNode *, basicblock_set_map > basicblock_set_map_map;
typedef basicblock_set_map_map::iterator basicblock_set_map_map_p;

/*
typedef map< basicblockNode *, basicblock_set > basicblockset_map;
typedef basicblockset_map::iterator basicblockset_map_p;

typedef map< basicblockNode *, basicblock_list> basicblock_map;
typedef basicblock_map::iterator basicblock_map_p;
*/

typedef vector<basicblockNode *> basicblock_vec;
typedef basicblock_vec::iterator basicblock_vec_p;

class DFPreds : public basicblock_set_map_map
{
private:

  // --- The procedure to analyze

  procNode * _proc;

  // --- The root node

  basicblockNode * _root;

  // --- Basic blocks in depth first order

  basicblock_vec df_vec;

  // --- Compute

  void depth_first_search(basicblockNode * node);
  void compute_dominance_frontiers();
  int _index;

public:

  DFPreds(procNode * proc);
};

#endif // 
