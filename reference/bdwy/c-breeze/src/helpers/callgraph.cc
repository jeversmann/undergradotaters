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
#include "callgraph.h"

callGraph::callGraph(procNode * root, Linker & linker)
  : Walker(Preorder, Subtree),
    _callgraph(),
    _root(root),
    _linker(linker),
    _current(0)
{
  // Create entries for all procedure definitions

  const proc_decl_map & procs = linker.procedures();
  for (proc_decl_map_cp p = procs.begin();
       p != procs.end();
       ++p)
    {
      procNode * pr = (*p).second;
      _current = lookup(pr);
      pr->walk(*this);
    }
}

callGraph::~callGraph()
{
  for (callgraph_map_p p = _callgraph.begin();
       p != _callgraph.end();
       ++p)
    delete (*p).second;
}      

void callGraph::at_id(idNode * the_id, Order ord)
{
  if (! the_id->decl())
    cout << "ID with no decl: " << the_id->name() << endl;
  else 
    if (the_id->decl()->type()->typ() == Func) {
      procNode * proc = _linker.lookup_procedure(the_id->decl());
      if (proc) {
	callGraphNode * cgn = lookup(proc);
	// cout << _current->proc()->decl()->name() << " calls " << cgn->proc()->decl()->name() << endl;
	_current->add_edge(cgn);
      }
    }
}

callGraphNode * callGraph::lookup(procNode * proc)
{
  callGraphNode * cgnode;
  callgraph_map_p p = _callgraph.find(proc);
  if (p == _callgraph.end()) {
    cgnode = new callGraphNode(proc);
    _callgraph[proc] = cgnode;
  }
  else
    cgnode = (*p).second;

  return cgnode;
}

void callGraph::show_all_contexts()
{
  callgraph_edge_list stack;
  int count = 0;
  callGraphNode * start = lookup(_root);
  show_all_contexts(start, stack, count);

  cout << "Total procedures: " << _callgraph.size() << endl;
  cout << "Total contexts: " << count << endl;
}
  
void callGraph::show_all_contexts(callGraphNode * cur,
				  callgraph_edge_list & stack,
				  int & count)
{
  callgraph_edge_list_p rec = find(stack.begin(), stack.end(), cur);
  if (rec != stack.end()) {
    // cout << "Recursion" << endl;
    return;
  }

  stack.push_back(cur);
  count++;

  /*
  for (callgraph_edge_list_p p = stack.begin();
       p != stack.end();
       ++p)
    {
      callGraphNode * cgn = *p;
      if (p != stack.begin())
	cout << ".";
      cout << cgn->proc()->decl()->name();
    }
  cout << endl;
  */

  for (callgraph_edge_list_cp p = cur->calls().begin();
       p != cur->calls().end();
       ++p)
    {
      callGraphNode * cgn = *p;
      show_all_contexts(cgn, stack, count);
    }

  stack.pop_back();
}

void callGraphNode::add_edge(callGraphNode * to)
{
  _calls.push_back(to);
  to->_called_by.push_back(this);
}

int callGraphNode::times_called()
{
  callgraph_node_set visited;

  return times_called(visited);
}

int callGraphNode::times_called(callgraph_node_set & visited)
{
  int total = 0;

  if (visited.find(this) != visited.end())
    return _times_called;

  visited.insert(this);

  // -- Memoize this computation so that it's not so expensive

  if (_times_called != -1)
    return _times_called;

  // -- Main is called exactly once

  if (_called_by.empty())
    total = 1;
  else {

    // -- The number of times a procedure is called is the sum of the times
    // it is called by it's predecessors in the graph. We just have to be
    // careful to avoid recursion.

    for (callgraph_edge_list_p p = _called_by.begin();
	 p != _called_by.end();
	 ++p)
      {
	callGraphNode * caller = *p;
	total += caller->times_called(visited);
      }
  }
  
  _times_called = total;

  return total;
}

void callGraphNode::ancestors(callgraph_node_set & ancestors_set)
{
  // -- Visit all the immediate callers

  for (callgraph_edge_list_p p = _called_by.begin();
       p != _called_by.end();
       ++p)
    {
      callGraphNode * caller = *p;

      // -- If the caller is not in the set, add it and go recursively up.

      if (ancestors_set.find(caller) == ancestors_set.end()) {
	ancestors_set.insert(caller);
	caller->ancestors(ancestors_set);
      }
    }
}

