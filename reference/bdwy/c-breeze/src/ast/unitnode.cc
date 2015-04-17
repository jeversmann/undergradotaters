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
#include "goto_label_walker.h"
#include "set_container_walker.h"
#include "scope_walker.h"
#include "id_lookup_walker.h"
#include "enum_value_walker.h"

#include <unistd.h>

extern FILE * cbin;
extern int cbparse();

// --------------------------------------------------------------------
// Constructors
// --------------------------------------------------------------------

unitNode::unitNode(string input_file, string output_file,
		   const Coord coord)
  : Node(Unit, coord),
    _defs(),
    _symbol_level(0),
    _types(new Identifiers_table()),
    _tags(new Tags_table()),
    _input_file(input_file),
    _output_file(output_file),
    _errors(0),
    _warnings(0),
    _undef_funcs(),
    _suespecs() { }

// ------------------------------------------------------------
// Parse
// ------------------------------------------------------------

void unitNode::parse(str_list * cpp_flags, bool abortOnError)
{
  // -- Open the input file

  if (_input_file == "(stdin)")
    cbin = stdin;
  else {
    cbin = _open_input_file(cpp_flags);
  }
    
  // -- Add the filename to the compilation list (init CBZ::Files)
  
  CBZ::set_file(_input_file.c_str(), 0, true);	

  // -- Make this the current unit...

  CBZ::current_unit = this;
  
  // -- Run the parser

  int parse_ok = cbparse();

  if (CBZ::Preprocess && (cbin != stdin))
#if !defined(_WIN32)
    pclose(cbin);
#else
    _pclose(cbin);
#endif
  
  // -- Abort on any errors

  if (CBZ::current_unit->errors() && abortOnError) {
  	abort();
  }

  // -- Perform "fixups"

  fixup();
}

// -- Utility: open the file and run the preprocessor...

FILE * unitNode::_open_input_file(str_list * cpp_flags)
{
  FILE * in_file = 0;
  bool std_in;

  std_in = (_input_file == "(stdin)");
  if (! std_in &&
      (access(_input_file.c_str(), F_OK) != 0) &&
      (access(_input_file.c_str(), R_OK) != 0)) {
    cerr << "Unable to open input file " << _input_file << endl;
    exit(1);
  }

  if (CBZ::Preprocess && ! std_in) {  
    string command = CBZ::preproc;
    for (str_list_p p = cpp_flags->begin();
	 p != cpp_flags->end();
	 ++p)
      command += " " + (*p);
    command += " " + _input_file;

    if (CBZ::ShowPreprocess)
      cout << "Preprocess: " << command << endl;
      
#if !defined(_WIN32)
    in_file = popen(command.c_str(), "r");
#else
    in_file = _popen(command.c_str(), "r");
#endif	/*	!defined(_WIN32)	*/    

    if (! in_file) {
      cerr << "Unable to preprocess input file " << _input_file << endl;
      cerr << "Command: " << command << endl;
      exit(1);
    }
  }
  else {
    if (! std_in) {
      in_file = fopen(_input_file.c_str(), "r");

      if (! in_file) {
	cerr << "Unable to open input file " << _input_file << endl;
	exit(1);
      }
    }
    else
      in_file = stdin;
  }

  return in_file;
}

void unitNode::fixup()
{
  // -- Perform "fixups"

  set_container_walker::fixup(this);
  goto_label_walker::fixup(this);
  id_lookup_walker::fixup(this, true);
  enum_value_walker::assign(this);
}
  
// ------------------------------------------------------------
// Symbol tables
// ------------------------------------------------------------

void unitNode::enter_scope()
{
  types()->enter_scope();
  tags()->enter_scope();

  ++_symbol_level;
}

void unitNode::exit_scope()
{
  if (_symbol_level == 0)
    CBZ::SyntaxError(string("missing '{' detected"));
  else {
    types()->exit_scope();
    tags()->exit_scope();

    --_symbol_level;
  }
}

// ------------------------------------------------------------
//  Walker
// ------------------------------------------------------------

void unitNode::visit(Visitor * the_visitor) 
{
  the_visitor->at_unit(this);
}

void unitNode::walk(Walker & the_walker)
{
  Walker::Order ord = the_walker.order(); 

  if (ord == Walker::Preorder || ord == Walker::Both)
    the_walker.at_unit(this, Walker::Preorder);

  if (the_walker.depth() == Walker::Subtree) {
    // -- Visit the children 

    list_walker(undef_funcs(), the_walker);
    list_walker(suespecs(), the_walker);
    list_walker(defs(), the_walker);
  }

  if (ord == Walker::Postorder || ord == Walker::Both)
    the_walker.at_unit(this, Walker::Postorder);
}

// ------------------------------------------------------------
//  Dataflow
// ------------------------------------------------------------

void unitNode::dataflow(FlowVal * v, FlowProblem & fp)
{
  if (fp.forward()) {
    fp.flow_unit(v, this, FlowProblem::Entry);
    fp.flow_unit(v, this, FlowProblem::Exit);
  }
  else {
    fp.flow_unit(v, this, FlowProblem::Exit);
    fp.flow_unit(v, this, FlowProblem::Entry);
  }
}

// ------------------------------------------------------------
// Ouput
// ------------------------------------------------------------

void unitNode::output(output_context & ct, Node * parent)
{
  def_list & ds = defs();

  _symbol_level = 0;
  for (def_list_p p = ds.begin();
       p != ds.end();
       ++p)
    {
      // -- Don't output code from included files...

      int fnum = (*p)->coord().file();
      bool included = false;

      if (fnum >= 0) {
	string & cur_name = CBZ::Files[fnum];
	included = (cur_name != _input_file);
      }

      if (! included) {
	ct << '\n';
	(*p)->output(ct, parent);
      }
    }
}


// ------------------------------------------------------------
//  Changer
// ------------------------------------------------------------

Node * unitNode::change(Changer & the_changer, bool redispatch)
{
  Changer::Order ord = the_changer.order(); 
  unitNode * the_unit = this;

  if ((ord == Changer::Preorder || ord == Changer::Both) && ! redispatch)
    the_unit = (unitNode *) the_changer.at_unit(the_unit, Changer::Preorder);

  if (the_unit) {

    if (the_unit != this)
      return the_unit->change(the_changer, true);

    change_list(the_unit->suespecs(), the_changer);
    change_list(the_unit->undef_funcs(), the_changer);
    change_list(the_unit->defs(), the_changer);
  }

  if ((ord == Changer::Postorder || ord == Changer::Both) && ! redispatch)
    the_unit = (unitNode *) the_changer.at_unit(the_unit, Changer::Postorder);

  return the_unit;
}


// ------------------------------------------------------------
// Destructor
// ------------------------------------------------------------

unitNode::~unitNode()
{
  //delete_list(_defs);
  //delete_list(_undef_funcs);
  //delete_list(_suespecs);
  // What about symbol tables?
}
