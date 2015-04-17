// $Id: dataflow.h,v 1.6 2003/08/07 23:14:12 pnav Exp $
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

#ifndef CBZ_DATAFLOW_H
#define CBZ_DATAFLOW_H


class FlowProblem;

/** @brief a flow value for a flow problem
 */
class FlowVal
{
private:

public:

  // -- Clone

  virtual FlowVal * clone() const =0;

  // -- Comparison

  virtual bool diff(FlowVal * other) const =0;

  // -- Meet "^" operator
  // Must satisfy the following:
  //    Associative: x ^ (y ^ z) = (x ^ y) ^ z
  //    Commutative: x ^ y = y ^ x
  //    Idempotent:  x ^ x = x

  virtual void meet(const FlowVal * other) =0;

  // -- "T" top element
  // Must satisfy : x ^ T = x

  virtual void to_top() =0;

  // -- Don't change this function...

  void meet_and_diff(const FlowVal * other, FlowProblem & fp);
};


/** @brief super-class for flow problem definitions
 */
class FlowProblem
{

public:

  /** @brief Entry point */
  void iterate(Node * n, FlowVal * initial = 0);

private:

  bool _forward;
  bool _changed;
  bool _last;
  bool _basicblocks;
  FlowVal * _top;

public:

  typedef enum { Entry, Exit } Point;

  // -- Constructor

  FlowProblem(bool forw, FlowVal * top)
    : _forward(forw),
      _changed(true),
      _last(false),
      _basicblocks(false),
      _top(top)
  {}

  /** @defgroup dataflow_h_fields
      @brief field accessors
   */
  /* @{*/

  inline bool forward() const { return _forward; }

  inline bool changed() const { return _changed; }
  inline void changed(bool ch) { _changed = ch; }

  inline bool last() const { return _last; }
  inline void last(bool l) { _last = l; }

  inline bool basicblocks() const { return _basicblocks; }
  inline void basicblocks(bool a) { _basicblocks = a; }

  inline FlowVal * top() const { return _top; }
  /*@}*/

  /** brief special transfer function for top of expression subtrees.

      -- Special transfer function: this one is called at the top of
      every expression subtree. These are not necessarily basic blocks
      because they may contain operators like "?:" and "&&" which have
      control-flow semantics. However, we can easily preprocess the
      tree to remove these. NOTE: This isn't for "real" basicblockNodes;
      this is for expressions which sort of act like basic blocks
  */
  virtual void flow_basicblock(FlowVal * v, exprNode * the_expr, Point p)
     { flow_node(v, the_expr, p); }

  /** @defgroup dataflow_h_ast_transfuncs
      @brief AST-specific transfer functions
  */
  /*@{*/
  virtual void flow_node(FlowVal * v, Node * the_node, Point p)
     { // Default: do nothing
     }

  virtual void flow_unit(FlowVal * v, unitNode * the_unit, Point p)
     { flow_node(v, the_unit, p); }

  virtual void flow_def(FlowVal * v, defNode * the_def, Point p)
     { flow_node(v, the_def, p); }

  virtual void flow_decl(FlowVal * v, declNode * the_decl, Point p)
     { flow_def(v, the_decl, p); }

  virtual void flow_proc(FlowVal * v, procNode * the_proc, Point p)
     { flow_def(v, the_proc, p); }

  virtual void flow_type(FlowVal * v, typeNode * the_type, Point p)
     { flow_node(v, the_type, p); }

  virtual void flow_prim(FlowVal * v, primNode * the_prim, Point p)
     { flow_type(v, the_prim, p); }

  virtual void flow_tdef(FlowVal * v, tdefNode * the_tdef, Point p)
     { flow_type(v, the_tdef, p); }

  virtual void flow_ptr(FlowVal * v, ptrNode * the_ptr, Point p)
     { flow_type(v, the_ptr, p); }

  virtual void flow_array(FlowVal * v, arrayNode * the_array, Point p)
     { flow_type(v, the_array, p); }

  virtual void flow_func(FlowVal * v, funcNode * the_func, Point p)
     { flow_type(v, the_func, p); }

  virtual void flow_sue(FlowVal * v, sueNode * the_sue, Point p)
     { flow_type(v, the_sue, p); }

  virtual void flow_struct(FlowVal * v, structNode * the_struct, Point p)
     { flow_sue(v, the_struct, p); }

