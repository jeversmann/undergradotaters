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
#include "ssa.h"

SSA::SSA(procNode * proc, bool forward, bool do_renaming)
  : _proc(proc),
    _root(0),
    _cfg(),
    DF(proc)
{
  // --- Store the CFG as a list of basicblockNodes

  stmt_list & sts = proc->body()->stmts();

  for (stmt_list_p p = sts.begin();
       p != sts.end();
       ++p)
    {
      stmtNode * s = *p;
      assert(s->typ() == Block);
      _cfg.push_back( (basicblockNode *) s);
    }

  _root = _cfg.front();

  // --- Initialize ssa_info structures

  for (basicblock_list_p p = _cfg.begin();
       p != _cfg.end();
       ++p)
    {
      basicblockNode * cur = (*p);
      cur->info(new ssa_info());
    }

  // --- Place phi functions

  place_phi_functions();

  // --- Renaming the variables is optional

  if (do_renaming)
    rename();

  // --- Remove all the ssa_info structures

  for (basicblock_list_p p = _cfg.begin();
       p != _cfg.end();
       ++p)
    {
      basicblockNode * curblock = (*p);
      if (curblock->info())
	delete curblock->info();
      
      curblock->info(0);
    }
}

typedef map<declNode *, basicblock_bitset> assignment_map;
typedef assignment_map::iterator assignment_map_p;

// --- A walker to gather assignment statements

class Assignment_walker : public Walker
{
private:

  int basicblock;
  assignment_map & _a_map;

public:

  Assignment_walker(assignment_map & a_map, int basicblock_num)
    : Walker(Preorder, Subtree),
      basicblock(basicblock_num),
      _a_map(a_map)
  {}

  void at_binary(binaryNode * the_binary, Order ord);
};

void Assignment_walker::at_binary(binaryNode * the_binary, Order ord)
{
  if (the_binary->op()->is_assignment())
    if (the_binary->left()->typ() == Id) {
      idNode * id = (idNode *) the_binary->left();
      declNode * de = id->decl();
      if (SSA::need_ssa (de)) {	
	if (_a_map.find(de) == _a_map.end())
	  _a_map[de] = basicblock_bitset();

	_a_map[de].set(basicblock);
	// cout << "Def " << id->name() << " at block " << basicblock << endl;
      }      
    }
}

// ------------------------------------------------------------
// Place the phi functions
// ------------------------------------------------------------

void SSA::place_phi_functions()
{
  int IterCount = 0;
  assignment_map a_map;
  basicblock_vec df_vec;

  // --- First collect all the variables that are def'ed. Keep them as
  // a list of basic blocks that modify each variable.

  df_vec.resize(_cfg.size(), (basicblockNode *) 0);

  for (basicblock_list_p p = _cfg.begin();
       p != _cfg.end();
       ++p)
    {
      basicblockNode * X = (*p);
      Assignment_walker aw(a_map, X->dfn());
      X->walk(aw);

      df_vec[X->dfn()] = X;
    }

  int num_nodes = df_vec.size();

  // --- Clear the renaming stack

  Stack.clear();
  Changes.clear();

  basicblock_list W;

  // --- Main loop: for each def'ed variable

  for (assignment_map_p p = a_map.begin();
       p != a_map.end();
       ++p)
    {
      declNode * d = (*p).first;
      basicblock_bitset & defs = (*p).second;

      // --- Increment the iteration count

      IterCount++;

      // cout << "At variable " << d->name() << endl;

      // --- Initialize the list of basic blocks to visit

      for (int i = 0; i < num_nodes; i++)
	if (defs.test(i)) {
	  basicblockNode * X = df_vec[i];
	  Work(X) = IterCount;
	  W.push_back(X);
	}

      // --- Also, build the stacks for renaming

      Stack[d] = int_list();
      
      // --- Start the count at 1 for formal parameters and static variables
      // because of the implicit assignment
      
      if (d->decl_location() == declNode::FORMAL
	  || d->storage_class() == declNode::STATIC )
	Counter[d] = 1;
      else Counter[d] = 0;

      // --- Process the work list until empty

      while ( ! W.empty() ) {

	// --- Choose and remove a node X from the work list

	basicblockNode * X = W.front();
	W.pop_front();

	// --- For each Y in the dominance frontier of X

	basicblock_list & DFX = DF[X];
	for (basicblock_list_p dfp = DFX.begin();
	     dfp != DFX.end();
	     ++dfp)
	  {
	    basicblockNode * Y = (* dfp);

	    // --- If it hasn't been processed yet, add the phi function

	    if (HasAlready(Y) < IterCount) {
	      place_one_phi(d, Y);
	      HasAlready(Y) = IterCount;

	      // --- If it hasn't been on the work list yet, add it

	      if (Work(Y) < IterCount) {
		Work(Y) = IterCount;
		W.push_back(Y);
	      }
	    }
	  }
      }
    }
}

