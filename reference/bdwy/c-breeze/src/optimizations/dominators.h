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

#ifndef CBZ_DOMINATORS_H
#define CBZ_DOMINATORS_H

#include <bitset>
#include "dismantle.h"

#define BASICBLOCK_BITSET_SIZE 4096

typedef bitset<BASICBLOCK_BITSET_SIZE> basicblock_bitset;

/** @brief information about a dominator node
 */

class dominator_info : public algorithm_info
{
public:

  // --- Used in computing dominator tree

  basicblockNode * label;
  basicblockNode * parent;
  basicblockNode * ancestor;
  basicblockNode * child;
  int sdno;
  int size;
  basicblock_bitset bucket;

  dominator_info(basicblockNode * node, int depth_first_num);
};



/** @brief dominator tree
 */

class Dominators
{
  friend class dominator_info;

public:
  // --- Some useful types

  typedef vector<basicblockNode *> basicblock_vec;
  typedef map<basicblockNode *, int> index_map;
  typedef map<basicblockNode *, basicblockNode *> basicblock_map;
  typedef map<basicblockNode *, basicblock_bitset> basicblockset_map;

private:

  /// Procedure 
  procNode * _proc;

  /// The root node
  basicblockNode * _root;

  /** @brief whether to build a dom-tree or a pdom-tree
      If true, build post-dominators information
      _forward = true  --> successors   --> dominators
      _forward = false --> predecessors --> post-dominators
  */
  const bool _forward;

  /// Map from basic blocks to df order
  index_map node_index;

  /// Basic blocks in depth first order
  basicblock_vec df_vec;

  /// A special basic block: not in any cfg
  static basicblockNode * None;

  int _index;

  // ----------------------------------------


  // --- Compute dominator tree
  
  void dominator_tree();
  void depth_first_search(basicblockNode * node,
			  basicblockNode * par);
  void compress(basicblockNode * v);
  basicblockNode * eval(basicblockNode * v);
  void link(basicblockNode * v, basicblockNode * w);

public:

  Dominators(procNode * proc, bool forward);

  ~Dominators();

  /// Check if A dominates B
  static bool dominates(basicblockNode * A, basicblockNode * B);


  void print();

};

#endif // CBZ_DOMINATORS_H
