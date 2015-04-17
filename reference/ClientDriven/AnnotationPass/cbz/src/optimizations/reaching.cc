// $Id: reaching.cc,v 1.8 2003/08/11 14:21:32 toktb Exp $
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

#include "c_breeze.h"
#include "bits.h"
#include "reaching.h"

/** @brief Internal data structure to store dataflow value on reaching
 * definitions. */
class reachingDefinitionsWalker::defFlowVal {
  friend class reachingGenKillWalker;
private:

  int                        size; // the number of definitions
  map <threeAddrNode*, int> *def2num;  // a map from a definition to its
                                       // index (bit position)
  Bits                      *bits;
  threeAddrNode            **definitions; // array mapping a definition's
                                          // index (bit position) to itself

public:
  /** @brief the constructor.

      Note: this constructor for defFlowVal is called from within the
      reachingGenKillWalker; you will likely not need to call it yourself.
      Use the new_flowval() function of reachingGenKillWalker to get a new
      flow value, set to top, with the right values for the reaching
      definitions problem the walker was called on. */

  defFlowVal (int n, map<threeAddrNode*,int> *m, threeAddrNode **d)
  : size(n), def2num(m), definitions(d), bits(new Bits(n+1)) { to_top (); }

  ~defFlowVal (void) { delete bits; }

  // set this flow value to top (i.e., the empty set)
  void to_top (void) { bits->reset_all(); }

  // get a clone of this flow value
  defFlowVal * clone (void) const {
    defFlowVal *other = new defFlowVal (size, def2num, definitions);
    other->bits = bits->clone();
    return other;
  }

  // returns true iff the two flow values differ
  bool diff (defFlowVal *other) const {
    if(!other) return true;
    for (int i=1; i<=size; i++)
      if (other->bits->value(i) != bits->value(i)) 
        return true;
    return false;
  }

  // return true if the definition is in the set, by a pointer to the definition
  bool in (threeAddrNode *s) const { return bits->value((*def2num)[s]); }

  // return true if the definition is in the set, by the index of the definition
  bool in (int i) const { return bits->value(i); }

  /// insert a definition into the set, by a pointer to the definition
  void insert (threeAddrNode *s) { bits->set ((*def2num)[s], true); }

  /// insert a definition into the set, by the index of the definition
  void insert (int i) { bits->set (i, true); }

  /// remove a definition from the set, by a pointer to the definition
  void remove (threeAddrNode *s) { bits->set ((*def2num)[s], false); }

  /// remove a definition from the set, by the index of the definition
  void remove (int i) { bits->set (i, false); }

  /// make this flow value a copy of the parameter
  void copy (defFlowVal *other) { if(other) bits->copy (other->bits); }

  /// set this definition to the union of itself with the parameter
  void Union (const defFlowVal *v) { if(v) bits->Or (v->bits); }

  /// set this definition to the intersection of itself with the parameter
  void Intersect (const defFlowVal *v) { if(v) bits->And (v->bits); }

  // set this definition to the set difference of itself with the parameter
  // (i.e. this - the other)
  void Difference (defFlowVal *other) {
    if(!other) return;
    Bits comp(size+1);
    comp.copy (other->bits);
    comp.Not ();
    bits->And (&comp);
  }

  /// meet operator (union for reaching definitions)
  void meet (const defFlowVal *v) { Union(v); }
}; // defFlowVal 


//////////////////////////////////////////////////////////////////////////////

/** @brief Walker to generate the gen/kill sets of each statement and
 * block in a procedure. */
class reachingDefinitionsWalker::reachingGenKillWalker : public Walker {
  friend class reachingDefinitionsWalker;

private:
  // reachingGenKillWalker creates the gen/kill sets of all statements and
  // blocks in a procedure. To do that, we need to gather all definitions in
  // the procedure. Use GetDefsWalker to do that.
  class GetDefsWalker : public Walker {
  private:

    // passed in

    map<threeAddrNode*, declNode*> * defines;

    // list of ambiguous definitions, passed in

    stmt_list * ambiguous_defs;

