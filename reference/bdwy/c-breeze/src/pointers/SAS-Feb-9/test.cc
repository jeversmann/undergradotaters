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

#include <bitset>

#include "semcheck.h"
#include "scope_walker.h"
#include "id_lookup_walker.h"
#include "callgraph_walker.h"
#include "ref_clone_changer.h"
#include "ssa.h"
#include "pointers.h"
#include "unreachable.h"
#include "dismantle.h"
#include "cfg.h"
#include "callgraph.h"
#include "dfpreds.h"

#include "constants.h"
#include "liveness.h"

#include <sys/time.h>
#include <unistd.h>

// ------------------------------------------------------------
// Make sure its a tree
// ------------------------------------------------------------

typedef set< Node * > node_set;
typedef node_set::iterator node_set_p;

class TreeFixer : public Changer
{
public:

  static void fix(Node * node)
  {
    TreeFixer tf;
    node->change(tf);
  }

private:

  node_set _nodes;

  TreeFixer()
    : Changer(Preorder, Subtree, false),
      _nodes()
  {}

public:

  Node * at_decl(declNode * the_node, Order ord)
  {
    node_set_p f = _nodes.find(the_node);
    if (f == _nodes.end()) {
      _nodes.insert(the_node);
      return the_node;
    }
    else {
      return ref_clone_changer::clone(the_node, false);
    }
  }
};

class count_walker : public Walker
{
private:

  int _id_count;
  int _const_count;

public:

  count_walker()
    : Walker(Preorder, Subtree),
      _id_count(0),
      _const_count(0)
  {}

  virtual void at_id(idNode * the_id, Order ord)
  {
    _id_count++;
  }

  virtual void at_const(constNode * the_const, Order ord)
  {
    _const_count++;
  }

  int final_id_count() const { return _id_count; }
  int final_const_count() const { return _const_count; }
};
      
class pointers_phase : public Phase
{
private:

  bool _debug;
  bool _defs_uses;
  bool _constants;
  bool _simplify;
  bool _deadcode;
  bool _stats;
  bool _callgraph;
  int _threshold;
  bool _use_mult;

public:

  void get_flags (str_list_p & arg) {

    // Defaults:

    _debug = false;
    _defs_uses = false;
    _constants = false;
    _simplify = false;
    _deadcode = false;
    _stats = false;
    _callgraph = false;
    _threshold = -1;
    _use_mult = true; 

    bool done = false;

    do {

      string & flag = (*arg);

      if (flag == "debug")
	_debug = true;
      else
	if (flag == "defsuses")
	  _defs_uses = true;
	else
	  if (flag == "constants")
	    _constants = true;
	  else
	    if (flag == "simplify")
	      _simplify = true;
	    else
	      if (flag == "deadcode")
		_deadcode = true;
	      else
		if (flag == "stats")
		  _stats = true;
		else
		  if (flag == "callgraph")
		    _stats = true;
		  else
		    if (flag == "mult")
		      _use_mult = true;
		    else
		      if (flag == "nomult")
			_use_mult = false;
		      else
			if (flag == "cs") {
			  ++arg;
			  _threshold = atoi((*arg).c_str());
			}
			else
			  done = true;

      if ( ! done )
	++arg;
    } while ( ! done );
  }

  void run()
  {

    cout << " *** Start Fixup *** " << endl;

    for (unit_list_p up = CBZ::Program.begin();
         up != CBZ::Program.end();
         ++up) {
      unitNode * u = (*up);

      // Dismantle

      Dismantle::dismantle(u, DEFAULT_DISMANTLER_FLAGS | INDEX_IS_PRIMITIVE);
      //Dismantle::dismantle(u, DEFAULT_DISMANTLER_FLAGS );

      // Build CFG

      cfg_changer cfgc;
      u->change (cfgc);

      // Remove unreachable code

      Unreachable::remove(u);

      // Fixup indentifiers

      id_lookup_walker::fixup(u, false);
      // semcheck_walker::check(u, false);
      TreeFixer::fix(u);
    }

    for (unit_list_p up = CBZ::Program.begin();
         up != CBZ::Program.end();
         ++up) {
      unitNode * u = (*up);
      procNode * main_proc = 0;

      // -- Find the main function

      for (def_list_p dp = u->defs().begin();
           dp != u->defs().end();
           ++dp) {
        defNode * dn = (*dp);
        if (dn->typ() == Proc) {
          procNode * pr = (procNode *) dn;
	  if (pr->decl()->name() == "main") {

	    if (_callgraph) {
	      Linker l;
	      callGraph cg(pr, l);
	      cg.show_all_contexts();
	    }
	    else {
	      Pointers * pointers = new Pointers(pr, u, _debug);

	      struct timeval before;
	      struct timeval after;

	      if (_use_mult) {
		pointers->turn_on_multiplicity();
		cout << "Multiplicity on" << endl;
	      }
	      else {
		pointers->turn_off_multiplicity();
		cout << "Multiplicity off" << endl;
	      }

	      cout << "*** Starting pointer analysis ***" << endl;

	      if (_threshold >= 0) {
		cout << " Context-sensitivity at " << _threshold << endl;
		pointers->set_context_sensitivity_threshold(_threshold);
	      }

	      // memoryAccess::Verbose = true;

	      gettimeofday(&before,0);

	      pointers->analyze();

	      gettimeofday(&after, 0);

	      if (_defs_uses)
		pointers->uses_and_defs();

	      cout << "Time: " << after.tv_sec - before.tv_sec << endl;
	      cout << "Calls to dominates: " << Location::dom_calls << endl;

	      if (_stats) {
		Location::stats();
		// memoryBlock::stats();
		pointers->stats(cout);
		memoryAccess::stats();
	      }

	      if (_constants) {
		int ids_before;
		int ids_after;
		int consts_before;
		int consts_after;

		count_walker before;
		count_walker after;

		u->walk(before);
		ids_before = before.final_id_count();
		consts_before = before.final_const_count();

		constantAnalyzer constants(_debug);

		// constantAnalysis::watch = new string("globalCrc");
		pointers->analyze(&constants);
		constantsChanger::optimize(u, &constants, _simplify);

		u->walk(after);
		ids_after = after.final_id_count();
		consts_after = after.final_const_count();

		if (_stats) {
		  cout << "Constant propagation results:" << endl;
		  cout << "   IDs: before = " << ids_before << " after = " <<
		    ids_after << " diff = " << ids_after - ids_before << endl;
		  cout << "   Consts: before = " << consts_before << " after = " <<
		    consts_after << " diff = " << consts_after - consts_before << endl;
		}

		pointers->analyze();
	      }

	      if (_deadcode) {
		livenessAnalyzer liveness(_debug);
		pointers->analyze(&liveness);
		deadcodeChanger::optimize(u, &liveness);
	      }

	      delete pointers;

	      /*
	      if (_stats) {
		Location::stats();
		memoryBlock::stats();
		memoryAccess::stats();
	      }
	      */
	    }
	  }
        }
      }

    }
  }
};