void SSA::place_one_phi(declNode * d, basicblockNode * block)
{
  // cout << "Place a phi " << d->name() << " at " << block->dfn() << endl;

  // --- Build the call to "__phi"

  idNode * phi = new idNode("__phi");

  callNode * call = new callNode(phi, 0);

  // --- One input for each control-flow predecessor

  int sz = block->preds().size();
  for (int i = 0; i < sz; i++) {
    idNode * v2 = new idNode(d->name().c_str());
    v2->decl(d);
    typeNode * t = d->no_tdef_type();
    if (t)
      v2->type((typeNode *) ref_clone_changer::clone(t, false));

    call->args().push_back(v2);
  }

  // --- Build the assignment statement

  idNode * v3 = new idNode(d->name().c_str());
  v3->decl(d);
  typeNode * t = d->no_tdef_type();
  if (t)
    v3->type((typeNode *) ref_clone_changer::clone(t, false));

  binaryNode * a = new binaryNode('=', v3, call);

  exprstmtNode * e = new exprstmtNode(a);

  // --- Push it to the front of the basic block, but first skip any
  // labels.

  stmt_list_p p = block->stmts().begin();
  stmtNode * s;

  do {
    s = (*p);
    p++;
  } while ((s->typ() != Label) &&
	   (p != block->stmts().end()));

  block->stmts().insert(p, e);
}

int SSA::which_pred(basicblockNode * node, basicblockNode * pred)
{
  basicblock_list & bbl = node->preds();
  int which = 0;

  for (basicblock_list_p p = bbl.begin();
       p != bbl.end();
       ++p)
    if (pred == (*p))
      return which;
    else
      which++;

  return -1;
}

// ------------------------------------------------------------
// Rename the variables
// ------------------------------------------------------------

void SSA::rename()
{
  // --- Call the recursive name changer

  search(_root);

  rename_all_variables();
}


void SSA::record_index(idNode * the_id)
{
  declNode * de = the_id->decl();
  if (Stack.find(de) != Stack.end())
    if ( ! Stack[de].empty() ) {
      int index = Stack[de].front();
      Changes.push_back(var_change(the_id, index));
    }
}

class renumber_walker : public Walker
{
public:

  static void renum(Node * n, SSA * ssa)
  {
    renumber_walker rnw(ssa);

    n->walk(rnw);
  }

private:

  SSA * ssa;

  renumber_walker(SSA * the_ssa)
    : Walker(Preorder, Subtree),
      ssa(the_ssa)
  {}

  void at_id(idNode * the_id, Order ord)
  {
    // --- Set the subscript to TOP(S(V)) by mapping the normal
    // declaration to the new sub-declaration that has the appropriate
    // index.

    ssa->record_index(the_id);
  }
};

// --- Useful predicates

binaryNode * SSA::assignment(stmtNode * s)
{
  if (s->typ() == Expr) {
    exprNode * e = ((exprstmtNode *) s)->expr();
    if (e && (e->typ() == Binary)) {
      binaryNode * the_binary = (binaryNode *) e;
      if (the_binary->op()->is_assignment())
	return the_binary;
    }
  }

  return 0;
}

idNode * SSA::lhs(stmtNode * s)
{
  binaryNode * b = assignment(s);

  if (b && (b->left()->typ() == Id)) {
    idNode* id = (idNode*) b->left();
    if (need_ssa (id->decl()))
      return (idNode *) b->left();
  }  
  return 0;
}

callNode * SSA::phi(stmtNode * s)
{
  binaryNode * b = assignment(s);

  if (b && (b->right()->typ() == Call)) {
    callNode * c = (callNode *) b->right();
    if (c->name()->typ() == Id) {
      idNode * id = (idNode *) c->name();
      if (id->name() == string("__phi"))
	return c;
    }
  }

  return 0;
}

// --- ssa is applied only on local variables and not on structs and unions

bool SSA::need_ssa (declNode* decl) {
  declNode::Decl_location loc = decl->decl_location();
    
  if ( (loc == declNode::FORMAL || loc == declNode::BLOCK)
       && decl->storage_class() != declNode::TYPEDEF) {
    typeNode* t = decl->no_tdef_type();

    return (t && (t->typ() != Struct) && (t->typ() != Union));    
  }
  return false;  
}
  
// --- Renaming main function

