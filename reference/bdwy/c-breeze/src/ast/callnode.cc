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

callNode::callNode(exprNode * name, expr_list * args, const Coord coord)
  : exprNode(Call, 0, coord),
    _name(name),
    _args(),
    _proc(0),
	_stack_arg_bytes(0)
{
  if (args) {
    _args.swap(* args);
    //delete args;
  }
}

// ------------------------------------------------------------
// Data type base
// ------------------------------------------------------------

typeNode * callNode::base_type(bool TdefIndir) const
{
  typeNode * calltype = name()->base_type(true);

  if (calltype->typ() == Ptr) {
    ptrNode * p = (ptrNode *) calltype;
    calltype = p->type();
    assert(p->typ() == Func);
  }

  return calltype->base_type(TdefIndir);
}

// ------------------------------------------------------------
// Symbol lookup
// ------------------------------------------------------------

/*
void callNode::lookup()
{
  declNode * tmp;

  if (name()->typ() == Id) {
    idNode * id = (idNode *) name();
    declNode * var;
   
    var = CBZ::current_unit->ids()->lookup(id->name());
    if (! var) {
      var = CBZ::current_unit->externs()->lookup(id->name());
      if (! var) {
	CBZ::Warning(2, id->coord(),
		     string("implicitly declaring function to return int: ") +
		     id->name() + string("()."));

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

	CBZ::current_unit->undef_funcs().push_back(var);

	tmp = CBZ::current_unit->ids()->insert(id->name(), var);
	tmp = CBZ::current_unit->externs()->insert(id->name(), var);
      }
      else { // only in Externals

	id->decl(var);

	// already referenced => no REFERENCE(var)
	// -- Ditto over here...
	// tmp = CBZ::current_unit->ids()->insert(id->text(), var);
      }
    }
    else {
      id->decl(var);
      var->inc_references();

      if (Symbols::TrackIds) {
	fprintf(stderr, "=== `%s' = ", id->name().c_str());
	// PrintNode(stderr, var, 0);
      }
    }
  }
  else
    name()->lookup();
}
*/

// ------------------------------------------------------------
// Expression evaluator
// ------------------------------------------------------------

void callNode::eval()
{
  // -- Evaluate all the arguments

  for (expr_list_p p = args().begin();
       p != args().end();
       ++p)
    {
      (*p)->eval();
    }

  // -- Cannot determine the value of a function

  value(constant());
}

// ------------------------------------------------------------
//  Walker
// ------------------------------------------------------------

void callNode::visit(Visitor * the_visitor) 
{
  the_visitor->at_call(this);
}

void callNode::walk(Walker & the_walker)
{
  Walker::Order ord = the_walker.order(); 

  if (ord == Walker::Preorder || ord == Walker::Both)
    the_walker.at_call(this, Walker::Preorder);

  if (the_walker.depth() == Walker::Subtree) {
    // -- Visit the children 

    if (name())
      name()->walk(the_walker);

    list_walker(args(), the_walker);

    if (type())
      type()->walk(the_walker);
  }

  if (ord == Walker::Postorder || ord == Walker::Both)
    the_walker.at_call(this, Walker::Postorder);
}

// ------------------------------------------------------------
//  Dataflow
// ------------------------------------------------------------

void callNode::dataflow(FlowVal * v, FlowProblem & fp)
{
  if (fp.forward()) {
    fp.flow_call(v, this, FlowProblem::Entry);

    if (name())
      name()->dataflow(v, fp);

    dataflow_forward_list(args(), v, fp);

    fp.flow_call(v, this, FlowProblem::Exit);
  }
  else {
    fp.flow_call(v, this, FlowProblem::Exit);

    dataflow_reverse_list(args(), v, fp);

    if (name())
      name()->dataflow(v, fp);

    fp.flow_call(v, this, FlowProblem::Entry);
  }
}

// ------------------------------------------------------------
// Output
// ------------------------------------------------------------

void callNode::output_expr(output_context & ct, Node * parent, int prec, Assoc assoc)
{
  bool par = parens(prec, assoc);
  int myprec;
  Assoc myassoc;

  myprec = precedence(myassoc);

  if (par)
    ct << '(';

  name()->output_expr(ct, this, myprec, Left);

  ct << '(';

  output_delim_list(args(), ct, this, ',');

  ct << ')';

  if (par)
    ct << ')';
}

// ------------------------------------------------------------
//  Changer
// ------------------------------------------------------------

Node * callNode::change(Changer & the_changer, bool redispatch)
{
  Changer::Order ord = the_changer.order(); 
  callNode * the_call = this;

  if ((ord == Changer::Preorder || ord == Changer::Both) && ! redispatch)
    the_call = (callNode *) the_changer.at_call(the_call, Changer::Preorder);

  if (the_call) {

    if (the_call != this)
      return the_call->change(the_changer, true);

    exprNode * old_name = the_call->name();
    if (old_name) {
      exprNode * new_name = (exprNode *) old_name->change(the_changer);
      if (old_name != new_name) {
	//if (the_changer.delete_old())
	  //delete old_name;
        the_call->name(new_name);
      }
    }

    change_list(the_call->args(), the_changer);

    typeNode * old_type = the_call->type();
    if (old_type) {
      typeNode * new_type = (typeNode *) old_type->change(the_changer);
      if (old_type != new_type) {
	//if (the_changer.delete_old())
	  //delete old_type;
        the_call->type(new_type);
      }
    }

  }

  if ((ord == Changer::Postorder || ord == Changer::Both) && ! redispatch)
    the_call = (callNode *) the_changer.at_call(the_call, Changer::Postorder);

  return the_call;
}


// ------------------------------------------------------------
// Destructor
// ------------------------------------------------------------

callNode::~callNode()
{
  //delete _name;
  //delete_list(_args);
}
