// $Id: walker.h,v 1.9 2003/08/07 23:13:57 pnav Exp $
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

#ifndef CBZ_WALKER_H
#define CBZ_WALKER_H

/** @brief A functional class to traverse an AST, and perform some
 *  function at each of its Nodes.
 *  @ingroup AST_Traversal
 *
 *  A Walker is a functional class that traverses an abstract syntax
 *  tree, and performs some function at each of its Nodes.  At each
 *  Node, the function it performs depends on the class of the Node.
 *
 *  Corresponding to each class in the Node hierarchy, Walker defines
 *  an "at_" method.  To specify the function that should be performed
 *  for Nodes of a particular class, create a subclass of Walker that
 *  overrides the "at_" method corresponding to that class.  By
 *  default, the "at_" methods perform no action.  Thus, a Walker
 *  subclass only needs to override the "at_" methods corresponding to
 *  Node types that are relevant to that walker.
 *
 *  As the AST is traversed, the most specific "at_" method that has
 *  been defined for each Node will be called on that Node.  For
 *  example, if a Walker subclass overrides "at_decl()", but not
 *  "at_subdecl()," then the "at_decl()" method wil be called for all
 *  declNodes and subDeclnodes encountered in the AST.  However, if a
 *  Walker subclass overrides both "at_decl()" and "at_subdecl," then
 *  the "at_decl()" method will be called for all declNodes
 *  encountered in the AST, and the "at_subdecl()" method will be
 *  called for all subdeclNodes encountered in the AST.  If you wish
 *  for both "at_subdecl()" and "at_decl()" to be called for
 *  subdeclNodes, then you should explicitly call the "at_decl()"
 *  method from "at_subdecl()."
 *
 *  As the walker traverses the AST, the "at_" methods for a Node can
 *  be called before its children have been visited, after its
 *  children have been visited, or both before and after its children
 *  have been visited.  When the "at_" methods for a Node are called
 *  before its children, they are said to be called in "preorder."
 *  When the "at_" methods for a Node are called after its children,
 *  they are said to be called in "postorder."  If "at_" methods are
 *  called both in preorder and in postorder, then the preorder call
 *  will always preceed the postorder call.
 *
 *  When a new Walker is created, the order(s) that Nodes are visited
 *  in should be specified.  In general, all instances of a given
 *  subclass of Walker will visit Nodes in the same order.  However,
 *  it may occasionally be useful to specify different orders for two
 *  different instances of the same subclass of Walker.
 *
 *  New instances of Walker can also specify the "depth" of the
 *  traversal.  If the depth is "SubTree," then a Walker will visit
 *  the entire AST rooted at the Node that it is called on.  If the
 *  depth is "NodeOnly," then the Walker will only call the "at_"
 *  method for Node it is called on, and not for any of its sub-Nodes.
 *  Thus, a Walker with a depth of NodeOnly functions as if it were a
 *  Visitor.  If a Walker subclass will always use a depth of
 *  NodeOnly, then it should be written as a Visitor instead.  The
 *  depth value is provided because it can occasionally be useful to
 *  use a Walker subclass that was designed to traverse an entire AST
 *  to simply visit one Node.  In this case, you can simply
 *  instantiate the Walker subclass with a depth of NodeOnly.
 *
 *  It is possible for a Walker to modify the fields of the Nodes it
 *  visits.  However, Walkers should not modify the structure of the
 *  AST that is being traversed.  In other words, Walkers should only
 *  modify Node fields that do not contain sub-Nodes.  If the
 *  structure of the traversed AST needs to be modified as it is
 *  traversed, then a Changer should be used instead of a Walker.
 *
 *  Currently, Walkers traverse the AST in a depth-first order.
 *  Preorder calls to the "at_" methods are called before descending
 *  into a Node's children, and postorder calls to the "at_" method
 *  are called after returning from the children in the traversal.  At
 *  the leaves, the postorder calls to the "at_" methods immediately
 *  follow the preorder calls.  However, this ordering is subject to
 *  change, and should not be relied upon.  The only aspects of the
 *  AST traversal ordering which are guaranteed are:
 *
 *  <UL>
 *      <LI> The preorder call to a Node's "at_" method is called
 *           before any of the calls to the "at_" methods of the
 *           Node's descendants.
 *      <LI> The postorder call to a Node's "at_" method is called
 *           after any of the calls to the "at_" methods of the
 *           Node's descendants.
 *      <LI> The preorder call to a Node's "at_" method is called
 *           before the postorder call to a Node's "at_" method
 *           is called.
 *  </UL>
 *
 *  Walkers should never be used to traverse ASTs containing cycles;
 *  since they detect no mechanism to detect cycles, this would cause
 *  an infinite loop.
 *
 *  To use a walker "my_walker" to traverse an AST rooted at
 *  "my_node," use the code:
 *
 *  @code
 *  my_node->walk(my_walker);
 *  @endcode
 *
 *  @see Visitor
 *  @see Changer
 *  @see Node::walk */

