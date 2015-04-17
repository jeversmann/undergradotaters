// $Id: memoryblock.h,v 1.31 2003/09/29 13:01:33 toktb Exp $
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

#ifndef CBZ_MEMORYBLOCK_H
#define CBZ_MEMORYBLOCK_H

#include "pointeroptions.h"
#include "location.h"
#include "memoryaccess.h"
#include "proceduredb.h"
#include "unification.h"

#include <set>

// ------------------------------------------------------------
//  Memory block

// Each object in memory that may be a pointer, or pointed to is
// represented by a memoryBlock. 

class memoryBlock;

typedef list< memoryBlock * > memoryblock_list;
typedef memoryblock_list::iterator memoryblock_list_p;

typedef map< int, memoryBlock *, less< int > > component_map;
typedef component_map::iterator component_map_p;
typedef component_map::const_iterator component_map_cp;

typedef vector< memoryBlock * > memoryblock_vector;
typedef memoryblock_vector::iterator memoryblock_vector_p;

// -- Keep track of the reaching objects that are combined at a
// context-insensitive procedure call (the self-assignments generated when
// this object is an external input).

typedef map< stmtLocation *, memoryblock_set > callsite_objects_map;
typedef callsite_objects_map::iterator callsite_objects_map_p;
typedef callsite_objects_map::const_iterator callsite_objects_map_cp;

typedef map< procNode *, callsite_objects_map > parameter_assign_map;
typedef parameter_assign_map::iterator parameter_assign_map_p;

class enumPropertyAnn;

class memoryBlock : public PerClassHeap< PointersHeap >
{
public:

  typedef enum { Control_flow, Parameter_pass, Weak_update, Additive } DestructiveKind;

  typedef map< Location *, DestructiveKind > destructive_assignment_map;
  typedef destructive_assignment_map::iterator destructive_assignment_map_p;
  typedef destructive_assignment_map::const_iterator destructive_assignment_map_cp;

  typedef map< Location *, memoryblock_set > complicit_assignment_map;
  typedef complicit_assignment_map::iterator complicit_assignment_map_p;
  typedef complicit_assignment_map::const_iterator complicit_assignment_map_cp;

  static int memoryblock_count;
  static void stats();

private:

  /** @brief The containing struct, if there is one. */

  REF memoryBlock * _container;

  /** @brief The uses and defs of this block.
   *
   *  The defs are maintained in the dominating order described in
   *  memoryaccess.h. The uses are maintained as a multi-map from the
   *  program location to the memoryUse object. It's a multimap because a
   *  merge (phi) will return multiple uses. */
  //@{

  TREE orderedDefs Defs;
  TREE orderedUses Uses;

  //@}

  /** @brief Parameter-assignments
   *
   * When this object is an input to a context-insensitive procedure, we
   * merge its state from each of the possible call sites. This map keeps
   * track of the reaching def for each callsite according to the procedure
   * being called. If there is no reaching def, then the memoryDef pointer
   * is null. */

  parameter_assign_map _parameter_assignments;

  /** @brief Current use
   *
   * This pointer is used during expression evaluation to hold the current
   * use of this block. It is set by the use_at() method. I'm not sure
   * where (or whether) it should be reset.
   *
   * NEW: for flow insensitive objects, we use this field to store the only
   * use (at main). */

  REF memoryUse * _current_use;

  /** @brief Current def
   *
   * This pointer is used during expression evaluation to hold the current
   * def of this block. It is set by the def_at() method. THIS WAS A BUG:
   * we need to at least reset these values when a memoryBlock def leaves a
   * procedure. The reason is that we can leave a procedure at any time
   * (when it converges), which doesn't necessarily leave the most recent
   * def in current_def.
   *
   * NEW: for flow insensitive objects, we use this field to store the only
   * def (at main). */

  REF memoryDef * _current_def;

  /** @brief A list of things contained in this memory block. */

  component_map  _components;

  /** @brief The declaration of this object.
   *
   *  This can be a synthetically created declNode for things that are not
   *  explicitly represented (like heap objects). */

  declNode * _decl;

  /** @brief Synthetic declaration flag
   *
   *  Set to true if the declNode object is synthetic (that is, created
   *  just so that there is something to put there). In this case, the
   *  destructor will also destroy the declNode. */

  int _synthetic_decl:1;

