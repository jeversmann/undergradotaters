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

#ifndef CBZ_CALLGRAPH_WALKER_H
#define CBZ_CALLGRAPH_WALKER_H


typedef pair< procNode *, unitNode * > procpair;

typedef map<string,
            procpair,
            less< string > > procnode_map;

typedef procnode_map::iterator procnode_map_p;

/** @brief Creates call-graph by associating procedure node with call nodes.
 *  @ingroup Phases
 */

class callgraph_walker : public Walker
{
public:

  // -- Use this interface to access the walker..

  static void graph();

protected:

  procnode_map _procs;
  unitNode * _cur_unit;

  callgraph_walker()
    : Walker(Preorder, Subtree),
      _procs()
  {
    find_procs();
  }

  void find_procs();

public:

  // -- Keep track of the current translation unit...

  inline unitNode * cur_unit() const { return _cur_unit; }
  inline void cur_unit(unitNode * u) { _cur_unit = u; }

  // -- Handle the callNodes...

  virtual void at_call(callNode * the_call, Order ord);
};


#endif // CBZ_CALLGRAPH_WALKER_H
