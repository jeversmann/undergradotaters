// $Id: main.cc,v 1.26 2006/08/10 15:40:23 abrown Exp $
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
#include "print_walker.h"
#include "ref_clone_changer.h"
#include "scope_walker.h"
#include "id_lookup_walker.h"
#include "goto_label_walker.h"
#include "cfg.h"
#include "bits.h"
#include "live.h"
#include "dead.h"
#include "gc_walker.h"
#include "dismantle.h"
#include "tree_checker.h"
#include "inliner.h"

#include <unistd.h>

/** @defgroup Phases phases defined in c-breeze (some of which are available from the command line).
 */

// ------------------------------------------------------------
// Predefined Phases
// ------------------------------------------------------------

/** @brief print ast phase
    @ingroup Phases
*/
class print_ast_phase : public Phase
{
public:

  void run()
  {
    for (unit_list_p p = CBZ::Program.begin();
	 p != CBZ::Program.end();
	 ++p)
      print_walker::print(*p, cout);
  }
};

Phases Print_AST("ast", new print_ast_phase());

/** @brief Output C code phase
    @ingroup Phases
*/
class output_c_phase : public Phase
{
public:

  void run()
  {
      for (unit_list_p p = CBZ::Program.begin();
       p != CBZ::Program.end();
	   ++p) {

	unitNode * the_unit = (*p);

	// haven't output any sue bodies for
	// this unit yet

	if (the_unit->output_file() == "(stdout)") {
	  output_context a = output_context(cout);
	  the_unit->output(a, 0);
	  cout << endl;
	}
	else {
	  ofstream outf(the_unit->output_file().c_str());
	  output_context b = output_context(outf);
	  the_unit->output(b, 0);
	  outf << endl;
	}
      }
  }
};

Phases Output_C("c-code", new output_c_phase());

/** @brief Use "cc" to generate object file
 *  @ingroup Phases
 */

class CCObj : public Phase {
public:
  void run() {
    for ( unit_list_p p = CBZ::Program.begin();
	  p != CBZ::Program.end();
	  p++ ) {
      unitNode * the_unit = *p;
      if ( the_unit->output_file() == "(stdout)" ) {
	cout << "Cannot compile source from stdin to object file" << endl;
	continue;
      }
      ofstream outf(the_unit->output_file().c_str());
      output_context out = output_context(outf);
      the_unit->output(out, 0);
      outf << endl;
      outf.close();

      FILE * cc_msgs;
      string cpp_flags;
      for ( str_list_p p = CBZ::cpp_flags->begin();
	    p != CBZ::cpp_flags->end();
	    p++ ) {
	cpp_flags += " " + (*p);
      }
      string cmd = CBZ::cc_cmd + cpp_flags + " " + CBZ::preproc_flags + 
	" -c -o " + the_unit->obj_file() + " " + the_unit->output_file();
#if !defined (_WIN32)
      cc_msgs = popen(cmd.c_str(), "r");
#else
      cc_msgs = _popen(cmd.c_str(), "r");
#endif /* !defined (_WIN32) */
      
      // TODO:  copy output from GCC to stdout?

#if !defined (_WIN32)
      pclose(cc_msgs);
#else
      _pclose(cc_msgs);
#endif /* !defined (_WIN32) */
    }
  }
};

Phases CCObj_Phase("ccobj", new CCObj());

/** @brief phase for the -dismantle-control flag.
 *  @ingroup Phases
 */
class DismantleControlChangerPhase : public Phase {
public:

  void run() {
    unit_list_p n;
    // dismantle each unit

    for (n=CBZ::Program.begin(); n != CBZ::Program.end(); n++)
      Dismantle::dismantle_control(*n);
  }
};

Phases dismantle_control_phase("dismantle-control", 
			       new DismantleControlChangerPhase());

/** @brief phase for the -dismantle flag.
 *  @ingroup Phases
 */
class DismantleChangerPhase : public Phase {
public:

  void run() {
    unit_list_p n;
    // dismantle each unit

    for (n=CBZ::Program.begin(); n != CBZ::Program.end(); n++)
      Dismantle::dismantle(*n);
  }
};

Phases dismantle_phase("dismantle", new DismantleChangerPhase());

