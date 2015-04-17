// $Id: tree_checker.cc,v 1.5 2003/08/26 21:38:36 toktb Exp $
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

#include "tree_checker.h"
#include "print_walker.h"

void TreeChecker::setPreDismantled(const char * nodeType, Node * node) {
  _preDismantled = true;
  if ( _verbose )
    cout << nodeType << " node (" << node << ") set the pre-dismantled flag."
	 << endl;
}

void TreeChecker::setPostDismantled(const char * nodeType, Node * node) {
  _postDismantled = true;
  if ( _verbose )
    cout << nodeType << " node (" << node << ") set the post-dismantled flag."
	 << endl;
}

void TreeChecker::checkTree (Node * n, const char * type) {
  if (_visitedNodes[n]++ > 0) {
    cout << "Error: " << type << " node (" << n << ") has been visited "
         << _visitedNodes[n] << " times.\n";
    print_walker::print(n, cout);
    _errorCount++;
  }
  else if (_verbose) 
    cout << "Visiting " << type << " node (" << n << ").\n";
}

void TreeChecker::checkField (Node * node, Node * field, 
                              const char * type, const char * fieldName, 
                              bool isError /*= true*/) {
  if (! field) {
    if (isError) {
      cout << "Error: " << type << " node (" << node 
           << ") has empty " << fieldName << " field.\n";
      _errorCount++;
    }
    else if (_warning)
      cout << type << " node (" << node << ") has empty "
           << fieldName << " field.\n";
  }
  else if (_verbose) 
    cout << type << " node (" << node << ") has " << fieldName
         << " field = " << field << ".\n";
}

void TreeChecker::checkString (Node * node, string & field, 
			       const char * type, const char * fieldName, 
			       bool isError /*= true*/) {
  if (field == "") {
    if (isError) {
      cout << "Error: " << type << " node (" << node 
           << ") has empty " << fieldName << " field.\n";
      _errorCount++;
    }
    else if (_warning)
      cout << type << " node (" << node << ") has empty "
           << fieldName << " field.\n";
  }
  else if (_verbose) 
    cout << type << " node (" << node << ") has " << fieldName
         << " field = " << field << ".\n";
}

void TreeChecker::checkConstant (Node * node, constant & field, 
				 const char * type, const char * fieldName, 
				 bool isError /*= true*/) {
  if (field.no_val()) {
    if (isError) {
      cout << "Error: " << type << " node (" << node
           << ") has non-constant " << fieldName << " field.\n";
      _errorCount++;
    }
    else
      cout << type << " node (" << node << ") has non-constant "
           << fieldName << " field.\n";
  }
  else if (_verbose) 
    cout << type << " node (" << node << ") has " << fieldName
         << " field = " << field.to_string() << ".\n";
}

template <class T>
void TreeChecker::checkOperator(Node * node, Operator * field, 
				const char * type, const char * fieldName, 
				T check, string desc,
				bool isError /*= true*/) {
  if (! field) {
    if (isError) {
      cout << "Error: " << type << " node (" << node
           << ") has empty " << fieldName << " field.\n";
      _errorCount++;
    }
    else if (_warning)
      cout << type << " node (" << node << ") has empty "
           << fieldName << " field.\n";
  }
  else {
    if ( ! check(field) ) {
      cout << "Error: " << type << " node (" << node
	   << ") has non-" << desc << " " << fieldName << endl;
      _errorCount++;
    }
  }
}

