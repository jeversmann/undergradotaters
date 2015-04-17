// $Id: meta.cc,v 1.4 2003/08/07 23:13:33 pnav Exp $
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
#include "meta.h"


list< string > metaexprNode::meta_expr_variables;
list< string > metastmtNode::meta_stmt_variables;

bool metaexprNode::is_meta_expr(char * name)
{
  list<string>::iterator i =  find(meta_expr_variables.begin(), meta_expr_variables.end(), string(name));
  return(i != meta_expr_variables.end());
}

void metaexprNode::add_meta_expr(char * name)
{
  meta_expr_variables.push_back(string(name));
}

void metaexprNode::clear()
{
  meta_expr_variables.clear();
}

void 
metaexprNode::visit(Visitor * the_visitor)
{
  the_visitor->at_expr(this);
}

void 
metaexprNode::walk(Walker & the_walker)
{
  Walker::Order ord = the_walker.order(); 

  if (ord == Walker::Preorder || ord == Walker::Both)
    the_walker.at_expr(this, Walker::Preorder);

  if (the_walker.depth() == Walker::Subtree) {
    // -- Visit the children 
    
    if (type())
      type()->walk(the_walker);
  }
  
  if (ord == Walker::Postorder || ord == Walker::Both)
    the_walker.at_expr(this, Walker::Postorder); 
}
  
Node * 
metaexprNode::change(Changer & the_changer, bool redispatch = false)
{
  Changer::Order ord = the_changer.order(); 
  exprNode * the_en = this;

  if ((ord == Changer::Preorder || ord == Changer::Both) && ! redispatch)
    the_en = (exprNode *) the_changer.at_expr(the_en, Changer::Preorder);
  
  if (the_en) {

    if (the_en != this)
      return the_en->change(the_changer, true);

    typeNode * old_type = the_en->type();
    if (old_type) {
      typeNode * new_type = (typeNode *) old_type->change(the_changer);
      if (old_type != new_type) {
	//if (the_changer.delete_old())
	  //delete old_type;
        the_en->type(new_type);
      }
    }

  }

  if ((ord == Changer::Postorder || ord == Changer::Both) && ! redispatch)
    the_en = (exprNode *) the_changer.at_expr(the_en, Changer::Postorder);

  return the_en;
}

// -- Dataflow
void 
metaexprNode::dataflow(FlowVal * v, FlowProblem & fp)
{
  if (fp.forward()) {
    fp.flow_expr(v, this, FlowProblem::Entry);
    fp.flow_expr(v, this, FlowProblem::Exit);
  }
  else {
    fp.flow_expr(v, this, FlowProblem::Exit);
    fp.flow_expr(v, this, FlowProblem::Entry);
  }
}


// -- Output
void 
metaexprNode::output_expr(output_context & ct, Node * par, int prec, Assoc assoc)
{
  bool p = parens(prec, assoc);
  
  if (p)
    ct << '(';
  
  ct << name();
  
  if (p)
    ct << ')';
}

bool metastmtNode::is_meta_stmt(char * name)
{
  list<string>::iterator i =  find(meta_stmt_variables.begin(), meta_stmt_variables.end(), string(name));
  return(i != meta_stmt_variables.end());
}

void metastmtNode::add_meta_stmt(char * name)
{
  meta_stmt_variables.push_back(string(name));  
}

void metastmtNode::clear()
{
  meta_stmt_variables.clear();
}

// -- Walk and change

void 
metastmtNode::visit(Visitor * the_visitor)
{
  the_visitor->at_stmt(this);
}

void 
metastmtNode::walk(Walker & the_walker)
{
  Walker::Order ord = the_walker.order(); 

  if (ord == Walker::Preorder || ord == Walker::Both)
    the_walker.at_stmt(this, Walker::Preorder);

  if (the_walker.depth() == Walker::Subtree) {
    // -- Visit the children 
  }

  if (ord == Walker::Postorder || ord == Walker::Both)
    the_walker.at_stmt(this, Walker::Postorder);
}

Node * 
metastmtNode::change(Changer & the_changer, bool redispatch)
{
  Changer::Order ord = the_changer.order(); 
  stmtNode * the_stmt = this;

  if ((ord == Changer::Preorder || ord == Changer::Both) && ! redispatch)
    the_stmt = (stmtNode *) the_changer.at_stmt(the_stmt, Changer::Preorder);

  if (the_stmt) {

    if (the_stmt != this)
      return the_stmt->change(the_changer, true);

  }

  if ((ord == Changer::Postorder || ord == Changer::Both) && ! redispatch)
    the_stmt = (labelNode *) the_changer.at_stmt(the_stmt, Changer::Postorder);

  return the_stmt;
}

// -- Dataflow

void 
metastmtNode::dataflow(FlowVal * v, FlowProblem & fp)
{
  if (fp.forward()) {
    fp.flow_stmt(v, this, FlowProblem::Entry);
    fp.flow_stmt(v, this, FlowProblem::Exit);
  }
  else {
    fp.flow_stmt(v, this, FlowProblem::Exit);
    fp.flow_stmt(v, this, FlowProblem::Entry);
  }
}

// -- Output 
void 
metastmtNode::output_stmt(output_context & ct, Node * par)
{
  ct << name() << ':';
  //????
  ct << ';';
}
