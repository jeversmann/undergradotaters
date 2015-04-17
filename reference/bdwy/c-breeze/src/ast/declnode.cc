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

declNode::declNode(const char * name, Storage_class sc, typeNode * the_type,
		   exprNode * init, exprNode * bitsize,
		   const Coord coord)
  : defNode(Decl, coord),
    _type(the_type),
    _name(string(name)),
    _decl_location(UNKNOWN),
    _storage_class(sc),
    _is_redundant_extern(false),
    _init(init), 
    _bitsize(bitsize),
    _references(0),
    _ref_list(),
	_is_return_value(false),
    _attribs()
{}

// -- Was ConvertIdToDecl

declNode::declNode(idNode * id, Storage_class sc, typeNode * the_type,
		   exprNode * init, exprNode * bitsize)
  : defNode(Decl, id->coord()),
    _type(the_type),
    _name(id->name()),
    _decl_location(UNKNOWN),
    _storage_class(sc),
    _is_redundant_extern(false),
    _init(init), 
    _bitsize(bitsize),
    _references(0),
    _ref_list(),
	_is_return_value(false),
    _attribs()
{
  //delete id;
}

// --- BuildEnumConst from sue.c

declNode::declNode(idNode * idname, exprNode * value)
  : defNode(Decl, idname->coord()),
    _type(0),
    _name(idname->name()),
    _decl_location(ENUM),
    _storage_class(NONE),
    _is_redundant_extern(false),
    _init(value),
    _bitsize(0),
    _references(0),
    _ref_list(),
	_is_return_value(false),
    _attribs()
{
  primNode * tp = new primNode(typeNode::CONST);
  type(tp);

  /*
  orig = CBZ::current_unit->ids()->insert(name(), this);
  if (orig) {
    CBZ::SyntaxError(coord(),
		     string("enum constant `") + 
		     orig->name() + 
		     string("' redeclares previous identifier"));
    cerr << "\tPrevious definition: " << orig->coord() << endl;
  }
  */

  //delete idname;
}

// ------------------------------------------------------------
// Make an abstract declarator into a  declaration
// ------------------------------------------------------------

declNode::declNode(typeNode * the_type, Storage_class sc = NONE)
  : defNode(Decl, the_type->coord()),
    _type(the_type),
    _name(string("")),
    _decl_location(UNKNOWN),
    _storage_class(sc),
    _is_redundant_extern(false),
    _init(0),
    _bitsize(0),
    _references(0),
    _ref_list(),
 	_is_return_value(false),
   _attribs()
{
  if (CBZ::OldStyleFunctionDefinition) {
    cout << "declNode::declNode \"" << _name << "\"" << endl;
  }
}

// ------------------------------------------------------------
// ModifyType in complex-types.c
// ------------------------------------------------------------

void declNode::modify_type(typeNode * the_type)
{
  if ( ! type())
    type(the_type);
  else
    type()->set_base_type(the_type);
}

declNode * declNode::modify_type_and(typeNode * the_type)
{
  modify_type(the_type);
  return this;
}

declNode * declNode::modify_type_and(declNode * the_decl)
{
  modify_type(the_decl->type());
  the_decl->type((typeNode *)0);
  //delete the_decl;
  return this;
}

// ------------------------------------------------------------
// FinishDecl from complex-types.c
// ------------------------------------------------------------

// We store the storage class for a declaration in a global variable
// (declNode::CUR_SC) and then retrieve it in finish().

//   WARNING:  FinishDecl may be run more than once on a decl, so it
//   should not blindly make unnecessary changes.

void declNode::finish(Storage_class sc)
{
  typeNode * the_type = type();

  the_type->finish();

  // -- Retrieve the storage class populated during parsing

  storage_class(sc);

  // -- check for incomplete struct/union/enum

  if (the_type && (storage_class() != TYPEDEF))
    the_type->verify_sue_complete();
}

declNode * declNode::finish_and(Storage_class sc)
{
  finish(sc);
  return this;
}

// ------------------------------------------------------------
// SetDeclType in complex-types.c
// Sets the type part of a declaration
// ------------------------------------------------------------

