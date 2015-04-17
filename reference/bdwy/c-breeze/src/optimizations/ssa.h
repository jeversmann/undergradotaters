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

#ifndef CBZ_SSA_H
#define CBZ_SSA_H

#include <bitset>
#include "ref_clone_changer.h"
#include "dismantle.h"
#include "dominators.h"
#include "dominancefrontiers.h"
/** @brief data for SSA translation.
 */
class ssa_info : public algorithm_info
{
public:

  int HasAlready;
  int Work;

  ssa_info()
    : HasAlready(0), Work(0)
  {}
};

typedef vector<basicblockNode *> basicblock_vec;

typedef map<declNode *, int_list> varstack_map;

typedef map<declNode *, int> counter_map;
typedef counter_map::iterator counter_map_p;

typedef map<basicblockNode *, basicblock_bitset> basicblockset_map;
typedef basicblockset_map::iterator basicblockset_map_p;

typedef pair< idNode * , int > var_change;
typedef list< var_change > var_changes;
typedef var_changes::iterator var_changes_p;

typedef vector< declNode * > decl_vector;
typedef map< declNode *, decl_vector > decl_decl_map;

/** @brief transform a block into SSA form.
 */
class SSA
{
private:

  /// The procedure to analyze
  const procNode * _proc;

  /// The root node
  basicblockNode * _root;
  basicblock_list _cfg;

  /** @defgroup SSA_H_VAR_RENAME
      @brief Used in variable renaming
  */
  /*@{*/
  varstack_map Stack;
  counter_map Counter;
  var_changes Changes;
  /*@}*/

  ///  Dominance frontiers
  DominanceFrontiers DF;

private:

  /** @defgroup SSA_H_PHI_FUNC
      @brief Place phi functions
  */
  /*@{*/
  void place_phi_functions();
  static void place_one_phi(declNode * d, basicblockNode * block);
  /*@}*/

  /** @defgroup SSA_H_RENAME_VARS
      @brief Rename variables
  */
  /*@{*/
  void rename();
  void search(basicblockNode * X);
  int which_pred(basicblockNode * node, basicblockNode * pred);
  void push_new_decl(declNode * de, int index);
  void rename_all_variables();
  string name_with_index(string & name, int index);
  /*@}*/

  /// Access the node info
  static int & HasAlready(basicblockNode * n)
  {
    return ((ssa_info *)(n)->info())->HasAlready;
  }

  static int & Work(basicblockNode * n)
  {
    return ((ssa_info *)(n)->info())->Work;
  }

public:

  SSA(procNode * proc, bool forward = true, bool rename = true);

  void print() {}

  /** @defgroup SSA_H_UTILS
      @brief Useful utilities
  */
  /*@{*/
  static binaryNode * assignment(stmtNode * s);
  static idNode * lhs(stmtNode * s);
  static bool need_ssa (declNode* decl);
  static callNode * phi(stmtNode * s);  
  void record_index(idNode * id);
  /*@}*/

  /** @ingroup SSA_H_PHI_FUNC
   */
  static bool add_phi_function(declNode * d, basicblockNode * block);
  // basicblock_list * dominance_frontier(basicblockNode * block);
};

#endif // CBZ_SSA_H
