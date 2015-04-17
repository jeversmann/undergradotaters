// $Id: copyprop.cc,v 1.3 2003/08/26 22:17:24 toktb Exp $
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

#include "copyprop.h"
#include "bits.h"

// helper class to obtain copies and ambiguous
class copyPropChanger::GetDefsWalker : public Walker {
private:

  threeAddr_set * copies, // copy statements
                * ambiguous_defs; // list of ambiguous definitions, passed in
  map<threeAddrNode*,declNode*> *defines; // what does statement define?

public:
  GetDefsWalker (threeAddr_set *c, threeAddr_set *l,
                 map<threeAddrNode*,declNode*> *d) : 
    Walker (Preorder, Subtree), ambiguous_defs(l), copies(c), defines(d) { }

  void at_threeAddr (threeAddrNode *stmt, Order) {
    bool ambiguous = false;
    if(stmt->lhs()) {
      operandNode *lhs = stmt->lhs();
      if(lhs->star() || ! lhs->fields().empty() || lhs->index())
        // note: possible future work: allow lhs fields.
        ambiguous = true;
      else {
        assert(lhs->var()->typ() == Id);
        (*defines)[stmt] = ((idNode*)lhs->var())->decl();
        if(!stmt->rhs2() &&
           (!stmt->op() || stmt->op()->id() == Operator::UPLUS)) {
          operandNode *rhs = stmt->rhs1();
          if(rhs->var()->typ()==Id && !rhs->star() && rhs->fields().empty() &&
             !rhs->index() && !rhs->cast() && !rhs->addr())
            copies->insert(stmt);
        }
      }
    }
    if(stmt->op() && stmt->op()->id() == Operator::FUNC_CALL)
      ambiguous = true;
    if(ambiguous)
      ambiguous_defs->insert (stmt);
  }
}; // class GetDefsWalker

//////////////////////////////////////////////////////////////////

// flow value
class copyPropChanger::copyFlowVal {
private:

  map<threeAddrNode*, int> def2num; // a map from a definition to its
                                    // index (bit position)
  map<int, threeAddrNode*> num2def; // a map from index to definition.
  int                      size;    // the number of definitions
  Bits                    *bits;

  static threeAddr_set    dummies;

public:
  /** @brief the constructor.
  */
  copyFlowVal (threeAddr_set copy_stmts, threeAddr_set ambiguous_defs) {
    // get all variables.
    decl_set vars;
    for(threeAddr_set_p s=copy_stmts.begin(); s!=copy_stmts.end();s++)
      vars.insert( ((idNode*) (*s)->lhs()->var())->decl() );

    // We need one bit for each variable (for arbiguous definitions) and
    // each copy statement.
    size = copy_stmts.size() + vars.size();
    bits = new Bits(size+1);

    // unambiguous copy statements
    int i=1;
    for(threeAddr_set_p s=copy_stmts.begin(); s!=copy_stmts.end();
        s++, i++) {
      def2num[*s] = i;
      num2def[i] = *s;
    }

    // ambiguous definitions: create one dummy for each var
    for (decl_set_p v=vars.begin(); v!=vars.end(); v++, i++) {
      threeAddrNode *dummy = new threeAddrNode (NULL,(operandNode*)NULL);
      dummies.insert(dummy);
      def2num[dummy] = i;
      num2def[i] = dummy;
    }

    to_top ();
  } // constructor

  ~copyFlowVal (void) { delete bits; }

  /// set this flow value to top (i.e., the empty set)
  void to_top (void) { bits->reset_all(); }

  void to_bottom(void) { bits->set_all(); }

  /// get a clone of this flow value
  copyFlowVal * clone (void) const {
    copyFlowVal *other = new copyFlowVal (*this);
    other->bits = bits->clone();
    return other;
  }

  // get all statements represented by this flow value.
  inline threeAddr_set stmts(void ) {
    threeAddr_set l;
    for (int i=1; i<=size; i++)
      if(in(i)) l.insert(num2def[i]);
    return l;
  }

