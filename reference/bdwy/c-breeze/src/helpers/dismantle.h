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

#ifndef CBZ_DISMANTLE_H
#define CBZ_DISMANTLE_H

//
// dismantle.h
//
// class declarations for the dismantling changers
//

// some flags to control dismantling
// Use at your own risk; they haven't been fully tested, except in
// the default configuration.

// [] is used as a primitive operator

#define	INDEX_IS_PRIMITIVE		0x00000001

// expressions like **p are dismantled into tmp = *p; *tmp

#define NO_MULTIPLE_INDIRECTION		0x00000002

// &&, || and ?: are dismantled to if/goto

#define DISMANTLE_LOGICAL_TO_GOTOS	0x00000004

// if is a special case of if/else

#define TWO_JUMP_IF			0x00000008

// whiles become repeats

#define LOOP_INVERSION			0x00000010

// if (!(a<b)) becomes if (a>=b)

#define INVERT_CONDITIONALS		0x00000020

// if (a && b) becomes if (a) if (b)

#define CONVERT_LOGICALS_TO_IFS		0x00000040

#define DEFAULT_DISMANTLER_FLAGS	(NO_MULTIPLE_INDIRECTION \
					|DISMANTLE_LOGICAL_TO_GOTOS \
					|TWO_JUMP_IF \
					|CONVERT_LOGICALS_TO_IFS)

/* took out LOOP_INVERSION so branches would be more biased untaken */

// abstract class; all the dismantlers subclass off of this one.

class DismantleChanger: public Changer {

	// string to add to labels, making them unique
	// among different subclasses

	string	changer_tempid_extra;

public:
	unsigned int dismantle_flags;

	// prefix for this subclass used to indicate purpose of temp id

	char	*prefix;

	// an empty expression statement; usually used as the statement
	// for a label

	blockNode *clone_empty_stmt (void) {
		blockNode *b = new blockNode (NULL, NULL);
		b->stmts().push_back (new exprstmtNode (NULL));
		return b;
	}

	// get a new idNode for a temp var or label

	idNode *new_id (char *s = "") {
		string t = CBZ::get_temp_id_str();
		t += "__";
		t += changer_tempid_extra;
		t += s;
		idNode	*i = new idNode (t.c_str());
		return i;
	}
	
	// constructor

	DismantleChanger (Order ord, string extra, 
		unsigned int flags = DEFAULT_DISMANTLER_FLAGS) :
		Changer(ord, Subtree, false), 
		changer_tempid_extra(extra),
		dismantle_flags(flags) {}
};

// dismantle loops

class LoopDismantleChanger: public DismantleChanger {
private:
	labelNode * Return_label;
	idNode *return_val;
	returnNode *return_node;

        // handles return calls exclusively 
        Node * handle_return(blockNode * bl);
        
	// dismantle a while or for loop

	Node * dismantle_while_for_loop (whileNode *, labelNode *);
	Node * dismantle_while_for_loop_with_inversion (whileNode *, labelNode *);

	// dismantle a while loop

	Node * dismantle_while_loop (whileNode *);

	// dismantle a for loop

	Node * dismantle_for_loop (forNode *);

	// dismantle a do loop

	Node * dismantle_do_loop (doNode *);

	// changer entry points

	Node * at_loop (loopNode *, Order);
	Node * at_return (returnNode *, Order);
	Node * at_proc (procNode *, Order);

public:

	// dismantle one loop (public entry)

	Node * dismantle_loop (loopNode *);

	// return the complement of a conditional expression

	exprNode * Not (exprNode *);

	// change all breaks and continues to goto's
	// (public because it's used by other dismantlers)

	static stmtNode * fix_break_continue 
		(stmtNode *, labelNode *, labelNode *, bool);

	// The constructor for LoopDismantleChanger
	// will call the contructor for Changer
	// such that LoopDismantleChanger will visit
	// tree in Postorder.

	LoopDismantleChanger (unsigned int flags) : 
		DismantleChanger (Preorder, "L", flags) {}
};

// dismantle selection statements: if, if/else, switch/case

class SelectionDismantleChanger: public DismantleChanger {
	// dismantle if stmt

	Node * dismantle_if (ifNode *);
	Node * dismantle_ifelse (ifNode *);
	exprNode * negate_if_condition(exprNode *);

	// dismantle switch/case stmt

	Node * dismantle_switch (switchNode *);

	// changer entry points

	Node * at_selection (selectionNode *, Order);
	Node * at_case (caseNode *, Order);
	// Node * at_default (defaultNode *, Order);

public:

	// dismantle a selection stmt (public entry)

	Node * dismantle_selection (selectionNode *);

	SelectionDismantleChanger (unsigned int flags) : 
		DismantleChanger (Preorder, "S", flags) {}
};

class IfConverterChanger: public DismantleChanger {
	// dismantle if (a && b && ... && z) to if (a) if (b) ... if (z)
	Node * at_if (ifNode *, Order);
	Node * at_if_noelse (ifNode *, Order);

public:

	bool change;

	IfConverterChanger (unsigned int flags) :
		change(false),
		DismantleChanger (Preorder, "S", flags) {}
};

// dismantle "normal" initializer, i.e., to scalars
// also a good time to dismantle any labels that don't label empty
// stmts

class InitializerDismantleChanger: public DismantleChanger {
public:
	Node * at_block (blockNode *, Order);
	Node * at_label (labelNode *, Order); 
	InitializerDismantleChanger (unsigned int flags) : 
		DismantleChanger (Preorder, "", flags) {}
};