  /** @brief Write protected
   *
   * We can mark objects as "write protected", which prevents any defs on
   * them. For now, this is only used on the null object. */

  int _write_protected:1;

  /** @brief Is array
   *
   * This flag is set to true if this object is a C array. */

  int _is_array:1;

  /** @brief Is array element
   *
   * This flag is set to true if this object represents the elements of a C
   * array. */

  int _is_array_element:1;

  /** @brief Is indexed
   *
   * This flag is set to true for objects that are indexed (array elements)
   * or any memory that is accessed by pointer arithmetic. Such objects
   * represent more than one actual objects. */

  int _is_indexed:1;

  /** @brief Is alloc object
   *
   * This flag is set to true for the allocation object (see above). */

  int _is_allocation_object:1;

  /** @brief Flow sensitivity
   *
   * We can control flow-sensitivity on a per-object basis. When this flag
   * is true this object is flow-sensitive. The default will be determined
   * by the memoryModel. */

  int _flow_sensitive:1;

  // TB_unify
  /** @brief Unification.
   *
   * We can control unification on a per-object basis. When this flag
   * is true this object can be unified with another object. The default will
   * be determined by the memoryModel. */

  int _unify:1;

  /** @brief Single assignment
   *
   * For flow-insensitive objects, record if there is only one assignment
   * ever made to the object. This helps the adaptive algorithm avoid
   * making too many variables flow sensitive. */

  int _single_assignment:1;

  /** @brief Is return value?
   *
   * Return value variables are never recorded as uses. */

  int _is_return_value:1;

  /** @brief Owner procedure
   *
   *  For local variables, this points to the procedure that contains the
   *  declaration. This allows us to detect non-local access to local
   *  variables. */

  procNode * _local_to;

  /** @brief Name
   *
   * We can specify alternate name for the object (just for debugging
   * really). */

  // string _name;

  /** @brief Heap allocation site
   *
   *  For heap allocated objects, this field holds the statement that
   *  generate the allocation (typically malloc() or calloc()). */

  stmtLocation * _allocation_site;

  /** @brief Initializer def
   *
   * This special def is not in the orderedDefs container. It represents
   * any initial value for this memoryBlock. Right now, we only use it to
   * represent the structure of arrays. If this object is an array, then
   * this field will hold a pointer to the element. */

  // memoryDef * _initializer_def;

  /** @brief Allocation object
   *
   * This is a pointer to a special memoryBlock that represents the
   * allocation state of heap allocated objects. This object is used for
   * the multiplicity analysis (which is a very sparse analysis
   * problem). */

  memoryBlock * _allocation_object;

  /** @brief Only def
   *
   * In flow insensitive mode a memoryBlock only has one def at the start
   * of the main function. */

  // memoryDef * _only_def;

  /** @brief Only use
   *
   * Like the only def, in flow insensitive mode a memoryBlock has only one
   * use at the end of the main function. */

  // memoryUse * _only_use;

  /** @brief Input to
   *
   * This set contains all the procedures for which this object is an
   * external input. We need this list for flow-insensitive objects so that
   * we can force reanalysis of those procedures. */

  procedureinfo_set _input_to;

  /** @brief Destructive assignments
   *
   * This map is used by the precision monitor to record destructive
   * assignments: where they occur and the cause. */

  destructive_assignment_map _destructive_assignments;

  /** @brief Complicit assignments
   *
   * This map is used by the precision monitor to record complicit
   * assignments: where they occur and the objects that are responsible. */

  complicit_assignment_map _complicit_assignments;

  /** @brief information on unified type, if this object is unified. */
  UnifyType *_unifytype; // TB_unify

public:

  // --- These two members are just for Broadway

  /** @brief Property handle
   *
   * In Broadway, we create special memoryBlocks that represent the value
   * of a particular property. This handle allows us to get from the
   * memoryBlock to the property it represents. */

  enumPropertyAnn * property;

  /** @brief Property blocks
   *
   * Each regular memoryBlock has a vector of pointers to the property
   * blocks. */

  memoryblock_vector property_blocks;

private:

  friend class memoryModel;

  /** @brief Create a new memoryBlock
   *
   *  The new memoryBlock is associated with the given declNode, which may
   *  have been synthetically created. Only the memoryModel class should be
   *  allocating new memoryBlocks. */
  
