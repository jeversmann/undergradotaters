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
#include "path.h"

// ------------------------------------------------------------
// Path
// ------------------------------------------------------------

const Path * Path::Always = new Path();

Path::Path(PathDB * db, const Path * parent,
		 procNode * proc)
  : Location(proc, 0, 0),
    _parent(0),
    _depth(0),
    // _refs(0),
    _dominates_exit(false),
    _db(0)
{
  if (parent) {
    _parent = db->intern(parent);
    _depth = 1 + _parent->depth();
    // _parent->inc_refs();
  }
}

Path::Path(PathDB * db, const Path * parent,
		 procNode * proc, basicblockNode * block)
  : Location(proc, block, 0),
    _parent(0),
    _depth(0),
    // _refs(0),
    _dominates_exit(false),
    _db(0)
{
  if (parent) {
    _parent = db->intern(parent);
    _depth = 1 + _parent->depth();
    // _parent->inc_refs();
  }
}

Path::Path(PathDB * db, const Path * parent,
		 const Location & loc)
  : Location(loc),
    _parent(0),
    _depth(0),
    // _refs(0),
    _dominates_exit(false),
    _db(0)
{
  if (parent) {
    _parent = db->intern(parent);
    _depth = 1 + _parent->depth();
    // _parent->inc_refs();
  }
}

// --- Copy constructor: copy the given object, but make it unrecorded

Path::Path(const Path & other)
  : Location(other),
    _parent(other._parent),
    _depth(other._depth),
    // _refs(0),
    _dominates_exit(false),
    _db(0)
{
  // if (_parent)
  //  _parent->inc_refs();
}

// --- Private default constructor for creating internal static paths

Path::Path()
  : Location(0, 0, 0),
    _parent(0),
    _depth(0),
    // _refs(0),
    _dominates_exit(false),
    _db(0)
{}

// --- Destructor

Path::~Path()
{
  if (_db)
    cerr << "ERROR: Attempted to delete a recorded path:" << *this << endl;

  // if (_refs)
  //  cerr << "ERROR: Attempted to delete a referenced path:" << *this << endl;

  // if (_parent)
  //   _parent->dec_refs();
}

// --- Check for recursion in the path sequence

bool Path::is_recursive() const
{
  if (is_always())
    return false;

  procNode * cur = proc();
  const Path * par = parent();

  while (par) {
    if (par->proc() == cur)
      return true;
    par = par->parent();
  }

  return false;
}

bool Path::is_proc_present(procNode * cur_proc) const
{
  if (is_always())
    return false;

  const Path * par = this;

  while (par) {
    if (par->proc() == cur_proc)
      return true;
    par = par->parent();
  }

  return false;
}


// ------------------------------------------------------------
// PathDB
// ------------------------------------------------------------

PathDB::PathDB()
  : _paths()
{}

// Equal_path is a comparator class for use in the find_if
// algorithm.

class Equal_path
{
public:
  const Path * _path_to_look_for;

  Equal_path(const Path * path_to_look_for)
    : _path_to_look_for(path_to_look_for)
  {}

  bool operator()(const pair<const Path * const, const Path *> & entry) {
    const Path * cur = entry.second;
    if (cur == _path_to_look_for)
      return true;

    return Location::equals(*cur, *_path_to_look_for);
  }
};  

// --- Look up a path, without creating a new entry. If the path
// doesn't exist in the database, then it returns null.

const Path * PathDB::lookup(const Path * con)
{
  if (con) {

    if (con->is_always())
      return con;

    // --- Invariant: the parent must already be interned

    const Path * parent = con->parent();

    // --- First, look up all paths under the given parent

    pair<path_map_cp, path_map_cp> range = _paths.equal_range(parent);

    // --- Search for the proper location

    path_map_cp p = find_if(range.first, range.second, Equal_path(con));

    if ((p != _paths.end()) &&
	(p != range.second))
      return (*p).second;
  }

  return 0;
}

// Record a path in the database, eliminating duplicates and
// sharing internal structure.

const Path * PathDB::intern(const Path * con)
{
  if (con) {

    const Path * orig = lookup(con);

    if (orig)
      return orig;
    else {

      // --- Create a new entry

      bool dom_exit;
      if ( ! con->is_always())
	dom_exit = Location::strictly_dominates(*con,
						Location::exit(*con));
      else
	dom_exit = true;

      Path * new_intern = new Path(*con);
      new_intern->_dominates_exit = dom_exit;
      new_intern->_db = this;

      // --- Insert it into the map, indexed by it's parent

      path_map_pair entry(con->parent(), new_intern);
      _paths.insert(entry);

      return new_intern;
    }
  }
  else
    cout << "ERROR: Interning a null path" << endl;
  
  return 0;
}  