  public:
    GetDefsWalker (map<threeAddrNode*, declNode*> *d, stmt_list *l) : 
      Walker (Preorder, Subtree), ambiguous_defs(l), defines(d) { }

    // for each definition, get what it defines, and check if it is an
    // ambiguous definition.
    void at_threeAddr (threeAddrNode *stmt, Order) {
      bool ambiguous = false;
      if(stmt->lhs()) { // found a definition.
        operandNode *lhs = stmt->lhs();
        if(lhs->star() || ! lhs->fields().empty() || lhs->index())
          ambiguous = true;
        else {
          assert(lhs->var()->typ() == Id);
          (*defines)[stmt] = ((idNode*)lhs->var())->decl();
        }
      }
      if(stmt->op() && stmt->op()->id() == Operator::FUNC_CALL)
        ambiguous = true;
      if(ambiguous)
        ambiguous_defs->push_back (stmt);
    }
  }; // class GetDefsWalker

  /////////////////////////////////////////////////////////////////
  // %{ remaining of class reachingGenKillWalker

  /// map threeAddrNode's to bit positions
  map <threeAddrNode *, int> * node2num;

  /// map threeAddrNode's to the variable (declarations) they define
  map <threeAddrNode *, declNode *> * defines;

  /// map variable declarations to the set of definitions for them
  map <declNode *, defFlowVal *> *defs;

  /// list of ambiguous definitions
  stmt_list ambiguous_defs;

  // mapping from bit positions to definitions in the current procedure.
  threeAddrNode ***num2node;

  /// number of definitions
  int *n; 

  // the instance of reaching definition class that created and used me.
  reachingDefinitionsWalker *_rd;

  /// a generic flow value to clone
  defFlowVal * _flowval;

  /// all ambiguous definitions
  defFlowVal * _everything;

  // dummy statements
  stmt_list & _dummies;

public:

  // constructor
  reachingGenKillWalker (map <threeAddrNode *, int> *n2n,
                         map <threeAddrNode *, declNode *> *dfns,
                         map <declNode *, defFlowVal *> *dfs,
                         threeAddrNode ***num2n,
                         int *numdefs,
                         reachingDefinitionsWalker *rd,
                         stmt_list & dummies) :
      Walker (Preorder, Subtree), 
      node2num(n2n),
      defines(dfns),
      defs(dfs),
      n(numdefs),
      _flowval(NULL),
      num2node(num2n),
      _rd(rd),
      _dummies(dummies) { }

  // destructor
  ~reachingGenKillWalker (void) {}

  /** @brief return a new flow value, set to top.

      this is how you get new empty flow values
      (rather than calling the defFlowValue constructor)
  */
  defFlowVal * new_flowval (void) {
    defFlowVal *f = (defFlowVal *) _flowval->clone();
    f->to_top();
    return f;
  }

  void at_proc (procNode *, Order);

  void at_basicblock (basicblockNode *, Order);

  // we just want to populate gen and kill sets at the procedure level; it
  // makes no sense at the unit level.
  void at_unit (unitNode *, Order) {
    cerr << "Don't invoke the reachingGenKillWalker from a unitNode!\n";
    exit (1);
  }
  // %} end of class reachingGenKillWalker
}; // class reachingGenKillWalker 


