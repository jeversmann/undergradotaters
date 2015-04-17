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

// --------------------------------------------------------------------
// Constructors
// --------------------------------------------------------------------

funcNode::funcNode(Type_qualifiers tq, decl_list * args, typeNode * returns,
		   const Coord coord)
  : typeNode(Func, tq, returns, coord),
    _args(),
    _is_knr(false)
{
  if (args) {
    _args.swap(*args);
    //delete args;
  }
}

// ------------------------------------------------------------
//  Type Equal
// ------------------------------------------------------------

bool funcNode::qualified_equal_to(typeNode * node2,
				  bool strict_toplevel, bool strict_recursive)
{
  funcNode * n2 = (funcNode *) node2;

  // -- Check the return value...

  if (! equal_to(returns(), n2->returns(),
		 strict_recursive, strict_recursive))
    return false;

  // -- If either arglist is empty, return successful

  decl_list & args1 = args();
  decl_list & args2 = n2->args();

  if (args1.empty() || args2.empty())
    return true;

  // -- Check number of args

  if (args1.size() != args2.size())
    return false;

  // -- Compare each arg...
  decl_list_p p1;
  decl_list_p p2;

  for (p1 = args1.begin(), p2 = args2.begin();
       (p1 != args1.end()) && (p1 != args2.end()) ;
       ++p1, ++p2) {
    typeNode * type1;
    typeNode * type2;

    // -- If arg is a declaration, get the type...

    if ((*p1)->typ() == Decl)
      type1 = ((declNode *)(*p1))->type();
    else
      type1 = (typeNode *)(*p1);

    if ((*p2)->typ() == Decl)
      type2 = ((declNode *)(*p2))->type();
    else
      type2 = (typeNode *)(*p2);

    if (! equal_to(type1, type2,
		   strict_recursive, strict_recursive))
      return false;
  }

  return true;
}

// ------------------------------------------------------------
//  Argument list checks
// ------------------------------------------------------------

bool funcNode::is_void_args()
{
  decl_list & the_args = args();

  if (the_args.size() == 1) {
    Node * f = the_args.front();
    return f->datatype()->is_void();
  }
  return false;
}

// ----------------------------------------------------------------------
// FunctionConflict
//
// This routine is called if a function declaration, i.e. an Func, is
// parsed and there is already a declaration.
//
// In general, either of ORIG or NEW could be a declaration, a
// prototype, or the start of a function definition and it is not
// possible to tell which is which.  In particular, the expression
//
//       int f()
//
// may be an old-style function declaration that permits an arbitrary
// number of arguments of types compatible with "the usual unary
// conversions" or it may be a function definition for a function with
// no arguments.
// 
// We assume that In a "standard" program, the original expression is
// an ANSI-C prototype and the create expression is the function
// declaration
//
// In the general case, this confirms that the two specifications are
// the same.  If the create declaration forces a revision of the old
// one
//
//     e.g.    int foo();
//             int foo(void) { ... }
//
// then it is required that the OLD one be mutated accordingly since
// there are already references to the old one
//
// ----------------------------------------------------------------------

// Must mutate original if changes required for consistency

bool funcNode::check_conversions()
{
  decl_list & the_args = args();
  decl_list_p p;

  // -- All parameters must be compatible...

  for (p = the_args.begin(); p != the_args.end(); ++p) {

    // -- Get the argument type

    typeNode * the_type = (*p)->datatype();

    // -- Check conversions..

    typeNode * conv = the_type->usual_unary_conversion_type();
    bool compatible = (* the_type == * conv );
    //delete conv;

    if ( ! compatible || the_type->is_ellipsis())
      return false;
  }

  return true;
}

// -- From ANSI 6.5.4.3

