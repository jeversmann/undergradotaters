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

int ncount = 0;
int freqs[100];
static void print_all (void);

#include "c_breeze.h"

// -- These are for tracking memory leaks...

int Node::_count = 0;
int Node::_uid_count = 0;
int Node::_t_count[50];

char * TypNames[] = {
  // expression nodes
  "Const", "Id", "Binary", "Unary", "Cast", "Comma", "Ternary",
  "Call", "Initializer",

  // statement nodes 
  "Label", "Switch", "Case", "If", "While", "Do",
  "For", "Goto", "Continue", "Break", "Return", "Block", 

  // type nodes 
  "Prim", "Tdef", "Ptr", "Array", "Func", "Struct", "Union", "Enum", "sueSpec",


  // declaration node 
  "Decl", 

  // GCC __attribute__ extension 
  "Attrib", 

  // procedure def node
  "Proc",

  // random text and preprocessor command node
  "Text",

  // A statement that just consists of an expression
  "Expr",

  // Special undeclared node
  "Undeclared",

  // Translation unit

  "Unit"
};

// --------------------------------------------------------------------
// Constructors
// --------------------------------------------------------------------

Node::Node(NodeType the_typ, const Coord coord, bool parenthesized)
  : _typ(the_typ), 
    _coord(coord), 
    _parenthesized(parenthesized),
    _annotations(),
    _gen(0),
    _kill(0)
{
/*
if (ncount == 0) {
for (int i=0; i<100; i++) freqs[i] = 0;
}
ncount++;
if (ncount % 1000 == 0) print_all ();
freqs[_typ]++;
*/
  Node::nodes.push_back (this);
  ++_count;
  ++_t_count[the_typ];
  //  ++_uid_count;
  // _uid = _uid_count;
}

// static map of all nodes, used for garbage collection

node_list Node::nodes;
map <Node *, bool> Node::deleted_nodes;

static void print_all (void) {
int	i;

printf ("node freqs:\n");
for (i=0; i<100; i++) if (freqs[i]) printf ("%d: %d\n", i, freqs[i]);
}
Node::Node(const Node & other)
  : _typ(other._typ), 
    _coord(other._coord), 
    _parenthesized(other._parenthesized),
    _annotations(),
    _gen(0),
    _kill(0)
{
/*  if (ncount == 0) {
for (int i=0; i<100; i++) freqs[i] = 0;
}
ncount++;
if (ncount % 1000 == 0) print_all ();
freqs[_typ]++;
*/
  Node::nodes.push_back (this);
  // ++_count;
  // ++_t_count[other._typ];
  // ++_uid_count;
  // _uid = _uid_count;
}

// ------------------------------------------------------------
//  Data type base
// ------------------------------------------------------------

// -- Each node implements it's own version of base_type()

typeNode * Node::base_type(bool TdefIndir) const
{
  return (typeNode *) 0;
}

typeNode * Node::datatype() const
{ 
  return base_type(true);
}

typeNode * Node::datatype_superior() const
{
  return base_type(false);
}


// ------------------------------------------------------------
// Destructor
// ------------------------------------------------------------

Node::~Node()
{
  deleted_nodes[this] = true;
  //delete_list(_annotations);
  // --_t_count[_typ];
  // --_count;
}

// ------------------------------------------------------------
// Report memory usage
// ------------------------------------------------------------

void Node::report()
{
  cout << "Nodes: " << _count << endl;
  for (int i = 0; i <= Undeclared; ++i) {
    if (_t_count[i] != 0) {

      cout << "  ";
      switch (i) {
      case Const:
	cout << "Const";
	break;
      case Id:
	cout << "Id";
	break;
      case Binary:
	cout << "Binary";
	break;
      case Unary:
	cout << "Unary";
	break;
      case Cast:
	cout << "Cast";
	break;
      case Comma:
	cout << "Comma";
	break;
      case Ternary:
	cout << "Ternary";
	break;
      case Call:
	cout << "Call";
	break;
      case Initializer:
	cout << "Initializer";
	break;
      case Label:
	cout << "Label";
	break;
      case Switch:
	cout << "Switch";
	break;
      case Case:
	cout << "Case";
	break;
      case If:
	cout << "If";
	break;
      case While:
	cout << "While";
	break;
      case Do:
	cout << "Do";
	break;
      case For:
	cout << "For";
	break;
      case Goto:
	cout << "Goto";
	break;
      case Continue:
	cout << "Continue";
	break;
      case Break:
	cout << "Break";
	break;
      case Return:
	cout << "Return";
	break;
      case Block:
	cout << "Block";
	break;
      case Prim:
	cout << "Prim";
	break;
      case Tdef:
	cout << "Tdef";
	break;
      case Ptr:
	cout << "Ptr";
	break;
      case Array:
	cout << "Array";
	break;
      case Func:
	cout << "Func";
	break;
      case Struct:
	cout << "Struct";
	break;
      case Union:
	cout << "Union";
	break;
      case Enum:
	cout << "Enum";
	break;
      case sueSpec:
	cout << "sueSpec";
	break;
      case Decl:
	cout << "Decl";
	break;
      case Attrib:
	cout << "Attrib";
	break;
      case Proc:
	cout << "Proc";
	break;
      case Text:
	cout << "Text";
	break;
      case Expr:
	cout << "Expr";
	break;
      case Undeclared:
	cout << "Undeclared";
	break;
      default:
	cout << "ERROR";
	break;
      }

      cout << " : " << _t_count[i] << endl;
    }
  }
}