void declNode::set_type(typeNode * the_type, Storage_class sc, ScopeState redeclare)
{
  declNode * orig; // -- For symbol table lookups

  // -- Set the type or base-type..

  modify_type(the_type);

  // djimenez

  storage_class(sc);

  if (the_type->is_derived()) {

    // these three will call SetDeclType again when
    // their base type is set

    return;
  }

  //*******************************************************
  //
  // Hereafter we finish up the decl, cleaning it up, moving
  // storage classes to Decl node, and inserting it in the
  // symbol table.

  // This part is largely duplicated in the id_lookup_walker
  // so, maybe I should chop this down.
  // SZG
  //********************************************************

  finish(sc);

  // -- This is the only symbol table maintenance that is done during
  // parsing. It is required to distinguish identifier names from type
  // names (the only context sensitive aspect of C).

  // pnav
  // commenting out the guard should fix typedef-ident clashes
  if (storage_class() == TYPEDEF)
    orig = CBZ::current_unit->types()->insert(name(), this);

  /*
  if (CBZ::OldStyleFunctionDefinition)
    // this declaration is part of an old-style function definition,
    // so treat it as a formal parameter
    if (redeclare != SU)
      redeclare = Formal;

  if (storage_class() == TYPEDEF)
    var = this;
  else 
    if (type()->typ() == Func && (redeclare != Formal)) {

      // the formal parameter line was added by Manish 2/2/94  this fixes bugs
      //   like :  
      //          int a(int f(int,int))
      //          {}
      //          
      //          int b(int f(double,float)) 
      //          {} 

      // if the arglist contains Id's, then we are in the middle of
      //   an old-style function definition, so don't insert the symbol.
      //   It will be inserted by DefineProc

      funcNode * the_func = (funcNode *) type();
      decl_list & the_args = the_func->args();

      if (! the_args.empty())
	if (the_args.front()->typ() == Id)
	  return;

      // -- normal function declaration, check for consistency with Externals

      var = this;
      orig = CBZ::current_unit->externs()->insert(var->name(), var);
      if (orig) {
	// -- FunctionConflict from procedure.c
	funcNode * ofunc = (funcNode *) orig->type();

	// -- If the old definition is not compatible...
	if (! ofunc->is_compatible_with(the_func)) {
	  // CBZ::SyntaxError(the_func->coord(),
	  //		   string("function identifier `") + name() +
	  //		   string("' redeclared"));
	  // cerr << "\tPrevious declaration: " << orig->coord() << endl;
	}

	var = orig;
      }

    } else
      if ((storage_class() == EXTERN) ||
	  (CBZ::current_unit->symbol_level() == 0  &&  redeclare == Redecl)) {

	// -- top-level variable, check for consistency with Externals
	var = this;
	orig = CBZ::current_unit->externs()->insert(var->name(), var);
	if (orig) {
	  // "external declarations for the same identifier must agree in
	  // type and linkage" --  K&R2, A10.2
	  if (! (* orig->type() == * var->type()) ) {
	    // CBZ::SyntaxError(var->coord(),
	    //	     string("extern `") + orig->name() + 
	    //	     string("' redeclared"));
	    // cerr << "\tPrevious declaration: " << orig->coord() << endl;

	    // orig->type(var->type());
	    // orig->coord(var->coord());
	  }
	  var = orig;
	}

      } else
	var = this;

  // Check if decl is a redundant external declaration.  (See
  // description of T_REDUNDANT_EXTERNAL_DECL in ast.h.)

  if (var != this) {
    // Name was already in Externals symbol table, so possibly redundant.
    // Look for previous declaration in scope

    orig = CBZ::current_unit->ids()->lookup(name());
    if (orig)
      set_redundant_extern(true);
  }

  switch (redeclare) {
  case NoRedecl:
    if (CBZ::current_unit->ids()->is_a_type(name()))
      CBZ::SyntaxError(coord(),
		       string("illegal to redeclare typedef `") + 
		       name() + string("' with default type"));

    // -- falls through to Redecl
  case Redecl:
    if (CBZ::current_unit->symbol_level() == 0)
      decl_location(TOP);
    else
      decl_location(BLOCK);

    // -- add to current scope
    orig = CBZ::current_unit->ids()->insert(name(), var);
    if ((orig) && (orig != var)) {
      // -- The two are equal for redundant function/extern declarations
      // CBZ::SyntaxError(var->coord(),
      //	       string("variable `") + orig->name() +
      //	       string("' redeclared"));
      // cerr << "\tPrevious declaration: " << orig->coord() << endl;
    }

    break;

  case SU:
    decl_location(SU);

    // each struct/union has it own namespace for fields
    break;

  case Formal:
    decl_location(FORMAL);

    if ((storage_class() != NONE) &&
	(storage_class() != REGISTER)) {
      CBZ::SyntaxError(coord(),
		       string("illegal storage class for parameter `") + 
		       name() + string("'"));

      if (storage_class() == TYPEDEF)
	break;

      // reset storage class for body
      //storage_class(NONE);
      // why? djimenez
    }

    // convert Array to pointer
      
    if (type()->typ() == Array) {
      arrayNode * atype = (arrayNode *) get_type();
      type(new ptrNode(atype->type_qualifiers(),
		       atype->type(),
		       atype->coord()));

      // -- Delete the old type...
      //delete atype;
    }

    // formals are not in scope yet; either
    //   1) this is only a function declaration, in which case the
    //      identifiers are only for documentation,  or
    //   2) this is part of a function definition, in which case the
    //      formal are not in scope until the upcoming function body.
    //      In this case, the formals are added by DefineProc just
    //      before the body is parsed.

    break;
  }
  */
}

