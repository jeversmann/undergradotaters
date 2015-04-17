// $Id: c_breeze.cc,v 1.18 2003/11/11 19:23:14 benh Exp $
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
#include <stdarg.h>

// ------------------------------------------------------------
//  Static configuration
// ------------------------------------------------------------

// -- The parsed program

unit_list CBZ::Program;

// -- Global list of all files

str_vec CBZ::Files;

// -- expected suffixes for input and output files 
string CBZ::input_suffix            = ".c";
string CBZ::output_suffix           = ".p.c";
string CBZ::obj_suffix              = ".o";

// -- preprocessor command lines
string CBZ::cc_cmd = "gcc-2.95";
// override with -cc <cmdline>

string CBZ::preproc_flags
= "-x c -D__C_BREEZE__ -U__GNUC__";
// override with -P <flags>, or -ansi, or -gcc

// djimenez
// this is a hack.  gcc headers recognize these attributes, but
// C-Breeze gets confused and thinks they're undeclared variables.  By
// making them constants, everyone is happy.
string CBZ::gcc_preproc_flags
= 
"-x c -D__C_BREEZE__ -D__QI__=1 -D__HI__=1 -D__SI__=1 "
"-D__DI__=1  -D__word__=1 -D__extension__='' -D__const=const -D__restrict='' "
"-D__printf__='' -D__builtin_va_list='...' -D__attribute__\\(arg\\)='' "
"-D__complex__=''";

string CBZ::ansi_preproc_flags
= "-ansi -x c";

// -- Basic Global Variables 

float CBZ::VersionNumber ;
string CBZ::VersionDate ;
string CBZ::Executable;

// -- default warning level 
int CBZ::WarningLevel               = 4;

// -- CPP flags
str_list * CBZ::cpp_flags;

// -- Current status in parsing

unitNode * CBZ::current_unit;
int CBZ::current_file;
int CBZ::current_line;
int CBZ::current_offset;
text_list CBZ::pragmas;

// -- Current phase in processing

string CBZ::PhaseName;

bool CBZ::OldStyleFunctionDefinition;

// -- Command Line Flags

bool CBZ::QuietlyIgnore     = false;
bool CBZ::DebugLex          = false;
bool CBZ::PrintSymTables    = false;
bool CBZ::TrackInsertSymbol = false;
bool CBZ::PrintLineOffset   = false;
bool CBZ::IgnoreLineDirectives = false;
bool CBZ::ANSIOnly          = false;
bool CBZ::GCCisms          = false;
bool CBZ::NoRegAlloc		= false;
bool CBZ::LeaveIncluded     = false;
bool CBZ::FormatReadably    = false;
#if (defined _WIN32) && ! (defined _MSC_VER)
// no popen support on Windows
bool CBZ::Preprocess        = false;
#else
bool CBZ::Preprocess        = true;
#endif
bool CBZ::ShowPreprocess    = false;

// ------------------------------------------------------------
// -- Was SetFile in c4.l
// ------------------------------------------------------------

void CBZ::set_file(const char *name, int line, bool is_new_file)
{
  int i;
  string nm(name);

  // -- Look for this file in the list..

  for (i=0; i < (int)Files.size(); ++i)
    if (Files[i] == nm)
      break;

  // -- If not there, add it...

  if (i == Files.size())
    Files.push_back(nm);
  else
    if (is_new_file)
      Warning(5, string("Header file `") + nm + 
	      string("' included more than once."));

  current_file = i;
}

// -- Returns true is the current file was included...

bool CBZ::is_file_included()
{
  if (Files.size() == 0)
    return false;
  else
    return (Files[current_file] != current_unit->input_file());
}

// -- Was CharToText in print-ast.c

