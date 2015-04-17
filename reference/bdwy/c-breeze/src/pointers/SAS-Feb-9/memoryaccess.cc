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
#include "memoryaccess.h"
// #include <limits>

/* #include "mergepoints.h" */

// ------------------------------------------------------------
// memoryAccess
// ------------------------------------------------------------

bool memoryAccess::Verbose = false;
unsigned int memoryAccess::def_count = 0;
unsigned int memoryAccess::use_count = 0;
unsigned int memoryAccess::merge_use_count = 0;

memoryAccess::memoryAccess(Location * where, memoryBlock * owner)
  : _where(where),
    _is_weak(false),
    _multiplicity(Unallocated),
    _search_for_def(true),
    _active(true),
    _owner(owner)
{}

void memoryAccess::stats()
{
  cout << "  memoryDef : " << def_count << " objects x " 
       << sizeof(memoryDef) << " bytes = " << def_count * sizeof(memoryDef) 
       << " bytes. " << endl;

  cout << "  memoryUse : " << use_count << " objects x " 
       << sizeof(memoryUse) << " bytes = " << use_count * sizeof(memoryUse) 
       << " bytes. " << endl;
  cout << "  memoryuse_list : " << sizeof(memoryuse_list) << " bytes" << endl;
  cout << "  memoryblock_set : " << sizeof(memoryblock_set) << " bytes" << endl;
}

// ------------------------------------------------------------
// memoryDef
// ------------------------------------------------------------

memoryDef::memoryDef(Location * where, memoryBlock * owner)
  : memoryAccess(where, owner),
    // _uses(),
    _points_to(),
    // _previous(),
    _value(0),
    _self_assign_use(0)
{
  memoryAccess::def_count++;
}

// --- Destructor

memoryDef::~memoryDef()
{
  memoryAccess::def_count--;
} 

// --- Add a whole bunch of points-to edges

void memoryDef::add_pointers(const memoryblock_set & mbs)
{
  _points_to.insert(mbs.begin(), mbs.end());
}

void memoryDef::clear_points_to()
{
  _points_to.clear();
}

// --- Output

void memoryDef::print(ostream & o) const
{
  o << "    Def at " << * where() << endl;

  /*
  for (memoryuse_list_cp p = _uses.begin();
       p != _uses.end();
       ++p)
    {
      o << "  ";
      (*p)->print(o);
    }
  */
}

// ------------------------------------------------------------
// orderedDefs
// ------------------------------------------------------------

// --- Find the memoryDef for a given program location

memoryDef * orderedDefs::find_def(Location * where)
{
  memorydef_location_map_cp q = _quick_lookup.find(where);

  if (q != _quick_lookup.end())
    return (*q).second;
  else
    return 0;
}

// --- Find the nearest strictly dominating memoryDef

memoryDef * orderedDefs::find_strictly_dominating_def(Location * where)
{
  /*
  Pred_defs_strictly_dominates comparator(where);
  memorydef_list_cp p = find_if(_defs.begin(), _defs.end(), comparator);

  if (p == _defs.end())
    return 0;
  else
    return *p;
  */

  bool dominates = false;
  memoryDef * dominator = 0;
  Location * current = 0;

  int looking_for_subtree = where->subtree_id();
  unsigned int looking_for_min = where->tree_min();
  unsigned int looking_for_max = where->tree_max();

  // -- Regular case: search the list for a dominator

  memorydef_list_cp end = _defs.end();
  memorydef_list_cp p = _defs.begin();

  while (p != end) {
    const memorydef_key & key = (*p);

    current = key.location;

    if (current != where) {

      // dominates = Location::strictly_dominates(current, where);

      if (current->subtree_id() == looking_for_subtree) {

        unsigned int dom_min = current->tree_min();
	unsigned int dom_max = current->tree_max();

	if ((dom_min < looking_for_min) && (looking_for_max <= dom_max)) {
	  dominator = key.def;
	  break;
	}
      }
    }

    ++p;
  }

  return dominator;
}

// --- Find the nearest dominating memoryDef

