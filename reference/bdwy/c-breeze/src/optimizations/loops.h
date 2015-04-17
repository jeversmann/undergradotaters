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

#ifndef CBZ_LOOPS_H
#define CBZ_LOOPS_H

#include <set>

typedef set< basicblockNode * > basicblock_set;
typedef basicblock_set::iterator basicblock_set_p;

typedef map< basicblockNode *, basicblock_set > edge_map;
typedef edge_map::iterator edge_map_p;

typedef map< basicblockNode *, basicblock_set > basicblock_set_map;
typedef basicblock_set_map::iterator basicblock_set_map_p;

typedef map< basicblockNode *, int > basicblock_int_map;
typedef basicblock_int_map::iterator basicblock_int_map_p;

class loopTreeNode;

typedef list< loopTreeNode * > loop_list;
typedef loop_list::iterator loop_list_p;

typedef set< loopTreeNode * > loop_set;
typedef loop_set::iterator loop_set_p;

class loopTreeNode
{
public:

  /** @brief Types of loops */

  typedef enum { SingleEntry, MultipleEntry, Top } LoopKind;

private:

  /** @brief Kind of loop */

  LoopKind _kind;

  /** @brief Loop depth (zero is the top) */

  int _depth;

  /** @brief The loop header */

  basicblockNode * _header;

  /** @brief The loop preheader */

  basicblockNode * _preheader;

  /** @brief The containing loop */

  loopTreeNode * _parentLoop;

  /** @brief The set of nested loops */

  loop_set _nestedLoops;

  /** @brief The set of basic blocks */

  basicblock_set _blocks;

public:

  /** @brief Create a new loop */

  loopTreeNode(LoopKind kind, basicblockNode * header);

  /** @brief Add a block to the loop */

  inline void addBlock(basicblockNode * block) { _blocks.insert(block); }

  /** @brief Add a nested loop to the loop */

  inline void addNestedLoop(loopTreeNode * nest){ _nestedLoops.insert(nest); }

  /** @brief Accessors */

  inline LoopKind kind() const { return _kind; }

  inline int depth() const { return _depth; }
  inline void depth(int d) { _depth = d; }

  inline basicblockNode * header() const { return _header; }

  inline basicblockNode * preheader() const { return _preheader; }

  inline loopTreeNode * parentLoop() const { return _parentLoop; }
  inline void parentLoop(loopTreeNode * parent) { _parentLoop = parent; }

  inline loop_set & nestedLoops() { return _nestedLoops; }

  inline basicblock_set & blocks() { return _blocks; }

  /** @brief Report */

  void report();
};

typedef map< basicblockNode *, loopTreeNode * > blockloop_map;
typedef blockloop_map::iterator blockloop_map_p;

class loopTree
{
public:

  typedef enum { TreeEdge, BackEdge, ForwardEdge, CrossEdge } EdgeKind;

private:

  /** @brief The procedure */

  procNode * _procedure;

  /** @brief The loop tree */

  loopTreeNode * _top;

  /** @brief A list of the loops */

  loop_list _loops;

  /** @brief Mapping from basic blocks to loops */

  blockloop_map _containedIn;

  /** @brief List of back edges
   *
   * Back edges are stored according to the *target* of the back-edge, not
   * the source. This is because the algorithm needs to easily find all the
   * back-edges that impinge on a candidate node. */

  edge_map _backEdges;

  /** @brief List of forward edges
   *
   *  Stored according to the source of the edge */

  edge_map _forwardEdges;

  /** @brief List of cross edges
   *
   *  Stored according to the source of the edge */

  edge_map _crossEdges;

  /** @brief Depth-first pre-order of the nodes */

  basicblock_int_map _preOrder;

  /** @brief Depth-first reverse-post-order of the nodes */

  basicblock_int_map _rpostOrder;

public:

  /** @brief Create a loop tree */

  loopTree(procNode * procedure);

  /** @brief Delete a loop tree
   *
   * This destructor is reponsible for deleting all of the loop tree nodes
   * as well. */

  ~loopTree();

  /** @brief Procedure */

  inline procNode * procedure() const { return _procedure; }

  /** @brief Top loop object
   *
   * Note that this is not really a loop; it's just used as the root of the
   * tree */

  inline loopTreeNode * topLoop() const { return _top; }

  /** @brief Which loop
   *
   * Determine which loop a particular basic block belongs to. */

  loopTreeNode * whichLoop(basicblockNode * block);

  /** @brief Find a loop ancestor */

  loopTreeNode * loopAncestor(loopTreeNode * loop);

  /** @brief Edge classification
   *
   * Given a particular source and target in the control-flow graph,
   * indicate which kind of edge it is: tree, forward, backward, or
   * cross. */

  EdgeKind classifyEdge(basicblockNode * source,
			basicblockNode * target);

  /** @brief Report */

  void report();

private:

  /** @brief Decide if a block is a loop header */

  void findLoop(basicblockNode * candidate,
		basicblock_set_map & generators_map);

  /** @brief Determine the body of a loop */

  void findBody(basicblockNode * head,
		basicblock_set & generators,
		loopTreeNode::LoopKind kind);

  /** @brief Find a common dominator for two blocks */

  basicblockNode * commonDominator(basicblockNode * first,
				   basicblockNode * second);

  /** @brief Classify edges
   *
   *  Perform a depth-first search of the control-flow graph, building
   *  pre-order and reverse-post-order orderings of the nodes, as well as
   *  classifying th edges. Also, return a list with the post-order
   *  traversal. */

  void classifyEdges(basicblock_list & post_order);

  /** @brief Depth-first search */

  void depthFirstSearch(basicblockNode * cur,
			basicblock_list & post_order,
			int & pre_order_number,
			int & post_order_number);

  /** @brief Set the depth on each loop nest */

  void setDepths(loopTreeNode * cur, int depth);

  /** @brief Report (tree recursion) */

  void report(loopTreeNode * cur, int level);

};

#endif // CBZ_LOOPS_H

