// $Id: dismantle.h,v 1.24 2007/05/10 14:30:36 abrown Exp $
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

#ifndef CBZ_DISMANTLE_H
#define CBZ_DISMANTLE_H

#include <map>
#include <set>
#include <stack>

typedef map<unsigned int, unsigned int> op_id_map;
typedef set<unsigned int> op_id_set;

class DismantleUtil {
 public:
  static void init(void);
  static stmtNode * empty_stmt(void) {
    return new exprstmtNode(NULL);
  }
  static blockNode * empty_block(void) {
    blockNode * ret = new blockNode(NULL, NULL);
    ret->stmts().push_back(empty_stmt());
    return ret;
  }
  // TODO: implement REAL unique id generator
  static char* new_id(const char *s = "") {
    char * buf = new char[80];
    snprintf(buf, 80, "__%s%s", CBZ::get_temp_id_str().c_str(), s);
    return buf;
  }
  static op_id_map not_relop;
  static exprNode * Not(exprNode *);
  static declNode * new_temp_decl(typeNode *, Coord);
  static labelNode * new_label(Coord);
};

/**
 * @brief main dismantler method
 * @ingroup Dismantlers
 *
 * The dismantler provides two distinct levels of dismantled output.
 * At all levels of dismantling, some common transformations are
 * applied to the input AST.  First, static block variables are made
 * into top-level (global) variables.  Second, initialization of
 * automatic variables is removed from the declaration and made into
 * an assignment statement at the beginning of the relevant block.
 * These changes to initializers only apply to scalar values (ints,
 * floats & pointers); however the implicit zero values in an
 * aggregate types initial value are explicitly added.  Third, all
 * labelNodes are modified so that the stmt field contains a blockNode
 * containing only an empty statement (exprstmtNode with a NULL
 * expression).  Last, after dismantling has occurred, the resulting
 * AST is flattened to remove unnecessary nesting of blocks, and all
 * variable declarations are moved up to the procNode level.
 * 
 * The first level of dismantling transforms complex control flow
 * constructs.  During this phase, all loops (whileNode, doNode,
 * forNode), if statements (ifNode), and switch statements
 * (switchNode) are replaced with equivalent
 * conditiongotoNodes, gotoNodes, and
 * labelNodes.  Additionally, the ternary operator
 * (ternaryNode) and short-circuit conditional operators
 * (&&, ||) are transformed into equivalent ifNodes,
 * which are subsequently transformed as mentioned above.
 * 
 * The final dismantling level transforms any complex expression
 * into a simple expression or a series of simple expressions with
 * intermediate results.  A complex expression is any expression that
 * does not conform to the guidelines of Section
 * prin:close-to-hardware.  An expression may be complex for
 * several reasons.  Obviously, if the expression contains more than one
 * operation (binary or unary), it is complex.  For assignments, one of
 * the source operands may not be addressable by a single load
 * instruction; conversely, the destination operand may not be
 * addressable by a single store instruction.
 */
class Dismantle : public Changer {
 private:
  int _flags;
  unitNode * _cur_unit;

  Dismantle(int flags);
 public:
  static const int DISMANTLE_CONTROL    = 1 << 0;
  static const int DISMANTLE_EXPRS      = 1 << 1;

  static void dismantle_control(unitNode *);
  static void dismantle(unitNode *);
  virtual Node * at_unit(unitNode *, Order);
  virtual Node * at_proc(procNode *, Order);
};

class StaticToGlobalDismantle : public Changer {
 public:
  StaticToGlobalDismantle(void);
  virtual Node * at_unit(unitNode *, Order);
  virtual Node * at_proc(procNode *, Order);
  virtual Node * at_block(blockNode *, Order);
  virtual Node * at_decl(declNode *, Order);

 private:
  unitNode * _cur_unit;
  procNode * _cur_proc;
  block_list _blockStack;
  map<declNode *, decl_list> _declsToMove;

  // indicates whether one of the statics refers to the containing procedure
  // declaration, thus requiring procedure to be declared early
  bool _declareProc;
  set < suespecNode * > _topLevelSuespecs;
  map < typeNode *, string > _newTypedefName;

  void rename_decl(declNode *, procNode *);
};

class LabelDismantle : public Changer {
 public:
  LabelDismantle(void);
  virtual Node * at_label(labelNode *, Order);
};