void reachingDefinitionsWalker::reachingGenKillWalker
     ::at_proc(procNode *p, Order ord) {
  int  i;

  // get the definitions from this procedure

  defines->clear();
  GetDefsWalker gdw (defines, & ambiguous_defs);
  p->walk (gdw);

  // get a list of all the variables defined

  decl_list vars;
  map <threeAddrNode *, declNode *>::iterator r;
  for (r=defines->begin(); r!=defines->end(); r++)
    vars.push_back ((*r).second);
  vars.sort();
  vars.unique();

  // n will be the length of the bit vector (minus 1). We need one bit for
  // each variable (for arbiguous definitions) and each definition.

  *n = defines->size() + vars.size();

  // num2node will map bit positions to statements.
  // bit positions go from 1..n, since 0 might be returned by the map on
  // an unrecognized statement

  *num2node = new threeAddrNode *[*n + 1];

  // get an empty flow value we can clone later

  _flowval = new defFlowVal (*n, node2num, *num2node);
  _flowval->to_top();

  // map stmtNode's to bit positions
  // and vice versa; also, populate defs[]

  // first, unambiguous definitions

  for (i=1, r=defines->begin(); r!=defines->end(); i++,r++) {

    // the bit position of this definition is i

    (*node2num)[(*r).first] = i;

    // the i'th bit position represents this definition

    (*num2node)[i] = (*r).first;

    // for the variable declaration (*p).second, defs[(*r).second] will
    // contain all its definitions

    defFlowVal *v = (*defs)[(*r).second];

    // no such flow val?  make one.

    if (v == NULL) {
      v = (defFlowVal *) _flowval->clone();
      (*defs)[(*r).second] = v;
    }
    v->insert (i);
  }

  // now, ambiguous definitions; we'll make up one for each variable

  _everything = (defFlowVal *) _flowval->clone();
  decl_list_p q;
  for (q=vars.begin(); q!=vars.end(); q++, i++) {

    // make a new statement that will represent
    // "the" ambiguous definition of this variable

    threeAddrNode *dummy = new threeAddrNode (NULL,(operandNode*)NULL);
    _dummies.push_back(dummy);

    // set up the mapping between bit positions
    // and the stmtNode *

    (*node2num)[dummy] = i;
    (*num2node)[i] = dummy;

    // this ``definition'' defines this variable

    (*defines)[dummy] = *q;

    // get the set of definitions for this variable

    defFlowVal *v = (*defs)[*q];

    // this should have been created already

    assert (v);

    // insert this ambiguous definition

    v->insert (i);

    // also insert it into _everything; at the end,
    // _everything will be a set, for each variable,
    // of "the" ambiguous definition for that variable.

    _everything->insert (i);
  }

  // ready to process procedure body.

  // if the first block has a label, we need a preheader to receive gen
  // for the parameters and globals

  basicblockNode *b = (basicblockNode *) p->body()->stmts().front();
  if (b->stmts().size() && b->stmts().front()->typ() == Label) {
    // new Goto to the first block
    stmt_list new_stmt;
    new_stmt.push_back( new gotoNode((labelNode*) b->stmts().front()) );

    // make an empty preheader for the procedure

    b = new basicblockNode (NULL, & new_stmt);
    b->comment() = "preheader to generate parameters and globals";
  
    // link it to the first block in the procedure
  
    ((basicblockNode *)(p->body()->stmts().front()))->
      preds().push_back (b);
    b->succs().push_back ((basicblockNode *) 
      p->body()->stmts().front());
    p->body()->stmts().push_front (b);
  }

  // make gen and kill sets for each statement (a basicblock),
  // giving them initially empty gen/kill sets

  stmt_list_p t;
  for (t=p->body()->stmts().begin(); t!=p->body()->stmts().end(); t++) {
    b = (basicblockNode *) *t;
    stmt_list_p r;
    for (r=b->stmts().begin(); r!=b->stmts().end(); r++) {
      _rd->gen[*r] = (defFlowVal *) _flowval->clone();
      _rd->kill[*r] = (defFlowVal *) _flowval->clone();
    }
  }

  // for every statement that is an ambiguous definition,
  // it generates everything

  for (t=ambiguous_defs.begin(); t!=ambiguous_defs.end(); t++) {
    defFlowVal *v = _rd->gen[*t];
    assert (v);
    v->Union (_everything);
  }

  // let at_basicblock take care of the rest
} // at_proc


