// $Id: path.h,v 1.4 2003/08/07 23:14:27 pnav Exp $
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

#ifndef CBZ_PATH_H
#define CBZ_PATH_H

#include "location.h"

class Path;

// ------------------------------------------------------------
// PathDB
// ------------------------------------------------------------

// Store the paths in a multi-map from parent to children

typedef multimap< const Path *, const Path * > path_map;
typedef path_map::iterator path_map_p;
typedef path_map::const_iterator path_map_cp;
typedef pair< const Path *, const Path * > path_map_pair;

class PathDB
{
private:

  path_map _paths;

public:

  PathDB();

  // --- Add a path to the database, eliminating duplicates and
  // sharing structure wherever possible.

  const Path * intern(const Path * con);

  // --- Look up a path, without creating a new entry. If the
  // path doesn't exist in the database, then it returns null.

  const Path * lookup(const Path * con);

  // --- Equality and dominance tests

  static bool equals(const Path * one, const Path * two);
  static bool dominates(const Path * dom, const Path * cur);
  static bool strictly_dominates(const Path * dom, const Path * cur);

  // --- Print some statistics

  void stats();

  // --- Output

  void print(ostream & o) const;

  friend ostream& operator<<(ostream & o, const PathDB & cdb) {
    cdb.print(o);
    return o;
  }
};

// ------------------------------------------------------------
// Path
// ------------------------------------------------------------

// A path is a chain of Locations. We use the PathDB class to
// allow "interning" of paths so that common components of the
// chain are shared. This also makes other comparisons more efficient
// because two identical paths will have the same pointer value.

// The PathDB passed into the constructors always interns the
// parent pointer. This maintains the invariant that all components of
// a path chain are interned, with the one exception of the
// last. This exception is primarily for performance: we don't want to
// end up with every single program position in the database.

class Path : public Location
{
  friend class PathDB;

private:

  const Path * _parent;
  int _depth;
  // int _refs;
  bool _dominates_exit;
  PathDB * _db;

  // --- Private default constructor

  Path();

  static const Path * Always;

public:

  // --- Constructor: build a new path by "pushing" a new procedure
  // on the end. The parent is automatically interned.

  Path(PathDB * db, const Path * parent,
       procNode * callee);

  Path(PathDB * db, const Path * parent,
       procNode * callee, basicblockNode * block);

  Path(PathDB * db, const Path * parent,
       const Location & loc);

  // --- Copy a path: this is useful for modifying one that has
  // been interned (and thus cannot be modified!).

  Path(const Path & other);

  // --- Destructor

  ~Path();

  // --- Fields

  inline const Path * parent() const { return _parent; }
  inline int depth() const { return _depth; }
  inline bool dominates_exit() const { return _dominates_exit; }

  // --- Useful query functions

  bool is_recursive() const;
  bool is_proc_present(procNode * proc) const;
  bool is_always() const { return this == Always; }

  // --- The special "Always" path

  static const Path * always() { return Always; }

  // --- Output

  void print_shallow(ostream & o) const {
    if (is_always())
      o << "Always";
    else
      print(o);
  }

  void print_deep(ostream & o) const {
    print_shallow(o);
    if (parent()) {
      o << ":";
      parent()->print_deep(o);
    }
  }

  friend ostream& operator<<(ostream & o, const Path & c) {
    c.print_deep(o);
    return o;
  }

private:

  // void inc_refs() { _refs++; }
  // void dec_refs() { _refs--; }

};

#endif // 
