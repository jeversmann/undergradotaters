
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

#include "nodeinfo.h"
#ifdef J_BREEZE
#include "spackage.h"
#include "conversion.h"
#include "hcfg.h"
#include "ana.h"
#include "chierarchy.h"
#endif
#include <unistd.h>

NodeInfo::NodeInfo(string filename, bool read, bool verbose) {
  _file = NULL;
  if(read) {
    if(access(filename.c_str(), F_OK) ==0 ||
       access(filename.c_str(), R_OK) ==0)
    _file = fopen(filename.c_str(), "r");
  } else
    _file = fopen(filename.c_str(), "w");
  if(!_file) {
    cerr << "NodeInfo: Unable to open file " << filename << endl;
    exit(1);
  }

  _read_mode=read;
  _verbose=verbose;
  _line=0;

  CBZ::PrintLineOffset = true;
#define BUFSIZE 1024

  if(_read_mode) {
    while(!feof(_file)) {
      ++_line;
      char buf[BUFSIZE];
      buf[0]='\0';
      fgets(buf, BUFSIZE, _file);
      if(strlen(buf)==0) continue;
      ostringstream ost;
      ost << buf;

      do {
        if(feof(_file)) break;
        if(buf[strlen(buf)-1] =='\n') break;
        // not yet read to end of line
        buf[0] = '\0';
        fgets(buf, BUFSIZE, _file);
        ost << buf; // append
      } while (1);

      ost << '\0';
      string s = ost.str();
      if(s=="") continue;
      strings[_line] = s;
//cout << "line " << _line << ": " << strings[_line];
      /*if(_verbose)
        cout << s; */
      if(s[0]=='#') continue;

      Node *node = getNode(s);
      index2node[_line] = node;
      node2index[node] = _line;
    }
  }
} // NodeInfo


extern char *TypNames[];
#define MAX_Typ Unit

#ifndef J_BREEZE
list<arrayNode*> NodeInfo::_canonical1;
map<typeNode*,arrayNode*> NodeInfo::_canonical2;
#endif

/////////////////////////////////////////////////////////////////

class NodeInfo::NodeLocator : public Walker {
public:
#ifdef J_BREEZE
  static Node *findClass(string name, Node *search_in) {
    if(!search_in) return NULL;

    // break up name into name and coord
    int pos = name.find('@');
    string coord_str = name.substr(pos+1, string::npos);
    name = name.substr(0, pos);

    pos = coord_str.rfind(':');
    string offset = coord_str.substr(pos+1,string::npos);
    coord_str = coord_str.substr(0,pos),
    pos = coord_str.rfind(':');
    string filename = coord_str.substr(0,pos),
           line     = coord_str.substr(pos+1,string::npos);
    Coord coord = Coord::Unknown;

    int fileno = 0;
    while(fileno < CBZ::Files.size()) {
      if(CBZ::Files[fileno] == filename) break;
      fileno++;
    }
    if(fileno < CBZ::Files.size())
      coord = Coord(atoi(line.c_str()), atoi(offset.c_str()), fileno);

    // note: if look for anonymous class, name should start with substring
    // "<anonymous-"; will be found by at_new (if exist)
    if(!nl) nl = new NodeLocator (Class, name, coord);
    else {
      nl->_typ    = Class;
      nl->_name   = name;
      nl->_coord  = coord;
      nl->_result = NULL;
    }
    search_in->walk(*nl);
    return nl->_result;
  } // findClass
#endif

  static Node *findCall(NodeType typ, string name, int nargs, Node *search_in,
                        Coord coord) {
    if(!search_in) return NULL;

    if(!nl) nl = new NodeLocator (typ, name, nargs, coord);
    else {
      nl->_typ    = typ;
      nl->_name   = name;
      nl->_coord  = coord;
      nl->_nargs  = nargs;
      nl->_result = NULL;
    }
    search_in->walk(*nl);
    return nl->_result;
  } // findCall

  static Node *findExprOrStmt(NodeType typ, Node *search_in, Coord coord) {
    if(!search_in) return NULL;

    if(!nl) nl = new NodeLocator (typ, "", coord);
    else {
      nl->_typ    = typ;
      nl->_name   = "";
      nl->_coord  = coord;
      nl->_nargs  = -1;
      nl->_result = NULL;
    }
    search_in->walk(*nl);
    return nl->_result;
  } // findExprOrStmt

  static declNode *findDecl(string name, Node *search_in, Coord coord) {
    if(!search_in) return NULL;

    if(!nl) nl = new NodeLocator (Decl, name, coord);
    else {
      nl->_typ    = Decl;
      nl->_name   = name;
      nl->_coord  = coord;
      nl->_nargs  = -1;
      nl->_result = NULL;
    }
    search_in->walk(*nl);
    return (declNode*) nl->_result;
  } // findDecl

  inline static void done(void) { if(nl) { delete nl; nl=NULL; } }


private:
  NodeLocator(NodeType typ, string name, Coord coord)
  : _typ(typ), _name(name), _coord(coord), _result(NULL),
    Walker(Preorder, Subtree) {}

  NodeLocator(NodeType typ, string name, int nargs, Coord coord)
  : _typ(typ), _name(name), _nargs(nargs), _result(NULL), _coord(coord),
    Walker(Preorder, Subtree) {}

  static NodeLocator *nl;

  NodeType  _typ;
  string    _name;
  Coord     _coord;
  int       _nargs;
  Node     *_result;

  inline bool match_coord(Coord c1, Coord c2) {
    return (c1.file()==c2.file()) && (c1.line()==c2.line()) &&
           (c1.offset()==c2.offset());
  } // match_coord

#ifdef J_BREEZE
  void at_class(classNode *c, Order ord) {
    if(_result) return;
    if(_typ!=Class) return;
    if(c->declLoc() != classNode::Local &&
       c->declLoc() != classNode::Anonymous) return;
    // look for anonymous class?
    if(_name.find("<anonymous-") == -1) { // no
      if(c->declLoc() != classNode::Local) return;
      if(_name==c->actual_name() && match_coord(_coord, c->coord())) {
        assert(!_result);
        _result = c;
      }
    } else { // yes
      if(c->declLoc() != classNode::Anonymous) return;
      if(match_coord(_coord, c->coord())) {
        assert(!_result);
        _result = c;
      }
    }
  } // at_class