void CBZ::char_to_text(char *array, unsigned char value)
{
  if (value < ' ') {
    static const char *names[32] = {
      "nul","soh","stx","etx","eot","enq","ack","bel",
      "\\b", "\\t", "\\n", "\\v", "ff", "cr", "so", "si",
      "dle","dc1","dc2","dc3","dc4","nak","syn","etb",
      "can","em", "sub","esc","fs", "gs", "rs", "us" };
    sprintf(array, "0x%02x (%s)", value, names[value]);
  } else if (value < 0x7f) {
    sprintf(array, "'%c'", value);
  } else if (value == 0x7f) {
    strcpy(array, "0x7f (del)");
  } else { /* value >= 0x80 */
    sprintf(array, "0x%x", value);
  }
}

// ------------------------------------------------------------
//  Errors and warnings
// ------------------------------------------------------------

void CBZ::Fail(const string file, int line, const string msg)
{
  cerr << "Assertion failed in " << file << ", line " << line << endl;
  cerr << "    " << msg << endl;

  if (PhaseName == "Parsing")
    cerr << "Input: " << CBZ::Files[CBZ::current_file] << ", line " << current_line << endl;

  abort();
}

void CBZ::SyntaxError(string s)
{
  CBZ::current_unit->inc_errors();
  cerr << CBZ::Files[CBZ::current_file] << ":" << current_line << ": Error: " << s << endl;
}

void CBZ::Warning(int level, string s)
{
  if (level <= WarningLevel) {
    CBZ::current_unit->inc_warnings();
    cerr << CBZ::Files[CBZ::current_file] << ":" << current_line << ": Warning: " << s << endl;
  }
}

void CBZ::SyntaxError(Coord c, string s)
{
  CBZ::current_unit->inc_errors();
  cerr << c << ": Error: " << s << endl;
}

void CBZ::Warning(int level, Coord c, string s)
{
  if (level <= WarningLevel) {
    CBZ::current_unit->inc_warnings();
    cerr << c << ": Warning: " << s << endl;
  }
}

void CBZ::WarnAboutPrecedence(unsigned int op_id, exprNode * the_expr)
{
  if (the_expr->typ() == Binary && ! the_expr->parenthesized()) {
    unsigned int subop_id = ((binaryNode *)the_expr)->op()->id();

    if (op_id == Operator::OROR && subop_id == Operator::ANDAND)
      CBZ::Warning(4, the_expr->coord(),
		   string("suggest parentheses around && in operand of ||"));
    else if ((op_id == '|' || op_id == '^') && 
	     (subop_id == '+' || subop_id == '-' || subop_id == '&' || subop_id == '^') &&
	     op_id != subop_id)
      CBZ::Warning(4, the_expr->coord(),
		   string("suggest parentheses around arithmetic in operand of ") + 
		   string(1,(char)op_id));
  }
}

void CBZ::Error(Coord c, string s)
{
  CBZ::current_unit->inc_errors();
  cerr << c << ": Error: " << s << endl;
}

// ---------------------------------------------------------
// temporary name stuff
// ---------------------------------------------------------

unsigned int CBZ::next_temp_id_seed = 0;

string CBZ::get_temp_id_str()
{
	// make a string with the next temp id
	const int bufsize = 20;
	char buf[ bufsize ];
#ifdef _WIN32
	_snprintf( buf, bufsize, "T%d", next_temp_id_seed++ );
#else
	snprintf( buf, bufsize, "T%d", next_temp_id_seed++ );
#endif
	buf[bufsize-1] = 0;
	return string( buf );
}

idNode * CBZ::get_temp_id()
{
	// make a new id node
	return new idNode( get_temp_id_str().c_str() );
}

idNode * CBZ::get_temp_var( blockNode * containing_block, typeNode * type )
{
	// make a temp id
	idNode * pId = get_temp_id();
	pId->type( type );

	// a declaration for it
	declNode * pDecl = new declNode( pId, declNode::AUTO, type, NULL, NULL );
	pDecl->decl_location( declNode::BLOCK );
	pId->decl( pDecl );

	// add this to the block
	containing_block->decls().push_back( pDecl );

	return pId;
}