  /// returns true iff the two flow values differ
  bool diff (copyFlowVal *other) const {
    if(! other) return true;
    for (int i=1; i<=size; i++)
      if (other->bits->value(i) != bits->value(i)) 
        return true;
    return false;
  }

  // return true if the definition is in the set, by a pointer to the definition
  bool in (threeAddrNode *s) { return bits->value(def2num[s]); }

  // return true if the definition is in the set, by the index of the definition
  bool in (int i) const { return bits->value(i); }

  /// insert a definition into the set, by a pointer to the definition
  void insert (threeAddrNode *s) { bits->set (def2num[s], true); }

  /// insert a definition into the set, by the index of the definition
  void insert (int i) { bits->set (i, true); }

  /// remove a definition from the set, by a pointer to the definition
  void remove (threeAddrNode *s) { bits->set (def2num[s], false); }

  /// remove a definition from the set, by the index of the definition
  void remove (int i) { bits->set (i, false); }

  /// make this flow value a copy of the parameter
  void copy (copyFlowVal *other) { if(other) bits->copy (other->bits); }

  /// set this definition to the union of itself with the parameter
  void Union (const copyFlowVal *v) { if(v) bits->Or (v->bits); }

  /// set this definition to the intersection of itself with the parameter
  void Intersect (const copyFlowVal *v) { if(v) bits->And (v->bits); }

  // set this definition to the set difference of itself with the
  // parameter (i.e. this - the other)
  void Difference (copyFlowVal *other) {
    if(!other) return;
    Bits comp(size+1);
    comp.copy (other->bits);
    comp.Not ();
    bits->And (&comp);
  }

  /// meet operator (union for reaching definitions)
  void meet (const copyFlowVal *v) { Union(v); }

  void print() {
    cout << "{";
    for (int i=1; i<=size; i++) if(in(i)) cout << i << ",";
    cout << "}";
  }

  static void clear_dummies() {
    for(threeAddr_set_p t=dummies.begin(); t!=dummies.end(); t++)
      delete *t;
    dummies.clear();
  }
}; // copyFlowVal

copyPropChanger::threeAddr_set copyPropChanger::copyFlowVal::dummies;

//////////////////////////////////////////////////////////////////

Node *copyPropChanger::at_proc(procNode *p, Order ord) {
  if(!p) return p;

  // get the copy statements and ambiguous definitions from this procedure

  GetDefsWalker gdw(&copies, &ambiguous, &defines);
  p->walk(gdw);

  // create flow value
  _top = new copyFlowVal(copies, ambiguous);
  _bottom = _top->clone();
  _bottom->to_bottom();

  // if the first block has a label, we need a preheader to receive gen
  // for the parameters and globals

  basicblockNode *b = (basicblockNode *) p->body()->stmts().front();
  if (b->stmts().size() && b->stmts().front()->typ() == Label) {
    // new Goto to the first block
    stmt_list new_stmt;
    new_stmt.push_back( new gotoNode((labelNode*) b->stmts().front()) );

    b = new basicblockNode (NULL, & new_stmt);
    b->comment() = "preheader to generate parameters and globals";
  
    // link it to the first block in the procedure
  
    ((basicblockNode *)(p->body()->stmts().front()))->
      preds().push_back (b);
    b->succs().push_back ((basicblockNode *) 
      p->body()->stmts().front());
    p->body()->stmts().push_front (b);
  }

  stmt_list stmts = p->body()->stmts();

  // start global phase. It works on all basic blocks.

  for(stmt_list_p s=stmts.begin(); s!=stmts.end(); s++) {
    assert( (*s)->typ() == Block);
    basicblockNode *b = (basicblockNode*) *s;
    Gen[b] = create_copy_set(b);
  }
  for(stmt_list_p s=stmts.begin(); s!=stmts.end(); s++) {
    basicblockNode *b = (basicblockNode*) *s;
    Kill[b] = create_kill_set(b, stmts);
  }

  solve_global_dataflow(stmts);

  // now local phase. It works on statements in each basic block.
  for(stmt_list_p s=stmts.begin(); s!=stmts.end(); s++) {
    basicblockNode *b = (basicblockNode*) *s;
    local_copy_prop(b);
  }

  // clean up
  copies.clear();  ambiguous.clear();  defines.clear();
  set<copyFlowVal*> flowvals;
  for(map<basicblockNode*, copyFlowVal*>::iterator m=Gen.begin();
      m!=Gen.end(); m++) if(m->second) flowvals.insert(m->second);
  for(map<basicblockNode*, copyFlowVal*>::iterator m=Kill.begin();
      m!=Kill.end(); m++) if(m->second) flowvals.insert(m->second);
  for(map<basicblockNode*, copyFlowVal*>::iterator m=In.begin();
      m!=In.end(); m++) if(m->second) flowvals.insert(m->second);
  for(map<basicblockNode*, copyFlowVal*>::iterator m=Out.begin();
      m!=Out.end(); m++) if(m->second) flowvals.insert(m->second);
  if(_top) flowvals.insert(_top);
  if(_bottom) flowvals.insert(_top);
  for(set<copyFlowVal*>::iterator f=flowvals.begin(); f!=flowvals.end(); f++)
    delete *f;
  Gen.clear();  Kill.clear();  In.clear();  Out.clear();
  copyFlowVal::clear_dummies();

  return p;
} // at_proc

