// $Id: dominancefrontiers.h,v 1.9 2003/08/07 23:14:13 pnav Exp $
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

#ifndef CBZ_DOMINANCEFRONTIERS_H
#define CBZ_DOMINANCEFRONTIERS_H

#include <bitset>
#include "ref_clone_changer.h"
#include "dominators.h"


// DominanceFrontiers dfs(my_proc);
// basicblock_list & dfx = dfs[x];

// From dominators: typedef bitset<1024> basicblock_bitset;

typedef map< basicblockNode *, basicblock_bitset > basicblockset_map;
typedef basicblockset_map::iterator basicblockset_map_p;

typedef map< basicblockNode *, basicblock_list> basicblock_map;
typedef basicblock_map::iterator basicblock_map_p;

typedef vector<basicblockNode *> basicblock_vec;
typedef basicblock_vec::iterator basicblock_vec_p;

/** @brief create a dominance frontier for a procedure
    @ingroup Phases

    The DominanceFrontier class compute the dominance frontier for the
    procedure given in its constructor. The class is a subclass of
    basicblock_map, which maps a basic block to the list containing the
    elements of its dominance frontier. Thus, it may be accessed
    directly like this:
    
    DominanceFrontiers dfs(my_proc);
    basicblock_list & dfx = dfs[x];
*/
class DominanceFrontiers : public basicblock_map
{
private:

  /// The procedure to analyze
  procNode * _proc;

  /// The root node
  basicblockNode * _root;

  /// Basic blocks in depth first order
  basicblock_vec df_vec;


  void depth_first_search(basicblockNode * node);
  void compute_dominance_frontiers();
  int _index;

public:

  DominanceFrontiers(procNode * proc);
};

#endif // 
