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

// --------------------------------------------------------------------
// Constructors
// --------------------------------------------------------------------

idNode::idNode(const char * text, const Coord coord)
  : exprNode(Id, 0, coord),
    _name(string(text)),
    _decl(0)
{}

idNode::idNode(declNode * the_decl, const Coord coord)
  : exprNode(Id, 0, coord),
    _name(the_decl->name()),
    _decl(0)
{
  if (the_decl) {
    _name = the_decl->name();
    this->decl(the_decl);
  }
}

// ------------------------------------------------------------
// Data type base
// ------------------------------------------------------------

typeNode * idNode::base_type(bool TdefIndir) const
{
  declNode * d = decl();

  if (d)
    return d->type()->base_type(TdefIndir);
  else
    return (typeNode *)0;
}

void idNode::decl(declNode * the_decl) {
  declNode * old_decl = _decl;
  _decl = the_decl;
  if ( old_decl )
    old_decl->ref_list().remove(this);
  if ( the_decl ) {
    the_decl->ref_list().push_back(this);
    the_decl->ref_list().sort();
    the_decl->ref_list().unique();
  }
}

// ------------------------------------------------------------
// Symbol lookup
// ------------------------------------------------------------

// -- LookupPostfixExpression in type.c
// This function performs the initial lookup of an identifier
// and establishes a pointer to it's declaration.

/*
void idNode::lookup()
{
  declNode * var;
  const string my_name = name();

  var = CBZ::current_unit->ids()->lookup(my_name);

  if (! var) {

    // -- Declaration not found...

    var = 0;
    CBZ::SyntaxError(coord(), string("undeclared variable `") +
		     my_name + string("'"));
  } else {

    // -- Declaration found...

    var->inc_references();

    if (Symbols::TrackIds)
      cerr << "=== `" << my_name << "' ===" << endl;
  }

  // -- Point to the declaration

  decl(var);
}

// -- For the parser

idNode * idNode::lookup_and()
{
  lookup();
  return this;
}
*/

// ------------------------------------------------------------
// Expression evaluator
// ------------------------------------------------------------

void idNode::eval()
{
  // -- Check for enumerated type...

  declNode * ed = decl();
  if (ed && (ed->decl_location() == declNode::ENUM)) {

    // -- It is a constant from an enum...find it's value

    exprNode * e = ed->init();
    if (e) {
      e->eval();
      value(constant(e->value()));
      return;
    }
  }

  // -- Default: no value

  value(constant());
}

// ------------------------------------------------------------
//  Walker
// ------------------------------------------------------------

void idNode::visit(Visitor * the_visitor) 
{
  the_visitor->at_id(this);
}

void idNode::walk(Walker & the_walker)
{
  Walker::Order ord = the_walker.order(); 

  if (ord == Walker::Preorder || ord == Walker::Both)
    the_walker.at_id(this, Walker::Preorder);

  if (the_walker.depth() == Walker::Subtree) {
    // -- Visit the children 

    if (type())
      type()->walk(the_walker);
  }

  if (ord == Walker::Postorder || ord == Walker::Both)
    the_walker.at_id(this, Walker::Postorder);
}

// ------------------------------------------------------------
//  Dataflow
// ------------------------------------------------------------

void idNode::dataflow(FlowVal * v, FlowProblem & fp)
{
  if (fp.forward()) {
    fp.flow_id(v, this, FlowProblem::Entry);
    fp.flow_id(v, this, FlowProblem::Exit);
  }
  else {
    fp.flow_id(v, this, FlowProblem::Exit);
    fp.flow_id(v, this, FlowProblem::Entry);
  }
}

// ------------------------------------------------------------
// Output
// ------------------------------------------------------------

void idNode::output_expr(output_context & ct, Node * parent, int prec, Assoc assoc)
{
  bool par = parens(prec, assoc);

  if (par)
    ct << '(';

  ct << name();

  if (par)
    ct << ')';
}

// ------------------------------------------------------------
//  Changer
// ------------------------------------------------------------

Node * idNode::change(Changer & the_changer, bool redispatch)
{
  Changer::Order ord = the_changer.order(); 
  idNode * the_id = this;

  if ((ord == Changer::Preorder || ord == Changer::Both) && ! redispatch)
    the_id = (idNode *) the_changer.at_id(the_id, Changer::Preorder);

  if (the_id) {

    if (the_id != this)
      return the_id->change(the_changer, true);

    typeNode * old_type = the_id->type();
    if (old_type) {
      typeNode * new_type = (typeNode *) old_type->change(the_changer);
      if (old_type != new_type) {
	//if (the_changer.delete_old())
	  //delete old_type;
        the_id->type(new_type);
      }
    }

  }

  if ((ord == Changer::Postorder || ord == Changer::Both) && ! redispatch)
    the_id = (idNode *) the_changer.at_id(the_id, Changer::Postorder);

  return the_id;
}


// ------------------------------------------------------------
// Destructor
// ------------------------------------------------------------

idNode::~idNode()
{
}
