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
#include "semcheck.h"
#include "print_walker.h"
#include "ref_clone_changer.h"

// ------------------------------------------------------------
// Main semantic checker
// ------------------------------------------------------------

bool semcheck_walker::showErrors = true;

void semcheck_walker::check(unitNode * u, bool verbose)
{
  semcheck_walker sw;
  bool old_val = showErrors;
  showErrors = verbose;

  u->walk(sw);

  showErrors = old_val;
}

// ------------------------------------------------------------
// Check different node types
// ------------------------------------------------------------

void semcheck_walker::at_exprstmt(exprstmtNode * the_exprstmt, Order ord)
{
  if (the_exprstmt->expr())
    semcheck_expr_visitor::check(the_exprstmt->expr());
}

void semcheck_walker::at_suespec(suespecNode * the_suespec, Order ord)
{
  // -- Compute the actual values for each enum

  if (the_suespec->owner() == Enum) {
    decl_list & decls = the_suespec->fields();
    int cur_val = 0;            // First enum defaults to zero
    char buf[32];

    for (decl_list_p p = decls.begin();
         p != decls.end();
         ++p)
      {
        declNode * cur = *p;

        if (cur->init()) {
          exprNode * e = cur->init();

	  // -- Try to compute a constant value

	  semcheck_expr_visitor::check(e);
	  e->eval();
	  if (e->value().basic().is_integer())
	    cur_val = e->value().Integer();
	  else
	    break;
        }
        else {
          sprintf(buf,"%d",cur_val);
          constNode * val = new constNode(constant(cur_val),
                                          buf, the_suespec->coord());
          cur->init(val);
        }
        cur_val++;
      }
  }
}

// ------------------------------------------------------------
// Expression checker
// ------------------------------------------------------------

// Remove any stale type information from expressions

class remove_stale_type_walker : public Walker
{
public:

  remove_stale_type_walker()
    : Walker(Postorder, Subtree)
  {}

  virtual void at_expr(exprNode * the_expr, Order ord)
  {
return;
    // -- Remove type information from everything but constNode and
    // the casts.

    if ((the_expr->typ() != Const) &&
	(the_expr->typ() != Cast)) {
      typeNode * t = the_expr->get_type();
      //delete t;
    }
  }
};

// -- Check one expression tree

void semcheck_expr_visitor::check(exprNode * e)
{
  // -- Remove any old type information
  remove_stale_type_walker rstw;

  e->walk(rstw);

  // -- Rebuild the types

  semcheck_expr_visitor sv;

  e->visit(&sv);
}

// ------------------------------------------------------------
// Visitor functions
// ------------------------------------------------------------

void semcheck_expr_visitor::at_const(constNode * the_const)
{
  // -- Nothing to do for constants
}

void semcheck_expr_visitor::at_id(idNode * the_id)
{
  // -- Set the type from it's declaration...

  declNode * d = the_id->decl();
  if (d) {
    typeNode * t = d->/* no_tdef_ */ type();
    if (t) {
      the_id->type((typeNode *) ref_clone_changer::clone(t, false));
      return;
    }
  }
  // -- Undeclared identifiers default to void type

  the_id->type(default_type());
}

void semcheck_expr_visitor::at_binary(binaryNode * the_binary)
{
  typeNode * ty;

  // -- Check the sub-expressions...

  the_binary->left()->visit(this);
  the_binary->right()->visit(this);

  // -- Depends on the operation...

  exprNode * lhs = the_binary->get_left();
  exprNode * rhs = the_binary->get_right();

  unsigned int opid = the_binary->op()->id();

  if (opid == Operator::Index) {

    // --
    // -- Array subscript (ANSI 6.3.2.1)
    // --

    // -- Perform integral promotion on the index argument...

    rhs = exprNode::integral_promotions(rhs);

    // -- Index must be an integer

    if (! rhs->no_tdef_type()->is_integer())
      if (semcheck_walker::showErrors)
	CBZ::SyntaxError(rhs->coord(), "Array subscript must be an integral type");

    // -- Name must be an array or pointer to some_type

    typeNode * nt = lhs->no_tdef_type();

    if ((nt->typ() != Array) && (nt->typ() != Ptr)) {
      if (semcheck_walker::showErrors)
	CBZ::SyntaxError(lhs->coord(),
			 "Array subscript must be applied to an array or ptr type");
      ty = default_type();
    }
    else {

      // -- If the name is pointer to some_type, then the result is some_type

      ty = (typeNode *) ref_clone_changer::clone(nt->type(), false);
    }
  }
  else {

    // -- Regular binary operators
    // -- Perform the type checking

    ty = check_binary(opid, lhs, rhs, &lhs, &rhs);
  }

  // -- Update the node

  the_binary->type(ty);
  the_binary->left(lhs);
  the_binary->right(rhs);
}