bool PathDB::equals(const Path * one, const Path * two)
{
  if (one == two)
    return true;

  if ( ! (one && two))
    return false;

  if (Location::equals(*one, *two))
    return (one->parent() == two->parent());

  return false;
}

// --- Path dominance: this method extends the concept of dominance
// to the full call-graph. However, it is specifically tailored for
// tracking assignments when dealing with path prefixes (see cases 2
// and 3 below).

// Given two paths DOM and CUR, the function works as follows:

// 1. If DOM and CUR are identical, return false

// 2. If DOM is a prefix of CUR, return false. This logic for this is
// that we want assignments inside a function call to precede (and be
// visible to) the assignment of its return value.

// 3. If CUR is a prefix of DOM, return true (see logic above).

// 4. If DOM and CUR have the same parent path, compare dominance
// at the local level. Otherwise, move up until we find a common
// parent in which to perform local dominance comparison. In addition,
// we need to make sure that DOM dominates the exit nodes of each
// path that we move up through. This can be visualized in this
// example:

// DOM = {main:6, foo:3, bar:8, zag:2 }
// CUR = {main:6, foo:3, bar:5, cats:6, dogs:2, fish:7}

// DOM and CUR are identical up to foo:3. So, if bar:8 dominates
// bar:5, AND zag:2 dominates the exit of function zag, the DOM
// dominates CUR. We don't need to worry about the chain down from
// bar:5 because clearly any path through fish:7 must pass through
// bar:5.


bool PathDB::dominates(const Path * dom, const Path * cur)
{
  if (equals(dom, cur))
    return true;

  return strictly_dominates(dom, cur);
}

bool PathDB::strictly_dominates(const Path * dom, const Path * cur)
{
  //  cout << "   Compare " << *dom << " >?> " << *cur << endl;

  // CASE 1: Equal paths, no strict dominance

  if (equals(dom, cur))
    return false;

  // CASE 1.5: "Always" dominates everything

  if (dom->is_always())
    return true;

  int depth_dom = dom->depth();
  int depth_cur = cur->depth();
  int min_depth = (depth_dom < depth_cur) ? depth_dom : depth_cur;
  int tmp;

  const Path * dom_p = dom;
  const Path * cur_p = cur;

  // --- Move up the dom call chain, collecting the dominates-exit values.

  bool dom_exit = true;

  while (dom_p && (dom_p->depth() > min_depth)) {
    dom_exit = dom_exit && dom_p->dominates_exit();
    dom_p = dom_p->parent();
  }

  // --- Move up the cur call chain

  while (cur_p && (cur_p->depth() > min_depth))
    cur_p = cur_p->parent();

  // CASE 2: Dominator is a prefix of the current: return false
  // CASE 3: Curhistoryrent is a prefix of the dominator: return true

  //  cout << "      Sub-Compare " << *dom_p << " >?> " << *cur_p << endl;

  if (equals(dom_p, cur_p))
    return (depth_dom > depth_cur);

  // --- Find a common parent

  while (dom_p && cur_p &&
	 (! equals(dom_p->parent(), cur_p->parent()))) {
    dom_exit = dom_exit && dom_p->dominates_exit();
    dom_p = dom_p->parent();
    cur_p = cur_p->parent();
  }   

  // --- CASE 4: Found a common parent, so test dominance locally

  if (dom_p && cur_p) {
    bool res = (Location::strictly_dominates( *dom_p, *cur_p ) && dom_exit);
    return res;
  }

  // --- CASE 5: This shouldn't happen

  cerr << "ERROR: Cannot determine path dominance." << endl;
  return false;
}

// --- Print some statistics

void PathDB::stats()
{
  cout << "CLASS: PathDB" << endl;
  cout << "  Total number of path nodes: " << _paths.size() << endl;
  cout << "  Total memory used (bytes): " <<
    _paths.size() * (sizeof(path_map::value_type) + sizeof(Path)) << endl;
}

void PathDB::print(ostream & o) const
{
  for (path_map_cp p = _paths.begin();
       p != _paths.end();
       ++p)
    {
      const Path * cur = (*p).second;
      cur->print_shallow(o);
      o << " in ";
      cur->parent()->print_shallow(o);
      o << endl;
    }
}

