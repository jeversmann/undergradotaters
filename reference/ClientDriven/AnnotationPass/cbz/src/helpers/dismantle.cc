// $Id: dismantle.cc,v 1.13 2007/05/10 14:30:36 abrown Exp $
// ----------------------------------------------------------------------
//
//  C-Breeze
//  C Compiler Framework
// 
//  Copyright (c) 2000 University of Texas at Austin
// 
//  Adam Brown
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
//
// dismantle.cc
//

#include "c_breeze.h"
#include "dismantle.h"
#include "semcheck.h"
#include "ref_clone_changer.h"

op_id_map DismantleUtil::not_relop;

void DismantleUtil::init() {
  if ( not_relop.empty() ) {
    not_relop['<'] = Operator::GE;
    not_relop[Operator::GE] = '<';
    not_relop['>'] = Operator::LE;
    not_relop[Operator::LE] = '>';
    not_relop[Operator::EQ] = Operator::NE;
    not_relop[Operator::NE] = Operator::EQ;
  }
}

exprNode * DismantleUtil::Not(exprNode * the_expr) {
  if ( the_expr->typ() == Binary ) {
    binaryNode * the_binary = (binaryNode *) the_expr;
    if ( not_relop.find(the_binary->op()->id()) != not_relop.end() ) {
      the_binary->op(Operators::table[not_relop[the_binary->op()->id()]]);
      return the_binary;
    }
  } else if ( the_expr->typ() == Unary ) {
    unaryNode * the_unary = (unaryNode *) the_expr;
    if ( the_unary->op()->id() == '!' )
      return the_unary->expr();
  }
  unaryNode * ret = new unaryNode('!', the_expr, the_expr->coord());
  ret->type(the_expr->type()); // TODO: is this right? does ! apply to floats?
  return ret;
}

declNode * DismantleUtil::new_temp_decl(typeNode * the_type,
						 Coord coord) {
  typeNode * decl_type = NULL;
  if ( the_type->is_pointer() ) {
    decl_type = 
      new ptrNode(typeNode::NONE,
		  (typeNode *) ref_clone_changer::clone(the_type->type(),
							false));
  } else {
    decl_type = (typeNode *) ref_clone_changer::clone(the_type, false);
  }
  decl_type->type_qualifiers(typeNode::NONE); // remove const-ness
  declNode * ret = new declNode(DismantleUtil::new_id(),
				declNode::NONE,
				decl_type,
				NULL,
				NULL,
				coord);
  ret->decl_location(declNode::BLOCK);
  return ret;
}

labelNode * DismantleUtil::new_label(Coord coord) {
  return new labelNode(new_id(), empty_block(), coord);
}

Dismantle::Dismantle(int flags):
  Changer(Preorder, Subtree, false),
  _flags(flags)
{}

void Dismantle::dismantle_control(unitNode * the_unit) {
  Dismantle tad(DISMANTLE_CONTROL);
  the_unit->change(tad);
}

void Dismantle::dismantle(unitNode * the_unit) {
  Dismantle tad(DISMANTLE_CONTROL | DISMANTLE_EXPRS);
  the_unit->change(tad);
}

Node * Dismantle::at_unit(unitNode * the_unit, Order ord) {
  DismantleUtil::init();
  _cur_unit = the_unit;

  // turn all 'static' variables into globals
  StaticToGlobalDismantle stgd;
  the_unit->change(stgd);

  // Fill in the blanks in initializers & dismantle the initialization of
  // scalars
  InitializerDismantle id;
  the_unit->change(id);

  semcheck_walker::check(the_unit, false);

  return the_unit;
}

Node * Dismantle::at_proc(procNode * the_proc, Order ord) {
  // 'dismantle' labels, so they label empty stmts
  LabelDismantle ld;
  the_proc->change(ld);

  // dismantle control flow: loops, &&, ||, ?:
  ControlDismantle cd;
  the_proc->change(cd);

  // opt:  dismantle expressions into threeAddrNodes
  if ( _flags & DISMANTLE_EXPRS ) {
    ExpressionDismantle::init();
    ExpressionDismantle ed;
    the_proc->change(ed);
  }

  // Flatten the resulting code
  FlattenDismantle fd(_cur_unit);
  the_proc->change(fd);

  return the_proc;
}
