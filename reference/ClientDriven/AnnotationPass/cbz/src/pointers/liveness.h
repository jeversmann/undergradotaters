// $Id: liveness.h,v 1.12 2005/06/03 02:00:48 akkartik Exp $
// ----------------------------------------------------------------------
//
//  C-Breeze
//  C Compiler Framework
// 
//  Copyright (c) 2003 University of Texas at Austin
// 
//  Samuel Z. Guyer
//  Adam Brown
//  Teck Bok Tok
//  Paul Arthur Navratil
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

#ifndef CBZ_LIVENESS_H
#define CBZ_LIVENESS_H

#include "pointers.h"

typedef set< procNode * > procedure_set;
typedef procedure_set::iterator procedure_set_p;
typedef procedure_set::const_iterator procedure_set_cp;

/** @brief Liveness analyzer
 *
 * Liveness is a property of statements in the program. A statement is live
 * if any of the defs created by the statement are live. A def is live if
 * it has uses, and at least one of it's uses is live. A use is live if the
 * statement in which it occurs is live. */

class livenessAnalyzer : public analysisProblem
{
private:

  typedef set< stmtNode * > stmt_liveness_set;
  typedef stmt_liveness_set::iterator stmt_liveness_set_p;

  typedef set< stmtLocation * > stmtlocation_liveness_set;
  typedef stmtlocation_liveness_set::iterator stmtlocation_liveness_set_p;

  typedef pair< Location *, memoryBlock * > mergepoint_pair; 
  typedef set< mergepoint_pair > mergepoint_liveness_set;
  typedef mergepoint_liveness_set::iterator mergepoint_liveness_set_p;

  typedef list< bool > change_stack;
  typedef change_stack::iterator change_stack_p;

  /** @brief Live statements
   *
   * This set records which statements are live. It is really
   * context-insensitive summary of the live statement locations. */

  stmt_liveness_set _live_stmts;

  /** @brief Live statement locations
   *
   * Set of live statements. Note that some kinds of statements are
   * automatically live (for example, the return statement and all if
   * statements. */

  stmtlocation_liveness_set _live_stmtlocations;

  /** @brief Live merge points
   *
   * This set records which merge points are live. A merge point is
   * specified by the basic block in which it occurs, and the memoryBlock
   * being merged. */

  mergepoint_liveness_set _live_mergepoints;

  /** @brief Collect defs
   *
   * This set is used on a per-statement basis to collect memoryDef
   * objects. Once we have the set of defs, we can decide whether or not
   * the current statement is live. */

  memorydef_set _defs;

  /** @brief Record changes */

  bool _change;

  /** @brief Change stack
   *
   * We need to record the state of the change boolean in procedures above
   * the current one. */

  change_stack _change_stack;

  /** @brief Set of visited procedures
   *
   * We record the visited procedures so that we don't remove dead code
   * from uncalled procedures. */

  procedure_set _visited_procedures;

public:

  /** @brief Create a liveness analysis object */

  livenessAnalyzer();

  /** @brief Name of the analysis
   *
   * Just for debug purposes. */

  virtual string name() { return string("Liveness"); }

  /** @brief Reset liveness information
   *
   * Allow the liveness analyzer to be run again. */

  void clear();

  /** @brief Is a particular statement live
   *
   * This is the context insensitive query */

  bool isLive(stmtNode * stmt);

protected:

  /** @brief Collect defs
   */

  void collectDefs(pointerValue & pointer);

  /** @brief Add one def */

  void addDef(memoryDef * def);

  /** @brief Determine liveness
   *
   * Given the set of defs collected earlier, visit each def and each use
   * that it reaches to see if any of them are live. */

  bool determineLiveness();

  /** @brief Look up liveness
   *
   * Look up the liveness of a memoryUse. This depends on the kind of
   * location it comes from. As a special case, procLocations are always
   * considered live. */

  bool isLive(memoryUse * use, memoryBlock * owner);

  /** @brief Set live for statements
   *
   * Record the given statement location as live. */

  void setLive(stmtLocation * where);

  /** @brief Set live for merge-points
   *
   * Record the given merge-point as live. Since all blocks share a single
   * merge-point location, we need to distinguish which ones are live by
   * the specific block being merged. */

  void setLive(Location * where, memoryBlock * block);

public:

  inline const procedure_set & visited_procedures() const { return _visited_procedures; }

  virtual void at_index(stmtLocation * current,
			operandNode * operand,
			pointerValue & left,
			pointerValue & right,
			pointerValue & result);

  // -- Procedure calls

  virtual void at_call(stmtLocation * current, operandNode * call,
		       pointerValue & call_target,
		       procNode * callee,
		       pointervalue_list & arguments,
		       pointerValue & return_val);

  // -- Assignments

  virtual void at_assignment(stmtLocation * current,
			     pointerValue & left,
			     pointerValue & right,
			     pointerValue & result,
			     memoryblock_set & changes);

  // -- Statement types

  virtual void at_return(stmtLocation * stmt,
			 returnNode * ret,
			 pointerValue & result,
			 pointerValue & return_val);

  virtual void at_threeAddr(stmtLocation * stmt,
			    threeAddrNode * threeaddr,
			    pointerValue & result);

  virtual void at_conditiongoto(stmtLocation * stmt,
		                conditiongotoNode * c,
                        pointerValue & left,
                        pointerValue & right,
		                pointerValue & result);

  // -- Process a merge point

  virtual void at_merge(Location * where,
			memoryBlock * block,
			memoryuse_list & phi_uses,
			pointerValue & result,
			memoryblock_set & changes);

  // -- Memory allocation and deallocation

  virtual void at_allocation(stmtLocation * stmt,
			     pointervalue_list & arguments,
			     memoryBlock * block,
			     memoryblock_set & changes);

  virtual void at_deallocation(stmtLocation * stmt,
			       pointerValue & to_deallocate,
			       memoryblock_set & changes);

  // -- Control-flow options

  virtual void at_stmt_exit(stmtLocation * stmt,
			    pointerValue & result);

  virtual void at_basicblock_entry(basicblockLocation * block,
				   procedureInfo * info,
				   pointerValue & initial);

  // -- Procedure boundaries

  virtual void at_procedure_entry(procLocation * proc,
				  procedureInfo * info,
				  pointerValue & return_val);

  virtual void at_procedure_exit(procLocation * proc,
				 procedureInfo * info,
				 pointerValue & return_val);
};

/** @brief Dead-code elimination
 */

class deadcodeChanger : public Changer
{
public:

  static void optimize(unitNode * u,
		       livenessAnalyzer * liveness);
private:

  livenessAnalyzer * _liveness;

  deadcodeChanger(livenessAnalyzer * liveness);

public:

  virtual Node * at_threeAddr(threeAddrNode * stmt, Order ord);
};


#endif // CBZ_LIVENESS_H
