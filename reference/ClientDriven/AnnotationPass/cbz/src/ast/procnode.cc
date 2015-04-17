// $Id: procnode.cc,v 1.13 2003/11/20 19:09:51 toktb Exp $
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

// -- Global pointer to the current procedure being parsed

procNode * procNode::_current = (procNode *)0;

// --------------------------------------------------------------------
// Constructors
// --------------------------------------------------------------------

procNode::procNode(declNode * decl, blockNode * body, const Coord coord)
  : defNode(Proc, coord),
    _decl(decl),
    _body(body),
    _return_label(NULL),
    _return_decl(NULL)
{}

// ------------------------------------------------------------
// -- This constructor corresponds to DefineProc in procedure.c
//     (old version)
// ------------------------------------------------------------

extern char * cbtext;

procNode::procNode(bool old_style, declNode * decl_in)
  : defNode(Proc, decl_in->coord()),
    _decl(decl_in),
    _body(0),
    _return_label(NULL),
    _return_decl(NULL)
{
  assert(decl_in);
  declNode * the_decl = decl_in;

  funcNode * func = (funcNode *) the_decl->type();
  
  assert(func);

  if (func->typ() != Func) {
    CBZ::SyntaxError(the_decl->coord(), 
		     string("expecting a function definition"));
    return;
  }

  // since the parser uses the '{' to determine that
  //   this is a definition, we know that cblex() just returned
  //   the '{' and so has entered the scope for the upcoming body.

  assert(*cbtext == '{'); // verify that last token was '{'
  // This doesn't make sense anymore: assert(CBZ::current_unit->symbol_level() == 0);

  // what the hell was this statement for?  all finish does is set
  // the storage class, so all this does is blow it away; it may
  // have been static, but it just throws it away.  argh!

  //the_decl->finish(declNode::NONE);

  // this preserves the storage class and calls finish; basically a no-op,
  // but maybe finish will do something else in the future?

  the_decl->finish(the_decl->storage_class());
  the_decl->decl_location(declNode::PROC);

  // We know that this is a function definition, rather than merely a
  // declaration.  Convert an empty arglist to (void) and then recheck
  // the definition against the external scope (where the previous
  // declarations will be).

  // -- Maintain the static "current" procedure pointer...

  _current = this;

  if (func->args().empty()) {

    // -- Create "void" arguments

    declNode * void_decl = new declNode(new primNode(basic_type::Void, func->coord()), declNode::NONE);
    void_decl->decl_location(declNode::FORMAL);

    func->args().push_front(void_decl);

    // -- Recheck against the prototype
    /*
    declNode * orig = CBZ::current_unit->externs()->insert(the_decl->name(), the_decl);
    if (orig) {

      // -- FunctionConflict from procedure.c

      funcNode * ofunc = (funcNode *) orig->type();

      // -- If the old definition is not compatible...

      if (! ofunc->is_compatible_with(func)) {
	// CBZ::SyntaxError(coord(),
	//		 string("function identifier `") + 
	//		 the_decl->name() + string("' redeclared"));
	// cerr << "\tPrevious declaration: " << orig->coord() << endl;
      }
    }
    */
  }

  if (old_style) {

    declNode * var = the_decl;

    // -- NOTE: This is done in the parse: identifier.list
    // Convert any formals of type ID to the default DECL

    if (false) {
      decl_list & the_args = func->args();
      decl_list_p p;
      decl_list new_args;

      for (p = the_args.begin(); p != the_args.end(); ++p) {
	if ((*p)->typ() == Id) {
	  idNode * id = (idNode *) (*p);

	  // -- Make the id into a declaration node
	  declNode * d = new declNode(id, declNode::NONE,
					  new primNode(id->coord()),
					  (exprNode *) 0, (exprNode *) 0);
	  d->decl_location(declNode::FORMAL);
	  new_args.push_back(d);
	}
	else
	  new_args.push_back(*p);

      } // -- END for all args

      // -- Replace with new arguments..

      func->args().swap(new_args);
    }

    // -- Insert the declaration for the first time...
    /*
    declNode * orig;
    orig = CBZ::current_unit->externs()->insert(the_decl->name(), var);
    if (! orig)
      orig = CBZ::current_unit->ids()->insert(the_decl->name(), var);

    // -- Check for conflict...

    if (orig) {
      // -- FunctionConflict from procedure.c

      funcNode * ofunc = (funcNode *) orig->type();

      // -- If the old definition is not compatible...
      
      if (! ofunc->is_compatible_with(func)) {
	// CBZ::SyntaxError(coord(),
	//		 string("function identifier `") + 
	//		 the_decl->name() + string("' redeclared"));
	// cerr << "\tPrevious declaration: " << orig->coord() << endl;
      }
    }
    */
  } // -- END if old_style

  // -- Enter scope of function body

  CBZ::current_unit->enter_scope();

  // -- Add formals to the scope of the upcoming block

  decl_list & the_args = func->args();
  decl_list_p p;

  bool first = true;
  for (p = the_args.begin(); p != the_args.end(); ++p) {
    CBZ::current_unit->types()->insert((*p)->name(), *p);

    if ((*p)->datatype()->is_ellipsis())
      ;
    else
      if ((*p)->datatype()->is_void()) {
	if (! first)
	  CBZ::SyntaxError((*p)->coord(),
			   string("void argument must be first"));
      }
      else
	if ((*p)->typ() == Decl) {

	  // -- Normal declaration...
	  /*
	  declNode * a_decl = (declNode *) (*p);
	  declNode * o_decl = CBZ::current_unit->ids()->insert(a_decl->name(), a_decl);

	  // -- Check for conflict...
	  if (o_decl)
	    CBZ::SyntaxError(a_decl->coord(),
			     string("formal `") + 
			     o_decl->name() + string("' used multiple times"));
	  */

	} else
	  CBZ::SyntaxError((*p)->coord(),
			   string("argument without a name"));

    first = false;
  }
}

