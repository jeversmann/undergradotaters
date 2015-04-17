
// ----------------------------------------------------------------------
//
//  J-Breeze
//  Java Compiler Framework
// 
//  Copyright (c) 2001 University of Texas at Austin
// 
//  Teck B. Tok
//  Samuel Z. Guyer
//  Daniel A. Jimenez
//  Calvin Lin
// 
//  Permission is hereby granted, free of charge, to any person
//  obtaining a copy of this software and associated documentation
//  files (the "Software"), to deal in the Software without
//  restriction, including without limitation the rights to use, copy,
//  modify, merge, publish, distribute, and/or sublicense copies
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
//  J-Breeze extends from C-Breeze (copyright University of Texas at
//  Austin), part of whose design is inspired by the C-to-C Translator
//  from MIT Laboratory for Computer Science.
//
// ----------------------------------------------------------------------

#include "vcg.h"

class vcgASTWalker : public vcgWalker {

  typedef pair<Node*, string>  Edge;  // <target node, name of edge>
  typedef list<Edge>           Edges;
  typedef Edges::iterator      Edges_p;
  typedef map<Node*,Edges>     Edge_map;

  node_list  nodes;  // nodes that are printed.
  Edge_map   edges;  // to avoid duplicate edges; printed is delayed, so that
                     // only whose nodes are printed that an edge is printed.

public:
  vcgASTWalker(ostream& ostr, const string& comment, string excludefilename)
    : vcgWalker(ostr, comment, excludefilename) {
    nodes.clear();  edges.clear();  
    // Start graph description
    print_comment();
    print_comment( "=== J-Breeze AST visualization ===" );
    print_comment();
    if( !comment.empty() ) {
      print_comment( comment );
      print_comment();
    }

    start_graph();
    print_graph_attribute( "orientation", "left_to_right" );
    print_graph_attribute( "display_edge_labels", "yes");
  } // vcgASTWalker

  virtual ~vcgASTWalker(void) { finalize_graph(); } // finalize_graph is called

///////////////////////////////////////////////////////////
private:
  virtual void finalize_graph() {
    delayed_print_edge();
    // Close the graph.
    //graph << endl;
    //graph << "} // end of graph" << endl;
  } // finalize_graph

  void print_node(Node *n) {
    // has the node been printed?
    if(find(nodes.begin(), nodes.end(), n) != nodes.end()) return;
#ifndef J_BREEZE
    if(n->typ()!=Unit && excluded(n->coord())) return;
#endif

    nodes.push_front(n);

    graph << endl;
    graph << "   node: {" << endl;
    print_node_attribute( "title", node_title(n));
    print_node_attribute( "label", node_label(n));
    switch(n->typ()) { // more node attributes
      // expr
      case Const: case Id: case Binary: case Unary: case Cast: case Comma:
      case Ternary: /*case Index:*/ case Call: case Initializer:
      // case ImplicitCast:
      case Expr:
#ifdef J_BREEZE
      case QualifiedName: case Special: case New: case AccessClass:
      case InstanceOf:
#endif
        print_node_value( "shape", "ellipse" ); break;
      // statements
      case Label: case Switch: case Case: /*case Default:*/ case If:
      // case IfElse:
      case While: case Do: case For: case Goto: case Continue: case Break:
      case Return: case Block:
#ifdef J_BREEZE
      case DeclStmt: case ForInit: case Break_Id: case Continue_Id: case Throw:
      case Synchronized: case Try: case Catch: case Finally:
#endif
        print_node_value( "shape", "box" ); break;
      // types
      case Prim: case Tdef: case Ptr: case Array: case Func: case Struct:
      case Union: case Enum: case sueSpec:
#ifdef J_BREEZE
      case Class: case Interface: case TypeName:
#endif
        print_node_value( "shape", "rhomb" );
        print_node_value( "textcolor", "green" );
        print_node_value( "bordercolor", "green" ); break;
      // defs
      case Decl: case Proc: case Unit:
#ifdef J_BREEZE
      case FieldVar: case Method: case Constructor: case FieldInit:
      case MemberType: case Package: case Import:
#endif
        print_node_value( "shape", "triangle" );
        print_node_value( "textcolor", "blue" );
        print_node_value( "bordercolor", "blue" ); break;
      // misc
      case Text: case Undeclared: case Meta: case Attrib:
        print_node_value( "shape", "triangle" );
        print_node_value( "textcolor", "red" );
        print_node_value( "bordercolor", "red" ); break;
      default: assert(false);
    }
    graph << "   }" << endl;
  } // delayed_print_node

