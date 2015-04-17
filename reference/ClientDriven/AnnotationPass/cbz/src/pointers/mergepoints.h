// $Id: mergepoints.h,v 1.9 2003/08/07 23:14:26 pnav Exp $
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

#ifndef CBZ_MERGEPOINTS_H
#define CBZ_MERGEPOINTS_H

#include "pointeroptions.h"
#include "memoryblock.h"
#include "proceduredb.h"

// ------------------------------------------------------------
// mergePoints : Holds all the merge points for the system --
// essentially, all the phi functions. Each entry holds the list of
// objects whose states should be merged at a particular point in the
// program.

// mergepoint_pair (df element, predecessors) : given a basic block,
// we use the DFPreds class to find its dominance frontier. For each
// element of the dominance frontier, we also return the list of basic
// blocks that come immediately before that element.

typedef pair< basicblockLocation *, basicblock_set > mergepoint_pair;
typedef list< mergepoint_pair > mergepoint_list;
typedef mergepoint_list::iterator mergepoint_list_p;

// mergepoint_map : for each merge point, record the objects that need
// to be merged there.

typedef map< basicblockLocation *, memoryblock_set > mergepoint_map;
typedef mergepoint_map::iterator mergepoint_map_p;

class mergePoints : public mergepoint_map
{
private:

  procedureDB * Procedures;
  bool _debug;

public:

  mergePoints(procedureDB * procedures, bool debug = false)
    : mergepoint_map(),
      Procedures(procedures),
      _debug(debug)
  {}



  // --- Given a list of definitions and their location, find the set
  // of merge points (dominance frontier) and record the objects at
  // those points. This is equivalent to the SSA process that adds phi
  // functions for a particular set of definitions.

  void add_merge_points(basicblockLocation * location_of_def,
			int last_stmt_num,
			const memoryblock_set & defs);

  // --- Find the dominance frontier for a particular program
  // point. We do this interprocedurally by searching up the call
  // stack until we find a suitable place for the merge.

  void find_merge_points(basicblockLocation * cur,
			 mergepoint_list & merge_points);

  // --- Add a block to the list of blocks that need to be merged at
  // the given merge point.

  void add_block_to_merge_point(mergepoint_pair & merge_point,
				memoryBlock * block);

  // --- Lookup a merge point: given a particular basic block, see if
  // there are any objects to be merged at its entry.

  memoryblock_set * lookup_merge_point(basicblockLocation * where);

  // --- Print some statistics

  void stats();
};

#endif // CBZ_MERGEPOINTS_H
