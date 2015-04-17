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

#ifndef CBZ_LOCATION_H
#define CBZ_LOCATION_H

#include "allocation.h"
#include "dominators.h"
#include "pointeroptions.h"

// ------------------------------------------------------------
// Location
// ------------------------------------------------------------

// Dominance testing : Dominance is tested in much the same way it was
// in the previous version: we have a chain of path components, so we
// move up until the two arguments have a common immediate
// parent. Then we call the type-specific dominance test. The only
// complication is when the two paths turn out to be in a direct
// ancestor/descendant relationship. In this case, we allow each type
// to determine what it does in that situation. The cases are based on
// the following rationale:

// 1. Assignments to the formal parameters are associated with the
// procLocation, and thus must dominate all other statements inside
// the procedure.
//       => ancestor procLocation always dominates descendant

// 2. Control-flow merges are associated with the basicblockLocation,
// and thus must dominate all other statements within that block.
//       => ancestor basicblockLocation always dominates descendant

// 3. The assignment of the return value at a callsite, which is
// associated with the stmtLocation, should be dominated by all
// assignments inside the procedure.
//       => ancestor stmtLocation never dominates descendant

// The rules for descendants are the mirrors of the above rules for
// ancestors.

//       => descendant stmtLocation only dominates ancestor stmtLocations
//       => descendant basicblockLocation only dominates ancestor stmtLocations
//       => descendant procLocation only dominates ancestor stmtLocations

// This divides locations into two categories: stmtLocations and
// non-stmtLocation. So we implement all of this logic with a boolean:
// is_stmt.

class procLocation;
class basicblockLocation;
class stmtLocation;

class Location : public PerClassHeap< PointersHeap >
{
public:

  typedef enum { Statement, BasicBlock, Procedure } LocationKind;

  static unsigned int stmt_count;
  static unsigned int block_count;
  static unsigned int proc_count;
  static unsigned int dom_calls;

private:

  Location & operator=(const Location & other)
  {
    cerr << "ERROR: Cannot assign Location objects" << endl;
    return *this;
  }

protected:

  Location * _parent;

  int _depth:9;
  int _kind:4;
  int _dominates_exit:1;
  int _not_basicblock:1;
  int _not_procedure:1;

  int _subtree_id:16;

  static unsigned int current_subtree_id;

#ifdef DOMINATOR_TREE

  Location * _dominator;
  int _num_children;

  unsigned int _tree_min;
  unsigned int _tree_max;

  static unsigned int current_tree_number;

  static unsigned int next_tree_number()
  { current_tree_number++; return current_tree_number; }

#endif

public:

  Location(Location * parent, LocationKind kind);

  // Fields

  inline int depth() const { return _depth; }
  inline LocationKind kind() const { return (LocationKind)_kind; }
  inline Location * parent() const { return _parent; }
  inline unsigned int subtree_id() const { return _subtree_id; }

#ifdef DOMINATOR_TREE

  inline Location * dominator() const { return _dominator; }
  inline void set_dominator(Location * d) { _dominator = d; }

  inline void init_children(int n) { _num_children = n; }
  inline int num_children() const { return _num_children; }
  inline void decrement_children() { _num_children--; }

  inline int tree_min() const { return _tree_min; }
  inline void set_tree_min(int m) { _tree_min = m; }

  inline int tree_max() const { return _tree_max; }
  inline void set_tree_max(int m) { _tree_max = m; }

  void visit();
  void finish();

#endif

  inline int dominates_exit() const { return _dominates_exit; }

  /** @brief Interprocedure dominance test */

  static bool dominates(const Location * dom, const Location * cur);

  /** @brief Interprocedure strict dominance test */

  static bool strictly_dominates(const Location * dom, const Location * cur);

  /** @brief OLD dominance test */

  static bool old_strictly_dominates(const Location * dom, const Location * cur);

  /** @brief Find a common parent location */

  static Location * common_parent(Location * one,
				  Location * two);

  /** @brief Find the enclosing procedure */

  static procLocation * procedure(Location * where);

  /** @brief See if one location is a prefix of the other */

  static bool is_prefix(const Location * prefix, const Location * longer);

  /** @brief Adjust depths
   *
   * These methods are needed to fix the depths of all Location nodes
   * during a context-insensitive reparenting. */
 
  virtual void adjust_depth() = 0;
 
  // Output

  virtual void print(ostream & o) const = 0;
  virtual void print_path(ostream & o) const = 0;

  friend ostream & operator<<(ostream & o, const Location & loc) {
    loc.print_path(o);
    return o;
  }

  // Destructor

  virtual ~Location();

