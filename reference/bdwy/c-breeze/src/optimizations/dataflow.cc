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
#include "init_flowproblem_walker.h"

// ------------------------------------------------------------
// Main dataflow loop:
// Call this after setting up gen/kill sets.
// ------------------------------------------------------------

void FlowProblem::iterate(Node * n, FlowVal * initial)
{
  // -- Initialize all the confluence points with "top"...

  init_flowproblem_walker::init(n, this);

  // -- Iterate the dataflow problem until convergence...

  if (initial == 0)
    initial = top();

  FlowVal * prev = initial->clone();
  bool end_changed;
  int i = 0;

  do {

    // -- Initialize the state...
    changed(false);
    FlowVal * next = initial->clone();

    // -- Run one pass...
    n->dataflow(next, * this);

    // -- Compare with previous pass...
    end_changed = next->diff(prev);

    delete prev;
    prev = next;

    i++;
    // cout << "Iteration " << i << " changed = " << (changed() || end_changed) << endl;

  } while (changed() || end_changed);

  // -- Last pass: collect the information...
  
  last(true);
  FlowVal * next = initial->clone();
  n->dataflow(next, * this);

  // -- Clean up the confluence points

  init_flowproblem_walker::cleanup(n, this);
}

// ------------------------------------------------------------
// Meet and diff
// This function is used to keep track of when the system
// converges.
// ------------------------------------------------------------

void FlowVal::meet_and_diff(const FlowVal * other, FlowProblem & fp)
{
  FlowVal * temp = clone();

  meet(other);

  if (diff(temp))
    fp.changed(true);

  delete temp;
}
