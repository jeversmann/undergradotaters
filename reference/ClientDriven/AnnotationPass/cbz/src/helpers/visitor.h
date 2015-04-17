// $Id: visitor.h,v 1.9 2003/08/07 23:13:57 pnav Exp $
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

#ifndef CBZ_VISITOR_H
#define CBZ_VISITOR_H


/** @defgroup AST_Traversal AST Traversal functional classes.
 *
 */

/** @brief A functional class to peform a function on a Node,
 *  depending on its class.
 *  @ingroup AST_Traversal
 *
 *  Corresponding to each class in the Node hierarchy, Visitor defines
 *  an "at_" method.  To specify the function that should be performed
 *  for Nodes of a particular class, create a subclass of Visitor that
 *  overrides the "at_" method corresponding to that class.  By
 *  default, the "at_" methods perform no action.  Thus, a Visitor
 *  subclass only needs to override the "at_" methods corresponding to
 *  Node types that are relevant to that visitor.
 *
 *  When a Node is visited, the most specific "at_" method that has
 *  been defined for that Node will be called with the Node.  For
 *  example, if a Visitor subclass overrides "at_decl()", but not
 *  "at_subdecl()," then the "at_decl()" method wil be called for any
 *  declNodes or subDeclnodes that are visited.  However, if a Visitor
 *  subclass overrides both "at_decl()" and "at_subdecl," then the
 *  "at_decl()" method will be called for any declNodes that are
 *  visited, and the "at_subdecl()" method will be called for any
 *  subdeclNodes that are visited.  If you wish for both
 *  "at_subdecl()" and "at_decl()" to be called for subdeclNodes, then
 *  you should explicitly call the "at_decl()" method from
 *  "at_subdecl()."
 *
 *  A visitor may modify any fields of the Nodes it visits, including
 *  fields that contain sub-Nodes.
 *
 *  To use a visitor "my_visitor" to visit "my_node," use the code:
 *
 *  @code
 *  my_node->visit(my_visitor);
 *  @endcode
 *
 *  Visitors only visit the Node they are given, and not any of its
 *  sub-Nodes.  If you wish to visit the entire AST rooted at a Node,
 *  use a Walker or a Changer.  You may also recurse to select
 *  sub-Nodes using code like the following within an "at_" method:
 *  @code
 *  sub_node->visit(*this);
 *  @endcode
 *
 *  @see Walker
 *  @see Changer
 *  @see Node::visit
 */

class Visitor {

private:

public:

  /** @brief Create a new instance of a Visitor. */

  Visitor()
  {}

  /** @name "at_" methods
   *
   *  These methods define the functions that should be performed when
   *  different classes of Nodes are visited.  The most specific "at_"
   *  method that matches a given Node's class will be called. */
  //@{

  virtual void at_node(Node * the_node)
     { // Default: do nothing
     }

  virtual void at_unit(unitNode * the_unit)
     { at_node(the_unit); }

  virtual void at_def(defNode * the_def)
     { at_node(the_def); }

  virtual void at_decl(declNode * the_decl)
     { at_def(the_decl); }

  virtual void at_subdecl(subdeclNode * the_subdecl)
     { at_decl(the_subdecl); }

  virtual void at_proc(procNode * the_proc)
     { at_def(the_proc); }

  virtual void at_type(typeNode * the_type)
     { at_node(the_type); }

  virtual void at_prim(primNode * the_prim)
     { at_type(the_prim); }

  virtual void at_tdef(tdefNode * the_tdef)
     { at_type(the_tdef); }

  virtual void at_ptr(ptrNode * the_ptr)
     { at_type(the_ptr); }

  virtual void at_array(arrayNode * the_array)
     { at_type(the_array); }

  virtual void at_func(funcNode * the_func)
     { at_type(the_func); }

  virtual void at_sue(sueNode * the_sue)
     { at_type(the_sue); }

  virtual void at_struct(structNode * the_struct)
     { at_sue(the_struct); }

  virtual void at_union(unionNode * the_union)
     { at_sue(the_union); }

  virtual void at_enum(enumNode * the_enum)
     { at_sue(the_enum); }