template <class T>
void TreeChecker::checkFieldInList (Node * node, list<T> & field, 
				    const char * type, 
				    const char * fieldName,
				    int count, list<T> & collection, 
				    const char * itemName, 
				    bool isError /*= true*/) {
  if (field.empty()) {
    if (isError) {
      cout << "Error: " << type << " node (" << node
           << ") has empty " << fieldName << " field.\n";
      _errorCount++;
    }
    else if (_warning)
      cout << type << " node (" << node << ") has empty "
           << fieldName << " field.\n";
  }
  else {
    if (field.size() != count) {
      cout << "Error: " << type << " node (" << node
           << ") has incorrect count for the " 
           << fieldName << " field ";
      cout << "<" << field.size() << " != " << count << ">.\n";
      _errorCount++;

      typename list<T>::iterator lp;
      for (lp = field.begin() ; lp != field.end() ; lp++) {
        typename list<T>::iterator p;
        p = find(collection.begin(), collection.end(), (*lp));
        if (p == collection.end())
          cout << "  Invalid " << itemName << " node (" 
               << (*lp) << ")\n";
        else if (_verbose)
          cout << "  " << itemName << " node (" << (*lp) << ")\n";
      }
    }
    else {
      if (_verbose) {
        cout << type << " node (" << node << ") has " 
             << field.size() << " " << fieldName;
        cout << (field.size() > 1 ? "s:\n" : ":\n");
      }
      typename list<T>::iterator lp;
      for (lp = field.begin() ; lp != field.end() ; lp++) {
        typename list<T>::iterator p;
        p = find(collection.begin(), collection.end(), (*lp));
        if (p == collection.end()) {
          cout << "Error: " << type << " node (" << node 
               << ") has invalid " << itemName << " reference at "
               << (*lp) << ".\n";
          _errorCount++;
        }
        else if (_verbose) 
          cout << "  " << itemName << " node (" << (*lp) << ")\n";
      }
    }
  }
}

template <class T>
void TreeChecker::checkList (Node * node, list<T> & field, 
			     const char * type, 
			     const char * fieldName, 
			     bool isError /*= true*/) {
  if (field.empty()) {
    if (isError) {
      cout << "Error: " << type << " node (" << node
           << ") has empty " << fieldName << " field.\n";
      _errorCount++;
    }
    else if (_warning)
      cout << type << " node (" << node << ") has empty "
           << fieldName << " field.\n";
  }
  else if (_verbose) {
    cout << type << " node (" << node << ") has " << field.size()
         << " " << fieldName;
    cout << (field.size() > 1 ? "s:\n" : ":\n");
  }
}

template <class T>
void TreeChecker::checkFieldInList (Node * node, T field, const char * type, 
				    const char * fieldName, 
				    list<T> & collection, 
				    bool isError /*= true*/) {
  if (! field) {
    if (isError) {
      cout << "Error: " << type << " node (" << node 
           << ") has empty " << fieldName << " field.\n";
      _errorCount++;
    }
    else if (_warning)
      cout << type << " node (" << node << ") has empty "
           << fieldName << " field.\n";
  }
  else {
    typename list<T>::iterator lp;
    lp = find(collection.begin(), collection.end(), field);
    if (lp == collection.end()) {
      cout << "Error: " << type << " node (" << node << ") has invalid ";
      cout << fieldName << " reference at " << field << ".\n";
      _errorCount++;
    }
    else if (_verbose) 
      cout << type << " node (" << node << ") has " << fieldName
           << " field = " << field << ".\n";
  }
}

TreeChecker::TreeChecker () : 
  Walker(Both, Subtree), _preDismantled(false), _postDismantled(false),
  _verbose(false), _errorCount(0), _typeDepth(0), _in_proc(false)
{}

TreeChecker::TreeChecker (bool verbose, bool warning) : 
  Walker(Both, Subtree), _preDismantled(false), _postDismantled(false), 
  _verbose(verbose), _warning(warning), _errorCount(0), _typeDepth(0),
  _in_proc(false)
{}

TreeChecker::~TreeChecker () {
  _visitedNodes.clear();
  _gotos.clear();
  _labels.clear();
  _procs.clear();
  _dupProcs.clear();
  _switches.clear();
  _decls.clear();
  _gotoCount.clear();
  _formals.clear();
}