/** @brief phase for the -cfg flag (implies -dismantle)
    @ingroup Phases
*/
class CfgPhase : public Phase {
public:
  void run() {
    unit_list_p n;
    for ( n = CBZ::Program.begin(); n != CBZ::Program.end(); n++ )
      cfg_changer::generate_cfg(*n);
  }
};

Phases make_cfg_phase("cfg", new CfgPhase());

/** @brief perform dead code elimination
    @ingroup Phases
*/
class DeadPhase : public Phase {
public:
  void run (void) {
    for (unit_list_p u = CBZ::Program.begin() ; u != CBZ::Program.end() ; u++)
      {
	cfg_changer::generate_cfg(*u);

	deadCodeEliminationChanger dcec;
	(*u)->change(dcec);
      }
  }
};

Phases deadPhase("dead", new DeadPhase());

/** @brief list all phases
    @ingroup Phases
*/
class ListPhase : public Phase {
public:
  void run (void) {
    phase_vec * pv = Phases::phases();
    for (phase_vec_p p = pv->begin() ; p != pv->end() ; p++) {
      Phase_entry * pe = (Phase_entry *) (*p);
      pe->print();
    }
  }
};

Phases listPhase("listphases", new ListPhase(), 
		 "This phase lists currently registered phases.");

/** @brief checks that the AST is indeed a tree.
    @ingroup Phases
*/
class TreeCheckerPhase : public Phase {
private:
  bool _verbose;
  bool _warning;
public:
  void run (void) {
    // an iterator through a list of translation units
    for (unit_list_p u = CBZ::Program.begin() ; u != CBZ::Program.end() ; u++) 
      {
	// an instance of tree checker walker
	TreeChecker tcw(_verbose, _warning);
	(*u)->walk(tcw);  // walk the AST using the Tree Checker walker
      }
  }

  void get_flags(str_list_p & arg) {
    string opt = *arg;
    if(strncmp("verbose:", opt.c_str(), 8) == 0) {
      _verbose = true;
      opt.erase(0,8);
      arg++;
    }
    else if(strncmp("warning:", opt.c_str(), 8) == 0) {
      _warning = true;
      opt.erase(0,8);
      arg++;
    }
  };

  void usage () {
    cout << "  It can be used to verify the AST both before and after running "
	 <<   "the dismantler." << endl
	 << "  If this phase is run after the code has been dismantled, "
	 <<   "then it will" << endl
	 << "  additionally check that only valid nodes appear in the AST." 
	 << endl
	 << "  usage: cbz -check-ast [verbose: | warning:] files..." << endl
	 << "    verbose:   Output all messages about what this phase is doing"
	 << endl
	 << "    warning:   Output warning and informative messages" << endl;
  }
};

Phases treeCheckerPhase("check-ast", new TreeCheckerPhase(), 
                         "This phase checks that the AST is indeed a tree.");

Phases fiPhase("fi", new fi());

// ------------------------------------------------------------
// Main program
// ------------------------------------------------------------

// --- Read in the command line

