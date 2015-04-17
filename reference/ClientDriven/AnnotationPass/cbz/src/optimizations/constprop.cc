// $Id: constprop.cc,v 1.8 2003/08/26 22:12:58 toktb Exp $
// ----------------------------------------------------------------------
//
//  C-Breeze
//  C Compiler Framework
// 
//  Copyright (c) 2000 University of Texas at Austin
// 
//  Teck Bok Tok
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
// global constant propagation using reaching definitions info
//

#include "c_breeze.h"
// #include "semcheck.h"
#include "bits.h"
#include "reaching.h"
#include "constprop.h"
#include "dismantle.h"

/** @brief Performs check on an expression, return the constant that can
 * replace it. */
exprNode *constantPropChanger::at_expr (exprNode *e) {
  // if e is an idNode, if it can be replaced by a constNode;
  // if e is an operandNode that contains an idNode, check that idNode;
  // if e has an id index, check that index too.
  // else return e untouched.

  if(!e) return NULL;
  idNode *id;
  if(e->typ() == Operand) {
    operandNode *o = (operandNode*) e;
    if(o->addr()) return e; // cannot change.
    if(o->index()) { // check on the index.
      indexNode *index = (indexNode*) at_expr(o->index());
      if(index != o->index()) o->index(index); // changed.
      // look for case A[i] where A points to a constant string.
      indexNode *A = (indexNode*) at_expr(o->var());
      if(A != o->var()) {
        assert(A->typ() == Const && ((constNode*)A)->value().is_str());
        o->var(A);
      }
      return e; // cannot change e further
    }
    if(! o->fields().empty()) return e;
    if(o->var()->typ() != Id) return e;
    id = (idNode*) o->var();
  } else if(e->typ() == Id)
    id = (idNode*) e;
  else return e;

  NodeType typ = e->typ();
  constant value;

  // get the ud chain for this node

  threeAddr_list defs = _udchain->defs(e);

  // no definitions?  it could happen.

  if (defs.size() == 0) return e;

  // do all definitions reaching this use define it as the same constant?

  for (threeAddr_list::iterator d=defs.begin(); d!=defs.end(); d++) {

    threeAddrNode *s = *d;

    if (!s) return e; // if it's NULL, it is an ambiguous definition.

    assert (s->lhs());

    // verify the lhs of assignment is same as id.
    operandNode *lhs = s->lhs();
    if(lhs->star() || lhs->index() || ! lhs->fields().empty()) return e;
    if(lhs->var()->typ() != Id) return e;
    idNode *lhs_id = (idNode *) lhs->var();
    if (lhs_id->decl() != id->decl()) return e;

    if(!s->rhs1() || s->rhs2()) return e; // exactly one rhs?
    // note: values of "sizeof(..)" are platform dependent and hence not
    // handled here.
    operandNode *rhs = s->rhs1();
    if(rhs->addr() || rhs->star() || !rhs->fields().empty() ||
       rhs->index()) return e;
    constant rhs_value; // get the rhs value
    if(rhs->var()->typ() == Const) rhs_value = rhs->var()->value();
    else return e;

    if(rhs->cast()) { // need to make sure right value after cast
      if(rhs_value.is_zero() && rhs->cast()->typ() == Ptr) ; // okay
      else {
        assert(rhs->cast()->typ() == Prim);
        rhs_value= constant::cast(((primNode*)rhs->cast())->basic(), rhs_value);
      }
    }

    // any unary operator?
    if(s->op()) {
      int opid = s->op()->id();
      if(opid == Operator::UPLUS) ; // okay;
      else if(opid == Operator::UMINUS)
        rhs_value = constant::eval(s->op(), rhs_value);
      else return e; // future work?
    }

    if ( d==defs.begin() ) { // first value?
      value = rhs_value; // remember it.
    } else if (! value.is_equal_to (rhs_value)) // not same constant.
      return e;
  }
  // at this point, every definition of i is the
  // same constant value; we'll return a constNode
  // with that value.
  _changed = true;
  if(typ == Id) return new constNode (value); // e is an idNode
  return new operandNode(new constNode(value)); // e is an operandNode
} // at_expr