// ------------------------------------------------------------
// Was SetProcBody in procedure.c
// ------------------------------------------------------------

void procNode::define(blockNode * the_body)
{
  assert(decl());

  CBZ::current_unit->exit_scope();

  body(the_body);

  if (! the_body) {
    CBZ::Warning(4, decl()->coord(),
		 string("procedure `") + decl()->name() + 
		 string("' has no code"));
  } else {
    // -- NOTE: This is handled by goto_label_walker...
    // check for unreferenced/unresolved labels,
    //   all labels are now out of scope
    // Symbols::Labels->reset();
  }

  _current = 0;
}

procNode * procNode::define_and(blockNode * the_body)
{
  define(the_body);
  return this;
}

// ------------------------------------------------------------
// For use in CFG form
// ------------------------------------------------------------

basicblockNode * procNode::entry() const
{
  return (basicblockNode *) _body->stmts().front();
}

basicblockNode * procNode::exit() const
{
  assert(_return_label);
  for(stmt_list_p s=_body->stmts().begin(); s!=_body->stmts().end(); s++) {
    basicblockNode *b = (basicblockNode*) *s;
    if(! b->stmts().empty()) {
      stmtNode *front = b->stmts().front();
      if(front->typ() == Label && front == _return_label)
        return b;
    }
  }
  assert(false);
}

// ------------------------------------------------------------
// Data type base
// ------------------------------------------------------------

typeNode * procNode::base_type(bool TdefIndir) const
{
  return decl()->base_type(TdefIndir);
}

// ------------------------------------------------------------
//  Walker
// ------------------------------------------------------------

void procNode::visit(Visitor * the_visitor) 
{
  the_visitor->at_proc(this);
}

void procNode::walk(Walker & the_walker)
{
  Walker::Order ord = the_walker.order(); 

  if (ord == Walker::Preorder || ord == Walker::Both)
    the_walker.at_proc(this, Walker::Preorder);

  if (the_walker.depth() == Walker::Subtree) {
    // -- Visit the children 

    list_walker(pragmas(), the_walker);

    if (decl())
      decl()->walk(the_walker);

    if (body())
      body()->walk(the_walker);
  }

  if (ord == Walker::Postorder || ord == Walker::Both)
    the_walker.at_proc(this, Walker::Postorder);
}

// ------------------------------------------------------------
//  Dataflow
// ------------------------------------------------------------

void procNode::dataflow(FlowVal * v, FlowProblem & fp)
{
  if (fp.forward()) {
    fp.flow_proc(v, this, FlowProblem::Entry);

    if (body())
      body()->dataflow(v, fp);

    v->meet(at_exit());

    fp.flow_proc(v, this, FlowProblem::Exit);
  }
  else {
    fp.flow_proc(v, this, FlowProblem::Exit);

    v->meet(at_exit());

    if (body())
      body()->dataflow(v, fp);

    fp.flow_proc(v, this, FlowProblem::Entry);
  }
}

// ------------------------------------------------------------
// Output
// ------------------------------------------------------------

void procNode::output(output_context & ct, Node * parent)
{
  // -- First see if there are any pragma things to output.  Right now, this
  // is only used to output the #includes

  if ( !CBZ::LeaveIncluded )
    for (text_list_p p = pragmas().begin();
	 p != pragmas().end();
	 ++p)
      (*p)->output(ct, parent);

  decl()->output(ct, this);

  if (body())
    body()->output(ct, this);
  else
    ct << '{' << '}';
}


// ------------------------------------------------------------
//  Changer
// ------------------------------------------------------------

Node * procNode::change(Changer & the_changer, bool redispatch)
{
  Changer::Order ord = the_changer.order(); 
  procNode * the_proc = this;

  if ((ord == Changer::Preorder || ord == Changer::Both) && ! redispatch)
    the_proc = (procNode *) the_changer.at_proc(the_proc, Changer::Preorder);

  if (the_proc) {

    if (the_proc != this)
      return the_proc->change(the_changer, true);

    change_list(the_proc->pragmas(), the_changer);

    declNode * old_decl = the_proc->decl();
    if (old_decl) {
      declNode * new_decl = (declNode *) old_decl->change(the_changer);
      if (old_decl != new_decl) {
	//if (the_changer.delete_old())
	  //delete old_decl;
        the_proc->decl(new_decl);
      }
    }

    blockNode * old_body = the_proc->body();
    if (old_body) {
      blockNode * new_body = (blockNode *) old_body->change(the_changer);
      if (old_body != new_body) {
	//if (the_changer.delete_old())
	  //delete old_body;
        the_proc->body(new_body);
      }
    }

  }

  if ((ord == Changer::Postorder || ord == Changer::Both) && ! redispatch)
    the_proc = (procNode *) the_changer.at_proc(the_proc, Changer::Postorder);

  return the_proc;
}

// ------------------------------------------------------------
// Destructor
// ------------------------------------------------------------

procNode::~procNode()
{
  //delete _decl;
  //delete _body;
}

