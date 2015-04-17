// $Id: mergepoints.cc,v 1.9 2003/08/07 23:14:26 pnav Exp $
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
#include <algorithm>
#include "mergepoints.h"

// --- find_merge_points : Essentially, perform an interprocedural
// version of dominance frontiers. Look for a dominance frontier in
// the current procedure; if there is none, move up to the calling
// procedure.

void mergePoints::find_merge_points(basicblockLocation * where,
				    mergepoint_list & merge_points)
{
  basicblockLocation * cur = where;
  procLocation * proc_loc;
  procNode * cur_proc;
  bool found = false;

  // --- Move up the call stack until we find a dominance frontier

  while ( cur && ! found ) {

    // --- Check to see if the given proc has a dominance frontier entry.

    proc_loc = cur->proc_location();
    cur_proc = proc_loc->proc();
    procedureInfo * info = Procedures->lookup(cur_proc);
    DFPreds * dfs = info->dominance_frontiers();

    basicblock_set_map_map_p p = dfs->find(cur->block());
    if (p != dfs->end()) {

      // --- Found one: get the list of DF blocks and make sure it's
      // not empty.

      basicblock_set_map & df = (*p).second;

      if ( ! df.empty()) {

	found = true;

	// --- Put each entry on the list

	for (basicblock_set_map_p q = df.begin();
	     q != df.end();
	     ++q)
	  {
	    basicblockNode * bb = (*q).first;
	    basicblock_set & preds = (*q).second;
	    basicblockLocation * bblock = proc_loc->lookup_block(bb);

	    merge_points.push_back(mergepoint_pair(bblock, preds));
	  }
      }
    }

    if (proc_loc->stmt_location())
      cur = proc_loc->stmt_location()->block_location();
    else
      cur = 0;
  }
}

// --- Add a block to the list of blocks that need to be merged at
// the given merge point.

void mergePoints::add_block_to_merge_point(mergepoint_pair & merge_point,
					   memoryBlock * block)
{
  basicblockLocation * merge_location = merge_point.first;

  // --- Look up the merge point

  mergepoint_map_p existing = find(merge_location);
  if (existing == end()) {

    // --- Not there; start a new entry

    memoryblock_set empty;
    pair< basicblockLocation *, memoryblock_set > new_entry(merge_location, empty);
    pair< mergepoint_map_p, bool > result = insert(new_entry);
    existing = result.first;
  }

  // --- Add the object to the list, skipping over it if it is not in
  // the scope of the given merge point (i.e., don't create a merge
  // point for a local variable outside it's potential scope).

  memoryblock_set & objects = (*existing).second;

  if (block->in_scope(merge_location))
    objects.insert(block);
      
  if (pointerOptions::Verbose)
    cout << "    - Merge " << block->name() << " at " << * merge_location << endl;
}

// --- Find the set of objects to be merged at a particular basic
// block entry point.

memoryblock_set * mergePoints::lookup_merge_point(basicblockLocation * merge_location)
{
  if (pointerOptions::Verbose) {
    cout << "  Lookup merge: " << *merge_location << endl;
  }

  mergepoint_map_p p = find(merge_location);
  if (p == end()) {
    if (pointerOptions::Verbose)
      cout << "    None found." << endl;
    return 0;
  }
  else {
      if (pointerOptions::Verbose) {
	memoryblock_set & objects = (*p).second;
	for (memoryblock_set_cp q = objects.begin();
	     q != objects.end();
	     ++q)
	  cout << "      Merge: " << (*q)->name() << endl;
      }

    return &((*p).second);
  }
}

// --- Print some statistics

void mergePoints::stats()
{
  cout << "CLASS: mergePoints" << endl;
  cout << "  Total number of merge points: " << size() << endl;
}

