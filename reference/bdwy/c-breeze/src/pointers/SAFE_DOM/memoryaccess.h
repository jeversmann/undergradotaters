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

#ifndef CBZ_MEMORYACCESS_H
#define CBZ_MEMORYACCESS_H

#include <set>
#include "pointeroptions.h"
#include "location.h"

// microsoft compiler does not like this include
#ifndef _MSC_VER
#include <ext/hash_map>
#endif

class memoryBlock;

#ifdef DOMINATOR_TREE

class memoryblock_set : public vector< memoryBlock * >
{
public:
  void insert(memoryBlock * block) {
    if (std::find(begin(), end(), block) == end())
      push_back(block);
  }

  void insert(const_iterator b,
	      const_iterator e) {
    for (const_iterator p = b;
	 p != e;
	 p++)
      insert(*p);
  }

  iterator find(memoryBlock * el) {
    return std::find(begin(), end(), el);
  }
};

#else

typedef set< memoryBlock *,
             less< memoryBlock * >, memoryblock_alloc > memoryblock_set;
#endif

typedef memoryblock_set::iterator memoryblock_set_p;
typedef memoryblock_set::const_iterator memoryblock_set_cp;

class memoryDef;

typedef pair< Location *, memoryDef * > memorydef_location_pair;

// typedef list< memorydef_location_pair, memorydef_alloc > memorydef_list;
#if defined(__MWERKS__) && !defined(CBZ_HEAPLAYERS)
typedef vector< memorydef_location_pair > memorydef_list;
#else
typedef vector< memorydef_location_pair /* , memorydef_alloc */ > memorydef_list;
#endif
typedef memorydef_list::iterator memorydef_list_p;
typedef memorydef_list::const_iterator memorydef_list_cp;

typedef set< memoryDef *, less< memoryDef *>, memorydef_alloc > memorydef_set;
typedef memorydef_set::iterator memorydef_set_p;
typedef memorydef_set::const_iterator memorydef_set_cp;

typedef map< Location *, memoryDef * > memorydef_location_map;
typedef memorydef_location_map::iterator memorydef_location_map_p;
typedef memorydef_location_map::const_iterator memorydef_location_map_cp;

class memoryUse;

//typedef list< memoryUse *, memoryuse_alloc > memoryuse_list;
typedef vector< memoryUse * /* , memoryuse_alloc */ > memoryuse_list;
typedef memoryuse_list::iterator memoryuse_list_p;
typedef memoryuse_list::const_iterator memoryuse_list_cp;

typedef set< memoryUse *, less< memoryUse *>, memoryuse_alloc > memoryuse_set;
typedef memoryuse_set::iterator memoryuse_set_p;
typedef memoryuse_set::const_iterator memoryuse_set_cp;

class memoryAssignment;

// ------------------------------------------------------------
// memoryAccess
// ------------------------------------------------------------

/** @brief Multiplicity values */

typedef enum Multiplicity
    { Top, Deallocated, Unallocated, Single, Bounded, Unbounded, Error } Multiplicity;

// Common base class for both defs and uses

class memoryAccess : public PerClassHeap< PointersHeap >
{
public:

  static bool Verbose;
  static unsigned int def_count;
  static unsigned int use_count;
  static unsigned int merge_use_count;

  static void stats();

  typedef enum { MultipleLHS, HighMultiplicity, Forced } WeakReason;

private:

  /** @brief Location of def or use */

  Location * _where;

  /** @brief Is it weak? */

  int _is_weak:1;

  /** @brief Multiplicity (defs only) */

  int _multiplicity:8;

  /** @brief Search for reaching def (uses only) */

  int _search_for_def:1;

  /** @brief Active SSA use (uses only) */

  int _active:1;

  /** @brief The memoryBlock */

  memoryBlock * const _owner;

  /** @brief The assignment
   *
   * If this memoryUse is the left or right side of an assignment, then
   * this refers to that assignment object. */

  //  memoryAssignment * _assignment;

public:

  // --- Constructor

  memoryAccess(Location * where, memoryBlock * owner);

  // --- Fields

  inline Location * where() const { return _where; }

  inline bool is_weak() const { return _is_weak; }
  inline void set_weak(bool is) { _is_weak = is; }

  // --- Get and set the multiplicity

  inline Multiplicity multiplicity() const { return (Multiplicity)_multiplicity; }
  inline void set_multiplicity(Multiplicity lin) { _multiplicity = lin; }

