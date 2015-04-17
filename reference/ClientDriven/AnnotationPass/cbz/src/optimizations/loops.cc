// $Id: loops.cc,v 1.7 2003/08/07 23:14:18 pnav Exp $
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

#include <set>
#include "c_breeze.h"
#include "dominators.h"
#include "loops.h"

// ----------------------------------------------------------------------
//   Loop Tree Node
// ----------------------------------------------------------------------

loopTreeNode::loopTreeNode(LoopKind kind, basicblockNode * header)
  : _kind(kind),
    _header(header),
    _preheader(0),
    _parentLoop(0),
    _nestedLoops(),
    _blocks()
{
  _blocks.insert(header);
}

void loopTreeNode::output(output_context & ct, Node * parent)
{
   output_stmt(ct, parent);
}

void loopTreeNode::output_stmt(output_context & ct, Node * parent)
{

   cout << "LoopTreeNode header: " << endl;
   _header->output(ct, NULL);

}


void loopTreeNode::report()
{
  switch (kind()) {
  case SingleEntry:
    cout << "Single entry: ";
    break;
  case MultipleEntry:
    cout << "Multiple entry: ";
    break;
  case Top:
    cout << "(Top): ";
    break;
  default:
    cout << "(ERROR): ";
    break;
  }

  cout << "Depth = " << depth() << ", ";

  cout << "Header is #" << header()->dfn();
  cout << ", blocks: ";

  for (basicblock_set_p p = blocks().begin();
       p != blocks().end();
       ++p)
    cout << "#" << (*p)->dfn() << " ";

  cout << endl;
}

// ----------------------------------------------------------------------
//   Loop Tree
// ----------------------------------------------------------------------

loopTree::loopTree(procNode * procedure)
  : _procedure(procedure),
    _top(0),
    _loops(),
    _containedIn(),
    _backEdges(),
    _forwardEdges(),
    _crossEdges()
{
  // -- Perform a depth-first search, classifying edges and building a
  // post-order list of basic blocks.

  basicblock_list post_order;
  classifyEdges(post_order);

  // -- Build the loop tree: visit the nodes in post-order (basically,
  // bottom up) finding loops and assembling them into the loop tree.

  basicblock_set_map generators_map;

  for (basicblock_list_p p = post_order.begin();
       p != post_order.end();
       ++p)
    {
      basicblockNode * cur = *p;

      // -- If the current block has generators, then it represents a
      // multiple entry loop that we previously found. The current node has
      // been chosen to represent the loop because it is the nearest node
      // that dominates all nodes in the loop.

      if ( ! generators_map[cur].empty() )
	findBody(cur, generators_map[cur], loopTreeNode::MultipleEntry);

      // -- Check to see if the current node is the entry point for a loop.

      findLoop(cur, generators_map);
    }

  // -- Create a fake top node for the tree

  basicblock_set just_exit;
  just_exit.insert(procedure->exit());

  findBody(procedure->entry(), just_exit, loopTreeNode::Top);

  // -- Set the depths of each loop tree node

  setDepths(_top, 0);
}

loopTree::~loopTree()
{
  // -- Delete all the loop tree nodes

  for (loop_list_p p = _loops.begin();
       p != _loops.end();
       ++p)
    delete *p;
}

loopTreeNode * loopTree::whichLoop(basicblockNode * block)
{
  blockloop_map_p p = _containedIn.find(block);
  if (p != _containedIn.end())
    return (*p).second;
  else
    return 0;
}

void loopTree::findLoop(basicblockNode * candidate,
			basicblock_set_map & generators_map)
{
  // -- A basic block is the entry to a loop if it has back-edges.

  edge_map_p p = _backEdges.find(candidate);

  if (p != _backEdges.end()) {

    // -- This is a loop, collect the loop information.

    // We collect the predecessors of this basic block that are in the loop
    // (the heads of the back-edges), which we call "generators". These are
    // used later to generate the loop body. We also compute the "header",
    // which is the common dominator for all nodes in the loop.

    basicblock_set & predecessors = (*p).second;

    // -- Start by assuming the header is the candidate itself

    basicblockNode * header = candidate;

    // -- Collect the generators into this set

    basicblock_set generators;

    // -- For each predecessor....

    for (basicblock_set_p q = predecessors.begin();
	 q != predecessors.end();
	 ++q)
      {
	basicblockNode * pred = *q;

	// -- Add it into the common dominator 

	header = commonDominator(header, pred);

	// -- Add it to the generators set

	if (pred != candidate)
	  generators.insert(pred);
      }

    // -- If the common dominator is the same as the candidate loop head,
    // then we have a single-entry loop. The reason is that if the
    // candidate node doesn't dominate all the nodes in the loop, then
    // there must be another entry point.

    if (header == candidate) {

      // -- Single-entry loop; find the body and create a loopTree entry
      // right now

      findBody(candidate, generators, loopTreeNode::SingleEntry);
    }
    else {

      // -- For multiple entry loops, we will construct the body later when
      // we encounter the header in the post-order traversal. We store the
      // generators in a map until then.

      generators_map[header].insert(generators.begin(), generators.end());
    }
  }
}

