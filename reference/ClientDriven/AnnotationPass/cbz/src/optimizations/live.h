// $Id: live.h,v 1.7 2003/08/07 23:14:17 pnav Exp $
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

#ifndef CBZ_LIVE_H
#define CBZ_LIVE_H

///
// live.h
//

#include "c_breeze.h"
#include "bits.h"

typedef set< declNode * > decl_set;
typedef decl_set::iterator decl_set_p;

/** @brief Determines global(intraprocedural) liveness on a procedure.
 *  
 * This Walker performs intraprocedureal liveness analysis.  The
 * current analysis is <b> VERY </b> conservative.  The input to
 * LivenessWalker::walk() must be a CFG.  The analysis is performed at
 * the level of a variable declaration.  Therefore, liveness is
 * generated for a struct or array whenever any element or field is
 * used; liveness is not currently determined on a per-element or
 * per-field basis.
 *
 * The liveness information is available for every stmtNode (namely,
 * threeAddrNode, conditiongotoNode and returnNode) and for every
 * basicblockNode.
 * 
 * Additionally, this Walker does not perform any pointer analysis.
 */
class LivenessWalker: public Walker {
 public:
  LivenessWalker();

  static LivenessWalker * walk(procNode *);

  virtual void at_proc (procNode *, Order);
  virtual void at_basicblock(basicblockNode *, Order);
  virtual void at_threeAddr(threeAddrNode *, Order);
  virtual void at_return(returnNode *, Order);
  virtual void at_conditiongoto(conditiongotoNode *, Order);
  virtual void at_stmt(stmtNode *, Order);

  /** @brief Returns the def set for the statement.
   */
  decl_set * defs(stmtNode *);

  /** @brief Returns the use set for the statment.
   */
  decl_set * uses(stmtNode *);

  /** @brief Returns the set of variables live into the statement.
   */
  decl_set * live_in(stmtNode *);

  /** @brief Returns the set of variables live out of the statement.
   */
  decl_set * live_out(stmtNode *);

 private:
  void defUseOperand(threeAddrNode *, operandNode *, bool, bool);
  decl_set * bits2decls(Bits *);

  int _num_decls;
  map<declNode *, int> _decl2num;
  //  declNode ** _num2decl;
  vector<declNode *> _num2decl;
  map<stmtNode *, Bits *> _live_in;
  map<stmtNode *, Bits *> _live_out;
  map<stmtNode *, Bits *> _def;
  map<stmtNode *, Bits *> _use;
};

/** @brief Adds comments to the AST indicating liveness information.
 */

class LivenessComments : public Walker {
 public:
  LivenessComments(void);
  
  virtual void at_proc(procNode *, Order);
  virtual void at_threeAddr(threeAddrNode *, Order);
  virtual void at_basicblock(basicblockNode *, Order);
  virtual void at_return(returnNode *, Order);
  virtual void at_conditiongoto(conditiongotoNode *, Order);

 private:
  void comment_stmt(stmtNode *);

  LivenessWalker * _lw;
};

#endif // CBZ_LIVE_H