bool funcNode::is_compatible_with(funcNode * nfunc)
{
  funcNode * ofunc = this;

  // -- Return types must be equal...

  if (! (* ofunc->returns() == * nfunc->returns()) )
    return false;

  // -- Check the parameters..

  decl_list & oargs = ofunc->args();
  decl_list & nargs = nfunc->args();

  // -- CASE: both lists are in prototype form..

  if (! oargs.empty() && ! nargs.empty()) {
    decl_list_p optr;
    decl_list_p nptr;

    // -- The parameter lists must be of the same length

    if (oargs.size() != nargs.size())
      return false;

    // -- All parameters must be compatible...

    for (optr = oargs.begin(), nptr = nargs.begin();
	 (optr != oargs.end()) && (nptr != nargs.end());
	 ++optr, ++nptr) {
      typeNode * otype = (*optr)->datatype_superior();
      typeNode * ntype = (*nptr)->datatype_superior();

      if ( * otype != * ntype ) {
	// (*optr) = (*nptr); Why do this?
	return false;
      }
    }

    return true;
  }
  else
    if (oargs.empty() && nargs.empty())
      // -- CASE: Both lists are empty
      return true;

    else {

      // -- CASE: One of the arguments lists is empty. If the first
      // (original) one is empty, copy the arguments to that
      // definition.

      if (oargs.empty()) {

	decl_list_p fptr;

	// -- Fill the empty arg-list with copies of the arguments

	for (fptr = oargs.begin();
	     fptr != oargs.end();
	     ++fptr)
	  nargs.push_back((declNode *) ref_clone_changer::clone(*fptr, false));
      }

      return true;
    }

  // --------- THE REST OF THIS IS NOT USED -------------------------------

  // -- CASE: Check for <Type> f(void)  vs  <Type> f()

  if (ofunc->is_void_args())
    return true;

  // -- CASE: Check for <Type> f()  vs  <Type> f(void)

  if (nfunc->is_void_args()) {

    // -- Make a (void) arg list for the original...

    declNode * void_decl = new declNode((typeNode *) new primNode(basic_type::Void), declNode::NONE);
    void_decl->decl_location(declNode::FORMAL);

    ofunc->args().push_front(void_decl);
    
    return true;
  }

  // -- CASE: One of the arg lists is empty
  // -- The types must be the usual unary conversions...

  if (! ofunc->check_conversions())
    return false;

  if (! nfunc->check_conversions())
    return false;

  return true;
}

// ------------------------------------------------------------
// Add parameter types
// ------------------------------------------------------------

// Support for old-style declaration lists
// AddParameterTypes
//
//   This takes a old-style function declaration `decl', which has a
//   list of identifiers (Id nodes) as its argument list, and and a list of
//   the parameter declarations `types', and converts the list
//   of identifiers to a list of declarations (Decl nodes) with the types
//   determined by the declaration list.  It is called upon the reduction of a
//   procedure defined using the old-sytle function declaration.
//
//   In: decl = (Decl name (Func args=(List Id's) returntype) NULL NULL)
//       types = (List Decl's)
//   Out : (Decl name (Func args=(List Decl's) returntype) NULL NULL)

void funcNode::add_parameter_types(decl_list * types)
{
  decl_list & ids = args();

  decl_list_p ids_p;
  decl_list_p types_p;

  bool found;

  // -- Declarator list may be empty or null
  if (types) {

    // -- For each declaration...

    for (types_p = types->begin(); types_p != types->end(); ++types_p) {

      declNode * t = ( * types_p );

      const string & the_name = t->name();
      found = false;

      // -- Find the matching identifier in the parameter
      // list. Because of the way that identifier.list is defined in
      // the parser, this is actually a list of declNodes with no
      // types.

      for (ids_p = ids.begin(); ids_p != ids.end(); ++ids_p) {

	declNode * the_param = (*ids_p);

	if (the_param->name() == the_name) {

	  found = true;  // name does exist

	  if (! the_param->type()) {
	    // if a name appears twice in the identifer list,
	    //   it will be caught by DefineProc when its adds the
	    //   formals to the scope of the body

	    // -- Replace the identifier with the declaration
	    //delete  the_param;
	    (*ids_p) = t;
	    found = true;

	    break;
	  }
	  else {
	    CBZ::SyntaxError(t->coord(),
			     string("multiple declarations for parameter `") + 
			     the_name + string("'"));
	    break;
	  }
	}
      }

      if (! found)
	CBZ::SyntaxError(t->coord(),
			 string("declaration for nonexistent parameter `") + 
			 the_name + string("'"));

    } // END for types

    //delete types;

  } // END if types

  // check for missing declarations
  
  for (ids_p = ids.begin(); ids_p != ids.end(); ++ids_p) {

    declNode * dec = (*ids_p);

    if (! dec->type()) {

      // -- Issue a warning and give the parameter a default type..

      CBZ::Warning(2, dec->coord(),
		   string("parameter `") + dec->name() + 
		   string("' defaults to signed int"));

      dec->type(new primNode(dec->coord()));
    }
  }
}

