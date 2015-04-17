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

attribNode::attribNode(const char * name, exprNode * arg, const Coord coord)
  : stmtNode(Attrib, coord),
    _arg(arg),
    _name(string(name))
{}

//    Was ConvertIdToAttrib in ast.c

attribNode::attribNode(idNode * id, exprNode * arg)
  : stmtNode(Attrib, id->coord()),
    _arg(arg),
    _name(id->name())
{
  //delete id;
}

// ------------------------------------------------------------
//  Walker
// ------------------------------------------------------------

void attribNode::visit(Visitor * the_visitor) 
{
  the_visitor->at_attrib(this);
}

void attribNode::walk(Walker & the_walker)
{
  Walker::Order ord = the_walker.order(); 

  if (ord == Walker::Preorder || ord == Walker::Both)
    the_walker.at_attrib(this, Walker::Preorder);

  if (the_walker.depth() == Walker::Subtree) {
    // -- Visit the children 

    if (arg())
      arg()->walk(the_walker);
  }

  if (ord == Walker::Postorder || ord == Walker::Both)
    the_walker.at_attrib(this, Walker::Postorder);
}

// ------------------------------------------------------------
// Output
// ------------------------------------------------------------

void attribNode::output_stmt(output_context & ct, Node * parent)
{
}

// ------------------------------------------------------------
//  Changer
// ------------------------------------------------------------

Node * attribNode::change(Changer & the_changer, bool redispatch)
{
  Changer::Order ord = the_changer.order(); 
  attribNode * the_attrib = this;

  if ((ord == Changer::Preorder || ord == Changer::Both) && ! redispatch)
    the_attrib = (attribNode *) the_changer.at_attrib(the_attrib, Changer::Preorder);

  if (the_attrib) {

    if (the_attrib != this)
      return the_attrib->change(the_changer, true);

    exprNode * old_arg = the_attrib->arg();
    if (old_arg) {
      exprNode * new_arg = (exprNode *) old_arg->change(the_changer);
      if (old_arg != new_arg) {
	//if (the_changer.delete_old())
	 // delete old_arg;
        the_attrib->arg(new_arg);
      }
    }

  }

  if ((ord == Changer::Postorder || ord == Changer::Both) && ! redispatch)
    the_attrib = (attribNode *) the_changer.at_attrib(the_attrib, Changer::Postorder);

  return the_attrib;
}


// ------------------------------------------------------------
// Destructor
// ------------------------------------------------------------

attribNode::~attribNode()
{
  //delete _arg;
}