void semcheck_expr_visitor::at_unary(unaryNode * the_unary)
{
  // -- Check the sub-expressions...

  // djimenez
  // sizeof(type) doesn't have an expr(), so we have to handle it specially:
  // we assume the sizeof_type() field is OK and just make
  // this expression of type unsigned int

  if (the_unary->op()->id() == Operator::SIZEOF && the_unary->sizeof_type()) {
	typeNode *ty = new primNode(basic_type::UInt);
	the_unary->type(ty);
	return;
  }

  the_unary->expr()->visit(this);

  // -- Depends on the operation...

  exprNode * ex = the_unary->get_expr();

  unsigned int opid = the_unary->op()->id();

  // -- Perform the type checking

  typeNode * ty = check_unary(opid, ex, &ex);

  // -- Update the node

  the_unary->type(ty);
  the_unary->expr(ex);
}

// --
// -- Cast (ANSI 6.3.4)
// --

void semcheck_expr_visitor::at_cast(castNode * the_cast)
{
  // -- Check the sub-expressions...

  the_cast->expr()->visit(this);

  // -- Checks are TBD

  // -- Automatically converts to the given type
}

// --
// -- Comma (ANSI 6.3.17)
// --

void semcheck_expr_visitor::at_comma(commaNode * the_comma)
{
  // -- Check the sub-expressions...

  list_visitor(the_comma->exprs(), this);

  // -- Result gets the type of the last expression

  typeNode * result_type = the_comma->exprs().back()->type();
  the_comma->type((typeNode *) ref_clone_changer::clone(result_type, false));
}

// --
// -- Ternary operator (ANSI 6.3.15)
// --

void semcheck_expr_visitor::at_ternary(ternaryNode * the_ternary)
{
  // -- Check the sub-expressions...

  the_ternary->cond()->visit(this);
  the_ternary->true_br()->visit(this);
  the_ternary->false_br()->visit(this);
  // djimenez - I think this might fix something
  typeNode *result_type = the_ternary->false_br()->type();
  the_ternary->type((typeNode *) ref_clone_changer::clone(result_type, false));

  // djimenez - done 

  // -- TBD Rules, rules, rules
}

// --
// -- Function call (ANSI 6.3.2.2)
// --

void semcheck_expr_visitor::at_call(callNode * the_call)
{
  // -- Check the sub-expressions...

  the_call->name()->visit(this);
  list_visitor(the_call->args(), this);

  // -- The name should be a funcNode or ptr to funcNode

  bool ok = false;
  typeNode * ft = the_call->name()->no_tdef_type();
  typeNode * ret = 0;

  if (ft->typ() == Func) {
    ok = true;
    ret = ft->no_tdef_type();
  }
  else
    if (ft->typ() == Ptr) {
      typeNode * subtype = ft->no_tdef_type();
      if (subtype->typ() == Func) {
	ok = true;
	ret = subtype->no_tdef_type();
      }
    }

  if (ok) {

    // -- Return type may not be array 

    if (ret->typ() == Array)
      if (semcheck_walker::showErrors)
	CBZ::SyntaxError(the_call->coord(),
			 "Functions may not return arrays");

    // -- Type of the call is the return type of the function

    the_call->type( (typeNode *) ref_clone_changer::clone(ret, false));
  }
}

// --
// --
// --

