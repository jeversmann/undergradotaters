// $Id: reaching.h,v 1.8 2003/08/08 17:51:55 toktb Exp $
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

#ifndef CBZ_REACHING_3ADDR_H
#define CBZ_REACHING_3ADDR_H

#include "udduChains.h"


/** @brief performs dataflow analysis for reaching definitions.
    @ingroup Phases

    This class performs dataflow analysis for solving the reaching
    definitions problem for basic blocks.  The program must already be
    dismantled. The end result of reaching definitions analysis is an
    instance of the udduChains class which contains the ud/du chain of the
    program/unit/procedure in question.

    The public functions are the analyze_all() function and the two
    analyze() functions, all of which are static functions.
    They are designed to analyze either the entire program, on a
    procNode, or on a unitNode.  Each of the functions returns an instance
    of the udduChains class.
*/
class reachingDefinitionsWalker : public Walker {
public:
  typedef list<threeAddrNode*> threeAddr_list;

  /** @brief Analyze entire program. */
  static udduChains * analyze_all() {
    reachingDefinitionsWalker r;
    for(unit_list_p u=CBZ::Program.begin(); u!=CBZ::Program.end(); u++)
      (*u)->walk(r);
    return r.udChain;
  }

  /** @brief Analyze a single unit. */
  static udduChains * analyze(unitNode *unit) {
    reachingDefinitionsWalker r;
    unit->walk(r);
    return r.udChain;
  }

  /** @brief Analyze a single procedure. */
  static udduChains * analyze(procNode *proc) {
    reachingDefinitionsWalker r;
    proc->walk(r);
    return r.udChain;
  }

private:
  /** @brief Internal data structure to store dataflow value on reaching
   * definitions. */
  class defFlowVal;

  /** @brief Walker to generate the gen/kill sets of each statement and block
   * in a procedure. */
  class reachingGenKillWalker;

  reachingDefinitionsWalker (void) : Walker (Both, Subtree)
    { udChain = new udduChains(); }

  void at_proc (procNode *, Order);
  void at_basicblock (basicblockNode *, Order);

  /** @brief Make ud-chains for each use in this expression. */
  void make_ud_chains (exprNode *e, exprNode *E=NULL, stmtNode * s = NULL);

  /** @brief Make ud-chains for each use in this statement. */
  void make_ud_chains (stmtNode *);

  /** @brief The `in' and `out' sets for each basic block. */
  map <stmtNode *, defFlowVal *> gen, kill, in, out;

  /** @brief Mapping from bit positions to definitions in the current
   * procedure. */
  threeAddrNode **num2node;

  /** @brief The is the number of definitions in the current procedure. */
  int  n;

  /** @brief The current value. */
  defFlowVal *current_in;

  /** @brief Mapping from definitions (stmts) to bit positions. */
  map <threeAddrNode *, int> node2num;  

  /** @brief Mapping from a statement to the variable it defines. */
  map <threeAddrNode *, declNode *> defines;

  /** @brief Mapping from a variable to the definitions that (may) affect
   * it. */
  map <declNode *, defFlowVal *> defs;

  /** @brief Dummy temporary statements created by analysis. */
  stmt_list dummies;

  /** @brief The resulting udChain. */
  udduChains *udChain;
};

#endif