  void at_new(newNode *n, Order ord) {
    if(_result) return;
    if(_typ!=New) return;
    if(_name==NodeInfo::type_string(n->type()) && n->args().size()==_nargs &&
       match_coord(_coord, n->coord())) {
      assert(!_result);
      _result = n;
    }
  } // at_new
#endif

#ifdef J_BREEZE
  void at_mcall(mcallNode *c, Order ord)
#else
  void at_call(callNode *c, Order ord)
#endif
  {
    if(_result) return;
    if(_typ!=Call) return;
    exprNode *c_name = c->name();
    string name;
    switch(c_name->typ()) {
#ifdef J_BREEZE
      case Id: case QualifiedName:
        name = qnameNode::expand_name(c_name); break;
      case Special:
        name = ((specialNode*)c_name)->text(); break;
#else
      case Id:
        name = ((idNode*)c_name)->name(); break;
      case Unary:
        name = "<unary>"; break;
#endif
      case Binary:
        name = "<binary>"; break;
      default:
        cout << "typ is " << c_name->typ() << endl;
        assert(false);
    }
//cout << "at_mcall @" << c->coord() << ": " << (_name==name)
     //<< (c->args().size()==_nargs) << match_coord(_coord, c->coord()) << endl;
    if(_name==name && c->args().size()==_nargs &&
       match_coord(_coord, c->coord())) {
       if(_result) {
         cerr << "NodeLocator: duplicate match: one at " << c->coord()
              << "\nthe other at " << _result->coord() << endl;
         exit(1);
       }
      _result = c;
    }
  } // at_call / at_mcall

  void at_expr(exprNode *expr, Order ord) {
    if(_result) return;
    if(expr->typ()==_typ && match_coord(_coord, expr->coord()))
      _result = expr;
  } // at_expr

  void at_stmt(stmtNode *stmt, Order ord) {
    if(_result) return;
    if(stmt->typ()==_typ && match_coord(_coord, stmt->coord()))
      _result = stmt;
#ifdef J_BREEZE
    if(stmt->typ()==Block && _typ==Expr &&
       match_coord(_coord, ((blockNode*)stmt)->right_brace())) {
      if(! StandardAnalysis::hcfg) {
        StandardAnalysis::hcfg = new high_cfg;
        for (unit_list_p n=CBZ::Program.begin(); n!=CBZ::Program.end(); n++)
          (*n)->walk(*StandardAnalysis::hcfg);
        ArrayClass::clazz()->walk(*StandardAnalysis::hcfg);
      }
      _result = StandardAnalysis::hcfg->endblock((blockNode*)stmt);
    }
#endif
  } // at_stmt

  void at_decl(declNode *decl, Order ord) {
    if(_result) return;
    if(_typ==Decl && match_coord(_coord, decl->coord()) && _name==decl->name())
      _result = decl;
  }
}; // class NodeLocator

NodeInfo::NodeLocator *NodeInfo::NodeLocator::nl = NULL;

/////////////////////////////////////////////////////////////////

NodeInfo::~NodeInfo() {
  fclose(_file);
  NodeLocator::done();
} // ~NodeInfo


Node *NodeInfo::getNode(string s) {
  if(!_read_mode) return NULL;
  int pos = s.find(':');
  string prefix = s.substr(0,pos);
  int typ1 = 0;
  while(typ1<=MAX_Typ) {
    if(prefix == TypNames[typ1]) // found
      break;
    typ1++;
  }

  if(typ1>=MAX_Typ) return NULL;
  NodeType typ = (NodeType) typ1;

  Node *node = NULL;

  switch(typ) {
    case Tdef: case Array:
#ifndef J_BREEZE
    case Struct: case Union: case Enum: case sueSpec: case Ptr: case Func:
#endif
      node = readType(s);  break;
#ifdef J_BREEZE
    case Class:
    case Interface: node = readClassOrInterface(s);  break;
    case Method: node = readMethod(s);  break;
    case FieldVar: node = readFieldvar(s);  break;
    case FieldInit: node = readFieldinit(s);  break;
    case Constructor: node = readConstructor(s);  break;
    case Call: node = readCall(s);  break;
    case New: node = readNew(s);  break;
#else
    case Proc: node = readProc(s); break;
    case Call: node = readCall(s); break;
#endif
    case Id: case Binary: case Unary: case Cast: case Ternary: case Initializer:
#ifdef J_BREEZE
    case QualifiedName: case Special: case AccessClass: case InstanceOf:
#endif
      node = readExprOrStmt(s, typ); break;
    case Label: case Switch: case Case: case If: case While: case Do: case For:
    case Continue: case Break: case Return: case Block:
#ifdef J_BREEZE
    case DeclStmt: case ForInit: case Break_Id: case Continue_Id: case Throw:
    case Synchronized: case Try: case Catch: case Finally: case Assert:
#endif
    case Expr:
      node = readExprOrStmt(s, typ); break;
    case Decl: node = readDecl(s); break;
    default:
      cerr << "NodeInfo::getNode: cannot handle typ " << typ << endl;
      exit(1);
  }

  if(!node) {
    CBZ::Warning(3, Coord::Unknown,
                 "NodeInfo: cannot get node for string `" + s + "'");
    exit(1);
  }
  return node;
} // getNode


typeNode *NodeInfo::readType(string s) {
#ifdef J_BREEZE
  if(s=="Tdef:nullType\n") return Conversion::nullType;
#endif

  // get rid of prefix
  int pos = s.find(':');
  string prefix = s.substr(0,pos+1);

#ifndef J_BREEZE
  if(prefix=="Struct:" || prefix=="Union:" || prefix=="Enum:" || prefix=="Func:")
    return readsuef(s);
#endif

#ifndef J_BREEZE
  bool is_ptr = (prefix == "Ptr:");
  if(!is_ptr && prefix != "Array:") return NULL;
#else
  if(prefix != "Array:") return NULL;
#endif
  s = s.substr(pos+1, string::npos);

  typeNode *base = NULL;
  pos = s.rfind("base=");
  if(pos>0) {
    string ref_base = s.substr(pos+strlen("base="), string::npos);
    Node *node = index2node[ atoi(ref_base.c_str()) ];
    assert(node);
#ifdef J_BREEZE
    assert(node->typ()==Array || node->typ()==Class || node->typ()==Interface);
#else
    assert(node->typ()==Array || node->typ()==sueSpec || node->typ()==Ptr ||
           node->typ()==Func);
#endif
    base = (typeNode*) node;

  } else {
#ifndef J_BREEZE
    if(is_ptr) pos = s.rfind("*");
    else // fall through
#endif
    pos = s.rfind("[]");
    s.resize(pos);
    assert(s.rfind("[]")<0 || s.rfind("[]")>s.length()); // no further base type
#ifndef J_BREEZE
    assert(s.rfind("*")<0 || s.rfind("*")>s.length()); // no further base type
#endif
    // assert: base is Prim
#ifdef J_BREEZE
    if(s=="byte")
      base = new primNode(basic_type::JByte, Coord::Unknown);
    else if(s=="short")
      base = new primNode(basic_type::JShort, Coord::Unknown);
    else if(s=="char")
      base = new primNode(basic_type::SChar, Coord::Unknown);
    else if(s=="int")
      base = new primNode(basic_type::SInt, Coord::Unknown);
    else if(s=="long")
      base = new primNode(basic_type::JLong, Coord::Unknown);
    else if(s=="float")
      base = new primNode(basic_type::Float, Coord::Unknown);
    else if(s=="double")
      base = new primNode(basic_type::Double, Coord::Unknown);
    else if(s=="boolean")
      base = new primNode(basic_type::Boolean, Coord::Unknown);
    else assert(false);
#else
    while(s.length()>0) {
      pos = s.find(" ");
      string s1 = (pos < 0 || pos > (int)s.length()) ? s : s.substr(0,pos);
      if(s==s1) s="";
      else s = s.substr(pos+1,string::npos);
      primNode *base1;
      if(s1=="short")
        base1 = new primNode(basic_type::Short, Coord::Unknown);
      else if(s1=="char")
        base1 = new primNode(basic_type::Char, Coord::Unknown);
      else if(s1=="int")
        base1 = new primNode(basic_type::Int, Coord::Unknown);
      else if(s1=="long")
        base1 = new primNode(basic_type::Long, Coord::Unknown);
      else if(s1=="float")
        base1 = new primNode(basic_type::Float, Coord::Unknown);
      else if(s1=="double")
        base1 = new primNode(basic_type::Double, Coord::Unknown);
      else if(s1=="signed")
        base1 = new primNode(basic_type::Signed, Coord::Unknown);
      else if(s1=="unsigned")
        base1 = new primNode(basic_type::Unsigned, Coord::Unknown);
      else assert(false);
      if(!base) base=base1;
      else {
        ((primNode*)base)->merge_in(base1);
        delete base1;
      }
    }
#endif
  }

#ifndef J_BREEZE
  if(is_ptr)
    return new ptrNode(typeNode::NONE, base, Coord::Unknown);
#endif
  return new arrayNode(typeNode::NONE, base, NULL, Coord::Unknown);
} // readType