void reachingDefinitionsWalker::reachingGenKillWalker
     ::at_basicblock(basicblockNode *b, Order ord) {
  stmt_list_p r;

  defFlowVal *gen;
  defFlowVal *kill;

  // make gen and kill for each statement in the basicblock

  for (r=b->stmts().begin(); r!=b->stmts().end(); r++) {
    if((*r)->typ() != ThreeAddr) continue;

    // d is the definition (or just a statement)

    threeAddrNode *d = (threeAddrNode*) *r;

    // gen and kill are new flowvals, initially empty

    gen = _rd->gen[d];
    kill = _rd->kill[d];

    // v is the variable defined by definition d.

    declNode *v = (*defines)[d];
    if (v == NULL) {

      // nothing to do; d is not a definition,
      // or may be ambiguous and thus not
      // in defines.

    } else {

      // definition d defines variable v
      // all definitions of v are killed

      kill->Union ((*defs)[v]);

      // except for this one!

      kill->remove (d);

      // this definition gens itself

      gen->insert (d);
    }
  }

  // gen and kill are defined for each statement.
  // put them all together for gen and kill
  // for this block.

  gen = (defFlowVal *) _flowval->clone();
  kill = (defFlowVal *) _flowval->clone();

  // the (pre)header should be seen to generate all ambiguous
  // definitions, so that parameters and global variables are 
  // seen with definitions reaching into the procedure

  if (b->preds().size() == 0) gen->Union (_everything);

  for (r=b->stmts().begin(); r!=b->stmts().end(); r++) {
    stmtNode *d = *r;

    // from Dragon book:
    // gen[S] = gen[S_2] U (gen[S_1] - kill[S_2])
    // kill[S] = kill[S_2] U (kill[S_1] - gen[S_2])
    // we think of the basic block as seen so far
    // as S_1, and the current statement (d) as S_2,
    // with the resulting current basic block so far
    // as S.

    gen->Difference (_rd->kill[d]);
    gen->Union (_rd->gen[d]);
    kill->Difference (_rd->gen[d]);
    kill->Union (_rd->kill[d]);
  }
  _rd->gen[b] = gen;
  _rd->kill[b] = kill;
} // at_basicblock

//////////////////////////////////////////////////////////////////////////////


void reachingDefinitionsWalker::at_proc (procNode *p, Order ord) {
  if (ord == Postorder) {
    // clean up
    delete [] num2node;
    num2node = NULL;
    for(stmt_list_p d=dummies.begin(); d!=dummies.end(); d++) delete *d;
    return;
  }
  node2num.clear();
  defines.clear();
  in.clear();
  out.clear();
  defs.clear();

  // get gen/kill sets for each basic block

  reachingGenKillWalker gcw (&node2num, &defines, &defs, &num2node, &n, this,
                             dummies);
  p->walk (gcw);
  defFlowVal *v = gcw.new_flowval();
  blockNode *b = p->body();

  // for each basic block B, initial value of in and out:
  // out[B] = gen[B],
  // in[B] = empty

  stmt_list_p r;
  for (r=b->stmts().begin(); r!=b->stmts().end(); r++) {
    basicblockNode *B = (basicblockNode *) *r;
    out[B] = (defFlowVal *) gen[B]->clone();
    in[B] = (defFlowVal *) v->clone();
  }

  defFlowVal *oldout = gcw.new_flowval(); // used in loop below.
  bool change = true;
  
  // there is a lot of copying going on in this loop,
  // but we do this because to move pointers around would
  // require lots of changing the map and malloc'ing,
  // which tends to run out of memory on some pathological
  // functions (like perl's eval.c)

  while (change) {
    change = false;

    // for each block B do

    for (r=b->stmts().begin(); r!=b->stmts().end(); r++) {
      basicblockNode *B = (basicblockNode *) *r;
      defFlowVal *inb = in[B];
      defFlowVal *outb = out[B];

      // from Dragon book:
      // in[B] = U_{q in pred(B)} out[Q]

      inb->to_top();
      basicblock_list_p q;
      for (q=B->preds().begin(); q!=B->preds().end(); q++)
        inb->Union (out[*q]);

      oldout->copy (outb); // remember value

      // out[B] = gen[B] U (in[B] - kill[B])

      // v is a copy of in[B]
      v->copy (inb);
      v->Difference (kill[B]);
      v->Union (gen[B]);
      outb->copy (v);
      if (oldout->diff(v)) change = true;
    }
  }
  delete v;
  delete oldout;
  delete gcw._flowval;
  // let at_basicblock() compute the values at statement level.
} // at_proc