void loopTree::findBody(basicblockNode * head,
			basicblock_set & generators,
			loopTreeNode::LoopKind kind)
{
  // -- Collect the basic blocks in this loop

  basicblock_set loop_nodes;

  // -- Maintain the queue of unprocessed nodes

  basicblock_list loop_queue;

  // -- First, initialize the queue and loop nodes with the generator
  // set. We skip any basic blocks that are already members of some other
  // loop.

  for (basicblock_set_p p = generators.begin();
       p != generators.end();
       ++p)
    {
      basicblockNode * generator = *p;

      loopTreeNode * loop = whichLoop(generator);
      if (! loop) {
	loop_nodes.insert(generator);
	loop_queue.push_back(generator);
      }
    }

  // -- Worklist algorithm: use the queue to add nodes to the loop by
  // moving backwards from the generators until we find the header.

  while ( ! loop_queue.empty()) {

    basicblockNode * cur = loop_queue.front();
    loop_queue.pop_front();

    // -- For each predecessor

    for (basicblock_list_p q = cur->preds().begin();
	 q != cur->preds().end();
	 ++q)
      {
	basicblockNode * pred = *q;

	// -- If we haven't reached the head and we haven't seen this node
	// yet, then put it on the queue.

	if ((pred != head) &&
	    (loop_nodes.find(pred) == loop_nodes.end())) {
	  loop_nodes.insert(pred);
	  loop_queue.push_back(pred);
	}
      }
  }

  // -- Create a new loopTreeNode with the basic information gathered. The
  // rest of the code populates the object.

  loopTreeNode * loop = new loopTreeNode(kind, head);

  _containedIn[head] = loop;

  _loops.push_front(loop);

  // -- Visit the blocks in the loop, determining which ones are owned by
  // this loop. As a consequence, we also determine the nested loops.

  for (basicblock_set_p w = loop_nodes.begin();
       w != loop_nodes.end();
       ++w)
    {
      basicblockNode * block = *w;

      // -- Check to see if this block is already "owned"

      loopTreeNode * nested_loop = whichLoop(block);
      if (nested_loop) {

	// -- It is, add it's nearest ancestor to the list of nested loops

	loopTreeNode * ancestor = loopAncestor(nested_loop);

	if (ancestor != loop) {
	  loop->addNestedLoop(ancestor);
	  ancestor->parentLoop(loop);
	}
      }
      else {

	// -- Not owned, record it as owned by this loop

	_containedIn[block] = loop;

	// -- Add the block to the loop

	loop->addBlock(block);
      }
    }

  // -- Put some information in the comment

  if (kind != loopTreeNode::Top) {

    ostringstream comment;

    comment << endl;
    switch (kind) {
    case loopTreeNode::SingleEntry:
      comment << "    Single entry loop: ";
      break;
    case loopTreeNode::MultipleEntry:
      comment << "    Multiple entry loop: ";
      break;
    default:
      comment << "    (ERROR): ";
      break;
    }

    for (basicblock_set_p p = loop->blocks().begin();
	 p != loop->blocks().end();
	 ++p)
      {
	basicblockNode * bb = *p;
	comment << " #" << _preOrder[bb] << " ";
      }

    head->comment() += comment.str();
  }

  // -- Finally, if the loop kind is "Top", record it as the top of the
  // tree.

  if (kind == loopTreeNode::Top)
    _top = loop;
}

basicblockNode * loopTree::commonDominator(basicblockNode * first,
					   basicblockNode * second)
{
  // -- Simple algorithm: go up one IDOM chain until we find a node that
  // dominates the other

  basicblockNode * cur = first;

  while ( cur && ( ! Dominators::dominates(cur, second)))
    cur = cur->parent();

  // -- Sanity check (the entry node should dominate everything)

  if ( ! cur )
    cerr << "ERROR: Entry node does not dominate the rest of the basic blocks" << endl;

  return cur;
}

loopTreeNode * loopTree::loopAncestor(loopTreeNode * loop)
{
  loopTreeNode * cur = loop;

  while (cur->parentLoop())
    cur = cur->parentLoop();

  return cur;
}

