// $Id: c_breeze.h,v 1.25 2006/08/10 15:40:23 abrown Exp $
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

#ifndef CBZ_C_BREEZE_H
#define CBZ_C_BREEZE_H


// EDB
#ifdef _WIN32
// Disable warnings about long (> 255 chars) identifiers.
#pragma warning(disable:4786)
#pragma warning(disable:4503)
// Disable warnings about int being forced to bool.
#pragma warning(disable:4800)
// Disable warnings about size conversion 
#pragma warning(disable:4267)
#endif // 

// -- Include the necessary standard library stuff

// #include <ios>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <list>
#include <vector>
#include <map>
#include <algorithm>
#include <functional>

#include <assert.h>
#include <stdio.h>
#include "cbz_util.h"

// -- Declare all the classes ahead of time...

class Node;
class unitNode;
class defNode;
class declNode;
class typeNode;
class primNode;
class tdefNode;
class ptrNode;
class arrayNode;
class funcNode;
class sueNode;
class structNode;
class unionNode;
class enumNode;
class suespecNode;
class exprNode;
class indexNode;
class constNode;
class idNode;
class binaryNode;
class unaryNode;
class castNode;
class commaNode;
class ternaryNode;
class callNode;
class initializerNode;
class stmtNode;
class blockNode;
class basicblockNode;
class exprstmtNode;
class targetNode;
class labelNode;
class caseNode;
class selectionNode;
class ifNode;
class switchNode;
class loopNode;
class whileNode;
class doNode;
class forNode;
class jumpNode;
class gotoNode;
class continueNode;
class breakNode;
class returnNode;
class attribNode;
class procNode;
class operandNode;
class conditiongotoNode;
class threeAddrNode;
class textNode;
class CBZ;
class Containers;
class Coord;
class Datatype;

class Symbols;
class Identifiers_table;
class Tags_table;
class Externals_table;

class FlowVal;
class FlowProblem;
class Annote;

// EDB
using namespace std;

typedef list< Node * > node_list;
typedef node_list::iterator node_list_p;

typedef list< exprNode * > expr_list;
typedef expr_list::iterator expr_list_p;

typedef list< declNode * > decl_list;
typedef decl_list::iterator decl_list_p;

typedef list< targetNode * > target_list;
typedef target_list::iterator target_list_p;

typedef list< attribNode * > attrib_list;
typedef attrib_list::iterator attrib_list_p;

typedef list< indexNode *> index_list;
typedef index_list::iterator index_list_p;

typedef list< idNode * > id_list;
typedef id_list::iterator id_list_p;

typedef list< stmtNode * > stmt_list;
typedef stmt_list::iterator stmt_list_p;

typedef list< gotoNode * > goto_list;
typedef goto_list::iterator goto_list_p;

typedef list< defNode * > def_list;
typedef def_list::iterator def_list_p;

typedef list< Annote * > annote_list;
typedef annote_list::iterator annote_list_p;

typedef list< unitNode * > unit_list;
typedef unit_list::iterator unit_list_p;

typedef vector< string > str_vec;
typedef str_vec::iterator str_vec_p;

typedef list< string > str_list;
typedef str_list::iterator str_list_p;

typedef list< textNode * > text_list;
typedef text_list::iterator text_list_p;

typedef list< suespecNode * > suespec_list;
typedef suespec_list::iterator suespec_list_p;

typedef list< blockNode * > block_list;
typedef block_list::iterator block_list_p;

typedef list< basicblockNode * > basicblock_list;
typedef basicblock_list::iterator basicblock_list_p;

typedef list< operandNode * > operand_list;
typedef operand_list::iterator operand_list_p;

class Visitor;
class Walker;
class Changer;

class output_context;

// --------------------------------------------------------------------
//   Node types
// --------------------------------------------------------------------

typedef enum {
  // expression nodes
  Const, Id, Binary, Unary, Cast, Comma, Ternary, Call, Initializer,

  // statement nodes 
  Label, Switch, Case, If, While, Do, For, Goto, Continue, 
  Break, Return, Block, 

  // type nodes 
  Prim, Tdef, Ptr, Array, Func, Struct, Union, Enum, sueSpec,


  // declaration node 
  Decl, 

  // GCC __attribute__ extension 
  Attrib, 

  // procedure def node
  Proc,

  // random text and preprocessor command node
  Text,

  // A statement that just consists of an expression
  Expr,

  // Special undeclared node
  Undeclared,

  // Translation unit
  Unit,

  // Indicates that the node is a meta-variable
  Meta,

  // An addressable memory location, assuming base is loaded
  Operand,

  // Condition for a dismantled ifNode
  Condition,

  // A three address node
  ThreeAddr

} NodeType;

