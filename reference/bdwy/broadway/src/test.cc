
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

#include "broadway.h"

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

class palPhase : public Phase
{
private:

  string _ann_filename;
  bool _debug;
  bool _defs_uses;
  Annotations * anns;
  
public:

  void get_flags (str_list_p & arg) {
    _ann_filename = (*arg);
    ++arg;

    string & flag = (*arg);

    if (flag == "debug") {
      _debug = true;
      ++arg;
    }
    else
      _debug = false;

    flag = (*arg);
    if (flag == "defsuses") {
      _defs_uses = true;
      ++arg;
    }
    else
      _defs_uses = false;
  }

  void run()
  {
    Annotations * a = new Annotations(_ann_filename, CBZ::cpp_flags);
    if ( a->errors() == 0) {
      anns = a;

      if (_debug)
	cout << *a;

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

      // -- Find the main function

      procNode * main_proc = 0;
      unitNode * u = 0;
      bool found = false;
      for (unit_list_p up = CBZ::Program.begin();
	   (up != CBZ::Program.end()) && ! found;
	   ++up)
	{
	  u = (*up);

	  for (def_list_p dp = u->defs().begin();
	       (dp != u->defs().end()) && ! found;
	       ++dp)
	    {
	      defNode * dn = (*dp);
	      if (dn->typ() == Proc) {
		procNode * pr = (procNode *) dn;
		if (pr->decl()->name() == "main") {
		  main_proc = pr;
		  found = true;
		}
	      }
	    }
	}

      // -- Run the analysis

      // Linker::debug = _debug;
      Analyzer analyzer(anns, main_proc, u, _debug);

      cout << "*** Starting structure analysis ***" << endl;

      analyzer.analyze();

      if (_defs_uses)
	analyzer.uses_and_defs();

      cout << "*** Starting property analysis ***" << endl;

      Annotations::debug = _debug;

      propertyAnalyzer pa(_debug);

      pa.set_analysis_mode();
      analyzer.analyze(&pa);

      cout << "*** Generate reports ***" << endl;

      pa.set_reporting_mode();
      analyzer.analyze(&pa);

      if (_debug) {
	Location::stats();
	memoryBlock::stats();
	analyzer.stats();
	memoryAccess::stats();
      }
    }
  }
};

Phases PalPhase("pal", new palPhase());
