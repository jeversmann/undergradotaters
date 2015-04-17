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
#include "scope_walker.h"
#include "id_lookup_walker.h"

// ------------------------------------------------------------
// Constructor
// ------------------------------------------------------------

id_lookup_walker::id_lookup_walker(unitNode * u, bool verb)
  : scope_walker(u),
    _verbose(verb)
{}

// ------------------------------------------------------------
// Utility class: clear all the pointers first
// ------------------------------------------------------------

class clear_ids_walker : public Walker
{
public:

  static void clear(unitNode * u)
  {
    clear_ids_walker ciw;

    u->walk(ciw);
  }

private:

  clear_ids_walker()
    : Walker(Preorder, Subtree)
  {}

public:

  virtual void at_tdef(tdefNode * the_tdef, Order ord)
  {
// djimenez
// what is this for?  I don't know, but it's screwing up things like:
// typedef int Int;
// struct foo {
// 	Int a;
// } foo;
// foo b;
// void bar (void) { b.a; }

//    the_tdef->def(0);
  }

  virtual void at_id(idNode * the_id, Order ord)
  {
    the_id->decl(0);
  }
};

// ------------------------------------------------------------
// Entry points
// ------------------------------------------------------------

void id_lookup_walker::fixup(unitNode * u, bool verb)
{
  id_lookup_walker idw(u,verb);
  clear_ids_walker::clear(u);

  u->walk(idw);
}

id_lookup_walker * id_lookup_walker::fixup_and_return(unitNode * u, bool verb)
{
  id_lookup_walker * idw = new id_lookup_walker(u,verb);
  clear_ids_walker::clear(u);

  u->walk(*idw);

  return idw;
}

// ------------------------------------------------------------
// Handle errors and conflicts
// ------------------------------------------------------------

void id_lookup_walker::conflict(const Coord & c, const string & msg)
{
  if (_verbose)
     CBZ::SyntaxError(c, msg);
}

// ------------------------------------------------------------
// Handle entries into the symbol table
// ------------------------------------------------------------

void id_lookup_walker::at_proc(procNode * the_proc, Order ord)
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
	  declNode * o_decl = ids()->insert(a_decl->name(), a_decl);

	  // -- Check for conflict...
	  if (o_decl)
	    conflict(a_decl->coord(),
		     string("formal `") + 
		     o_decl->name() + string("' used multiple times"));

	} else
	  conflict((*p)->coord(),
		   string("argument without a name"));
    }

    // -- Local variables get their own scope (SZG)

    enter_scope();
  }

  if (ord == Postorder) {

    // -- Set decl_location on the func decl

    the_proc->decl()->decl_location(declNode::PROC);

    // -- Exit the local variable scope (SZG)

    exit_scope();

    // -- Exit the formal parameter scope

    exit_scope();
  }

}

void id_lookup_walker::at_decl(declNode * the_decl, Order ord)
{
  // SZG Let's try preorder to fix things like this:
  //    struct Foo * f = malloc( sizeof *f );
  // if (ord == Postorder) {

  if (ord == Preorder) {

    declNode * orig;

    // -- Skip abstract declarators...

    if (the_decl->name().empty()) {
      // djimenez - added this check
	if (in_formals()) {
		the_decl->decl_location(declNode::FORMAL);
	} else if (in_su()) {
		the_decl->decl_location(declNode::SU);
	}
      return;
    }

    // -- Reset the reference count

    the_decl->references(0);

    // -- Handle formal parameters...

    if (in_formals()) {
      the_decl->decl_location(declNode::FORMAL);

      if ((the_decl->storage_class() != declNode::NONE) &&
	  (the_decl->storage_class() != declNode::REGISTER)) {
	conflict(the_decl->coord(),
		 string("illegal storage class for parameter `") + 
		 the_decl->name() + string("'"));

	//if (the_decl->storage_class() == declNode::TYPEDEF)
	//  break;

	// reset storage class for body
	//the_decl->storage_class(declNode::NONE);
	// why? djimenez
      }

      // convert Array to pointer
      // djimenez
      // why?  it gains us nothing, and if we do this here but not 
      // for abstract declarators, we'll get incompatible types
      // when they are compared.
      
      //if (the_decl->type()->typ() == Array) {
//	arrayNode * atype = (arrayNode *) the_decl->get_type();
//	the_decl->type(new ptrNode(atype->type_qualifiers(),
//				   atype->type(),
//				   atype->coord()));

	// -- Delete the old type...
//	delete atype;
//     }

      return;
    }

    // -- Insert declarations into the symbol tables...

    if ((the_decl->type()->typ() == Func) ||
	(the_decl->storage_class() == declNode::EXTERN)) {

    // -- Put externs in the externs symbol table. It allows
    // duplicates, as long as they are compatible.

      orig = externs()->insert(the_decl->name(), the_decl);
      if (orig) {

	if (the_decl->type()->typ() == Func) {
	  funcNode * the_func = (funcNode *) the_decl->type();
	  funcNode * ofunc = (funcNode *) orig->type();

	  // -- Duplicate function declarations must be compatible...

	  if (! ofunc->is_compatible_with(the_func)) {
	    conflict(the_func->coord(),
		     string("function identifier `") + the_decl->name() +
		     "' redeclared\n        Previous declaration: " +
		     orig->coord().to_string());
	  }
	}
	else {
	  // "external declarations for the same identifier must agree in
	  // type and linkage" --  K&R2, A10.2

	  if (! (* orig->type() == * the_decl->type()) ) {
	    conflict(the_decl->coord(),
		     string("extern `") + orig->name() + 
		     "' redeclared\n        Previous declaration: " +
		     orig->coord().to_string());
	  }
	}
      }
    }
    else
      if (! in_su() /* && ! in_enum() */) {

	// -- Regular (non-extern) declarations...

	orig = ids()->insert(the_decl->name(), the_decl);
	if ((orig) && (orig != the_decl)) {
	  conflict(the_decl->coord(),
		   string("variable `") + orig->name() +
		   "' redeclared\n        Previous declaration: " +
		   orig->coord().to_string());
	}
      }

    // -- Set decl_location

    if (in_su())
      the_decl->decl_location(declNode::SU);
    else
      if (in_enum())
	the_decl->decl_location(declNode::ENUM);
      else
	if (symbol_level() == 0)
	  the_decl->decl_location(declNode::TOP);
	else
	  the_decl->decl_location(declNode::BLOCK);
  }
}