// A declarator has several context-dependent scoping options:

typedef enum {
  Redecl,    // may redeclare a typdef name
  NoRedecl,  // may not redeclare a typedef name
  SU,        // is a structure or union field
  Formal     // is a formal parameter, goes into scope of the following body
             //  (storage class NOT extern, static, typedef, auto)
} ScopeState;

// -- Assoc is used in the output phase to control associativity
// for operators and context for types.

typedef enum { Right, Left } Assoc;

// -- Inlcude other headers, including the AST

#include "coord.h"
#include "operators.h"
#include "basic_type.h"
#include "constant.h"
#include "annote.h"
#include "output_context.h"
#include "ast.h"
#include "visitor.h"
#include "walker.h"
#include "changer.h"
#include "dataflow.h"
#include "symbol.h"
#include "phase.h"

// --- Map for node numberings

  typedef map<Node *,
	  unsigned int,
	  less< Node * > > numbering_map;

// --- The CBZ class holds many of the global configuration values as
// static data members.

class CBZ {

public:

  // -- The parsed program
  static unit_list Program;

  // -- Global list of all files
  static str_vec Files;
	  
  // -- expected suffixes for input and output files 
  static string input_suffix ;
  static string output_suffix ;
  static string obj_suffix ;

  // -- preprocessor command lines 
  static string cc_cmd;
  static string preproc_flags;
  static string gcc_preproc_flags;
  static string ansi_preproc_flags;

  // -- Basic Global Variables 

  static float VersionNumber ;
  static string VersionDate ;
  static string Executable;

  // -- default warning level 
  static int WarningLevel ;

  // -- CPP flags
  static str_list * cpp_flags;

  // -- Parsing state

  static unitNode * current_unit;
  static int current_file;
  static int current_line;
  static int current_offset;

  static text_list pragmas;

  static void set_file(const char *name, int line, bool is_new_file);

  static bool is_file_included();

  static void char_to_text(char * array, unsigned char value);

  // -- Current phase in processing

  static string PhaseName;

  // -- Command Line Flags

  static bool QuietlyIgnore;
  static bool DebugLex;
  static bool PrintSymTables;
  static bool TrackInsertSymbol;
  static bool PrintLineOffset;
  static bool IgnoreLineDirectives;
  static bool ANSIOnly;
  static bool FormatReadably;
  static bool Preprocess;
  static bool ShowPreprocess;
  static bool GCCisms;
  static bool NoRegAlloc;
  static bool LeaveIncluded;

  // --- Flag old-style fdcls

  static bool OldStyleFunctionDefinition;

  // --- Errors, warnings, failures...

  static void Fail(const string file, int line, const string msg);
  static void SyntaxError(string s);
  static void Warning(int level, string s);
  static void SyntaxError(Coord c, string s);
  static void Warning(int level, Coord c, string s);
  static void WarnAboutPrecedence(unsigned int op_id, exprNode * the_expr);
  static void Error(Coord c, string s);

  // handy error reporting macro that lets you fail with a formatted message
  //
  // example usage:
  //
  //	CBZFAIL(("Don't know how to handle type %s in construct %s", type.c_str(), construct.c_str()));
  //
#define CBZFAIL( errfmt ) \
  CBZ::Fail( __FILE__, __LINE__, cbz_util::string_format errfmt );

  // -- helper functions for creating temporary variables for various uses

  // just get a new identifier with a unique name
  static string get_temp_id_str();

  // get a new identifier node with a unique name
  static idNode * get_temp_id();

  // get a new variable with a unique name - its decl will be added to the given block
  static idNode * get_temp_var( blockNode * containing_block, typeNode * type );

private:

  // integer used to generate temporary variable names
  static unsigned int next_temp_id_seed;

};

// -- Struct to hold parsing information about type qualifiers...

struct _TQ {
  typeNode::Type_qualifiers tq;
  declNode::Storage_class sc;
  simpleCoord coord;
};

struct _TYPE {
  typeNode * ty;
  declNode::Storage_class sc;
};


#endif // CBZ_C_BREEZE_H
