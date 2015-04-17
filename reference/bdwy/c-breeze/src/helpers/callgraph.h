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

#ifndef CBZ_CALLGRAPH_H
#define CBZ_CALLGRAPH_H

#include "linker.h"
#include <set>

class callGraphNode;

typedef list< callGraphNode * > callgraph_edge_list;
typedef callgraph_edge_list::iterator callgraph_edge_list_p;
typedef callgraph_edge_list::const_iterator callgraph_edge_list_cp;

typedef set< callGraphNode * > callgraph_node_set;
typedef callgraph_node_set::iterator callgraph_node_set_p;

/** @brief node in the call graph.
 */
class callGraphNode
{
private:

  procNode * _proc;
  callgraph_edge_list _calls;
  callgraph_edge_list _called_by;
  int _times_called;

public:

  callGraphNode(procNode * proc)
    : _proc(proc),
      _calls(),
      _called_by(),
      _times_called(-1)
  {}

  procNode * proc() const { return _proc; }

  void add_edge(callGraphNode * to);

  const callgraph_edge_list & calls() const { return _calls; }

  const callgraph_edge_list & called_by() const { return _called_by; }

  int times_called();

  void ancestors(callgraph_node_set & ancestors_set);

private:

  int times_called(callgraph_node_set & visited);

};

typedef map< procNode *, callGraphNode *> callgraph_map;
typedef callgraph_map::iterator callgraph_map_p;

/** @brief visits each node in the call graph.
    @ingroup Phases
*/
class callGraph : public Walker
{
private:

  callgraph_map _callgraph;
  procNode * _root;
  Linker & _linker;
  callGraphNode * _current;

public:

  callGraph(procNode * root, Linker & linker);

  ~callGraph();

  virtual void at_id(idNode * the_id, Order ord);

  callGraphNode * lookup(procNode * proc);

  void show_all_contexts();

private:

  void show_all_contexts(callGraphNode * cur,
			 callgraph_edge_list & stack,
			 int & count);
};


#endif // CBZ_CALLGRAPH_H