//////////////////////////////////////////////////////////////////
// global phase

copyPropChanger::copyFlowVal *
copyPropChanger::create_copy_set(basicblockNode *b) {
  // compute the set of copy statements "a=b" in the block b where nether a nor
  // b is assigned-to later in block.
  if(!b) return _top;

  threeAddr_set copy_stmts;
  for(stmt_list_p s=b->stmts().begin(); s!=b->stmts().end(); s++) {
    if((*s)->typ() != ThreeAddr) continue;
    threeAddrNode *t = (threeAddrNode*) *s;
    if(ambiguous.find(t) != ambiguous.end()) {
      copy_stmts.clear(); // kill all
      continue;
    }
    if(! defines[t]) continue;
    for(threeAddr_set_p c=copy_stmts.begin(); c!=copy_stmts.end(); ) {
      bool kill_c = false; // kill c if t defines lhs or rhs of c.
      if(defines[*c] == defines[t]) kill_c = true;
      else {
        idNode *rhs = (idNode*) (*c)->rhs1()->var();
        if(rhs->decl() == defines[t]) kill_c = true;
      }
      if(kill_c) { threeAddrNode *C = *c;  c++;  copy_stmts.erase(C); }
      else c++;
    }
    if(copies.find(t) != copies.end())
      copy_stmts.insert(t);
  }

  if(copy_stmts.empty()) return _top;
  copyFlowVal *fv= _top->clone();
  for(threeAddr_set_p c=copy_stmts.begin(); c!=copy_stmts.end(); c++)
    fv->insert(*c);
  return fv;
} // create_copy_set


copyPropChanger::copyFlowVal *
copyPropChanger::create_kill_set(basicblockNode *b, stmt_list bbs) {
  // compute the set of copy statements outside of b and get killed by b.
  if(!b) return _top;

  // first, obtain all statements killed by b
  decl_set kill_vars;
  for(stmt_list_p s=b->stmts().begin(); s!=b->stmts().end(); s++) {
    if((*s)->typ() != ThreeAddr) continue;
    threeAddrNode *t = (threeAddrNode*) *s;
    if(ambiguous.find(t) != ambiguous.end())
      return _bottom; // kill all

    if(defines[t])
      kill_vars.insert(defines[t]);
  }
  if(kill_vars.empty()) return _top;  // kill nothing

  // now go through the Gen of all other blocks to obtain the Kill set of b.
  threeAddr_set kill_stmts;
  for(stmt_list_p o=bbs.begin(); o!=bbs.end(); o++) {
    if(*o == b) continue;
    basicblockNode *other = (basicblockNode*) *o;
    if(!Gen[other]) continue;  // other generates nothing
    threeAddr_set defs = Gen[other]->stmts();
    for(threeAddr_set_p d=defs.begin(); d!=defs.end(); d++)
      if(defines[*d] && kill_vars.find(defines[*d]) != kill_vars.end())
        kill_stmts.insert(*d);
  }

  if(kill_stmts.empty()) return _top; // kill nothing
  copyFlowVal *fv= _top->clone();
  for(threeAddr_set_p k=kill_stmts.begin(); k!=kill_stmts.end(); k++)
    fv->insert(*k);
  return fv;
} // create_kill_set