  // Stats

  static void stats();
};

// ------------------------------------------------------------
// stmtLocation
// ------------------------------------------------------------

class stmtLocation : public Location
{
  friend class basicblockLocation;
  friend class procLocation;

private:

  int _stmt_num;
  stmtNode * _stmt;
  procLocation * _calls;

  inline void stmt_num(int num) { _stmt_num = num; }
  inline void stmt(stmtNode * s) { _stmt = s; }

public:

  stmtLocation(basicblockLocation * parent);

  /** @brief Make a procedure call
   *
   * If the callsite doesn't already have a procLocation under it, then
   * create one. If the procedure is context insensitive, then just create
   * a dummy procLocation. */

  void setup_call(procNode * proc, bool is_context_insensitive);

  /** @brief Remove procedure
   *
   * Remove a procedure from it's callsite, adjusting the depths to reflect
   * this. */

  procLocation * remove_call();

  /** @brief Get the containing basic block */

  inline basicblockLocation * block_location() const
  { return (basicblockLocation *) _parent; }

  /** @brief Return the number of this statement
   *
   * Statements in a basic block are numbered in order, starting with
   * zero.*/

  inline int stmt_num() const { return _stmt_num; }

  /** @brief Get the stmtNode itself. */

  inline stmtNode * stmt() const { return _stmt; }

  /** @brief Get the called procedure
   *
   * If this statement is a procedure call, return the procLocation of the
   * called procedure. */

  inline procLocation * calls() const { return _calls; }

  /** @brief Get the callNode
   *
   * If this statement is a procedure call, return the callNode. */

  callNode * callnode();

  // Check for the presence of a function call

  bool is_present(const procNode * proc) const;

  /** @brief Adjust tree depths */

  virtual void adjust_depth();

  // Output

  virtual void print(ostream & o) const;
  virtual void print_path(ostream & o) const;
  void print_callsite(ostream & o) const;

  // Destructor

  virtual ~stmtLocation();
};

typedef vector< stmtLocation > stmt_location_vec;
typedef stmt_location_vec::iterator stmt_location_vec_p;

// ------------------------------------------------------------
// basicblockLocation
// ------------------------------------------------------------

class basicblockLocation : public Location
{
  friend class procLocation;

private:

  basicblockNode * const _block;
  stmt_location_vec _statements;

public:

  basicblockLocation(procLocation * parent,
		     basicblockNode * block);

  // Fields

  inline procLocation * proc_location() const { return (procLocation *) _parent; }
  inline basicblockNode * block() const { return _block; }
  inline stmt_location_vec & statements() { return _statements; }

  /** @brief Last statement */

  stmtLocation * last() { return & _statements.back(); }

  /** @brief Adjust tree depths */

  virtual void adjust_depth();

  // Output

  virtual void print(ostream & o) const;
  virtual void print_path(ostream & o) const;

  // Destructor

  virtual ~basicblockLocation();
};

#if defined(__MWERKS__) && !defined(CBZ_HEAPLAYERS)
typedef map< basicblockNode *, basicblockLocation * > basicblock_location_map;
#else
typedef map< basicblockNode *, basicblockLocation *,
              less< basicblockNode * >, basicblock_alloc > basicblock_location_map;
#endif	/*	__MWERKS__ && !CBZ_HEAPLAYERS	*/
typedef basicblock_location_map::iterator basicblock_location_map_p;
typedef basicblock_location_map::const_iterator basicblock_location_map_cp;

// ------------------------------------------------------------
// procLocation
// ------------------------------------------------------------

class procLocation : public Location
{
private:

  procNode * const _proc;
  basicblock_location_map _basicblocks;

  static procLocation * Main;

public:

  procLocation(stmtLocation * parent,
	       procNode * proc,
	       bool is_context_insensitive = false);

  // Fields

  inline stmtLocation * stmt_location() const { return (stmtLocation *) _parent; }
  inline procNode * proc() const { return _proc; }
  basicblockLocation * lookup_block(basicblockNode * block) const;
  procLocation * parent_proc() const;

  static inline procLocation * main() { return Main; }

  // -- Get the last statement in the procedure

  stmtLocation * last();

  /** @brief Remove from tree
   *
   * Used to remove an existing branch and adjust the depths
   * appropriately. (use with stmtLocation::remove_call(). */

  void remove_from_tree();

  /** @brief Adjust tree depths */

  virtual void adjust_depth();

  // Output

  virtual void print(ostream & o) const;
  virtual void print_path(ostream & o) const;

  // Destructor

  virtual ~procLocation();
};

#endif // CBZ_LOCATION_H
