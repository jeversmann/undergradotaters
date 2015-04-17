// $Id: testNodeInfo.cc,v 1.4 2003/08/07 23:13:36 pnav Exp $
// ----------------------------------------------------------------------
//
//  C-Breeze
//  C Compiler Framework
// 
//  Copyright (c) 2003 University of Texas at Austin
// 
//  Samuel Z. Guyer
//  Adam Brown
//  Teck Bok Tok
//  Paul Arthur Navratil
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

#include "nodeinfo.h"
#include "linker.h"

/* test program for NodeInfo.
 * This program writes some AST nodes to a file name "file".
 * It also writes the callgraph to the same file.
 */

class W : public Walker {
public:
  NodeInfo *ni;
  int_list container;
  W() : Walker(Both, Subtree) {
    ni = new NodeInfo("file", false,true);
    container.push_back(-1);
  }

  void at_type(typeNode *type, Order ord) {
    if(ord==Postorder) {
      if(ni->nodeIndex(type) == container.back())
        container.pop_back();
      return;
    }
    if(type->typ()==Func) return;
    ni->writeType(type);
    if(ni->nodeIndex(type)>0)
      container.push_back(ni->nodeIndex(type));
  }
  void at_proc(procNode *proc, Order ord) {
    if(ord==Postorder) {
      if(ni->nodeIndex(proc) == container.back())
        container.pop_back();
      return;
    }
    ni->writeProc(proc);
    if(ni->nodeIndex(proc)>0)
      container.push_back(ni->nodeIndex(proc));
  }

#define CG_PREFIX "Callgraph:"
  void at_call(callNode *call, Order ord) {
    if(ord==Postorder) return;
    ni->writeCall(call);
    procNode *proc = call->proc();
    if(!proc) return;
    ni->writeProc(proc);
    ostringstream ost;
    ost << CG_PREFIX << ni->nodeIndex(call) << ":" << ni->nodeIndex(proc)
        << endl << '\0';
    ni->writeString(ost.str());
  }
  void at_decl(declNode *decl, Order ord)
  { if(ord==Postorder) return;
    ni->writeDecl(decl, container.back()); }
  void at_stmt(stmtNode *stmt, Order ord)
  { if(ord==Postorder) return;
    ni->writeStmt(stmt, container.back()); }
};

extern char *TypNames[];

class P : public Phase {
public:
  bool read;
  P(bool _read) : read(_read), Phase() {}
  void run() {
    if(!read) {
      Linker linker;
      linker.link();
      W w;
      for(unit_list_p u=CBZ::Program.begin(); u!=CBZ::Program.end(); u++)
        (*u)->walk(w);
      return;
    }

    /*
    cout << "#units: " << CBZ::Program.size() << endl;
    for(unit_list_p u=CBZ::Program.begin(); u!=CBZ::Program.end(); u++) {
      cout << "#defs = " << (*u)->defs().size() << endl;
      for(def_list_p d=(*u)->defs().begin(); d!=(*u)->defs().end(); d++)
        cout << (*d)->typ() << " @" << (*d)->coord() << endl;
      for(suespec_list_p p=(*u)->suespecs().begin();
          p!=(*u)->suespecs().end();p++)
        cout << (*p)->name() << " @" << (*p)->coord() << endl;
    }*/

    // read...
    NodeInfo *ni = new NodeInfo("file", true, true);
    int n = ni->nStrings();
    for(int i=1; i<=n; i++) {
      Node *node = ni->indexNode(i);
      if(!node) {
        string s = ni->index(i);
        int pos = s.find(CG_PREFIX);
        if(pos != 0) {
          cerr << "invalid line #" << i << endl;
          continue;
        }
        // get rid of prefix
        s = s.substr(pos+strlen(CG_PREFIX),string::npos);
        // get caller
        pos = s.find(':');
        string caller_str = s.substr(0,pos);
        Node *caller = ni->indexNode( atoi(caller_str.c_str()) );
        if(!caller) continue;
        assert(caller->typ() == Call);

        // get callees
        s = s.substr(pos+1,string::npos);
        Node *callee = ni->indexNode( atoi(s.c_str()) );
        if(!callee) continue;
        assert(callee->typ() == Proc);
        cout << "line " << i << " " << CG_PREFIX << ni->nodeIndex(caller)
             << "->" << ni->nodeIndex(callee) << endl;
      }
      else if(node->typ()==Array)
        cout << "line " << i << " Array " << type_name((arrayNode*)node)<<endl;
      else if(node->typ()==Ptr)
        cout << "line " << i << " Ptr " << type_name((ptrNode*)node)<<endl;
      else if(node->typ()==Func)
        cout << "line " << i << " Func " << type_name((funcNode*)node) <<endl;
      else
        cout << "line " << i << " typ=" << TypNames[node->typ()] << " @"
             << node->coord() << endl;
    }
  }
  string type_name(typeNode *t) {
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
        return type_name( ((funcNode*)t)->type() ) + "(func)";

      case Struct:
      case Union:
      case Enum:
        return string(TypNames[t->typ()]) + "-" + ((sueNode*)t)->spec()->name();

      case sueSpec:
        return string(TypNames[t->typ()]) + "-" + ((suespecNode*)t)->name();

      default:
        cout << "type_name: unknown typenode "
             << t->typ() << endl;
        exit(1);
    }
  } // type_name

};

Phases READ("read", new P(true));
Phases WRITE("write", new P(false));