void copyPropChanger::solve_global_dataflow(stmt_list stmts) {
  // initialize
  for(stmt_list_p s=stmts.begin(); s!=stmts.end(); s++) {
    basicblockNode *B = (basicblockNode *) *s;
    In[B] = _top->clone();
    Out[B] = _top->clone();
  }

  stmt_list worklist = stmts; // repeat until fixpoint

  copyFlowVal *oldout = _top->clone();
  while (! worklist.empty()) {
    basicblockNode *B = (basicblockNode *) worklist.front();
    worklist.remove(B);
    copyFlowVal *inb = In[B],
                *outb = Out[B];

    // In[B] = \Intersect_{q \in pred[B]} Out[q]
    if(B->preds().empty()) inb->to_top(); // is B the first block in proc?
    for (basicblock_list_p q=B->preds().begin(); q!=B->preds().end(); q++)
      if(q == B->preds().begin())
        inb->copy(Out[*q]);
      else
        inb->Intersect (Out[*q]);

    oldout->copy (outb); // save a copy
    // Out[B] = Gen[B] \U { In[B] - Kill[B] }
    outb->copy (inb);
    outb->Difference (Kill[B]);
    outb->Union (Gen[B]);
    if (oldout->diff(outb)) // change detected. put B's successors to worklist
      for (basicblock_list_p s=B->succs().begin(); s!=B->succs().end(); s++)
        if(find(worklist.begin(), worklist.end(), *s) == worklist.end())
          worklist.push_back(*s);
  }
  delete oldout;
} // solve_global_dataflow

//////////////////////////////////////////////////////////////////
// local phase

void copyPropChanger::local_copy_prop(basicblockNode *b) {
  if(!b) return;

  ACP acp; // the set of available copy instructions ("statements")
  if(In[b]) { // initialize acp for b to what's in In[b].
    threeAddr_set in = In[b]->stmts();
    for(threeAddr_set_p i=in.begin(); i!=in.end(); i++) {
      operandNode *lhs = (*i)->lhs(), *rhs = (*i)->rhs1();
      assert(lhs && lhs->var()->typ()==Id);
      assert(rhs && rhs->var()->typ()==Id);
      acp.insert(Var_pair(((idNode*)lhs->var())->decl(),
                          ((idNode*)rhs->var())->decl()));
    }
  }

  // now go through statements in b, propagate copies.
  for(stmt_list_p s=b->stmts().begin(); s!=b->stmts().end(); s++) {
    switch((*s)->typ()) {
      case ThreeAddr: {
        threeAddrNode *t = (threeAddrNode*) *s;
        // perform propagation where possible.
        copy_value(t->rhs1(), acp);
        copy_value(t->rhs2(), acp);
        for(operand_list_p a=t->arg_list().begin(); a!=t->arg_list().end(); a++)
          copy_value(*a, acp);

        // update acp
        if(ambiguous.find(t) != ambiguous.end())
          acp.clear();  // clear, no more acp.
        else {
          operandNode *lhs = t->lhs();
          if(lhs && !lhs->star() && lhs->fields().empty() &&
             !lhs->index()) {
            // remove acp on the copy that involves the lhs.
            assert(lhs->var()->typ() == Id);
            remove_ACP(acp, ((idNode*)lhs->var())->decl());
          }
          if(copies.find(t) != copies.end()) { // t itself is also an acp
            operandNode *rhs = t->rhs1();
            assert(rhs && !rhs->star() && rhs->fields().empty() &&
                   !rhs->index() && rhs->var()->typ() == Id);
            acp.insert(Var_pair(((idNode*)lhs->var())->decl(),
                                ((idNode*)rhs->var())->decl()));
          }
        }
        break;
      }

      // for Condition and Return, similar to ThreeAddr except there is no def.
      case Condition: {
        conditiongotoNode *g = (conditiongotoNode*) *s;
        assert(g->left());
        if(g->left()->typ()==Id) {
          idNode *new_id = copy_value((idNode*) g->left(), acp);
          if(new_id != g->left()) g->left(new_id);
        }
        assert(g->right());
        if(g->right()->typ()==Id) {
          idNode *new_id = copy_value((idNode*) g->right(), acp);
          if(new_id != g->right()) g->right(new_id);
        }
        break;
      }

      case Return: {
        // Note: procNode's return_decl() is not updated.
        // The optimization here will interfere with the backend, in different
        // ways depending on if the return_decl() is updated or not.

        // For now, do nothing.
        /* returnNode *r = (returnNode*) *s;
        idNode *new_id = copy_value((idNode*) r->expr(), acp);
        if(new_id != r->expr())
          r->expr(new_id); */
        break;
      }

      case Goto: case Label: break;
      case Expr: assert(! ((exprstmtNode*)*s)->expr());
                 break;
      default: assert(false); // unreachable
    }
  }
} // local_copy_prop

