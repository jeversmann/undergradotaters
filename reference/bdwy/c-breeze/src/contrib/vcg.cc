
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

vcgWalker::vcgWalker(ostream& ostr, const string& comment,
                     string excludefilename)
  : Walker( Both, Subtree ), graph(ostr), currently_excluded(false) {
#ifdef J_BREEZE
  current_type.clear();
#endif
  exclude.clear();

  // Read the exclude file if present
  //char *excludefilename = getenv( "CALLG_EXCLUDE_FILE" );
  if( ! excludefilename.empty() ) {
    ifstream excludefile(excludefilename.c_str());
    if(! excludefile.is_open()) {
      cerr << "exclude file " << excludefilename << " cannot be opened\n";
      exit(1);
    } else {
      string name;
      while( excludefile >> name )
        exclude.push_back(name);
    }
  } //else cout << "no exclude file\n";

} // constructor

#ifdef J_BREEZE
bool vcgWalker::excluded(typeNode *type) {
  if(!type || (type->typ()!=Class && type->typ()!=Interface)) return true;
  if(type->coord().line() == Coord::Unknown.line()) return true;
  string name = typenameNode::type_name(type);
  packageNode *p = packageNode::packageOf(type);
  string pname = p->fullname();
  string fullname = pname + "." + name;

  // search through list
  for(strings::iterator s=exclude.begin(); s!=exclude.end(); s++)
    if(*s==name || *s==pname || *s==fullname) return true;
  return false;
} // excluded
#else
bool vcgWalker::excluded(procNode *proc) {
  string name = proc->decl()->name();

  // search through list
  return find(exclude.begin(), exclude.end(), name) != exclude.end();
} // excluded

bool vcgWalker::excluded(Coord coord) {
  if(coord.line() == Coord::Unknown.line()) return true;
  string filename = coord.filename();

  // search through list
  return find(exclude.begin(), exclude.end(), filename) != exclude.end();
} // excluded
#endif

#ifdef J_BREEZE
void vcgWalker::at_type(typeNode *type, Order order) {
  if(type->typ()!=Class && type->typ()!=Interface) return;
  if(order==Preorder) current_type.push_front(type);
  else                current_type.pop_front();
  if(current_type.empty())
    currently_excluded = true;
  else
    currently_excluded = excluded(current_type.front());
} // at_type
#else
void vcgWalker::at_proc(procNode *proc, Order order) {
  if(order==Postorder) currently_excluded = false;
  else                 currently_excluded = excluded(proc);
} // at_proc
#endif

void vcgWalker::print_comment(const string& comment) {
  graph << "// " << comment << endl; }

void vcgWalker::print_graph_attribute(const string& attribute,
                                      const string& value) {
  graph << endl;
  graph << "   " << attribute << " : " << value << endl;
}

void vcgWalker::print_node_attribute(const string& attribute,
                                     const string& value) {
  graph << "\t" << attribute << ": \"" << value << "\"" << endl;
}
void vcgWalker::print_node_attribute(const string& attribute, int value) {
  graph << "\t" << attribute << ": " << value << endl;
}

void vcgWalker::print_node_value(const string& attribute, const string& value) {
  graph << "\t" << attribute << ": " << value << endl;
}; // vcgWalker