// ------------------------------------------------------------
// Lookup identifiers in table
// ------------------------------------------------------------

void id_lookup_walker::at_call(callNode * the_call, Order ord)
{
  if ((ord == Preorder) &&
      (the_call->name()->typ() == Id)) {
    idNode * id = (idNode *) the_call->name();
    declNode * var;

    var = ids()->lookup(id->name());
    if (! var) {
      var = externs()->lookup(id->name());
      if (var) {
	id->decl(var);
	var->inc_references();
      }
      else {
	// -- Not found, declare returning int...

	var = new declNode(id->name().c_str(), declNode::EXTERN,
			   new funcNode(typeNode::NONE, (decl_list *)0,
					new primNode(id->coord()),
					id->coord()),
			   (exprNode *)0,
			   (exprNode *)0,
			   id->coord());

	var->decl_location(declNode::TOP);
	var->inc_references();
	id->decl(var);

	unit()->undef_funcs().push_back(var);

	declNode * tmp;
	tmp = ids()->insert(id->name(), var);
	tmp = externs()->insert(id->name(), var);
      }
    }
  }
}

void id_lookup_walker::at_tdef(tdefNode * the_tdef, Order ord)
{
  if (ord == Postorder) {
    declNode * d1 = ids()->lookup(the_tdef->name());
    declNode * d2 = externs()->lookup(the_tdef->name());
    declNode * d;
    if (d1 || d2) {
      if (d1) d = d1;
      if (d2) d = d2;

      the_tdef->def(d->type());
      d->inc_references();
    }
  }
}

void id_lookup_walker::at_id(idNode * the_id, Order ord)
{
  if (ord == Postorder) {

    // -- Skip it if its already been set (in at_call).

    if (the_id->decl())
      return;

    declNode * d1 = ids()->lookup(the_id->name());
    declNode * d2 = externs()->lookup(the_id->name());
    declNode * d;
    if (d1 || d2) {
      if (d1) 
 	d = d1;
      else if (d2) 
	d = d2;

      the_id->decl(d);
      d->inc_references();
    }
    else
      conflict(the_id->coord(), string("undeclared variable `") +
	       the_id->name() + string("'"));
  }
}

void id_lookup_walker::at_binary(binaryNode * the_binary, Order ord)
{
  // -- This code is to handle the identifiers that represent struct/union
  // fields (only works if the semantic checker has been run first).

  unsigned int opid = the_binary->op()->id();

  if ((ord == Preorder) &&
      ((opid == Operator::ARROW) ||
       (opid == '.')) &&
      the_binary->left()->type())
    {
      idNode * field = (idNode *) the_binary->right();
      exprNode * left = the_binary->left();
      typeNode * left_type = left->no_tdef_type();

      if (opid == Operator::ARROW) {
	if (left_type->typ() != Ptr)
	  CBZ::SyntaxError(left->coord(),
			   "Left operand of \"->\" must be a pointer");

	left_type = left_type->no_tdef_type();
      }

      // -- Make sure the LHS is a struct or union

      if ((left_type->typ() != Struct) &&
	  (left_type->typ() != Union)) {
	CBZ::SyntaxError(left->coord(),
			 "Left operand of \"->\"/\".\" must be a struct or union");
      }
      else {
	sueNode * sue = (sueNode *) left_type;
	suespecNode * sp = sue->spec();

	declNode * field_decl = sp->find_field(field->name());

	field->decl(field_decl);
	if (field_decl)
	  field_decl->inc_references();
      }
    }
}
