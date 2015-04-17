// $Id: stmtnode.cc,v 1.5 2003/09/07 19:48:37 abrown Exp $
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

// --------------------------------------------------------------------
// Constructors
// --------------------------------------------------------------------

stmtNode::stmtNode(NodeType typ, const Coord coord)
  : Node(typ, coord)
{
}

// ------------------------------------------------------------
// Output
// ------------------------------------------------------------

void stmtNode::output(output_context & ct, Node * parent)
{
  if (typ() != Block) {
    output_comment(ct);
    ct.new_line();
  }

  output_stmt(ct, parent);
}

void stmtNode::output_comment(output_context & ct)
{
  if (! comment().empty()) {

    ct.new_line();

    ct << "/* ";

    ct.indent_in();

    string front;
    string back = comment();
    bool first = false;
    int nl;

    do {
      nl = back.find('\n');
      if (nl != string::npos) {
	front = back.substr(0, nl);
	back = back.substr(nl+1, string::npos);
	ct << front;
	if (first) {
	  ct.indent_in();
	  first = false;
	}
	ct.new_line();
      }
      else
	ct << back;
    } while (nl != string::npos);

    ct << " */";

    ct.indent_out();
  }
}  

// ------------------------------------------------------------
// Destructor
// ------------------------------------------------------------

stmtNode::~stmtNode()
{
}