  inline bool search_for_def() const { return _search_for_def; }
  inline void search_for_def(bool val) { _search_for_def = val; }

  inline bool is_active() const { return _active; }
  inline void set_active() { _active = true; }
  inline void set_inactive() { _active = false; }

  /*
  inline WeakReason why_weak() const { return _why_weak; }
  inline void set_why_weak(WeakReason reason) { _why_weak = reason; }
  */
  
  inline memoryBlock * owner() const { return _owner; }

  /*
  inline memoryAssignment * assignment() const { return _assignment; }
  inline void set_assignment(memoryAssignment * a) { _assignment = a; }
  */
};

typedef set< memoryAccess * > memoryaccess_set;
typedef memoryaccess_set::iterator memoryaccess_set_p;
typedef memoryaccess_set::const_iterator memoryaccess_set_cp;

// ------------------------------------------------------------
// memoryDef
// ------------------------------------------------------------

// A memoryDef represents a single definition of (assignment to) a
// particular memoryBlock. It records the place where the def occured,
// and the object deing modified. In addition, it is used to record
// the points-to values of any pointer objects.

class memoryDef : public memoryAccess
{
private:

  /** @brief Uses of this def */

  // memoryuse_list _uses;

  /** @brief The points-to set at this def */

  memoryblock_set _points_to;

  /** @brief The previous reaching definition */

  // memoryDef * _previous;

  /** @brief Constant value */

  const constant * _value;

  /** @brief Self-assign use
   *
   * For defs caused by self-assignments, point back to the use because it
   * might be at a different location. For example, in the case of external
   * outputs the use is at the return statement and the def is at the call
   * site. */

  memoryUse * _self_assign_use;

public:

  // --- Constructor

  memoryDef(Location * where, memoryBlock * owner);

  // --- Destructor

  ~memoryDef();

  // --- Fields

  // inline memoryuse_list & uses() { return _uses; }
  inline const memoryblock_set & points_to() const { return _points_to; }

  // inline memoryDef * previous() const { return _previous; }
  // inline void set_previous(memoryDef * previous_def) { _previous = previous_def; }

  inline const constant * value() const { return _value; }
  inline void set_value(const constant * newval) { _value = newval; }

  // -- Self assign use

  inline memoryUse * self_assign_use() const { return _self_assign_use; }
  inline void set_self_assign_use(memoryUse * new_use) { _self_assign_use = new_use; }

  /** @brief Add pointers
   *
   * Add pointer values into the points-to set for this def. */

  void add_pointers(const memoryblock_set & mbs);

  /** @brief Clear the points-to set
   *
   * Remove all the previous values. This is usually done as part of a
   * strong update. */

  void clear_points_to();

  // --- Output

  void print(ostream & o) const;
};

// ------------------------------------------------------------
// orderedDefs
// ------------------------------------------------------------

// An orderedDef object holds a sequence of memoryDefs in dominating
// order (a particular def always comes before the defs that dominate
// it).

class orderedUses;

class orderedDefs
{
private:

  memorydef_list _defs;

  memorydef_location_map _quick_lookup;

public:

  orderedDefs()
    : _defs(),
      _quick_lookup()
  {}

  // --- Return the list of defs

  const memorydef_list & def_list() const { return _defs; }

  // --- Find the memoryDef for a given program location

  memoryDef * find_def(Location * where);

  // --- Find the nearest strictly dominating memoryDef (use this to
  // find the reaching definition for a regular use.

  memoryDef * find_strictly_dominating_def(Location * where);

  // --- Find the nearest dominating memoryDef (use this to find the
  // reaching definition for a merge-point use.

  memoryDef * find_dominating_def(Location * where);

  // --- Add a new memoryDef in the proper place, and return a
  // reference to it. If there is already an entry for the given path,
  // just return that memoryDef.

  memoryDef * make_def_at(Location * where, memoryBlock * owner, bool & is_new);

  // --- Prune out unnecessary defs

  void prune(orderedUses & Uses);

  // --- Output

  void print(ostream & o) const;

  // --- Clear

  void clear();

  // --- Stats

  void stats(int & total_defs, int & merge_defs, int & weak_defs);

};

// ------------------------------------------------------------
// memoryUse
// ------------------------------------------------------------

// A memoryUse represents a single use of the memoryBlock. It points
// to the reaching definition (from the orderedDefs list). For merges
// (phi functions) we create one for each of the control-flow
// predecessors.