void TreeChecker::check() {
  for ( unit_list_p u = CBZ::Program.begin(); u != CBZ::Program.end(); u++ )
    check(*u);
}

void TreeChecker::check(unitNode * the_unit) {
  TreeChecker tc;
  the_unit->walk(tc);
}

void TreeChecker::at_node(Node * the_node, Order ord) {
  if ( ord == Preorder ) {
    checkTree(the_node, "Node");
  }
}

void TreeChecker::at_unit (unitNode * unit, Order ord) {
  if (ord == Preorder) {
    UnitWalker uw;
    unit->walk(uw);  // collect all proc and related nodes
    _procs = uw.procs();  // proc nodes
    _dupProcs = uw.dupProcs();  // re-declared proc nodes
    _formals = uw.formals();  // formal parameter count
  }
}

void TreeChecker::at_proc (procNode * proc, Order ord) {
  if (ord == Preorder) {
    ProcWalker pw;
    proc->walk(pw);  // collect all kind of nodes for this procedure
    _labels = pw.labels();
    _gotos = pw.gotos();
    _gotoCount = pw.gotoCount();
    _switches = pw.switches();
    _decls = pw.decls();
    _errorCount = 0;
    _preDismantled = false;
    _postDismantled = false;
    _in_proc = true;

    cout << "\n---- Begin: Procedure \"" << proc->decl()->name()
         << "\"----\n";

    // check for re-definition of procedure/function
    proc_list_p dup = find(_dupProcs.begin(), _dupProcs.end(), proc);
    if (dup != _dupProcs.end()) {  // found duplicates
      _errorCount++;
      cout << "Error: Procedure \"" << proc->decl()->name() 
           << "\" at (" << proc << ") is re-declared.\n";
    }
  }
  else { // ord == Postorder
    _in_proc = false;
    if (_preDismantled && _postDismantled) 
      cout << "Note: Procedure \"" << proc->decl()->name()
           << "\" contains both dismantled and non-dismantled Nodes.\n";
    cout << "---- End: Procedure \"" << proc->decl()->name()
         << "\"----\n====> ";
    if (_errorCount == 0) 
      cout << "Procedure \"" << proc->decl()->name()
           << "\" has no detected errors.\n";
    else { 
      cout << "Procedure \"" << proc->decl()->name() << "\" has "
           << _errorCount << " error" 
           << (_errorCount > 1 ? "s.\n" : ".\n");
    }
  }
}

void TreeChecker::at_decl(declNode * the_decl, Order ord) {
  if ( ord == Preorder ) {
    checkTree(the_decl, "Decl");
    // declarations of variables MUST have a type
    checkField(the_decl, the_decl->type(), "Decl", "type");
    if (arrayNode * the_array = dynamic_cast<arrayNode *>(the_decl->type())) {
      // declarations of array's in dismantled code must have 
      if ( ! the_array->dim() ) {
	setPreDismantled("Decl", the_decl);
      } else {
	the_array->dim()->eval();
	checkConstant(the_decl, the_array->dim()->value(), "Decl", "dim");
      }
    }
  }
}

// target node
void TreeChecker::at_case (caseNode * the_case, Order ord) {
  if (ord == Preorder) {
    setPreDismantled("Case", the_case);
    checkTree(the_case, "Case");
    // the expr field may be NULL (default case)
    checkField(the_case, the_case->expr(), "Case", "expr", false);
    if (the_case->expr()) {  // check for constant-ness
      the_case->expr()->eval();  // try to evaluate this expression
      checkConstant(the_case, the_case->expr()->value(), "Case", "expr");
    }
    checkField(the_case, the_case->stmt(), "Case", "statement");
    checkFieldInList(the_case, the_case->container(), "Case", "switch", 
		     _switches);
  }
}