class Walker {

public:

  /** @brief The order in which AST Nodes should be visited */
  typedef enum {
    Preorder,     /**< Nodes should be visited before their children. */
    Postorder,    /**< Nodes should be visited after their children. */
    Both          /**< Nodes should be visited both before and after
                     their children. */
  } Order;

  /** @brief Which Nodes in the AST tree should be visited */
  typedef enum {
    Subtree,     /**< The entire AST should be traversed. */
    NodeOnly     /**< Only the root Node should be visited. */
  } Depth;

private:

  /** @brief The order in which AST Nodes should be visited. */
  Order _order;

  /** @brief Which Nodes in the AST should be visited. */
  Depth _depth;


public:

  /** @brief Create a new instance of a Walker.
   *
   *  This constructor is usually called by a subclass.
   *  @param the_order The order in which AST Nodes should be visited.
   *      Nodes can be visited before their children (Preorder), after
   *      their children (Postorder), or both (Both).
   *  @param depth Specifies which Nodes in the AST should be visited.
   *      A value of SubTree specifies that the entire AST should be
   *      traversed.  A value of NodeOnly specifies that only the root
   *      Node in the AST should be visited.  In general, this
   *      parameter should never be hard-coded as NodeOnly; instead, a
   *      Visitor should be used.  However, the constructor for a
   *      subclass of Walker may take a Depth as one of its arguments,
   *      and pass it to the Walker subclass.
   */

  Walker(Order the_order, Depth depth)
    : _order(the_order),
      _depth(depth)
  {}

  /** @name Accessors
   *
   *  Methods to get and set fields in the class. */
  //@{

  /** @brief Return the order in which Nodes of an AST should be
   *  visited.
   *
   *  Nodes can be visited before their children (Preorder), after
   *  their children (Postorder), or both (Both). */

  inline Order order() const { return _order; }

  /** @brief Return which Nodes of an AST should be visited.
   *
   *  A value of SubTree specifies that the entire AST should be
   *  traversed.  A value of NodeOnly specifies that only the root
   *  Node in the AST should be visited. */

  inline Depth depth() const { return _depth; }

  //@}

  /** @name "at_" methods
   *
   *  These methods define the functions that should be performed when
   *  different classes of Nodes are encountered in the AST.  The most
   *  specific "at_" method that matches a given Node's class will be
   *  called. */
  //@{

  virtual void at_node(Node * the_node, Order ord)
     { // Default: do nothing
     }

  virtual void at_unit(unitNode * the_unit, Order ord)
     { at_node(the_unit, ord); }

  virtual void at_def(defNode * the_def, Order ord)
     { at_node(the_def, ord); }

  virtual void at_decl(declNode * the_decl, Order ord)
     { at_def(the_decl, ord); }

  virtual void at_subdecl(subdeclNode * the_subdecl, Order ord)
     { at_decl(the_subdecl, ord); }

  virtual void at_proc(procNode * the_proc, Order ord)
     { at_def(the_proc, ord); }

  virtual void at_type(typeNode * the_type, Order ord)
     { at_node(the_type, ord); }

  virtual void at_prim(primNode * the_prim, Order ord)
     { at_type(the_prim, ord); }

  virtual void at_tdef(tdefNode * the_tdef, Order ord)
     { at_type(the_tdef, ord); }

  virtual void at_ptr(ptrNode * the_ptr, Order ord)
     { at_type(the_ptr, ord); }

  virtual void at_array(arrayNode * the_array, Order ord)
     { at_type(the_array, ord); }

  virtual void at_func(funcNode * the_func, Order ord)
     { at_type(the_func, ord); }

  virtual void at_sue(sueNode * the_sue, Order ord)
     { at_type(the_sue, ord); }

  virtual void at_struct(structNode * the_struct, Order ord)
     { at_sue(the_struct, ord); }

  virtual void at_union(unionNode * the_union, Order ord)
     { at_sue(the_union, ord); }

  virtual void at_enum(enumNode * the_enum, Order ord)
     { at_sue(the_enum, ord); }

  virtual void at_suespec(suespecNode * the_suespec, Order ord)
     { at_type(the_suespec, ord); }

  virtual void at_expr(exprNode * the_expr, Order ord)
     { at_node(the_expr, ord); }

