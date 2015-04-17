// $Id: changer.h,v 1.10 2003/08/07 23:13:42 pnav Exp $
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

#ifndef CBZ_CHANGER_H
#define CBZ_CHANGER_H

/** @brief A functional class to traverse an AST, and perform some
 *  transformation at each of its Nodes.
 *  @ingroup AST_Traversal
 *
 *  A Changer is a functional class that traverses an abstract syntax
 *  tree, and performs some transformation at each of its Nodes.  At
 *  each Node, the transformation it performs depends on the class of
 *  the Node.
 *
 *  Corresponding to each class in the Node hierarchy, Changer defines
 *  an "at_" method.  To specify the transformation that should be
 *  performed for Nodes of a particular class, create a subclass of
 *  Changer that overrides the "at_" method corresponding to that
 *  class.  These "at_" methods take a Node, and return a transformed
 *  Node.  By default, the "at_" methods simply return the Node they
 *  are passed.  Thus, a Changer subclass only needs to override the
 *  "at_" methods corresponding to Node types that are relevant to
 *  that changer.
 *
 *  As the AST is traversed, the most specific "at_" method that has
 *  been defined for each Node will be called on that Node.  For
 *  example, if a Changer subclass overrides "at_decl()", but not
 *  "at_subdecl()," then the "at_decl()" method wil be called for all
 *  declNodes and subDeclnodes encountered in the AST.  However, if a
 *  Changer subclass overrides both "at_decl()" and "at_subdecl," then
 *  the "at_decl()" method will be called for all declNodes
 *  encountered in the AST, and the "at_subdecl()" method will be
 *  called for all subdeclNodes encountered in the AST.  If you wish
 *  for both "at_subdecl()" and "at_decl()" to be called for
 *  subdeclNodes, then you should explicitly call the "at_decl()"
 *  method from "at_subdecl()."
 *
 *  As the changer traverses the AST, the "at_" methods for a Node can
 *  be called before its children have been visited, after its
 *  children have been visited, or both before and after its children
 *  have been visited.  When the "at_" methods for a Node are called
 *  before its children, they are said to be called in "preorder."
 *  When the "at_" methods for a Node are called after its children,
 *  they are said to be called in "postorder."  If "at_" methods are
 *  called both in preorder and in postorder, then the preorder call
 *  will always preceed the postorder call.
 *
 *  When a new Changer is created, the order(s) that Nodes are visited
 *  in should be specified.  In general, all instances of a given
 *  subclass of Changer will visit Nodes in the same order.  However,
 *  it may occasionally be useful to specify different orders for two
 *  different instances of the same subclass of Changer.
 *
 *  New instances of Changer can also specify the "depth" of the
 *  traversal.  If the depth is "SubTree," then a Changer will visit
 *  the entire AST rooted at the Node that it is called on.  If the
 *  depth is "NodeOnly," then the Changer will only call the "at_"
 *  method for Node it is called on, and not for any of its sub-Nodes.
 *  A Changer with a depth of NodeOnly is different from a Visitor,
 *  because the "Node.change()" method will return a transformed Node,
 *  whereas "Node.visit()" cannot return a transformed Node.
 *
 *  Changers may modify any fields of the Nodes it visits, including
 *  fields that contain sub-Nodes.  If a field containing a sub-Node
 *  is changed in a preorder call to an "at_" method, then the AST
 *  rooted at the new sub-Node will be traversed.  If a field
 *  containing a sub-Node is changed in a postorder call to an "at_"
 *  method, then the AST rooted at the new sub-Node will not be
 *  traversed.
 *
 *  Currently, Changers traverse the AST in a depth-first order.
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
 *  Changers should never be used to traverse ASTs containing cycles;
 *  since they detect no mechanism to detect cycles, this would cause
 *  an infinite loop.
 *
 *  To use a changer "my_changer" to traverse an AST rooted at
 *  "my_node," use the code:
 *
 *  @code
 *  my_node = my_node->change(my_changer);
 *  @endcode
 *
 *  @see Visitor
 *  @see Walker
 *  @see Node::change */


class Changer {

public:

  typedef enum { Preorder, Postorder, Both } Order;
  typedef enum { Subtree, NodeOnly } Depth;

private:

  /** @brief The order in which AST Nodes should be visited. */

  Order _order;

  /** @brief Which Nodes in the AST should be visited. */

  Depth _depth;

  /** @brief Whether old Nodes should be deleted */

  bool _delete_old;

public:

  /** @brief Create a new instance of a Changer.
   *
   *  This constructor is usually called by a subclass.
   *  @param the_order The order in which AST Nodes should be visited.
   *      Nodes can be visited before their children (Preorder), after
   *      their children (Postorder), or both (Both).
   *  @param depth Specifies which Nodes in the AST should be visited.
   *      A value of SubTree specifies that the entire AST should be
   *      traversed.  A value of NodeOnly specifies that only the root
   *      Node in the AST should be visited.
   * @param delete_old Whether this changer deletes Nodes which are
   *     removed from the AST by "at_" methods.  */

  Changer(Order the_order, Depth depth, bool delete_old)
    : _order(the_order),
      _depth(depth),
      _delete_old(delete_old)
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

  /** @brief Return whether this Changer deletes Nodes which are
   *  removed from the AST by "at_" methods. */

  inline bool delete_old() const { return _delete_old; }

  //@}

  /** @name "at_" methods
   *
   *  These methods define the functions that should be performed when
   *  different classes of Nodes are encountered in the AST.  The most
   *  specific "at_" method that matches a given Node's class will be
   *  called. */
  //@{



  virtual Node * at_node(Node * the_node, Order ord)
     { return the_node; }

  virtual Node * at_unit(unitNode * the_unit, Order ord)
     { return at_node(the_unit, ord); }

  virtual Node * at_def(defNode * the_def, Order ord)
     { return at_node(the_def, ord); }

  virtual Node * at_decl(declNode * the_decl, Order ord)
     { return at_def(the_decl, ord); }

  virtual Node * at_subdecl(subdeclNode * the_subdecl, Order ord)
     { return at_decl(the_subdecl, ord); }

  virtual Node * at_proc(procNode * the_proc, Order ord)
     { return at_def(the_proc, ord); }

  virtual Node * at_type(typeNode * the_type, Order ord)
     { return at_node(the_type, ord); }

  virtual Node * at_prim(primNode * the_prim, Order ord)
     { return at_type(the_prim, ord); }

  virtual Node * at_tdef(tdefNode * the_tdef, Order ord)
     { return at_type(the_tdef, ord); }

  virtual Node * at_ptr(ptrNode * the_ptr, Order ord)
     { return at_type(the_ptr, ord); }

  virtual Node * at_array(arrayNode * the_array, Order ord)
     { return at_type(the_array, ord); }

  virtual Node * at_func(funcNode * the_func, Order ord)
     { return at_type(the_func, ord); }

  virtual Node * at_sue(sueNode * the_sue, Order ord)
     { return at_type(the_sue, ord); }

  virtual Node * at_struct(structNode * the_struct, Order ord)
     { return at_sue(the_struct, ord); }

  virtual Node * at_union(unionNode * the_union, Order ord)
     { return at_sue(the_union, ord); }

  virtual Node * at_enum(enumNode * the_enum, Order ord)
     { return at_sue(the_enum, ord); }

  virtual Node * at_suespec(suespecNode * the_suespec, Order ord)
     { return at_type(the_suespec, ord); }

  virtual Node * at_expr(exprNode * the_expr, Order ord)
     { return at_node(the_expr, ord); }

  virtual Node * at_index(indexNode * the_index, Order ord)
    { return at_expr(the_index, ord); }

  virtual Node * at_const(constNode * the_const, Order ord)
     { return at_index(the_const, ord); }

  virtual Node * at_id(idNode * the_id, Order ord)
     { return at_index(the_id, ord); }

  virtual Node * at_binary(binaryNode * the_binary, Order ord)
     { return at_expr(the_binary, ord); }

  virtual Node * at_unary(unaryNode * the_unary, Order ord)
     { return at_expr(the_unary, ord); }

  virtual Node * at_cast(castNode * the_cast, Order ord)
     { return at_expr(the_cast, ord); }

  virtual Node * at_comma(commaNode * the_comma, Order ord)
     { return at_expr(the_comma, ord); }

  virtual Node * at_ternary(ternaryNode * the_ternary, Order ord)
     { return at_expr(the_ternary, ord); }

