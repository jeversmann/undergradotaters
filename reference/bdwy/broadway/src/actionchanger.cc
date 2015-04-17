

#include "broadway.h"

actionChanger::actionChanger(Analyzer * analyzer, Annotations * annotations)
  : Changer(Both, Subtree, true),
    _analyzer(analyzer),
    _property_analyzer(analyzer->property_analyzer()),
    _annotations(annotations),
    _call(0)
{
  // -- NOTE: We traverse the tree both pre-order and post-order in order
  // to collect the callNodes properly.
}

Node * actionChanger::at_call(callNode * the_call, Order ord)
{
  _call = the_call;
  return the_call;
}

Node * actionChanger::at_exprstmt(exprstmtNode * the_exprstmt, Order ord)
{
  if (ord == Preorder) {

    // -- On the pre-order visit, just reset the _call to null. If it's
    // non-null on the post-order visit, that means that this statement
    // contains a procedure call.

    _call = 0;
  }
  else {

    // -- Post-order visit; only process statements with procedure calls.

    if (_call) {

      const procedures_map & procs = _annotations->procedures();

      // -- Figure out which procedure is being called

      procedureAnn * proc_anns = 0;
      callsiteEntry * callsite = 0;

      for (procedures_map_cp p = procs.begin();
	   p != procs.end();
	   ++p)
	{
	  proc_anns = (*p).second;
	  callsite = proc_anns->find_callsite(the_exprstmt);
	  if (callsite)
	    break;
	}

      if (callsite) {

	// -- If we found a library call, see if there is an applicable
	// action.

	actionAnn * action = proc_anns->find_applicable_action(the_exprstmt);

	// -- If there is one, apply it

	if (action) {

	  // -- Make sure all the defs and uses are set

	  const proclocation_set & proclocations = callsite->contexts();
	  procLocation * libproc_location = * (proclocations.begin());
	  callNode * the_call = callsite->call();
	  _analyzer->setup_all_bindings(proc_anns, the_call, libproc_location);

	  // -- Generate the replacement code

	  stmtNode * repl = action->generate_replacement(callsite, _property_analyzer, _annotations);
	  if (repl) {

	    // -- Show the original code in a comment

	    ostringstream ostr;
	    ostr << " -- Action: ";
	    if (action->condition())
	      action->condition()->print(ostr);
	    output_context oc(ostr);
	    the_exprstmt->output(oc, 0);

	    repl->comment() = ostr.str();

	    if (Annotations::Show_actions) {
	      cout << " -- Action: ";
	      if (action->condition())
		action->condition()->print(cout);
	      cout << " --------------------------------------------" << endl;

	      output_context oc(cout);
	      cout << "   Original code:" << endl;
	      the_exprstmt->output(oc, 0);
	      cout << endl;

	      cout << "   Replacement code:" << endl;
	      repl->output(oc, 0);
	      cout << endl;
	    }

	    // -- Return the new code

	    return repl;
	  }
	}
      }
    }
  }

  return the_exprstmt;
}