  virtual void at_index(indexNode * the_index, Order ord)
    { at_expr(the_index, ord); }

  virtual void at_const(constNode * the_const, Order ord)
     { at_index(the_const, ord); }

  virtual void at_id(idNode * the_id, Order ord)
     { at_index(the_id, ord); }

  virtual void at_binary(binaryNode * the_binary, Order ord)
     { at_expr(the_binary, ord); }

  virtual void at_unary(unaryNode * the_unary, Order ord)
     { at_expr(the_unary, ord); }

  virtual void at_cast(castNode * the_cast, Order ord)
     { at_expr(the_cast, ord); }

  virtual void at_comma(commaNode * the_comma, Order ord)
     { at_expr(the_comma, ord); }

  virtual void at_ternary(ternaryNode * the_ternary, Order ord)
     { at_expr(the_ternary, ord); }

  virtual void at_call(callNode * the_call, Order ord)
     { at_expr(the_call, ord); }

  virtual void at_initializer(initializerNode * the_initializer, Order ord)
     { at_expr(the_initializer, ord); }

  virtual void at_stmt(stmtNode * the_stmt, Order ord)
     { at_node(the_stmt, ord); }

  virtual void at_block(blockNode * the_block, Order ord)
     { at_stmt(the_block, ord); }

  virtual void at_basicblock(basicblockNode * the_basicblock, Order ord)
     { at_block(the_basicblock, ord); }

  virtual void at_exprstmt(exprstmtNode * the_exprstmt, Order ord)
     { at_stmt(the_exprstmt, ord); }

  virtual void at_target(targetNode * the_target, Order ord)
     { at_stmt(the_target, ord); }

  virtual void at_label(labelNode * the_label, Order ord)
     { at_target(the_label, ord); }

  virtual void at_case(caseNode * the_case, Order ord)
     { at_target(the_case, ord); }

  virtual void at_selection(selectionNode * the_selection, Order ord)
     { at_stmt(the_selection, ord); }

  virtual void at_if(ifNode * the_if, Order ord)
     { at_selection(the_if, ord); }

  virtual void at_switch(switchNode * the_switch, Order ord)
     { at_selection(the_switch, ord); }

  virtual void at_loop(loopNode * the_loop, Order ord)
     { at_stmt(the_loop, ord); }

  virtual void at_while(whileNode * the_while, Order ord)
     { at_loop(the_while, ord); }

  virtual void at_do(doNode * the_do, Order ord)
     { at_loop(the_do, ord); }

  virtual void at_for(forNode * the_for, Order ord)
     { at_loop(the_for, ord); }

  virtual void at_jump(jumpNode * the_jump, Order ord)
     { at_stmt(the_jump, ord); }

  virtual void at_goto(gotoNode * the_goto, Order ord)
     { at_jump(the_goto, ord); }

  virtual void at_continue(continueNode * the_continue, Order ord)
     { at_jump(the_continue, ord); }

  virtual void at_break(breakNode * the_break, Order ord)
     { at_jump(the_break, ord); }

  virtual void at_return(returnNode * the_return, Order ord)
     { at_jump(the_return, ord); }

  virtual void at_attrib(attribNode * the_attrib, Order ord)
     { at_stmt(the_attrib, ord); }

  virtual void at_operand(operandNode * the_oper, Order ord)
    { at_expr(the_oper, ord); }

  virtual void at_conditiongoto(conditiongotoNode * the_condgoto, Order ord)
    { at_goto(the_condgoto, ord); }

  virtual void at_threeAddr(threeAddrNode * the_3addr, Order ord)
    { at_stmt(the_3addr, ord); }

  virtual void at_text(textNode * the_text, Order ord)
     { at_node(the_text, ord); }

  //@}

private:

  /* -- Deprecated...

  virtual void at_implicitcast(implicitcastNode * the_implicitcast, Order ord)
     { at_expr(the_implicitcast, ord); }

  virtual void at_default(defaultNode * the_default, Order ord)
     { at_target(the_default, ord); }

  virtual void at_ifelse(ifelseNode * the_ifelse, Order ord)
     { at_selection(the_ifelse, ord); }

  */
};

// -- Invoke the walker on a list...

/** @brief Invoke a given walker on every element of a list.
 *
 *  @param the_walker The walker which should be invoked on every
 *  element of the list.
 *
 *  @param l The list on which to invoke the Walker.  This list should
 *  contain only Nodes or subclasses of Node. */

template< class T >
void list_walker(list< T > & l, Walker & the_walker)
{
  for (typename list< T >::iterator p = l.begin();
       p != l.end();
       ++p)
    (*p)->walk(the_walker);
}


#endif // CBZ_WALKER_H