  memoryBlock(declNode * decl, bool synthetic,
	      memoryBlock * container,
	      procNode * local_to);

  /** @brief Destructor
   *
   * Destroy the memoryBlock and all use/def information. This should only
   * be called by the memoryModel class destructor. */

  ~memoryBlock();

public:

  /** @brief Clear
   *
   *  Remove all def and use information, preparing for reanalysis. */

  void clear();

  // --- Data members

  declNode * decl() const { return _decl; }
  procNode * local_to() const { return _local_to; }

  bool is_synthetic_decl() const { return _synthetic_decl; }

  bool write_protected() const { return _write_protected; }
  void set_write_protected() { _write_protected = true; }

  bool is_indexed() const { return _is_indexed; }
  void set_indexed() { _is_indexed = true; }

  // void set_name(const string & new_name) { _name = new_name; }

  stmtLocation * allocation_site() const { return _allocation_site; }
  bool is_heap_object() const { return _allocation_site != 0; }
  void set_heap_object(stmtLocation * alloc_site) { _allocation_site = alloc_site; }

  bool is_allocation_object() const { return _is_allocation_object; }

  memoryUse * current_use() const { return _current_use; }
  memoryDef * current_def() const { return _current_def; }

  void reset_current_def_use(Location * unused);

  void set_current_def_use(Location * where);

  inline memoryBlock * container() const { return _container; }

  inline bool is_flow_sensitive() const { return _flow_sensitive; }
  inline void set_flow_sensitive() {
    _flow_sensitive = true;
    // cout << "SETFS: (0x" << this << ") " << name() << endl;
  }
  inline void set_flow_insensitive() { _flow_sensitive = false; }

  inline bool is_unify() const { return _unify; }
  inline void set_unify(bool flag) { _unify = flag; }
  inline UnifyType *unifyType() const { return _unifytype; }
  inline void unifyType(UnifyType *t) { _unifytype=t; }

  inline bool is_single_assignment() const { return _single_assignment; }

  inline bool is_return_value() const { return _is_return_value; }
  inline bool set_return_value() { _is_return_value = true; return true; }

  // inline memoryDef * only_def() const { return _only_def; }
  // inline memoryUse * only_use() const { return _only_use; }

  inline procedureinfo_set & input_to() { return _input_to; }

  // --- Manage uses and defs

  /** @brief Create a definition
   *
   * Create a def of the memoryBlock at the given location, if one doesn't
   * already exist. Return the memoryDef object, and also set the
   * current_def to this def. */

  memoryDef * def_at(Location * where, bool & is_new);

  /** @brief Find the nearest dominating def
   *
   * This requires a dominance-based search through the ordered defs list
   * to determine the nearest dominating definition. It is guaranteed to be
   * unique by the placement of the merge nodes (phi functions). */

  memoryDef * nearest_def_at(Location * where);

  /** @brief Find the nearest interprocedural dominating def
   *
   * This version properly handles context-insensitivity by taking the
   * current call stack as input. */

  /*
  memoryDef * nearest_def_at(Location * where,
			     const procedurecall_stack & callstack);
  */

  /** @brief Create a use
   *
   * Create a use of the memoryBlock at the given location, if one doesn't
   * already exist. Return the memoryUse object and set the current_use to
   * the use. */

  memoryUse * use_at(Location * where);

  /** @brief Find the last def in a basic block
   *
   * This is used when processing merge (phi function) to find the reaching
   * definition for each input to the merge. It finds the def that
   * dominates the last statement of the basic block. */

  memoryDef * last_def_at(basicblockLocation * block);

  /** @brief Find a def
   *
   * See if there is a def at the given location, but return NULL if there
   * isn't one. */

  memoryDef * find_def_at(Location * where);

  /** @brief Find a use
   *
   * See if there is a use of this block at the given location, returning
   * NULL if there isn't. This version does not work for merge uses. */

  memoryUse * find_use_at(Location * where);

  /** @brief Find uses
   *
   * Return any uses at this location. This version does work for merge
   * uses. */

  void find_uses_at(Location * where, memoryuse_set & uses);

  /** @brief List of defs
   *
   * Return a list of all the defs of this object. */

  inline const memorydef_list & defs() const { return Defs.def_list(); }

  /** @brief Add a parameter-assignment
   *
   * Record the procedure, callsite and reaching def. This version is used
   * for self-assignments when there is just one reaching def and it
   * belongs to this object. */

