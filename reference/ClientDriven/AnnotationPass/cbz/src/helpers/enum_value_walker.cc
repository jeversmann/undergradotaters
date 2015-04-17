// $Id: enum_value_walker.cc,v 1.4 2003/08/07 23:13:44 pnav Exp $
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
#include "enum_value_walker.h"

// WARNING: This is only temporary. The evaluation of the initializers
// must be more general and powerful. This functionality should be
// integrated into the semantic checker.

void enum_value_walker::assign(Node * n)
{
  enum_value_walker evw;

  n->walk(evw);
}

void enum_value_walker::at_enum(enumNode * the_enum, Order ord)
{
  if (the_enum->elaborated()) {
    decl_list & decls = the_enum->spec()->fields();
    int cur_val = 0;		// First enum defaults to zero
    char buf[32];

    for (decl_list_p p = decls.begin();
	 p != decls.end();
	 ++p)
      {
	declNode * cur = *p;

	if (cur->init()) {
	  exprNode * e = cur->init();
	  e->eval();
	  if (e->value().basic().is_integer()) {
	    cur_val = e->value().Integer();
	  }
	  else
	    break;
	}
	else {
	  sprintf(buf,"%d",cur_val);
	  constNode * val = new constNode(constant(cur_val),
					  buf, the_enum->coord());
	  cur->init(val);
	}
	cur_val++;
      }
  }
}
