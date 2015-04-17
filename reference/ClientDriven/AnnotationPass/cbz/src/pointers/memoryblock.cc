// $Id: memoryblock.cc,v 1.35 2005/06/03 02:00:48 akkartik Exp $
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
#include "memorymodel.h"
#include "memoryblock.h"
#include "unification.h"

// ------------------------------------------------------------
// Constructor
// NOTE: New memoryBlocks should only be created by the
//       memoryModel class.
// ------------------------------------------------------------

int memoryBlock::memoryblock_count = 0;


// --- Constructor for objects that have declarations

memoryBlock::memoryBlock(declNode * decl, bool synthetic,
			 memoryBlock * container,
			 procNode * local_to)
  : _container(container),
    Defs(),
    Uses(),
    _parameter_assignments(),
    _current_use(0),
    _current_def(0),
    _components(),
    _decl(decl),
    _synthetic_decl(synthetic),
    _write_protected(false),
    _is_array(false),
    _is_array_element(false),
    _is_indexed(false),
    _is_allocation_object(false),
    _flow_sensitive(true),
    _unify(false), // TB_unify
    _unifytype(0),
    _single_assignment(true),
    _is_return_value(false),
    _local_to(local_to),
    // _name(),
    _allocation_site(0),
    // _initializer_def(0),
    _allocation_object(0),
    // _only_def(0),
    // _only_use(0),
    _input_to(),
    _destructive_assignments(),
    _complicit_assignments(),
    property(0),
    property_blocks()
{
  // If this new memoryBlock is contained by another, and that container is
  // a multiply-object representative, then set the flags on this block as
  // well.

  if (container) {
    if (container->is_indexed())
      set_indexed();

    if (container->is_heap_object())
      set_heap_object(container->allocation_site());

    if (container->is_return_value())
      set_return_value();
  }

  memoryblock_count++;
}

memoryBlock::~memoryBlock()
{
  if (_synthetic_decl && ! is_heap_object())
    delete _decl;

  clear();

  memoryblock_count--;

  if(unifyType()) unifyType()->block(NULL); // TB_unify
}

void memoryBlock::clear()
{
  if (is_array() && _current_def) {
    // want to delete _current_def, but in unify mode, possible that it it also
    // in Defs. Check to prevent double deletion.
    bool ok_delete = true;
    if(unifyType()) {
      const memorydef_list & defs = Defs.def_list();
      for(memorydef_list_cp d=defs.begin(); d!=defs.end(); d++)
        if(d->def == _current_def)
        { ok_delete = false; break; }
    }

    if(ok_delete)
      delete _current_def;
  }

  Defs.clear();
  Uses.clear();


  /*
  if (_initializer_def) {
    delete _initializer_def;
    _initializer_def = 0;
  }
  */
}

void memoryBlock::stats()
{
  cout << "  memoryBlock : " << memoryblock_count << " objects x " 
       << sizeof(memoryBlock) << " bytes = " << memoryblock_count * sizeof(memoryBlock) 
       << " bytes. " << endl;

  FieldNames.stats();
}

// ------------------------------------------------------------
//   Memory block operators
// ------------------------------------------------------------

// --- Managing uses and defs

// def_at: Create a memoryDef for the given location, inserting it
// into the orderedDefs list in the proper place. If an entry already
// exists for the location, just return that one.

memoryDef * memoryBlock::def_at(Location * where, bool & is_new)
{
  //  if (name() == "global") {
  //    memoryAccess::Verbose = true;
  //    cout << "Def at " << name() << endl;
  //  }

  if (write_protected())
    assert(0);

  if (memoryAccess::Verbose)
    cout << "memoryBlock::def_at " << name() << endl;

  if ( ! is_array()) {

    // -- Check flow sensitivity

    if (is_flow_sensitive()) {

      // -- Flow sensitive: create a def at the given location

      _current_def = Defs.make_def_at(where, this, is_new);
    }
    else {

      // -- Flow insensitive: the only def is the one at main

      if ( ! _current_def)
	_current_def = Defs.make_def_at(where, this, is_new);
      else
	is_new = false;
    }

    // -- For the adaptive algorithm, record if there are multiple
    // different defs (even if we don't store them).

    Location * cur_where = _current_def->where();

    if (_single_assignment) {
      if (cur_where->kind() != where->kind())
	_single_assignment = false;
      else {
	if (where->kind() == Location::Statement) {
	  stmtLocation * one = (stmtLocation *) cur_where;
	  stmtLocation * two = (stmtLocation *) where;

	  if (one->stmt() != two->stmt())
	    _single_assignment = false;
	}
	if (where->kind() == Location::Procedure) {
	  procLocation * one = (procLocation *) cur_where;
	  procLocation * two = (procLocation *) where;

	  if (one->proc() != two->proc())
	    _single_assignment = false;
	}
	if (where->kind() == Location::BasicBlock)
	  _single_assignment = false;
      }
    }
  }
  else {

    // -- Array objects: just set up the pointer to the elements

    setup_array_def();
    is_new = false;
  }

  //  memoryAccess::Verbose = false;

  if (_write_protected)
    cout << "WARNING: Def of write-protected object \"" << name() << "\"" << endl;

  //  if (name() == "inName_el_1")
  //    cout << "DEF AT = " << * (_current_def->where()) << endl;

  return _current_def;
}

// nearest_def_at: Given a program location, find the nearest strictly
// dominating assignment. If none exists, return null (not a good
// situation).

memoryDef * memoryBlock::nearest_def_at(Location * where)
{
  memoryDef * def =  0;

  if ( ! is_array()) {

    // -- Check flow sensitivity

    if (is_flow_sensitive()) {

      // -- Flow sensitive: find the dominating definition

      def = Defs.find_strictly_dominating_def(where);
    }
    else {

      // -- Flow insensitive: return the only def

      bool is_new;
      if ( ! _current_def)
	_current_def = Defs.make_def_at(procLocation::main(), this, is_new);

      def = _current_def;
    }
  }
  else
    def = setup_array_def();

  //  memoryAccess::Verbose = false;

  return def;
}

/** @brief Find the nearest interprocedural dominating def
 *
 * This version properly handles context-insensitivity by taking the
 * current call stack as input. */