#ifndef J_BREEZE
typeNode *NodeInfo::readsuef(string s) {
  if(!_read_mode) return NULL;
  // get rid of prefix
  int pos = s.find(':');
  string prefix = s.substr(0,pos);
  s = s.substr(pos+1, string::npos);

  NodeType typ;
  if(prefix=="Struct")     typ = Struct;
  else if(prefix=="Union") typ = Union;
  else if(prefix=="Enum")  typ = Enum;
  else if(prefix=="Func")  typ = Func;
  else assert(false);

  // obtain Coord and unit
  unitNode *unit;
  Coord coord = getCoord(s, &unit);

  // get rid of coord suffix
  pos = s.rfind('@');
  s = s.substr(0, pos);
  // get rid of trailing spaces
  while(s[s.length()-1]==' ')
    s.resize(s.length()-1);

  // obtain type
  typeNode *type = NULL;
  if(typ==Func) {
    ostringstream ost;
    ost << coord.to_string() << '\0';
    string coord_str = ost.str();
cout << coord_str << " vs " << coord << "|\n";
    if(unit)
      for(def_list_p p=unit->defs().begin(); p!=unit->defs().end();p++)
        if((*p)->typ()==Decl && ((declNode*)*p)->type()->typ()==Func) {
           funcNode *func = (funcNode*) ((declNode*)*p)->type();
           ostringstream ost1;
           ost1 << func->coord().to_string() << '\0';
           if(coord_str == ost.str())
           { type=((declNode*)*p)->type();  break; }
        }
    for(unit_list_p u=CBZ::Program.begin(); !type && u!=CBZ::Program.end(); u++)
      for(def_list_p p=(*u)->defs().begin(); p!=(*u)->defs().end(); p++)
        if((*p)->typ()==Decl && ((declNode*)*p)->type()->typ()==Func) {
           funcNode *func = (funcNode*) ((declNode*)*p)->type();
           ostringstream ost1;
           ost1 << func->coord().to_string() << '\0';
           if(coord_str == ost.str())
           { type=((declNode*)*p)->type();  break; }
        }
    if(!type) return NULL;
    assert(type->typ() == Func);
    return type;

  } else { // Struct/Union/Enum
    if(unit)
      for(suespec_list_p p=unit->suespecs().begin();
          p!=unit->suespecs().end();p++)
        if((*p)->name() == s) { type=*p;  break; }
    for(unit_list_p u=CBZ::Program.begin(); !type && u!=CBZ::Program.end(); u++)
      for(suespec_list_p p=(*u)->suespecs().begin();
          p!=(*u)->suespecs().end();p++)
        if((*p)->name() == s) { type=*p;  break; }
    if(!type) return NULL;
    assert(type->typ() == sueSpec);
    assert(((suespecNode*)type)->owner() == typ);
    return type;
  }
} // readsuef


procNode *NodeInfo::readProc(string s) {
  if(!_read_mode) return NULL;
  // get rid of prefix "Method:"
  int pos = s.find(':');
  s = s.substr(pos+1, string::npos);

  // obtain Coord and unit
  unitNode *unit;
  Coord coord = getCoord(s, &unit);

  // get rid of coord suffix
  pos = s.rfind('@');
  s = s.substr(0, pos);

  // obtain args
  pos = s.find('(');
  string args_string = s.substr(pos+1,string::npos);
  s = s.substr(0,pos);

  // finally, find method

  if(unit)
    for(def_list_p def=unit->defs().begin(); def!=unit->defs().end(); def++) {
      if((*def)->typ() != Proc) continue;
      procNode *p = (procNode*) *def;
      assert(p->decl()->type()->typ()==Func);
      decl_list args = ((funcNode*)p->decl()->type())->args();
      if(p->decl()->name() == s && match_args(args_string, args))
        return p;
    }
  for(unit_list_p u=CBZ::Program.begin(); u!=CBZ::Program.end(); u++)
    for(def_list_p def=(*u)->defs().begin(); def!=(*u)->defs().end(); def++) {
      if((*def)->typ() != Proc) continue;
      procNode *p = (procNode*) *def;
      assert(p->decl()->type()->typ()==Func);
      decl_list args = ((funcNode*)p->decl()->type())->args();
      if(p->decl()->name() == s && match_args(args_string, args))
        return p;
    }

  return NULL; // not found
} // readProc

#else
typeNode *NodeInfo::readClassOrInterface(string s) {
  if(!_read_mode) return NULL;
  // get rid of prefix
  int pos = s.find(':');
  string prefix = s.substr(0,pos);
  s = s.substr(pos+1, string::npos);

  NodeType typ;
  if(prefix=="Class")          typ = Class;
  else if(prefix=="Interface") typ = Interface;
  else assert(false);

  // obtain Coord and unit
  unitNode *unit;
  Coord coord = getCoord(s, &unit);
  //if(!unit) return NULL;

  // get rid of coord suffix
  pos = s.rfind('@');
  s = s.substr(0, pos);

  // obtain type
  typeNode *type = getType(unit, s);
  if(!type) return NULL;
  assert(type->typ() == typ);
  return type;
} // readClassOrInterface