/** @brief Check and replace operands in an threeAddrNode. */
Node *constantPropChanger::at_threeAddr (threeAddrNode *t, Order) {
  if(t->rhs1()) { // check and replace rhs1
    operandNode *v = (operandNode*) at_expr(t->rhs1());
    if(v != t->rhs1()) {
      v->cast( t->rhs1()->cast() ); // keep the same cast
      t->rhs1(v);
    }
  }
  if(t->rhs2()) { // check and replace rhs2
    operandNode *v = (operandNode*) at_expr(t->rhs2());
    if(v != t->rhs2()) {
      v->cast( t->rhs2()->cast() );
      t->rhs2(v);
    }
  }
  if(t->lhs() && t->lhs()->index()) { // check and replace lhs's index
    indexNode *v = (indexNode*) at_expr(t->lhs()->index());
    if(v != t->lhs()->index()) t->lhs()->index(v);
  }
  // check and replace all call arguments
  for(operand_list_p o=t->arg_list().begin(); o!=t->arg_list().end(); o++) {
    operandNode *v = (operandNode*) at_expr(*o);
    if(v != *o) {
      v->cast( (*o)->cast() );
      *o = v;
    }
  }
  return t;
} // at_threeAddr


/** @brief Check and replace operands in an conditiongotoNode. */
Node *constantPropChanger::at_conditiongoto (conditiongotoNode *c, Order) {
  assert(c->left());
  exprNode *v = at_expr(c->left());
  if(v != c->left()) {
    assert(v->typ()==Id || v->typ()==Const);
    c->left((indexNode*) v);
  }

  assert(c->right());
  v = at_expr(c->right());
  if(v != c->right()) {
    assert(v->typ()==Id || v->typ()==Const);
    c->right((indexNode*) v);
  }
  return c;
} // at_conditiongoto

/** @brief Check and replace return expression. */
Node *constantPropChanger::at_return (returnNode *r, Order) {
  // Note: procNode's return_decl() is not updated.
  // The optimization here will interfere with the backend, in different
  // ways depending on if the return_decl() is updated or not.

  // For now, do nothing.
  /* exprNode *v = at_expr(r->expr());
  if(v != r->expr()) r->expr(v); */
  return r;
} // at_return

//////////////////////////////////////////////////////////////////

