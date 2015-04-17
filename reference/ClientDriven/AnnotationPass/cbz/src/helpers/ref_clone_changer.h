// $Id: ref_clone_changer.h,v 1.10 2003/08/07 23:13:50 pnav Exp $
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

#ifndef CBZ_REF_CLONE_CHANGER_H
#define CBZ_REF_CLONE_CHANGER_H


typedef map< Node *,
             Node *,
             less< Node * > > node_map;

typedef node_map::iterator node_map_p;

/** @brief Performs a deep copy on part of the AST.
 *  @ingroup Phases
 *
 *  This Changer performs a deep copy on the portion of the AST underneath
 *  the Node given to it.  In addition to copying the structure, it also
 *  updates internal links between Nodes in the structure.  These links
 *  include those between idNode & declNode, tdefNode & the relevant
 *  typeNode, callNode & procNode, caseNode & switchNode, gotoNode
 *  (including conditiongotoNode) & labelNode, continueNode/breakNode &
 *  the container stmtNode, returnNode & procNode, and procNode &
 *  the related return_label() & return_decl() in dismantled code.
 */
class ref_clone_changer : public Changer
{
 public:

  /** @brief Entry point for ref_clone_changer
   *
   * This static method is the entry point for using the
   * ref_clone_changer.  The portion of the AST under Node n is
   * copied.  Relations between nodes that are both copied is updated
   * by the Changer.  For example, if both an idNode and its declNode
   * are copied, then the new idNode will refer to the declNode and
   * vice versa.
   *
   * If nil_bad_links is false, then pointers from copied nodes to a
   * Node outside the subtree are left unchanged.  For example, if an
   * idNode is copied, but the original declNode is not copied, then the
   * new idNode will refer to the original declNode.  If nil_bad_links is
   * true, then these references will be set to NULL.  In this case, an idNode
   * copied without copying the declNode will have a NULL-reference for its
   * decl().
   */
  static Node * clone(Node * n, bool nil_bad_links);

 private:

  node_map * map() const { return _map; }

  node_map * _map;

  ref_clone_changer()
    : Changer(Preorder, Subtree, false),
      _map(new node_map())
  {}

  virtual Node * at_node(Node * the_node, Order ord)
  {
    Node * newnode =  the_node->clone();
    (*_map)[the_node] = newnode;
    return newnode;
  }

  virtual Node * at_decl(declNode * the_decl, Order ord) {
    declNode * newnode = (declNode *) this->at_node(the_decl, ord);
    newnode->ref_list().clear();
    return newnode;
  }

  virtual Node * at_label(labelNode * the_label, Order ord) {
    labelNode * newnode = (labelNode *) this->at_node(the_label, ord);
    newnode->references().clear();
    return newnode;
  }

  virtual Node * at_switch(switchNode * the_switch, Order ord) {
    switchNode * newnode = (switchNode *) this->at_node(the_switch, ord);
    newnode->cases().clear();
    return newnode;
  }
};

class ref_fix_walker : public Walker
{
private:

  node_map * _map;
  bool _nil_bad_links;

  Node * lookup(Node * old);

public:

  ref_fix_walker(node_map * m, bool nil_bad_links)
    : Walker(Preorder, Subtree),
      _map(m),
      _nil_bad_links(nil_bad_links)
  {}

  virtual void at_tdef(tdefNode * the_tdef, Order ord);
  virtual void at_id(idNode * the_id, Order ord);
  virtual void at_call(callNode * the_call, Order ord);
  virtual void at_case(caseNode * the_case, Order ord);
  virtual void at_goto(gotoNode * the_goto, Order ord);
  virtual void at_continue(continueNode * the_continue, Order ord);
  virtual void at_break(breakNode * the_break, Order ord);
  virtual void at_return(returnNode * the_return, Order ord);
  virtual void at_proc(procNode * the_proc, Order ord);
};


#endif // CBZ_REF_CLONE_CHANGER_H