void TreeChecker::at_label (labelNode * the_label, Order ord) {
  if (ord == Preorder) {
    checkTree(the_label, "Label");
    checkString(the_label, the_label->name(), "Label", "name");
    checkField(the_label, the_label->stmt(), "Label", "statement");
    // some label node may have empty references (gotos)
    checkFieldInList(the_label, the_label->references(), "Label", "reference", 
		     _gotoCount[the_label], _gotos, "goto", false);

    if (the_label->stmt()) {  // contains a block
      stmt_list stmts = the_label->stmt()->stmts();
      if (stmts.size() == 0) {
        if (_verbose || _warning)
          cout << "Label node (" << the_label << ") has an empty block.\n";
        setPreDismantled("Label", the_label);
      }
      else if (stmts.size() > 1) {
        if (_verbose || _warning)
          cout << "Label node (" << the_label
               << ") has a block with multiple statements.\n";
        setPreDismantled("Label", the_label);
      }
      else {
        stmtNode * stmt = stmts.front();  // get the only stmt
        if (stmt->typ() == Expr) {
          exprstmtNode * exprstmt = (exprstmtNode *) stmt;
          exprNode * expr = exprstmt->expr();
          if (expr) {
	    if (_verbose || _warning)
              cout << "Label node (" << the_label 
                << ") has a block with non-empty expression statement.\n";
            setPreDismantled("Label", the_label);
	  }
          else {
            if (_verbose || _warning)
              cout << "Label node (" << the_label
                   << ") has a block with an empty expression statement.\n";
	  }
	}
        else {
	  if (_verbose || _warning)
            cout << "Label node (" << the_label 
                 << ") has a block with non-expression statement.\n";
          setPreDismantled("Label", the_label);
        }
      }
    }
  }
}

// jump node
void TreeChecker::at_goto (gotoNode * the_goto, Order ord) {
  if (ord == Preorder) {
    checkTree(the_goto, "Goto");
    checkString(the_goto, the_goto->name(), "Goto", "name");
    checkFieldInList(the_goto, the_goto->label(), "Goto", "label", _labels);
  }
}

class ComparisonOperator : public OperatorCheck {
public:
  bool operator()(Operator * op) {
    return op->is_comparison();
  }
  string desc() { return string("comparison"); }
};

void TreeChecker::at_conditiongoto (conditiongotoNode * the_condgoto, 
				    Order ord) {
  if (ord == Preorder) {
    setPostDismantled("Condition goto", the_condgoto);
    checkTree(the_condgoto, "Condition goto");
    checkField(the_condgoto, the_condgoto->left(), "Condition goto", 
	       "left operand");
    checkField(the_condgoto, the_condgoto->right(), "Condition goto", 
	       "right operand");
    ComparisonOperator co;
    checkOperator(the_condgoto, the_condgoto->op(), "Condition goto",
		  "operator", co, co.desc());
  }
}

void TreeChecker::at_break (breakNode * the_break, Order ord) {
  if (ord == Preorder) {
    setPreDismantled("Break", the_break);
    checkTree(the_break, "Break");
    checkField(the_break, the_break->container(), "Break", "container");
  }
}

void TreeChecker::at_continue (continueNode * the_continue, Order ord) {
  if (ord == Preorder) {
    setPreDismantled("Continue", the_continue);
    checkTree(the_continue, "Continue");
    checkField(the_continue, the_continue->container(), "Continue", 
	       "container");
  }
}

void TreeChecker::at_return (returnNode * the_return, Order ord) {
  if (ord == Preorder) {
    checkTree(the_return, "Return");
    // the expr field may be NULL
    checkField(the_return, the_return->expr(), "Return", "expr", false);
    // dismantled code should only return idNode
    if (the_return->expr()) {
      if (the_return->expr()->typ() != Id) { 
        setPreDismantled("Return", the_return);
        if (_verbose || _warning) 
          cout << "Return node (" << the_return 
               << ") has non id return type.\n";
      }
      else if (_verbose || _warning) 
        cout << "Return node (" << the_return << ") has id return type.\n";
    }
    checkFieldInList(the_return, the_return->proc(), "Return", "proc", _procs);
  }
}