/** @brief Performs constant folding on an threeAddrNode. */
Node * constantFoldingChanger::at_threeAddr (threeAddrNode *t, Order ord) {
  if(!t->rhs1() || !t->op()) return t;

  constNode *c1 = NULL, *c2 = NULL;

  operandNode *o = t->rhs1();
  if(o->var()->typ() == Const && !o->star() && !o->addr() &&
     o->fields().empty() && !o->index())
    c1 = (constNode*) o->var();

  o = t->rhs2();
  if(o && o->var()->typ() == Const && !o->star() && !o->addr() &&
     o->fields().empty() && !o->index())
    c2 = (constNode*) o->var();

  if(c1 && !t->rhs2()) { // unary operator on constant. handle just three cases:
    switch(t->op()->id()) {
      case Operator::UMINUS:
      case '!':
        // note: '~' operator is platform dependent.
        t->rhs1()->var(new constNode(constant::eval(t->op(), c1->value())));
        // fall through
      case Operator::UPLUS: t->op(NULL);
    }
    return t;
  }

  // binary operator.

  if(!c1 && !c2) return t; // can't do folding.

  // check for acceptable operator.
  switch (t->op()->id()) {
    case '*': case '/': case '%': case '+': case '-':
    case Operator::LS: case Operator::RS: case Operator::LE: case Operator::GE:
    case '<': case '>': case Operator::EQ: case Operator::NE: 
    // exclude bit operations - they're not portable
    //case '&': case '|': case '^': 
      break;
    default: 
      return t;
  }

  // TB new: on top of previous implementation, introduce new optimizations:
  // 1. if operator is '+','-' and one operand is zero;
  // 2. if operator is '*','/' and one operand is one.
  // In these two cases, even if we cannot fold, we may reduce it to just
  // operand.

  // what to do with this?
  // semcheck_expr_visitor::check(b);

  if(c1 && c2) { // both are constants, do normal folding

    constant oper1 = c1->value();
    constant oper2 = c2->value();

#define cast_constant(cast_type, oper) \
  if(cast_type) { \
    assert(cast_type->typ() == Prim); \
    oper = constant::cast(((primNode*)cast_type)->basic(), oper); \
  }
    cast_constant(t->rhs1()->cast(), oper1)
    cast_constant(t->rhs2()->cast(), oper2)

    if (oper1.basic() == oper2.basic()) {
      constant c = constant::eval (t->op(), oper1, oper2);
      if (c.no_val() == false) {
        _changed = true;
        t->rhs1()->var( new constNode (c) );
        if(t->rhs1()->cast() && 
           !(((primNode*)t->rhs1()->cast())->basic() == oper1.basic()))
          t->rhs1()->cast( new primNode(typeNode::NONE, oper1.basic()));
        // delete t->rhs2();
        t->rhs2( NULL );
        t->op(NULL);
      }
    }

  } else if(t->op()->id()=='+' || t->op()->id()=='-') {
    constNode *c = c1 ? c1 : c2; // exactly one of them is a constNode
    if(c->value().is_zero()) {
      if(c==c1)
        t->rhs1(t->rhs2());
      t->rhs2(NULL);
      if(t->op()->id()=='-' && c==c1) // expression is "0-x"
        t->op( Operators::table[Operator::UMINUS] ); // negate
      else t->op( NULL );
    }

  } else if(t->op()->id()=='*' || t->op()->id()=='/') {
    constNode *c = c1 ? c1 : c2; // exactly one of them is constNode
    constant v = c->value();

    // there are obviously many cases we can handle
    // (1,0,-1,float,double,integer,...), and there is the issue of
    // numeric type promotion. Just handle some simple cases below.

    /* if(v.is_zero()) {
      if(t->op()->id()=='/') ; // can't fold
      else {
        t->rhs1()->var(c);
        t->rhs2(NULL);
        t->op(NULL);
      }

    } else*/ if(v.Integer() == 1 /*|| 
       v.basic()==basic_type::Float && v.Float()==1.0 ||
       v.basic()==basic_type::Double && v.Double()==1.0*/) {
      if(t->op()->id()=='/' && c==c1) ; // can't fold
      else { // expression is "1*x" or "x*1" or "x/1"
        if(c==c1)
          t->rhs1(t->rhs2());
        t->rhs2(NULL);
        t->op(NULL);
      }
    }
  }

  return t;
} // at_threeAddr


Node * constantFoldingChanger::at_basicblock(basicblockNode *the_bb,
                                                   Order ord) {
  return _current_block = the_bb;
}


Node * constantFoldingChanger::at_conditiongoto(conditiongotoNode *the_cg,
                                                      Order ord) {
  bool is_constant = false;
  bool which_branch;

  // -- Get the value of the condition

  // every conditiongotoNode should have a right and a op, but let's check
  // to be certain
  assert(the_cg->op());
  assert(the_cg->left());
  assert(the_cg->right());

  //take both the right hand and left hand sides
  indexNode * left = the_cg->left();
  indexNode * right = the_cg->right();

  //If they are both constant, evaluate and replace with appropriate action
  if(left->typ()==Const && right->typ() == Const) {
    constNode * l = (constNode *)left;
    constNode * r = (constNode *)right;
    constant lhs = l->value();
    constant rhs = r->value();

    //evaluate
    constant result = constant::eval(the_cg->op(), lhs, rhs);
    if(! result.no_val()) {
      is_constant = true;
      which_branch = result.Boolean();
    }
  }

  // -- If we can determine the outcome, replace the condition with the
  // appropriate branch.
  if (is_constant) {
    _changed = _cfg_changed = true;
    // choose appropriate branch - for true, want the goto.  for false, want
    // fall-through

#define label_of_block(B) \
   ((B && !(B)->stmts().empty() && (B)->stmts().front()->typ() == Label) ? \
    (labelNode*) (B)->stmts().front() : NULL)

#define erase_succ_pred(pred, succ) { \
  (pred)->succs().remove(succ); \
  (succ)->preds().remove(pred); \
  /* reflect change in comment. too troublesome to rebuild comment the way \
   * cfg does, so just append. */ \
  labelNode *l_succ = label_of_block(succ), \
            *l_pred = label_of_block(pred); \
  (pred)->comment() += " remove # "; \
  if(l_succ) (pred)->comment() += l_succ->name(); \
  (succ)->comment() += " remove # "; \
  if(l_pred) (succ)->comment() += l_pred->name(); \
}

    basicblock_list succs = _current_block->succs();
    set<basicblockNode*> unreachables;
    stmtNode *ret_val = NULL;

    if(which_branch) {
      ret_val = new gotoNode (the_cg->label());
      if(succs.size() > 1) {
        // short-circuit: if succs.size()==1, no edge will be erased.
        for(basicblock_list_p s=succs.begin(); s!=succs.end(); s++) {
          labelNode *label = label_of_block(*s);
          if(label != the_cg->label()) { // erase
            erase_succ_pred (_current_block, *s)
            unreachableBlocks (*s, unreachables);
          }
        }
      }
    } else {
      ret_val = NULL;
      if(succs.size() > 1) {
        // if succs.size()==1, there could be an Goto after this
        // conditiongoto that jumps to same target. In any case, erasing
        // the edge would lead to wrong cfg.
        for(basicblock_list_p s=succs.begin(); s!=succs.end(); s++) {
          labelNode *label = label_of_block(*s);
          if(label == the_cg->label()) { // erase
            erase_succ_pred (_current_block, *s)
            unreachableBlocks (*s, unreachables);
            break;
          }
        }
      }
    }

    for(set<basicblockNode*>::iterator u=unreachables.begin();
        u!=unreachables.end(); u++) {
      (*u)->comment() += " unreachable";
      succs = (*u)->succs();
      for(basicblock_list_p s=succs.begin(); s!=succs.end(); s++)
        erase_succ_pred (*u, *s)
    }

    return ret_val;
  }

  return the_cg;
} // at_conditiongoto


