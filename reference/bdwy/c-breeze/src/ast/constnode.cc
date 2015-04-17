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

// ------------------------------------------------------------
// Constructor
// ------------------------------------------------------------

constNode::constNode(constant val, const char * text, const Coord coord)
  : exprNode(Const, 0, coord),
    _text(string(text))
{
  value(val);

  if (val.is_str())
    // -- Special case for string..
    type(new arrayNode(typeNode::NONE,
		      new primNode(basic_type::Char),
		      new constNode(constant(strlen(val.Str())+1))));
  else
    if (val.is_ptr())
      // -- Special case for ptr..
      type(new ptrNode(typeNode::NONE, 
		       new primNode(basic_type::Void)));
    else
      // -- Otherwise just take the type from the constant..
      type(new primNode(val.basic()));
}

// ------------------------------------------------------------
// Expression evaluator
// ------------------------------------------------------------

void constNode::eval()
{

}

// ------------------------------------------------------------
//  Walker
// ------------------------------------------------------------

void constNode::visit(Visitor * the_visitor) 
{
  the_visitor->at_const(this);
}

void constNode::walk(Walker & the_walker)
{
  Walker::Order ord = the_walker.order(); 

  if (ord == Walker::Preorder || ord == Walker::Both)
    the_walker.at_const(this, Walker::Preorder);

  if (the_walker.depth() == Walker::Subtree) {
    // -- Visit the children 

    if (type())
      type()->walk(the_walker);
  }

  if (ord == Walker::Postorder || ord == Walker::Both)
    the_walker.at_const(this, Walker::Postorder);
}

// ------------------------------------------------------------
//  Dataflow
// ------------------------------------------------------------

void constNode::dataflow(FlowVal * v, FlowProblem & fp)
{
  if (fp.forward()) {
    fp.flow_const(v, this, FlowProblem::Entry);
    fp.flow_const(v, this, FlowProblem::Exit);
  }
  else {
    fp.flow_const(v, this, FlowProblem::Exit);
    fp.flow_const(v, this, FlowProblem::Entry);
  }
}

// ------------------------------------------------------------
// Output
// ------------------------------------------------------------

void constNode::output_expr(output_context & ct, Node * parent, int prec, Assoc assoc)
{
  bool par = parens(prec, assoc);

  if (par)
    ct << '(';

  // SZG: Always regenerate the textual representation
  //  if (! text().empty())
  //    ct << text();
  //  else

  ct << value().to_string();

  if (par)
    ct << ')';
}

// ------------------------------------------------------------
//  Changer
// ------------------------------------------------------------

Node * constNode::change(Changer & the_changer, bool redispatch)
{
  Changer::Order ord = the_changer.order(); 
  constNode * the_const = this;

  if ((ord == Changer::Preorder || ord == Changer::Both) && ! redispatch)
    the_const = (constNode *) the_changer.at_const(the_const, Changer::Preorder);

  if (the_const) {

    if (the_const != this)
      return the_const->change(the_changer, true);

    typeNode * old_type = the_const->type();
    if (old_type) {
      typeNode * new_type = (typeNode *) old_type->change(the_changer);
      if (old_type != new_type) {
	//if (the_changer.delete_old())
	 // delete old_type;
        the_const->type(new_type);
      }
    }

  }

  if ((ord == Changer::Postorder || ord == Changer::Both) && ! redispatch)
    the_const = (constNode *) the_changer.at_const(the_const, Changer::Postorder);

  return the_const;
}


// ------------------------------------------------------------
// Destructor
// ------------------------------------------------------------

constNode::~constNode()
{
}