  void add_parameter_assignment(procNode * proc, stmtLocation * callsite,
				memoryBlock * block);

  /** @brief Add a parameter-assignment
   *
   * This version is used for regular formal parameter assignments, where
   * there could be multiple actual parameters coming from different
   * objects. */

  void add_parameter_assignment(procNode * proc, stmtLocation * callsite,
				memoryblock_set & blocks);

  /** @brief Get parameter-assignments
   *
   * Return the reaching defs for a particular context-insensitive call. */

  const callsite_objects_map & parameter_assignments(procNode * proc)
    { return _parameter_assignments[proc]; }

  /** @brief Apply weak update
   *
   * This method is used to collect the previous points-to values in the
   * case of a weak def. It assumes that a def has just been created for
   * the given location. */

  void apply_weak_update(Location * current,
			 memoryDef * previous_def,
			 memoryuse_set & uses);

  /** @brief Set up a merge point
   *
   * After encountering a def of the memoryBlock, set up the merge uses
   * (inputs to the phi function) at the given merge location. The merge
   * location will be a member of the dominance frontier of the def
   * location. */

  void setup_merge_uses_at(basicblockLocation * merge_location,
			   memoryDef * reaching_def,
			   basicblock_set & predecessors);

  /** @brief Process a merge point
   *
   * When a merge point is actually encountered, this function returns the
   * list of merge uses (inputs to the phi function), which will have been
   * previously set up by the setup_merge_uses_at() method above. In
   * addition, during the computation of def-use chains, this method finds
   * the reaching def for each merge use (I believe that I've found a
   * clever way to do this). */

  void merge_uses_at(basicblockLocation * where,
		     memoryuse_list & uses,
		     cfg_edge_set & active_edges,
		     memoryDef * dominating_def,
		     bool computePointers);

  /** @brief Reachable blocks
   *
   * In the given context, return all of the memoryBlocks reachable by
   * traversing pointers (the closure of the points-to relation). */

  void reachable_blocks(Location * where,
			bool has_use_here,
			memoryblock_list & worklist,
			memoryblock_set & already_seen,
			memoryBlock * null,
                        bool components_only = false);

  /** @brief Get the allocation object
   *
   * Get the allocation object, creating it if necessary. This object is
   * used to track the state of heap allocated objects (multiplicity
   * analysis). */

  memoryBlock * get_allocation_object(memoryModel & Memory);

  /** @brief Return the allocation object
   *
   * Returns null if the block has no allocation object. */

  memoryBlock * allocation_object();

  /** @brief At an allocation
   *
   * This method handles the multiplicity analysis for a heap allocation
   * site. It should only be called on the special allocation object. The
   * method returns the new multiplicity value. */

  Multiplicity at_allocation(Location * current,
			     memoryDef * reaching_def,
			     memoryblock_set & defs,
			     memoryuse_set & uses,
			     memoryblock_set & changes);

  /** @brief At a deallocation
   *
   * This method handles the multiplicity analysis for a heap deallocation
   * site. The same conditions hold for this method as for at_allocation()
   * above. */

  Multiplicity at_deallocation(Location * current,
			       memoryDef * reaching_def,
			       memoryblock_set & defs,
			       memoryuse_set & uses,
			       memoryblock_set & changes);

  /** @name Precision analysis */

  //@{

  /** @brief Add destructive assignment
   *
   * Add the given cause and location to the destructive assignment
   * list. If it already exists, the cause is overwritten. */

  void add_destructive_assignment(Location * where, DestructiveKind cause);

  /** @brief Add complicit assignment
   *
   * Add the given objects to the complicit assignment set for the given
   * location. */

  void add_complicit_assignment(Location * where, memoryblock_set & objects);

  /** @brief Add complicit assignment
   *
   * Add one object to the complicit assignment set for the given
   * location. */

  void add_complicit_assignment(Location * where, memoryBlock * object);

  /** @brief Get destructive assignment list */

  const destructive_assignment_map & destructive_assignments() const
  { return _destructive_assignments; }

  /** @brief Get the complicit assignment list */

  const complicit_assignment_map & complicit_assignments() const
  { return _complicit_assignments; }