void loopTree::classifyEdges(basicblock_list & post_order)
{
  // -- Make sure all the orderings and lists are clear (so that we can
  // call this routine more than once)

  _backEdges.clear();
  _forwardEdges.clear();
  _crossEdges.clear();
  _preOrder.clear();
  _rpostOrder.clear();

  // -- Initialize all the numberings to -1, which signifies "unprocessed
  // block"

  stmt_list & body = _procedure->body()->stmts();

  int pre_order_number = 0;
  int rpost_order_number = body.size() - 1;

  for (stmt_list_p x = body.begin();
       x != body.end();
       ++x)
    {
      basicblockNode * cur = (basicblockNode *) *x;
      _preOrder[cur] = -1;
      _rpostOrder[cur] = -1;
    }

  // -- Run the depth-first search

  depthFirstSearch(_procedure->entry(), post_order,
		   pre_order_number, rpost_order_number);
}

void loopTree::depthFirstSearch(basicblockNode * cur,
				basicblock_list & post_order,
				int & pre_order_number,
				int & rpost_order_number)
{
  // -- Build a comment

  ostringstream comment;

  // -- Set the pre-order number

  _preOrder[cur] = pre_order_number;
  comment << " BB #" << pre_order_number;
  pre_order_number++;

  // -- For each successor, classify the edge and call recursively if
  // necessary.

  comment << " : Successors: ";

  for (basicblock_list_p p = cur->succs().begin();
       p != cur->succs().end();
       ++p)
    {
      basicblockNode * successor = *p;
      EdgeKind kind;

      if (_preOrder[successor] == -1) {

	// -- Unvisited child: this is a tree edge

	kind = TreeEdge;

	_treeEdges[cur].insert(successor);

	depthFirstSearch(successor, post_order,
			 pre_order_number, rpost_order_number);
      }
      else if (_rpostOrder[successor] == -1) {

	// -- Successor is an ancestor: this is a back-edge

	kind = BackEdge;

	_backEdges[successor].insert(cur);
      }
      else if (_preOrder[cur] < _preOrder[successor]) {

	// -- Successor is a descendant: this is a forward edge

	kind = ForwardEdge;

	_forwardEdges[cur].insert(successor);
      }
      else {

	// -- Any other edge is a cross edge

	kind = CrossEdge;

	_crossEdges[cur].insert(successor);
      }

      comment << " #" << _preOrder[successor] << " ";
      switch (kind) {
      case TreeEdge: comment << "(Tree)";
	break;
      case BackEdge: comment << "(Back)";
	break;
      case ForwardEdge: comment << "(Forward)";
	break;
      case CrossEdge: default: comment << "(Cross)";
	break;
      }
      comment << " ";
    }

  // -- Store comment

  cur->comment() = comment.str(); 

  // -- Store this node in the post-order

  post_order.push_back(cur);

  // -- Set the post-order number

  _rpostOrder[cur] = rpost_order_number;
  rpost_order_number--;
}

void loopTree::setDepths(loopTreeNode * cur, int depth)
{
  // -- Set the current depth

  cur->depth(depth);

  // -- Visit the children

  for (loop_set_p q = cur->nestedLoops().begin();
       q != cur->nestedLoops().end();
       ++q)
    setDepths(*q, depth+1);
}

loopTree::EdgeKind loopTree::classifyEdge(basicblockNode * source,
					  basicblockNode * target)
{
  edge_map_p p;

  // -- Try the backedges first (NOTE: they are indexed by the target block).

  
  p = _backEdges.find(target);
  if (p != _backEdges.end()) {
    basicblock_set & edges = (*p).second;
    if (edges.find(source) != edges.end())
      return BackEdge;
  }

  // -- Try the forward edges

  p = _forwardEdges.find(source);
  if (p != _forwardEdges.end()) {
    basicblock_set & edges = (*p).second;
    if (edges.find(target) != edges.end())
      return ForwardEdge;
  }

  // -- Try the cross edges

  p = _crossEdges.find(source);
  if (p != _crossEdges.end()) {
    basicblock_set & edges = (*p).second;
    if (edges.find(target) != edges.end())
      return CrossEdge;
  }

  // -- If it's none of the above, then it's a tree edge

  return TreeEdge;
}

void loopTree::report()
{
  cout << "Loop tree:" << endl;

  report(_top, 0);
}


void loopTree::report(loopTreeNode * cur, int level)
{
  for (int i = 0; i < level*2; i++)
    cout << " ";

  cur->report();

  for (loop_set_p q = cur->nestedLoops().begin();
       q != cur->nestedLoops().end();
       ++q)
    report(*q, level+1);
}