memoryDef * orderedDefs::find_dominating_def(Location * where)
{
  /*
  Pred_defs_dominates comparator(where);
  memorydef_list_cp p = find_if(_defs.begin(), _defs.end(), comparator);

  if (p == _defs.end())
    return 0;
  else
    return *p;
  */

  bool dominates = false;
  memoryDef * dominator = 0;
  Location * current = 0;

  int looking_for_subtree = where->subtree_id();
  unsigned int looking_for_min = where->tree_min();
  unsigned int looking_for_max = where->tree_max();

  // if (looking_for_max == 0) looking_for_max = numeric_limits<unsigned int>::max();

  // -- Regular case: search the list for a dominator

  memorydef_list_cp end = _defs.end();
  memorydef_list_cp p = _defs.begin();

  while (p != end) {
    const memorydef_key & key = (*p);

    current = key.location;

    if (current == where) {
      dominator = key.def;
      break;
    }

    // dominates = Location::strictly_dominates(current, where);

    if (current->subtree_id() == looking_for_subtree) {

      unsigned int dom_min = current->tree_min();
      unsigned int dom_max = current->tree_max();
      
      // if (dom_max == 0) dom_max = numeric_limits<unsigned int>::max();

      if ((dom_min < looking_for_min) && (looking_for_max <= dom_max)) {
	dominator = key.def;
	break;
      }
    }

    ++p;
  }

  return dominator;
}

// --- Add a new memoryDef in the proper place, and return a pointer
// to it. If there is already an entry for the given path, just return
// that one.

memoryDef * orderedDefs::make_def_at(Location * where, memoryBlock * owner, bool & is_new)
{
  // --- First, see if it already exists

  is_new = false;

  memorydef_location_map_p q = _quick_lookup.find(where);

  if (q != _quick_lookup.end())
    return (*q).second;

  // --- Not there, so figure out where it should be

  if (memoryAccess::Verbose)
    cout << "make_def_at: dominator for " << * where << endl;
 
  //  Pred_defs_dominates comparator(where);

  bool dominates = false;
  memoryDef * dominator = 0;
  Location * current = 0;

  int looking_for_subtree = where->subtree_id();
  unsigned int looking_for_min = where->tree_min();
  unsigned int looking_for_max = where->tree_max();

  // if (looking_for_max == 0) looking_for_max = numeric_limits<unsigned int>::max();

  // -- Regular case: search the list for a dominator

  memorydef_list_p end = _defs.end();
  memorydef_list_p p = _defs.begin();

  while (p != end) {
    const memorydef_key & key = (*p);

    current = key.location;

    /** This should never happen
    if (current == where) {
      dominator = key.def;
      break;
    }
    */

    // dominates = Location::strictly_dominates(current, where);

    if (current->subtree_id() == looking_for_subtree) {

      unsigned int dom_min = current->tree_min();
      unsigned int dom_max = current->tree_max();
      
      // if (dom_max == 0) dom_max = numeric_limits<unsigned int>::max();
 
      if ((dom_min < looking_for_min) && (looking_for_max <= dom_max)) {
	dominator = key.def;
	break;
      }
    }

    ++p;
  }

  /*
  p = _defs.begin();
  while ((p != _defs.end()) &&  ! Location::dominates((*p)->where(), where)) {

    if (memoryAccess::Verbose)
      cout << "  NOT dominated by " << * (*p)->where() << endl;

    ++p;
  }
  */
  
  if (memoryAccess::Verbose)
    if (p != _defs.end())
      cout << "  dominated by " << * current << endl;

  // p = find_if(_defs.begin(), _defs.end(), comparator);

  // --- Add an entry with the proper path

  memoryDef * new_def = new memoryDef(where, owner);

  memorydef_key new_entry(new_def);
  _defs.insert(p, new_entry);

  _quick_lookup[where] = new_def;
    
  is_new = true;

  // --- Return a reference to the new element

  return new_def;
}

// --- Prune out unnecessary defs

void orderedDefs::prune(orderedUses & Uses)
{
  memorydef_list_p p = _defs.begin();
  memorydef_list_p to_remove;

  while (p != _defs.end()) {
    memoryDef * cur = (*p).def;

    to_remove = p;
    ++p;

    // --- Look for unused merges

    /*
    if (cur->uses().empty()
	// TBD: && mergePoints::is_merge_point(cur->where())
	)
      {
	_defs.erase(to_remove);
	Uses.prune(cur->where());
	// delete cur;
      }
    */
  }
}

// --- Output

void orderedDefs::print(ostream & o) const
{
  for (memorydef_list_cp p = _defs.begin();
       p != _defs.end();
       ++p)
    (*p).def->print(o);
}