declNode * declNode::set_type_and(typeNode * the_type, Storage_class sc, ScopeState redeclare)
{
  set_type(the_type, sc, redeclare);
  return this;
}

declNode * declNode::set_type_and(declNode * the_decltype, Storage_class sc, ScopeState redeclare)
{
  set_type(the_decltype->get_type(), sc, redeclare);
  //delete the_decltype;
  return this;
}

// ------------------------------------------------------------
//  AppendDecl from complex-types.c
//  Append decl adds decl to list, giving it the same type and declaration
//  qualifiers as the decls already on list.
// ------------------------------------------------------------

void declNode::inherit_type(decl_list * others, ScopeState redeclare)
{
  if (! others)
    return;

  declNode * first = others->front();
  typeNode * the_type = first->type()->deep_base_type();
  typeNode * copy = (typeNode *) ref_clone_changer::clone(the_type, false);

  set_type(copy, first->storage_class(), redeclare);

  decl_location(first->decl_location());
}

declNode * declNode::inherit_type_and(decl_list * others, ScopeState redeclare)
{
  inherit_type(others, redeclare);
  return this;
}

// ------------------------------------------------------------
//  SetDeclInit from complex-types.c
// ------------------------------------------------------------

void declNode::set_init(exprNode * new_init)
{
  assert(init() == NULL);

  _init = new_init;

  if (new_init) {
    if (is_redundant_extern())
      // fix up misprediction made in SetDeclType.  
      //   decl has an initializer, so it isn't redundant.
      set_redundant_extern(false);
  }
}

declNode * declNode::set_init_and(exprNode * new_init)
{
  set_init(new_init);
  return this;
}

// ------------------------------------------------------------
// Data type base
// ------------------------------------------------------------

typeNode * declNode::base_type(bool TdefIndir) const
{
  return type()->base_type(TdefIndir);
}

// ------------------------------------------------------------
// Other queries
// ------------------------------------------------------------

typeNode * declNode::no_tdef_type()
{
  return type()->follow_tdefs();
}

// ------------------------------------------------------------
// Support for old-style declaration lists
// ------------------------------------------------------------

void declNode::add_parameter_types(decl_list * types)
{
  funcNode * func = (funcNode *) type();
  assert(func->typ() == Func);

  func->add_parameter_types(types);
}

declNode * declNode::add_parameter_types_and(decl_list * types)
{
  add_parameter_types(types);
  return this;
}

// --- Attributes

void declNode::merge_attribs(attrib_list * attribs)
{
  if (attribs) {
    _attribs.splice(_attribs.end(), * attribs);
    //delete attribs;
  }
}

// ------------------------------------------------------------
//  Storage class and decl location
// ------------------------------------------------------------

string declNode::storage_class_name(Storage_class sc)
{
  switch (sc) {
  case AUTO:     return string("auto");
  case EXTERN:   return string("extern");
  case REGISTER: return string("register");
  case STATIC:   return string("static");
  case TYPEDEF:  return string("typedef");
  default:
    return string("");
  }
}

string declNode::decl_location_name(Decl_location dl)
{
  switch (dl) {
  case TOP:    return string("top");
  case BLOCK:  return string("block");
  case FORMAL: return string("formal");
  case SU:     return string("struct/union");
  case ENUM:   return string("enum");
  case PROC:   return string("proc");
  default: return string("unknown");
  }
}

// ------------------------------------------------------------
//  Walker
// ------------------------------------------------------------

void declNode::visit(Visitor * the_visitor) 
{
  the_visitor->at_decl(this);
}