methodNode *NodeInfo::readMethod(string s) {
  if(!_read_mode) return NULL;
  // get rid of prefix "Method:"
  int pos = s.find(':');
  s = s.substr(pos+1, string::npos);

  // obtain Coord and unit
  unitNode *unit;
  Coord coord = getCoord(s, &unit);

  // get rid of coord suffix
  pos = s.rfind('@');
  s = s.substr(0, pos);

  // obtain args
  pos = s.find('(');
  string args_string = s.substr(pos+1,string::npos);
  s = s.substr(0,pos);

  // obtain type where method is defined.
  pos = s.rfind('$');
  string type_string = s.substr(0, pos);
  typeNode *type = getType(unit, type_string);
  if(!type) return NULL;

  // finally, find method
  string mtd_string = s.substr(pos+1, string::npos);

  if(standardPackage::existType(type))
    standardPackage::addMethods((referenceNode*)type, mtd_string.c_str());

  field_list fields;
  if(type->typ() == Class)          fields=((classNode*) type)->fields();
  else if(type->typ() == Interface) fields=((interfaceNode*) type)->fields();
  else assert(false);

  for(field_list_p f=fields.begin(); f!=fields.end(); f++) {
    if((*f)->typ() != Method) continue;
    methodNode *m = (methodNode*)*f;
    if(m->decl()->name() == mtd_string && match_args(args_string, m->args()))
      return m;
  }

  return NULL; // not found
} // readMethod


constructorNode *NodeInfo::readConstructor(string s) {
  if(!_read_mode) return NULL;
  // get rid of prefix "Constructor:"
  int pos = s.find(':');
  s = s.substr(pos+1, string::npos);

  // obtain Coord and unit
  unitNode *unit;
  Coord coord = getCoord(s, &unit);

  // get rid of coord suffix
  pos = s.rfind('@');
  s = s.substr(0, pos);

  // obtain type where constructor is defined.
  pos = s.rfind('(');
  string type_string = s.substr(0, pos);
  typeNode *type = getType(unit, type_string);
  if(!type) return NULL;

  if(type->typ()==Class && standardPackage::existType(type))
    standardPackage::addConstructors((classNode*) type);

  // finally, find constructor
  string args_string = s.substr(pos+1,string::npos);
  field_list fields;
  if(type->typ() == Class)          fields=((classNode*) type)->fields();
  else if(type->typ() == Interface) fields=((interfaceNode*) type)->fields();
  else assert(false);

  for(field_list_p f=fields.begin(); f!=fields.end(); f++) {
    if((*f)->typ() != Constructor) continue;
    constructorNode *c = (constructorNode*)*f;
    if(match_args(args_string, c->args()))
      return c;
  }

  return NULL; // not found
} // readConstructor
#endif // J_BREEZE


bool NodeInfo::match_args(string args, decl_list formals) {
  // args must be in form of "arg1,arg2,...,argn) "
  if(formals.empty()) return args[0]==')';
  bool last = false;;
  for(decl_list_p f=formals.begin(); f!=formals.end(); ) {
    if(args=="") return false;
    int pos = args.find(',');
    if(pos==-1)  { // either last or no argument
      pos = args.find(')');
      last = true;
    }
    if(pos==-1) return false;
    string arg = args.substr(0, pos);
#ifdef J_BREEZE
    if(arg != typenameNode::type_name((*f)->type())) return false;
#else
    if(arg != type_name((*f)->type())) return false;
#endif
    args = args.substr(pos+1,string::npos);

    f++;
    if(last) return f==formals.end();
  }
  // not yet last arg, but no more formal
  return false;
} // match_args


#ifdef J_BREEZE
fieldvarNode *NodeInfo::readFieldvar(string s) {
  if(!_read_mode) return NULL;
  // get rid of prefix "FieldVar:"
  int pos = s.find(':');
  s = s.substr(pos+1, string::npos);

  // obtain Coord and unit
  unitNode *unit;
  Coord coord = getCoord(s, &unit);

  // get rid of coord suffix
  pos = s.rfind(" @");
  s = s.substr(0, pos);

  // obtain type where fieldvar is defined.
  pos = s.rfind('$');
  string type_string = s.substr(0, pos);
  typeNode *type = getType(unit, type_string);
  if(!type) return NULL;

  // finally, find fieldvar
  string fv_string = s.substr(pos+1, string::npos);

  if(standardPackage::existType(type))
    standardPackage::addFieldvar((referenceNode*) type, fv_string.c_str());

  field_list fields;
  if(type->typ() == Class)          fields=((classNode*) type)->fields();
  else if(type->typ() == Interface) fields=((interfaceNode*) type)->fields();
  else assert(false);

  for(field_list_p f=fields.begin(); f!=fields.end(); f++) {
    if((*f)->typ() != FieldVar) continue;
    fieldvarNode *fv = (fieldvarNode*)*f;
    for(decl_list_p d=fv->decls().begin(); d!=fv->decls().end(); d++)
      if((*d)->name() == fv_string)
        return fv;
  }

  return NULL; // not found
} // readFieldvar


fieldinitNode *NodeInfo::readFieldinit(string s) {
  if(!_read_mode) return NULL;
  // get rid of prefix "FieldInit:"
  int pos = s.find(':');
  s = s.substr(pos+1, string::npos);

  // obtain Coord and unit
  unitNode *unit;
  Coord coord = getCoord(s, &unit);

  // get rid of coord suffix
  pos = s.rfind(" @");
  s = s.substr(0, pos);

  // obtain type where fieldinit is defined.
  typeNode *type = getType(unit, s);
  if(!type) return NULL;

  field_list fields;
  if(type->typ() == Class)          fields=((classNode*) type)->fields();
  else if(type->typ() == Interface) fields=((interfaceNode*) type)->fields();
  else assert(false);

  for(field_list_p f=fields.begin(); f!=fields.end(); f++) {
    if((*f)->typ() != FieldInit) continue;
    fieldinitNode *fi = (fieldinitNode*)*f;
    if(fi->coord().line() == coord.line())
      return fi;
  }

  return NULL; // not found
} // readFieldinit
#endif // J_BREEZE