// --- Clear

void orderedDefs::clear()
{
  for (memorydef_list_cp p = _defs.begin();
       p != _defs.end();
       ++p)
    delete (*p).def;

  _defs.clear();
}

// --- Stats

void orderedDefs::stats(int & total_defs, int & merge_defs, int & weak_defs)
{
  total_defs = 0;
  merge_defs = 0;
  weak_defs = 0;

  for (memorydef_list_p p = _defs.begin();
       p != _defs.end();
       ++p)
    {
      memoryDef * def = (*p).def;

      total_defs++;

      if (def->where()->kind() == Location::BasicBlock)
	merge_defs++;

      if (def->is_weak())
	weak_defs++;
    }
}

// ------------------------------------------------------------
// memoryUse
// ------------------------------------------------------------

memoryUse::memoryUse(Location * where, memoryBlock * owner)
  : memoryAccess(where, owner),
    _reaching_def(0)
{
  memoryAccess::use_count++;
}

// --- Destructor

memoryUse::~memoryUse()
{
  memoryAccess::use_count--;
}

void memoryUse::reaching_def(memoryDef * def)
{
  if (memoryAccess::Verbose) {
    cout << "-> Use at " << * where();

    if (def)
      cout << " setting reaching def to " << * def->where();
    else
      cout << " no reaching def, ";

    if (_reaching_def)
      cout << " was " << * _reaching_def->where() << endl;
    else
      cout << " no previous reaching def" << endl;
  }

  _reaching_def = def;
}

// --- Output

void memoryUse::print(ostream & o) const
{
  o << "    Use at " << * where() << endl;
  o << "  ";
  if (_reaching_def)
    _reaching_def->print(o);
  else
    o << " (no reaching def)" << endl;
}

// ------------------------------------------------------------
// orderedUses
// ------------------------------------------------------------

// --- Find an existing use for the given location (doesn't work for
// merge/phi functions because it assumes that there is only one). If
// it can't be found, return null.

memoryUse * orderedUses::find_use(Location * where) const
{
  if (where) {
    memoryuse_map_cp p = _uses.find(where);
    if (p != _uses.end())
      return (*p).second;
  }

  return 0;
}

// -- Find uses at (works for phi functions)

void orderedUses::find_uses_at(Location * where, memoryuse_set & uses)
{
  memoryUse * use;

  // -- Regular use

  use = find_use(where);
  if (use) {
    uses.insert(use);
    return;
  }

  // -- Merge use

  if (where->kind() == Location::BasicBlock) {

    basicblockLocation * bb = (basicblockLocation *) where;

    merge_use_map_p found = _merge_uses.find(bb);
    if (found != _merge_uses.end()) {

      pred_use_map & pum = (*found).second;
      
      for (pred_use_map_p p = pum.begin();
	   p != pum.end();
	   ++p)
	{
	  uses.insert((*p).second);
	}
    }
  }
}

// --- Add a new use for the given location, or return the existing
// one.

memoryUse * orderedUses::make_use_at(Location * where, memoryBlock * owner)
{
  // --- First, see if it already exists

  memoryUse * res = find_use(where);
  if ( ! res ) {
    res = new memoryUse(where, owner);
    _uses.insert(memoryuse_map_pair(where, res));
  }

  return res;
}

// --- Add a new set of uses for a particular merge point: one use
// for each control-flow predecessor.

const pred_use_map & orderedUses::make_merge_uses_at(basicblockLocation * where, memoryBlock * owner)
{
  // --- Check to see if there is already a set of uses.

  merge_use_map_p found = _merge_uses.find(where);
  if (found == _merge_uses.end()) {

    // --- Not found, create one for each control-flow predecessor

    _merge_uses[where] = pred_use_map();
    found = _merge_uses.find(where);
    pred_use_map & new_uses = (*found).second;

    // --- Populate the predecessor map. Each use is paired with the
    // basicblock predecessor that it represents.

    basicblock_list & preds = where->block()->preds();
    for (basicblock_list_p p = preds.begin();
	 p != preds.end();
	 ++p) {
      memoryUse * use = new memoryUse(where, owner);
      new_uses[*p] = use;
    }
  }

  return (*found).second;
}

// --- Update the def-use chains (only performed once at the end of
// analysis by the memoryModel).

