// $Id: linker.cc,v 1.10 2003/08/07 23:13:48 pnav Exp $
// ----------------------------------------------------------------------
//
//  C-Breeze
//  C Compiler Framework
// 
//  Copyright (c) 2003 University of Texas at Austin
// 
//  Samuel Z. Guyer
//  Adam Brown
//  Teck Bok Tok
//  Paul Arthur Navratil
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
#include "linker.h"
#include "ref_clone_changer.h"

bool Linker::debug = false;

Linker::Linker()
  : Walker(Preorder, Subtree),
    _external_symbols(),
    _internal_symbols(),
    _procedure_declarations(),
    _synthetic(),
    current_unit(0)
{}

Linker::~Linker()
{
  clear();
}

void Linker::clear()
{
  // -- Clear the symbol tables

  _external_symbols.clear();
  _internal_symbols.clear();
  _procedure_declarations.clear();

  // -- Put the synthetics back in the tables

  for (decl_name_map_p p = _synthetic.begin();
       p != _synthetic.end();
       ++p)
    {
      declNode * synth = (*p).second;
      _synthetic[synth->name()] = synth;
      _external_symbols[synth->name()] = synth;
    }

  current_unit = 0;
}

void Linker::link()
{

  // -- Make sure the tables are clear

  clear();

  // -- First pass, collect all the external and internal definitions
  // (including both procedure definitions and global variables).

  if (debug)
    cout << "Linker: first pass, collect external and internal symbols" << endl;

  for (unit_list_p up = CBZ::Program.begin();
       up != CBZ::Program.end();
       ++up)
    {
      unitNode * unit = (*up);
      def_list & ds = unit->defs();

      // -- Visit all the top-level and store them according to their
      // linkage...

      for (def_list_p p = ds.begin();
	   p != ds.end();
	   ++p) {
	defNode * d = (*p);
	procNode * proc = 0;
	declNode * decl = 0;

	if (d->typ() == Proc) {
	  proc = (procNode *) d;
	  decl = proc->decl();
	  if (decl->decl_location() == declNode::PROC)
	    _procedure_declarations[decl] = proc;
	}

	if (d->typ() == Decl)
	  decl = (declNode *)d;

	if (decl) {
	  if (decl->storage_class() == declNode::STATIC) {
	    _internal_symbols[unit][decl->name()] = decl;
	    if (debug)
	      cout << "    " << decl->name() << " at " << decl->coord() <<
		" : internal to " << unit->input_file() << endl;
	  }
	  else {

	    // External declarations are either procedure definitions or
	    // top-level variables. We need to make sure to skip over
	    // function declarations (top-level variables of type Func).
	    
	    bool is_procedure_definition = (decl->decl_location() == declNode::PROC);
	    bool is_toplevel_var = (decl->storage_class() == declNode::NONE) &&
	      (decl->decl_location() == declNode::TOP) && (decl->type()->typ() != Func);

	    if (is_procedure_definition || is_toplevel_var) {
	      _external_symbols[decl->name()] = decl;
	      if (debug)
		cout << "    " << decl->name() << " at " << decl->coord() <<
		  " : extern -- declared in " << unit->input_file() << endl;
	    }
	  }
	}
	
      } // END for all top-level definitions

    } // END for all translation units


  // -- Second pass, look for "extern" declarations for which there is no
  // actual definition. Create a synthetic declNode and enter it into the
  // externals map. Note that for external procedures, we create a single
  // declNode, but we don't attempt to create a synthetic procNode. That
  // could confuse some algorithms.

  if (debug)
    cout << "Linker: second pass, find externals with no definition:" << endl;

  for (unit_list_p up = CBZ::Program.begin();
       up != CBZ::Program.end();
       ++up)
    {
      unitNode * unit = (*up);
      def_list & ds = unit->defs();

      // -- Visit all the top-level definitions...

      for (def_list_p p = ds.begin();
	   p != ds.end();
	   ++p) {
	defNode * d = (*p);

	// If this is a declaration...

	if (d->typ() == Decl) {
	  declNode * decl = (declNode *)d;

	  // ... and it's extern...

	  if ((decl->storage_class() == declNode::EXTERN) ||
	      (decl->type()->typ() == Func)) {

	    // ... and there's no entry for it already...

	    bool unused;
	    if ( ! lookup_symbol(unit, decl->name(), unused)) {

	      // ... then create one.
	      
	      create_synthetic(decl);

	      if (debug)
		cout << "    " << decl->name() << " is synthetic." << endl;
	    }
	  }
	}

      } // END for all top-level definitions
      
    } // END for all translation units

  // Finally, walk the entire set of ASTs, fixing the declaration
  // references on the idNodes.
  
  if (debug)
    cout << "Linker: third pass, fix idNode declarations:" << endl;

  for (unit_list_p up = CBZ::Program.begin();
       up != CBZ::Program.end();
       ++up)
    {
      unitNode * unit = (*up);
      current_unit = unit;
      unit->walk(*this);
    }
}

/** @brief Look up a global variable */