/*
memoryDef * memoryBlock::nearest_def_at(Location * where,
					const procedurecall_stack & callstack)
{
  memoryDef * def = nearest_def_at(where);
  procedureInfo * current_info = 0;
  stmtLocation * current_callsite = 0;

  // -- Move up the call stack until we find a reaching def

  procedurecall_stack_crp stack_p = callstack.rbegin();

  while ( ! def &&
	  (stack_p != callstack.rend()))
    {
      current_callsite = (*stack_p).first;
      current_info = (*stack_p).second;

      // -- Short-circuit: don't both looking for a non-local def of a local
      // variable

      if (local_to() == current_info->proc())
	return 0;

      // -- See if there is a def that reaches the current callsite

      if (current_callsite)
	def = nearest_def_at(current_callsite);

      // -- Move up the call stack

      stack_p++;
    }

  return def;
}
*/

// use_at : Create a memoryUse for the given location and add it to
// the Uses map. If one already exists, just return that one.

// Special case: for arrays, we will set the reaching def to the special
// initializer def, that way we can treat things like "*A" and "A[0]"
// uniformly.

memoryUse * memoryBlock::use_at(Location * where)
{
  if (memoryAccess::Verbose)
    cout << "memoryBlock::use_at " << name() << endl;

  // cout << "Use at " << name() << endl;

  // -- Check flow sensitivity

  if (is_flow_sensitive()) {

    // -- Flow sensitive: make a use at the given location

    _current_use = Uses.make_use_at(where, this);
  }
  else {

    // -- Flow insensitive: there is only one use at the end of the main function

    if ( ! _current_use)
      _current_use = Uses.make_use_at(procLocation::main()->last(), this);
  }

  // -- Arrays are special: make sure all uses have a single reaching def
  // that points to the elements.

  if (is_array())
    memoryDef * def = setup_array_def();

  return _current_use;
}

memoryDef * memoryBlock::last_def_at(basicblockLocation * block)
{
  memoryDef * def = 0;

  if (! is_array()) {

    // -- Check flow sensitivity

    if (is_flow_sensitive()) {

      // -- Flow sensitive: find the def that dominates the last statement

      stmtLocation * last = block->last();

      def = Defs.find_dominating_def(last);
    }
    else {

      // -- Flow insensitive: return the only def

      bool is_new;
      if ( ! _current_def)
	_current_def = Defs.make_def_at(procLocation::main(), this, is_new);

      def = _current_def;
    }
  }
  else
    def = setup_array_def();

  return def;
}

memoryDef *  memoryBlock::find_def_at(Location * where)
{
  if ( ! is_array()) {

    // -- Check flow sensitivity

    if (is_flow_sensitive()) {

      // -- Flow sensitive: see if there is a def at the location

      return Defs.find_def(where);
    }
    else {

      // -- Flow insensitive: return the only def

      return _current_def;
    }
  }
  else
    return setup_array_def();
}

/** @brief Find a use
 *
 * See if there is a use of this block at the given location, returning
 * NULL if there isn't. This version does not work for merge uses. */

memoryUse * memoryBlock::find_use_at(Location * where)
{
  // -- Check flow sensitivity

  if (is_flow_sensitive()) {

    // -- Flow sensitive: see if there is a use at the location

    return Uses.find_use(where);
  }
  else {

    // -- Flow insensitive: return the only use
    
    return _current_use;
  }
}


/** @brief Find uses
 *
 * Return any uses at this location. */

void memoryBlock::find_uses_at(Location * where, memoryuse_set & uses)
{
  Uses.find_uses_at(where, uses);
}

/** @brief Add a parameter-assignment
 *
 * Record the procedure, callsite and reaching def. This version is used
 * for self-assignments when there is just one reaching def and it
 * belongs to this object. */

void memoryBlock::add_parameter_assignment(procNode * proc, stmtLocation * callsite,
					   memoryBlock * block)
{
  memoryblock_set temp;
  temp.insert(block);

  _parameter_assignments[proc][callsite] = temp;
}

/** @brief Add a parameter-assignment
 *
 * This version is used for regular formal parameter assignments, where
 * there could be multiple actual parameters coming from different
 * objects. */

void memoryBlock::add_parameter_assignment(procNode * proc, stmtLocation * callsite,
					   memoryblock_set & blocks)
{
  _parameter_assignments[proc][callsite] = blocks;
}

void memoryBlock::apply_weak_update(Location * current,
				    memoryDef * previous_def,
				    memoryuse_set & uses)
{
  // -- We must have a current def.

  memoryDef * block_def = current_def();

  // -- Mark the def as weak
  
  block_def->set_weak(true);

  // -- Collect the previous pointer values

  if (previous_def)
    block_def->add_pointers(previous_def->points_to());

  // -- We will also consider this to be a "use" of the old
  // value. This is necessary to make liveness analysis work
  // correctly (and probably other analyses as well).
	    
  memoryUse * weak_use = use_at(current);

  uses.insert(weak_use);

  weak_use->reaching_def(previous_def);
  weak_use->set_weak(true);
}

