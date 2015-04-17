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
#include "callgraph_walker.h"

void callgraph_walker::graph()
{
  callgraph_walker cw;

  // -- First collect all the procedures...

  cw.find_procs();

  // -- Then find all the callsites...

  for (unit_list_p up = CBZ::Program.begin();
       up != CBZ::Program.end();
       ++up)
    {
      cw.cur_unit(*up);
      (*up)->walk(cw);
    }
}

void callgraph_walker::find_procs()
{
  // -- Visit all translation units...

  for (unit_list_p up = CBZ::Program.begin();
       up != CBZ::Program.end();
       ++up) {

    def_list & ds = (*up)->defs();

    // -- Visit all the top-level definitions...

    for (def_list_p p = ds.begin();
	 p != ds.end();
	 ++p) {
      defNode * d = (*p);

      // -- If this is a procedure, enter it into the map..

      if (d->typ() == Proc) {
	procNode * pr = (procNode *) d;
	string & name = pr->decl()->name();
	_procs[ name ] = procpair(pr, *up);
      }
    }
  }
}

void callgraph_walker::at_call(callNode * the_call, Order ord)
{
  exprNode * nm = the_call->name();

  // -- Only handle simple function calls...

  if (nm->typ() == Id) {
    idNode * the_id = (idNode *)nm;
    string & name = the_id->name();

    // -- Look up the procedure being called...

    procnode_map_p p = _procs.find(name);
    if (p != _procs.end()) {
      procpair pp = _procs[ name ];
      procNode * pr = pp.first;
      unitNode * pu = pp.second;

      // -- Make sure the procedure is visible...

      if ((pr->decl()->storage_class() == declNode::STATIC) &&
	  (pu != cur_unit())) {
	// -- Static procedures in other units are not visible
      }
      else
	// -- Set the proc field
	the_call->proc(pr);
    }
  }
}