// selection node
void TreeChecker::at_if (ifNode * the_if, Order ord) {
  if (ord == Preorder) {
    setPreDismantled("If", the_if);
    checkTree(the_if, "If");
    checkField(the_if, the_if->expr(), "If", "expr");
    // TODO: can the true branch be NULL?
    checkField(the_if, the_if->true_br(), "If", "true-branch");
    // the false branch may be NULL
    checkField(the_if, the_if->false_br(), "If", "false-branch", false);
  }
}

void TreeChecker::at_switch (switchNode * the_switch, Order ord) {
  if (ord == Preorder) {
    setPreDismantled("Switch", the_switch);
    checkTree(the_switch, "Switch");
    checkField(the_switch, the_switch->expr(), "Switch", 
	       "conditional expression");
    checkField(the_switch, the_switch->stmt(), "Switch", "statements");
    checkList(the_switch, the_switch->cases(), "Switch", "cases");
    // TODO: should we check that cases who think they belong to the switch
    //       actually do belong?
  }
}

// loop node
void TreeChecker::at_for(forNode * the_for, Order ord) {
  if ( ord == Preorder ) {
    setPreDismantled("For-loop", the_for);
    checkTree(the_for, "For-loop");
    // the expr field may be NULL (empty for loop)
    checkField(the_for, the_for->cond(), "For-loop", "expr", false);
    checkField(the_for, the_for->body(), "For-loop", "body");
  }
}

void TreeChecker::at_loop (loopNode * loop, Order ord) {
  if (ord == Preorder) {
    setPreDismantled("Loop", loop);
    checkTree(loop, "Loop");
    checkField(loop, loop->cond(), "Loop", "expr");
    checkField(loop, loop->body(), "Loop", "body");
  }
}

// expr node
void TreeChecker::at_call (callNode * the_call, Order ord) {
  if (ord == Preorder) {
    if ( _typeDepth == 0 )
      setPreDismantled("Call", the_call);
    checkTree(the_call, "Call");
    checkField(the_call, the_call->name(), "Call", "name");
    // the argument list may be EMPTY
    checkList(the_call, the_call->args(), "Call", "argument list", false);
    // the procedure field may be NULL
    // TODO: is this only when name()->typ() != Id?
    checkField(the_call, the_call->proc(), "Call", "procedure", false);

    // Check that the actual arguments match 
    // the number of the formal parameters.
    if (the_call->name()) {
      if (the_call->name()->typ() == Id) {
        // get the argument counts for procedure with this name
        idNode * id = (idNode *) the_call->name();
	if ( _formals.find(id->name()) != _formals.end() ) {
	  if ( _formals[id->name()] != the_call->args().size() ) {
	    cout << "Error: Call node (" << the_call
		 << ") has incorrect number of arguments = " 
		 << the_call->args().size() << ".\n";
	    _errorCount++;
	  } else if (_verbose || _warning) {
	    cout << "Call node (" << the_call << ") has " 
		 << the_call->args().size() << " argument";
	    cout << ((the_call->args().size() > 1) ? "s.\n" : ".\n");
	  }
	}
      } else {
	// TODO: what about comparing to the type of the name()'s decl?
	//       aka: for function pointers
	cout << "Unable to check that the actual arguments for call node ("
	     << the_call << ") match the number of formal parameters." << endl;
      }
    }
    // TODO: Check that the actual arguments match the types of the 
    //       formal parameters (with type promotions).
  }
}

void TreeChecker::at_id (idNode * id, Order ord) {
  if (ord == Preorder) {
    checkTree(id, "Id");
    checkString(id, id->name(), "Id", "name");
    checkField(id, id->decl(), "Id", "decl");
    // checkFieldInList(id, id->decl(), "Id", "decl", _decls);
    // TODO: check that id is in the the id_list for the declNode.
  }
}