  virtual void flow_union(FlowVal * v, unionNode * the_union, Point p)
     { flow_sue(v, the_union, p); }

  virtual void flow_enum(FlowVal * v, enumNode * the_enum, Point p)
     { flow_sue(v, the_enum, p); }

  virtual void flow_suespec(FlowVal * v, suespecNode * the_suespec, Point p)
     { flow_type(v, the_suespec, p); }

  virtual void flow_expr(FlowVal * v, exprNode * the_expr, Point p)
     { flow_node(v, the_expr, p); }

  virtual void flow_const(FlowVal * v, constNode * the_const, Point p)
     { flow_expr(v, the_const, p); }

  virtual void flow_id(FlowVal * v, idNode * the_id, Point p)
     { flow_expr(v, the_id, p); }

  virtual void flow_binary(FlowVal * v, binaryNode * the_binary, Point p)
     { flow_expr(v, the_binary, p); }

  virtual void flow_unary(FlowVal * v, unaryNode * the_unary, Point p)
     { flow_expr(v, the_unary, p); }

  virtual void flow_cast(FlowVal * v, castNode * the_cast, Point p)
     { flow_expr(v, the_cast, p); }

  virtual void flow_comma(FlowVal * v, commaNode * the_comma, Point p)
     { flow_expr(v, the_comma, p); }

  virtual void flow_ternary(FlowVal * v, ternaryNode * the_ternary, Point p)
     { flow_expr(v, the_ternary, p); }

  virtual void flow_call(FlowVal * v, callNode * the_call, Point p)
     { flow_expr(v, the_call, p); }

  virtual void flow_initializer(FlowVal * v, initializerNode * the_initializer, Point p)
     { flow_expr(v, the_initializer, p); }

  virtual void flow_stmt(FlowVal * v, stmtNode * the_stmt, Point p)
     { flow_node(v, the_stmt, p); }

  virtual void flow_block(FlowVal * v, blockNode * the_block, Point p)
     { flow_stmt(v, the_block, p); }

  virtual void flow_exprstmt(FlowVal * v, exprstmtNode * the_exprstmt, Point p)
     { flow_stmt(v, the_exprstmt, p); }

  virtual void flow_target(FlowVal * v, targetNode * the_target, Point p)
     { flow_stmt(v, the_target, p); }

  virtual void flow_label(FlowVal * v, labelNode * the_label, Point p)
     { flow_target(v, the_label, p); }

  virtual void flow_case(FlowVal * v, caseNode * the_case, Point p)
     { flow_target(v, the_case, p); }

  virtual void flow_selection(FlowVal * v, selectionNode * the_selection, Point p)
     { flow_stmt(v, the_selection, p); }

  virtual void flow_if(FlowVal * v, ifNode * the_if, Point p)
     { flow_selection(v, the_if, p); }

  virtual void flow_switch(FlowVal * v, switchNode * the_switch, Point p)
     { flow_selection(v, the_switch, p); }

  virtual void flow_loop(FlowVal * v, loopNode * the_loop, Point p)
     { flow_stmt(v, the_loop, p); }

  virtual void flow_while(FlowVal * v, whileNode * the_while, Point p)
     { flow_loop(v, the_while, p); }

  virtual void flow_do(FlowVal * v, doNode * the_do, Point p)
     { flow_loop(v, the_do, p); }

  virtual void flow_for(FlowVal * v, forNode * the_for, Point p)
     { flow_loop(v, the_for, p); }

  virtual void flow_jump(FlowVal * v, jumpNode * the_jump, Point p)
     { flow_stmt(v, the_jump, p); }

  virtual void flow_goto(FlowVal * v, gotoNode * the_goto, Point p)
     { flow_jump(v, the_goto, p); }

  virtual void flow_continue(FlowVal * v, continueNode * the_continue, Point p)
     { flow_jump(v, the_continue, p); }

  virtual void flow_break(FlowVal * v, breakNode * the_break, Point p)
     { flow_jump(v, the_break, p); }

  virtual void flow_return(FlowVal * v, returnNode * the_return, Point p)
     { flow_jump(v, the_return, p); }

  virtual void flow_attrib(FlowVal * v, attribNode * the_attrib, Point p)
     { flow_stmt(v, the_attrib, p); }

  virtual void flow_text(FlowVal * v, textNode * the_text, Point p)
     { flow_node(v, the_text, p); }

  /*@}*/
};





#endif // CBZ_DATAFLOW_H