  virtual void at_suespec(suespecNode * the_suespec)
     { at_type(the_suespec); }

  virtual void at_expr(exprNode * the_expr)
     { at_node(the_expr); }

  virtual void at_index(indexNode * the_index)
    { at_expr(the_index); }

  virtual void at_const(constNode * the_const)
     { at_index(the_const); }

  virtual void at_id(idNode * the_id)
     { at_index(the_id); }

  virtual void at_binary(binaryNode * the_binary)
     { at_expr(the_binary); }

  virtual void at_unary(unaryNode * the_unary)
     { at_expr(the_unary); }

  virtual void at_cast(castNode * the_cast)
     { at_expr(the_cast); }

  virtual void at_comma(commaNode * the_comma)
     { at_expr(the_comma); }

  virtual void at_ternary(ternaryNode * the_ternary)
     { at_expr(the_ternary); }

  virtual void at_call(callNode * the_call)
     { at_expr(the_call); }

  virtual void at_initializer(initializerNode * the_initializer)
     { at_expr(the_initializer); }

  virtual void at_stmt(stmtNode * the_stmt)
     { at_node(the_stmt); }

  virtual void at_block(blockNode * the_block)
     { at_stmt(the_block); }

  virtual void at_basicblock(basicblockNode * the_basicblock)
     { at_block(the_basicblock); }

  virtual void at_exprstmt(exprstmtNode * the_exprstmt)
     { at_stmt(the_exprstmt); }

  virtual void at_target(targetNode * the_target)
     { at_stmt(the_target); }

  virtual void at_label(labelNode * the_label)
     { at_target(the_label); }

  virtual void at_case(caseNode * the_case)
     { at_target(the_case); }

  virtual void at_selection(selectionNode * the_selection)
     { at_stmt(the_selection); }

  virtual void at_if(ifNode * the_if)
     { at_selection(the_if); }

  virtual void at_switch(switchNode * the_switch)
     { at_selection(the_switch); }

  virtual void at_loop(loopNode * the_loop)
     { at_stmt(the_loop); }

  virtual void at_while(whileNode * the_while)
     { at_loop(the_while); }

  virtual void at_do(doNode * the_do)
     { at_loop(the_do); }

  virtual void at_for(forNode * the_for)
     { at_loop(the_for); }

  virtual void at_jump(jumpNode * the_jump)
     { at_stmt(the_jump); }

  virtual void at_goto(gotoNode * the_goto)
     { at_jump(the_goto); }

  virtual void at_continue(continueNode * the_continue)
     { at_jump(the_continue); }

  virtual void at_break(breakNode * the_break)
     { at_jump(the_break); }

  virtual void at_return(returnNode * the_return)
     { at_jump(the_return); }

  virtual void at_attrib(attribNode * the_attrib)
     { at_stmt(the_attrib); }

  virtual void at_operand(operandNode * the_oper)
    { at_expr(the_oper); }

  virtual void at_conditiongoto(conditiongotoNode * the_condgoto)
    { at_goto(the_condgoto); }

  virtual void at_threeAddr(threeAddrNode * the_3addr)
    { at_stmt(the_3addr); }

  virtual void at_text(textNode * the_text)
     { at_node(the_text); }
  //@}

private:

  /* -- Deprecated...

  virtual void at_implicitcast(implicitcastNode * the_implicitcast)
     { at_expr(the_implicitcast); }

  virtual void at_default(defaultNode * the_default)
     { at_target(the_default); }

  virtual void at_ifelse(ifelseNode * the_ifelse)
     { at_selection(the_ifelse); }
  */

};

// -- Invoke the visitor on a list...

/** @brief Invoke a given visiter on every element of a list.
 *
 *  @param the_visiter The visiter which should be invoked on every
 *  element of the list.
 *
 *  @param l The list on which to invoke the Visiter.  This list should
 *  contain only Nodes or subclasses of Node. */

template< class T >
void list_visitor(list< T > & l, Visitor * the_visitor)
{
  for (typename list< T >::iterator p = l.begin();
       p != l.end();
       ++p)
    (*p)->visit(the_visitor);
}


#endif // CBZ_VISITOR_H