void TreeChecker::at_const (constNode * the_const, Order ord) {
  if (ord == Preorder) {
    checkTree(the_const, "Const");
    the_const->eval();  // try to evaluate the constant expression
    checkConstant(the_const, the_const->value(), "Const", "value");
  }
}

class UnaryOperator : OperatorCheck {
public:
  virtual bool operator()(Operator * op) { return op->is_unary(); }
  virtual string desc() { return string("unary"); }
};

void TreeChecker::at_unary (unaryNode * unary, Order ord) {
  if (ord == Preorder) {
    if ( _typeDepth == 0 )
      setPreDismantled("Unary", unary);
    checkTree(unary, "Unary");
    UnaryOperator uo;
    checkOperator(unary, unary->op(), "Unary", "operator", uo, uo.desc());
    if ( unary->op()->id() != Operator::SIZEOF ) {
      checkField(unary, unary->expr(), "Unary", "expr");
    } else {
      checkField(unary, unary->sizeof_type(), "Unary", "sizeof_type");
    }
  }
}

class BinaryOperator : public OperatorCheck {
public:
  bool operator()(Operator * op) {
    return op->is_binary();
  }
  string desc() { return string("binary"); }
};

void TreeChecker::at_binary (binaryNode * binary, Order ord) {
  if (ord == Preorder) {
    if ( _typeDepth == 0 )
      setPreDismantled("Binary", binary);
    checkTree(binary, "binary");
    BinaryOperator bo;
    checkOperator(binary, binary->op(), "Binary", "operator", bo, bo.desc());
    checkField(binary, binary->left(), "Binary", "left side");
    checkField(binary, binary->right(), "Binary", "right side");
  }
}

void TreeChecker::at_ternary (ternaryNode * ternary, Order ord) {
  if (ord == Preorder) {
    if ( _typeDepth == 0 )
      setPreDismantled("Ternary", ternary);
    checkTree(ternary, "Ternary");
    ComparisonOperator co;
//     checkOperator(ternary, ternary->cond(), "Ternary", "cond", co, co.desc());
    checkField(ternary, ternary->cond(), "Ternary", "cond");
    checkField(ternary, ternary->true_br(), "Ternary", "true branch");
    checkField(ternary, ternary->false_br(), "Ternary", "false branch");
  }
}

void TreeChecker::at_threeAddr (threeAddrNode * the_3addr, Order ord) {
  if (ord == Preorder) {
    setPostDismantled("Three address", the_3addr);
    checkTree(the_3addr, "Three address");
    // all the fields may be NULL or EMPTY depending on the actual node
    // TODO: is that true?  i think the rules are more complex

    // TODO:  check that lhs()->typ() is Id
    checkField(the_3addr, the_3addr->lhs(), "Three address", "lhs", false);
    Operator * op = the_3addr->op();
    if ( op ) {
      if ( op->id() == Operator::SIZEOF ) {
	checkField(the_3addr, the_3addr->sizeof_type(), "Three address", 
		   "sizeof_type");
      } else {
	checkField(the_3addr, the_3addr->rhs1(), "Three address", "rhs1");
	if ( op->id() == Operator::FUNC_CALL ) {
	  checkList(the_3addr, the_3addr->arg_list(), "Three address",
		    "arg_list", false);
	} else if ( op->is_dismantled_binary() ) {
	  checkField(the_3addr, the_3addr->rhs2(), "Three address", "rhs2");
	}
      }
    }
  }
}

void TreeChecker::at_operand (operandNode * operand, Order ord) {
  if (ord == Preorder) {
    setPostDismantled("Operand", operand);
    checkTree(operand, "Operand");
    checkField(operand, operand->var(), "Operand", "variable");
    // all the other fields may be NULL or EMPTY depending on the actual node
    // TODO:  are they necessary?
    checkField(operand, operand->cast(), "Operand", "typecast", false);
    checkField(operand, operand->index(), "Operand", "index", false);
    checkList(operand, operand->fields(), "Operand", "fields", false);
  }
}