  string node_title(Node *n) {
    ostringstream title;
    char hex[16];
    sprintf(hex, "(%x)\0", n);
    title << node_name(n) << '@' << n->coord() << '.' << n->coord().offset()
          << hex << '\0';
    return title.str();
  } // node_title

  string node_label(Node *n) {
    ostringstream label;
    Coord coord = n->coord();
    label << node_name(n) << ':' << coord.line() << '.' << coord.offset()
          << '\0';
    return label.str();
  } // node_label

  void print_edge(Node *from, Node *to, string name) {
    if(!from || !to) return;
#ifndef J_BREEZE
    if((from->typ()!=Unit && excluded(from->coord()))
       || excluded(to->coord())) return;
#endif
    Edges targets=edges[from];
    // has the edge been printed?
    for(Edges_p e=targets.begin(); e!=targets.end(); e++)
      if((*e).first == to) return;
    edges[from].push_front( Edge(to,name) );
  }

  void delayed_print_edge(void) {
    for(Edge_map::iterator es=edges.begin(); es!=edges.end(); es++) {
      Node *from = (*es).first;
      for(Edges_p e=(*es).second.begin(); e!=(*es).second.end(); e++) {
        Node *to = (*e).first;
        // if node is not in nodes, do not print the edge
        if(find(nodes.begin(), nodes.end(), to)==nodes.end()) continue;

        graph << endl;
        graph << "   edge: {";
        graph << "  sourcename: \"" << node_title(from) << "\"";
        graph << "  targetname: \"" << node_title(to)   << "\"";
        graph << "  label: \"" << (*e).second << "\"";
        graph << "  }" << endl;
      }
    }
  } // print_edge

#define STRING(s1,s2) (string(s1 + string(" `") + s2 + "'"))