void SSA::search(basicblockNode * X)
{
  // --- For each statement

  for (stmt_list_p p = X->stmts().begin();
       p != X->stmts().end();
       ++p)
    {
      stmtNode * s = *p;
      binaryNode * A = assignment(s);
      callNode * Phi = phi(s);

      // --- If the statement is an ordinary expr statement

      if ((s->typ() == Expr) && ! Phi) {

	if ( A && A->left()->typ() == Id ) {

	  // -- For assignments, renumber only the right-hand side

	  renumber_walker::renum(A->right(), this);
	}
	else {

	  // -- For all others, renumber everything

	  renumber_walker::renum(s, this);
	}
      }

      // --- Handle the conditional and return:

      if (s->typ() == If || s->typ() == Return)
	renumber_walker::renum(s, this);

      // --- Handle the left-hand side

      idNode * LHS = lhs(s);

      if (LHS) {
        declNode * de = LHS->decl();
        int i = Counter[de];
        Stack[de].push_front(i);
	record_index(LHS);
        Counter[de] = i + 1;
      }
    }

  // --- For each successor

  basicblock_list & bbl = X->succs();

  for (basicblock_list_p p = bbl.begin();
       p != bbl.end();
       ++p)
    {
      basicblockNode * Y = *p;
      int j = which_pred(Y, X);
      // cout << "Node " << X->dfn() << " is the " << j << " predecessor of " << Y->dfn() << endl;

      // --- For each Phi node in the sucessor

      for (stmt_list_p p = Y->stmts().begin();
	   p != Y->stmts().end();
	   ++p)
	{
	  callNode * Phi = phi(*p);
	  if (Phi) {

	    // --- Set replace the jth operand with Vk, k = TOP(S(V))
	    
	    int i = 0;
	    for (expr_list_p ap = Phi->args().begin();
		 ap != Phi->args().end();
		 ++ap, ++i)
	      if (i == j) {
		idNode * nid = (idNode *) (*ap);
		declNode * de = nid->decl();

		if ( ! Stack[de].empty()) {
		  record_index(nid);
		  // cout << "Set subscript on " << nid->name() << " = " << Stack[de].front() << endl;
		}
		break;
	      }
	  }
	}
    }

  // --- For each child (in the dominator tree)

  for (basicblock_list_p p = X->children().begin();
       p != X->children().end();
       ++p)
    search(*p);

  // --- For each assignment

  for (stmt_list_p p = X->stmts().begin();
       p != X->stmts().end();
       ++p)
    {
      stmtNode * s = *p;
      idNode * LHS = lhs(s);
      if (LHS) {
	declNode * de = LHS->decl();
	Stack[de].pop_front();
      }
    }
}

string SSA::name_with_index(string & name, int index)
{
  // --- ostringstream is broken.
  // ostringstream ost;
  // ost << name << "@" << index;
  // ost.close();
  // return ost.str();

  char buf[256];
  sprintf(buf, "%s@%d", name.c_str(), index);
  return string(buf);
}

void SSA::rename_all_variables()
{
  decl_decl_map NewDeclarations;

  // --- First create all the sub-declarations

  for (counter_map_p p = Counter.begin();
       p != Counter.end();
       ++p)
    {
      declNode * de = (*p).first;
      int max = (*p).second;

      NewDeclarations[de] = decl_vector(max);

      // SZG: Experiment -- only rename versions 1 and up. Leave the
      // regular variable as version 0. This ought to fix the problem
      // with formal parameters.

      for (int i = 1; i < max; ++i) {

	// --- Create new sub-declaration

	declNode * sde = new subdeclNode( de, i );
	sde->decl_location(declNode::BLOCK);

	// --- Store it in the vector

	NewDeclarations[de][i] = sde;

	// --- Add it to the top-level declarations

	_proc->body()->decls().push_back(sde);
      }
    }

  // --- Fix the variables that need indices (and corresponding new
  // declarations).

  for (var_changes_p p = Changes.begin();
       p != Changes.end();
       ++p)
    {
      idNode * id = (*p).first;
      int index = (*p).second;
      declNode * de = id->decl();

      // --- Replace it's declaration with a properly numbered
      // subdeclaration.

      if (index > 0) {
	subdeclNode * new_decl = (subdeclNode*) NewDeclarations[de][index];

	id->decl(new_decl);
	id->name(new_decl->name_with_index());
      }
    }
}

// ------------------------------------------------------------
// Public access functions
// ------------------------------------------------------------

// Add a phi function to the given block for the given
// variable. Return false if a phi function is already there.

bool SSA::add_phi_function(declNode * d, basicblockNode * block)
{
  // --- Make sure it doesn't already exist

  for (stmt_list_p p = block->stmts().begin();
       p != block->stmts().end();
       ++p)
    {
      callNode * Phi = phi(*p);
      idNode * id = lhs(*p);

      if (Phi && id && (id->decl() == d))
	return false;
    }

  place_one_phi(d, block);

  return true;
}

