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

/*Adapted from C-breeze version*/

#include "name_mangle_walker.h"


// ------------------------------------------------------------
// Entry point
// ------------------------------------------------------------

void name_mangle_walker::mangle()
{

  for(unit_list_p n= CBZ::Program.begin(); n!= CBZ::Program.end(); n++)
    name_mangle_walker::mangle(*n); 
  
}

// ------------------------------------------------------------
// Entry point
// ------------------------------------------------------------

void name_mangle_walker::mangle(unitNode * u)
{

  name_mangle_walker nmw(u);
  fix_goto_walker fgw;

  u->walk(nmw);

  u->walk(fgw);

}



// ------------------------------------------------------------
// Handle entries into the symbol table
// ------------------------------------------------------------

void name_mangle_walker::at_proc(procNode * the_proc, Order ord)
{
  if (ord == Preorder) {

    // -- Put the formal parameters into the symbol table

    enter_scope();

    funcNode * f = (funcNode *) the_proc->decl()->type();
    decl_list & as = f->args();

    for (decl_list_p p = as.begin(); p != as.end(); ++p) {

      if ((*p)->datatype()->is_ellipsis() ||
	  (*p)->datatype()->is_void())
	;
      else
	if (! (*p)->name().empty()) {

	  // -- Normal declaration...

	  declNode * a_decl = (declNode *) (*p);
	  string newname = ids()->insert_unique(a_decl->name(), a_decl);
	  if (newname != a_decl->name())
	    a_decl->name(newname);
	}
    }
  }

  if (ord == Postorder) {

    // -- Exit the scope

    exit_scope();

  }

}

void name_mangle_walker::at_decl(declNode * the_decl, Order ord)
{
  if (ord == Preorder) {

    // -- Skip abstract declarators...

    if (the_decl->name().empty())
      return;

    // -- Skip function declarations...

    if ((the_decl->type()->typ() == Func) &&
	! in_formals())
      return;

    //  -- Skip external declarations...

    if ((the_decl->storage_class() == declNode::EXTERN) ||
	(symbol_level() == 0))
      return;

    // -- Set decl_location and insert into the symbol table...

    if (in_su())
      return;
    else
      if (in_formals())
	return;
      else {

	// -- add to current scope
	string newname = ids()->insert_unique(the_decl->name(), the_decl);
	if (newname != the_decl->name())
	    the_decl->name(newname);
      }
  }
}

void name_mangle_walker::at_label(labelNode * the_label, Order ord)
{
  if (ord == Preorder)
  {
    // -- add to current scope
    string newname = labels()->insert_unique(the_label->name(), the_label);
    if (newname != the_label->name())
        the_label->name(newname);
  }

}


// ------------------------------------------------------------
// Fix identifier names
// ------------------------------------------------------------

void name_mangle_walker::at_id(idNode * the_id, Order ord)
{
  declNode * d = the_id->decl();
  if ((ord == Preorder) &&
      d && 
      (d->name() != the_id->name()))
    the_id->name(d->name());
}

// ------------------------------------------------------------
// Fix goto names
// ------------------------------------------------------------

void fix_goto_walker::at_goto(gotoNode * g, Order ord)
{
  labelNode * lbl = g->label();
  if ((ord == Preorder) && lbl && (lbl->name() != g->name()))
    g->name(lbl->name());
}