void semcheck_expr_visitor::at_initializer(initializerNode * the_initializer)
{
  // -- Check the sub-expressions...

  list_visitor(the_initializer->exprs(), this);
}

// ------------------------------------------------------------
// Checking functions
// ------------------------------------------------------------

typeNode * semcheck_expr_visitor::check_unary(unsigned int opid, exprNode * ex,
		       exprNode ** new_ex)
{
  typeNode * result_type = 0;

  // --
  // -- Increment and decrement (ANSI 6.3.2.4, ANSI 6.3.3.1)
  // --

  if ((opid == Operator::ICR) ||
      (opid == Operator::POSTINC) ||
      (opid == Operator::PREINC) ||
      (opid == Operator::DECR) ||
      (opid == Operator::POSTDEC) ||
      (opid == Operator::PREDEC)) {

    // -- Apply integral promotions..

    ex = exprNode::integral_promotions(ex);

    // -- Must be scalar and lvalue

    if (! ex->no_tdef_type()->is_scalar() ||
	! ex->is_lvalue()) {
      if (semcheck_walker::showErrors)
	CBZ::SyntaxError(ex->coord(), "Argument to increment/decrement must be a scalar "
			 "and must be a modifiable lvalue");
      result_type = default_type();
    }
    else {
      // -- Result type is the same

      result_type = (typeNode *) ref_clone_changer::clone(ex->type(), false);
    }
  }

  // --
  // -- Address and indirection (ANSI 6.3.3.2)
  // --

  if (opid == Operator::INDIR) {

    // -- Argument must be point to something

    if (ex->no_tdef_type()->typ() != Ptr 
	// djimenez - an array can be "dereferenced" as well
    	&& ex->no_tdef_type()->typ() != Array) {
      if (semcheck_walker::showErrors)
	CBZ::SyntaxError(ex->coord(),
			 "Cannot dereference a non-pointer");
      result_type = default_type();
    }
    else {

      // -- Follow the pointer...

      result_type = (typeNode *) ref_clone_changer::clone(ex->no_tdef_type()->type(), false) ;
    }
  }

  if (opid == Operator::ADDRESS) {

    // -- Must be an lvalue, and cannot be "register" or bitfield (TBD)

    if (! ex->is_lvalue()) {
      if (semcheck_walker::showErrors)
	CBZ::SyntaxError(ex->coord(),
			 "Cannot take the address of a non-lvalue");
      result_type = default_type();
    }
    else {
      // -- Result type is pointer to type of expression

      typeNode * base = (typeNode *) ref_clone_changer::clone(ex->type(), false);
      result_type = new ptrNode(typeNode::NONE, base, base->coord());

      /*
      output_context oc(cout);
      cout << "Type in:";
      print_walker::print(ex->type(), cout);
      cout << "Type out:";
      print_walker::print(result_type, cout);      
      */
    }
  }

  // --
  // -- Unary +/-
  // --

  if ((opid == Operator::UPLUS) ||
      (opid == Operator::UMINUS)) {

    // -- Apply integral promotions..

    ex = exprNode::integral_promotions(ex);

    // -- Must be arithmetic

    typeNode * t = ex->no_tdef_type();

    if ( ! t->is_arithmetic() ) {
      if (semcheck_walker::showErrors)
	CBZ::SyntaxError(ex->coord(),
			 "Argument to unary +/- must be arithmetic");
      result_type = default_type();
    }
    else {
      // -- Result is the same type

      result_type = (typeNode *) ref_clone_changer::clone(t, false);
    }
  }

  // --
  // -- Unary ~
  // --

  if (opid == '~') {

    // -- Apply integral promotions..

    ex = exprNode::integral_promotions(ex);

    // -- Must be integral

    typeNode * t = ex->no_tdef_type();
    if ( ! t->is_integer() ) {
      if (semcheck_walker::showErrors)
	CBZ::SyntaxError(ex->coord(),
			 "Argument to unary ~ must be integral");
      result_type = default_type();
    }
    else {
      // -- Result is the same type

      result_type = (typeNode *) ref_clone_changer::clone(t, false);
    }
  }

  // --
  // -- Unary !
  // --

  if (opid == '!') {

    // -- Must be scalar

    typeNode * t = ex->no_tdef_type();
    if ( ! t->is_scalar() ) {
      if (semcheck_walker::showErrors)
	CBZ::SyntaxError(ex->coord(),
			 "Argument to unary ! must be scalar");
      result_type = default_type();
    }
    else {
      // -- Result is int

      result_type = new primNode(ex->coord());
    }
  }

  // --
  // -- Sizeof (ANSI 6.3.3.4)
  // --

  if (opid == Operator::SIZEOF) {

    // -- Checks are TBD

    // -- Result is int

    result_type = new primNode(ex->coord());
  }

  * new_ex = ex;

  return result_type;
}