  /** @brief Add to flow-sensitivity list
   *
   * This method adds the block to the given flow-sensitivity list. It does
   * this by adding a flow_sensitive_pair object containing a statement and
   * a declaration, which can be used to identify the block on subsequent
   * analysis passes. We need this capability because the memoryBlocks
   * themselves are destroyed and recreated on each pass. */

  void add_to_flow_sensitive_list(flow_sensitive_set & flow_sensitive_objects);

  /** @brief Find in flow-sensitivity list
   *
   * Return true if the block is identified in the given list. */

  bool is_in_flow_sensitive_list(flow_sensitive_set & flow_sensitive_objects);

  /** @brief Set flow sensivity
   *
   * Check the given list of flow-sensitive objects to see if this block is
   * identified on the list. Depending on the analysis mode, set the
   * flow-sensitivity accordingly. */

  void set_flow_sensitivity(flow_sensitive_set & flow_sensitive_objects);

  /** @brief Add to non-unify list
   *
   * This method adds the block to the given flow-sensitivity list. It does
   * this by adding a flow_sensitive_pair object containing a statement and
   * a declaration, which can be used to identify the block on subsequent
   * analysis passes. We need this capability because the memoryBlocks
   * themselves are destroyed and recreated on each pass. */

  void add_to_non_unify_list(UnifyTypes & non_unify_types);
  /** @brief Set unification.
   *
   * Check the given list of unify objects to see if this block is
   * identified on the list. Depending on the analysis mode, set the
   * flow-sensitivity accordingly. */

  void set_unification(UnifyTypes & unify_types);

  //@}

  /** @brief Top-most container
   *
   * Traverse the container chain up to find the top-most container. */

  memoryBlock * top_most_container();

  // TB_unify
  /* @brief Get all parents. */
  memoryblock_set containers() const;
  memoryblock_set top_most_containers();

  /** @brief Print multiplicity values */

  static void print_multiplicity(Multiplicity m, ostream & out);  

  /** @brief Def uses 
   *
   * Collect all the uses reached by the given definition. */

  void def_uses(memoryDef * def,
		memoryuse_list & uses);

  // --- Prune out unneeded uses and defs

  void prune_defs_uses();

  // --- Manage structure fields

  void dot(const string & field_name,
	   declNode * field_decl,
	   memoryModel & Memory,
	   memoryblock_set & results);

  // --- Handle special array objects

  bool is_array() const { return _is_array; }

  bool is_array_element() const { return _is_array_element; }

  void set_array_element(memoryBlock * element);

  memoryBlock * get_array_element();

  memoryDef * setup_array_def();

  // --- Scoping

  bool in_scope(basicblockLocation * where) const;

  // --- Output

  friend ostream& operator<<(ostream & o, const memoryBlock & mb) {
    // mb.print(o, Path::always());
    return o;
  }

  // --- Pass after = true to see the points-to information after the
  // given statement has been executed.

  typedef enum { NAME_ONLY, CURRENT_VALUE, AFTER_ASSIGNMENT, ALL_VALUES } Output_mode;

  void print(ostream & o, Location * path, Output_mode mode = CURRENT_VALUE) const;
  string name() const;

  string generate_su_field_name(const string & field) const;

  void print_def_use(ostream & o) const;

  // --- Update the def-use chains (only performed once at the end of
  // analysis by the memoryModel).

  void update_def_use_chains();

  // --- Statistics

  void stats(ostream & out, bool header,
	     long int & global_defs,
	     long int & global_merge_defs,
	     long int & global_weak_defs,
	     long int & global_uses,
	     long int & global_merge_uses);
  
private:

  //  Structure field-name database

  // Since the number of possible field names is small, we keep them in
  // a database and then refer to them by number in the memoryBlock class.

  class FieldNameDB
  {
  private:

    typedef map< string, int, less< string > > field_name_map;
    typedef field_name_map::iterator field_name_map_p;

    field_name_map _fields;
    int _count;

  public:

    static int ArrayField;

    FieldNameDB()
      : _fields(),
	_count(0)
    {}

    // --- Get the index for a field name, create if needed

    int get_field(const string & name);

    // --- Reverse search: find the name for a particular index

    string field_name(int index);

    void stats() {
      cout << "  Field names: " << _count << endl;
    }
  };

  static FieldNameDB FieldNames;

  // TB_unify helper functionn.
  memoryblock_set top_most_containers(memoryblock_set &visited);
};


#endif // CBZ_MEMORYBLOCK_H