str_list * process_flags(int argc, char *argv[])
{
  str_list * args = new str_list();
  str_list * pre = new str_list();
  str_list_p p, pn;

  for (int i = 1; i < argc; ++i) {
    char s[1000];
    int k=0, j=0;
    // change all double quotes to backslash-double quotes, so
    // the shell will see them as literal quotes
    for(;argv[i][j]; j++) {
	if (argv[i][j] == '"')
		s[k++] = '\\';
	s[k++] = argv[i][j];
    }
    s[k] = 0;
    args->push_back(string(s));
  }

  p = args->begin();

  while (p != args->end()) {

    // After "--" the rest of the args are passed to the
    // preprocessor

    if (*p == "--") {
      ++p;
      pre->splice(pre->begin(), * args, p, args->end());
      break;
    }

    // -- Test for phases...

    if (Phases::phase_flag(p)) {
      // Nothing extra to do
    }
    else if (*p == "-format") {
      CBZ::FormatReadably = true;
      ++p;
    }
    else if (*p == "-cc" ) {
      p++;
      CBZ::cc_cmd = *p;
      p++;
    }
    else if (*p == "-ansi") {
      CBZ::ANSIOnly = true;
      ++p;
    }
    else if (*p == "-gcc") {
      CBZ::GCCisms = true;
      ++p;
    }
    else if (*p == "-P") {
      ++p;
      CBZ::preproc_flags = *p;
      ++p;
    }
    else if (*p == "-no-pre") {
      CBZ::Preprocess = false;
      ++p;
    }
    else if (*p == "-pre") {
      CBZ::ShowPreprocess = true;
      ++p;
    } else if (*p == "-incl") {
      CBZ::LeaveIncluded = true;
      ++p;
    }
    else if (*p == "-suff") {
      ++p;
      CBZ::output_suffix = *p;
      ++p;
    }
    else if (*p == "-W") {
      ++p;
      CBZ::WarningLevel = atoi((*p).c_str());
      if (CBZ::WarningLevel < 0 || CBZ::WarningLevel > 5) {
	cerr << "Error: Warning level must be between 0 and 5." << endl;
	CBZ::WarningLevel = 4;
      } else
	++p;
    }
    else if (*p == "-Wall") {
      CBZ::WarningLevel = 5;
      ++p;
    }
    else {
      // -- Add a source code file to process...
      string output_file;
      string obj_file;

      if (*p == "(stdin)") {
	output_file = string("(stdout)");
	obj_file = ""; // -ccobj won't compile from (stdin)
      } else {
	output_file = (*p);
	int suff_pos = output_file.rfind(CBZ::input_suffix);
	if (suff_pos >= 0 && suff_pos < (int)(*p).length())
	  output_file.replace(suff_pos,
			      CBZ::input_suffix.length(),
			      CBZ::output_suffix);
	else
	  output_file = string("(stdout)");

	obj_file = (*p);
	suff_pos = obj_file.rfind(CBZ::input_suffix);
	if ( suff_pos >= 0 && suff_pos < (int)(*p).length())
	  obj_file.replace(suff_pos,
			   CBZ::input_suffix.length(),
			   CBZ::obj_suffix);
	else
	  obj_file = ""; // -ccobj won't compile from (stdin)
      }

      CBZ::Program.push_back(new unitNode(*p, output_file, obj_file));
      ++p;
    }
  } // END while more flags

  // -- Deal with any inspecified flags...

  if (CBZ::ANSIOnly)
    CBZ::preproc_flags = CBZ::ansi_preproc_flags;
  else if (CBZ::GCCisms)
    CBZ::preproc_flags = CBZ::gcc_preproc_flags;

  //delete args;

  return pre;
}

/* ---
FILE * open_input(str_list * cpp_flags)
{
  FILE * in_file;
  bool std_in;

  std_in = (input_file == "(stdin)");
  if (! std_in &&
      (access(input_file.c_str(), F_OK) != 0) &&
      (access(input_file.c_str(), R_OK) != 0)) {
    cerr << "Unable to open input file " << input_file << endl;
    exit(1);
  }

  if (CBZ::Preprocess && ! std_in) {
    string command = CBZ::preproc;
    for (str_list_p p = cpp_flags->begin();
	 p != cpp_flags->end();
	 ++p)
      command += " " + (*p);
    command += " " + input_file;

    delete cpp_flags;

    if (CBZ::ShowPreprocess)
      cout << "Preprocess: " << command << endl;

    in_file = popen(command.c_str(), "r");

    if (! in_file) {
      cerr << "Unable to preprocess input file " << input_file << endl;
      cerr << "Command: " << command << endl;
      exit(1);
    }
  }
  else {
    if (! std_in) {
      in_file = fopen(input_file.c_str(), "r");

      if (! in_file) {
	cerr << "Unable to open input file " << input_file << endl;
	exit(1);
      }
    }
    else
      in_file = stdin;
  }

  return in_file;
}
-- */

int main(int argc, char * argv[])
{
  // -- Initialize operators

  Operators::init();

  // -- Process the command line arguments

  CBZ::cpp_flags = process_flags(argc, argv);
  if ( ! CBZ::cpp_flags )
	  return 1;

  // -- Parse the files

  for (unit_list_p p = CBZ::Program.begin();
       p != CBZ::Program.end();
       ++p)
    (*p)->parse(CBZ::cpp_flags);

  // -- Run the various phases

  Phases::run_all();

  // -- Debug some stuff

  // cout << "After parsing -------------------- " << endl;
  // Node::report();

  // delete CBZ::Program;

  // cout << "After deleting ------------------- " << endl;
  // Node::report();
}

