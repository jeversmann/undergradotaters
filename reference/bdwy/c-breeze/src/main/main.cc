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
#include "dismantle.h"
#include "goto_label_walker.h"
#include "cfg.h"
#include "bits.h"
#include "defuse.h"
#include "live.h"
#include "dead.h"
#include "localcopyprop.h"
#include "gc_walker.h"
#include "optimize.h"
#include "lir_gen_walker.h"
#include "lir_flow_walker.h"
#include "briggs_reg_alloc.h"
#include "dummy_reg_alloc.h"
#include "asm_gen_walker.h"

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

/** @brief phase for the -dismantle flag.
    @ingroup Phases
*/
class DismantleChangerPhase : public Phase {
  public:

  void run () {
    unit_list_p n;
    // dismantle each unit

    for (n=CBZ::Program.begin(); n!=CBZ::Program.end(); n++)
    Dismantle::dismantle (*n);
  }
};

Phases dismantle_phase ("dismantle", new DismantleChangerPhase());

/** @brief phase for the -cfg flag (implies -dismantle)
    @ingroup Phases
*/
class cfg_phase : public Phase {

public:

  void run () {
    unit_list_p n;
    // make a cfg for each unit
    for (n=CBZ::Program.begin(); n!=CBZ::Program.end(); n++)
      cfg_changer::generate_cfg (*n);
    gcWalker::collect();
  }

};

Phases make_cfg_phase ("cfg", new cfg_phase());

/** @brief dead code elimination
    @ingroup Phases
*/
class deadCodeElimPhase: public Phase {
	public:

	livenessWalker lw;
	deadCodeEliminationChanger dc;
	// unusedVariable... is unsafe for now, see dead.h
	unusedVariableCleanupChanger cc;
	LocalCopyPropChanger lcp;

	void run () {
		unit_list_p n;

		// for each unit...

		for (n=CBZ::Program.begin(); n!=CBZ::Program.end(); n++) {

			// local copy propagation
			// (one day we'll do it globally)

			(*n)->change (lcp);

			// liveness analysis

			(*n)->walk (lw);

			// dead code elimination

			(*n)->change (dc);

			// clean up unused variable declarations 

			(*n)->change (cc);
			gcWalker::collect();
		}
	}
};

Phases dead_code_phase ("dead", new deadCodeElimPhase ());

/** @brief optimization phase - does all of -cfg, -dead, and lots more
    @ingroup Phases
*/
class optPhase : public Phase {
public:

void run () {
	unit_list_p n;
	Optimizer opt;

	for (n=CBZ::Program.begin(); n!=CBZ::Program.end(); n++)
	(*n)->change (opt);
	gcWalker::collect();
}
};

Phases opt_phase ("opt", new optPhase());

// phase to generate LIR code for a translation unit
class lirGenWalkerPhase: public Phase {
	public: 
	void run() {

		// do we have architecture info?
		if (! CBZ::ArchInfo.is_valid()) {
			printf( "ERROR: Can't generate LIR - missing architecture specification file.  specify with '-arch' option\n" );
			return;
		}

		// walk the tree generating LIR instructions
		lir_gen_walker lgw;
		unit_list_p u;
		for (u=CBZ::Program.begin(); u!=CBZ::Program.end(); u++){
			(*u) -> walk(lgw);
		}

		// if we have no register allocator, go do simple register allocation
		//	so that generated assembler is compilable.
		if (CBZ::NoRegAlloc) {
			dummy_reg_alloc_walker drw;
			unit_list_p u;
			for (u=CBZ::Program.begin(); u!=CBZ::Program.end(); u++){
				(*u) -> walk(drw);
			}
		}
	}
};

Phases lir_gen_walker_phase ("lir_gen_walker", new lirGenWalkerPhase());

// phase to do various flow analysis of LIR code
class lirFlowWalkerPhase: public Phase {
	public: 
	void run() {

		// walk the tree generating LIR instructions
		lir_flow_walker lfw;
		unit_list_p u;
		for (u=CBZ::Program.begin(); u!=CBZ::Program.end(); u++){
			(*u) -> walk(lfw);
		}
	}
};

Phases lir_flow_walker_phase ("lir_flow_walker", new lirFlowWalkerPhase());

// phase to do register allocation on LIR code for a translation unit
class regAllocPhase: public Phase {
	public: 
	void run() {

		// do we have architecture info?
		if (! CBZ::ArchInfo.is_valid()) {
			printf( "ERROR: Can't run register allocator - missing architecture specification file.  specify with '-arch' option\n" );
			return;
		}

		// run register allocator for each translation unit
		RegAllocWalker ra;
		unit_list_p u;
		for (u=CBZ::Program.begin(); u!=CBZ::Program.end(); u++){
			(*u) -> walk(ra);
		}
	}
};

Phases reg_alloc_phase("reg_alloc", new regAllocPhase());

// phase to generate assembler source for a translation unit
class asmGenWalkerPhase: public Phase {
	public: 
	void run() {

		// do we have architecture info?
		if (! CBZ::ArchInfo.is_valid()) {
			printf( "ERROR: Can't run asm generator - missing architecture specification file.  specify with '-arch' option\n" );
			return;
		}

		// run asm generator for each translation unit
		asm_gen_walker asm_gen;
		unit_list_p u;
		for (u=CBZ::Program.begin(); u!=CBZ::Program.end(); u++){
			(*u) -> walk(asm_gen);
		}
	}
};

Phases asm_gen_phase("asm_gen_walker", new asmGenWalkerPhase());

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
    else if (*p == "-ansi") {
      CBZ::ANSIOnly = true;
      ++p;
    }
    else if (*p == "-format") {
      CBZ::FormatReadably = true;
      ++p;
    }
    else if (*p == "-no-pre") {
      CBZ::Preprocess = false;
      ++p;
    }
    else if (*p == "-pre") {
      CBZ::ShowPreprocess = true;
      ++p;
    }
    else if (*p == "-gcc") {
      CBZ::GCCisms = true;
      ++p;
    }
    else if (*p == "-P") {
      ++p;
      CBZ::preproc = *p;
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
	else if (*p == "-arch") {
      ++p;
      string spec_file = *p;
      ++p;

      // try to read this thing
	  if (! CBZ::ArchInfo.load_arch_info(spec_file.c_str())) {
	cerr << "Error: Can't read architecture specification file '" << spec_file << "'." << endl;
	return NULL;
	  }
	}
	else if (*p == "-noreg") {
		CBZ::NoRegAlloc = true;
		++p;
	}
    else {
      // -- Add a source code file to process...
      string output_file;

      if (*p == "(stdin)")
	output_file = string("(stdout)");
      else {
	output_file = (*p);
	int suff_pos = output_file.rfind(CBZ::input_suffix);
	if (suff_pos >= 0 && suff_pos < (int)(*p).length())
	  output_file.replace(suff_pos,
			      CBZ::input_suffix.length(),
			      CBZ::output_suffix);
	else
	  output_file = string("(stdout)");
      }

      CBZ::Program.push_back(new unitNode(*p, output_file));
      ++p;
    }
  } // END while more flags

  // -- Deal with any inspecified flags...

  if (CBZ::preproc.empty()) {
    if (CBZ::ANSIOnly)
      CBZ::preproc = CBZ::ansi_preproc;
    else if (CBZ::GCCisms)
      CBZ::preproc = CBZ::gcc_preproc;
    else
      CBZ::preproc = CBZ::default_preproc;
  }

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