void TreeChecker::at_cast (castNode * cast, Order ord) {
  if (ord == Preorder) {
    if ( _typeDepth == 0 )
      setPreDismantled("Cast", cast);
    checkTree(cast, "Cast");
    checkField(cast, cast->expr(), "Cast", "expression");
  }
}

void TreeChecker::at_comma (commaNode * comma, Order ord) {
  if (ord == Preorder) {
    if ( _typeDepth == 0 )
      setPreDismantled("Comma", comma);
    checkTree(comma, "Comma");
    checkList(comma, comma->exprs(), "Comma", "expression list");
  }
}

void TreeChecker::at_initializer (initializerNode * init, Order ord) {
  if (ord == Preorder) {
    if ( _arrayDepth == 0 )
      setPreDismantled("Initializer", init);
    checkTree(init, "Initializer");
    checkList(init, init->exprs(), "Initializer", "expression list");
  }
}

// type node
void TreeChecker::at_type (typeNode * the_type, Order ord) {
  if ( ord == Preorder ) {
    _typeDepth++;
  } else {
    _typeDepth--;
  }
}

void TreeChecker::at_func (funcNode * func, Order ord) {
  at_type(func, ord);
  if (ord == Preorder && _in_proc) {
    checkTree(func, "Function");
    checkField(func, func->returns(), "Function", "return");
    // TODO: actually should check that number of actual arguments
    //       matches that number of formal arguments
    checkFieldInList(func, func->args(), "Function", "argument list", 
		     func->args().size(), _decls, "argument", false);
  }
}

void TreeChecker::at_tdef (tdefNode * tdef, Order ord) {
  at_type(tdef, ord);
  if (ord == Preorder) {
    // TODO:  check with Sam, are these sufficient/correct?
    checkTree(tdef, "Typedef");
    checkString(tdef, tdef->name(), "Typedef", "name");
    checkField(tdef, tdef->def(), "Typedef", "def");
  }
}

void TreeChecker::at_array (arrayNode * array, Order ord) {
  at_type(array, ord);
  if (ord == Preorder) {
    _arrayDepth++;
    checkTree(array, "Array");
    checkField(array, array->dim(), "Array", "dimension expression", 
               false);  // array dimension may be NULL (in case of [])
    // actually, must be constant or constant expression
  } else {
    _arrayDepth--;
  }
}

UnitWalker::UnitWalker () : 
  Walker(Preorder, Subtree) 
{}

UnitWalker::~UnitWalker () { 
  _procs.clear();
  _procNames.clear();
  _dupProcs.clear();
  _formals.clear();
}

void UnitWalker::at_proc (procNode * the_proc, Order ord) {
  _procs.push_back(the_proc);
  declNode * proc_decl = the_proc->decl();
  if ( !proc_decl ) {
    cout << "procNode " << the_proc << " contains no declNode." << endl;
    return;
  }
  string procName = proc_decl->name();

  // get the formal argument count
  funcNode * proc_func = (funcNode *) proc_decl->type();
  if ( !proc_func ) {
    cout << "procNode " << the_proc << " w/ declNode " << proc_decl
	 << " does not have Func typ()." << endl;
    return;
  }
  int argc = proc_func->args().size();
  if (proc_func->is_void_args()) 
    argc = 0;

  // look for re-definition of procedure/function
  if (find(_procNames.begin(), _procNames.end(), procName)
      != _procNames.end()) {  // found duplicates
    _dupProcs.push_back(the_proc);  // re-defined
    return;  // quit
  }
  else _procNames.push_back(procName);  // collect procedure name

  // only insert once
  if ( _formals.find(procName) == _formals.end() )
    _formals[procName] = argc;
}