void orderedUses::update_def_use_chains()
{
  memoryUse * use;
  memoryDef * def;

  for (memoryuse_map_p p = _uses.begin();
       p != _uses.end();
       ++p)
    {
      use = (*p).second;
      def = use->reaching_def();
      /*
      if (def)
	def->uses().push_back(use);
      */
    }

  for (merge_use_map_cp q = _merge_uses.begin();
       q != _merge_uses.end();
       ++q)
    {
      const pred_use_map & pred_uses = (*q).second;

      for (pred_use_map_cp w = pred_uses.begin();
	   w != pred_uses.end();
	   ++w)
	{
	  use = (*w).second;
	  def = use->reaching_def();
	  /*
	  if (def)
	    def->uses().push_back(use);
	  */
	}
    }
}

void orderedUses::def_uses(memoryDef * def,
			   memoryuse_list & uses)
{
  for (memoryuse_map_p p = _uses.begin();
       p != _uses.end();
       ++p)
    {
      memoryUse * use = (*p).second;
      memoryDef * rdef = use->reaching_def();
      if (rdef == def)
	uses.push_back(use);
    }

  for (merge_use_map_cp q = _merge_uses.begin();
       q != _merge_uses.end();
       ++q)
    {
      const pred_use_map & pred_uses = (*q).second;

      for (pred_use_map_cp w = pred_uses.begin();
	   w != pred_uses.end();
	   ++w)
	{
	  memoryUse * use = (*w).second;
	  memoryDef * rdef = use->reaching_def();
	  if (rdef == def)
	    uses.push_back(use);
	}
    }
}

// --- Prune uses for a particular location

void orderedUses::prune(Location * where)
{
  // Find the use at this particular location

  memoryuse_map_p p = _uses.find(where);

  if (p != _uses.end()) {

    // Remove any references to it, and then delete it.

    memoryUse * use = (*p).second;
    memoryDef * def = use->reaching_def();
    if (def) {

      // Remove it from the uses list in the reaching definition

      memoryuse_list uses;
      /*
      def->owner()->def_uses(def, uses);
      memoryuse_list_p rem = find(uses.begin(), uses.end(), use);
      if (rem == uses.end()) {
	cout << "BAD BAD situation ";
	// def->owner()->print_def_use(cout);
	cout << endl;
      }
      else
	uses.erase(rem);
      */
    }

    // Delete the actual object
    
    delete use;

    // Remove the use pointers from the orderedUses list

    _uses.erase(p);
  }
}

// --- Output

void orderedUses::print(ostream & o) const
{
  for (memoryuse_map_cp p = _uses.begin();
       p != _uses.end();
       ++p)
    (*p).second->print(o);

  for (merge_use_map_cp q = _merge_uses.begin();
       q != _merge_uses.end();
       ++q)
    {
      const pred_use_map & pred_uses = (*q).second;

      for (pred_use_map_cp w = pred_uses.begin();
	   w != pred_uses.end();
	   ++w)
	(*w).second->print(o);
    }
}

// --- Clear

void orderedUses::clear()
{
  for (memoryuse_map_cp p = _uses.begin();
       p != _uses.end();
       ++p)
    delete (*p).second;

  for (merge_use_map_cp q = _merge_uses.begin();
       q != _merge_uses.end();
       ++q)
    {
      const pred_use_map & pred_uses = (*q).second;

      for (pred_use_map_cp w = pred_uses.begin();
	   w != pred_uses.end();
	   ++w)
	delete (*w).second;
    }

  _uses.clear();
  _merge_uses.clear();
}

// -- Stats

void orderedUses::stats(int & total_uses, int & merge_uses)
{
  merge_uses = _merge_uses.size();
  total_uses = _uses.size() + merge_uses;
}

// ----------------------------------------------------------------------
//  Assignments
// ----------------------------------------------------------------------

memoryAssignment * assignmentSet::assignment_at(Location * where,
						memoryBlock * block,
						bool create)
{
  assignment_key key(where, block);

  // -- See if it's already there

  assignment_map_p p = _assignments.find(key);
  if (p != _assignments.end())
    return (*p).second;

  // -- It's not there, should we create a new one?

  if (create) {
    memoryAssignment * result = new memoryAssignment(where);
    _assignments[key] = result;
    return result;
  }

  return 0;
}