// by the time we reach this guy, the only expressions should be in
// expression statements (we hope?).  So it will be nice to do it from
// at_block(), so we can have a block into which to insert temps

class ExpressionDismantleChanger: public DismantleChanger {
	blockNode	*code;
	int		recursion_level;
	void emit_stmt (stmtNode *);
	void emit_decl (idNode *, typeNode *);
	void emit_expr (exprNode *);
	void emit_temp_assign (exprNode *);
        bool is_simple_expression(exprNode *);
        
	// return the id of a new declared variable the same type as this expr
	// return the id of a new declared variable of type int
	idNode *make_int (void);
	idNode *make_var (exprNode *);

public:
	Node * at_block (blockNode *, Order);
	exprNode * dismantle_expr (exprNode *);
	exprNode * dismantle_binary (binaryNode *);
	exprNode * dismantle_unary (unaryNode *);
	exprNode * dismantle_cast (castNode *);
	exprNode * dismantle_comma (commaNode *);
	exprNode * dismantle_ternary (ternaryNode *);
	exprNode * dismantle_call (callNode *);
	exprNode * dismantle_opeq (binaryNode *, unsigned int);
	ExpressionDismantleChanger (unsigned int flags) : 
		DismantleChanger (Postorder, "E", flags) {}
};

class FlattenDismantleChanger: public DismantleChanger {
private:
	int			_change;
	map <string,bool>	*labels;
	bool			unreachable;
        bool                    iterateflag;
	// find the target of a goto stmt

	stmt_list_p search_target (stmt_list_p, gotoNode *, stmt_list_p);

	// replace all occurences of one label with another in a list of stmts

	void search_and_replace_labels (stmt_list, stmt_list_p, stmt_list_p);

public:
        void setflag(bool val){iterateflag = val;}
        bool getflag(){return iterateflag;}
	Node * at_label (labelNode *, Order);
	Node * at_proc (procNode *, Order);
	Node * at_unary (unaryNode *, Order);
	void flatten_block (blockNode *, blockNode *);
	int change (void) { return _change; }

	FlattenDismantleChanger (map<string,bool> *l, unsigned int flags) : 
		DismantleChanger (Both, "", flags),
		_change(0), labels(l) {}

};

class UsedLabelWalker: public Walker {
private:

	map<string,bool>	*_labels;

public:

	map<string,bool> * labels (void) {
		return _labels;
	}

	void at_goto (gotoNode *p, Order) {
		(*_labels)[p->name()] = true;
	}

	UsedLabelWalker (void): Walker (Preorder, Subtree) {
		_labels = new map<string,bool>;
	}
};


class Dismantle {
public:
	static void dismantle (unitNode *, unsigned int flags = DEFAULT_DISMANTLER_FLAGS);
};


class RemoveConstChanger: public Changer {
 public:
   RemoveConstChanger(void):       // constructor
     Changer (Preorder, Subtree, false) {}
 
   Node * at_type (typeNode *n, Order ord) {
     // remove the constant Type_qualifier for this node
     // don't bother checking for const, 
     // if not const, will have no effect.
 
     n->remove_type_qualifiers(typeNode::CONST);
     return n;
   }
 
 };
 

// annotation for nodes that say where they came from, so
// later passes can have a sense of what kind of higher level
// construct is involved.
//
// Most if's will have "from_if", but some will have "from_for_test1", etc.
//

#define FROM_FOR_TEST1		1
#define FROM_FOR_TEST2		2
#define FROM_WHILE_TEST1	3
#define FROM_WHILE_TEST2	4
#define FROM_DO			5
#define FROM_WHILE		6
#define FROM_IF			7
#define FROM_IFELSE		8
#define FROM_SWITCH		9
#define FROM_TERNARY		10
#define FROM_AND1		11
#define FROM_AND2		12
#define FROM_OR1		13
#define FROM_OR2		14
#define FROM_MANY_CASES		15

class fromAnnote : public Annote {
private:
	int	_from;
public:
	fromAnnote (int f) : _from(f) { }

	int from (void) { return _from; }
	void from (int f) { _from = f; }
	bool is_annote_kind (string & name) {
		switch (_from) {
		case FROM_FOR_TEST1: return name == "from_for_test1";
		case FROM_FOR_TEST2: return name == "from_for_test2";
		case FROM_WHILE_TEST1: return name == "from_while_test1";
		case FROM_WHILE_TEST2: return name == "from_while_test2";
		case FROM_DO: return name == "from_do";
		case FROM_IF: return name == "from_if";
		case FROM_IFELSE: return name == "from_ifelse";
		case FROM_SWITCH: return name == "from_switch";
		case FROM_TERNARY: return name == "from_ternary";
		case FROM_AND1: return name == "from_and1";
		case FROM_AND2: return name == "from_and2";
		case FROM_OR1: return name == "from_or1";
		case FROM_OR2: return name == "from_or2";
		case FROM_MANY_CASES: return name == "from_many_cases";
		default: return false;
		}
	}

	// search a node's annotation list for something

	static bool find_from (Node *n, string s) {
		annote_list_p a;
		for (a=n->annotations().begin(); 
			a!=n->annotations().end(); a++) {
			if ((*a)->is_annote_kind (s)) return true;
		}
		return false;
	}
};

#endif // CBZ_DISMANTLE_H