static unsigned long int preds_size_histogram[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static unsigned long int preds_size_count = 0;

// Create the set of merge uses (the inputs to the phi function) at the
// given merge point.

void memoryBlock::setup_merge_uses_at(basicblockLocation * merge_location,
				      memoryDef * reaching_def,
				      basicblock_set & predecessors)
{
  //  if (name() == "outName_el_3")
  //    cout << "    Setup Merge " << name() << endl;

  /** We don't need this, now that Pointers::nearest_def_at works correctly

  // -- This is new: create the def right now. The question is whether we
  // need to do anything with the is_new value because it won't be new when
  // we actually hit the merge point.

  bool is_new = false;
  memoryDef * merge_def = def_at(merge_location, is_new);
  */

  // --- Current block

  basicblockNode * basicblock = merge_location->block();

  // --- Look up (or create) the set of uses for the merge (one for
  // each control-flow predecessor).

  const pred_use_map & pred_uses = Uses.make_merge_uses_at(merge_location, this);

  for (basicblock_set_p p = predecessors.begin();
       p != predecessors.end();
       ++p)
    {
      basicblockNode * predecessor = *p;

      // --- Find the merge-point use corresponding to the given
      // predecessor.

      pred_use_map_cp cp = pred_uses.find(predecessor);
      memoryUse * use = (*cp).second;

      // --- Set the reaching def field, and indicate NOT to search
      // for it later when the merge point is actually encountered.

      memoryDef * old_def = use->reaching_def();
      if (( ! old_def) ||
	  (old_def && Location::strictly_dominates(old_def->where(), reaching_def->where()))) {

	use->reaching_def(reaching_def);
	use->search_for_def(false);
      }

      /*
      if (name() == "outName_el_3") {
	cout << "      at " << * merge_location << " through pred " << predecessor->dfn() << endl;

	if (( ! old_def) ||
	    (old_def && Location::strictly_dominates(old_def->where(), reaching_def->where()))) {
	  cout <<   "      take trigger def at " << * (reaching_def->where()) << endl;
	  if (old_def) 
	    cout << "          over old def at " << * (old_def->where()) << endl;
	  else
	    cout << "         (no old def)" << endl;	
	}
	else {
	  cout <<   "      keep old def at " << * (old_def->where()) << endl <<
	            "  over trigger def at " << * (reaching_def->where()) << endl;
	}
      } */
    }
}

// merge_uses_at : (When a merge point is encountered) Look up the set
// of uses for a merge point (one for each control-flow
// predecessor). This set will already have been created by calls to
// setup_merge_uses_at above. The purpose of this function is simply
// to fill in any uses that don't have reaching definitions yet.

void memoryBlock::merge_uses_at(basicblockLocation * where,
				memoryuse_list & uses,
				cfg_edge_set & active_edges,
				memoryDef * dominating_def,
				bool computePointers)
{
  /*
  bool flag = false;
  if (decl()->name() == "nHeap") {
    flag = true;
    memoryAccess::Verbose = true;
  }
  */

  // cout << " MERGE " << name() << " at " << * where << endl;

  // cout << "    Merge " << name() << endl;
  
  // Get the uses associated with this merge point. There should be
  // one for each control-flow predecessor. They are returned as a map
  // from the particular predecessor to the use that represents it.

  const pred_use_map & pred_uses = Uses.make_merge_uses_at(where, this);

  basicblock_list & preds = where->block()->preds();
  const procLocation * proc_loc = where->proc_location();

  int size = preds.size();
  if (size != pred_uses.size())
    cerr << "ERROR: Wrong number of uses in a merge node." << endl;
  
  if (size < 2)
    cerr << "ERROR: Merge-point with only one predecessor." << endl;

  /* Debug stuff
  preds_size_count++;
  int temp = size;
  if (temp > 9) temp = 9;
  preds_size_histogram[temp]++;
  */

  // -- Get the current basic block (where the merge is occuring)

  basicblockNode * basicblock = where->block();

  // -- Visit the uses

  if (computePointers) {

    // --- TRUE Hypothesis: the reaching def for all the remaining phi
    // uses is the def that dominates the phi node itself.

    // --- For each basic block predecessor

    int skipped = 0;

    for (pred_use_map_cp cp = pred_uses.begin();
	 cp != pred_uses.end();
	 ++cp) 
      {
	basicblockNode * predecessor = (*cp).first;
	memoryUse * use = (*cp).second;

	// -- Conditional analysis

	if (pointerOptions::Conditional_analysis) {
	  
	  // -- Set a use to "inactive" if its incoming edge is not active.

	  cfg_edge_pair edge(predecessor, basicblock);
	  if (active_edges.find(edge) == active_edges.end()) {

	    // -- This use is ignored because it comes from a branch that
	    // we're not executing.

	    use->set_inactive();

	    if (pointerOptions::Verbose)
	      cout << "MERGE: skipping input of " << name() << " from inactive edge "
		   << predecessor->dfn() << " -> " << basicblock->dfn() << endl;
	  }
	  else {

	    // -- Active use: we executed the path that it comes from

	    use->set_active();

	    // -- Find its reaching def

	    if (use->search_for_def())
	      use->reaching_def(dominating_def);

	    // -- Add to the return results

	    uses.push_back(use);
	  }
	}
	else {

	  // -- Regular analysis

	  // -- This is key: any merge uses that don't already have their
	  // reaching defs set (and have the search_for_def flag true) are
	  // reached by the def that dominated the merge point itself.
	  
	  if (use->search_for_def())
	    use->reaching_def(dominating_def);

	  // -- Add to the return results

	  uses.push_back(use);
	}
      }
  }
  else {

    // We are no longer building the def-use chains, so just get the list
    // of uses.

    for (pred_use_map_cp cp = pred_uses.begin();
	 cp != pred_uses.end();
	 ++cp) 
      {
	memoryUse * use = (*cp).second;
	uses.push_back(use);
      }
  }
}

void memoryBlock::reset_current_def_use(Location * unused)
{
  // -- Reset the current_use field, but only for flow-sensitive objects.

  if (is_flow_sensitive() &&
      ! is_array() && !unifyType() /* TB_unify */) {
    _current_use = 0;
    _current_def = 0;
  }

  //  if (name() == "outName_el_3")
  //    cout << "RES outName_el_3 " << endl;

  /*
  // -- Find the definition that reaches the procedure exit

  _current_def = nearest_def_at(last_stmt);

  if (name() == "outName_el_3") {
    cout << "Reset " << name() << " at " << * last_stmt << " reaching def at " << * _current_def->where() << endl;
  }
  */
}

void memoryBlock::set_current_def_use(Location * where)
{
  if ( ! is_array()) {

    // -- Check flow sensitivity

    if (is_flow_sensitive()) {

      // -- Flow sensitive: look up def and use at the given location
      
      _current_def = Defs.find_def(where);
      _current_use = Uses.find_use(where);
    }
    else {

      // -- Flow insensitive: there is only one use and def, and we store
      // them in the current_use/current_def fields.

      //      _current_def = _only_def;
      //      _current_use = _only_use;
    }
  }
}

// ------------------------------------------------------------
// Managing structure fields
// ------------------------------------------------------------

// Dot "." operator: retrieve a sub-component by name, assuming the
// memoryBlock is a struct-like thing. If the field doesn't exist,
// then create one

void memoryBlock::dot(const string & field_name,
		      declNode * field_decl,
		      memoryModel & Memory,
		      memoryblock_set & results)
{
  // cout << "DOT  " << name() << endl;

  memoryBlock * new_block = 0;

  // --- Map the field name using the field name database

  int field_num = FieldNames.get_field(field_name);

  // --- Try to find that component

  component_map_p p = _components.find(field_num);

  if (p == _components.end()) {

    // TB_unify
    if(Memory.unification() && !is_unify() && decl() &&
       (decl()->decl_location()==declNode::PROC ||
        decl()->no_tdef_type() && decl()->no_tdef_type()->typ() == Func))
      return;
if(name() == "load_actions_file" && field_name=="filename") {
  cout << Memory.unification() << " " << is_unify() << " " << this << endl;
  if(decl()) {
    cout << decl()->name() << " loc=" << decl()->decl_location();
    if(decl()->no_tdef_type()) cout << " " << decl()->no_tdef_type()->typ();
    cout << endl;
  }
  assert(false);
}

    // -- Create the new field, with this object as the container

    new_block = Memory.generate_su_field(field_name, field_decl, this);

    // --- Store the new field object in the parent object

    _components[field_num] = new_block;
    
    // --- Also, assume the owning procedure is the same as that of
    // the container.

    new_block->_local_to = _local_to;

    results.insert(new_block);
  }
  else {

    // --- Found it

    results.insert((*p).second);
  } 
}

void memoryBlock::set_array_element(memoryBlock * element)
{
  // -- Store the array element in the special component position

  _components[FieldNameDB::ArrayField] = element;

  // -- Set the is_array flag

  _is_array = true;

  // --- Also, assume the owning procedure is the same as that of
  // the container.

  element->_local_to = _local_to;

  // -- Set the container pointer on the element

  element->_container = this;

  // -- Set the flag on the element

  element->_is_array_element = true;

  // -- Array objects are not writable

  if(! unifyType()) // TB_unify
    set_write_protected();
}

memoryBlock * memoryBlock::get_array_element()
{
  // -- The array element is the special -1 field

  component_map_p p = _components.find(FieldNameDB::ArrayField);
  if (p != _components.end()) {

    // --- Found it

    return (*p).second;
  }
  else
    return 0;
} 

memoryDef * memoryBlock::setup_array_def()
{
  // -- Make sure this special def is set up

  if ( ! _current_def) {

    // -- Make this block point to it's element in all contexts

    _current_def = new memoryDef(procLocation::main(), this);

    memoryBlock * element = get_array_element();
    if ( element ) {

      memoryblock_set temp;

      temp.insert(element);
      _current_def->add_pointers(temp);
    }
  }

  // -- Always make this def the reaching def.
  // use->reaching_def(_initializer_def);

  return _current_def;
}

// ------------------------------------------------------------
//  Reachability
// ------------------------------------------------------------

void memoryBlock::reachable_blocks(Location * where,
				   bool has_use_here,
				   memoryblock_list & worklist,
				   memoryblock_set & already_seen,
				   memoryBlock * null,
                                   bool components_only)
{
  // -- Find the nearest dominating def, if there is one

  if(! components_only) {
    memoryDef * def = 0;

    if (has_use_here) {

      // -- Already have a use: just get it's reaching def

      memoryUse * use = find_use_at(where);
      if (use)
        def = use->reaching_def();
    }
    else {

      // -- Need to compute the reaching def

      def = nearest_def_at(where);
    }

    if (def) {

      // -- Look at the reachable blocks, adding only those that haven't been
      // seen before.

      const memoryblock_set & pointers = def->points_to();
      for (memoryblock_set_cp p = pointers.begin();
           p != pointers.end();
           ++p)
        {
          memoryBlock * mb = *p;
          if ((mb != null) &&
              (already_seen.find(mb) == already_seen.end())) {

            // -- We haven't seen this one yet

            already_seen.insert(mb);
            worklist.push_back(mb);
          }
        }
    }
  }

  // -- Add all the field components...

  for (component_map_p p = _components.begin();
       p != _components.end();
       ++p)
    {
      memoryBlock * mb = (*p).second;
      if (already_seen.find(mb) == already_seen.end()) {

	// -- We haven't seen this one yet

	already_seen.insert(mb);
	worklist.push_back(mb);
      }
    }
}

// ------------------------------------------------------------
//  Heap allocation object
// ------------------------------------------------------------

memoryBlock * memoryBlock::get_allocation_object(memoryModel & Memory)
{
  memoryBlock * current = this;

  if (pointerOptions::Verbose)
    cout << "    + Get allocation object for " << name() << endl;

  // -- Find the top-most object container

  while (current->_container) {
    if(current == current->_container) break;
    current = current->_container;
  }

  if (current->_allocation_object == 0) {

    // -- Treat it like a struct field of the object

    memoryBlock * alloc_object = Memory.generate_su_field(string("__alloc"),
							  (declNode *)0, this);

    // -- Mark it as special

    alloc_object->_is_allocation_object = true;

    // -- Inherit flow sensitivity, or look up in the list

    if (current->is_unify()) {
      if(! alloc_object->is_unify()) {
        pointerOptions::Unify_objects++;
        alloc_object->set_unify(true);
      }
      alloc_object->set_flow_insensitive();
    } else if (current->is_flow_sensitive())
      alloc_object->set_flow_sensitive();
    else
      alloc_object->set_flow_sensitivity(pointerOptions::Flow_sensitive_allocation_objects);

    // -- Store it on the heap object

    current->_allocation_object = alloc_object;

    if (pointerOptions::Verbose) {
      cout << "      -> Not found: create " << alloc_object->name();
      if (alloc_object->is_flow_sensitive())
	cout << "  (flow-sensitive)" << endl;
      else
	cout << "  (not flow-sensitive)" << endl;
    }
  }

  return current->_allocation_object;
}

/** @brief Return the allocation object
 *
 * Returns null if the block has no allocation object. */

memoryBlock * memoryBlock::allocation_object()
{
  memoryBlock * current = this;

  // -- Find the top-most object container

  while (current->_container) {
    if(current == current->_container) break;
    current = current->_container;
  }

  return current->_allocation_object;
}


Multiplicity memoryBlock::at_allocation(Location * current,
					memoryDef * reaching_def,
					memoryblock_set & defs,
					memoryuse_set & uses,
					memoryblock_set & changes)
{
  bool changed = false;

  // -- Set up a def of the heap object

  bool is_new;
  memoryDef * def = def_at(current, is_new);

#ifdef COLLECT_DEFS
  defs.insert(this);
#endif

  // -- If this allocation resulted in a new memoryDef instance, then it
  // automatically is a "change".

  if (is_new) {
    changes.insert(this);
    changed = true;
  }

  // -- Get the multiplicity value of the reaching definition

  Multiplicity reaching_multiplicity = Unallocated;

  if (reaching_def)
    reaching_multiplicity = reaching_def->multiplicity();

  // -- Reading the multiplicity is a use of this object

  memoryUse * use = use_at(current);
  use->reaching_def(reaching_def);
  uses.insert(use);

  // -- Save the old multiplicity value

  Multiplicity old_multiplicity = def->multiplicity();

  // -- Compute the new multiplicity value. This is essentially the
  // transfer function for allocation.

  Multiplicity new_multiplicity;

  switch (reaching_multiplicity) {
  case Unallocated:
  case Deallocated: new_multiplicity = Single;
    break;
  case Single:      new_multiplicity = Unbounded;
    break;
  case Bounded:     new_multiplicity = Bounded;
    break;
  case Unbounded:   new_multiplicity = Unbounded;
    break;
  default:          new_multiplicity = Error;
    break;
  }

  // -- Handle flow-insensitivity: in order to be safe all allocations make
  // the object Unbounded.

  if ( ! is_flow_sensitive())
    new_multiplicity = Unbounded;

  // -- Mark the def with the new value. If it changed, record this as a
  // change to the object.
    
  if (old_multiplicity != new_multiplicity) {

    def->set_multiplicity(new_multiplicity);
	
    changes.insert(this);
    changed = true;
  }

  // -- Print some debug info

  if (pointerOptions::Verbose) {
    cout << "  Allocate: " << _container->name() << " -- ";

    print_multiplicity(reaching_multiplicity, cout);
    cout << " -> ";
    print_multiplicity(new_multiplicity, cout);
    cout << " -- ";

    if (changed)
      cout << "changed";
    else
      cout << "unchanged";

    cout << endl;
  }

  return new_multiplicity;
}

Multiplicity memoryBlock::at_deallocation(Location * current,
					  memoryDef * reaching_def,
					  memoryblock_set & defs,
					  memoryuse_set & uses,
					  memoryblock_set & changes)
{
  bool changed;

  // -- Set up a def of the heap object

  bool is_new;
  memoryDef * def = def_at(current, is_new);

#ifdef COLLECT_DEFS
  defs.insert(this);
#endif

  // -- If this deallocation resulted in a new memoryDef instance, then it
  // automatically is a "change".

  if (is_new) {
    changes.insert(this);
    changed = true;
  }

  // -- Get the multiplicity value of the reaching definition

  Multiplicity reaching_multiplicity = Unallocated;

  if (reaching_def)
    reaching_multiplicity = reaching_def->multiplicity();

  // -- Reading the multiplicity is a use of this object

  memoryUse * use = use_at(current);
  use->reaching_def(reaching_def);
  uses.insert(use);

  // -- Save the old multiplicity value

  Multiplicity old_multiplicity = def->multiplicity();

  // -- Compute the new multiplicity value. This is essentially the
  // transfer function for the deallocation.

  Multiplicity new_multiplicity;

  switch (reaching_multiplicity) {
  case Unallocated:
  case Deallocated: new_multiplicity = Unallocated;
    break;
  case Single:      new_multiplicity = Deallocated;
    break;
  case Bounded:     new_multiplicity = Bounded;
    break;
  case Unbounded:   new_multiplicity = Unbounded;
    break;
  default:          new_multiplicity = Error;
    break;    
  }

  // -- Handle flow-insensitivity: in order to be safe all allocations make
  // the object Unbounded.

  if ( ! is_flow_sensitive())
    new_multiplicity = Unbounded;

  // -- Mark the def with the new value. If it changed, record this as a
  // change to the object.
    
  if (old_multiplicity != new_multiplicity) {

    def->set_multiplicity(new_multiplicity);
	
    changes.insert(this);
    changed = true;
  }

  // -- Print some debug info

  if (pointerOptions::Verbose) {
    cout << "  Deallocate: " << _container->name() << " -- ";

    print_multiplicity(reaching_multiplicity, cout);
    cout << " -> ";
    print_multiplicity(new_multiplicity, cout);
    cout << " -- ";

    if (changed)
      cout << "changed";
    else
      cout << "unchanged";

    cout << endl;
  }

  return new_multiplicity;
}

void memoryBlock::print_multiplicity(Multiplicity m, ostream & out)
{
  switch (m) {
  case Unallocated: out << "unallocated";
    break;
  case Deallocated: out << "deallocated";
    break;
  case Single: out << "single";
    break;
  case Bounded: out << "bounded";
    break;
  case Unbounded: out << "unbounded";
    break;
  case Error: out << "(error)";
    break;
  case Top: out << "(top)";
    break;
  default: out << "(unknown)";
    break;
  }
}

// ------------------------------------------------------------
//  Manage precision analysis
// ------------------------------------------------------------

/** @brief Add destructive assignment
 *
 * Add the given cause and location to the destructive assignment
 * list. If it already exists, the cause is overwritten. */

void memoryBlock::add_destructive_assignment(Location * where, DestructiveKind cause)
{
  _destructive_assignments[where] = cause;

  if (pointerOptions::Verbose) {
    cout << " Monitor: add destructive assignment of " << name() << " at " << * where;
    switch (cause) {
    case Control_flow:   cout << " -- control-flow merge " << endl;
      break;
    case Parameter_pass: cout << " -- parameter pass" << endl;
      break;
    case Weak_update:    cout << " -- multiplicity weak update" << endl;
      break;
    case Additive:       cout << " -- flow-insensitive assignment" << endl;
      break;
    default: cout << " -- ERROR unknown cause" << endl;
      break;
    }
  }
}

/** @brief Add complicit assignment
 *
 * Add the given objects to the complicit assignment set for the given
 * location. */

void memoryBlock::add_complicit_assignment(Location * where, memoryblock_set & objects)
{
  if ( ! objects.empty()) {

    memoryblock_set & comps = _complicit_assignments[where];

    // -- Loop through the objects, skipping any write-protected objects
    // (how could it be their fault?)

    for (memoryblock_set_p p = objects.begin();
	 p != objects.end();
	 ++p)
      {
	memoryBlock * comp = *p;

	if ( ! comp->write_protected())
	  comps.insert(comp);
      }
    
    if (pointerOptions::Verbose) {
      cout << " Monitor: add complicit assignment of " << name() << " at " << * where << endl;
      cout << "            <= ";
      for (memoryblock_set_p p = objects.begin();
	   p != objects.end();
	   ++p)
	cout << (*p)->name() << " ";
      cout << endl;
    }
  }
}

/** @brief Add complicit assignment
 *
 * Add one object to the complicit assignment set for the given
 * location. */

void memoryBlock::add_complicit_assignment(Location * where, memoryBlock * object)
{
  _complicit_assignments[where].insert(object);
  if (pointerOptions::Verbose) {
    cout << " Monitor: add complicit assignment of " << name() << " at "
         << * where << " <= " << object->name() << endl;
  }
}

/** @brief Add to flow-sensitivity list
 *
 * This method adds the block to the given flow-sensitivity list. It does
 * this by adding a flow_sensitive_pair object containing a statement and a
 * declaration, which can be used to identify the block on subsequent
 * analysis passes. We need this capability because the memoryBlocks
 * themselves are destroyed and recreated on each pass. */

void memoryBlock::add_to_flow_sensitive_list(flow_sensitive_set & flow_sensitive_objects)
{
  stmtNode * the_stmt = 0;
  declNode * the_decl = 0;
  string field;

  if (container())
    field = decl()->name();

  // -- If it's a heap object, then the allocation site matters

  if (is_heap_object())
    the_stmt = allocation_site()->stmt();

  // -- Find the top-most containing object

  memoryBlock * cur = this;
  while (cur->container()) {
    if(cur == cur->container()) break; //TB_unify
    cur = cur->container();
  }

  // -- Get the declaration, if it's not synthetic
  
  if ( ! cur->is_synthetic_decl())
    the_decl = cur->decl();

  // -- Make the identifying pair

  FSKey key(the_stmt, the_decl, field);

  // -- Add it to the list

  flow_sensitive_set_p found = find(flow_sensitive_objects.begin(),
				    flow_sensitive_objects.end(),
				    key);

  if (found == flow_sensitive_objects.end()) {
    flow_sensitive_objects.push_back(key);
    /*
    cout << "NEW fs object: " << name() << " -- key is "
	 << "(" << the_stmt << "," << the_decl << "," << field << ")" << endl;
    if (the_decl)
      cout << " declared at " << the_decl->coord() << endl;
    */
  }

  if (pointerOptions::Verbose) {

    if (found == flow_sensitive_objects.end())
      cout << "Adaptivity: Record block " << name() << " flow-sensitive -- key is "
	   << "(" << the_stmt << "," << the_decl << "," << field << ")" << endl;
  }
}

/** @brief Find in flow-sensitivity list
 *
 * Return true if the block is identified in the given list. */

bool memoryBlock::is_in_flow_sensitive_list(flow_sensitive_set & flow_sensitive_objects)
{
  // -- Make the identifying key (same as above method)

  stmtNode * the_stmt = 0;
  declNode * the_decl = 0;
  string field;

  // -- If it's a heap object, then the allocation site matters

  if (is_heap_object())
    the_stmt = allocation_site()->stmt();

  if (container())
    field = decl()->name();

  // -- Find the top-most containing object

  memoryBlock * cur = this;
  while (cur->container()) {
    if(cur == cur->container()) break; //TB_unify
    cur = cur->container();
  }

  // -- Get the declaration, if it's not synthetic
  
  if ( ! cur->is_synthetic_decl())
    the_decl = cur->decl();

  // -- Make the identifying pair

  FSKey key(the_stmt, the_decl, field);

  // -- Add it to the list

  flow_sensitive_set_p found = find(flow_sensitive_objects.begin(),
				    flow_sensitive_objects.end(),
				    key);

  if (pointerOptions::Verbose) {

    if (found != flow_sensitive_objects.end()) {
      cout << "Adaptivity: Set block " << name();

      if (found != flow_sensitive_objects.end())
	cout << " flow-sensitive ";
      else
	cout << " flow-insensitive ";
    
      cout << "-- key is "
	   << "(" << the_stmt << "," << the_decl << "," << field << ")" << endl;
    }
  }

  return (found != flow_sensitive_objects.end());
}

/** @brief Set flow sensivity
 *
 * Check the given list of flow-sensitive objects to see if this block is
 * identified on the list. Depending on the analysis mode, set the
 * flow-sensitivity accordingly. */

void memoryBlock::set_flow_sensitivity(flow_sensitive_set & flow_sensitive_objects)
{
  if (pointerOptions::Flow_insensitive) {

    // -- See if it's in the list

    if (is_in_flow_sensitive_list(flow_sensitive_objects)) {

      // -- Found: override the default

      set_flow_sensitive();
    }
    else {

      // -- Not found; inherit from the container, or just default

      /* NO, now we handle structure fields.
      if (container()) {
	if (container()->is_flow_sensitive())
	  set_flow_sensitive();
	else
	set_flow_insensitive();
	}
	else
      */

      set_flow_insensitive();
    }
  }
  else {

    // -- Flow sensitive mode

    set_flow_sensitive();
  }
}


/** @brief Add to non-unify list */
void memoryBlock::add_to_non_unify_list(UnifyTypes & non_unify_types)
{
  if (unifyType()) {
    non_unify_types.insert(unifyType());

    if (pointerOptions::Verbose) {
      cout << "Adaptivity: Record block " << name() << " non-unify\n";
    }
  }
}

/** @brief Find in flow-sensitivity list
 *
 * Return true if the block is identified in the given list. */

void memoryBlock::set_unification(UnifyTypes & non_unify_types) {
  if (! pointerOptions::Unification) {

    // -- See if it's in the list

    if (! unifyType() ||
        memoryModel::is_in_non_unify_list(non_unify_types, unifyType())) {

      // -- Found: override the default

      set_unify(false);
      _unifytype = NULL; // so that if set_unification() is called again on
                         // this object, we can skip is_in_non_unify_list().
    }
    else {
      if(! is_unify()) {
        pointerOptions::Unify_objects++;
        set_unify(true);
      }
    }

    if (pointerOptions::Verbose) {
      cout << "Adaptivity: Set block " << name();

      if (is_unify())
        cout << " as unified ";
      else
        cout << " as non-unified ";
      cout << endl;
    }
  }
  else {

    // -- Unify mode

    if(! is_unify() && unifyType()) {
      set_unify(true);
      pointerOptions::Unify_objects++;
    }
  }
}


// ------------------------------------------------------------

/** @brief Top-most container
 *
 * Traverse the container chain up to find the top-most container. */

memoryBlock * memoryBlock::top_most_container()
{
  memoryBlock * temp = this;

  while (temp->_container) {
    if(pointerOptions::Unification && temp == temp->_container) break;
    temp = temp->_container;
  }

  if(is_unify() && decl() && decl()->type()) {
    // debug. type()->not annotated var. e.g. annotated global var may have a
    // field but it won't have container.
    memoryblock_set tops = top_most_containers();
    if(tops.find(temp) == tops.end()) {
      cout << "this " << name();
      if(unifyType()) cout << " " << *unifyType();
      cout << "\ntemp " << name() << endl;
      cout << "tops: ";
      for(memoryblock_set_p t=tops.begin(); t!=tops.end(); t++)
        cout << (*t)->name() << " " << *(*t)->unifyType() << endl;
    }
    assert(tops.find(temp) != tops.end());
  }
if(is_unify()) cout << name() << endl;
  assert(! is_unify()); // should not use this function in this mode
  return temp;
}

memoryblock_set memoryBlock::containers() const {
  memoryblock_set result;
  if(unifyType()) {
    set<Unify_ECR*> parents;
    switch(unifyType()->objTyp()) {
      case BOTTOM: break;
      case BLANK: parents = unifyType()->blank()->p.parents(); break;
      case SIMPLE: parents = unifyType()->simple()->p.parents(); break;
      case STRUCTURE: parents = unifyType()->structure()->p.parents(); break;
      case OBJECT: parents = unifyType()->object()->p.parents();
    }
    for(set<Unify_ECR*>::iterator p=parents.begin(); p!=parents.end(); p++)
      if((*p)->type()->block())
        result.insert((*p)->type()->block());
  }
  return result;
} // containers


memoryblock_set memoryBlock::top_most_containers() {
  if(!unifyType()) {
    if(is_allocation_object() || property)
      return container()->top_most_containers();
    if(decl()->decl_location() == declNode::ENUM ||
       decl()->type() && decl()->type()->typ() == Func) {
      memoryblock_set r;
      r.insert(this);
      return r;
    }
    if(decl()->type() && decl()->no_tdef_type()->typ() == Ptr &&
       decl()->no_tdef_type()->no_tdef_type()->typ() == Func ||
       !decl()->type() /* annotated variable */ ) {
      if(container()) return container()->top_most_containers();
      memoryblock_set r;
      r.insert(this);
      return r;
    }
    cout << this << " " << name() << endl;
    if(decl()) cout << decl()->name() << decl()->coord() << endl;
    if(local_to()) cout << "local_to " << local_to()->decl()->name() << endl;
    assert(false);
  }
  memoryblock_set visited;
if(pointerOptions::Verbose)
cout << "top_most_containers on " << name() << endl;
  return top_most_containers(visited);
} // top_most_containers


memoryblock_set memoryBlock::top_most_containers(memoryblock_set &visited) {
  if(visited.find(this) != visited.end()) return memoryblock_set();
  visited.insert(this);
if(pointerOptions::Verbose)
cout << "top_most_containers (helper) on " << name() << endl;
  memoryblock_set parents = containers(),
                  result;
  for(memoryblock_set_p p=parents.begin(); p!=parents.end(); p++) {
    memoryblock_set more = (*p)->top_most_containers(visited);
    result.insert(more.begin(), more.end());
  }
  if(parents.empty()) result.insert(this);
  else if(result.empty()) { // cycle/dag detected
    if(parents.size() == 1 && *parents.begin() == this) result = parents;
    /*else {
      cout << "I am " << name() << " " << *unifyType() << "\nparents:\n";
      for(memoryblock_set_p p=parents.begin(); p!=parents.end(); p++) {
        cout << (*p)->name();
        if((*p)->unifyType()) cout << " " << *(*p)->unifyType();
        cout << endl;
      }
      assert(false); // what to do?? TBD
    }*/
  }
if(pointerOptions::Verbose) {
cout << "top_most_containers on " << name() <<" returns "<< result.size()<<endl;
if(result.size()==1) cout << "  " << (*result.begin())->name() << endl;
}
  return result;
} // top_most_containers (helper)

// ------------------------------------------------------------
// Def use chains
// ------------------------------------------------------------

/** @brief Def uses 
 *
 * Collect all the uses reached by the given definition. */

void memoryBlock::def_uses(memoryDef * def,
			   memoryuse_list & uses)
{
  Uses.def_uses(def, uses);
}

// --- Update the def-use chains (only performed once at the end of
// analysis by the memoryModel).

void memoryBlock::update_def_use_chains()
{
  Uses.update_def_use_chains();
}

// ------------------------------------------------------------
// Prune uses and defs
// ------------------------------------------------------------

void memoryBlock::prune_defs_uses()
{
  Defs.prune(Uses);
}

// ------------------------------------------------------------
// Scoping
// ------------------------------------------------------------

// Return true if the given memory-block is "in-scope": that is, it is
// variable that is local to a particular procedure and that procedure
// is in the given call stack.

bool memoryBlock::in_scope(basicblockLocation * where) const
{
  if ( _local_to == 0)
    return true;

  const procLocation * cur = where->proc_location();
  while (cur) {
    if (cur->proc() == _local_to)
      return true;
    cur = cur->parent_proc();
  }

  return false;
}

// ------------------------------------------------------------
// Printing and debug
// ------------------------------------------------------------

// --- Statistics

void memoryBlock::stats(ostream & out, bool header,
			long int & global_defs,
			long int & global_merge_defs,
			long int & global_weak_defs,
			long int & global_uses,
			long int & global_merge_uses)
{

  int total_defs;
  int merge_defs;
  int weak_defs;
  int total_uses;
  int merge_uses;

  Defs.stats(total_defs, merge_defs, weak_defs);
  Uses.stats(total_uses, merge_uses);

  global_defs += total_defs;
  global_merge_defs += merge_defs;
  global_weak_defs += weak_defs;
  global_uses += total_uses;
  global_merge_uses += merge_uses;

  if (pointerOptions::Show_memoryblocks) {

    out.width(35);
    if (header)
      out << "memoryBlock";
    else
      out << name();

    out.width(6);
    if (header)
      out << "#defs";
    else
      out << total_defs;

    out.width(7);
    if (header)
      out << "#merge";
    else
      out << merge_defs;

    out.width(6);
    if (header)
      out << "#weak";
    else
      out << weak_defs;

    out.width(6);
    if (header)
      out << "#uses";
    else
      out << total_uses;

    out.width(7);
    if (header)
      out << "#merge";
    else
      out << merge_uses;

    out.width(12);
    if (header)
      out << "local-to";
    else {
      if (_local_to)
	out << _local_to->decl()->name();
      else
	out << "(non-local)";
    }

    out.width(6);
    if (header)
      out << "array";
    else {
      if (_is_array)
	out << "true";
      else
	out << "false";
    }

    out.width(6);
    if (header)
      out << "index";
    else {
      if (_is_indexed)
	out << "true";
      else
	out << "false";
    }

    out.width(6);
    if (header)
      out << "heap";
    else {
      if (is_heap_object())
	out << "true";
      else
	out << "false";
    }
    
    out << endl;
  }
}

void memoryBlock::print_def_use(ostream & o) const
{
  o << "memoryBlock: " << name();

  if ( ! _components.empty() ) {
    o << " {" << endl;
    for (component_map_cp p = _components.begin();
	 p != _components.end();
	 ++p)
      {
	o << "  " << FieldNames.field_name((*p).first) << " ";
	o << (*p).second->name() << ";" << endl;
      }
    o << "}" << endl;
  }
  else {
    o << endl;
    o << "  Defs: " << endl;
    Defs.print(o);
    o << "  Uses: " << endl;
    Uses.print(o);
  }
}

void memoryBlock::print(ostream & o, Location * path, Output_mode mode) const
{
  o << "memoryBlock: " << name();

  if (mode == NAME_ONLY) {
    o << endl;
    return;
  }

  if ( ! _components.empty() ) {
    o << " {" << endl;
    for (component_map_cp p = _components.begin();
	 p != _components.end();
	 ++p)
      {
	o << "  " << FieldNames.field_name((*p).first) << " ";
	(*p).second->print(o, path, mode);
      }
    o << "}";
  }
  else {
    memoryblock_list ps;
    o << endl;

    if (mode == ALL_VALUES) {

      /*
      // --- Print out the whole points-to relation

      for (path_pointer_list_cp p = _points_to.begin();
	   p != _points_to.end();
	   ++p)
	{
	  o << "     at " << (*p).path() << endl;
	  for (block_list::const_iterator q = (*p).begin();
	       q != (*p).end();
	       ++q)
	    {
	      o << "        " << (*q)->name() << endl;
	    }
	}
      */
    }
    else {

      // --- To see the status "after" the current statement (e.g., to
      // see the results of an assignment, we set the stmt_num ahead
      // one.

      /*

      if (mode == AFTER_ASSIGNMENT) {
	Path temp(*path);
	temp.next_stmt();
	star(&temp, ps);
      }
      else
	star(path, ps);
      */

      for (memoryblock_list_p p = ps.begin();
	   p != ps.end();
	   ++p)
	{
	  memoryBlock * pb = *p;

	  o << "        --> " << pb->name() << endl;
	}
    }
  }
}

string memoryBlock::name() const
{
  // if (_name.empty()) {

  string name0;

  if (container() && ! is_array_element()) {
    return container()->name() + "." + decl()->name();
  }
  else {

    string & name1 = _decl->name();

    if (decl()->type() &&
	decl()->type()->is_ellipsis())
      name1 = string("...");

    if (_local_to)
      return _local_to->decl()->name() + "::" + name1;
    else {

      if (_allocation_site) {

	procLocation * alloc_proc = Location::procedure(_allocation_site);

	if (alloc_proc->proc()->decl()->decl_location() == declNode::UNKNOWN)
	  {
	    // -- Allocated in a library routine, print the caller

	    procLocation * caller = Location::procedure(alloc_proc->stmt_location());
	    if (caller)
	      return caller->proc()->decl()->name() + "/" + name1;
	  }
      }

      return name1;
    }
  }

  // }
  // else
  //   return _name;
}

string memoryBlock::generate_su_field_name(const string & field) const
{
  //  if (_name.empty()) {

    string & name1 = _decl->name();

    if (decl()->type() &&
	decl()->type()->is_ellipsis())
      name1 = string("...");

    return name1 + "." + field;
    //  }
    //  else
    //    return _name + "." + field;
}

// ------------------------------------------------------------
//  Field-name database
// ------------------------------------------------------------

// Since the number of possible field names is small, we keep them in
// a database and then refer to them by number in the memoryBlock class.

memoryBlock::FieldNameDB memoryBlock::FieldNames;

int memoryBlock::FieldNameDB::get_field(const string & name)
{
  field_name_map_p p = _fields.find(name);
  if (p == _fields.end()) {
    ++_count;
    _fields[name] = _count;
    return _count;
  }
  else
    return (*p).second;
}

string memoryBlock::FieldNameDB::field_name(int index)
{
  field_name_map_p p = _fields.begin();

  while (p != _fields.end()) {
    if ((*p).second == index)
      return (*p).first;
    ++p;
  }
  return string("");
}

int memoryBlock::FieldNameDB::ArrayField = -1;
