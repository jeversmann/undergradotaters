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

#ifndef CBZ_COPYPROP1_H
#define CBZ_COPYPROP1_H

#include "c_breeze.h"

/** @brief Performs global (intraprocedural) copy propagation.
    @ingroup Phases

    This class performs the constant propagation. The program must already
    be cfg'd. The three public static change() functions allow the
    optimization to be performed on the entire program, on one unit, or on
    a single procedure. 
    The algorithm used is adopted from Muchnick chapter 12.5.
    Note that copy statements with casts are ignored.
    Also, the algorithm itself has limitation. For example.
    @code
      if(..)
        x = y;
      else
        x = y;
      z = x;   // <-- x not replaced by y.
    @endcode
    To handle this case is left as future work.

*/

class copyPropChanger : public Changer {
public:
  typedef set<threeAddrNode*>     threeAddr_set;
  typedef threeAddr_set::iterator threeAddr_set_p;
  typedef set<declNode*>          decl_set;
  typedef decl_set::iterator      decl_set_p;

  /** @brief Copy propagation on entire program. */
  static void change();

  /** @brief Copy propagation on one unit. */
  static void change(unitNode *);

  /** @brief Copy propagation on one procedure. */
  static void change(procNode *);

private:

  /** @brief Constructor. */
  copyPropChanger() : Changer(Preorder, Subtree, false) {}

  Node *at_proc(procNode *, Order ord);

  threeAddr_set                 copies,        // copy statements
                                ambiguous;     // statements with ambiguous
                                               // statements
  map<threeAddrNode*,declNode*> defines;       // what does statement define?
  class                         GetDefsWalker; // helper class to obtain
                                               // copies and ambiguous

  /* The algorithm is divided into two phase: a global phase and a local
   * phase. */

  //{ global phase

  // flow value
  class copyFlowVal;
  copyFlowVal *_top, *_bottom;

  map<basicblockNode*, copyFlowVal*> Gen, Kill, In, Out;

  copyFlowVal * create_copy_set(basicblockNode *);
  copyFlowVal * create_kill_set(basicblockNode *, stmt_list);
  void solve_global_dataflow(stmt_list);

  //}

  //{ local phase

  // internal data structure
  typedef pair<declNode*,declNode*> Var_pair; // a copy pair.
  typedef set<Var_pair>             ACP;      // available copy instuctions

  void local_copy_prop(basicblockNode *);

  static void remove_ACP(ACP &acp, declNode *v);
  void copy_value(operandNode *opnd, const ACP &acp);
  idNode *copy_value(idNode *opnd, const ACP &acp);
  //}
};

#endif // CBZ_COPYPROP_H