  string node_name(Node *n) {
    if(!n) return string("");
    switch(n->typ()) { // more node attributes
      // expr
      case Const: {
        constant v = ((constNode*)n)->value();
        string s = v.to_string();
        if(v.is_str()) { s.erase(0,1);  s.erase(s.length()-1,1); }
        return STRING("Const", s);
      }
      case Id:
        return STRING("Id", ((idNode*)n)->name());
      case Binary:
        return STRING("Binary", ((binaryNode*)n)->op()->print());
      case Unary:
        return STRING("Unary", ((unaryNode*)n)->op()->print());
      case Cast:
#ifdef J_BREEZE
        return STRING("Cast", typenameNode::type_name(((castNode*)n)->type()));
#else
        return string("Cast");
#endif
      case Comma: return string("Comma");
      case Ternary: return string("Ternary");
      /*case Index:
#ifdef J_BREEZE
        return STRING("Index", qnameNode::expand_name(((indexNode*)n)->name()));
#else
        return string("Index");
#endif
*/
#ifdef J_BREEZE
      case Call:
        return STRING("Call", qnameNode::expand_name(((mcallNode*)n)->name()));
#else
      case Call: return string("Call");
#endif
      case Initializer: return string("Initializer");
      //case ImplicitCast: return string("Implicitcast");
#ifdef J_BREEZE
      case QualifiedName: return string("QualifiedName");
        //return STRING("QualifiedName",((qnameNode*)n)->expand_name());
      case Special: return STRING("Special", (((specialNode*)n)->text()));
      case New:
        return STRING("New", typenameNode::type_name(((newNode*)n)->type()));
      case AccessClass:
        return STRING("AccessClass",
                      typenameNode::type_name(((classLiteralNode*)n)->type()));
      case InstanceOf: return string("InstanceOf");
#endif
      case Expr: return string("Expr");
      // statements
      case Label: return STRING("Label", ((labelNode*)n)->name());
      case Switch: return string("Switch");
      case Case:
#ifdef J_BREEZE
        return STRING("Case", qnameNode::expand_name(((caseNode*)n)->expr()));
#else
        return string("Case");
#endif
      //case Default: return string("Default");
      case If: return string("If");
      //case IfElse: return string("Ifelse");
      case While: return string("While");
      case Do: return string("Do");
      case For: return string("For");
#ifndef J_BREEZE
      case Goto: return STRING("Goto", ((gotoNode*)n)->name());
#endif
      case Continue: return string("Continue");
      case Break: return string("Break");
      case Return: return string("Return");
      case Block: return string("Block");
#ifdef J_BREEZE
      case DeclStmt: return string("DeclStmt");
      case ForInit: return string("ForInit");
      case Break_Id:
        return STRING("Break_Id", ((break_idNode*)n)->name());
      case Continue_Id:
        return STRING("Continue_Id", ((continue_idNode*)n)->name());
      case Throw: return string("Throw");
      case Synchronized:
        return STRING("Synchronized",
                      qnameNode::expand_name(((synchNode*)n)->expr()));
      case Try: return string("Try");
      case Catch: return string("Catch");
        /*return STRING("Catch",
                      ((catchNode*)n)->to_catch()->name()); */
      case Finally: return string("finally");
#endif
      // types
      case Prim: return STRING("Prim", (((primNode*)n)->basic().to_string()));
      case Tdef: return STRING("Tdef", ((tdefNode*)n)->name());
      case Ptr: return string("Ptr");
      case Array:
#ifdef J_BREEZE
        return STRING("Array",typenameNode::type_name((arrayNode*)n));
#else
        return string("Array");
#endif
      case Func: return string("Func");
#ifndef J_BREEZE
      case Struct: return string("Struct");
      case Union: return string("Union");
      case Enum: return string("Enum");
      case sueSpec: return STRING("sueSpec", (((suespecNode*)n)->name()));
#else
      case Class: return STRING("Class", ((classNode*)n)->name());
      case Interface: return STRING("Interface", ((interfaceNode*)n)->name());
      case TypeName:
        return STRING("TypeName",
                      qnameNode::expand_name(((typenameNode*)n)->name()));
#endif
      // defs
      case Decl: return STRING("Decl", ((declNode*)n)->name());
      case Unit: return STRING("Unit", ((unitNode*)n)->input_file());
#ifndef J_BREEZE
      case Proc: return STRING("Proc", ((procNode*)n)->decl()->name());
#else
      case FieldVar: return string("FieldVar");
      case Method: return STRING("Method", ((methodNode*)n)->decl()->name());
      case Constructor: return string("Constructor");
      case FieldInit: return string("FieldInit");
      case MemberType: return string("MemberType");
      case Package: return string("Package");
      case Import: return string("Import");
#endif
      // misc
#ifndef J_BREEZE
      case Text: return string("Text"); //STRING("Text",((textNode*)n)->text());
      case Undeclared: return string("Undeclared");
      case Meta: return string("Meta");
      case Attrib: return STRING("Attrib", ((attribNode*)n)->name());
#endif
      default: assert(false);
    }
  } // node_name

  template< class S, class T >
  void print_edges(S parent, list< T > &children, string name) {
    for (list< T >::iterator c=children.begin(); c!=children.end(); ++c)
      print_edge(parent, *c, name);
  } // print_edges

  void at_unit(unitNode * the_unit, Order ord) {
    if (ord == Postorder) return;
    print_node(the_unit);
#ifndef J_BREEZE
    print_edges(the_unit, the_unit->undef_funcs(), "undef_funcs");
    print_edges(the_unit, the_unit->suespecs(), "suspecs");
#endif
    print_edges(the_unit, the_unit->defs(), "defs");
  } // at_unit