Phases make_pointers_phase ("pointers", new pointers_phase());

// ------------------------------------------------------------

class ssa_phase : public Phase
{
public:

  void run()
  {
    for (unit_list_p up = CBZ::Program.begin();
         up != CBZ::Program.end();
         ++up) {
      unitNode * u = (*up);
      TreeFixer::fix(u);
      for (def_list_p dp = u->defs().begin();
           dp != u->defs().end();
           ++dp) {
        defNode * dn = (*dp);
        if (dn->typ() == Proc) {
          procNode * pr = (procNode *) dn;
	  SSA ssa(pr);
	}
      }
    }
  }
};

Phases make_ssa_phase("ssa", new ssa_phase());

// ------------------------------------------------------------

class dfpreds_phase : public Phase
{
public:

  void run()
  {
    for (unit_list_p up = CBZ::Program.begin();
         up != CBZ::Program.end();
         ++up) {
      unitNode * u = (*up);
      TreeFixer::fix(u);
      for (def_list_p dp = u->defs().begin();
           dp != u->defs().end();
           ++dp) {
        defNode * dn = (*dp);
        if (dn->typ() == Proc) {
          procNode * pr = (procNode *) dn;
	  DFPreds dfpreds(pr);
	}
      }
    }
  }
};

Phases make_dfpreds_phase("dfpreds", new dfpreds_phase());


// ----------------------------------------------------------------------

/*
bool operator<(const constant & one, const constant & two)
{
  // -- CASE 1: Non value is the smallest element

  if (one.no_val() && two.no_val())
    return false;

  if (one.no_val())
    return true;

  if (two.no_val())
    return false;

  // -- CASE 2: Strings

  if (one.is_str() &&
      two.is_str())
    return (strcmp(one.Str(),two.Str()) < 0);

  // -- CASE 3: Pointers

  if (one.is_ptr() &&
      two.is_ptr())
    return one.Ptr() < two.Ptr();

  // -- CASE 4: Regular numeric types

  if (one.basic() == two.basic()) {
    constant result = constant::eval(Operators::table['<'], one, two);
    return result.Boolean();
  }
  else {

    // -- CASE 5: Two different basic types: Always return false?

    return one.basic() < two.basic();
  }
}
*/

class const_phase : public Phase
{
public:

  typedef set< constant > constant_set;
  typedef constant_set::iterator constant_set_p;



  void run()
  {
    constant_set temp;

    temp.insert(constant(1));
    temp.insert(constant(2));
    temp.insert(constant(3));

    constant_set_p p;

    p = temp.find(constant(2));
    if (p == temp.end())
      cout << "Not found (unexpected)" << endl;
    else
      cout << "Found (expected)" << endl;

    p = temp.find(constant(4));
    if (p == temp.end())
      cout << "Not found (expected)" << endl;
    else
      cout << "Found (unexpected)" << endl;

    p = temp.find(constant(2.0));
    if (p == temp.end())
      cout << "Not found (expected)" << endl;
    else
      cout << "Found (unexpected)" << endl;

    temp.insert(constant(2.0));

    p = temp.find(constant(2.0));
    if (p == temp.end())
      cout << "Not found (unexpected)" << endl;
    else
      cout << "Found (expected)" << endl;

    constant nv;

    nv.set_no_val();

    temp.insert(nv);

    constant nv2;

    nv2.set_no_val();

    p = temp.find(nv2);
    if (p == temp.end())
      cout << "Not found (unexpected)" << endl;
    else
      cout << "Found (expected)" << endl;

    temp.insert(nv);

    cout << "Size = " << temp.size() << endl;

  }
};

Phases make_const_phase("const", new const_phase());

