
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

#ifndef NODEINFO_H
#define NODEINFO_H

#ifdef J_BREEZE
#include "j_breeze.h"
#include "arrayclass.h"
#else
#include "c_breeze.h"
#endif

/** @brief Helper functions that help saving AST nodes and other information to
 * file, and read back from file.
 * @ingroup JBreeze_Helpers
 *
 * This class helps to save program information, including AST nodes, to file,
 * and read back from file.
 * The file format is text and is line-sensitive, with one piece of data
 * (such as information on one AST node) per line. For example:
 * @code
 *     ... (16 preceding lines)
 *     Call:super(0) container=16 @sov/java/lang/System.java:58:4
 *     Constructor:Object() @sov/java/lang/Object.java:24:7
 *     Callgraph:17:18
 * @endcode
 * In the example, 16 preceding lines are omitted. Line 17 and 18 refers to a
 * call node and a constructor node, respectively. Line 19 is meant to say that
 * the call node at line 17 has callee represented by line 18.
 *
 * The class defines some functions that write the AST nodes. Each AST node
 * will be written as a line at most once.
 * User decides what node to write, and can supply more strings to write (such
 * as line 19 in the example which represents an AST edge), but such strings
 * have to be generated and formatted before submit for writing.
 * In read mode, there are functions to query for the strings at specific
 * lines.
 */
class NodeInfo {
private:
  FILE *_file;
  bool  _read_mode;
  bool  _verbose;

  //str_list strings;
  map<int,string> strings;
  map<int,Node*>  index2node;
  map<Node*,int>  node2index;
  int             _line;

#ifndef J_BREEZE
  // canonicalize arrays. for J-Breeze, already handled by ArrayClass.
  // for primitive base type
  static list<arrayNode*> _canonical1;
  // for reference base type, map from an array's component type to its
  // canonical array.
  static map<typeNode*,arrayNode*> _canonical2;
#endif

public:
  /** @name Constructors and destructors. */
  // @{
  /** @brief Constructor: Create an instance of this class.
   *
   * @param filename the file to read or write.
   * @param readmode true if the file is to be read, else data is to be
   *        written.
   * @param verbose verbose mode.
   *
   * If read mode is specified, the file's content is read.
   * If write mode is specified, a file is created, overwriting if it already
   * exists.
   */
  NodeInfo(string filename, bool readmode, bool verbose);

  /** @brief Destructor: Destroy an instance of this class. */
  ~NodeInfo();
  // @}

  /** @name General query and read-mode functions. */
  // @{
  /** @brief Query if it is read mode. */
  inline bool    read_mode(void) { return _read_mode; }

  /** @brief Obtain the string at a line.
   * @param i the line number of the string to obtain. */
  inline string  index(int i)          { return strings[i]; }

  /** @brief Obtain the AST node at a line.
   * @param i the line number of the node to obtain.
   * @returns the node if line <i>i</i> is an AST node, else return NULL. */
  inline Node   *indexNode(int i)      { return index2node[i]; }

  /** @brief Obtain the unique line number for an AST node.
   * @param node the AST node
   * @returns the line number for the node, 0 if NULL or the node was never
   * written or not read from file. */
  inline int     nodeIndex(Node *node) {
    if(!node) return 0;
    if(node->typ()==Array)
#ifdef J_BREEZE
      return node2index[ ArrayClass::canonical((arrayNode*)node) ];
#else
      return node2index[ canonical((arrayNode*)node) ];
#endif
    return node2index[node];
  }

  /** @brief Obtain number of lines in file. */
  inline int     nStrings() { return strings.size(); }
  // @}

  /** @name Write-mode functions.
   *
   * For functions that take a <i>container</i> parameter, the node to be
   * written is either a statement or expression. The <i>container</i> should
   * be the line number for an AST node already written, such that the latter,
   * such as a method node, `contains' the first node. That is, in the AST, the
   * first node should be an decestor of the second. Specifying this parameter
   * enables the information to be written to the file, so that later when the
   * file is read, the node can be located faster. */
  // @{
  /** @brief The current line number.
   *
   * Useful only in write mode. In write mode, one line of string is written to
   * the file at a time. The function returns the last written string's line
   * number. The first line written is line 1. */
  inline int     current_line(void)    { return _line; }