  void at_decl(declNode * the_decl, Order ord) {
    if (ord == Postorder || currently_excluded) return;
    print_node(the_decl);
    typeNode *type = the_decl->type();
#ifdef J_BREEZE
    if((type->typ()!=Class && type->typ()!=Interface) ||
       !excluded(type))
#endif
      print_edge(the_decl, type, "type");
    print_edge(the_decl, the_decl->init(), "init");
#ifndef J_BREEZE
    print_edge(the_decl, the_decl->bitsize(), "bitsize");
#endif
  } // at_decl

#ifndef J_BREEZE
  void at_proc(procNode * the_proc, Order ord) {
    if(ord==Postorder) currently_excluded = false;
    else               currently_excluded = excluded(the_proc);
    if (ord == Postorder || currently_excluded) return;
    print_node(the_proc);
    print_edge(the_proc, the_proc->decl(), "decl");
    print_edge(the_proc, the_proc->body(), "body");
  } // at_proc
#endif

  void at_prim(primNode * the_prim, Order ord) {
    if (ord == Postorder || currently_excluded) return;
    print_node(the_prim);
  } // at_prim

  void at_tdef(tdefNode * the_tdef, Order ord) {
    if (ord == Postorder || currently_excluded) return;
    print_node(the_tdef);
  } // at_tdef

#ifndef J_BREEZE
  void at_ptr(ptrNode * the_ptr, Order ord) {
    if (ord == Postorder || currently_excluded) return;
    print_node(the_ptr);
    print_edge(the_ptr, the_ptr->type(), "type");
  } // at_ptr
#endif

  void at_array(arrayNode * the_array, Order ord) {
    if (ord == Postorder || currently_excluded) return;
    print_node(the_array);
    print_edge(the_array, the_array->type(), "type");
    print_edge(the_array, the_array->dim(), "dim");
  } // at_array

  void at_func(funcNode * the_func, Order ord) {
    if (ord == Postorder || currently_excluded) return;
    print_node(the_func);
    print_edges(the_func, the_func->args(), "args");
    print_edge(the_func, the_func->returns(), "returns");
  } // at_func

#ifndef J_BREEZE
  void at_sue(sueNode * the_sue, Order ord) {
    if (ord == Postorder || currently_excluded) return;
    print_node(the_sue);
  } // at_sue

  void at_suespec(suespecNode * the_suespec, Order ord) {
    if (ord == Postorder || currently_excluded) return;
    print_node(the_suespec);
    print_edges(the_suespec, the_suespec->fields(), "fields");
  } // at_suespec
#endif

  void at_const(constNode * the_const, Order ord) {
    if (ord == Postorder || currently_excluded) return;
    print_node(the_const);
    print_edge(the_const, the_const->type(), "type");
  } // at_const

  void at_id(idNode * the_id, Order ord) {
    if (ord == Postorder || currently_excluded) return;
    print_node(the_id);
    print_edge(the_id, the_id->type(), "type");
  } // at_id

  void at_binary(binaryNode * the_binary, Order ord) {
    if (ord == Postorder || currently_excluded) return;
    print_node(the_binary);
    print_edge(the_binary, the_binary->type(), "type");
    print_edge(the_binary, the_binary->left(), "left");
    print_edge(the_binary, the_binary->right(), "right");
    // hack, due to binarynode.cc
    if(the_binary->right() &&
       (the_binary->op()->id()=='.' ||
#ifdef J_BREEZE
        the_binary->op()->id()==Operator::opARROW
#else
        the_binary->op()->id()==Operator::ARROW
#endif
       ))
      the_binary->right()->walk(*this);
  } // at_binary

  void at_unary(unaryNode * the_unary, Order ord) {
    if (ord == Postorder || currently_excluded) return;
    print_node(the_unary);
    print_edge(the_unary, the_unary->expr(), "expr");
    print_edge(the_unary, the_unary->type(), "type");
#ifndef J_BREEZE
    print_edge(the_unary, the_unary->sizeof_type(), "sizeof_type");
#endif
  } // at_unary

