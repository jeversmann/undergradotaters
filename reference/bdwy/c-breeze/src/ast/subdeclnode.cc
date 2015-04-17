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

#include "changer.h"
#include "ref_clone_changer.h"

// --------------------------------------------------------------------
// Constructors
// --------------------------------------------------------------------

subdeclNode::subdeclNode(declNode * orig, int index)
  : declNode(*orig),
    _original(orig),
    _index(index)

{
  if (type())
    type((typeNode *) ref_clone_changer::clone(type(), false));

  init(0);
  bitsize(0);
  attrib_list().clear();
}

// ------------------------------------------------------------
// Fields
// ------------------------------------------------------------

string subdeclNode::name_with_index()
{
  ostringstream ost;

  ost << name() << '@' << index();

  return ost.str();
}

// ------------------------------------------------------------
//  Walker
// ------------------------------------------------------------

void subdeclNode::visit(Visitor * the_visitor) 
{
  the_visitor->at_subdecl(this);
}

void subdeclNode::walk(Walker & the_walker)
{
  Walker::Order ord = the_walker.order(); 

  if (ord == Walker::Preorder || ord == Walker::Both)
    the_walker.at_subdecl(this, Walker::Preorder);

  if (the_walker.depth() == Walker::Subtree) {
    // -- Visit the children 

    if (type())
      type()->walk(the_walker);

    if (init())
      init()->walk(the_walker);

    if (bitsize())
      bitsize()->walk(the_walker);

    list_walker(attribs(), the_walker);
  }

  if (ord == Walker::Postorder || ord == Walker::Both)
    the_walker.at_subdecl(this, Walker::Postorder);
}

// ------------------------------------------------------------
// Ouput
// ------------------------------------------------------------

void subdeclNode::output(output_context & ct, Node * parent)
{
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
      ct << name_with_index();
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

Node * subdeclNode::change(Changer & the_changer, bool redispatch)
{
  Changer::Order ord = the_changer.order(); 
  subdeclNode * the_subdecl = this;

  if ((ord == Changer::Preorder || ord == Changer::Both) && ! redispatch)
    the_subdecl = (subdeclNode *) the_changer.at_subdecl(the_subdecl, Changer::Preorder);

  if (the_subdecl) {

    if (the_subdecl != this)
      return the_subdecl->change(the_changer, true);

    typeNode * old_type = the_subdecl->type();
    if (old_type) {
      typeNode * new_type = (typeNode *) old_type->change(the_changer);
      if (old_type != new_type) {
	if (the_changer.delete_old())
	  delete old_type;
        the_subdecl->type(new_type);
      }
    }

    exprNode * old_init = the_subdecl->init();
    if (old_init) {
      exprNode * new_init = (exprNode *) old_init->change(the_changer);
      if (old_init != new_init) {
	if (the_changer.delete_old())
	  delete old_init;
        the_subdecl->init(new_init);
      }
    }

    exprNode * old_bitsize = the_subdecl->bitsize();
    if (old_bitsize) {
      exprNode * new_bitsize = (exprNode *) old_bitsize->change(the_changer);
      if (old_bitsize != new_bitsize) {
	if (the_changer.delete_old())
	  delete old_bitsize;
        the_subdecl->bitsize(new_bitsize);
      }
    }

    change_list(the_subdecl->attribs(), the_changer);

  }

  if ((ord == Changer::Postorder || ord == Changer::Both) && ! redispatch)
    the_subdecl = (subdeclNode *) the_changer.at_subdecl(the_subdecl, Changer::Postorder);

  return the_subdecl;
}

// ------------------------------------------------------------
// Destructor
// ------------------------------------------------------------

subdeclNode::~subdeclNode()
{
}
