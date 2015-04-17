
#include "broadway.h"
#include "broadway.h"
#include "broadway.h"
#include "broadway.h"

#include "broadway.h"

// ------------------------------------------------------------
// Make sure its a tree
// ------------------------------------------------------------

class TreeFixer : public Changer
{
public:

  static void fix(Node * node)
  {
    TreeFixer tf;
    node->change(tf);
  }

private:

  node_list _nodes;

  TreeFixer()
    : Changer(Preorder, Subtree, false),
      _nodes()
  {}

public:

  Node * at_node(Node * the_node, Order ord)
  {
    node_list_p f = find(_nodes.begin(), _nodes.end(), the_node);
    if (f == _nodes.end()) {
      _nodes.push_back(the_node);
      return the_node;
    }
    else {
      return clone_changer::clone(the_node);
    }
  }
};

// ------------------------------------------------------------
// Phase
// ------------------------------------------------------------

class pal_phase : public Phase
{
private:

  string _ann_filename;

public:

  static Annotations * anns;

  void run()
  {
    Annotations * a = new Annotations(_ann_filename, CBZ::cpp_flags);
    if ( ! a->has_errors()) {
      anns = a;
      cout << *a;

      callgraph_walker::graph();

      for (unit_list_p up = CBZ::Program.begin();
	   up != CBZ::Program.end();
	   ++up) {
	unitNode * u = (*up);
	procNode * main_proc = 0;

	// --- Perform some house-keeping

	id_lookup_walker::fixup(u, false);
	semcheck_walker::check(u, false);
	// TOO SLOW: TreeFixer::fix(u);

	// -- Find the main function

	for (def_list_p dp = u->defs().begin();
	     dp != u->defs().end();
	     ++dp) {
	  defNode * dn = (*dp);
	  if (dn->typ() == Proc) {
	    procNode * pr = (procNode *) dn;
	    if (pr->decl()->name() == "main") {

	      // --- Run the structure analyzer

	      StructureAnalyzer sa(anns, pr, u);
	      sa.analyze();
	      memoryBlock::stats();
	    }
	  }
	}
      }
    }
  }

  void get_flags(str_list_p & arg)
  {
    _ann_filename = (*arg);
    ++arg;
  }
};

Annotations * pal_phase::anns = 0;

Phases PalPhase("pal", new pal_phase());

/*

class context_phase : public Phase
{
private:

  string _proc_name;
  string _context_name;

public:

  void run()
  {
    if (! pal_phase::anns) {
      cerr << "ERROR: No annotations loaded." << endl;
      return;
    }

    for (unit_list_p up = CBZ::Program.begin();
         up != CBZ::Program.end();
         ++up) {
      unitNode * u = (*up);
      for (def_list_p dp = u->defs().begin();
           dp != u->defs().end();
           ++dp) {
        defNode * dn = (*dp);
        if (dn->typ() == Proc) {
	  procNode * pr = (procNode *) dn;

	  if (pr->decl()->name() == _proc_name)
	    context_flowproblem::flow(pal_phase::anns, _context_name, pr);
	}
      }
    }
  }

  void get_flags(str_list_p & arg)
  {
    _proc_name = (*arg);
    ++arg;
    _context_name = (*arg);
    ++arg;
  }

};
    
Phases ContextPhase("context", new context_phase());

*/
