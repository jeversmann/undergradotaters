// $Id: constprop.h,v 1.8 2003/08/26 22:02:17 toktb Exp $
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

#ifndef CBZ_CONSTPROP_H
#define CBZ_CONSTPROP_H

#include "udduChains.h"

/** @brief Performs global (intraprocedural) constant propagation.
    @ingroup Phases

    This class performs the constant propagation. The program must already
    be cfg'd. The three public static change() functions allow the
    optimization to be performed on the entire program, on one unit, or on
    a single procedure.
*/

class constantPropChanger : public Changer {
public:
  typedef list<threeAddrNode*> threeAddr_list;

  /** @brief Constant propagation on entire program. */
  static void change();

  /** @brief Constant propagation on one unit. */
  static void change(unitNode *);

  /** @brief Constant propagation on one procedure. */
  static void change(procNode *);

private:
  /** @brief Constructor.
   *
   * The ud chain must be provided. */
  constantPropChanger (udduChains *chain) : 
    Changer (Preorder, Subtree, false),
    _changed (false), _udchain(chain) { }

  /** @brief Reset the _changed flag. */
  inline void reset() { _changed = false; }

  /** @brief The ud chain supplied to and used by this optimizatin. */
  udduChains *_udchain;

  /** @brief Flag to indicate if any code is changed by this optimization. */
  bool _changed;

  exprNode *at_expr (exprNode *);  // not related to Changer's at_expr()
  Node *at_threeAddr (threeAddrNode *, Order);
  Node *at_conditiongoto (conditiongotoNode *, Order);
  Node *at_return (returnNode*, Order);
};


/** @brief Folds constants in binary expressions
    @ingroup Phases

    This class performs the constant folding. The program must already
    be dismantled. The three public static change() functions allow the
    optimization to be performed on the entire program, on one unit, or on
    a single procedure. The ud chain of the relevant part of the program
    must be provided.

    Constant folding is performed at threeAddrNode's and
    conditiongotoNode's. At a threeAddrNode, if there are two
    right-hand-side constants, they are folded into one.
    At a conditiongotoNode, if the two operands are constants, the value
    of the condition is evaluated.
    If the value is non-zero, the conditiongotoNode is replaced with a
    goto to the true branch; otherwise, the conditiongotoNode is
    eliminated.

    As a result of constant folding on conditions, some branches may
    become unreachable code.
    The cfg phase can be used to eliminate the dead code.
*/
class constantFoldingChanger : public Changer {
  friend class constantPropChanger;
public:

  /** @brief Constant propagation on entire program. */
  static void change();

  /** @brief Constant propagation on one unit. */
  static void change(unitNode *);

  /** @brief Constant propagation on one procedure. */
  static void change(procNode *);

private:
  constantFoldingChanger () : 
    Changer (Preorder, Subtree, false),
    _changed (false), _cfg_changed(false), _current_block(NULL) {}

  /** @brief Reset the _changed flag. */
  inline void reset() { _changed = _cfg_changed = false; }

  /** @brief Flag to indicate if any code is changed by this optimization. */
  bool _changed;

  /** @brief Flag to indicate if cfg is changed by this optimization. */
  bool _cfg_changed;

  /** @brief Current basic block. */
  basicblockNode *_current_block;

  Node *at_threeAddr (threeAddrNode *, Order);
  Node *at_conditiongoto (conditiongotoNode *, Order);
  Node *at_basicblock (basicblockNode *, Order);

  /** @brief Collect all unreachable blocks starting from block. */
  void unreachableBlocks (basicblockNode *block,
                          set<basicblockNode*> & unreachables);
};

#endif // CBZ_CONSTPROP_H