  void at_cast(castNode * the_cast, Order ord) {
    if (ord == Postorder || currently_excluded) return;
    print_node(the_cast);
    print_edge(the_cast, the_cast->type(), "type");
    print_edge(the_cast, the_cast->expr(), "expr");
  } // at_cast

  void at_comma(commaNode * the_comma, Order ord) {
    if (ord == Postorder || currently_excluded) return;
    print_node(the_comma);
    print_edges(the_comma, the_comma->exprs(), "exprs");
    print_edge(the_comma, the_comma->type(), "type");
  } // at_comma

  void at_ternary(ternaryNode * the_ternary, Order ord) {
    if (ord == Postorder || currently_excluded) return;
    print_node(the_ternary);
    print_edge(the_ternary, the_ternary->cond(), "cond");
    print_edge(the_ternary, the_ternary->true_br(), "true_br");
    print_edge(the_ternary, the_ternary->false_br(), "false_br");
    print_edge(the_ternary, the_ternary->type(), "type");
  } // at_ternary

  /*void at_index(indexNode * the_index, Order ord) {
    if (ord == Postorder || currently_excluded) return;
    print_node(the_index);
    print_edge(the_index, the_index->name(), "name");
    print_edge(the_index, the_index->dim(), "dim");
    print_edge(the_index, the_index->type(), "type");
  } // at_index */

#ifndef J_BREEZE
  void at_call(callNode * the_call, Order ord) {
    if (ord == Postorder || currently_excluded) return;
    print_node(the_call);
    print_edge(the_call, the_call->name(), "name");
    print_edges(the_call, the_call->args(), "args");
    print_edge(the_call, the_call->type(), "type");
  } // at_call
#else
  void at_mcall(mcallNode * the_call, Order ord) {
    if (ord == Postorder || currently_excluded) return;
    print_node(the_call);
    print_edge(the_call, the_call->name(), "name");
    print_edges(the_call, the_call->args(), "args");
    print_edge(the_call, the_call->type(), "type");
  } // at_mcall
#endif

  void at_initializer(initializerNode * the_initializer, Order ord) {
    if (ord == Postorder || currently_excluded) return;
    print_node(the_initializer);
    print_edges(the_initializer, the_initializer->exprs(), "exprs");
    print_edge(the_initializer, the_initializer->type(), "type");
  } // at_initializer

  /*void at_implicitcast(implicitcastNode * the_implicitcast, Order ord) {
    if (ord == Postorder || currently_excluded) return;
    print_node(the_implicitcast);
    print_edge(the_implicitcast, the_implicitcast->type(), "type");
    print_edge(the_implicitcast, the_implicitcast->expr(), "expr");
  } // at_implicitcast */

  void at_block(blockNode * the_block, Order ord) {
    if (ord == Postorder || currently_excluded) return;
    print_node(the_block);
    print_edges(the_block, the_block->decls(), "decls");
    print_edges(the_block, the_block->stmts(), "stmts");
  } // at_block

  void at_exprstmt(exprstmtNode * the_exprstmt, Order ord) {
    if (ord == Postorder || currently_excluded) return;
    print_node(the_exprstmt);
    print_edge(the_exprstmt, the_exprstmt->expr(), "expr");
  } // at_exprstmt

  void at_label(labelNode * the_label, Order ord) {
    if (ord == Postorder || currently_excluded) return;
    print_node(the_label);
    print_edge(the_label, the_label->stmt(), "stmt");
  } // at_label

  void at_case(caseNode * the_case, Order ord) {
    if (ord == Postorder || currently_excluded) return;
    print_node(the_case);
    print_edge(the_case, the_case->expr(), "expr");
    print_edge(the_case, the_case->stmt(), "stmt");
  } // at_case