// ------------------------------------------------------------
//  Walker
// ------------------------------------------------------------

void funcNode::visit(Visitor * the_visitor) 
{
  the_visitor->at_func(this);
}

void funcNode::walk(Walker & the_walker)
{
  Walker::Order ord = the_walker.order(); 

  if (ord == Walker::Preorder || ord == Walker::Both)
    the_walker.at_func(this, Walker::Preorder);

  if (the_walker.depth() == Walker::Subtree) {
    // -- Visit the children 

    list_walker(args(), the_walker);

    if (returns())
      returns()->walk(the_walker);
  }

  if (ord == Walker::Postorder || ord == Walker::Both)
    the_walker.at_func(this, Walker::Postorder);
}

// ------------------------------------------------------------
// Output
// ------------------------------------------------------------

void funcNode::output_type(output_context & ct, Node * parent, Assoc context, Type_qualifiers q)
{
  if (context == Left) {
    const string & qs = type_qualifiers_name();

    if (! qs.empty())
      ct.space();
    ct << qs;

    returns()->output_type(ct, this, Left, q);
  }
  else {
    ct << '(';
    if (is_knr()) {
	// output old-style declarator.
	// why not stick in the prototypes?
	// because ANSI C is more strict about
	// arguments matching and will complain
	// if the number of args doesn't match.
	// it will also try to promote things
	// that the author might just want
	// reinterpreted.
        decl_list_p i=args().begin(); 
	for (;;) {
	    ct << (*i)->name();
	    i++;
	    if (i == args().end()) break;
	    ct << ", ";
	}
        ct << ")\n";
        output_delim_list(args(), ct, this, ';');
	ct << ';';
    } else {
        output_delim_list(args(), ct, this, ',');
	ct << ')';
    }

    returns()->output_type(ct, this, Right, q);
  }
}

// ------------------------------------------------------------
//  Changer
// ------------------------------------------------------------

Node * funcNode::change(Changer & the_changer, bool redispatch)
{
  Changer::Order ord = the_changer.order(); 
  funcNode * the_func = this;

  if ((ord == Changer::Preorder || ord == Changer::Both) && ! redispatch)
    the_func = (funcNode *) the_changer.at_func(the_func, Changer::Preorder);

  if (the_func) {

    if (the_func != this)
      return the_func->change(the_changer, true);

    change_list(the_func->args(), the_changer);

    typeNode * old_returns = the_func->returns();
    if (old_returns) {
      typeNode * new_returns = (typeNode *) old_returns->change(the_changer);
      if (old_returns != new_returns) {
	//if (the_changer.delete_old())
	  //delete old_returns;
        the_func->returns(new_returns);
      }
    }

  }

  if ((ord == Changer::Postorder || ord == Changer::Both) && ! redispatch)
    the_func = (funcNode *) the_changer.at_func(the_func, Changer::Postorder);

  return the_func;
}


// ------------------------------------------------------------
// Destructor
// ------------------------------------------------------------

funcNode::~funcNode()
{
  //delete_list(_args);
}