void constantFoldingChanger::unreachableBlocks (basicblockNode *block,
                                          set<basicblockNode*> & unreachables) {
  if(unreachables.find(block) != unreachables.end()) return;

  // check if all its predecessors are unreachable.
  basicblock_list preds = block->preds();
  for(basicblock_list_p p=preds.begin(); p!=preds.end(); p++)
    if(unreachables.find(*p) == unreachables.end())
      return;

  unreachables.insert(block);

  // recurse.
  basicblock_list succs = block->succs();
  for(basicblock_list_p s=succs.begin(); s!=succs.end(); s++)
    unreachableBlocks(*s, unreachables);
} // unreachableBlocks


//////////////////////////////////////////////////////////////////

void constantPropChanger::change() {
  for (unit_list_p n=CBZ::Program.begin(); n!=CBZ::Program.end(); n++)
    change(*n);
} // change


void constantPropChanger::change(unitNode *u) {
  if(!u) return;
  for(def_list_p d=u->defs().begin(); d!=u->defs().end(); d++)
    if((*d)->typ() == Proc) change((procNode*) *d);
} // change(unitNode)

void constantPropChanger::change(procNode *p) {
  if(!p) return;
  udduChains *udchain = reachingDefinitionsWalker::analyze(p);
  constantPropChanger cpc(udchain);
  constantFoldingChanger cfc;

  bool first_time = true;
  do {
    cpc.reset();
    p->change (cpc);
    if(! cpc._changed && !first_time) // ensure folding is performed at least
      break;                          // once
    first_time = false;

    cfc.reset();
    p->change (cfc);
    if(! cfc._changed) break;
    if(cfc._cfg_changed) {
      delete udchain;
      udchain = reachingDefinitionsWalker::analyze(p); // recompute
      cpc._udchain = udchain;
    }
  } while(true); // repeat until fixpoint

  delete udchain;
} // change(procNode)

/////////////////////////////////////////////

void constantFoldingChanger::change() {
  for (unit_list_p n=CBZ::Program.begin(); n!=CBZ::Program.end(); n++)
    change(*n);
} // change

void constantFoldingChanger::change(unitNode *u) {
  if(!u) return;
  for(def_list_p d=u->defs().begin(); d!=u->defs().end(); d++)
    if((*d)->typ() == Proc) change((procNode*) *d);
} // change(unitNode)

void constantFoldingChanger::change(procNode *p) {
  if(!p) return;
  constantFoldingChanger cfc;
  p->change (cfc);
} // change(procNode)

//////////////////////////////////////////////////////////////////

class constpropphase : public Phase {
  void run() {
    constantPropChanger::change();
  }
};
Phases constprop_phase("constprop", new constpropphase());