  virtual Node * at_call(callNode * the_call, Order ord)
     { return at_expr(the_call, ord); }

  virtual Node * at_initializer(initializerNode * the_initializer, Order ord)
     { return at_expr(the_initializer, ord); }

  virtual Node * at_stmt(stmtNode * the_stmt, Order ord)
     { return at_node(the_stmt, ord); }

  virtual Node * at_block(blockNode * the_block, Order ord)
     { return at_stmt(the_block, ord); }

  virtual Node * at_basicblock(basicblockNode * the_basicblock, Order ord)
     { return at_block(the_basicblock, ord); }

  virtual Node * at_exprstmt(exprstmtNode * the_exprstmt, Order ord)
     { return at_stmt(the_exprstmt, ord); }

  virtual Node * at_target(targetNode * the_target, Order ord)
     { return at_stmt(the_target, ord); }

  virtual Node * at_label(labelNode * the_label, Order ord)
     { return at_target(the_label, ord); }

  virtual Node * at_case(caseNode * the_case, Order ord)
     { return at_target(the_case, ord); }

  virtual Node * at_selection(selectionNode * the_selection, Order ord)
     { return at_stmt(the_selection, ord); }

  virtual Node * at_if(ifNode * the_if, Order ord)
     { return at_selection(the_if, ord); }

  virtual Node * at_switch(switchNode * the_switch, Order ord)
     { return at_selection(the_switch, ord); }

  virtual Node * at_loop(loopNode * the_loop, Order ord)
     { return at_stmt(the_loop, ord); }

  virtual Node * at_while(whileNode * the_while, Order ord)
     { return at_loop(the_while, ord); }

  virtual Node * at_do(doNode * the_do, Order ord)
     { return at_loop(the_do, ord); }

  virtual Node * at_for(forNode * the_for, Order ord)
     { return at_loop(the_for, ord); }

  virtual Node * at_jump(jumpNode * the_jump, Order ord)
     { return at_stmt(the_jump, ord); }

  virtual Node * at_goto(gotoNode * the_goto, Order ord)
     { return at_jump(the_goto, ord); }

  virtual Node * at_continue(continueNode * the_continue, Order ord)
     { return at_jump(the_continue, ord); }

  virtual Node * at_break(breakNode * the_break, Order ord)
     { return at_jump(the_break, ord); }

  virtual Node * at_return(returnNode * the_return, Order ord)
     { return at_jump(the_return, ord); }

  virtual Node * at_attrib(attribNode * the_attrib, Order ord)
     { return at_stmt(the_attrib, ord); }
  
  virtual Node * at_operand(operandNode * the_oper, Order ord)
    { return at_expr(the_oper, ord); }

  virtual Node * at_conditiongoto(conditiongotoNode * the_condgoto, Order ord)
    { return at_goto(the_condgoto, ord); }

  virtual Node * at_threeAddr(threeAddrNode * the_3addr, Order ord)
    { return at_stmt(the_3addr, ord); }

  virtual Node * at_text(textNode * the_text, Order ord)
     { return at_node(the_text, ord); }
  //@}

private:

  /* -- Deprecated...

  virtual Node * at_implicitcast(implicitcastNode * the_implicitcast, Order ord)
     { return at_expr(the_implicitcast, ord); }

  virtual Node * at_default(defaultNode * the_default, Order ord)
     { return at_target(the_default, ord); }

  virtual Node * at_ifelse(ifelseNode * the_ifelse, Order ord)
     { return at_selection(the_ifelse, ord); }

  */

};

/** @brief Changes each member of a list safely
 *  @ingroup AST_Traversal
 *
 *
 */

template< class T >
void change_list(list< T > & l, Changer & the_changer)
{
  typename list< T >::iterator p = l.begin();

  while (p != l.end()) {

    Node * n = (*p)->change(the_changer);

    if (!n) {

      // -- Delete this element

      typename list< T >::iterator q = p;
      p++;
      l.erase(q);
    }
    else {

      if (n != (*p)) {

	// -- Replace the element

	if (the_changer.delete_old())
	  delete (*p);

	(*p) = (T) n;
      }

      p++;
    }
  }
}


#endif // CBZ_CHANGER_H