  /*void at_default(defaultNode * the_default, Order ord) {
    if (ord == Postorder || currently_excluded) return;
    print_node(the_default);
    print_edge(the_default, the_default->stmt(), "stmt");
  } // at_default */

  void at_if(ifNode * the_if, Order ord) {
    if (ord == Postorder || currently_excluded) return;
    print_node(the_if);
    print_edge(the_if, the_if->expr(), "expr");
    print_edge(the_if, the_if->true_br(), "true_br");
    print_edge(the_if, the_if->false_br(), "false_br");
  } // at_if

  /*void at_ifelse(ifelseNode * the_ifelse, Order ord) {
    if (ord == Postorder || currently_excluded) return;
    print_node(the_ifelse);
    print_edge(the_ifelse, the_ifelse->expr(), "expr");
    print_edge(the_ifelse, the_ifelse->true_br(), "true_br");
    print_edge(the_ifelse, the_ifelse->false_br(), "false_br");
  } // at_ifelse */

  void at_switch(switchNode * the_switch, Order ord) {
    if (ord == Postorder || currently_excluded) return;
    print_node(the_switch);
    print_edge(the_switch, the_switch->expr(), "expr");
    print_edge(the_switch, the_switch->stmt(), "stmt");
  } // at_switch

  void at_while(whileNode * the_while, Order ord) {
    if (ord == Postorder || currently_excluded) return;
    print_node(the_while);
    print_edge(the_while, the_while->cond(), "cond");
    print_edge(the_while, the_while->body(), "body");
  } // at_while

  void at_do(doNode * the_do, Order ord) {
    if (ord == Postorder || currently_excluded) return;
    print_node(the_do);
    print_edge(the_do, the_do->body(), "body");
    print_edge(the_do, the_do->cond(), "cond");
  } // at_do

  void at_for(forNode * the_for, Order ord) {
    if (ord == Postorder || currently_excluded) return;
    print_node(the_for);
    print_edge(the_for, the_for->init(), "init");
    print_edge(the_for, the_for->cond(), "cond");
    print_edge(the_for, the_for->next(), "next");
    print_edge(the_for, the_for->body(), "body");
  } // at_for

#ifndef J_BREEZE
  void at_goto(gotoNode * the_goto, Order ord) {
    if (ord == Postorder || currently_excluded) return;
    print_node(the_goto);
  } // at_goto
#endif

  void at_continue(continueNode * the_continue, Order ord) {
    if (ord == Postorder || currently_excluded) return;
    print_node(the_continue);
  } // at_continue

  void at_break(breakNode * the_break, Order ord) {
    if (ord == Postorder || currently_excluded) return;
    print_node(the_break);
  } // at_break

  void at_return(returnNode * the_return, Order ord) {
    if (ord == Postorder || currently_excluded) return;
    print_node(the_return);
    print_edge(the_return, the_return->expr(), "expr");
  } // at_return

#ifndef J_BREEZE
  void at_attrib(attribNode * the_attrib, Order ord) {
    if (ord == Postorder || currently_excluded) return;
    print_node(the_attrib);
    print_edge(the_attrib, the_attrib->arg(), "arg");
  } // at_attrib

  void at_text(textNode * the_text, Order ord) {
    if (ord == Postorder || currently_excluded) return;
    print_node(the_text);
  } // at_text
#endif

#ifdef J_BREEZE
  void at_class(classNode * the_class, Order ord) {
    if(ord==Preorder) current_type.push_front(the_class);
    else              current_type.pop_front();
    if(current_type.empty())
      currently_excluded = false;
    else
      currently_excluded = excluded(current_type.front());
    if (ord == Postorder || currently_excluded) return;
    print_node(the_class);
    print_edge(the_class, the_class->extend(), "extend");
    print_edges(the_class, the_class->interfaces(), "interfaces");
    print_edges(the_class, the_class->fields(), "fields");
  } // at_class
 
