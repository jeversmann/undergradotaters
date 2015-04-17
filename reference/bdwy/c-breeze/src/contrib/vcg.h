
// ----------------------------------------------------------------------
//
//  J-Breeze
//  Java Compiler Framework
// 
//  Copyright (c) 2001 University of Texas at Austin
// 
//  Teck B. Tok
//  Samuel Z. Guyer
//  Daniel A. Jimenez
//  Calvin Lin
// 
//  Permission is hereby granted, free of charge, to any person
//  obtaining a copy of this software and associated documentation
//  files (the "Software"), to deal in the Software without
//  restriction, including without limitation the rights to use, copy,
//  modify, merge, publish, distribute, and/or sublicense copies
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
//  J-Breeze extends from C-Breeze (copyright University of Texas at
//  Austin), part of whose design is inspired by the C-to-C Translator
//  from MIT Laboratory for Computer Science.
//
// ----------------------------------------------------------------------

#ifdef J_BREEZE
#include "j_breeze.h"
#else
#include "c_breeze.h"
#endif

// "abstract" class to generate input for vcg.
// subclass this class so generate special output eg. callgraph, class
// hierarchy, etc.
class vcgWalker: public Walker {

protected:  // -- local types

  typedef list<string> strings;

  ostream& graph; // the output file

  strings   exclude; // nodes to exclude from the graph; read in from file.
#ifdef J_BREEZE
  type_list current_type;
#endif
  bool      currently_excluded; // whether we are currently in a context (type,
                                // proc etc) that is to be excluded

public:   // -- constructors and destructors

  vcgWalker(ostream& ostr, const string& comment, string excludefilename);
  virtual ~vcgWalker(void) { finalize_graph(); }

protected:
#ifdef J_BREEZE
  // is type to be excluded?
  bool excluded(typeNode *type);
#else
  // is type to be excluded?
  bool excluded(procNode *proc);
  bool excluded(Coord coord);
#endif

private:
#ifdef J_BREEZE
  virtual void at_type(typeNode *type, Order order);
#else
  virtual void at_proc(procNode *proc, Order order);
#endif

protected:   // -- other methods

  inline void start_graph() {
    graph << "graph: {"  << endl;
  }

  virtual void finalize_graph() {
    // Close the graph.
    graph << endl;
    graph << "} // end of graph" << endl;
  } // finalize_graph


  void print_comment(const string& comment="");

  void print_graph_attribute(const string& attribute, const string& value);

  void print_node_attribute(const string& attribute, const string& value);
  void print_node_attribute(const string& attribute, int value);

  void print_node_value(const string& attribute, const string& value);
}; // vcgWalker