  /** @brief Write node for a type. */
  void writeType(typeNode *t);
#ifdef J_BREEZE
  /** @brief Write node for a Java class or interface. */
  void writeClassOrInterface(referenceNode *t);
  /** @brief Write node for a Java method. */
  void writeMethod(methodNode *m);
  /** @brief Write node for a Java constructor. */
  void writeConstructor(constructorNode *c);
  /** @brief Write node for a field variable. */
  void writeFieldvar(fieldvarNode *fv, declNode *d=0);
  /** @brief Write node for a instance or static initializer. */
  void writeFieldinit(fieldinitNode *fi);
#else
  /** @brief Write node for a procedure. */
  void writeProc(procNode *proc);
#endif

  /* for the following, container = line_number refering to the node containing
   * the expression/statement */

  /** @brief Write node for a statement.
   * @param s the statement node to write.
   * @param container the container for the statement. */
  void writeStmt(stmtNode *s, int container=-1);

#ifdef J_BREEZE
  /** @brief Write node for a call.
   * @param c the call node to write.
   * @param container the container for the call. */
  void writeCall(mcallNode *c, int container=-1);

  /** @brief Write node for a new-expression.
   * @param c the new-expression node to write.
   * @param container the container for the expression. */
  void writeNew(newNode *n, int container=-1);

#else
  /** @brief Write node for a call.
   * @param c the call node to write.
   * @param container the container for the call. */
  void writeCall(callNode *c, int container=-1);
#endif

  /** @brief Write node for a expression.
   * @param expr the expression node to write.
   * @param container the container for the expression.
   *
   * If the expression is a call or new-expression, @ref writeCall or @ref
   * writeNew should be used instead, respectively. */
  void writeExpr(exprNode *expr, int container=-1);

  /** @brief Write node for a declaration
   * @param d the declaration node to write.
   * @param container the container for the declaration. */
  void writeDecl(declNode *d, int container=-1);

  /** @brief Write a string constructed by user.
   *
   * The string should be contain a new line character '\n' followed and
   * terminated by the null character. */
  void writeString(string s);

#ifdef J_BREEZE
  /** @brief Obtain the fully-qualified name for a type.
   * @param ty the type whose name is wanted.
   * It can be any type, including anonymous or local classes, for which case
   * the name would include the location where the type is defined. */
  static string type_string(typeNode *ty);
#endif
  // @}

#ifndef J_BREEZE
  /** @brief Get the canonical array for a given array.
   *
   * Two arrays have same canonical array if they have the same component
   * type
   */
  static arrayNode *canonical(arrayNode *array);
#endif

private:
  class NodeLocator;
  /** @brief Locate an AST node.
   *  @internal */
  friend class NodeLocator;

  /** @name PRIVATE_MISC
   *
   * Misc methods. */
  // @{
  // extract the coord from inside the string ("...@coord")
  // also obtain the corresponding unitNode
  Coord getCoord(string s, unitNode **unit);

  // search for a class/interface in an unit. s is the fully qualified name.
  typeNode *getType(unitNode *unit, string s);

#ifdef J_BREEZE
  // search for a class/interface using standardPackage::findType
  typeNode *getLibType(string s);
#endif

#ifndef J_BREEZE
  static string type_name(typeNode *t);
  static typeNode *def_type(typeNode *ty);
#endif
  // @}

  /** @name PRIVATE_READ
   *
   * Methods for read-mode */
  // @{
  Node *getNode(string s);

  typeNode        *readType(string s);
#ifdef J_BREEZE
  typeNode        *readClassOrInterface(string s);
  methodNode      *readMethod(string s);
  constructorNode *readConstructor(string s);
  fieldvarNode    *readFieldvar(string s);
  fieldinitNode   *readFieldinit(string s);
  mcallNode       *readCall(string s);
  newNode         *readNew(string s);
#else
  procNode        *readProc(string s);
  typeNode        *readsuef(string s);
  callNode        *readCall(string s);
#endif
  Node            *readExprOrStmt(string s, NodeType typ);
  declNode        *readDecl(string s);

  bool match_args(string args, decl_list formals);
  // @}
};

#endif