void copyPropChanger::remove_ACP(ACP &acp, declNode *v) {
  if(!v) return;
  for(ACP::iterator m=acp.begin(); m!=acp.end(); )
    if(m->first==v || m->second==v) {
      Var_pair p=*m;  m++;  acp.erase(p);  // remove pair
    } else m++;
} // remove_ACP


void copyPropChanger::copy_value(operandNode *opnd, const ACP &acp) {
  if(!opnd) return;
  // Q: allow fields or index?
  if(opnd->addr() /*|| !opnd->fields().empty() || opnd->index()*/) return;
  if(opnd->var()->typ() != Id) return;
  idNode *new_id = copy_value((idNode*) opnd->var(), acp);
  if(new_id != opnd->var())
    opnd->var(new_id);
  if(opnd->index() && opnd->index()->typ() == Id) {
    new_id = copy_value((idNode*) opnd->index(), acp);
    if(new_id != opnd->index())
      opnd->index(new_id);
  }
} // copy_value(operandNode)


idNode *copyPropChanger::copy_value(idNode *opnd, const ACP &acp) {
  if(!opnd) return opnd;
  declNode *decl = opnd->decl();

  decl_set copy_found;
  for(ACP::const_iterator m=acp.begin(); m!=acp.end(); m++)
    if(m->first==decl)
      copy_found.insert( m->second );

  if(copy_found.empty()) return opnd;

  if(copy_found.size() == 1)
    return new idNode(* copy_found.begin());

  assert(false); // not reachable, unless...big changes; essentially make
                 // duplicate but identical statements share same bit in flow
                 // value.
  for(decl_set_p c=copy_found.begin(); c!=copy_found.end(); c++)
    if(*c != * copy_found.begin())
      return opnd;  // no unit copy found
  // found unique copy
  return new idNode(* copy_found.begin());
} // copy_value (idNode)

//////////////////////////////////////////////////////////////////

void copyPropChanger::change() {
  for (unit_list_p n=CBZ::Program.begin(); n!=CBZ::Program.end(); n++)
    change(*n);
} // change


void copyPropChanger::change(unitNode *u) {
  if(!u) return;
  for(def_list_p d=u->defs().begin(); d!=u->defs().end(); d++)
    if((*d)->typ() == Proc) change((procNode*) *d);
} // change(unitNode)

void copyPropChanger::change(procNode *p) {
  if(!p) return;
  copyPropChanger cpc;
  p->change (cpc);
} // change(procNode)

//////////////////////////////////////////////////////////////////

class copypropphase : public Phase {
  void run() {
    copyPropChanger::change();
  }
};
Phases copyprop_phase("copyprop", new copypropphase());

