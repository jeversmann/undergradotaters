// $Id: ref_clone_changer.cc,v 1.9 2003/08/07 23:13:50 pnav Exp $
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
#include "ref_clone_changer.h"

Node * ref_clone_changer::clone(Node * n, bool nil_bad_links)
{
  if (!n)
    return n;

  ref_clone_changer rcc;
  Node * cl = n->change(rcc);

  ref_fix_walker rfw(rcc.map(), nil_bad_links);
  cl->walk(rfw);

  delete rcc.map();

  return cl;
}

Node * ref_fix_walker::lookup(Node * old)
{
  if (! old)
    return 0;

  node_map_p p = _map->find(old);
  if (p == _map->end()) {
    if (_nil_bad_links)
      return 0;
    else
      return old;
  }
  else
    return (*p).second;
}

void ref_fix_walker::at_tdef(tdefNode * the_tdef, Order ord)
{
  typeNode * t = (typeNode *) lookup(the_tdef->def());
  the_tdef->def(t); 
}

void ref_fix_walker::at_id(idNode * the_id, Order ord)
{
  declNode * d = (declNode *) lookup(the_id->decl());
  the_id->decl(d);
}

void ref_fix_walker::at_call(callNode * the_call, Order ord)
{
  procNode * p = (procNode *) lookup(the_call->proc());
  the_call->proc(p);
}

void ref_fix_walker::at_case(caseNode * the_case, Order ord)
{
  switchNode * s = (switchNode *) lookup(the_case->container());
  the_case->container(s);
}

void ref_fix_walker::at_goto(gotoNode * the_goto, Order ord)
{
  labelNode * target = (labelNode *) lookup(the_goto->label());
  the_goto->label(target);
}

void ref_fix_walker::at_continue(continueNode * the_continue, Order ord)
{
  loopNode * l = (loopNode *) lookup(the_continue->container());
  the_continue->container(l);
}

void ref_fix_walker::at_break(breakNode * the_break, Order ord)
{
  stmtNode * s = (stmtNode *) lookup(the_break->container());
  the_break->container(s);
}

void ref_fix_walker::at_return(returnNode * the_return, Order ord)
{
  procNode * p = (procNode *) lookup(the_return->proc());
  the_return->proc(p);
}

void ref_fix_walker::at_proc(procNode * the_proc, Order ord)
{
  labelNode * l = (labelNode *) lookup(the_proc->return_label());
  the_proc->return_label(l);
  declNode * d = (declNode *) lookup(the_proc->return_decl());
  the_proc->return_decl(d);
}
