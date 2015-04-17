// $Id: tree_checker.h,v 1.4 2003/08/26 21:38:36 toktb Exp $
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

#ifndef CBZ_TREE_CHECKER_H
#define CBZ_TREE_CHECKER_H

#include "c_breeze.h"

typedef list<procNode *> proc_list;
typedef proc_list::iterator proc_list_p;
typedef list<switchNode *> switch_list;
typedef switch_list::iterator switch_list_p;
typedef list<labelNode *> label_list;
typedef label_list::iterator label_list_p;

class OperatorCheck {
 public:
  virtual bool operator()(Operator *) = 0;
  virtual string desc() = 0;
};

class TreeChecker : public Walker {
private:
  map<Node *, int> _visitedNodes;
  goto_list  _gotos;
  label_list _labels;
  proc_list _procs;
  proc_list _dupProcs;  // re-defined proc nodes 
  switch_list _switches;
  decl_list _decls;
  // indicates the number of gotoNodes that "point to" a labelNode
  map<labelNode *, int> _gotoCount;
  // argument count in procedures' formal parameter list
  map<string, int > _formals;
  int _typeDepth; // number of typeNodes we've traversed upto this Node
  int _arrayDepth; // number of arrayNodes we've traverse upto this Node

  bool _in_proc;
  bool _verbose;
  bool _warning;
  bool _preDismantled;
  bool _postDismantled;
  int  _errorCount;

  void setPreDismantled(const char *, Node *);
  void setPostDismantled(const char *, Node *);

  void checkTree (Node * n, const char * type);

  void checkField (Node * node, Node * field, const char * type, 
                   const char * fieldName, bool isError = true);
  void checkString (Node * node, string & field, const char * type, 
                   const char * fieldName, bool isError = true);
  void checkConstant (Node * node, constant & field, const char * type, 
                   const char * fieldName, bool isError = true);
  template <class T>
    void checkOperator (Node * node, Operator * field, const char * type, 
			const char * fieldName, T check, string desc,
			bool isError = true);
  template <class T>
    void checkFieldInList (Node * node, T field, const char * type, 
			   const char * fieldName, list<T> & collection, 
			   bool isError = true);
  template <class T>
    void checkList (Node * node, list<T> & field, const char * type, 
		    const char * fieldName, bool isError = true);
  template <class T>
    void checkFieldInList (Node * node, list<T> & field, const char * type, 
			   const char * fieldName, int count, 
			   list<T> & collection, const char * itemName,
			   bool isError = true);

public:
  TreeChecker ();
  TreeChecker (bool verbose, bool warning);
  virtual ~TreeChecker ();

  static void check();
  static void check(unitNode *);

  virtual void at_node (Node *, Order);
  virtual void at_unit (unitNode *, Order);
  virtual void at_proc (procNode *, Order);
  virtual void at_decl (declNode *, Order);

  // target node
  virtual void at_label (labelNode *, Order);
  virtual void at_case (caseNode *, Order);

  // jump node
  virtual void at_goto (gotoNode *, Order);
  virtual void at_conditiongoto (conditiongotoNode *, Order);
  virtual void at_break (breakNode *, Order);
  virtual void at_continue (continueNode *, Order);
  virtual void at_return (returnNode *, Order);

  // selection node
  virtual void at_if (ifNode *, Order);
  virtual void at_switch (switchNode *, Order);

  // loop node
  virtual void at_for (forNode *, Order);
  virtual void at_loop (loopNode *, Order);

  // expr node
  virtual void at_call (callNode *, Order);
  virtual void at_id (idNode *, Order);
  virtual void at_const (constNode *, Order);
  virtual void at_unary (unaryNode *, Order);
  virtual void at_binary (binaryNode *, Order);
  virtual void at_ternary (ternaryNode *, Order);
  virtual void at_threeAddr (threeAddrNode *, Order);
  virtual void at_operand (operandNode *, Order);
  virtual void at_cast (castNode *, Order);
  virtual void at_comma (commaNode *, Order);
  virtual void at_initializer (initializerNode *, Order);

  // type node
  virtual void at_type (typeNode *, Order);
  virtual void at_func (funcNode *, Order);
  virtual void at_tdef (tdefNode *, Order);
  virtual void at_array (arrayNode *, Order);
};

class ProcWalker : public Walker {
public:
  ProcWalker () : Walker(Preorder, Subtree) {}
  virtual ~ProcWalker () {
    _labels.clear();
    _gotos.clear();
    _switches.clear();
    _gotoCount.clear();
    _decls.clear();
  }

  virtual void at_decl (declNode * the_decl, Order ord)
    { _decls.push_back(the_decl); }
  virtual void at_label (labelNode * the_label, Order ord)
    { _labels.push_back(the_label); }
  virtual void at_switch (switchNode * the_switch, Order ord)
    { _switches.push_back(the_switch); }
  virtual void at_goto (gotoNode * the_goto, Order ord) {
    _gotos.push_back(the_goto);
    // add a reference count to the label node
    if (the_goto->label()) _gotoCount[the_goto->label()]++;
  }

  const label_list & labels() const { return _labels; }
  const goto_list & gotos() const { return _gotos; }
  const switch_list & switches() const { return _switches; }
  const decl_list & decls() const { return _decls; }
  const map<labelNode *, int> & gotoCount() const { return _gotoCount; }

 private:
  label_list _labels;
  goto_list _gotos;
  switch_list _switches;
  decl_list _decls;
  // indicates the number of gotoNodes that "point to" a labelNode
  map<labelNode *, int> _gotoCount;
};

class UnitWalker : public Walker {
 public:
  UnitWalker();
  ~UnitWalker();

  virtual void at_proc(procNode *, Order);
  
  const proc_list & procs() const { return _procs; }
  const list<string> & procNames() const { return _procNames; }
  const proc_list & dupProcs() const { return _dupProcs; }
  const map<string, int> & formals() const { return _formals; }
 private:
  proc_list _procs;
  list<string> _procNames;
  proc_list _dupProcs;  // re-defined proc nodes
  // argument count in procedures' formal parameter list
  map<string, int> _formals;
};

#endif /* CBZ_TREE_CHECKER_H */