typeNode * semcheck_expr_visitor::check_binary(unsigned int opid, exprNode * lhs, exprNode * rhs,
			exprNode ** new_lhs, exprNode ** new_rhs)
{
  typeNode * result_type = 0;
  // --
  // -- Struct/Union access...
  // --

  if ((opid == Operator::ARROW) ||
      (opid == '.')) {
    typeNode * t = lhs->no_tdef_type();

    // -- Only apply -> to a pointer

    if (opid == Operator::ARROW) {
      if (t->typ() != Ptr) {
	if (semcheck_walker::showErrors)
	  CBZ::SyntaxError(lhs->coord(),
			   "Left operand of \"->\" must be a pointer");
      }
      else
	t = t->no_tdef_type();
    }

    // -- Make sure the LHS is a struct or union

    if ((t->typ() != Struct) &&
	(t->typ() != Union)) {
      if (semcheck_walker::showErrors)
	CBZ::SyntaxError(lhs->coord(),
			 "Left operand of \"->\"/\".\" must be a struct or union");
      result_type = default_type();
    }
    else {
      sueNode * sue = (sueNode *) t;
      suespecNode * sp = sue->spec();
      idNode * field = (idNode *) rhs;
      string & nm = field->name();

      // -- Try to find the field

      bool found = false;
      declNode * field_decl = sp->find_field(nm);
      typeNode * field_type = 0;

      if (field_decl) {
	field_type = field_decl->type();
	found = true;
      }

      // -- Set the decl of the field name to point to the field
      // declaration.

      field->decl(field_decl);

      // -- If found, the result type is the type of the field

      if (found) {
	if (field_type->typ() == Tdef) {
		if (((tdefNode *)field_type)->def() == NULL) 
      if (semcheck_walker::showErrors)
	CBZ::SyntaxError(rhs->coord(), "internal error: can't"
			 " find a typedef definition!");
	}
	result_type = (typeNode *) ref_clone_changer::clone(field_type, false);
      }
      else {
      if (semcheck_walker::showErrors)
	CBZ::SyntaxError(rhs->coord(),
			 string("Struct/union \"") + sp->name() + "\" has no field \"" +
			 nm + "\".");
	result_type = default_type();
      }
    }

  }

  // --
  // -- Multiplicative operators (ANSI 6.3.5)
  // --

  if ((opid == '*') ||
      (opid == '/') ||
      (opid == '%')) {

    // -- Apply the arithmetic conversions

    pair<exprNode *, exprNode *> res =
      exprNode::usual_arithmetic_conversions(lhs, rhs);
    lhs = res.first;
    rhs = res.second;

    // -- Get the resulting types

    typeNode * lt = lhs->no_tdef_type();
    typeNode * rt = rhs->no_tdef_type();

    // -- Mod requires integral operands

    if ((opid == '%') &&
	(! lt->is_integer() ||
	 ! rt->is_integer())) {
      if (semcheck_walker::showErrors)
	CBZ::SyntaxError(lhs->coord(),
			 "The mod operator requires integral operands");
      result_type = default_type();
    }
    else

      // -- Mult/div require arithmetic operands

      if (((opid == '*') ||
	   (opid == '/')) &&
	  (! lt->is_arithmetic() ||
	   ! rt->is_arithmetic())) {
      if (semcheck_walker::showErrors)
	CBZ::SyntaxError(lhs->coord(),
			 "The * and / operators require arithmetic operands");
	result_type = default_type();
      }
      else
	result_type = (typeNode *) ref_clone_changer::clone(lt, false);
  }

  // --
  // -- Additive operators (ANSI 6.3.6)
  // --

  if ((opid == '+') ||
      (opid == '-')) {

    typeNode * lt = lhs->no_tdef_type();
    typeNode * rt = rhs->no_tdef_type();

    // -- Check the normal arithmetic case

    if (lt->is_arithmetic() && rt->is_arithmetic()) {

      // -- Apply the arithmetic conversions

      pair<exprNode *, exprNode *> res =
	exprNode::usual_arithmetic_conversions(lhs, rhs);
      lhs = res.first;
      rhs = res.second;

      result_type = (typeNode *) ref_clone_changer::clone(lhs->type(), false);
    }
    else
      // -- Check the subtraction of pointers case

      if (lt->is_pointer() && rt->is_pointer()) {

	// -- This is only allowed for subtraction

	if (opid == '+') {
      if (semcheck_walker::showErrors)
	CBZ::SyntaxError(lhs->coord(),
			 "Addition of pointers is not allowed");
	  result_type = default_type();
	}
	else
	  // -- TBD: pointers must be compatible
	  // djimenez - ANSI 6.3.6 - the result of pointer
	  // subtraction is an implementation defined signed
	  // integral type (not a pointer).

	  //result_type = (typeNode *) ref_clone_changer::clone(lt, false);
	  result_type = new primNode (basic_type::SInt);
      }
      else
	// -- Check the pointer +/- number case

	if (lt->is_pointer() && rt->is_integer()) {

	  // -- TBD Do we need integral promotion here?
	  
	  result_type = (typeNode *) ref_clone_changer::clone(lt, false);
	}
	else
	  // -- Check the number + pointer case

	  if (rt->is_pointer() && lt->is_integer() && (opid == '+')) {

	    // -- TBD Do we need integral promotion here?
	  
	    result_type = (typeNode *) ref_clone_changer::clone(rt, false);
	  }
	  else {
	    // -- None of those cases matched
      if (semcheck_walker::showErrors)
	CBZ::SyntaxError(lhs->coord(),
			 "Illegal operands for + or -");
	    result_type = default_type();
	  }
  }

  // --
  // -- Bitwise shift operators (ANSI 6.3.7)
  // --

  if ((opid == Operator::LS) ||
      (opid == Operator::RS)) {

    // -- Perform integral promotions

    lhs = exprNode::integral_promotions(lhs);
    rhs = exprNode::integral_promotions(rhs);

    // -- Both arguments must be integral

    if (! lhs->no_tdef_type()->is_integer() ||
	! rhs->no_tdef_type()->is_integer()) {
      if (semcheck_walker::showErrors)
	CBZ::SyntaxError(lhs->coord(),
			 "Arguments to bitwise shift must be integral");
      result_type = default_type();
    }
    else {
      // -- Apply the arithmetic conversions
      
      pair<exprNode *, exprNode *> res =
	exprNode::usual_arithmetic_conversions(lhs, rhs);
      lhs = res.first;
      rhs = res.second;

      result_type = (typeNode *) ref_clone_changer::clone(lhs->no_tdef_type(), false);
    }
  }

  // --
  // -- Relational operators (ANSI 6.3.8)
  // --

  if ((opid == '>') ||
      (opid == '<') ||
      (opid == Operator::GE) ||
      (opid == Operator::LE)) {

    typeNode * lt = lhs->no_tdef_type();
    typeNode * rt = rhs->no_tdef_type();

    // -- Check the normal arithmetic case

    if (lt->is_arithmetic() && rt->is_arithmetic()) {

      // -- Apply the arithmetic conversions

      pair<exprNode *, exprNode *> res =
	exprNode::usual_arithmetic_conversions(lhs, rhs);
      lhs = res.first;
      rhs = res.second;

      // -- Result type is int

      result_type = new primNode(lhs->coord());
    }
    else
      // -- Check the comparison of pointers

      if (lt->is_pointer() && rt->is_pointer()) {

	// -- TBD check pointer compatibility

	// -- Result type is int

	result_type = new primNode(lhs->coord());
      }
      else {
	// -- None of those cases matched
      if (semcheck_walker::showErrors)
	CBZ::SyntaxError(lhs->coord(),
			 "Illegal operands for comparison");
	result_type = default_type();
      }
  }

  // --
  // -- Equality operators (ANSI 6.3.9)
  // --

  if ((opid == Operator::EQ) ||
      (opid == Operator::NE)) {

    typeNode * lt = lhs->no_tdef_type();
    typeNode * rt = rhs->no_tdef_type();

    // -- Check the normal arithmetic case

    if (lt->is_arithmetic() && rt->is_arithmetic()) {

      // -- Apply the arithmetic conversions

      pair<exprNode *, exprNode *> res =
	exprNode::usual_arithmetic_conversions(lhs, rhs);
      lhs = res.first;
      rhs = res.second;

      // -- Result type is int

      result_type = new primNode(lhs->coord());
    }
    else
      // -- Check the comparison of pointers

      if (lt->is_pointer() && rt->is_pointer()) {

	// -- TBD check pointer compatibility
	// -- Result type is int

	result_type = new primNode(lhs->coord());
      }
      else
	// -- Check comparison of pointer and constant

	if ((lt->is_pointer() && (rt->typ() == Prim)) ||
	    (rt->is_pointer() && (lt->typ() == Prim))) {

	  // -- TBD check pointer compatibility
	  // -- Result type is int

	  result_type = new primNode(lhs->coord());
	}
	else {
	  // -- None of those cases matched
      if (semcheck_walker::showErrors)
	CBZ::SyntaxError(lhs->coord(),
			 "Illegal operands for comparison");
	  result_type = default_type();
	}
  }

  // --
  // -- Bitwise AND, OR, XOR (ANSI 6.3.10 6.3.11 6.3.12)
  // --

  if ((opid == '&') ||
      (opid == '^') ||
      (opid == '|')) {

    // -- Perform integral promotions

    lhs = exprNode::integral_promotions(lhs);
    rhs = exprNode::integral_promotions(rhs);

    // -- Both arguments must be integral

    if (! lhs->no_tdef_type()->is_integer() ||
	! rhs->no_tdef_type()->is_integer()) {
      if (semcheck_walker::showErrors)
	CBZ::SyntaxError(lhs->coord(),
			 "Arguments to bitwise operators must be integral");
      result_type = default_type();
    }
    else {
      // -- Apply the arithmetic conversions

      pair<exprNode *, exprNode *> res =
	exprNode::usual_arithmetic_conversions(lhs, rhs);
      lhs = res.first;
      rhs = res.second;

      result_type = (typeNode *) ref_clone_changer::clone(lhs->no_tdef_type(), false);
    }
  }

  // --
  // -- Logical AND, OR (ANSI 6.3.13 6.3.14)
  // --

  if ((opid == Operator::ANDAND) ||
      (opid == Operator::OROR)) {

    typeNode * lt = lhs->no_tdef_type();
    typeNode * rt = rhs->no_tdef_type();

    if (lt->is_scalar() && rt->is_scalar())
      result_type = new primNode(lhs->coord());
    else {
      if (semcheck_walker::showErrors)
	CBZ::SyntaxError(lhs->coord(),
			 "Arguments to &&/|| must be scalar");
      result_type = default_type();
    }
  }

  // --
  // -- Simple assignment (6.3.16.1)
  // --

  if (opid == '=') {

    if ( ! lhs->is_lvalue() ) {
      if (semcheck_walker::showErrors)
	CBZ::SyntaxError(lhs->coord(),
			 "Left-hand side of assignment must be a modifiable lvalue");
      result_type = default_type();
    }
    else {

      typeNode * lt = lhs->no_tdef_type();
      typeNode * rt = rhs->no_tdef_type();

      // TBD check assignment compatibility

      // -- If the types are not the same...

      if ( ! ( *lt <= *rt ))
	if (lt->is_arithmetic() &&
	    rt->is_arithmetic()) {

	  // -- Arithmetic types may be different..cast the RHS

	  rhs = new castNode((typeNode *) ref_clone_changer::clone(lt, false),
			     rhs, true, rhs->coord());
	  // -- Result type is that of the LHS

	  result_type = (typeNode *) ref_clone_changer::clone(lt, false);

	  // -- Remove qualifiers

	  result_type->type_qualifiers(typeNode::NONE);
	}
	else {
	  // djimenez
	  // a void pointer can be assigned to any pointer type
	  if (lt->typ() == Ptr 
		&& rt->typ() == Ptr 
		&& rt->type()->is_void()) {
	  		result_type = (typeNode *) ref_clone_changer::clone(lt, false);
	  } else {
	    if (semcheck_walker::showErrors)
	      CBZ::SyntaxError(lhs->coord(),
			       "Incompatible types for assignment");
	    result_type = default_type();
	  }
	}
      else {

	// -- Result type is that of the LHS

	result_type = (typeNode *) ref_clone_changer::clone(lt, false);

	// -- Remove qualifiers

	result_type->type_qualifiers(typeNode::NONE);
      }
    }
  }

  // --
  // -- Compound assignment (6.3.16.2)
  // --

  if (opid == Operator::MULTassign) {
    typeNode * rtype = check_binary('*', lhs, rhs, &lhs, &rhs);
    result_type = check_binary('=', lhs, rhs, &lhs, &rhs);
    //delete rtype;
  }

  if (opid == Operator::DIVassign) {
    typeNode * rtype = check_binary('/', lhs, rhs, &lhs, &rhs);
    result_type = check_binary('=', lhs, rhs, &lhs, &rhs);
    //delete rtype;
  }

  if (opid == Operator::MODassign) {
    typeNode * rtype = check_binary('%', lhs, rhs, &lhs, &rhs);
    result_type = check_binary('=', lhs, rhs, &lhs, &rhs);
    //delete rtype;
  }

  if (opid == Operator::PLUSassign) {
    typeNode * rtype = check_binary('+', lhs, rhs, &lhs, &rhs);
// djimenez
// this is a little different from the other op*assigns because
// we may need to do something like pointer += integer.
    typeNode * save = rhs->type();
    rhs->type (rtype);
    result_type = check_binary('=', lhs, rhs, &lhs, &rhs);
    rhs->type (save);
    ////delete rtype;
  }

  if (opid == Operator::MINUSassign) {
// same thing as above
    typeNode * rtype = check_binary('-', lhs, rhs, &lhs, &rhs);
    typeNode * save = rhs->type();
    rhs->type (rtype);
    result_type = check_binary('=', lhs, rhs, &lhs, &rhs);
    rhs->type (save);
  }

  if (opid == Operator::LSassign) {
    typeNode * rtype = check_binary(Operator::LS, lhs, rhs, &lhs, &rhs);
    result_type = check_binary('=', lhs, rhs, &lhs, &rhs);
    //delete rtype;
  }

  if (opid == Operator::RSassign) {
    typeNode * rtype = check_binary(Operator::RS, lhs, rhs, &lhs, &rhs);
    result_type = check_binary('=', lhs, rhs, &lhs, &rhs);
    //delete rtype;
  }

  if (opid == Operator::ANDassign) {
    typeNode * rtype = check_binary('&', lhs, rhs, &lhs, &rhs);
    result_type = check_binary('=', lhs, rhs, &lhs, &rhs);
    //delete rtype;
  }

  if (opid == Operator::ERassign) {
    typeNode * rtype = check_binary('^', lhs, rhs, &lhs, &rhs);
    result_type = check_binary('=', lhs, rhs, &lhs, &rhs);
    //delete rtype;
  }

  if (opid == Operator::ORassign) {
    typeNode * rtype = check_binary('|', lhs, rhs, &lhs, &rhs);
    result_type = check_binary('=', lhs, rhs, &lhs, &rhs);
    //delete rtype;
  }

  * new_lhs = lhs;
  * new_rhs = rhs;

  return result_type;
}

typeNode * semcheck_expr_visitor::default_type()
{
  return new primNode(typeNode::NONE, basic_type::Void);
}