class InitializerDismantle : public Changer {
 public:
  InitializerDismantle(void);
  virtual Node * at_block(blockNode *, Order);
  virtual Node * at_decl(declNode *, Order);

 private:
  map<blockNode *, blockNode *> _assgblock;
  stack<blockNode *> _blockStack;
  
  exprNode * eval_or_cast(exprNode *, typeNode *);

  void init_scalar(declNode *, exprNode *);
  initializerNode * init_array(arrayNode *, expr_list_p &, expr_list_p,
			       bool inSublist);
  initializerNode * init_struct(structNode *, expr_list_p &, expr_list_p,
				bool inSublist);

  bool isStringLiteral(exprNode *);
  initializerNode * strLit2Init(const char *);
};

class ControlDismantle : public Changer {
 public:
  ControlDismantle(void);
  virtual Node * at_proc(procNode *, Order ord);
};

class BreakContinueChanger : public Changer {
 private:
  Node * _top;
  labelNode * _break_label;
  labelNode * _continue_label;
  loopNode  * _new_container;

 public:
  BreakContinueChanger(Node *, labelNode *, labelNode *, loopNode *L=NULL);
  virtual Node * at_break(breakNode *, Order);
  virtual Node * at_continue(continueNode *, Order);
};

class LoopDismantle : public Changer {
 public:
  LoopDismantle(void);
  virtual Node * at_while(whileNode *, Order);
  virtual Node * at_for(forNode *, Order);
  virtual Node * at_do(doNode *, Order);
};

class TernaryDismantle : public Changer {
  map<stmtNode*,blockNode*> _new_block;
  stmtNode                 *_cur_stmt;
  int                       _in_type;

 public:
  TernaryDismantle(void);
  virtual Node * at_stmt(stmtNode *, Order);
  virtual Node * at_ternary(ternaryNode *, Order);
  virtual Node * at_type(typeNode *, Order);
};

class SelectionDismantle : public Changer {
 private:
  map<stmtNode*,blockNode*> _expr_block;
  stmtNode *_cur_stmt;
  bool _in_expr;

 public:
  SelectionDismantle(void);
  virtual Node * at_exprstmt(exprstmtNode *, Order);
  virtual Node * at_return(returnNode *, Order);
  virtual Node * at_binary(binaryNode *, Order);
  virtual Node * at_if(ifNode *, Order);
  virtual Node * at_switch(switchNode *, Order);
  virtual Node * at_stmt(stmtNode *, Order);
 private:
  indexNode * comparison_operand(exprNode *, blockNode *);
  Node * andand_oror_in_expr(stmtNode *, Order);
};

class ReturnDismantle : public Changer {
 public:
  ReturnDismantle(void);
  virtual Node * at_proc(procNode *, Order);
  virtual Node * at_return(returnNode *, Order);
};

class ArrowDismantle : public Changer {
 public:
  ArrowDismantle(void);
  virtual Node * at_binary(binaryNode *, Order);
};

class ExpressionDismantle : public Changer {
 private:
  blockNode * _new_block;
  int _in_type;
  static op_id_map _op_assign_map;
  static op_id_set _op_post;

 public:
  static void init();

  ExpressionDismantle(void);
  virtual Node * at_exprstmt(exprstmtNode *, Order);
  virtual Node * at_type(typeNode *, Order);
  virtual Node * at_binary(binaryNode *, Order);
  virtual Node * at_unary(unaryNode *, Order);
  virtual Node * at_call(callNode *, Order);
  virtual Node * at_comma(commaNode *, Order);
  virtual Node * at_cast(castNode *, Order);
};

class FlattenDismantle : public Changer {
 private:
  blockNode * _new_body;
  stmtNode * _last_stmt;
  labelNode * _return_label;
  procNode * _cur_proc;
  set<string> _vars;
  set<string> _labels;
  unitNode * _cur_unit;

 public:
  FlattenDismantle(unitNode *);
  virtual Node * at_proc(procNode *, Order);
  virtual Node * at_decl(declNode *, Order);
  virtual Node * at_label(labelNode *, Order);
  virtual Node * at_threeAddr(threeAddrNode *, Order);
  virtual Node * at_goto(gotoNode *, Order);
  virtual Node * at_return(returnNode *, Order);
  virtual Node * at_exprstmt(exprstmtNode *, Order);
};

#endif /* CBZ_DISMANTLE_H */