  void at_interface(interfaceNode * the_interface, Order ord) {
    if(ord==Preorder) current_type.push_front(the_interface);
    else              current_type.pop_front();
    if(current_type.empty())
      currently_excluded = false;
    else
      currently_excluded = excluded(current_type.front());
    if (ord == Postorder || currently_excluded) return;
    print_node(the_interface);
    print_edges(the_interface, the_interface->extends(), "extends");
    print_edges(the_interface, the_interface->fields(), "fields");
  } // at_interface
 
  void at_typename(typenameNode * the_tn, Order ord) {
    if (ord == Postorder || currently_excluded) return;
    print_node(the_tn);
    print_edge(the_tn, the_tn->name(), "name");
  } // at_typename
 
  void at_fieldvar(fieldvarNode * the_fieldvar, Order ord) {
    if (ord == Postorder || currently_excluded) return;
    print_node(the_fieldvar);
    print_edges(the_fieldvar, the_fieldvar->decls(), "decls");
  } // at_fieldvar
 
  void at_method(methodNode * the_method, Order ord) {
    if (ord == Postorder || currently_excluded) return;
    print_node(the_method);
    print_edge(the_method, the_method->decl(), "decl");
    print_edges(the_method, the_method->throws(), "throws");
    print_edge(the_method, the_method->body(), "body");
  } // at_method
 
  void at_constructor(constructorNode * the_constructor, Order ord) {
    if (ord == Postorder || currently_excluded) return;
    print_node(the_constructor);
    print_edge(the_constructor, the_constructor->decl(), "decl");
    print_edges(the_constructor, the_constructor->throws(), "throws");
    print_edge(the_constructor, the_constructor->body(), "body");
  } // at_constructor
 
  void at_fieldinit(fieldinitNode * the_init, Order ord) {
    if (ord == Postorder || currently_excluded) return;
    print_node(the_init);
    print_edge(the_init, the_init->body(), "body");
  } // at_fieldinit

  void at_membertype(membertypeNode * the_membertype, Order ord) {
    if (ord == Postorder || currently_excluded) return;
    print_node(the_membertype);
    print_edge(the_membertype, the_membertype->member(), "membertype");
  } // at_membertype
 
  void at_declstmt(declstmtNode * the_declstmt, Order ord) {
    if (ord == Postorder || currently_excluded) return;
    print_node(the_declstmt);
    print_edges(the_declstmt, the_declstmt->decls(), "decls");
  } // at_declstmt
 
  void at_forinit(forinitNode * the_forinit, Order ord) {
    if (ord == Postorder || currently_excluded) return;
    print_node(the_forinit);
    print_edge(the_forinit, the_forinit->init(), "init");
    print_edge(the_forinit, the_forinit->cond(), "cond");
    print_edge(the_forinit, the_forinit->next(), "next");
    print_edge(the_forinit, the_forinit->body(), "body");
  } // at_forinit
 
  void at_break_id(break_idNode * the_break_id, Order ord) {
    if (ord == Postorder || currently_excluded) return;
    print_node(the_break_id);
  } // at_break_id
 
  void at_continue_id(continue_idNode * the_continue_id, Order ord) {
    if (ord == Postorder || currently_excluded) return;
    print_node(the_continue_id);
  } // at_continue_id
 
  void at_throw(throwNode * the_throw, Order ord) {
    if (ord == Postorder || currently_excluded) return;
    print_node(the_throw);
    print_edge(the_throw, the_throw->expr(), "expr");
  } // at_throw
 
  void at_synch(synchNode * the_synch, Order ord) {
    if (ord == Postorder || currently_excluded) return;
    print_node(the_synch);
    print_edge(the_synch, the_synch->expr(), "expr");
    print_edge(the_synch, the_synch->body(), "body");
  } // at_synch
 
  void at_try(tryNode * the_try, Order ord) {
    if (ord == Postorder || currently_excluded) return;
    print_node(the_try);
    print_edge(the_try, the_try->stmts(), "stmts");
    print_edges(the_try, the_try->catches(), "catches");
    print_edge(the_try, the_try->finally(), "finally");
  } // at_try

