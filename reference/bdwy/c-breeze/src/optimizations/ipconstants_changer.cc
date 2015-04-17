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
#include "ipconstants_changer.h"

ipConstantsChanger::ipConstantsChanger(ipConstantPropagation * constants,
				       Pointers * ptrs)
  : Changer( Postorder, Subtree, true ),
    ipconstants(constants),
    pointers(ptrs)
{}

Node * ipConstantsChanger::at_expr(exprNode * the_expr, Order ord)
{
  memoryuse_set uses;
  pointers->indirect_uses(the_expr, uses);
  if ( ! uses.empty()) {

    analysisVal * val = ipconstants->top();
    for (memoryuse_set_p use_p = uses.begin();
	 use_p != uses.end();
	 ++use_p)
      {
	memoryUse * use = *use_p;
	analysisVal * cur_val = 0;
	if (use->reaching_def())
	  cur_val = ipconstants->lookup(use->reaching_def()->owner(), use);
	if (! cur_val)
	  cur_val = ipconstants->bottom();
	val->meet_with(cur_val);
	ipconstants->free(cur_val);
      }

    ipConstant * const_val = (ipConstant *) val;

    if ( ! const_val->is_bottom() &&
	 ! const_val->is_top()) {
      constNode * out = new constNode(const_val->value());
      ipconstants->free(val);
      return out;
    }
  }
  else {
    the_expr->eval();
    if ( ! the_expr->value().no_val()) {
      constNode * out = new constNode(the_expr->value());
      return out;
    }
  }

  return the_expr;
}