#ifdef J_BREEZE
mcallNode *NodeInfo::readCall(string s) {
#else
callNode *NodeInfo::readCall(string s) {
#endif // J_BREEZE
  // get rid of prefix "Call:"
  int pos = s.find(':');
  s = s.substr(pos+1, string::npos);

  // obtain Coord and unit
  unitNode *unit;
  Coord coord = getCoord(s, &unit);
  //if(!unit) return NULL;

  // get rid of coord suffix
  pos = s.rfind('@');
  s = s.substr(0, pos);

  // obtain container, if any
  Node *container=NULL;
  pos = s.rfind("container=");
  if(pos != -1) {
    pos = s.rfind('=');
    string container_str = s.substr(pos+1, string::npos);
    s = s.substr(0,pos);
    container = index2node[atoi(container_str.c_str())];
if(!container) cout << "container_str=" << container_str << ", s="<<s<<endl;
    assert(container);
  } else
    container = unit;

  // obtain #args
  pos = s.find('(');
  int pos2 = s.find(')');
  int nargs = atoi(s.substr(pos+1,pos2).c_str());
  s = s.substr(0,pos);

  // finally, find newNode
#ifdef J_BREEZE
  mcallNode *c = (mcallNode*)
#else
  callNode *c = (callNode*)
#endif
    NodeLocator::findCall(Call, s, nargs, container, coord);

#ifndef J_BREEZE
  // container could be NULL, due to unknown unit due to included file
  for(unit_list_p u=CBZ::Program.begin(); !c && u!=CBZ::Program.end(); u++)
    c = (callNode*) NodeLocator::findCall(Call, s, nargs, *u, coord);
#endif

  if(!c) {
    cerr << "error in NodeInfo::readCall\n";
    cerr << "s=`" << s << "'\n";
    if(container->typ()==Unit)
      cerr << "container=" << ((unitNode*)container)->input_file() << endl;
    else
      cerr << "container @" << container->coord() << endl;
    cerr << "nargs=" << nargs << endl;
    cerr << "coord=" << coord << endl;
    exit(1);
  }

  return c;
} // readCall


#ifdef J_BREEZE
newNode *NodeInfo::readNew(string s) {
  // get rid of prefix "New:"
  int pos = s.find(':');
  s = s.substr(pos+1, string::npos);

  // obtain Coord and unit
  unitNode *unit;
  Coord coord = getCoord(s, &unit);
  //if(!unit) return NULL;

  // get rid of coord suffix
  pos = s.rfind('@');
  s = s.substr(0, pos);

  // obtain container, if any
  Node *container=NULL;
  pos = s.rfind("container=");
  if(pos != -1) {
    pos = s.rfind('=');
    string container_str = s.substr(pos+1, string::npos);
    s = s.substr(0,pos);
    container = index2node[atoi(container_str.c_str())];
    assert(container);
  } else
    container = unit;

  // obtain #args
  pos = s.find('(');
  int pos2 = s.find(')');
  int nargs = atoi(s.substr(pos+1,pos2).c_str());
  s = s.substr(0,pos);

  // finally, find newNode
  newNode *n
    = (newNode*) NodeLocator::findCall(New, s, nargs, container, coord);

  if(!n) {
    cerr << "error in NodeInfo::readNew\n";
    cerr << "s=`" << s << "'\n";
    if(container->typ()==Unit)
      cerr << "container=" << ((unitNode*)container)->input_file() << endl;
    else
      cerr << "container @" << container->coord() << endl;
    cerr << "nargs=" << nargs << endl;
    cerr << "coord=" << coord << endl;
    exit(1);
  }
  return n;
} // readNew
#endif // J_BREEZE


Node *NodeInfo::readExprOrStmt(string s, NodeType typ) {
  // get rid of prefix
  int pos = s.find(':');
  s = s.substr(pos+1, string::npos);

  // obtain Coord and unit
  unitNode *unit;
  Coord coord = getCoord(s, &unit);
  //if(!unit) return NULL;

  // get rid of coord suffix
  pos = s.rfind('@');
  s = s.substr(0, pos);

  // obtain container, if any
  Node *container=NULL;
  pos = s.rfind("container=");
  assert(pos != -1);
  pos = s.rfind('=');
  string container_str = s.substr(pos+1, string::npos);
  s = s.substr(0,pos);
  container = index2node[atoi(container_str.c_str())];
if(!container) cout << "container_str=" << container_str << ", s="<<s<<endl;
  assert(container);

  // finally, find Node
  Node *e = NodeLocator::findExprOrStmt(typ, container, coord);

  if(!e) {
    cerr << "error in NodeInfo::readExprOrStmt\n";
    cerr << "s=`" << s << "'\n";
    if(container->typ()==Unit)
      cerr << "container=" << ((unitNode*)container)->input_file() << endl;
    else
      cerr << "container @" << container->coord() << endl;
    cerr << "coord=" << coord << endl;
    exit(1);
  }

  return e;
} // readExprOrStmt


declNode *NodeInfo::readDecl(string s) {
  // get rid of prefix
  int pos = s.find(':');
  s = s.substr(pos+1, string::npos);

  // obtain Coord and unit
  unitNode *unit;
  Coord coord = getCoord(s, &unit);
  //if(!unit) return NULL;

  // get rid of coord suffix
  pos = s.rfind('@');
  s = s.substr(0, pos);

  // obtain container, if any
  Node *container=NULL;
  pos = s.rfind("container=");
  assert(pos != -1);
  pos = s.rfind('=');
  string container_str = s.substr(pos+1, string::npos);
  container = index2node[atoi(container_str.c_str())];
if(!container) {
  cout << "container_str=" << container_str << ", s="<<s<<endl;
  cout << "strings[@] = " << strings[atoi(container_str.c_str())] << endl;
}
  assert(container);

  // obtain name of decl
  pos = s.rfind(" container=");
  string name = s.substr(0,pos);

  // finally, find Node
  declNode *d = NodeLocator::findDecl(name, container, coord);

  if(!d) {
    cerr << "error in NodeInfo::readDecl\n";
    cerr << "s=`" << s << "'\n";
    if(container->typ()==Unit)
      cerr << "container=" << ((unitNode*)container)->input_file() << endl;
    else
      cerr << "container @" << container->coord() << endl;
    cerr << "coord=" << coord << endl;
    exit(1);
  }

  return d;
} // readDecl

/////////////////////////////////////////////////////////////////

#ifdef J_BREEZE
// format: "Class:nested_type @coord" or
//         "Interface:nested_type @coord" or
void NodeInfo::writeClassOrInterface(referenceNode *ty) {
  if(_read_mode || !ty) return;
  if(node2index[ty] > 0) return;
  NodeType typ = ty->typ();
  if(typ!=Class && typ!=Interface) return;

  string s = string(TypNames[typ]) + ':' + type_string(ty);
  ostringstream ost;
  ost << ty->coord() << '\0';
  string s1 = s + string(" @") + ost.str() + string("\n");
  s = s1;
  fputs(s.c_str(), _file);
  index2node[++_line] = ty;
  node2index[ty] = _line;
  strings[_line] = s;
  fflush(_file);
} // writeClassOrInterface
#endif // J_BREEZE


void NodeInfo::writeType(typeNode *ty) {
  if(_read_mode || !ty) return;
  if(node2index[ty] > 0) return;
  if(ty->typ() == Prim) return;  // ignore
#ifdef J_BREEZE
  ty = classHierarchy::def_type(ty);
  if(ty->typ()==Class || ty->typ()==Interface) {
    writeClassOrInterface((referenceNode*) ty);
    return;
  }
  
#else

  ty = def_type(ty);
  if(ty->typ() == Prim) return;  // ignore
  if(ty->typ()==sueSpec) {
    if(node2index[ty] > 0) return;
    suespecNode *spec = (suespecNode*) ty;
    string s = string(TypNames[spec->owner()]) + ':' + spec->name();
    ostringstream ost;
    ost << ty->coord() << '\0';
    string s1 = s + string(" @") + ost.str() + string("\n");
    s = s1;
    fputs(s.c_str(), _file);
    index2node[++_line] = ty;
    node2index[ty] = _line;
    strings[_line] = s;
    fflush(_file);
    return;
  }
#endif

#ifdef J_BREEZE
  if(ty->typ()==Tdef && ty==Conversion::nullType) {
    string s = "Tdef:nullType\n";
    fputs(s.c_str(), _file);
    index2node[++_line] = ty;
    node2index[ty] = _line;
    strings[_line] = s;
    fflush(_file);
    return;
  }
#else
#endif

#ifndef J_BREEZE
  if(ty->typ() == Func) {
    if(node2index[ty] > 0) return;
    string s = string(TypNames[Func]) + ':';
    ostringstream ost;
    ost << ty->coord() << '\0';
    string s1 = s + string(" @") + ost.str() + string("\n");
    s = s1;
    fputs(s.c_str(), _file);
    index2node[++_line] = ty;
    node2index[ty] = _line;
    strings[_line] = s;
    fflush(_file);
    return;
  }

  if(ty->typ() == Ptr) {
    ptrNode *ptr = (ptrNode*) ty;
    typeNode *base = def_type(ptr->type());
    writeType(base);
    int ref_base = node2index[base];
    ostringstream ost;
    ost << "Ptr:" << type_name(ptr);
    if(ref_base>0)
      ost << " base=" << ref_base;
    ost << '\0';
    string s = ost.str() + string("\n");
    fputs(s.c_str(), _file);
    index2node[++_line] = ptr;
    node2index[ptr] = _line;
    strings[_line] = s;
    fflush(_file);
    return;
  }
#endif

  if(ty->typ() != Array) {
    cerr << "NodeInfo::writeType: typ " << ty->typ() << endl;
    exit(1);
  }
#ifdef J_BREEZE
  arrayNode *array = ArrayClass::canonical((arrayNode*) ty);
#else
  arrayNode *array = canonical((arrayNode*) ty);
#endif
  if(node2index[array] > 0) return;
  typeNode *base = array->type();
  if(base->typ()==Array)
#ifdef J_BREEZE
    base = ArrayClass::canonical((arrayNode*) base);
#else
    base = canonical((arrayNode*) base);
#endif
  else
#ifdef J_BREEZE
    base = classHierarchy::def_type(base);
#else
    base = def_type(base);
#endif
  writeType(base);
  int ref_base = node2index[base];

  ostringstream ost;
#ifdef J_BREEZE
  ost << "Array:" << typenameNode::type_name(array);
#else
  ost << "Array:" << type_name(array);
#endif
  if(ref_base>0)
    ost << " base=" << ref_base;
  ost << '\0';
  string s = ost.str() + string("\n");
  fputs(s.c_str(), _file);
  index2node[++_line] = array;
  node2index[array] = _line;
  strings[_line] = s;
  fflush(_file);
} // writeType


#ifdef J_BREEZE
// format: "Method:nested_type$method(args) @coord"
void NodeInfo::writeMethod(methodNode *m) {
  if(_read_mode || !m) return;
  if(node2index[m] > 0) return;
  typeNode *fieldof = m->fieldof();

  string s = string(TypNames[Method]) + ':' + type_string(fieldof) + '$' +
             m->decl()->name() + "(";
  decl_list args = m->args();
  for(decl_list_p a=args.begin(); a!=args.end(); ) {
    s += typenameNode::type_name((*a)->type());
    if(++a!=args.end()) s += ",";
  }
  ostringstream ost;
  ost << m->coord() << '\0';
  s += string(") @") + ost.str() + string("\n");
  fputs(s.c_str(), _file);
  index2node[++_line] = m;
  node2index[m] = _line;
  strings[_line]   = s;
  fflush(_file);
} // writeMethod


// format: "Constructor:nested_type$constructor(args) @coord"
void NodeInfo::writeConstructor(constructorNode *c) {
  if(_read_mode || !c) return;
  if(node2index[c] > 0) return;
  typeNode *fieldof = c->fieldof();

  string s = string(TypNames[Constructor]) + ':' + type_string(fieldof) + "(";
  decl_list args = c->args();
  for(decl_list_p a=args.begin(); a!=args.end(); ) {
    s += typenameNode::type_name((*a)->type());
    if(++a!=args.end()) s += ",";
  }
  ostringstream ost;
  ost << c->coord() << '\0';
  s += string(") @") + ost.str() + "\n";
  fputs(s.c_str(), _file);
  index2node[++_line] = c;
  node2index[c] = _line;
  strings[_line]   = s;
  fflush(_file);
} // writeConstructor


// format: "FieldVar:nested_type$fieldvar @coord"
void NodeInfo::writeFieldvar(fieldvarNode *fv, declNode *d) {
  if(_read_mode || !fv) return;
  if(node2index[fv] > 0) return;
  typeNode *fieldof = fv->fieldof();
  if(!d) d=fv->decl();

  string s = string(TypNames[FieldVar]) + ':' + type_string(fieldof) + '$' +
             d->name();
  ostringstream ost;
  ost << fv->coord() << '\0';
  s += string(" @") + ost.str() + string("\n");
  fputs(s.c_str(), _file);
  index2node[++_line] = fv;
  node2index[fv] = _line;
  strings[_line]   = s;
  fflush(_file);
} // writeFieldvar


// format: "FieldInit:nested_type @coord"
void NodeInfo::writeFieldinit(fieldinitNode *fi) {
  if(_read_mode || !fi) return;
  if(node2index[fi] > 0) return;
  typeNode *fieldof = fi->fieldof();

  string s = string(TypNames[FieldInit]) + ':' + type_string(fieldof);
  ostringstream ost;
  ost << fi->coord() << '\0';
  s += string(" @") + ost.str() + string("\n");
  fputs(s.c_str(), _file);
  index2node[++_line] = fi;
  node2index[fi] = _line;
  strings[_line]   = s;
  fflush(_file);
} // writeFieldvar

#else
void NodeInfo::writeProc(procNode *proc) {
  if(_read_mode || !proc) return;
  if(node2index[proc] > 0) return;

  string s = string(TypNames[Proc]) + ':' + proc->decl()->name() + "(";
  assert(proc->decl()->type()->typ()==Func);
  decl_list args = ((funcNode*)proc->decl()->type())->args();
  for(decl_list_p a=args.begin(); a!=args.end(); ) {
    s += type_name((*a)->type());
    if(++a!=args.end()) s += ",";
  }
  ostringstream ost;
  ost << proc->coord() << '\0';
  s += string(") @") + ost.str() + string("\n");
  fputs(s.c_str(), _file);
  index2node[++_line] = proc;
  node2index[proc] = _line;
  strings[_line]= s;
  fflush(_file);
} // writeMethod
#endif // J_BREEZE


// format: "Call:name(num_arg) container=n @coord"
//      or "Call:name(num_arg) @coord"
// todo: binary name?
#ifdef J_BREEZE
void NodeInfo::writeCall(mcallNode *c, int container)
#else
void NodeInfo::writeCall(callNode *c, int container)
#endif
{
  if(_read_mode || !c) return;
  if(node2index[c] > 0) return;
  exprNode *c_name = c->name();
  string name;
  switch(c_name->typ()) {
#ifdef J_BREEZE
    case Id: case QualifiedName:
      name = qnameNode::expand_name(c_name); break;
    case Special:
      name = ((specialNode*)c_name)->text(); break;
#else
    case Id:
      name = ((idNode*)c_name)->name(); break;
#endif
    case Binary:
      name = "<binary>"; break;
    case Unary:
      name = "<unary>"; break;
    default:
      cout << "typ is " << c_name->typ() << endl;
      assert(false);
  }

  ostringstream ost;
  ost << TypNames[Call] << ':' << name << "(" << c->args().size() << ")";

  if(container>0)
    ost << " container=" << container;

  ost << " @" << c->coord() << '\0';
  string s = ost.str() + string("\n");
  fputs(s.c_str(), _file);
  index2node[++_line] = c;
  node2index[c] = _line;
  strings[_line]   = s;
  fflush(_file);
} // writeCall


#ifdef J_BREEZE
// format: "New:type(num_arg) container=n @coord"
//      or "New:type(num_arg) @coord"
void NodeInfo::writeNew(newNode *n, int container) {
  if(_read_mode || !n) return;
  if(node2index[n] > 0) return;
  typeNode *type = n->type();

  ostringstream ost;
  ost << TypNames[New] << ':' << type_string(type) << "(" << n->args().size()
      << ")";

  if(container>0)
    ost << " container=" << container;

  ost << " @" << n->coord() << '\0';
  string s = ost.str() + string("\n");
  fputs(s.c_str(), _file);
  index2node[++_line] = n;
  node2index[n] = _line;
  strings[_line]   = s;
  fflush(_file);
} // writeNew

#endif // J_BREEZE


// format: "<typ>:container=n @coord"
void NodeInfo::writeExpr(exprNode *e, int container) {
  if(_read_mode || !e) return;
  if(node2index[e] > 0) return;
  if(e->coord().file()==-1) {
    cerr << "NodeInfo::writeExpr: cannot write if coord is unknown.\n";
    exit(1);
  }
  assert(container>0);
  ostringstream ost;
  ost << TypNames[e->typ()] << ':' << " container=" << container;
  ost << " @" << e->coord() << '\0';
  string str = ost.str() + string("\n");
  fputs(str.c_str(), _file);
  index2node[++_line] = e;
  node2index[e] = _line;
  strings[_line]   = str;
  fflush(_file);
} // writeExpr


// format: "Decl:name container=n @coord"
void NodeInfo::writeDecl(declNode *decl, int container) {
  if(_read_mode || !decl) return;
  if(node2index[decl] > 0) return;
  if(container<=0) return;

  ostringstream ost;
  ost << TypNames[Decl] << ":" << decl->name() << " container=" << container
      << string(" @") << decl->coord() << '\0';
  string str = ost.str() + string("\n");
  fputs(str.c_str(), _file);
  index2node[++_line] = decl;
  node2index[decl] = _line;
  strings[_line]   = str;
  fflush(_file);
} // writeDecl


// format: "<typ>:container=n @coord"
void NodeInfo::writeStmt(stmtNode *s, int container) {
  if(_read_mode || !s) return;
  if(node2index[s] > 0) return;
  if(s->coord().file()==-1) {
    cerr << "NodeInfo::writeStmt: cannot write if coord is unknown.\n";
    exit(1);
  }
  assert(container>0);
  ostringstream ost;
  ost << TypNames[s->typ()] << ':' << " container=" << container;
  ost << " @" << s->coord() << '\0';
  string str = ost.str() + string("\n");
  fputs(str.c_str(), _file);
  index2node[++_line] = s;
  node2index[s] = _line;
  strings[_line]   = str;
  fflush(_file);
} // writeStmt

void NodeInfo::writeString(string s) {
  fputs(s.c_str(), _file);
  index2node[++_line] = NULL;
  strings[_line]   = s;
  fflush(_file);
} // writeString

//////////////////////////////////////////////////////////

Coord NodeInfo::getCoord(string s, unitNode **unit) {
  *unit = NULL;
  int pos = s.rfind('@');
  if(pos==-1) {
    if(_verbose)
      cerr << "coord not found\n";
    return Coord::Unknown;
  }
  string sub = s.substr(pos+1, string::npos);
  pos = sub.rfind(':');
  string offset = sub.substr(pos+1,string::npos);
  sub = sub.substr(0,pos),
  pos = sub.rfind(':');
  string filename = sub.substr(0,pos),
         line = sub.substr(pos+1,string::npos);
  int fileno = 0;
  while(fileno < (int)CBZ::Files.size()) {
    if(CBZ::Files[fileno] == filename) break;
    fileno++;
  }
  if(fileno >= (int)CBZ::Files.size()) {
    if(_verbose && filename!="(unknown)\n") {
      cerr << "cannot find fileno for file " << filename << endl;
      cerr << "s is " << s << endl;
      exit(1);
    }
    return Coord::Unknown;
  }

  // obtain unitNode
  for(unit_list_p u=CBZ::Program.begin(); u!=CBZ::Program.end(); u++)
    if((*u)->input_file() == filename) {
      *unit = *u;
      break;
    }
#ifdef J_BREEZE  // in C, could be a header file not in CBZ::Program.
  assert(*unit);
#endif

  return Coord(atoi(line.c_str()), atoi(offset.c_str()), fileno);
} // getCoord


typeNode *NodeInfo::getType(unitNode *unit, string s) {
  // get rid of trailing spaces
  while(s[s.length()-1]==' ')
    s.resize(s.length()-1);

  int pos = s.find('$');
  string top_string;
  if(pos==-1) top_string = s;
  else        top_string = s.substr(0,pos);

  // search for top type in unit
  typeNode *type = NULL;
  if(unit) {
    for(def_list_p def=unit->defs().begin(); def!=unit->defs().end(); def++) {
      if((*def)->typ() != Decl) continue;
      declNode *d = (declNode*) *def;
      if(d->name() == top_string) {
        type = d->type();
        break;
      }
    }
  }

#ifdef J_BREEZE
  if(!type) {
    ArrayClass::clazz();
    type = getLibType(top_string);
  }
#endif

  if(!type) {
    if(_verbose) {
      cerr << "getType: cannot find top-type " << top_string << endl;
      if(unit) cerr << " in unit " << unit->input_file() << endl;
    }
    return NULL;
  }
  if(pos==-1) return type;  // no nested type, done

#ifdef J_BREEZE
  string nested_string = s.substr(pos+1, string::npos);
  while(nested_string.length() > 0) {
    pos = nested_string.find('$');
    string next;
    if(pos==-1) next = nested_string;
    else        next = nested_string.substr(0,pos);
    typeNode *nested = NULL;

    // is next a local/anonymous class?
    if(next.find('@') == -1) {
      // not a local/anonymous class
      field_list fields;
      if(type->typ() == Class)          fields=((classNode*) type)->fields();
      else if(type->typ() == Interface) fields=((interfaceNode*)type)->fields();
      else assert(false);

      for(field_list_p f=fields.begin(); f!=fields.end(); f++) {
        if((*f)->typ() != MemberType) continue;
        declNode *d = ((membertypeNode*)*f)->member();
        if(d->name() == next) {
          nested = d->type();
          break;
        }
      }

    } else {
      // local/anonymous class, with the help from NodeLocator
      nested = (classNode*) NodeLocator::findClass(next, type);
      if(nested)
        if(((classNode*)nested)->enclosing() != type)  // wrong nested
          nested = NULL;
    }

    if(!nested) {
      if(_verbose)
        cerr << "getType: cannot find type " << next << " in type "
             << typenameNode::type_name(type) << endl;
      return NULL;
    }

    // get ready for recursion
    type = nested;
    if(pos==-1) break;  // done with recursion
    nested_string = nested_string.substr(pos+1, string::npos);
  }
#endif

  return type;
} // getType


#ifdef J_BREEZE
typeNode *NodeInfo::getLibType(string s) {
  if(s.length() <= 0) return NULL;
  // cannot contain '@'
  if(s.find('@') != -1) return NULL;

  // get rid of trailing spaces
  while(s[s.length()-1]==' ')
    s.resize(s.length()-1);

  // if s is a fully qualified name (including package name):
  // format of string: eg. java.io.PrintStream
  string remaining = s;
  packageNode *package = NULL;
  do {
    int pos = remaining.find('.'); // '.' was not converted to '$' in
                                   // type_string()
    if(pos == -1) break;
    string prefix = remaining.substr(0, pos);
    if(!package)
      package = packageNode::findPackage(prefix.c_str());  // main package
    else
      package = // sub-package
        (packageNode*) package->findMember(prefix.c_str(), true, false);
    remaining = remaining.substr(pos+1, string::npos);
  } while(package);

  if(package) {
    declNode *decl =
      (declNode*) package->findMember(remaining.c_str(), false, true);
    if(decl) return decl->type();
  }

  // last resort
  return standardPackage::findType(s.c_str());
} // getLibType
#endif


#ifdef J_BREEZE
string NodeInfo::type_string(typeNode *ty) {
  string s = "";
  bool unknown_coord = (ty->coord().file()==-1);
  packageNode *package = NULL;
  while(ty) {
    ty = classHierarchy::def_type(ty);
    string extra = "";
    bool localOrAnony = (ty->typ()==Class &&
                        (((classNode*)ty)->declLoc()==classNode::Local ||
                         ((classNode*)ty)->declLoc()==classNode::Anonymous));
    if(localOrAnony) {
      ostringstream ost;
      ost << "@" << ty->coord() << '\0';
      extra = ost.str() + string("\0");
      // get rid of tail ".java"
      /*int pos = extra.rfind(".java");
      extra = extra.replace(pos,5,""); */
    }

    if(s.length()==0) {
      if(extra != "")
        s = typenameNode::type_name(ty) + extra;
      else
        s = typenameNode::type_name(ty);
    } else {
      string s1;
      if(extra != "")
        s1 = typenameNode::type_name(ty) + extra + "$" + s;
      else
        s1 = typenameNode::type_name(ty) + "$" + s;
      s = s1;
    }
    if(ty->typ()==Class) {
      package = ((classNode*)ty)->package();
      if(localOrAnony)
        ty = ((classNode*)ty)->enclosing();
      else
        break;
    } else if(ty->typ()==Interface) {
      package = ((interfaceNode*)ty)->package();
      if(localOrAnony)
        ty = ((interfaceNode*)ty)->enclosing();
      else
        break;
    } else if(ty->typ()==Array) {
      package = packageNode::unnamed();
      break;
    } else
      break;
  }

  if(unknown_coord) {
    // also add in package name
    assert(package);
    string s1 = package->fullname() + '.' + s;
    s = s1;
  }

  return s;
} // type_string
#endif


#ifndef J_BREEZE
string NodeInfo::type_name(typeNode *t) {
  assert(t!=NULL);
  switch(t->typ()) {
    case Prim: {
      basic_type basic = ((primNode*)t)->basic();
      return basic.to_string();
    }

    case Tdef:
      return ((tdefNode*)t)->name();

    case Array:
      return type_name( ((arrayNode*) t)->type() ) + "[]\0";

    case Ptr:
      return type_name( ((ptrNode*) t)->type() ) + "*\0";

    case Func:
      return "(func)";

    case Struct:
    case Union:
    case Enum:
      return string(TypNames[t->typ()]) + "-" + ((sueNode*)t)->spec()->name();

    case sueSpec:

    default:
      cout << "typenameNode::type_name: unknown typenode "
           << t->typ() << endl;
      exit(1);
  }
} // type_name


typeNode *NodeInfo::def_type(typeNode *ty) {
    if(!ty) return NULL;
  switch(ty->typ()) {
    case Prim:
    case Array:
    case Ptr:
    case Func:
    case sueSpec: return ty;
    case Struct: case Union: case Enum:
      return ((sueNode*)ty)->spec();
    case Tdef: {
      typeNode *base = def_type( ((tdefNode*) ty)->def());
      if(base) return base;
      else return ty;
    }
    default:
      cerr << "def_type: shouldn't have typ " << ty->typ() << endl;
      exit(1);
  }
} // def_type

arrayNode *NodeInfo::canonical(arrayNode *arr) {
  if(!arr) return NULL;
  typeNode *base = arr->type();
  arrayNode *result;
  switch(base->typ()) {
    case Prim:
      result = NULL;
      for(list<arrayNode*>::iterator c=_canonical1.begin();
          c!=_canonical1.end(); c++)
        if(((primNode*) (*c)->type())->basic() == ((primNode*)base)->basic()) {
          result=*c;  break;  // found
        }
      if(!result) {
        _canonical1.push_back(arr);
        result = arr;
      }
      break;
    case Array:
      base = canonical((arrayNode*) base);
      result = _canonical2[base];
      if(!result) result = _canonical2[base] = arr;
      break;
    default:
      base = def_type(base);
      result = _canonical2[base];
      if(!result) result = _canonical2[base] = arr;
  }
  return result;
} // canonical
#endif // not J_BREEZE