  void at_catch(catchNode * the_catch, Order ord) {
    if (ord == Postorder || currently_excluded) return;
    print_node(the_catch);
    print_edge(the_catch, the_catch->to_catch(), "to_catch");
    print_edge(the_catch, the_catch->body(), "body");
  } // at_catch
 
  void at_finally(finallyNode * the_finally, Order ord) {
    if (ord == Postorder || currently_excluded) return;
    print_node(the_finally);
    print_edge(the_finally, the_finally->block(), "block");
  } // at_finally
 
  void at_qualifiedname(qnameNode * the_qname, Order ord) {
    if (ord == Postorder || currently_excluded) return;
    print_node(the_qname);
    print_edge(the_qname, the_qname->left(), "left");
    print_edge(the_qname, the_qname->right(), "right");
  } // at_qualifiedname
 
  void at_special(specialNode * the_special, Order ord) {
    if (ord == Postorder || currently_excluded) return;
    print_node(the_special);
  } // at_special
 
  void at_new(newNode * the_new, Order ord) {
    if (ord == Postorder || currently_excluded) return;
    print_node(the_new);
    print_edge(the_new, the_new->type(), "type");
    if(the_new->newtype()==newNode::Array)
      print_edge(the_new, the_new->array_init(), "array_init");
    else {
      print_edges(the_new, the_new->args(), "args");
      print_edge(the_new, the_new->anonymous(), "anonymous");
    }
  } // at_new
 
  void at_classLiteral(classLiteralNode * the_ac, Order ord) {
    if (ord == Postorder || currently_excluded) return;
    print_node(the_ac);
    print_edge(the_ac, the_ac->class_of(), "class_of");
  } // at_classLiteral
 
  void at_instanceof(instanceofNode * the_instanceof, Order ord) {
    if (ord == Postorder || currently_excluded) return;
    print_node(the_instanceof);
    print_edge(the_instanceof, the_instanceof->left(), "left");
    print_edge(the_instanceof, the_instanceof->right(), "right");
  } // at_instanceof

  void at_packagestmt(packagestmtNode *p, Order ord) {
    if (ord == Postorder) return;
    print_node(p);
    print_edge(p, p->name(), "name");
  } // at_packagestmt

  void at_import(importNode *import, Order ord) {
    if (ord == Postorder) return;
    print_node(import);
    print_edge(import, import->name(), "name");
  } // at_import
#endif
}; // class vcgASTWalker

// phase for generating call graph
class vcgASTPhase: public Phase {
private:
  string excludefilename;
public:
  vcgASTPhase() {}
  virtual ~vcgASTPhase() {}
  void run (void) {
    string graph_file_name;

    if( CBZ::Program.empty() ) return;  // no units
    else if( CBZ::Program.size()==1 ) {
      // just one unit
       unitNode* u = CBZ::Program.front();
       graph_file_name = u->input_file() + ".vcgast";
    } else {
      cout << "Several compilation units present. Creating combined call graph."
           << endl;
      graph_file_name = "combined.vcgast";
    }

    // Open the output file.
    ofstream graph_file( graph_file_name.c_str() );
    
    if( graph_file ) {
      // Create the walker and process the ASTs of each unit.
      vcgASTWalker astw(graph_file, "File: " + graph_file_name,
                        excludefilename);

      for(unit_list_p i=CBZ::Program.begin(); i!=CBZ::Program.end(); ++i) {
        unitNode* u = *i;
        u->walk( astw );  // build the graph
      }

    } else /* if( !graph_file ) */ {
      cerr << "vcgASTPhase: Can not open '" << graph_file_name
           << "' for writing." << endl;
    }
  }

  void get_flags(str_list_p & arg) { 
    string opt = *arg;
    if(strncmp("exclude:", opt.c_str(), 8) == 0) {
      excludefilename = opt;
      excludefilename.erase(0,8);
      arg++;
    }
  };
}; // vcgASTPhase

Phases vcgastPhase( "vcgAST", new vcgASTPhase() );