declNode * Linker::lookup_symbol(unitNode * current, string name,
				 bool & is_synthetic_decl)
{
  // See if it's a synthetic

  is_synthetic_decl = (_synthetic.find(name) != _synthetic.end());
 
  // First look for a variable with internal linkage

  decl_name_map & current_unit_internals = _internal_symbols[current];
  decl_name_map_p p = current_unit_internals.find(name);
  if (p == current_unit_internals.end()) {

    // Not found, search for a global with external linkage

    p = _external_symbols.find(name);
    if (p == _external_symbols.end()) 
    {
      //cout << "Whoops!  returning null for a declaration" << endl;
      return 0;
    }
  }

  return (*p).second;
}

/** @brief Lookup a procedure definition */

procNode * Linker::lookup_procedure(declNode * decl)
{
  // First look for the procedure symbol

  if (decl) {
    proc_decl_map_p p = _procedure_declarations.find(decl);
    if (p != _procedure_declarations.end())
      return (*p).second;
  }

  return 0;
}

bool Linker::create_synthetic(declNode * local_decl)
{
  decl_name_map_p p = _synthetic.find(local_decl->name());
  if (p != _synthetic.end()) {
    cerr << "Linker error: external variable \"" << local_decl->name() <<
      "\" has duplicate definitions." << endl;
    return false;
  }
  else {
    declNode * decl = (declNode *) ref_clone_changer::clone(local_decl, false);
    _synthetic[local_decl->name()] = decl;
    _external_symbols[decl->name()] = decl;    
    return true;
  }
}

// at_id: At each idNode, if the variable refers to some global symbol,
// then look it up and set the declaration pointer appropriately.

void Linker::at_id(idNode * the_id, Order ord)
{
  declNode * decl = the_id->decl();
  bool lookup = false;

  // Figure out if this identifier refers to a global variable. We'll
  // assume that any variable with no declaration pointer is also a global
  // variable.

  /*lookup is false if it is a decl and a block variable*/
  if (decl) {
    if ((decl->decl_location() == declNode::TOP) || /*global*/
	(decl->decl_location() == declNode::PROC) || /*??*/
	(decl->decl_location() == declNode::UNKNOWN)) /*screwed up*/
      lookup = true;

    if (decl->type() && (decl->type()->typ() == Func))
      lookup = true;
  }
  else
    lookup = true;

  if (lookup) {

    // Look up the symbol, first searching the internal linkage, then the
    // external linkage. Set the idNode's declaration.

    bool is_synthetic_decl;
    declNode * real_decl = lookup_symbol(current_unit, the_id->name(), is_synthetic_decl);
    the_id->decl(real_decl);

    if (debug) {
      cout << "    " << the_id->name() << " at " << the_id->coord();
      if (real_decl)
	cout << " : actual declaration at " << real_decl->coord() << endl;
      else
	cout << " : no declaration found." << endl;
    }
  }
}

/** @brief Special case: function calls
 *
 *  The a function call can refer to an undeclared function. In this
 *  case, we create a declaration on the fly */

void Linker::at_call(callNode * the_call, Order ord)
{
  if (the_call->name()->typ() == Id) {
    idNode * the_id = (idNode *) the_call->name();

    process_call(the_id, the_call);
  }
}


/** @brief Special case: function calls
 *
 *  Does same thing as at_call, but for new dismantler, there is no callNode,
 *  instead there is threeAddrNode. */
void
Linker::at_threeAddr(threeAddrNode * ta, Order ord)
{
   /**Since the code has been dismantled, we have two cases to worry about.
    *1) a call by itself
    *2) a call as the rhs of an assignment statement
    */

   /*Do we have a callNode?*/
   if (ta->op() && ta->op()->id()==Operator::FUNC_CALL)
   { 
      operandNode * o = (operandNode *) ta->rhs1();
      assert(o);
      indexNode * i = o->var();
  
      assert(i->typ()==Id); /*since we know this is a call, can't be a const*/
      idNode * call = (idNode *)i;

      bool is_func_ptr = o->star() || !o->fields().empty() || o->index() ||
                         call->decl()->type()->follow_tdefs()->typ() == Ptr;

      if(! is_func_ptr)
        process_call(call, NULL);
   }
}

/** Used by both at_call and at_threeAddr, process call performs the 
  * steps common to each.  callnode should be NULL from at_threeaddr since
  * this is the way we differentiate.
  */
void
Linker::process_call(idNode * call, callNode * callnode)
{
   bool unused;
   declNode * real_decl = lookup_symbol(current_unit, call->name(), unused);

   if (! real_decl) {

      // Found a function call where the function name is undeclared (old
      // style).

      declNode * decl = call->decl();
      create_synthetic(decl);

      if (debug)
         cout << "    " << decl->name() << " is a synthetic, old-style function." << endl;
   }
   else if(callnode)
   {

      // -- Point to the called procedure

      callnode->proc(lookup_procedure(real_decl));
   }
}

proc_decl_map & Linker::procedure_declarations()
{
   return _procedure_declarations;
}