void reachingDefinitionsWalker::at_basicblock (basicblockNode *b,
                                                     Order ord) {
  stmt_list_p p;
  if (ord == Postorder) {
    // cleanup
    for (p=b->stmts().begin(); p!=b->stmts().end(); p++) {
      stmtNode *s = *p;
      if (gen[s]) {
        delete gen[s];
        gen[s] = NULL;
      }
      if (kill[s]) {
        delete kill[s];
        kill[s] = NULL;
      }
    }
    if (gen[b]) {
      delete gen[b];
      gen[b] = NULL;
    }
    if (kill[b]) {
      delete kill[b];
      kill[b] = NULL;
    }
    delete in[b];
    delete out[b];
    return;
  }

  current_in = (defFlowVal *) in[b]->clone();
  for (p=b->stmts().begin(); p!=b->stmts().end(); p++) {
    make_ud_chains (*p); // make ud chain using current_in

    // the current reaching definitions
    // are the current ones union
    // the ones generated here minus
    // the ones killed here.

    current_in->Difference (kill[*p]);
    current_in->Union (gen[*p]);
  }
  delete current_in;
} // at_basicblock



void reachingDefinitionsWalker::make_ud_chains (stmtNode *s) {
  switch(s->typ()) {
    case Condition: {
      conditiongotoNode *g = (conditiongotoNode*) s;
      make_ud_chains(g->left(), NULL, s);
      make_ud_chains(g->right(), NULL, s);
      break;
    }

    case ThreeAddr: {
      threeAddrNode *S = (threeAddrNode*) s;
      if(S->lhs() && S->lhs()->index())
        make_ud_chains(S->lhs()->index(), NULL, s);
      make_ud_chains(S->rhs1(), NULL, s);
      make_ud_chains(S->rhs2(), NULL, s);
      for(operand_list_p a=S->arg_list().begin(); a!=S->arg_list().end(); a++)
        make_ud_chains(*a, NULL, s);
      break;
    }

    case Return: make_ud_chains(((returnNode*)s)->expr(), NULL, s);
                 break;

    case Expr: assert(! ((exprstmtNode*)s)->expr()); // no Expr in
               break;                                // dismanted code.

    case Label:
    case Goto:  break; // nothing to do
    default: cout << s->typ() << " " << s->coord() << endl;
             assert(false); // unreachable
  }
} // make_ud_chains(stmt)


void reachingDefinitionsWalker::make_ud_chains (exprNode *e,
                                                      exprNode *E,
                                                      stmtNode *usesite) {
  if (!e) return;
  if (!E) E=e;   // if E was not NULL, it is an Operand and e is the id or
                 // index inside. In that case we want to make ud-chain on
                 // both E and e.
  switch (e->typ()) {
    case Id: {
      idNode *i = (idNode *) e;

      // i_defs is the set of definitions of i

      defFlowVal *i_defs = defs[i->decl()];

      // maybe unknown id, like a function identifier

      if (!i_defs) return;

      bool has_ambiguous = false;
      for (int j=1; j<=n; j++)
        if (i_defs->in (j) && current_in->in (j)) {
          threeAddrNode *t = num2node[j];
          assert(t);
          if (!t->lhs() && !t->rhs1()) // this is a dummy threeAddrNode created
                                       // by reachingGenKillWalker::at_proc();
            has_ambiguous = true;
          else {
            udChain->add(E, num2node[j], usesite);
            if(E!=i) udChain->add(i, num2node[j], usesite);
          }
        }
      if (has_ambiguous) {
        udChain->add(E, NULL, usesite); // The NULL indicates an ambiguous defn
        if(E!=i) udChain->add(E, NULL, usesite);
      }
      break;
    }

    case Operand: {
      operandNode *o = (operandNode*) e;
      if(o->addr()) break; // not an use.
      make_ud_chains(o->var(), o, usesite);
      make_ud_chains(o->index(), NULL, usesite);
      break;
    }

    case Const: break; // nothing to do

    default: assert(false); // unreachable
  }
} // make_ud_chains(expr)