void declNode::walk(Walker & the_walker)
{
  Walker::Order ord = the_walker.order(); 

  if (ord == Walker::Preorder || ord == Walker::Both)
    the_walker.at_decl(this, Walker::Preorder);

  if (the_walker.depth() == Walker::Subtree) {
    // -- Visit the children 

    list_walker(pragmas(), the_walker);

    if (type())
      type()->walk(the_walker);

    if (init())
      init()->walk(the_walker);

    if (bitsize())
      bitsize()->walk(the_walker);

    list_walker(attribs(), the_walker);
  }

  if (ord == Walker::Postorder || ord == Walker::Both)
    the_walker.at_decl(this, Walker::Postorder);
}

// ------------------------------------------------------------
//  Dataflow
// ------------------------------------------------------------

void declNode::dataflow(FlowVal * v, FlowProblem & fp)
{
  if (fp.forward()) {
    fp.flow_decl(v, this, FlowProblem::Entry);

    if (init())
      init()->dataflow(v, fp);

    fp.flow_decl(v, this, FlowProblem::Exit);
  }
  else {
    fp.flow_decl(v, this, FlowProblem::Exit);

    if (init())
      init()->dataflow(v, fp);

    fp.flow_decl(v, this, FlowProblem::Entry);
  }
}

// ------------------------------------------------------------
// Ouput
// ------------------------------------------------------------

void declNode::output(output_context & ct, Node * parent)
{
  // -- First see if there are any pragma things to output. Right now, this
  // is only used to output the #includes.

  for (text_list_p p = pragmas().begin();
       p != pragmas().end();
       ++p)
    (*p)->output(ct, parent);

  // -- Now show the declaration

  Decl_location loc = decl_location();

  if (loc == ENUM)
    ct << name();
  else {

    if (loc != FORMAL)
      ct.new_line();

    ct << storage_class_name(storage_class());
    ct.space();

    type()->output_type(ct, this, Left, typeNode::NONE);

    if (! name().empty()) {
      ct.space();
      ct << name();
    }

    type()->output_type(ct, this, Right, typeNode::NONE);
  }

  if (init()) {
    ct.space();
    ct << '=';
    ct.space();
    init()->output(ct, this);
  }

  if (bitsize()) {
    ct.space();
    ct << ':';
    ct.space();
    bitsize()->output(ct, this);
  }

  if ((loc == TOP) ||
      (loc == BLOCK) ||
      (loc == SU) ||
      (loc == UNKNOWN) )
    ct << ';';
}

// ------------------------------------------------------------
//  Changer
// ------------------------------------------------------------

Node * declNode::change(Changer & the_changer, bool redispatch)
{
  Changer::Order ord = the_changer.order(); 
  declNode * the_decl = this;

  if ((ord == Changer::Preorder || ord == Changer::Both) && ! redispatch)
    the_decl = (declNode *) the_changer.at_decl(the_decl, Changer::Preorder);

  if (the_decl) {

    if (the_decl != this)
      return the_decl->change(the_changer, true);

    change_list(the_decl->pragmas(), the_changer);

    typeNode * old_type = the_decl->type();
    if (old_type) {
      typeNode * new_type = (typeNode *) old_type->change(the_changer);
      if (old_type != new_type) {
	//if (the_changer.delete_old())
	 // delete old_type;
        the_decl->type(new_type);
      }
    }

    exprNode * old_init = the_decl->init();
    if (old_init) {
      exprNode * new_init = (exprNode *) old_init->change(the_changer);
      if (old_init != new_init) {
	//if (the_changer.delete_old())
	 // delete old_init;
        the_decl->init(new_init);
      }
    }

    exprNode * old_bitsize = the_decl->bitsize();
    if (old_bitsize) {
      exprNode * new_bitsize = (exprNode *) old_bitsize->change(the_changer);
      if (old_bitsize != new_bitsize) {
	//if (the_changer.delete_old())
	  //delete old_bitsize;
        the_decl->bitsize(new_bitsize);
      }
    }

    change_list(the_decl->attribs(), the_changer);

  }

  if ((ord == Changer::Postorder || ord == Changer::Both) && ! redispatch)
    the_decl = (declNode *) the_changer.at_decl(the_decl, Changer::Postorder);

  return the_decl;
}


// ------------------------------------------------------------
// Destructor
// ------------------------------------------------------------

declNode::~declNode()
{
  //delete _type;
  //delete _init;
  //delete _bitsize;
  //delete_list(_attribs);
}