class memoryUse : public memoryAccess
{
private:

  memoryDef * _reaching_def;

public:

  // --- Constructor

  memoryUse(Location * where, memoryBlock * owner);

  // --- Destructor

  ~memoryUse();

  // --- Fields

  memoryDef * reaching_def() const { return _reaching_def; }
  void reaching_def(memoryDef * def);

  // --- Output

  void print(ostream & o) const;
};


typedef map< Location *, memoryUse * > memoryuse_map;
typedef memoryuse_map::iterator memoryuse_map_p;
typedef memoryuse_map::const_iterator memoryuse_map_cp;
typedef memoryuse_map::value_type memoryuse_map_pair;

typedef map< basicblockNode *, memoryUse *> pred_use_map;
typedef pred_use_map::iterator pred_use_map_p;
typedef pred_use_map::const_iterator pred_use_map_cp;

typedef map< Location *, pred_use_map > merge_use_map;
typedef merge_use_map::iterator merge_use_map_p;
typedef merge_use_map::const_iterator merge_use_map_cp;

// ------------------------------------------------------------
// orderedUses
// ------------------------------------------------------------

class orderedUses
{
private:

  memoryuse_map _uses;
  merge_use_map _merge_uses;
  
public:

  orderedUses()
    : _uses(),
      _merge_uses()
  {}

  // --- Find an existing use for the given location (doesn't work for
  // merge/phi functions).

  memoryUse * find_use(Location * where) const;

  // -- Find uses at (works for phi functions)

  void find_uses_at(Location * where, memoryuse_set & uses);

  // --- Add a new use for the given location, or return the existing
  // one.

  memoryUse * make_use_at(Location * where, memoryBlock * owner);

  // --- Add a new set of uses for a particular merge point: one use
  // for each control-flow predecessor, with the reaching definitions
  // set appropriately.

  const pred_use_map & make_merge_uses_at(basicblockLocation * where,
					  memoryBlock * owner);

  // --- Update the def-use chains (only performed once at the end of
  // analysis by the memoryModel).

  void update_def_use_chains();

  /** @brief Def uses
   *
   * Go through the uses and collect the ones that are reached by the given
   * definition. */

  void def_uses(memoryDef * def,
		memoryuse_list & uses);

  // --- Prune uses for a particular location

  void prune(Location * where);

  // --- Output

  void print(ostream & o) const;

  // --- Clear

  void clear();

  // --- Stats

  void stats(int & total_uses, int & merge_uses);
};

// ----------------------------------------------------------------------
// memoryAssignment
// ----------------------------------------------------------------------

class memoryAssignment : public PerClassHeap< PointersHeap >
{
private:

  /** @brief Location of the assignment
   *
   * */

  Location * _where;

  /** @brief Right-hand side
   *
   * This is a set of memoryUse. */

  memoryuse_set _uses;

  /** @brief Left-hand side
   *
   * This is a set of memoryDef. */

  memorydef_set _defs;

public:

  /** @brief Create a new assignment */
  
  memoryAssignment(Location * where)
    : _where(),
      _uses(),
      _defs()
  {}

  /** @brief Location */

  inline Location * where() const { return _where; }

  /** @brief Add to the right-hand side */

  inline void add_use(memoryUse * use) { _uses.insert(use); }

  /** @brief Add to the left-hand side */

  inline void add_def(memoryDef * def) { _defs.insert(def); }

  /** @brief Get the right-hand side */

  inline const memoryuse_set & uses() const { return _uses; }

  /** @brief Get the left-hand side */

  inline const memorydef_set & defs() const { return _defs; }
};

// ----------------------------------------------------------------------
//  Assignments
// ----------------------------------------------------------------------

typedef pair< Location *, memoryBlock *> assignment_key;

typedef map< assignment_key, memoryAssignment * > assignment_map;
typedef assignment_map::iterator assignment_map_p;

class assignmentSet
{
private:

  /** @brief The set of assignments */

  assignment_map _assignments;

public:

  /** @brief Create a new set of assignments */

  assignmentSet()
    : _assignments()
  {}

  /** @brief Lookup an assignment
   *
   * The create boolean controls what to do if there isn't one at the given
   * location. Passing true causes the method to create a new one; passing
   * false causes it to return null. */

  memoryAssignment * assignment_at(Location * where, memoryBlock * block,
				   bool create);
};

#endif // CBZ_MEMORYACCESS_H

