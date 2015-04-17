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

#ifndef CBZ_AST_H
#define CBZ_AST_H


// -- REF and TREE are empty defines for information only

#define REF
#define TREE

#include "register.h"

/** @defgroup AST The AST nodes
 *
 */

/**
 * @brief AST node base class.
 * @ingroup AST
 *
 * Each AST node corresponds to a C construction, such as a unary
 * expression or a while loop.  Subclasses are used to represent
 * specific constructions.
 *
 * To determine what type of AST node is contained in a Node, use the
 * typ() method.
 *
 * Each node has a source location, known as its "coord."  This
 * location specifies what part of the source C code a Node
 * corresponds to.
 *
 * A Node can have zero or more children.  Each child is a Node that
 * is "contained" within the construction represented by the Node.
 * For example, a binary expression node has two children: one for its
 * right operand, and one for its left operand.  The Walker and
 * Changer classes may be used to perform operations on a Node and all
 * of its ancestors.  See AST traversal for more information.  */

class Node
{

private:

  /** @brief The kind of AST node.
   *
   *  This should be set by the constructor for the specific node subclass
   *  being created. */

  NodeType     _typ;

  /** @brief The source location.
   *
   *  For nodes created during parsing, this indicates (roughly) where in
   *  the source code the node came from. For nodes created by other
   *  processing, you can either pass an existing Coord, or use
   *  Coord::Unknown. */

  Coord        _coord;

  /** @brief Whether this Node was enclosed in parenthases in the
   *  souce code.
   *
   *  This value is used to decide whether to insert extra parenthases
   *  when generating C code from the AST.  Note that parenthases will
   *  always be included when they are necessary to disambiguate an
   *  expression, regardless of the value of this variable. */

  bool         _parenthesized;

  /** @brief Annotations list.
   *
   *  This is a user-accessible list of Annotation objects that allows
   *  general information to be attached to any node.  Every element
   *  in the list should point to a valid Annotation; in particular,
   *  the list should contain no pointers to NULL values.  */

  annote_list  _annotations;

  /** @brief "Gen" flow value.
   *
   *  For dataflow analysis, this field can hold a flow value that
   *  stores information generated at this node.  When not in use, its
   *  value is NULL. */

  FlowVal * _gen;

  /** @brief "Kill" flow value.
   *
   *  For dataflow analysis, this field can hold a flow value that
   *  stores information killed at this node.  When not in use, its
   *  value is NULL.*/

  FlowVal * _kill;

  /** @name Memory leak detection.
   *  @internal
   *
   *  Internal code for detecting memory leaks.
   */
  //@{

  /** @internal Node unique identifier. */
  int _uid;

  /** @internal Count the total number of nodes. */
  static int _count;

  /** @internal Node count, sorted by node type. */
  static int _t_count[50];

  /** @internal Counter for unique node identifiers. */
  static int _uid_count;

  //@}

public:

  /** @name Garbage collection.
   *  @internal
   *
   *  Support for a simple mark-and-sweep garbage collector. The actual
   *  collection is invoked explicitly by the users code.
   */
  //@{

  /** @internal The list of all allocated nodes. */
  static node_list nodes;

  /** @internal A mapping of deleted nodes. */
  static map <Node *, bool> deleted_nodes;

  /** @internal The mark, indicating when the node is still in use. */
  bool mark;

  //@}

  /** @brief Construct a new node.
   *
   *  The new Node's annotation list is initialized to be empty; its
   *  GEN and KILL values are set to NULL.
   *
   *  @param typ the kind of node.
   *  @param coord the location of the construct in the source file.
   *  @param parenthesized true if the construct is parenthesized.
   */

  Node(NodeType typ, const Coord coord, bool parenthesized = false);

  /** @brief Copy constructor.
   *
   *  Construct a copy of the given node, with the same type,
   *  coordinate, and parenthasization.  Annotations and the GEN and
   *  KILL value are not copied: the annotation list is initialized to
   *  be empty, and the GEN and KILL values are set to NULL.
   *
   *  @param other the node to copy.  */

  Node(const Node & other);

  /** @brief Destructor.
   * 
   *  @warning Do not use the destructors to delete AST nodes. Instead,
   *  rely on the node garbage collector.
   *  @todo Fix the AST node garbage collector. */

  virtual ~Node();

  /** @name Accessors
   *
   *  Methods to get and set fields in the class. */
  //@{

  /** @brief Get the node type.
   *
   *  This has nothing to do with types in the source code. It indicates
   *  what kind of AST node this is (e.g., a unary expression node or a while 
   *  node).
   *
   *  @return the node type.  */

  inline NodeType typ() const { return _typ; }

  /** @brief Get the source location.
   *
   *  The Coord class holds a location in the input source (file,
   *  line, position). During parsing, each AST node created records
   *  the position of the source text that it represents.  However,
   *  subsequent phases may create or change nodes, so no guarantee is
   *  given about how the source location corresponds to the original
   *  source text.
   *
   *  @return the Coord structure indicating where the node came from in
   *  the source file.  */

  inline Coord coord() const { return _coord; }

  /** @brief Set the source location.
   *
   *  This location should indicate the position in the source text
   *  that this Node represents, or Coord::Unknown if it does not
   *  represent any node in the source text.
   *
   *  It is not common to set the source location of a node. Currently,
   *  only the compiler error messages actually make use of it.
   *
   *  @param coord the new location.  */

  inline void coord(const Coord coord) { _coord = coord; }

  /** @brief Get the parenthesized boolean.
   *
   *  @return true if the construct corresponding to this Node was
   *  parenthesized in the source.  */

  inline bool parenthesized() const { return _parenthesized; }

  /** @brief Set the parenthesized boolean.
   *
   *  This boolean determines whether this expression will be
   *  parenthesized in the output.  Note that that the parentheses
   *  will always be added when they are needed to disambiguate the
   *  output.  This variable only controls the use of "un-necessary"
   *  parentheses.
   *
   *  @param paren the new value of the parenthesized boolean.  */

  inline void parenthesized(bool paren) { _parenthesized = paren; }

  /** @brief Get the annotations list.
   *
   *  This method returns a modifiable reference to the list of
   *  annotations on the node. Users can add new annotations, search
   *  for annotations, or remove annotations.  Users are responsible
   *  for ensuring that every element of the annotation list points to
   *  a valid annotation.  In particular, no element should be NULL.
   *
   *  @see Annote class  */

  inline annote_list & annotations() { return _annotations; }

  /** @brief Get the "gen" flow value.
   *
   * This value is used in dataflow analyses to store information that
   * is generated at this node.  Note that each node has exactly one
   * "gen" flow value.
   *
   *  @return the "gen" flow value.  */

  inline FlowVal * gen() const { return _gen; }

  /** @brief Set the "gen" flow value.
   *
   * This value is used in dataflow analyses to store information that
   * is generated at this node.  Note that each node has exactly one
   * "gen" flow value.
   *
   * In order to set the flow value to be empty, call this method
   * with a value of NULL.
   *
   *  @param g the new gen flow value.  */

  inline void gen(FlowVal * g) { _gen = g; }

  /** @brief Get the "kill" flow value.
   *
   * This value is used in dataflow analyses to store information that
   * is killed at this node.  Note that each node has exactly one
   * "kill" flow value.
   *
   *  @return the "kill" flow value.  */

  inline FlowVal * kill() const { return _kill; }

  /** @brief Set the "kill" flow value.
   *  
   * This value is used in dataflow analyses to store information that
   * is killed at this node.  Note that each node has exactly one
   * "kill" flow value.
   *
   * To set the flow value to be empty, call this method with a
   * value of NULL.
   *
   *  @param k the new kill flow value.  */

  inline void kill(FlowVal * k) { _kill = k; }

  //@}

  /** @brief Report node count statistics.
   *
   *  The code can be configured to gather statistics about node usage
   *  according to type. This method prints the current state of that
   *  accounting information to standard out.  */

  static void report();

  /** @brief Return the node data type.
   *
   *  This method returns the C data type of a node. Several different
   *  kinds of nodes contain typeNode pointers. This method is just a
   *  unified way to access them. It does not attempt to compute the type,
   *  it just returns whatever typeNode pointer resides on the node. The
   *  subclasses that have types return them, and the others return null.
   *
   *  @return the typeNode pointer on the node.  */

  virtual typeNode * type() const { return 0; }

  /** @brief Return the base data type of a node.
   *
   *  This method differs from the Node::type() method in two
   *  respects. First, it follows some of the simple type inferences. For
   *  example, calling it on an idNode will return the type of its
   *  declaration. Second, the boolean argument indicates whether or not to
   *  follow typedef links.
   *
   *  @param TdefIndir pass true to follow typedefs to their definitions.  */

  virtual typeNode * base_type(bool TdefIndir) const;

  /** @brief Call base_type() with the argument true.
   */

  typeNode * datatype() const;

  /** @brief Call base_type() with the argument false.
   */
  typeNode * datatype_superior() const;

  /** @name AST traversal
   *
   *  These methods provide a uniform way to traverse the AST without
   *  explicitly testing the node types. They are all variations of the
   *  visitor design pattern. The input object is an instance of a
   *  user-defined subclass of Visitor, Walker or Changer. By overriding
   *  virtual methods, the user can define what to do at each kind of node
   *  that is encountered. See the C-Breeze documentation for more
   *  details. */
  //@{

  /** @brief Dispatch a Visitor.
   *
   *  This abstract method is the entry point for the
   *  visitor design pattern. Each node subclass defines a visit() method
   *  that calls the appropriate at_ method in the visitor. For more
   *  information see the Visitor documentation.
   *
   *  @param the_visitor the specific Visitor object to use.
   */

  virtual void visit(Visitor * the_visitor) =0;

  /** @brief Dispatch a Walker.
   *
   *  This abstract method works much like the visitor, but instead walks
   *  the entire underlying subtree calling the appropriate at_ method at
   *  each node. For more information see the Walker documentation.
   *
   *  @param the_walker the specific Walker object to use.  */

  virtual void walk(Walker & the_walker) =0;

  /** @brief Dispatch a Changer.
   *
   *  This abstract method works much the walker, but allows the tree to be
   *  changed.
   *
   *  @param the_changer the specific Changer object to use.
   *  @param redispatch pass true to revisit parts of the tree that have changed.  */

  virtual Node * change(Changer & the_changer, bool redispatch = false) =0;

  //@}

  /** @brief Run the dataflow analyzer.
   *
   *  Each subclass overrides this method to define it's semantics for
   *  dataflow analysis. It alters the input flow value to reflect the
   *  effect of the node within the given flow problem, calling dataflow()
   *  on it's subtrees as necessary. See the dataflow analysis
   *  documentation for more information.
   *
   *  @param v the flow value to pass through the node.
   *  @param fp the flow problem (specifying the transfer functions).  */

  virtual void dataflow(FlowVal * v, FlowProblem & fp) =0;

  /** @brief Clone the input node.
   *
   *  This is not a "deep" clone, so be careful. For a deep clone, use the
   *  clone_changer class.
   *
   *  @return a shallow copy of the node (no subtrees are copied).  */

  virtual Node * clone() const =0;

  /** @brief Generate C code.
   *
   *  Each subclass overrides this method to define how to produce the
   *  output C code. To use this method, pass an output_context and a null
   *  parent.
   *
   *  @param ct the output context which describes the formatting.
   *  @param par the parent node (or null, if at the top of a subtree).  */

  virtual void output(output_context & ct, Node * par) =0;
};

/** @brief Holds the AST of one translation unit.
 *  @ingroup AST
 *
 *  A translation unit corresponds to a single input source file (with all
 *  header files included). To parse a source file in, create a unitNode
 *  and then call the parse() method. However, this is typically done by
 *  the main driver, which also handles the flags provided on the command
 *  line.
 *
 *  All instances of unitNode created by the main driver are stored in a
 *  global list in the CBZ class.
 *
 *  The @ref NodeType is Unit.
 *
 *  In addition to containing the AST for the code, unitNode contains
 *  auxillary information gathered during the parsing. This includes:
 *  <ul>
 *    <li> A table of the type names in the file (only used during parsing).
 *    <li> A table of the struct/union tags (only used during parsing).
 *    <li> A list of any functions without explicit declarations.
 *    <li> The list of struct/union bodies.
 *  </ul> */

class unitNode : public Node
{

private:

  /** @brief The external definitions
   *
   *  The list of external definitions (top level) parsed in from the
   *  source. This is the top of the AST. */

  TREE def_list _defs;

  /** @brief Depth of the scope
   *  @internal 
   *
   *  The depth of the scope (only used during parsing). */

  int _symbol_level;

  /** @brief Table of type names
   *  @internal
   *
   *  This table is used during parsing to maintain the type names that are
   *  currently in scope. */

  Identifiers_table * _types;

  /** @brief Table of struct/union tags
   *  @internal
   *
   *  This table is used during parsing to store the names of struct and
   *  union tags. */

  Tags_table * _tags;

  /** @brief List of instructions for this translation unit.
   *
   *  The list of instructions in this translation unit that do not have an 
   *  associated procedure.  Currently this is just global variable declarations
   *  and such. */

  instruction_list _instructions;

  /** @brief Source file name
   *
   *  The name of the source file from which this AST was parsed. */

  string _input_file;

  /** @brief The output file name
   *
   *  The name of the output file to which the resulting code is written
   *  (only if the -c-code flag is given). */

  string _output_file;

  /** @brief The number of errors
   *
   *  The number of errors encountered during parsing. */

  int _errors;

  /** @brief The number of warnings
   *
   *  The number of warnings encountered during parsing. */

  int _warnings;

  /** @brief Undeclared functions
   *
   *  This list will contain a function declaration for any functions not
   *  explicitly declared in the source. */

  TREE decl_list _undef_funcs; 

  // -- Maintain a global list of all struct/enum/union. This means
  // that the spec component of a sueNode is never a part of the TREE.

  /** @brief struct/enum/union contents
   *
   *  During parsing, we maintain a separate, global list of the contents
   *  of struct/enum/union definitions. The reason is that this avoids the
   *  problem of which declaration "owns" the content definition. Instead,
   *  all uses of the s/u/e type simply refer to one of the elements of
   *  this list. */

  TREE suespec_list _suespecs;

  /** @brief Open the input file
   *  @internal
   *
   *  Open the input source file, running the preprocessor as necessary. */

  FILE * _open_input_file(str_list * cpp_flags);
  
public:

  /** @brief Create a new translation unit from a source file.
   *
   *  Instantiate a new unitNode with the given input and output source file
   *  names. This is typically done automatically by the front-end driver.
   *
   *  @param input_file the name of the input source file.
   *  @param output_file the name of the output source file.
   *  @param coord the location of the construct in the source file. */

  unitNode(string input_file, string output_file,
	   const Coord coord = Coord::Unknown);

  /** @brief Destroy a unitNode.
   * 
   *  @warning Do not use the destructors to delete AST nodes. Instead,
   *  rely on the node garbage collector. */

  virtual ~unitNode();

  /** @brief Parse the input file
   *
   *  Run the parser on the input file using the given preprocessor
   *  flags. The resulting AST is stored in the various unitNode fields (
   *  _defs, _types, etc.). 
   *
   * @param cpp_flags a list of strings, each one is a preprocessor flag. */

  void parse(str_list * cpp_flags, bool abortOnError = true);

  /** @brief Run fixup passes
   *
   *  Run the various fix-up passes, just as we would after parsing. */

  void fixup();
  
  /** @name Accessors
   *
   *  Methods to get and set fields in the class. */
  //@{

  // -- Fields

  inline def_list & defs() { return _defs; }
  inline const def_list & defs() const { return _defs; }

  inline int symbol_level() const { return _symbol_level; }
  inline Identifiers_table * types() const { return _types; }
  inline Tags_table * tags() const { return _tags; }
  inline instruction_list & instructions() { return _instructions; }

  inline string & input_file() { return _input_file; }
  inline string & output_file() { return _output_file; }

  inline int errors() const { return _errors; }
  inline void inc_errors() { ++_errors; }

  inline int warnings() const { return _warnings; }
  inline void inc_warnings() { ++_warnings; }

  inline decl_list & undef_funcs() { return _undef_funcs; }

  inline suespec_list & suespecs() { return _suespecs; }

  //@}

  // -- Merge in definitions...
  // NOTE: the defs is deleted

  /** @brief Merge in external definitions.
   *  @internal
   *
   *  Used during parsing to add external definitions as they are
   *  discovered and parsed into AST. The definitions are removed from the
   *  list and the list is deleted.
   *
   *  @param defs the list of definitions to add. */

  void merge_in(def_list * defs) {
   _defs.splice(_defs.end(), * defs); delete defs; }

  /** @brief Enter a new scope
   *  @internal
   *
   *  Enter a new scope by adding a level to the two symbol tables ( _types
   *  and _tags ). Only used during parsing. */

  void enter_scope();

  /** @brief Exit scope
   *  @internal
   *
   *  Exit a scope by popping off the current level in the two symbol
   *  tables ( _types and _tags ). Only used during parsing. */

  void exit_scope();

  /** @name AST Traversal
   *
   *  Methods to uniformly traverse the AST. See the documentation in the Node class. */
  //@{

  virtual void visit(Visitor * the_visitor);
  virtual void walk(Walker & the_walker);
  virtual Node * change(Changer & the_changer, bool redispatch = false);

  //@}

  // -- Dataflow

  virtual void dataflow(FlowVal * v, FlowProblem & fp);

  // -- Clone

  virtual Node * clone() const { return new unitNode ( *this ); }

  // -- Output

  virtual void output(output_context & ct, Node * par);
};

/** @brief Definition
 *  @ingroup AST
 *
 *  This class provides a base class for procNode and declNode, which are
 *  the two kinds of external definitions in a translation unit.
 *
 *  The @ref NodeType is determined by the subclass (either Proc or Decl).  */

class defNode : public Node
{
private:

  /** @brief Pragmas
   *
   * This list will contain any pragmas that precede the given
   * defNode. During parsing, they are collected into the CBZ::pragmas list
   * and then placed here in the defNode constructor. */

  text_list _pragmas;

public:

  /** @brief Create a definition node.
   *
   *  @param typ the node type (always either Decl or Proc).
   *  @param coord the location of the construct in the source file. */

  defNode(NodeType typ, const Coord coord);

  /** @name Accessors
   *
   *  Methods to get and set fields in the class. */
  //@{

  inline text_list & pragmas() { return _pragmas; }

  //@}

  /** @brief Destroy a defNode.
   * 
   *  @warning Do not use the destructors to delete AST nodes. Instead,
   *  rely on the node garbage collector. */

  virtual ~defNode();
};

/** @brief Declaration
 *  @ingroup AST
 *
 *  This class represents all declarations. At its core, it is just a name
 *  and a data type, with the optional initializer and bitsize
 *  expressions. However, it also holds information about the syntactic
 *  location of the declaration and any associated storage class. 
 *
 *  The @ref NodeType is Decl.  */

class declNode : public defNode
{

public:

  /** @brief Declaration location.
   *
   *  This type is used to record some syntactic context information about
   *  the declaration. This information is computed by the
   *  id_lookup_walker.
   *
   *  @see id_lookup_walker */

  typedef enum {
    UNKNOWN,  /**< No context provided */
    TOP,      /**< A top-level declaration (either a global variable or a
                   function declaration). */
    BLOCK,    /**< A regular local variable declared inside a function body. */
    FORMAL,   /**< A formal argument declared in the function interface. */
    SU,       /**< A struct/union field declaration. */
    ENUM,     /**< An element of an enum declaration. */
    PROC      /**< A function declaration that has the function definition. */
  } Decl_location;

  /** @brief Storage class specifier.
   *
   * This type indicates any storage class information associated with the
   * declaration. It is overloaded to also indicate when a declaration is a
   * @c typedef. */

  typedef enum {
    NONE, AUTO, EXTERN, REGISTER, STATIC, TYPEDEF
  } Storage_class;

  /** @brief Actual Storage Type
   *
   * This type indicates the actual storage that will be used for the data
   * associated with this declaration. */

  /** @brief Storage Location
   *
   * This structure indicates where the data associated with this declaration
   * will be stored.  It is initially setup by the lir_gen_walker phase and may
   * be modified by later phases. */

  struct Storage_location
  {
    /**< Constructor */
    Storage_location()
	{
		// init values
		_type = storageloc_unknown;
		_stack_offset = 0;
	}

    /**< The type of storage used for this data item. */
    enum 
    {
      storageloc_unknown,     /**< placeholder for invalid value */
      storageloc_register,	  /**< data is stored in a register */
      storageloc_stack,       /**< data is stored on the stack */
      storageloc_mem_global,  /**< data is in the global static memory area */
      storageloc_advanced,	  /**< data may be stored in different places at different times */

    } _type;
	  
    /**< If in a register, the register in which the data is stored. */
    Register               _register;

    /**< If on the stack, the stack offset where the data lives.
         This is usually a negative offset from the frame pointer. */
    int                   _stack_offset;

    /**< If in the global memory area, the label used to identify 
	     its location. */
    string               _global_location;

  };

  /** @name Output methods.
   *
   *  These two methods are used when generating C code to convert the
   *  Storage_class and Decl_location values into strings. */
  //@{

  /** @brief Return a string value containing the C code for this
   *  Declaration's storage class */

  static string storage_class_name(Storage_class sc);

  /** @brief Return a string value containing the C code for this
   *  Declaration's location. */

  static string decl_location_name(Decl_location dl);

  // @}

private:

  /** @brief Type
   *
   *  This AST subtree defines the type of the declaration. */

  TREE typeNode * _type;

  /** @brief Name
   *
   *  The name of the declaration. */

  string _name;

  /** @brief Declaration location
   *
   *  The syntactic location of the declaration.
   *  @see Decl_location */

  Decl_location _decl_location;

  /** @brief Storage class
   *
   *  Storage class specifier.
   *  @see Storage_class */

  Storage_class _storage_class;

  /** @brief Storage location
   *
   *  Storage location information.
   *  @see Storage_location_type */

  Storage_location _storage_location;

  /** @brief Redundant extern
   *
   *  This boolean is true if the declaration is a redundant extern
   *  declaration (as allowed in C). */

  bool _is_redundant_extern;

  /** @brief Initializer expression
   *
   *  This AST subtree specifies the initializer expression, or is null if
   *  there isn't one. */

  TREE exprNode * _init;

  /** @brief Bitsize expression
   *
   *  This AST subtree specifies the bitsize expression for bitfields, or
   *  is null if the declaration is not a bitfield. */

  TREE exprNode * _bitsize;

  /** @brief Reference count
   *
   *  Counts the number of identifier nodes that refer to this declaration.
   *  @see idNode */

  int _references;

  /** @brief List of ids
   *
   *  This list contains all the idNode's which refer to this declNode.
   *  @see idNode */
  
  REF id_list _ref_list;

  /** @brief Attributes
   *
   * Currently unused. */

  TREE attrib_list _attribs;

  /** @brief Is Return Value
   *
   * Indicates whether or not this value is the function's return value. */

  bool _is_return_value;

public:

  /** @brief Create a declaration.
   *
   *  Create a new declaration with the given parameters. This
   *  constructor allows most of the properties of the declaration to
   *  be specified explicitly.  The exceptions are the declNode's
   *  location, which is set to UNKNOWN, and the boolean
   *  redundant_extern, which is set to false.  User code should only
   *  need this constructor.  The following example declaration is
   *  used to point out which features each parameter represents.
   *
   *  @code
   *    static int x = 12;
   *  @endcode
   *
   *  @param name a C string specifying the declaration name ("x" in
   *      the example).  This value is copied.
   *  @param sc the storage class of the declaration ("static" in the
   *      example).
   *  @param the_type the type of the declaration ("int" in the
   *      example).
   *  @param init an initializer expression, or NULL for none ("12" in
   *      the example).
   *  @param bitsize a bitfield size expression, or NONE if the
   *      declaration does not declare a bitfield (the example does
   *      not declare a bitfield).
   *  @param coord the location of the construct in the source file. */

  declNode(const char * name, Storage_class sc, typeNode * the_type, 
	   exprNode * init, exprNode * bitsize,
	   const Coord coord = Coord::Unknown);

  /** @name Parser constructors
   *  @internal
   *
   *  These constructors are specifically designed for use by the
   *  parser. There is no need for user code to call them. */
  //@{

  /** @brief Create a declaration from an identifier
   *  @internal
   *
   *  Only used during parsing. Create a declaration from an identifier by
   *  using the identifiers name. The input idNode is @b not deleted. (Was
   *  called ConvertIdToDecl in ast.c)
   * 
   *  @param id the idNode that provides the name and Coord 
   *  @param sc the storage class of the declaration
   *  @param the_type the type of the declaration
   *  @param init an initializer expression, or 0 for none.
   *  @param bitsize a bitfield size expression, or 0 for none. */

  declNode(idNode * id, Storage_class sc, typeNode * the_type, 
	   exprNode * init, exprNode * bitsize);

  /** @brief Create an enum declaration
   *  @internal
   *
   *  Only used during parsing. Create a declaration for an element of an
   *  enum. All that's needed is the name and the optional initial
   *  value. The name comes from the input idNode, which is deleted. (Was
   *  called BuildEnumConst in sue.c).
   *
   *  @param name the idNode that provides the name and Coord.
   *  @param value the initial value, or 0 for none. */

  declNode(idNode * name, exprNode * value);

  /** @brief Create a type-only declaration.
   *  @internal
   *
   *  Only used during parsing. Convert an abstract declarator (a type with
   *  a storage class specifier) into a declaration with no name.
   *
   *  @param the_type the type of the declaration
   *  @param sc the storage class of the declaration */

  declNode(typeNode * the_type, Storage_class sc);

  //@}

  /** @brief Destroy a declNode.
   * 
   *  @warning Do not use the destructors to delete AST nodes. Instead,
   *  rely on the node garbage collector. */

  virtual ~declNode();

  /** @name Accessors
   *
   *  Methods to get and set fields in the class. */
  //@{

  /** @brief Return this declNode's base C data type
   */

  inline typeNode * type() const { return _type; }

  /** @brief Return this declNode's base C data type, and set the base
   *  C data type to be empty.
   */

  inline typeNode * get_type() { typeNode * out = _type; _type = 0; return out; }

  /** @brief Set this declNode's base C data type.
   *
   * To set the data type to be empty, call this method with a
   * value of NULL.  */

  inline void type(typeNode * the_type) { _type = the_type; }

  /** @brief Return the name of the variable declared by this
   *  declNode. */

  inline string & name() { return _name; }

  /** @brief Set the name of the variable declared by this
   *  declNode. */

  inline void name(string name) { _name = name; }

  /** @brief Return a Decl_location indicating the syntactic context
   *  in which this declaration occured. */

  inline Decl_location decl_location() const { return _decl_location; }

  /** @brief Set the syntactic context in which this declaration
   *  occured. */

  inline void decl_location(Decl_location loc) { _decl_location = loc; }

  /** @brief Return the storage class of the variable declared by this
   *  declNode. */

  inline Storage_class storage_class() const { return _storage_class; }

  /** @brief Set the storage class of the variable declared by this
   *  declNode.  */

  inline void storage_class(Storage_class sc) { _storage_class = sc; }

  /** @brief Return true iff this declaration is a redundant extern
   *  declaration.
   *
   *  Redundant extern declarations are allowed in C.
   */

  inline bool is_redundant_extern() const { return _is_redundant_extern; }

  /** @brief Set whether this declaration is a redundant extern
   *  declaration. */

  inline void set_redundant_extern(bool v) { _is_redundant_extern = v; }

  /** @brief Increment the number of identifier nodes that point to
   *  this declaration. */

  inline void inc_references() { _references++; }

  /** @brief Return the AST used to initialize the variable declared
   *  by this declNode. */

  inline exprNode * init() const { return _init; }

  /** @brief Define the AST node used to initialize the variable
   *  declared by this declNode.
   *
   *  To set the initializer of this declNode to be empty, call
   *  this method with a value of NULL.  */

  void init(exprNode * init) { _init = init; }

  /** @brief Return this Node's bitsize.  */

  inline exprNode * bitsize() const { return _bitsize; }

  /** @brief Define the bitsize for this declNode.
   *
   *  To set the bitsize of this declNode to be empty, call this
   *  method with a value of NULL. */

  inline void bitsize(exprNode * bitsize) { _bitsize = bitsize; }

  /** @brief Return the number of idNodes that point to this
   *  declaration.  */

  inline int references() const { return _references; }

  /** @brief Set the number of idNodes that point to this
   *  declaration. */

  inline void references(int references) { _references = references; }

  inline id_list & ref_list() { return _ref_list; }
  inline const id_list & ref_list() const { return _ref_list; }

  /** @brief Return a mutable reference to this declaration's
   *  attribute list. */

  inline attrib_list & attribs() { return _attribs; }

  /** @brief Return an immutable reference to this declaration's
   *  attribute list. */

  inline const attrib_list & attribs() const { return _attribs; }

  /** @brief Add each attribute in the given attribute list to this
   *  declNode's attribute list. */

  void merge_attribs(attrib_list * attribs);

  /** @brief Returns a reference to the storage location for this data.
   *  @see _storage_location */

  inline Storage_location & storage_location() { return _storage_location; }

  /** @brief Indictates whether or not this value is the function return value.
   *  @see _is_return_value */

  inline bool is_return_value() const { return _is_return_value; }
  inline void is_return_value( bool is ) { _is_return_value = is; }


  //@}

  /** @name Parser methods
   *  @internal
   *
   *  These methods are used during parsing to build up declarations. They
   *  are a consequence of the odd way that C allows declarations to be
   *  chained together:
   * 
   *  @code
   *  int * x, y, z[3], f();
   *  @endcode
   *
   *  The fragment above reall specifies four different declarations with
   *  different types. They share only the base type, @c int:
   *
   *  @code
   *  int * x;   // A pointer to an int
   *  int y;     // A plain int
   *  int z[3];  // An array of three ints
   *  int f();   // A function that returns an int
   *  @endcode
   *
   *  These methods allow the type to be built in stages: first building a
   *  compound type (such as pointer, array or function) and then going
   *  back and putting the base type at the bottom. */
  //@{

  //    Was SetDeclType in complex-types.c

  void       set_type(typeNode * the_type, Storage_class sc, ScopeState redeclare);
  declNode * set_type_and(typeNode * the_type, Storage_class sc, ScopeState redeclare);
  declNode * set_type_and(declNode * the_decltype, Storage_class sc, ScopeState redeclare);

  //    Was AppendDecl in complex-types.c

  void       inherit_type(decl_list * others, ScopeState redeclare);
  declNode * inherit_type_and(decl_list * others, ScopeState redeclare);

  //    Was ModifyType in complex-types.c

  void       modify_type(typeNode * the_type);
  declNode * modify_type_and(typeNode * the_type);
  declNode * modify_type_and(declNode * the_type);

  //    Was SetDeclInit in complex-types.c

  void       set_init(exprNode * init);
  declNode * set_init_and(exprNode * init);

  //    Was AddParameterTypes in procedure.c

  void       add_parameter_types(decl_list * types);
  declNode * add_parameter_types_and(decl_list * types);

  //    FinishDecl in complex-types.c

  void       finish(Storage_class sc);
  declNode * finish_and(Storage_class sc);

  //@}

  // -- Data type base

  typeNode * base_type(bool TdefIndir) const;

  /** @brief Return the type
   *
   *  Return the type after following any typedefs to the real type.
   *
   *  @return the type */

  typeNode * no_tdef_type();

  /** @brief SSA original declaration
   *
   *  For use with subdeclNode when the code is in SSA form. This method
   *  returns the original declaration from which the sub-declaration was
   *  derived.
   *
   *  @return the original declaration */

  virtual declNode * original() { return this; }

  /** @name AST Traversal
   *
   *  Methods to uniformly traverse the AST. See the documentation in the Node class. */
  //@{

  virtual void visit(Visitor * the_visitor);
  virtual void walk(Walker & the_walker);
  virtual Node * change(Changer & the_changer, bool redispatch = false);

  //@}

  // -- Dataflow

  virtual void dataflow(FlowVal * v, FlowProblem & fp);

  // -- Clone

  virtual Node * clone() const { return new declNode ( *this ); }

  // -- Output

  virtual void output(output_context & ct, Node * par);
};

/** @brief Indexed declaration
 *  @ingroup AST
 *
*
 *  Subclass of declNode used to create derived indexed
 *  declarations. It is currently only used in the SSA pass, which
 *  creates a series of indexed version of each variable.  A
 *  subdeclNode consists of a base declaration and a subscript.  For
 *  example, the indexed declaration for \f$x_1\f$ has the declaration
 *  for x as it base and has a subscript of 1.  Sometimes, the
 *  subdeclNode's base is also refered to as its "original"
 *  declaration.  The subdeclNode's subscript is also called its
 *  version or its index.
 *
 *  The @ref NodeType is Decl.  */

class subdeclNode : public declNode
{
private:

  REF declNode * _original;
  int _index;

public:

  /** @brief Create a sub-declaration
   *
   *  This constructor creates a new sub-declaration of the given input
   *  declaration. The index indicates which version (or subscript) to
   *  associate with the sub-declaration.
   *
   *  @param orig the original declaration from which this
   *      subdeclaration is derived
   *  @param index the index of the new sub-declaration */

  subdeclNode(declNode * orig, int index);

  /** @brief Destroy a subdeclNode.
   * 
   *  @warning Do not use the destructors to delete AST nodes. Instead,
   *  rely on the node garbage collector. */

  virtual ~subdeclNode();

  /** @name Accessors
   *
   *  Methods to get and set fields in the class. */
  //@{

  virtual declNode * original() { return _original->original(); }
  int index() const { return _index; }

  //@}

  /** @brief Get the name with index
   *
   *  This method returns the name of the declaration, with its index
   *  appended.  For example, when this method is called on the
   *  subdeclaration for \f$x_1\f$, the string "x@1" will be returned.
   *
   *  @return the name with the index appended */

  string name_with_index();

  /** @name AST Traversal
   *
   *  Methods to uniformly traverse the AST. See the documentation in the Node class. */
  //@{

  virtual void visit(Visitor * the_visitor);
  virtual void walk(Walker & the_walker);
  virtual Node * change(Changer & the_changer, bool redispatch = false);

  //@}

  // -- Clone

  virtual Node * clone() const { return new subdeclNode ( *this ); }

  // -- Output

  virtual void output(output_context & ct, Node * par);
};

/** @brief Procedure definition
 *  @ingroup AST
 *
 *  This class represents a procedure (or function) definition. The
 *  definition consists of a declarations (which gives the name and
 *  interface of the procedure) and the procedure body. 
 *
 *  The @ref NodeType is Proc.  */

class procNode : public defNode
{

private:

  /** @brief Procedure declaration
   *
   *  The declaration of the procedure consists of the name, plus the
   *  type, which specifies the function interface. The type is always
   *  a funcNode.  If _decl is NULL, then the procedure's declaration
   *  is empty.
   */

  TREE declNode * _decl;

  /** @brief Procedure body
   *
   *  The procedure body is represented as a statement block */

  TREE blockNode * _body;

  /** @brief List of instructions for this procedure
   *
   *  The list of instructions that was generated for this procedure. */

  instruction_list _instructions;

  /** @brief Lir Blocks.
   *
   *  The basic blocks for this procedure's LIR code. */

  LirBlockList _lir_blocks;

  /** @brief Last Stack Local
   *
   *  The last stack location that was allocated to a local variable for this procedure.
   *  This is used to implement the stack_alloc_local routine in this class. 
   *
   *  We are storing the last allocated location because we assume the stack grows down,
   *  so we need to know where the last thing we allocated went, in order to put the next
   *  thing below it.  The next thing will have its own size and alignment requirements, 
   *  so we have to just wait until we encounter it to know where it will go. */

  int _last_stack_local;

   /** @brief Stack Frame Size
   *
   *  The total size of the stack frame for this procedure.  Computed by backend. */

  unsigned int _stack_frame_size;

 /** @brief Current procedure during parsing
   *  @internal
   *
   *  This field is only used during parsing to keep track of the current
   *  procedure being parsed. */

  static procNode * _current;


  /** @name Dataflow analysis
   *  @internal
   *
   *  These fields are used to record the flow values and the entry and
   *  exit of the procedure. They are only used by the dataflow analyzer
   *  and show not be directly accessed by the user.
   *
   *  @see FlowProblem */
  //@{

  FlowVal * _at_entry;
  FlowVal * _at_exit;

  //@}

public:

  /** @brief Create a new procedure
   *
   *  @param decl the procedure declaration (always type funcNode )
   *  @param body the code of the procedure body 
   *  @param coord the location of the construct in the source file. */

  procNode(declNode * decl, blockNode * body,
	   const Coord coord = Coord::Unknown);

  /** @brief Create a procedure during parsing
   *  @internal
   *
   *  This constructor is only used during parsing. Among other things, it
   *  handles procedures for old K-and-R style function declarations. (Was
   *  DefineProc in procedure.c)
   *
   *  @param old_style true for old K-and-R style function declarations
   *  @param decl the procedure declaration (always type funcNode ) */

  procNode(bool old_style, declNode * decl);

  /** @brief Destroy a procNode.
   * 
   *  @warning Do not use the destructors to delete AST nodes. Instead,
   *  rely on the node garbage collector. */

  virtual ~procNode();

  /** @name Accessors
   *
   *  Methods to get and set fields in the class. */
  //@{


  /** @brief Return the declaration for this procNode.
   *
   *  The declaration of the procedure consists of the name, plus the
   *  type, which specifies the function interface. The declaration's
   *  type is guaranteed to be a funcNode. */

  inline declNode * decl() const { return _decl; }

  /** @brief Return the declaration for this procNode, and set it to
   *   be empty.
   *
   *  The declaration of the procedure consists of the name, plus the
   *  type, which specifies the function interface. The declaration's
   *  type is guaranteed to be a funcNode.  */

  inline declNode * get_decl() { declNode * out = _decl; _decl = 0; return out; }

  /** @brief Set the declaration for this procNode to the given
   * value.
   *
   * To set this procNode's declaration to be empty, use a value of
   * NULL. */

  inline void decl(declNode * decl) { _decl = decl; }

  /** @brief Return this procNode's body.  */

  inline blockNode * body() const { return _body; }

  /** @brief Return the body for this procNode, and set it to be
   * empty.  */

  inline blockNode * get_body() { blockNode * out = _body; _body = 0; return out; }

  /** @brief Set this procNode's body to the given value.
   *
   * To set this procNode's body to be empty, use a value of
   * NULL. */

  inline void body(blockNode * body) { _body = body; }

  /** @brief Access the list of instructions for this procedure. */

  inline instruction_list & instructions() { return _instructions; }

  /** @brief Access the list of LIR blocks for this procedure. */

  inline LirBlockList & lir_blocks() { return _lir_blocks; }

  /** @brief Set the initial value for the first stack local parameter. */

  void set_initial_stack_local_offset( int offset );

  /** @brief Allocate stack space for a local declaration.  Returns the assigned
   * stack location, and also sets this information into the declNode. If a particular
   * size is required, pass that size, otherwise pass zero.  If a size is specified,
   * it is assumed that the alignment is the same as the size.  */

  int alloc_stack_local( declNode * decl, unsigned int size = 0 );

  /** @brief Find out how much stack space is required for local variables in this
   * procedure. */

  int get_locals_size() { return abs( _last_stack_local ); }

  /** @brief Get/set total stack frame size for this procedure. */

  unsigned int stack_frame_size() { return _stack_frame_size; }
  void stack_frame_size( unsigned int size ) { _stack_frame_size = size; }

  // -- Flow values (for IP analyzer)

  /** @brief Return this Node's at_entry dataflow analysis value.
   *  @internal */

  inline FlowVal * at_entry() const { return _at_entry; }

  /** @brief Set this procNode's entry flow value
     * @internal */

  inline void at_entry(FlowVal * ae) { _at_entry = ae; }

  /** @brief Return this Node's at_exit dataflow analysis value.
   *  @internal */

  inline FlowVal * at_exit() const { return _at_exit; }

  /** @brief Set this procNode's exit flow value
   * @internal */

  inline void at_exit(FlowVal * ae) { _at_exit = ae; }

  //@}

  /** @name Control-flow Graph
   *
   *  These methods return the entry and exit basic blocks when a procedure
   *  is in control-flow graph form. Currently, the convention is that the
   *  first basic block is the entry and the last basic block is the exit;
   *  however, the user should not depend on this fact.
   *
   *  The control-flow graph is built using the cfg_changer.
   *
   *  @see cfg_changer */
  //@{

  /** @brief Entry basic block
   *
   *  @return the entry basic block */

  basicblockNode * entry() const;

  /** @brief Exit basic block
   *
   * @return the exit basic block */

  basicblockNode * exit() const;

  //@}

  /** @name Parser methods
   *  @internal
   *
   *  These methods are only used during parsing to build procNode objects
   *  from function definitions. */
  //@{

  // -- DefineProc in procedure.c

  void       define(blockNode * body); 
  procNode * define_and(blockNode * body); 

  static procNode * current() { return _current; }

  //@}

  // -- Data type base

  typeNode * base_type(bool TdefIndir) const;

  /** @name AST Traversal
   *
   *  Methods to uniformly traverse the AST. See the documentation in the Node class. */
  //@{

  virtual void visit(Visitor * the_visitor);
  virtual void walk(Walker & the_walker);
  virtual Node * change(Changer & the_changer, bool redispatch = false);

  //@}

  // -- Dataflow

  virtual void dataflow(FlowVal * v, FlowProblem & fp);

  // -- Clone

  virtual Node * clone() const { return new procNode ( *this ); }

  // -- Output

  virtual void output(output_context & ct, Node * par);
};

/** @brief Type node
 *  @ingroup AST
 *
 *  This class provides a base class for all nodes that represent C data
 *  types. Types are represented in the tree with the outer-most type
 *  specifier at the top and the primitive base type at the bottom. For
 *  example, "int * a[3]" is translated into a structure like this:
 *  arrayNode --> ptrNode -->primNode (int). 
 *
 *  The @ref NodeType is determined by the subclass.  */

class typeNode : public Node
{

public:

  /** @brief Type qualifiers
   *
   * This enum holds the possible type qualifiers. The special @c
   * COMPATIBLE value indicates which type qualifiers are relevant when
   * comparing two types for compatibility: const and volatile. */

  typedef enum {
    NONE     = 0x0,     /**< No type qualifier */
    CONST    = 0x1,     /**< Type is const */
    VOLATILE = 0x2,     /**< Type is volatile */
    INLINE   = 0x4,     /**< Type is inline */

    COMPATIBLE = 0x3    /**< Mask the qualifiers that are relevant to type compatibility */
  } Type_qualifiers;

  /** @brief Convert type qualifiers to string
   *
   *  This method is used when generating C code to convert the type
   *  qualifiers into string form.
   *
   *  @param tq the type qualifiers to convert
   *  @return the string form of the type qualifiers */

  static string type_qualifiers_name(Type_qualifiers tq);

  /** @brief Integral promotions
   *
   *  This method is used during parsing to convert smaller types (@c char,
   *  @c short, bit-fields and enums) into integers according to the rules
   *  in ANSI 6.2.1.1. In addition, our version converts @c float into @c
   *  double.
   *
   *  @see exprNode::integral_promotions()
   *
   *  @param old_type the original small type to be promoted
   *  @return the resulting integral type */

  static typeNode * integral_promotions(typeNode * old_type);

  /** @brief Usual arithmetic conversions
   *
   *  From ANSI 6.2.1.5: Many binary operators that expect operands of
   *  arithmetic type cause conversions and yield result types in a similar
   *  way. The purpose is to yield a common type, which is also the type of
   *  the result.
   *
   *  This method takes the types of the left and right operands and
   *  returns a pair of types indicating the conversions of the two
   *  operands, respectively. When necessary, these conversion include the
   *  integral promotions.
   *
   *  @see integral_promotions()
   *  @see exprNode::usual_arithmetic_conversions()
   *
   *  @param left the type of the left operand
   *  @param right the type of the right operand
   *  @return a pair of converted types for the left and right, respectively */

  static pair<typeNode *, typeNode *> usual_arithmetic_conversions(typeNode * left,
								   typeNode * right);

private:

  /** @brief The sub-type
   *
   *  This field holds the sub-type for compound types such as array,
   *  pointer, and function declaration. */

  TREE typeNode * _type;

  /** @brief Type qualifiers
   *
   *  The type qualifiers for this type */

  Type_qualifiers _type_qualifiers;

public:

  /** @brief Create a new type
   *
   *  Create a type node of the appropriate node type. This constructor is
   *  only used by the subclasses, which represent specific types.
   *
   *  @param typ the kind of node (passed to the superclass constructor)
   *  @param tq the type qualifiers
   *  @param subtype the subtype, or 0 if there is none
   *  @param coord the location of the construct in the source file. */

  typeNode(NodeType typ, Type_qualifiers tq, typeNode * subtype,
	   const Coord coord);

  /** @brief Destroy a typeNode.
   * 
   *  @warning Do not use the destructors to delete AST nodes. Instead,
   *  rely on the node garbage collector. */

  virtual ~typeNode();

  /** @name Accessors
   *
   *  Methods to get and set fields in the class. */
  //@{

  /** @brief Return this typeNode's subtype. */

  inline typeNode * type() const { return _type; }

  /** @brief Return this typeNode's subtype, and set it to
   *  be empty. */

  inline typeNode * get_type() { typeNode * out = _type; _type = 0; return out; }

  /** @brief Set this typeNode's subtype.
   *
   * To set the subtype to be empty, call this method with a
   * value of NULL.  */

  inline void type(typeNode * the_type) { _type = the_type; }

  /** @brief Return this typeNode's type qualifiers. */

  inline Type_qualifiers type_qualifiers() const { return _type_qualifiers; }

  /** @brief Set this typeNode's type qualifiers. */

  inline void type_qualifiers(Type_qualifiers the_tq) { _type_qualifiers = the_tq; }

 /** @brief Return a string representation of this typeNode's type qualifiers.  */
  inline string type_qualifiers_name() { return type_qualifiers_name(type_qualifiers()); }

  /** @brief Add a new type qualifier to this typeNode. */

  inline void add_type_qualifiers(Type_qualifiers the_tq)
  { _type_qualifiers = Type_qualifiers(_type_qualifiers | the_tq); }

  /** @brief Add a new type qualifier to this typeNode, and return the
   *  typeNode.*/

  inline typeNode * add_type_qualifiers_and(Type_qualifiers the_tq)
  { add_type_qualifiers(the_tq); return this; }

  /** @brief Remove a type qualifier from this typeNode. */

  inline void remove_type_qualifiers(Type_qualifiers the_tq)
  { _type_qualifiers = Type_qualifiers(_type_qualifiers & ~the_tq); }

  //@}

  /** @name Parser methods
   *  @internal
   *
   *  These methods are only used during parsing. (Was FinishType in
   *  complex-types.c) */
  //@{

  void       finish();
  typeNode * finish_and();

  // -- Set base type
  //    Was SetBaseType from complex-types.c

  void       set_base_type(typeNode * base);
  typeNode * set_base_type_and(typeNode * base);

  // -- SUE complete

  void verify_sue_complete();

  //@}

  /** @name Type comparison operators
   *
   *  These operators are used to compare types. They recursively descend
   *  the compound types and verify the structure. There are several
   *  different variations, depending on the level of strictness. In all
   *  cases, typedefs are followed so they don't interfere with the
   *  comparison. All operators use the static equal_to() method.
   *
   *  @see equal_to() */
  //@{

  /** @brief Strict type comparison
   *
   *  Compare this type to the given type, requiring all type qualifiers to
   *  be the same.
   *
   *  @param second the right type in the comparison
   *  @return true if the types are the same */

  bool operator==(typeNode & second) {
    return equal_to(this, & second, true, true); }

  /** @brief Weaker type comparison
   *
   *  Compare this type to the given type, masking off type qualifiers that
   *  don't affect compatibility of types.
   *  @see Type_qualifiers
   *
   *  @param second the right type in the comparison
   *  @return true if the types are the same */

  bool operator<=(typeNode & second) {
    return equal_to(this, & second, false, false); }

  /** @brief Strict type inequality
   *
   *  This is just a negation of the operator==
   *
   *  @param second the right type in the comparison
   *  @return true if the types are the same */

  bool operator!=(typeNode & second) {
    return ! ( (*this) == second ); }

  //@}

  /** @brief Type comparison
   *
   *  This static method compares two types, descending into the subtypes
   *  and following typedefs as necessary. The two boolean arguments
   *  control how strict the algorithm is with respect to type
   *  qualifiers. Passing true requires all type qualifiers to be the
   *  same. Passing false only requires those type qualifiers that affect
   *  compatibility to be the same. (Was TypeEqualQualified in type.c)
   *
   *  This routine relies on the qualified_equal_to() methods on each kind
   *  of typeNode to perform the appropriate comparison and dispatch to the
   *  sub-type when necessary.
   *
   *  @see qualified_equal_to()
   *
   *  @param first the first type
   *  @param second the second type
   *  @param strict_toplevel pass true to require strict comparison of top-level types
   *  @param strict_recursive pass true to require strict comparson of sub-types
   *  @return true, if the types pass the equality test */

  static bool equal_to(typeNode * first, typeNode * second,
		       bool strict_toplevel, bool strict_recursive);

  /** @brief Virtual type comparison routine
   *
   *  Each typeNode subclass overrides this routine to provide its specific
   *  type comparison. This is used by the static equal_to() method to
   *  perform general deep type comparison.
   *
   *  @see equal_to()
   *
   *  @param other the type to compare against
   *  @param strict_toplevel pass true to require strict comparison of top-level types
   *  @param strict_recursive pass true to require strict comparson of sub-types
   *  @return true, if the types pass the equality test */

  virtual bool qualified_equal_to(typeNode * other,
				  bool strict_toplevel, bool strict_recursive);

  /** @brief Unwind typedefs
   *  @internal
   *
   *  Mainly used in the type comparison code, this routine follows the
   *  chain of typedefs, gathering up type qualifiers along the way.
   *
   *  @param the_tq the value used to collect up type qualifiers
   *  @return the type at the end of the typedef chain */

  typeNode * unwind_tdefs(Type_qualifiers & the_tq);

  typeNode * no_tdef_type();

  /** @brief Follow typedefs
   *
   *  Follow the chain of typedefs from this type, returning the underlying
   *  (non-typedef) type.
   *
   * @return the type at the end of the typedef chain */
  typeNode * follow_tdefs();

  /** @name Type predicates
   *
   *  These methods support a variety of useful type predicates, most of
   *  which are self-explanatory. Each typeNode subclass overrides the
   *  methods for which it returns true. */
  //@{

  virtual bool is_char() const { return false; }
  virtual bool is_int() const { return false; }
  virtual bool is_float() const { return false; }
  virtual bool is_void() const { return false; }
  virtual bool is_ellipsis() const { return false; }

  virtual bool is_integer() const { return false; }
  virtual bool is_arithmetic() const { return false; }
  virtual bool is_scalar() const { return false; }
  virtual bool is_aggregate() const { return false; }
  virtual bool is_derived() const { return false; }
  virtual bool is_pointer() const { return false; }

  //@}

  /** @brief Usual unary conversion type
   *
   *  The purpose of this method escapes me. The constNode class seems to
   *  use it, but all it does is return itself. No other typeNode overrides
   *  it. */

  virtual typeNode * usual_unary_conversion_type() { return this; }

  // -- Data type base

  virtual typeNode * base_type(bool TdefIndir) const {
    return (typeNode *)this; }

  // -- Deep base type

  typeNode * deep_base_type();

  // -- Dataflow
  //    Note: All type nodes share a no-op dataflow function

  virtual void dataflow(FlowVal * v, FlowProblem & fp) { }

  // -- Output

  virtual void output(output_context & ct, Node * par);

  /** @brief Output a type
   *  @internal
   *
   *  This method generates the C code form of a type expression. It should
   *  only be called by the output() method. Each typeNode subclass
   *  overrides this method to produce the appropriate output.
   *
   *  @param ct the output context
   *  @param par the parent containing node
   *  @param context the associativity of the parent type (determine parens)
   *  @param q the type qualifiers */

  virtual void output_type(output_context & ct, Node * par, Assoc context, Type_qualifiers q) =0;
};

/** @brief Primitive type node
 *  @ingroup AST
 *
 *  This class represents a primitive type and is really just an AST node
 *  container for a basic_type. Note that this includes the @c void
 *  type. 
 *
 *  The @ref NodeType is Prim.  */

class primNode : public typeNode
{

private:

  /** @brief The basic type
   *
   *  The basic_type class represents all the different variations of
   *  primitive types, such as int, char, float, etc. */

  basic_type _basic;

public:

  /** @brief Create a new primitive type
   *
   *  The new type has the given basic type and type qualifiers.
   *
   *  @param tq the type qualifiers
   *  @param basic the underlying basic type
   *  @param coord the location of the construct in the source file. */

  primNode(Type_qualifiers tq, basic_type basic,
	   const Coord coord = Coord::Unknown);

  /** @brief Create a new default type primitive type
   *
   *  The new type has the given type qualifiers and the default basic type
   *  of int. This default is determined by calling the no-arg constructor
   *  for basic_type. (Was MakeDefaultPrimType in type.c)
   *
   *  @param tq the type qualifiers
   *  @param coord the location of the construct in the source file. */

  primNode(Type_qualifiers tq,
	   const Coord coord = Coord::Unknown);

  /** @brief Create a new default primitive type
   *
   *  The new type has no type qualifiers and the default basic type of
   *  int. This default is determined by calling the no-arg constructor for
   *  basic_type. 
   *
   *  @param coord the location of the construct in the source file. */

  primNode(const Coord coord);

  /** @brief Create a new primitive type without type qualifiers
   *
   *  The new type has the given basic type and no type qualifiers. (Was
   *  StartPrimType in type.c)
   *
   *  @param basic the underlying basic type
   *  @param coord the location of the construct in the source file. */

  primNode(basic_type basic, const Coord coord = Coord::Unknown);

  /** @brief Destroy a primNode.
   * 
   *  @warning Do not use the destructors to delete AST nodes. Instead,
   *  rely on the node garbage collector. */

  virtual ~primNode();

  /** @name Accessors
   *
   *  Methods to get and set fields in the class. */
  //@{

  //    Return a reference so basic_type can be modified...

  inline const basic_type & basic() const { return _basic; }
  inline basic_type & basic() { return _basic; }
  inline void basic(basic_type basic) { _basic = basic; }

  //@}

  /** @name Parser methods
   *
   *  These methods are only used during parsing. */
  //@{
  // -- FinishPrimType from type.c

  void       finish();
  primNode * finish_and();

  // -- MergePrimTypes from type.c (other is deleted)

  void       merge_in(primNode * other);
  primNode * merge_in_and(primNode * other);

  //@}

  /** @name Type predicates
   *
   *  These methods support a variety of useful type predicates, most of
   *  which are self-explanatory. Each typeNode subclass overrides the
   *  methods for which it returns true. See typeNode for details. */
  //@{

  virtual bool is_int() const      { return _basic.is_int(); }
  virtual bool is_char() const     { return _basic.is_char(); }
  virtual bool is_float() const    { return _basic.is_float(); }
  virtual bool is_void() const     { return _basic.is_void(); }
  virtual bool is_ellipsis() const { return _basic.is_ellipsis(); }

  virtual bool is_integer() const  { return _basic.is_integer(); }
  virtual bool is_arithmetic() const { return _basic.is_arithmetic(); }
  virtual bool is_scalar() const   { return _basic.is_scalar(); }

  //@}

  // -- Type equal

  bool qualified_equal_to(typeNode * node2,
			  bool strict_toplevel, bool strict_recursive);

  /** @name AST Traversal
   *
   *  Methods to uniformly traverse the AST. See the documentation in the Node class. */
  //@{

  virtual void visit(Visitor * the_visitor);
  virtual void walk(Walker & the_walker);
  virtual Node * change(Changer & the_changer, bool redispatch = false);

  //@}

  // -- Clone

  virtual Node * clone() const { return new primNode ( *this ); }

  // -- Output

  virtual void output_type(output_context & ct, Node * par, Assoc context, Type_qualifiers q);
};

/** @brief Typedef type node
 *  @ingroup AST
 *
 *  This class represents a typedef type. Since these types are just names
 *  for other types, objects of this class refer to the real underlying
 *  types that they represent. The original types can by found on the
 *  declaration with the typedef storage class specifier.
 *
 *  @note This node does not refer to the declaration of the typedef, but
 *  rather the use of the type name.
 *
 *  The @ref NodeType is Tdef.
 *
 *  @see declNode
 *  @see Storage_class */

class tdefNode : public typeNode
{

private:

  /** @brief Name of the type
   *
   *  This name is the same as the name in the declaration that introduced
   *  the typedef. */

  string _name;

  /** @brief The underlying type
   *
   *  This is a reference to the underlying type. This reference is set up
   *  by the id_lookup_walker.
   *
   * @see id_lookup_walker */

  REF typeNode * _def;

public:

  /** @brief Create a new typedef type
   *
   *  The new typedef type has the given name and type qualifiers. The name
   *  should by in the scope of an appropriate typedef declaration.
   *
   *  @param tq the type qualifiers
   *  @param name the name of the typedef type (copied; not deleted)
   *  @param coord the location of the construct in the source file. */

  tdefNode(Type_qualifiers tq, const char * name,
	   const Coord coord = Coord::Unknown);

  /** @brief Create a typedef type from an identifier
   *  @internal
   *
   *  This method is only used during parsing to convert an identifier into
   *  a typedef name. The identifier provides the name of the type. The
   *  idNode is not deleted. (Was ConvertIdToTdef in ast.c)
   *
   *  @param the_id the identifier providing the type name (and Coord)
   *  @param tq the type qualifiers
   *  @param the_type the actual underlying type to refer to */

  tdefNode(idNode * the_id, Type_qualifiers tq, typeNode * the_type);

  /** @brief Destroy a tdefNode.
   * 
   *  @warning Do not use the destructors to delete AST nodes. Instead,
   *  rely on the node garbage collector. */

  virtual ~tdefNode();

  /** @name Accessors
   *
   *  Methods to get and set fields in the class. */
  //@{

  /** @brief Return the name of this typedef Node. */

  inline string & name() { return _name; }

  /** @brief Set the name of this typedef Node to the given value. */

  inline void name(string name) { _name = name; }

  /** @brief Return this typedef Node's type definition. */

  inline typeNode * def() const { return _def; }

  /** @brief Set this typedef Node's type definition
   *
   * To set the type definition to be empty, call this method with a
   * value of NULL.  */

  inline void def(typeNode * d) { _def = d; }

  //@}

  // -- Data type base

  typeNode * base_type(bool TdefIndir) const;

  // -- Data type predicates

  /** @name AST Traversal
   *
   *  Methods to uniformly traverse the AST. See the documentation in the Node class. */
  //@{

  virtual void visit(Visitor * the_visitor);
  virtual void walk(Walker & the_walker);
  virtual Node * change(Changer & the_changer, bool redispatch = false);

  //@}

  // -- Clone

  virtual Node * clone() const { return new tdefNode ( *this ); }

  // -- Output

  virtual void output_type(output_context & ct, Node * par, Assoc context, Type_qualifiers q);
};

/** @brief Pointer type node
 *  @ingroup AST
 *
 *  This class represents the pointer-to compound type. The sub-type is the
 *  type that is pointed to. For example, for a node to represent "int **",
 *  the subtype would be "int *". 
 *
 *  The @ref NodeType is Ptr.  */

class ptrNode : public typeNode
{

private:

public:

  /** @brief Create a pointer type
   *
   *  The new pointer type has the given type qualifiers and sub-type (the
   *  type being pointed to).
   *
   *  @param tq the type qualifiers
   *  @param the_type the sub-type (the type being pointed to)
   *  @param coord the location of the construct in the source file. */

  ptrNode(Type_qualifiers tq, typeNode * the_type,
	  const Coord coord = Coord::Unknown);

  /** @brief Destroy a ptrNode.
   * 
   *  @warning Do not use the destructors to delete AST nodes. Instead,
   *  rely on the node garbage collector. */

  virtual ~ptrNode();

  // -- Type equal

  bool qualified_equal_to(typeNode * node2,
			  bool strict_toplevel, bool strict_recursive);

  /** @name Type predicates
   *
   *  These methods support a variety of useful type predicates, most of
   *  which are self-explanatory. Each typeNode subclass overrides the
   *  methods for which it returns true. See typeNode for details. */
  //@{

  virtual bool is_scalar() const { return true; }
  virtual bool is_derived() const { return true; }
  virtual bool is_pointer() const { return true; }

  //@}

  /** @name AST Traversal
   *
   *  Methods to uniformly traverse the AST. See the documentation in the Node class. */
  //@{

  virtual void visit(Visitor * the_visitor);
  virtual void walk(Walker & the_walker);
  virtual Node * change(Changer & the_changer, bool redispatch = false);

  //@}

  // -- Clone

  virtual Node * clone() const { return new ptrNode ( *this ); }

  // -- Output

  virtual void output_type(output_context & ct, Node * par, Assoc context, Type_qualifiers q);
};

/** @brief Array type node
 *  @ingroup AST
 *
 *  This class represents array types, where the sub-type is the type of
 *  the array elements. Note that we automatically break multi-dimensional
 *  arrays into nested array types. For example, "int A[3][5]" is
 *  represented as: arrayNode [3] --> arrayNode [5] --> primNode (int)
 *
 *  We recognize the equivalence between an array of elements and a pointer
 *  to the element type. In particular, for the purposes of parameter
 *  passing, an array is a pointer to the element type. Formal array
 *  parameters have no size expression because they accept any array of the
 *  given element type.
 *
 *  We also recognize that an array of char is a string. All string
 *  constants will have this type. 
 *
 *  The @ref NodeType is Array.
 *
 *  @see ptrNode
 *  @see constNode */

class arrayNode : public typeNode
{

private:

  /** @brief Array size expression
   *
   *  This expression holds the size of the array. Since an array size can
   *  be any constant expression, we store the expression as-is, and use
   *  the exprNode::eval() functions to determine the actual numeric size
   *  when necessary.
   *
   *  @see exprNode::eval() */

  TREE exprNode * _dim;

  /** @brief Array size
   *
   *  Currently unused, this could hold the actual size of the array
   *  computed by evaluating the size expression.
   *
   *  @see _dim
   *  @see exprNode::eval() */

  int _size;

public:

  /** @brief Create a new array type
   *
   *  The new type has the given type qualifiers, element type, and size
   *  expression.
   *
   *  @param tq the type qualifiers
   *  @param the_type the type of the array element
   *  @param the_dim the expression indicating the size of the array
   *  @param coord the location of the construct in the source file. */

  arrayNode(Type_qualifiers tq, typeNode * the_type, exprNode * the_dim,
	   const Coord coord = Coord::Unknown);

  /** @brief Destroy a arrayNode.
   * 
   *  @warning Do not use the destructors to delete AST nodes. Instead,
   *  rely on the node garbage collector. */

  virtual ~arrayNode();

  /** @name Accessors
   *
   *  Methods to get and set fields in the class. */
  //@{

  inline exprNode * dim() const { return _dim; }
  inline exprNode * get_dim() { exprNode * out = _dim; _dim = 0; return out; }
  inline void dim(exprNode * the_dim) { _dim = the_dim; }

  inline int size() const { return _size; }
  inline void size(int the_size) { _size = the_size; }

  //@}

  /** @name Type predicates
   *
   *  These methods support a variety of useful type predicates, most of
   *  which are self-explanatory. Each typeNode subclass overrides the
   *  methods for which it returns true. See typeNode for details. */
  //@{

  virtual bool is_aggregate() const { return true; }
  virtual bool is_derived() const { return true; }
  virtual bool is_pointer() const { return true; }

  // an array will be seen as a pointer (thus a scalar)
  // if it has no dimension, e.g., is a parameter

  virtual bool is_scalar() const { return _dim == NULL; }
  
  bool is_string() const;

  //@}

  // -- Type equal

  bool qualified_equal_to(typeNode * node2,
			  bool strict_toplevel, bool strict_recursive);

  /** @name AST Traversal
   *
   *  Methods to uniformly traverse the AST. See the documentation in the Node class. */
  //@{

  virtual void visit(Visitor * the_visitor);
  virtual void walk(Walker & the_walker);
  virtual Node * change(Changer & the_changer, bool redispatch = false);

  //@}

  // -- Clone

  virtual Node * clone() const { return new arrayNode ( *this ); }

  // -- Output

  virtual void output_type(output_context & ct, Node * par, Assoc context, Type_qualifiers q);
};

/** @brief Function type node
 *  @ingroup AST
 *
 *  This class represents function types. It is used for both procedure
 *  definitions, and for declaring variables of type
 *  pointer-to-function. The sub-type is used to hold the return type of
 *  the function. 
 *
 *  The @ref NodeType is Func.  */

class funcNode : public typeNode
{

private:

  /** @brief Formal parameters
   *
   *  This list contains the declarations of the formal parameters to the
   *  function. There are several special kinds of arguments:
   *  <ul>
   *    <li> An empty list of arguments. This indicates a function
   *    declaration that does not specify the input parameters.
   *    <li> A list with one argument and that argument is a declaration
   *    with no name and primitive type @c void. This indicates a function
   *    that takes no arguments.
   *    <li> Arguments with types, but no names. These are abstract declarators.
   *    <li> The last argument has primitive type ellipsis and no
   *    name. This indicates a variable argument function, such as @c
   *    printf.
   *  </ul>
   *
   *  The class provides methods to conveniently check for these cases. */

  TREE decl_list _args;

  /** @brief K-and-R style
   *
   *  True if the function type was parsed from an old K-and-R style
   *  declaration (argument types separate from the argument list). */
  bool _is_knr;

public:

  /** @brief Create a new function type
   *
   *  The new function type has the given type qualifiers, argument list
   *  and return type.
   *
   *  @param tq the type qualifiers
   *  @param args the arguments, which are removed and the list deleted
   *  @param returns the return type
   *  @param coord the location of the construct in the source file. */

  funcNode(Type_qualifiers tq, decl_list * args, typeNode * returns,
	   const Coord coord = Coord::Unknown);

  /** @brief Destroy a funcNode.
   * 
   *  @warning Do not use the destructors to delete AST nodes. Instead,
   *  rely on the node garbage collector. */

  virtual ~funcNode();

  /** @name Accessors
   *
   *  Methods to get and set fields in the class. */
  //@{

  inline decl_list & args() { return _args; }
  inline const decl_list & args() const { return _args; }

  inline typeNode * returns() const { return type(); }
  inline void returns(typeNode * returns) { type(returns); }

  inline bool is_knr() const { return _is_knr; }
  inline void is_knr (bool v) { _is_knr = v; }

  //@}

  // -- Type equal

  bool qualified_equal_to(typeNode * node2,
			  bool strict_toplevel, bool strict_recursive);

  /** @name Type predicates
   *
   *  These methods support a variety of useful type predicates, most of
   *  which are self-explanatory. Each typeNode subclass overrides the
   *  methods for which it returns true. See typeNode for details. */
  //@{

  virtual bool is_derived() const { return true; }
  virtual bool is_pointer() const { return true; }

  //@}

  /** @brief Is void argument list
   *
   *  @returns true if the argument list is void */

  bool is_void_args();

  /** @brief Check conversions
   *
   *  This method may be obsolete. It just calls
   *  typeNode::usual_unary_conversion_type() on each argument, returning
   *  false if any of the resulting types are incompatible with the
   *  original types. */

  bool check_conversions();

  /** @brief Compare two functions for compatibility
   *
   *  From ANSI 6.5.4.3, this method compares two function types for
   *  compatibility. This is the basis for the type comparison of this
   *  type. See the ANSI standard for the rules.
   *
   *  @param nfunc the other function type to compare against
   *  @returns true if they are compatible */

  bool is_compatible_with(funcNode * nfunc);

  /** @brief Add parameter types
   *  @internal
   *
   *  This method processes old-style K-and-R function declarations, where
   *  the argument types are separate from the argument list itself. This
   *  routine takes the declarations of the parameters and transfers the
   *  types to the actual parameter list. This can be tricky because any
   *  missing types default to int, and they need not be in the same order.
   *
   *  @param types the declarations of the parameters */

  void add_parameter_types(decl_list * types);

  /** @name AST Traversal
   *
   *  Methods to uniformly traverse the AST. See the documentation in the Node class. */
  //@{

  virtual void visit(Visitor * the_visitor);
  virtual void walk(Walker & the_walker);
  virtual Node * change(Changer & the_changer, bool redispatch = false);

  //@}

  // -- Clone

  virtual Node * clone() const { return new funcNode ( *this ); }

  // -- Output

  virtual void output_type(output_context & ct, Node * par, Assoc context, Type_qualifiers q);
};

/** @brief Struct/Union/Enum type node
 *  @ingroup AST
 *
 *  This class provides a common base class for the aggregate types struct
 *  and union, and enum. A significant design decision for these types was
 *  to completely remove the definition of the contents from this part of
 *  the AST. To see the problem, consider the following code:
 *
 *  @code
 *     struct A * head;
 *     struct A {
 *        struct A * next;
 *        int data;
 *     }
 *  @endcode
 *
 *  There are three uses of the "struct A" type: the first is a forward
 *  reference and does not include the contents of the struct (it doesn't
 *  need to); the second defines the contents; the third is a
 *  self-reference within the struct definition.
 *
 *  This is represented as follows. All struct/union/enum contents are
 *  defined in a class called suespecNode. These objects are removed from
 *  the main AST and placed in the list at the unitNode. All uses of the
 *  type refer to those objects. In order to remember which declarations
 *  originally contained the contents, we mark them as "elaborated". 
 *
 *  The @ref NodeType is determined by the subclass (either Struct, Union,
 *  or Enum).
 *
 *  @see suespecNode
 *  @see unitNode::_suespecs */

class sueNode : public typeNode
{

private:

  /** @brief The contents of the aggregate
   *
   *  This field refers to the type declaration of the contents of the
   *  struct/union/enum type. */

  REF suespecNode * _spec;

  /** @brief Is elaborated
   *
   *  True if the original declaration contained the contents of the
   *  struct/union/enum */
  bool _elaborated;

public:

  /** @brief Create a new struct/union/enum type
   *
   *  The new type is of the given kind (struct, union or enum) and is not
   *  elaborated, by default.
   *
   *  @param typ the kind of sue (struct, union, enum)
   *  @param coord the location of the construct in the source file. */

  sueNode(NodeType typ, const Coord coord);

  /** @brief Destroy a sueNode.
   * 
   *  @warning Do not use the destructors to delete AST nodes. Instead,
   *  rely on the node garbage collector. */

  virtual ~sueNode();

  /** @name Accessors
   *
   *  Methods to get and set fields in the class. */
  //@{

  inline bool elaborated() const { return _elaborated; }
  inline void elaborated(bool elab) { _elaborated = elab; }

  inline suespecNode * spec() const { return _spec; }
  inline void spec(suespecNode * s) { _spec = s; }

  //@}

  /** @name Parser methods
   */
  //@{

  // --- Was SetSUdclNameFields from sue.c

  void      set_name_fields(idNode *id, decl_list * fields, 
			    const Coord left_coord,
			    const Coord the_right_coord);

  sueNode * set_name_fields_and(idNode *id, decl_list * fields, 
				const Coord left_coord,
				const Coord the_right_coord);

  // -- Was SetSUdclName in sue.c

  void      set_name(idNode * id, const Coord coord);
  sueNode * set_name_and(idNode * id, const Coord coord);

  // -- ForceNewSU in sue.c

  void      force_new(const Coord coord);
  sueNode * force_new_and(const Coord coord);

  // -- Tags

  bool same_tag_as(sueNode * other);
  void tag_conflict(sueNode * new_sue); // from sue.c

  //@}

  // -- Type equal

  bool qualified_equal_to(typeNode * node2,
			  bool strict_toplevel, bool strict_recursive);

  // -- Data type predicates

  // -- Output

  virtual void output_type(output_context & ct, Node * par, Assoc context, Type_qualifiers q);
};

/** @brief Struct type node
 *  @ingroup AST
 *
 *  This class represents struct types. Note that it does not represent the
 *  definition of the structs, but just an instant of the type. For
 *  example, in the declaration "struct A * x;" the AST built would look
 *  like this: declNode "x" --> ptrNode --> structNode. The actual
 *  definition resides in an suespecNode.
 *
 *  The @ref NodeType is Struct.
 *
 *  @see sueNode
 *  @see suespecNode */

class structNode : public sueNode
{

private:

public:

  /** @brief Create new struct type
   *
   *  The new struct type has no tag and doesn't refer to any
   *  definition. Use the sueNode::spec() methods to get and set the
   *  reference to the definition, which also contains the name.
   *
   *  @param coord the location of the construct in the source file. */

  structNode(const Coord coord = Coord::Unknown);

  /** @brief Destroy a structNode.
   * 
   *  @warning Do not use the destructors to delete AST nodes. Instead,
   *  rely on the node garbage collector. */

  virtual ~structNode();

  /** @name Type predicates
   *
   *  These methods support a variety of useful type predicates, most of
   *  which are self-explanatory. Each typeNode subclass overrides the
   *  methods for which it returns true. See typeNode for details. */
  //@{

  virtual bool is_aggregate() const { return true; }

  //@}

  /** @name AST Traversal
   *
   *  Methods to uniformly traverse the AST. See the documentation in the Node class. */
  //@{

  virtual void visit(Visitor * the_visitor);
  virtual void walk(Walker & the_walker);
  virtual Node * change(Changer & the_changer, bool redispatch = false);

  //@}

  // -- Clone

  virtual Node * clone() const { return new structNode ( *this ); }
};

/** @brief Union type node
 *  @ingroup AST
 *
 *  This class represents union types. Note that it does not represent the
 *  definition of the unions, but just an instant of the type. For
 *  example, in the declaration "union A * x;" the AST built would look
 *  like this: declNode "x" --> ptrNode --> unionNode. The actual
 *  definition resides in an suespecNode.
 *
 *  The @ref NodeType is Union.
 *
 *  @see sueNode
 *  @see suespecNode */

class unionNode : public sueNode
{

private:

public:

  /** @brief Create new union type
   *
   *  The new union type has no tag and doesn't refer to any
   *  definition. Use the sueNode::spec() methods to get and set the
   *  reference to the definition, which also contains the name.
   *
   *  @param coord the location of the construct in the source file. */

  unionNode(const Coord coord = Coord::Unknown);

  /** @brief Destroy a unionNode.
   * 
   *  @warning Do not use the destructors to delete AST nodes. Instead,
   *  rely on the node garbage collector. */

  virtual ~unionNode();

  /** @name AST Traversal
   *
   *  Methods to uniformly traverse the AST. See the documentation in the Node class. */
  //@{

  virtual void visit(Visitor * the_visitor);
  virtual void walk(Walker & the_walker);
  virtual Node * change(Changer & the_changer, bool redispatch = false);

  //@}

  // -- Clone

  virtual Node * clone() const { return new unionNode ( *this ); }
};

/** @brief Enum type node
 *  @ingroup AST
 *
 *  This class represents enum types. Note that it does not represent the
 *  definition of the enums, but just an instant of the type. Any time the
 *  source refers to one of the enum constants, or has a variable or
 *  parameter that accepts the enum, this is the type associated. The
 *  actual definition resides in an suespecNode.
 *
 *  The @ref NodeType is Enum.
 *
 *  @see sueNode
 *  @see suespecNode */

class enumNode : public sueNode
{

private:

public:

  /** @brief Create new enum type
   *
   *  The new enum type has no tag and doesn't refer to any
   *  definition. Use the sueNode::spec() methods to get and set the
   *  reference to the definition, which also contains the name.
   *
   *  @param coord the location of the construct in the source file. */

  enumNode(const Coord coord = Coord::Unknown);

  /** @brief Create a new enum during parsing
   *  @internal
   *
   *  This routine creates a new enum type given the name (from the input
   *  idNode) and the member list. ( Was BuildEnum in sue.c)
   *
   *  @param id the idNode providing the name (tag) for the enum
   *  @param values the list of members
   *  @param enum_coord the location in the source of the enum keyword
   *  @param left_coord the location in the source of the left curly brace
   *  @param right_coord the location in the source of the right curly brace */

  enumNode(idNode *id, decl_list * values, 
	   const Coord enum_coord, const Coord left_coord,
	   const Coord right_coord);

  /** @brief Destroy a enumNode.
   * 
   *  @warning Do not use the destructors to delete AST nodes. Instead,
   *  rely on the node garbage collector. */

  virtual ~enumNode();

  /** @name Type predicates
   *
   *  These methods support a variety of useful type predicates, most of
   *  which are self-explanatory. Each typeNode subclass overrides the
   *  methods for which it returns true. See typeNode for details. */
  //@{

  virtual bool is_int() const { return true; }
  virtual bool is_integer() const { return true; }
  virtual bool is_arithmetic() const { return true; }
  virtual bool is_scalar() const { return true; }

  //@}

  /** @name AST Traversal
   *
   *  Methods to uniformly traverse the AST. See the documentation in the Node class. */
  //@{

  virtual void visit(Visitor * the_visitor);
  virtual void walk(Walker & the_walker);
  virtual Node * change(Changer & the_changer, bool redispatch = false);

  //@}

  // -- Clone

  virtual Node * clone() const { return new enumNode ( *this ); }
};

/** @brief Struct/Union/Enum definition
 *  @ingroup AST
 *
 *  This class holds the tag and fields for struct/union/enum. Many
 *  different sueNodes may point to one suespecNode, but only the one that
 *  has is_elaborated equal to true actually "owns" the suespecNode. All
 *  suespecNode objects are stored in the unitNode. For a more detailed
 *  explanation see the related documentation.
 *
 *  The @ref NodeType is sueSpec. However, there is a special field called
 *  @c _owner that will match that of the refering sueNode (either Struct,
 *  Union, or Enum).
 *
 *  @see sueNode
 *  @see unitNode */

class suespecNode : public typeNode
{

private:

  /** @brief the struct/union/enum tag
   *
   *  This field should really be called "tag" according to the ANSI C
   *  lingo. Even though tags are optional in s/u/e definitions (anonymoung
   *  s/u/e), this field is never empty. The sueNode routine that sets them
   *  up will generate unique names automatically */

  string _name;

  /** @brief the struct/union/enum fields
   *
   *  This declaration list holds the declarations of the members of the
   *  struct/union/enum. For structs and unions the Decl_location will be
   *  SU, and for enums the Decl_location will be ENUM. */

  TREE decl_list _fields;

  /** @brief struct/union/enum complete
   *
   *  This boolean is used primarily during parsing to indicate when a
   *  struct or union has it's fields defined. 
   *
   *  @see sue_complete_walker */

  bool _complete;

  /** @brief visited flag
   *
   *  This flag seems to be obsolete (it is always false). However, it may
   *  be causing some problems in the output algorithm.
   *
   *  @todo Check if this flag is causing bugs in the C code generator. */

  bool _visited;

  /** @brief Output scope control
   *
   *  This value is used during output to control generation of struct and
   *  union contents. It represents the depth of scope nesting at which the
   *  definition occurs. This helps the algorithm generate struct and union
   *  contents in the right places, even if tags are reused in different
   *  scopes.
   *
   *  @todo Check if this value is causing bugs in the C code generator. */

  int _scope_output;

  /** @brief Size in bytes
   *
   *  This size information for an s/u/e is currently not computed. */

  int _size;

  /** @brief Alignment in memory
   *
   *  The alignment information for an s/u/e is currently not computed. */

  int _align;

  /** @brief Owner type
   *
   *  This field indicates which kind of s/u/e owns this definition. It is
   *  always one of Struct, Union, or Enum. */

  NodeType _owner;

public:

  /** @brief Create a new SUE definition
   *
   *  The new s/u/e has the given name, set of fields and owner type. In
   *  addition, since all suespecNode objects are collected in the
   *  unitNode, the unit is passed as an argument.
   *
   *  @param name the tag for the s/u/e (copied; not deleted).  @param
   *  fields the list of field declarations, which are removed and the list
   *  deleted.  @param owner the @ref NodeType of the s/u/e (either Struct,
   *  Union, or Enum) @param the_unit the translation unit @param coord the
   *  location of the construct in the source file. */

  suespecNode(const char * name, decl_list * fields, NodeType owner,
	      unitNode * the_unit,
	      const Coord coord);

  /** @brief Destroy a suespecNode.
   * 
   *  @warning Do not use the destructors to delete AST nodes. Instead,
   *  rely on the node garbage collector. */

  virtual ~suespecNode();

  /** @name Accessors
   *
   *  Methods to get and set fields in the class. */
  //@{

  inline bool complete() const { return _complete; }
  inline void complete(bool comp) { _complete = comp; }

  inline bool visited() const { return _visited; }
  inline void visited(bool v) { _visited = v; }

  inline bool scope_output() const { return _scope_output; }
  inline void scope_output(int v) { _scope_output = v; }

  inline int size() const { return _size; }
  inline void size(int size) { _size = size; }

  inline int align() const { return _align; }
  inline void align(int align) { _align = align; }

  inline string & name() { return _name; }
  inline void name(string name) { _name = name; }

  inline decl_list & fields() { return _fields; }
  inline const decl_list & fields() const { return _fields; }

  inline NodeType owner() const { return _owner; }

  //@}

  // -- Tags and fields

  /** @brief Same tag test
   *
   *  This methods tests to see if this suespecNode has the same tag as the
   *  input one. This can be done by simply comparing the names. However,
   *  this code appears to be written to handle empty names (anonymous
   *  s/u/e), which never happens.
   *
   *  @param other the suespecNode to compare tags with
   *  @return true if they have the same tag */

  bool same_tag_as(suespecNode * other);

  /** @brief Update the s/u/e with fields
   *
   *  This method is used to add fields to an s/u/e during parsing.
   *
   *  @param fields the list of field declarations
   *  @param sue the owner sueNode
   *  @param right the location in the source of the right curly brace */

  void update(decl_list * fields, sueNode * sue, const Coord right);

  /** @brief Find a field by name
   *
   *  Search the _fields list for the given name, returning the
   *  declaration, if found, or null if not found. (Was SUE_FindField in
   *  sue.c).
   *
   *  @param name the field name to look for
   *  @return the declaration, if found, or null */

  declNode * find_field(const string & name);

  /** @name AST Traversal
   *
   *  Methods to uniformly traverse the AST. See the documentation in the Node class. */
  //@{

  virtual void visit(Visitor * the_visitor);
  virtual void walk(Walker & the_walker);
  virtual Node * change(Changer & the_changer, bool redispatch = false);

  //@}

  // -- Clone

  virtual Node * clone() const { return new suespecNode ( *this ); }

  // -- Output

  virtual void output_type(output_context & ct, Node * par, Assoc context, Type_qualifiers q);
};


/** @brief Expression node
 *  @ingroup AST
 *
 *  This class rovides a base class for all expression nodes. It contains
 *  two fields that all expressions have: a type and a value. Initially,
 *  both fields are empty, with two exceptions. Constant expression have
 *  their values set, and the cast expressions have their type set. We can
 *  compute these values for any expression subtree using the appropriate
 *  facility. To compute the type of intermediate expressions, use the
 *  semcheck_walker. To compute the values of constant expressions, call
 *  the eval() method.
 *
 *  The @ref NodeType depends on the specific exprNode subclass.
 *
 *  @see semcheck_walker
 *  */

class exprNode : public Node
{
public:

  /** @brief Add integral promotions
   *
   *  This method takes an expression and calls
   *  typeNode::integral_promotions() on its type to determine if any
   *  apply. If they do, it inserts an implicit castNode above the input
   *  expression that represents this implicit conversion.
   *
   *  @see castNode
   *
   *  @param old_expr the expression to perform integral promotion on
   *  @return the resulting expression, with a cast added if necessary */

  static exprNode * integral_promotions(exprNode * old_expr);

  /** @brief Usual arithmetic conversions
   *
   *  This method takes two expressions and adds any casts that are
   *  necessary to make them compatible for arithmetic operations. It calls
   *  typeNode::usual_arithmetic_conversions(), passing the types of the
   *  expressions, to determine when the casts are needed. It inserts
   *  implicit castNode objects above the expressions for the casts.
   *
   *  @see castNode
   *
   *  @param left the left-hand expression
   *  @param right the right-hand expression
   *  @return the left and right expressions, possibly with casts added */

  static pair<exprNode *, exprNode *>
    usual_arithmetic_conversions(exprNode * left,
				 exprNode * right);

private:

  /** @brief the type of the expression
   *
   *  This field is only populated by calling the semcheck_walker. */

  TREE typeNode * _type;

  /** @brief the value of the expression
   *
   *  This field is only populated by calling the eval() method. For
   *  non-constant expressions, the result may be a special "no val" value.
   *
   *  @see constant::no_val() */

  constant _value;

public:

  /** @brief Create a new expression node
   *
   *  This constructor is only called directly by the exprNode
   *  subclasses. The @ref NodeType is determined by the subclass.
   *
   *  @param typ the kind of expression node
   *  @param type the type of the expression (only for casts) or null
   *  @param coord the location of the construct in the source file. */

  exprNode(NodeType typ, typeNode * type, const Coord coord);

  /** @brief Destroy a exprNode.
   * 
   *  @warning Do not use the destructors to delete AST nodes. Instead,
   *  rely on the node garbage collector. */

  virtual ~exprNode();

  /** @name Accessors
   *
   *  Methods to get and set fields in the class. */
  //@{

  inline typeNode * type() const { return _type; }
  inline typeNode * get_type() { typeNode * out = _type; _type = 0; return out; }
  inline void type(typeNode *type) { _type = type; }

  inline const constant & value() const { return _value; }
  inline constant & value() { return _value; }
  inline void value(const constant & newval) { _value = newval; }

  //@}

  // -- Data type base

  virtual typeNode * base_type(bool TdefIndir) const;

  // -- Symbol lookup
  // LookupPostfixExpression in type.c

  // virtual void lookup() {  } // -- Default; do nothing

  // -- Type conversions
  // THESE ARE NOT USED
  // exprNode * assignment_conversions(typeNode * to_type);
  // exprNode * call_conversions(typeNode * to_type);
  // exprNode * return_conversions(typeNode * to_type);
  // exprNode * cast_conversions(typeNode * to_type);
  // exprNode * usual_unary_conversions(typeNode * to_type, bool f_to_d);
  // exprNode * usual_binary_conversions(typeNode * to_type);

  /** @brief Constant expression evaluator
   *
   *  This method attempts to evaluate an expression at compile-time. This
   *  only yields a meaningful value when the leaves of the given
   *  expression are constants, enums, or other compile-time values (e.g.,
   *  sizeof). The resulting value is stored on each exprNode, in the
   *  _value field. Each exprNode sublcass implements this method, calling
   *  it recursively when necessary. */

  virtual void eval() =0;

  /** @brief Is l-value
   *
   *  Indicates if the expression is an l-value (that is, the left side of
   *  an assignment).
   *  @todo This doesn't work yet.
   *
   *  @return true if the given expression is an l-value */

  virtual bool is_lvalue() { return true; } // THIS IS A TBD

  typeNode * no_tdef_type() { return type()->follow_tdefs(); }

  // -- Output

  virtual void output(output_context & ct, Node * par);

  /** @brief Output a expression
   *  @internal
   *
   *  This method generates the C code form of a expression. It should only
   *  be called by the output() method. Each exprNode subclass overrides
   *  this method to produce the appropriate output. The prec and assoc
   *  arguments capture operator precedence and associativity,
   *  respectively, and help the code determine when parentheses are
   *  needed. See the Operator and Operators classes for more information.
   *
   *  @see Operator
   *  @see Operators
   *  @see exprNode::precedence()
   *  @see exprNode::parens()
   *
   *  @param ct the output context
   *  @param par the parent containing node
   *  @param prec a value that captures operator precedence
   *  @param assoc a value that captures operator associativity */

  virtual void output_expr(output_context & ct, Node * par, int prec, Assoc assoc) =0;

  /** @brief Associativity and precedence
   *
   *  Determine the associativity and precedence of the expression. Each
   *  exprNode subclass overrides this method to provide the specific
   *  results. The default is highest precedence and left-associative.
   *
   *  @see exprNode::output_expr()
   *  @see exprNode::parens()
   *
   *  @param assoc a reference parameter that is set to the associativity value
   *  @return the precedence value */

  virtual int precedence(Assoc & assoc);

  /** @brief Determine if parenthesis are needed
   *
   *  This method takes the associativity and precedence values of the
   *  enclosing expression and determines if parentheses are needed.
   *
   *  @see exprNode::output_expr()
   *  @see exprNode::precedence()
   *
   *  @param outer_prec the precedence value of the enclosing expression
   *  @param outer_assoc the associativity value of the enclosing expression
   *  @return true if parentheses are needed. */

  bool parens(int outer_prec, Assoc outer_assoc);
};

/** @brief Constant expression
 *  @ingroup AST
 *
 *  This node holds a single constant value. The actual value is held in
 *  the exprNode::_value field. For constants derived from parsing, we also
 *  save the original textual representation of the constant.
 *
 *  The @ref NodeType is Const.  */

class constNode : public exprNode
{

private:

  /** @brief textual representation of the constant
   *
   *  We store the original textual representation or constants that come
   *  directly from the input source code. This allows us to regenerate the
   *  input code more accurately. It is not required, and can be
   *  automatically generated by the semantic type checker.
   *
   *  @see semcheck_walker */

  string _text;

public:

  /** @brief Create a new constant value
   *
   *  The new constant should have a value, but need not have a textual
   *  representation. The input string is copied, and not deleted.
   *
   *  @param value the value of the constant
   *  @param text a textual representation of the constant (optional)
   *  @param coord the location of the construct in the source file. */

  constNode(constant value, const char * text = "",
	    const Coord coord = Coord::Unknown);

  /** @brief Destroy a constNode.
   * 
   *  @warning Do not use the destructors to delete AST nodes. Instead,
   *  rely on the node garbage collector. */

  virtual ~constNode();

  /** @name Accessors
   *
   *  Methods to get and set fields in the class. */
  //@{

  inline string & text() { return _text; }

  //@}

  /** @brief Usual unary conversions
   *
   *  @warning This method is probably obsolete */

  typeNode * usual_unary_conversion_type() 
      { return type()->usual_unary_conversion_type(); }

  // -- Constant folding

  virtual void eval();

  /** @name AST Traversal
   *
   *  Methods to uniformly traverse the AST. See the documentation in the Node class. */
  //@{

  virtual void visit(Visitor * the_visitor);
  virtual void walk(Walker & the_walker);
  virtual Node * change(Changer & the_changer, bool redispatch = false);

  //@}

  // -- Dataflow

  virtual void dataflow(FlowVal * v, FlowProblem & fp);

  // -- Clone

  virtual Node * clone() const { return new constNode ( *this ); }

  // -- Output

  virtual void output_expr(output_context & ct, Node * par, int prec, Assoc assoc);
};

/** @brief Identifier
 *  @ingroup AST
 *
 *  This class represents identifiers in the code. Each identifier has a
 *  name, and a reference to a declaration. Initially, the reference is
 *  empty, but can be populated using id_lookup_walker.
 *
 *  The @ref NodeType is Id.
 *
 *  @see id_lookup_walker */

class idNode : public exprNode
{

private:

  /** @brief the name
   *
   *  The name of the identifier */

  string _name;

  /** @brief the declaration
   *
   *  The declaration to which this identifier refers. Use the
   *  id_lookup_walker to compute the declarations of all identifiers.
   * 
   *  @see id_lookup_walker */

  REF declNode * _decl;

public:

  /** @brief Create a new identfier
   *
   *  The new identifier has the given name, which is copies, and not
   *  deleted.
   *
   *  @param text the name of the identifier (copied; not deleted)
   *  @param coord the location of the construct in the source file. */

  idNode(const char * text, const Coord coord = Coord::Unknown);

  /** @brief Create an identifier with a declaration
   *
   *  This constructor allows you to specify the associated declaration
   *  explicitly. The name of the identifier is copied from the
   *  declaration.
   *
   *  @param the_decl the declaration to refer to
   *  @param coord the location of the construct in the source file. */

  idNode(declNode * the_decl, const Coord coord = Coord::Unknown);

  /** @brief Destroy a idNode.
   * 
   *  @warning Do not use the destructors to delete AST nodes. Instead,
   *  rely on the node garbage collector. */

  virtual ~idNode();

  /** @name Accessors
   *
   *  Methods to get and set fields in the class. */
  //@{

  inline string & name() { return _name; }
  inline const string & name() const { return _name; }
  inline void name(string nm) { _name = nm; }

  inline declNode * decl() const { return _decl; }
  void decl(declNode *decl);

  //@}

  // -- Data type base

  typeNode * base_type(bool TdefIndir) const;

  // -- Type (overloaded to call into decl if possible)

  // void type(typeNode * t);
  // typeNode * type() const;

  // -- Symbol lookup

  // virtual void lookup();
  // idNode * lookup_and();

  // -- Constant folding

  virtual void eval();

  /** @name AST Traversal
   *
   *  Methods to uniformly traverse the AST. See the documentation in the Node class. */
  //@{

  virtual void visit(Visitor * the_visitor);
  virtual void walk(Walker & the_walker);
  virtual Node * change(Changer & the_changer, bool redispatch = false);

  //@}

  // -- Dataflow

  virtual void dataflow(FlowVal * v, FlowProblem & fp);

  // -- Clone

  virtual Node * clone() const { return new idNode ( *this ); }

  // -- Output

  virtual void output_expr(output_context & ct, Node * par, int prec, Assoc assoc);
};

/** @brief Binary operator
 *  @ingroup AST
 *
 *  This class represents most expressions with two arguments (binary
 *  operator expressions). The only exception is the castNode, because it
 *  takes a type as an argument.
 *
 *  The indexNode class has been folded into the binaryNode class, so now
 *  array index expressions are represented as binary operations. The
 *  operator is Operator::Index. The left expression is the array and the
 *  right expression is the index. Note that multi-dimensional array access
 *  generates nested index operators, one for each dimension. The operator
 *  is left associative, so the tree for "A[2][7]" looks like this:
 *  indexNode [7] --> indexNode [2] --> idNode "A".
 *
 *  The @ref NodeType is Binary.  */

class binaryNode : public exprNode
{

private:

  /** @brief the operator
   *
   *  The operator object actually resides the Operators table.
   *
   *  @see Operator
   *  @see Operators */

  Operator * _op;

  /** @brief the left expression subtree */

  TREE exprNode * _left;

  /** @brief the right expression subtree */

  TREE exprNode * _right;

public:

  /** @brief Create a new binary expression
   *
   *  The new expression has the given operator, and left and right
   *  subexpressions. The operator is given using its identifier from the
   *  parser. For most operators, this is simply the char
   *  representation. For example, we pass '+' to get the addition
   *  operator. For ambiguous operators and multiple-character operators,
   *  you need to look up the proper identifier in the Operators table.
   *
   *  @see Operator
   *  @see Operators
   *  @see Operators::table
   *
   *  @param op_id the id of the operator (usually, just the char representation)
   *  @param left the left expression subtree
   *  @param right the right expression subtree
   *  @param coord the location of the construct in the source file. */

  binaryNode(unsigned int op_id, exprNode * left, exprNode * right,
	     const Coord coord = Coord::Unknown);

  /** @brief Destroy a binaryNode.
   * 
   *  @warning Do not use the destructors to delete AST nodes. Instead,
   *  rely on the node garbage collector. */

  virtual ~binaryNode();

  /** @name Accessors
   *
   *  Methods to get and set fields in the class. */
  //@{

  inline Operator * op() const { return _op; }
  inline void op(Operator * op) { _op = op; }

  inline exprNode * left() const { return _left; }
  inline exprNode * get_left() { exprNode * out = _left; _left = 0; return out; }
  inline void left(exprNode * left) { _left = left; }

  inline exprNode * right() const { return _right; }
  inline exprNode * get_right() { exprNode * out = _right; _right = 0; return out; }
  inline void right(exprNode * right) { _right = right; }

  //@}

  // -- Symbol lookup

  // virtual void lookup();

  // -- Constant folding

  virtual void eval();

  /** @name AST Traversal
   *
   *  Methods to uniformly traverse the AST. See the documentation in the Node class. */
  //@{

  virtual void visit(Visitor * the_visitor);
  virtual void walk(Walker & the_walker);
  virtual Node * change(Changer & the_changer, bool redispatch = false);

  //@}

  // -- Dataflow

  virtual void dataflow(FlowVal * v, FlowProblem & fp);

  // -- Clone

  virtual Node * clone() const { return new binaryNode ( *this ); }

  // -- Output

  virtual void output_expr(output_context & ct, Node * par, int prec, Assoc assoc);
  virtual int precedence(Assoc & assoc);
};

/** @brief Unary operator
 *  @ingroup AST
 *
 *  This class represents most expression with one argument (unary operator
 *  expressions). 
 *
 *  The @ref NodeType is Unary.  */

class unaryNode : public exprNode
{

private:

  /** @brief the operator
   *
   *  The operator object actually resides the Operators table.
   *
   *  @see Operator
   *  @see Operators */

  Operator * _op;

  /** @brief the sub-expression */

  TREE exprNode * _expr;

  /** @brief the sizeof type
   *
   *  For sizeof expressions given with a type, this field holds that type. */

  TREE typeNode * _sizeof_type;

public:

  /** @brief Create a new unary expression
   *
   *  The new expression has the given operator and subexpression. The
   *  operator is given using its identifier from the parser. For most
   *  operators, this is simply the char representation. For example, we
   *  pass '+' to get the addition operator. For ambiguous operators and
   *  multiple-character operators, you need to look up the proper
   *  identifier in the Operators table.
   *
   *  @see Operator
   *  @see Operators
   *  @see Operators::table
   *
   *  @param op_id the id of the operator (usually, just the char representation)
   *  @param expr the sub-expression 
   *  @param coord the location of the construct in the source file. */

  unaryNode(unsigned int op_id, exprNode * expr,
	    const Coord coord = Coord::Unknown);

  /** @brief Create a sizeof expression
   *
   *  This constructor differs in that it takes a type as the subexpression.
   *
   *  @param op_id the operator (should always be sizeof -- why is this optional?)
   *  @param the_type the type given to the sizeof operator
   *  @param coord the location of the construct in the source file. */

  unaryNode(unsigned int op_id, typeNode * the_type,
	    const Coord coord = Coord::Unknown);

  /** @brief Destroy a unaryNode.
   * 
   *  @warning Do not use the destructors to delete AST nodes. Instead,
   *  rely on the node garbage collector. */

  virtual ~unaryNode();

  /** @name Accessors
   *
   *  Methods to get and set fields in the class. */
  //@{

  inline Operator * op() const { return _op; }
  inline void op(Operator * op) { _op = op; }

  inline exprNode * expr() const { return _expr; }
  inline exprNode * get_expr() { exprNode * out = _expr; _expr = 0; return out; }
  inline void expr(exprNode * expr) { _expr = expr; }

  //    sizeof_type is only used if the operation is "sizeof()"

  inline typeNode * sizeof_type() const { return _sizeof_type; }
  inline typeNode * get_sizeof_type() 
  { typeNode * out = _sizeof_type; _sizeof_type = 0; return out; }
  inline void sizeof_type(typeNode *sizeof_type)
  { _sizeof_type = sizeof_type; }

  //@}

  // -- Symbol lookup

  // virtual void lookup();

  // -- Constant folding

  virtual void eval();

  /** @name AST Traversal
   *
   *  Methods to uniformly traverse the AST. See the documentation in the Node class. */
  //@{

  virtual void visit(Visitor * the_visitor);
  virtual void walk(Walker & the_walker);
  virtual Node * change(Changer & the_changer, bool redispatch = false);

  //@}

  // -- Dataflow

  virtual void dataflow(FlowVal * v, FlowProblem & fp);

  // -- Clone

  virtual Node * clone() const { return new unaryNode ( *this ); }

  // -- Output

  virtual void output_expr(output_context & ct, Node * par, int prec, Assoc assoc);
  virtual int precedence(Assoc & assoc);
};

/** @brief Cast expression
 *  @ingroup AST
 *
 *  This class represents casts that are explicit in the source code. This
 *  is really a binary operator, where one argument is a type and the other
 *  is an expression. The semantic checker also inserts casts for integral
 *  promotions and arithmetic conversions. These casts are called implicit
 *  casts, and do not appear in the output. 
 *
 *  The @ref NodeType is Cast.
 *
 *  @see exprNode::integral_promotions()
 *  @see exprNode::usual_arithmetic_conversions()
 *  @see implicitcastNode */

class castNode : public exprNode
{

private:

  /** @brief the expression to which the cast is applied */

  TREE exprNode * _expr;

  /** @brief true if the cast is implicit
   *
   *  Implicit casts are generated by the semantic checker to make explicit
   *  any conversions between operands caused by the integral promotions
   *  and usual arithmetic conversions. */

  bool _implicit;

public:

  /** @brief Create a new cast expression
   *
   *  The new cast expression has the given type and subexpression.
   *
   *  @param type the type to which the expression is being cast
   *  @param expr the subexpression receiving the cast
   *  @param implicit pass true to create an implicit cast
   *  @param coord the location of the construct in the source file. */

  castNode(typeNode * type, exprNode * expr, bool implicit = false,
	   const Coord coord = Coord::Unknown);

  /** @brief Destroy a castNode.
   * 
   *  @warning Do not use the destructors to delete AST nodes. Instead,
   *  rely on the node garbage collector. */

  virtual ~castNode();

  /** @name Accessors
   *
   *  Methods to get and set fields in the class. */
  //@{

  inline exprNode * expr() const { return _expr; }
  inline exprNode * get_expr() { exprNode * out = _expr; _expr = 0; return out; }
  inline void expr(exprNode * expr) { _expr = expr; }

  inline bool is_implicit() const { return _implicit; }
  //@}

  // -- Constant folding

  virtual void eval();

  /** @name AST Traversal
   *
   *  Methods to uniformly traverse the AST. See the documentation in the Node class. */
  //@{

  virtual void visit(Visitor * the_visitor);
  virtual void walk(Walker & the_walker);
  virtual Node * change(Changer & the_changer, bool redispatch = false);

  //@}

  // -- Dataflow

  virtual void dataflow(FlowVal * v, FlowProblem & fp);

  // -- Clone

  virtual Node * clone() const { return new castNode ( *this ); }

  // -- Output

  virtual void output_expr(output_context & ct, Node * par, int prec, Assoc assoc);
  virtual int precedence(Assoc & assoc);
};


/** @brief Comma-separated expressions
 *  @ingroup AST
 *
 *  This class represents a comma-separated list of expressions. Rather
 *  than represent the comma operator as a binary expression, we gather
 *  together a series of commas and put them in a single node. 
 *
 *  The @ref NodeType is Comma.  */

class commaNode : public exprNode
{

private:

  /** @brief The comma-separated expressions
   *
   *  The expression in this list are in the order they appeared in the
   *  source. */

  TREE expr_list _exprs;

public:

  /** @brief Create a new comma expression
   *
   *  The new comma node has the given list of expressions. The elements
   *  are removed from the list and the list is destroyed.
   *
   *  @param exprs the list of expressions
   *  @param coord the location of the construct in the source file. */

  commaNode(expr_list * exprs, const Coord coord = Coord::Unknown);

  /** @brief Create an empty comma expression
   *
   *  The new comma node has no expressions initially. They can be added by
   *  modifying the _exprs list.
   *  @param coord the location of the construct in the source file. */

  commaNode(const Coord coord = Coord::Unknown);

  /** @brief Destroy a commaNode.
   * 
   *  @warning Do not use the destructors to delete AST nodes. Instead,
   *  rely on the node garbage collector. */

  virtual ~commaNode();

  /** @name Accessors
   *
   *  Methods to get and set fields in the class. */
  //@{

  inline expr_list & exprs() { return _exprs; }
  inline const expr_list & exprs() const { return _exprs; }

  //@}

  // -- Data type base

  typeNode * base_type(bool TdefIndir) const;

  // -- Constant folding

  virtual void eval();

  /** @name AST Traversal
   *
   *  Methods to uniformly traverse the AST. See the documentation in the Node class. */
  //@{

  virtual void visit(Visitor * the_visitor);
  virtual void walk(Walker & the_walker);
  virtual Node * change(Changer & the_changer, bool redispatch = false);

  //@}

  // -- Dataflow

  virtual void dataflow(FlowVal * v, FlowProblem & fp);

  // -- Clone

  virtual Node * clone() const { return new commaNode ( *this ); }

  // -- Output

  virtual void output_expr(output_context & ct, Node * par, int prec, Assoc assoc);
  virtual int precedence(Assoc & assoc);
};

/** @brief Ternary expression
 *  @ingroup AST
 *
 *  This class represents the ternary operator ( expr ? expr : expr ). 
 *
 *  The @ref NodeType is Ternary.  */

class ternaryNode : public exprNode
{

private:

  /** @brief the ternary condition expression */

  TREE exprNode * _cond;

  /** @brief the true branch expression */

  TREE exprNode * _true_br;

  /** @brief the false branch expression */

  TREE exprNode * _false_br;

  /** @brief the location of the colon in the source code */

  Coord _colon_coord;

public:

  /** @brief Create a new ternary expression
   *
   *  The new ternary expression has the given condition, true and false
   *  expressions.
   *
   *  @param cond the condition expression
   *  @param true_br the true branch expression
   *  @param false_br the false branch expression
   *  @param qmark_coord the location of the question mark in the source code
   *  @param colon_coord the location of the colon in the source code */

  ternaryNode(exprNode * cond, exprNode * true_br, exprNode * false_br,
	      const Coord qmark_coord = Coord::Unknown,
	      const Coord colon_coord = Coord::Unknown);

  /** @brief Destroy a ternaryNode.
   * 
   *  @warning Do not use the destructors to delete AST nodes. Instead,
   *  rely on the node garbage collector. */

  virtual ~ternaryNode();

  /** @name Accessors
   *
   *  Methods to get and set fields in the class. */
  //@{

  inline exprNode * cond() const { return _cond; }
  inline exprNode * get_cond() { exprNode * out = _cond; _cond = 0; return out; }
  inline void cond(exprNode * cond) { _cond = cond; }

  inline exprNode * true_br() const { return _true_br; }
  inline exprNode * get_true_br() { exprNode * out = _true_br; _true_br = 0; return out; }
  inline void true_br(exprNode * true_br) { _true_br = true_br; }

  inline exprNode * false_br() const { return _false_br; }
  inline exprNode * get_false_br() { exprNode * out = _false_br; _false_br = 0; return out; }
  inline void false_br(exprNode * false_br) { _false_br = false_br; }

  //@}

  // -- Constant folding

  virtual void eval();

  /** @name AST Traversal
   *
   *  Methods to uniformly traverse the AST. See the documentation in the Node class. */
  //@{

  virtual void visit(Visitor * the_visitor);
  virtual void walk(Walker & the_walker);
  virtual Node * change(Changer & the_changer, bool redispatch = false);

  //@}

  // -- Dataflow

  virtual void dataflow(FlowVal * v, FlowProblem & fp);

  // -- Clone

  virtual Node * clone() const { return new ternaryNode ( *this ); }

  // -- Output

  virtual void output_expr(output_context & ct, Node * par, int prec, Assoc assoc);
  virtual int precedence(Assoc & assoc);
};

/** @brief Function call expression
 *  @ingroup AST
 *
 *  This class represents a function call expression. This includes the
 *  actual argument expressions passed into the function, as well as the
 *  expression that determines the function to call. Note that this class
 *  handles all forms of function call, including calling through a
 *  function pointer. That's why the target function is specified as an
 *  expression. In most cases, it is simply an identifier whose declaration
 *  refers directly to a function definition or declaration (funcNode or
 *  the declaration on a procNode). 
 *
 *  The callNode object also has a placeholder for a pointer to the target
 *  procNode. Initially, this pointer is null. However, we can perform
 *  call-graph analysis to determine the target of a callNode. The
 *  callgraph_walker does a reasonably job of determining the target, but
 *  cannot handle calls through a function pointer.
 *
 *  Currently, no facility exists to make sure that the actual arguments
 *  match the number or types of the formal parameters.
 *
 *  The @ref NodeType is Call.
 *
 *  @see callgraph_walker
 *  @see procNode
 *  @see funcNode */

class callNode : public exprNode
{

private:

  /** @brief the expression indicating the function to call */

  TREE exprNode * _name;

  /** @brief the arguments
   *
   *  The arguments are maintained in the order they appear in the input
   *  source. */

  TREE expr_list _args;

  /** @brief the target procedure
   *
   *  Initially, this pointer is null, but can be populated by calling one
   *  of the call graph facilities. The callgraph_walker works fairly well,
   *  but can only handle simple calls where the target function is given
   *  explicitly.
   *
   *  @see callgraph_walker */

  REF procNode * _proc;

  /** @brief stack argument bytes
   *
   *  The number of bytes of stack space that will be required for this
   *  function call.  This is setup at LIR generation time. */

  unsigned int _stack_arg_bytes;

public:

  /** @brief Create a new function call expression
   *
   *  The new function call has the given target function expression (name)
   *  and the given arguments. The arguments are extracted from the list
   *  and the list is deleted.
   *
   *  @param name the expression specifying the target procedure to call
   *  @param args the list of actual argument expressions
   *  @param coord the location of the construct in the source file. */

  callNode(exprNode * name, expr_list * args,
	   const Coord coord = Coord::Unknown);

  /** @brief Destroy a callNode.
   * 
   *  @warning Do not use the destructors to delete AST nodes. Instead,
   *  rely on the node garbage collector. */

  virtual ~callNode();

  /** @name Accessors
   *
   *  Methods to get and set fields in the class. */
  //@{

  inline exprNode * name() const { return _name; }
  inline exprNode * get_name() { exprNode * out = _name; _name = 0; return out; }
  inline void name(exprNode * name) { _name = name; }

  inline expr_list & args() { return _args; }
  inline const expr_list & args() const { return _args; }

  inline procNode * proc() const { return _proc; }
  inline void proc(procNode * proc) { _proc = proc; }

  inline unsigned int stack_arg_bytes() const { return _stack_arg_bytes; }
  inline void stack_arg_bytes(unsigned int bytes) { _stack_arg_bytes = bytes; }

  //@}

  // -- Data type base

  typeNode * base_type(bool TdefIndir) const;

  // -- Symbol lookup

  // virtual void lookup();

  // -- Constant folding

  virtual void eval();

  /** @name AST Traversal
   *
   *  Methods to uniformly traverse the AST. See the documentation in the Node class. */
  //@{

  virtual void visit(Visitor * the_visitor);
  virtual void walk(Walker & the_walker);
  virtual Node * change(Changer & the_changer, bool redispatch = false);

  //@}

  // -- Dataflow

  virtual void dataflow(FlowVal * v, FlowProblem & fp);

  // -- Clone

  virtual Node * clone() const { return new callNode ( *this ); }

  // -- Output

  virtual void output_expr(output_context & ct, Node * par, int prec, Assoc assoc);
};

/** @brief Initializer expression
 *  @ingroup AST
 *
 *  This class represents the initializer expressions used to set the
 *  initial values of structs and arrays. They are generally nested lists
 *  of constant expressions, like this:
 *
 *  @code
 *    int A[2][3] = { { 17, 23, 5 } , { 4, 19, 102 } };
 *  @endcode
 *
 *  The code above consists of three initializerNode objects: one for the
 *  top-level array, and one for each subarray. The two subarray
 *  initializers contain lists of constNode objects (one for each integer). */

class initializerNode : public exprNode
{

private:

  TREE expr_list _exprs;

public:

  // -- Constructors

  initializerNode(expr_list * exprs, const Coord coord = Coord::Unknown);

  /** @brief Destroy a initializerNode.
   * 
   *  @warning Do not use the destructors to delete AST nodes. Instead,
   *  rely on the node garbage collector. */

  virtual ~initializerNode();

  /** @name Accessors
   *
   *  Methods to get and set fields in the class. */
  //@{

  inline expr_list & exprs() { return _exprs; }
  inline const expr_list & exprs() const { return _exprs; }
  inline void add_expr(exprNode * the_expr) { _exprs.push_back(the_expr); }

  //@}

  // -- Constant folding

  virtual void eval();

  /** @name AST Traversal
   *
   *  Methods to uniformly traverse the AST. See the documentation in the Node class. */
  //@{

  virtual void visit(Visitor * the_visitor);
  virtual void walk(Walker & the_walker);
  virtual Node * change(Changer & the_changer, bool redispatch = false);

  //@}

  // -- Dataflow

  virtual void dataflow(FlowVal * v, FlowProblem & fp);

  // -- Clone

  virtual Node * clone() const { return new initializerNode ( *this ); }

  // -- Output

  virtual void output_expr(output_context & ct, Node * par, int prec, Assoc assoc);
};

/** @brief Statement
 *  @ingroup AST
 *
 *  This class provides a base class for all statement nodes.
 *
 *  The @ref NodeType is determined by the subclass. */

class stmtNode : public Node
{

private:

  /** @brief Dataflow analysis exit value
   *  @internal
   *
   *  This value is used to hold the exit flow value during dataflow analysis. */

  FlowVal * _at_exit;

  /** @brief Comment
   *
   *  Each statement may have a comment string associated with it. If the
   *  string is non-empty, then the C code generator will output the
   *  comment right before the statement. The generator provides the
   *  comment delimiters automatically, so there is no need to put them in
   *  the string. */

  string _comment;

  /** @brief Pragmas
   *
   * This list will contain any pragmas that precede the given
   * statement. During parsing, they are collected into the CBZ::pragmas
   * list and then placed here in the stmtNode constructor. */

  text_list _pragmas;

public:

  /** @brief Create a new statement
   *
   *  The new statement has the given @ref NodeType. This constructor is
   *  only called by the statement subclasses.
   *
   *  @param typ the kind of statement
   *  @param coord the location of the construct in the source file. */

  stmtNode(NodeType typ, const Coord coord);

  /** @brief Destroy a stmtNode.
   * 
   *  @warning Do not use the destructors to delete AST nodes. Instead,
   *  rely on the node garbage collector. */

  virtual ~stmtNode();

  /** @name Accessors
   *
   *  Methods to get and set fields in the class. */
  //@{

  inline string & comment() { return _comment; }

  // -- Dataflow

  inline FlowVal * at_exit() const { return _at_exit; }
  inline void at_exit(FlowVal * ae) { _at_exit = ae; }

  // -- Pragmas

  inline text_list & pragmas() { return _pragmas; }

  //@}

  // -- Output

  virtual void output(output_context & ct, Node * par);

  /** @brief Output a statement
   *  @internal
   *
   *  This method generates the C code form of a statement. It should only
   *  be called by the output() method. Each stmtNode subclass overrides
   *  this method to produce the appropriate output. 
   *
   *  @param ct the output context
   *  @param par the parent containing node */

  virtual void output_stmt(output_context & ct, Node * par) =0;

  void output_comment(output_context & ct);
};

/** @brief Compund statement or block
 *  @ingroup AST
 *
 *  This class represents a curly-brace delimited compound statement or
 *  statement block. It generally introduces a new scope, allowing new
 *  declarations as well as code. It is used as the top-level container for
 *  procedure bodies, branch bodies, loop bodies, etc. The general form is:
 *
 *  @code
 *  {
 *    <declarations>
 *
 *    <statements>
 *  }
 *  @endcode
 *
 *  The @ref NodeType is Block. */

class blockNode : public stmtNode
{

public:

  /** @brief Convert to block
   *
   *  This method is used primarily by the parser to make sure that all
   *  branch bodies and loop bodies are represented by blockNodes, even if
   *  they aren't in the source code. For example:
   *
   *  @code
   *  if (cond) x = x + 1;
   *  @endcode
   *
   *  Even though the body of the branch is just a single statement, we
   *  build a block containing that one statement:
   *
   *  @code
   *  if (cond) { x = x + 1; }
   *  @endcode
   *
   *  This simplifies many other kinds of processing. For example, it makes
   *  it easy to add statements to the branch body.
   *
   *  The method looks at the type of the input statement. If it is already
   *  a compound statement, it is return unchanged. Otherwise, it creates a
   *  compound statement, puts the statement in it, and then returns the
   *  compound statement.
   *
   *  @param stmt the statement to covert, if necessary
   *  @param coord the location of the construct in the source file.
   *  @return the statement as a blockNode */

  static blockNode * toBlock(stmtNode * stmt, Coord coord);

private:

  /** @brief the local declarations
   *
   *  This list contains any declarations made in the scope of the block. */

  TREE decl_list _decls;

  /** @brief the statements
   *
   *  This list contains the statements contained in the block. */

  TREE stmt_list _stmts;

  /** @brief the location of the right curly brace */

  Coord _right_brace;
  // TREE typeNode * _type; Why does block have a type?

public:

  /** @brief Create a new block
   *
   *  The new block has the given list of declarations and statements. In
   *  both bases, the actual declarations and statements are removed from
   *  the lists, and the lists are deleted. Null may be passed for either
   *  argument.
   *
   *  @param decls the list of declarations
   *  @param stmts the list of statements
   *  @param coord the location of the left curly brace in the source file.
   *  @param coord the location of the right curly brace in the source file. */

  blockNode(decl_list * decls, stmt_list * stmts,
	    const Coord left_coord = Coord::Unknown,
	    const Coord right_brace = Coord::Unknown);

  /** @brief Destroy a blockNode.
   * 
   *  @warning Do not use the destructors to delete AST nodes. Instead,
   *  rely on the node garbage collector. */

  virtual ~blockNode();

  /** @name Accessors
   *
   *  Methods to get and set fields in the class. */
  //@{

  inline decl_list & decls() { return _decls; }
  inline const decl_list & decls() const { return _decls; }

  inline stmt_list & stmts() { return _stmts; }
  inline const stmt_list & stmts() const { return _stmts; }

  inline Coord right_brace() const { return _right_brace; }
  inline void right_brace(const Coord right_brace) { _right_brace = right_brace; }

  //@}

  // -- Data type base

  typeNode * base_type(bool TdefIndir) const;

  /** @name AST Traversal
   *
   *  Methods to uniformly traverse the AST. See the documentation in the Node class. */
  //@{

  virtual void visit(Visitor * the_visitor);
  virtual void walk(Walker & the_walker);
  virtual Node * change(Changer & the_changer, bool redispatch = false);

  //@}

  // -- Dataflow

  virtual void dataflow(FlowVal * v, FlowProblem & fp);

  // -- Clone

  virtual Node * clone() const { return new blockNode ( *this ); }

  // -- Output 
  virtual void output_stmt(output_context & ct, Node * par);

};

class algorithm_info
{
};

/** @brief Basic block
 *  @ingroup AST
 *
 *  This subclass of blockNode represents code in basic block form to
 *  support the construction of a control-flow graph and algorithms based
 *  on that graph. Initial parsing does not produce this form. To convert
 *  the AST into a control-flow graph, first dismantle the code (using the
 *  Dismantle class) then invoke the control-flow graph algorithm (using
 *  the cfg_changer). This form also supports several other analysis
 *  algorithms, such as dominators (using the Dominators class), dominance
 *  frontiers (using the DominanceFrontiers class) and pointer analysis
 *  (using the Pointers class).
 *
 *  Each basicblockNode has added fields for specifying control-flow
 *  predecessors and successors, as well as fields for representing a
 *  dominator tree. 
 *
 *  The @ref NodeType is Block (from the superclass)
 *
 *  @see Dismantle
 *  @see cfg_changer
 *  @see Dominators
 *  @see DominanceFrontiers
 *  @see Pointers */

class basicblockNode : public blockNode {

private:

  /** @brief the control-flow predecessors */

  basicblock_list _preds;

  /** @brief the control-flow successors */

  basicblock_list _succs;

  /** @brief the parent in the dominator tree
   *
   *  This is the immediate dominator of this basic block. */

  basicblockNode * _parent;

  /** @brief the children in the dominator tree
   *
   *  This is the set of basic blocks that are immediately dominated by
   *  this one. */

  basicblock_list _children;

  /** @brief algorithm info
   *
   *  This field is used by various algorithms to temporarily attach some
   *  information to a basic block. */

  algorithm_info * _info;

  /** @brief depth-first numbering
   *
   *  Each basic block can be assignmed a number, usually in depth-first
   *  order. This is useful for building a bit-vector representation of
   *  sets of basic blocks. */

  int _dfn;

  /** @brief dataflow analysis entry value
   *
   *  This field is used by the dataflow analysis framework to accumulate
   *  the flow-value coming into the basic block from it's
   *  predecessors. Sometimes, it's referred to as IN[B]. */

  FlowVal * _at_entry;

  /** @brief dataflow analysis exit value
   *
   *  This field is used by the dataflow analysis framework to hold the
   *  flow value exiting the basic block. It is typically computed by
   *  passing the _at_entry through the transfer functions of the
   *  statements in the block. Sometimes, it's referred to as OUT[B]. */

  FlowVal * _at_exit;

public:

  /** @brief Create a new basic block
   *
   *  The basic block has the given declarations and statements (see the
   *  constructor for blockNode). By default, all the lists are empty, and
   *  the other references are null.
   *
   *  @param decls the list of declarations
   *  @param stmts the list of statements
   *  @param coord the location of the left curly brace in the source file.
   *  @param coord the location of the right curly brace in the source file. */

  basicblockNode(decl_list * decls, stmt_list * stmts,
	    const Coord left_coord = Coord::Unknown,
	    const Coord right_brace = Coord::Unknown);

  /** @brief Destroy a basicblockNode.
   * 
   *  @warning Do not use the destructors to delete AST nodes. Instead,
   *  rely on the node garbage collector. */

  virtual ~basicblockNode();

  /** @name Accessors
   *
   *  Methods to get and set fields in the class. */
  //@{

  inline basicblock_list & preds() { return _preds; }
  inline const basicblock_list & preds() const { return _preds; }

  inline basicblock_list & succs() { return _succs; }
  inline const basicblock_list & succs() const { return _succs; }

  inline basicblockNode * parent() const { return _parent; }
  inline void parent(basicblockNode *par) { _parent = par; }

  inline basicblock_list & children() { return _children; }
  inline const basicblock_list & children() const { return _children; }

  inline algorithm_info * info() const { return _info; }
  inline void info(algorithm_info * i) { _info = i; }

  inline int dfn() const { return _dfn; }
  inline void dfn(int d) { _dfn = d; }

  // -- Flow values (for IP analyzer)

  inline FlowVal * at_entry() const { return _at_entry; }
  inline void at_entry(FlowVal * ae) { _at_entry = ae; }

  inline FlowVal * at_exit() const { return _at_exit; }
  inline void at_exit(FlowVal * ae) { _at_exit = ae; }

  //@}

  /** @name AST Traversal
   *
   *  Methods to uniformly traverse the AST. See the documentation in the Node class. */
  //@{

  virtual void visit(Visitor * the_visitor);
  virtual void walk(Walker & the_walker);
  virtual Node * change(Changer & the_changer, bool redispatch = false);

  //@}

  // Dataflow is inherited from blockNode

  // -- Clone

  virtual Node * clone() const { return new basicblockNode ( *this ); }

};
 
  

/** @brief Expression statement
 *  @ingroup AST
 *
 *  This class represent simple statements that just consist of an
 *  expression. This is just a wrapper that makes explicit that fact that
 *  any expression is also a statement.
 *
 *  The @ref NodeType is Expr */

class exprstmtNode : public stmtNode
{

private:

  /** @brief the expression */

  TREE exprNode * _expr;

public:

  /** @brief Create an expression statement
   *
   *  The new statement has the given expression
   *
   *  @param expr the expression */

  exprstmtNode(exprNode * expr);

  /** @brief Destroy a exprstmtNode.
   * 
   *  @warning Do not use the destructors to delete AST nodes. Instead,
   *  rely on the node garbage collector. */

  virtual ~exprstmtNode();

  /** @name Accessors
   *
   *  Methods to get and set fields in the class. */
  //@{

  inline exprNode * expr() const { return _expr; }
  inline exprNode * get_expr() { exprNode * out = _expr; _expr = 0; return out; }
  inline void expr(exprNode * expr) { _expr = expr; }

  //@}

  // -- Data type base

  typeNode * base_type(bool TdefIndir) const;


  /** @name AST Traversal
   *
   *  Methods to uniformly traverse the AST. See the documentation in the Node class. */
  //@{

  virtual void visit(Visitor * the_visitor);
  virtual void walk(Walker & the_walker);
  virtual Node * change(Changer & the_changer, bool redispatch = false);

  //@}

  // -- Dataflow

  virtual void dataflow(FlowVal * v, FlowProblem & fp);

  // -- Clone

  virtual Node * clone() const { return new exprstmtNode ( *this ); }

  // -- Output

  virtual void output_stmt(output_context & ct, Node * par);
};

/** @brief Jump target
 *  @ingroup AST
 *
 *  This class provides a base class for all statements that serve as
 *  targets for jumps. This includes the regular goto labels, as well as
 *  the switch cases and default. Note that in C, all labels refer to a
 *  statement. Syntactically, the labelled statement in the child of the
 *  label. For example, "case 5: x = 10; " results in a labelNode that has
 *  an exprstmtNode as it's child.
 *
 *  The @ref NodeType is determined by the subclass (either Label, Case, or
 *  Default).  */

class targetNode : public stmtNode
{

private:

  /** @brief the target statement */

  TREE blockNode * _stmt;

  /** @brief dataflow analysis entry value
   *
   *  This field is used by the dataflow analysis framework to accumulate
   *  the flow-value coming in from the various jump statements. */

  FlowVal * _at_entry;

public:

  /** @brief Create a new target node
   *
   *  The new node has the given type and target statement.
   *
   *  @param typ the kind of target node (either Label, Case, or Default)
   *  @param stmt the target statement */

  targetNode(NodeType typ, stmtNode * stmt, const Coord coord);

  /** @brief Destroy a targetNode.
   * 
   *  @warning Do not use the destructors to delete AST nodes. Instead,
   *  rely on the node garbage collector. */

  virtual ~targetNode();

  /** @name Accessors
   *
   *  Methods to get and set fields in the class. */
  //@{

  inline blockNode * stmt() const { return _stmt; }
  inline blockNode * get_stmt() { blockNode * out = _stmt; _stmt = 0; return out; }
  inline void stmt(blockNode * stmt) { _stmt = stmt; }

  // -- Dataflow

  inline FlowVal * at_entry() const { return _at_entry; }
  inline void at_entry(FlowVal * ae) { _at_entry = ae; }

  //@}
};

/** @brief Label statement
 *  @ingroup AST
 *
 *  This class represents label statements. The label has a name, and a
 *  child statement inherited from targetNode. In addition, it has a list
 *  that can hold references to all the goto statements that refer to
 *  it. This information is populated during initial parsing by the
 *  goto_label_walker.
 *
 *  The @ref NodeType is Label.
 *
 *  @see gotoNode
 *  @see goto_label_walker */

class labelNode : public targetNode
{

private:

  /** @brief the name of the label */

  string _name;

  /** @brief the list of referring gotos
   *
   *  This list is initially empty, but is populated by the
   *  goto_label_walker after parsing. */

  REF goto_list _references;

public:

  /** @brief Create a new label statement
   *
   *  The new label has the given name and child statement. The input
   *  string is copied and not deleted.
   *
   *  @param name the name of the label (copied; not deleted)
   *  @param stmt the child statement
   *  @param coord the location of the construct in the source file. */

  labelNode(const char * name, stmtNode * stmt,
	    const Coord coord = Coord::Unknown);

  /** @name Parser methods
   */
  //@{

  /** @brief Create a label from an identfier
   *  @internal
   *
   *  Used in BuildLabel and ResolveGoto in procedure.c
   *
   *  @param ident the identifier, which is deleted
   *  @param stmt the child statement */

  labelNode(idNode * ident, stmtNode * stmt);

  /** @brief Create an empty label from an identifier
   *  @internal
   *
   *    Build an undeclared label in ResolveGoto
   *
   *  @param ident the identifier, which is deleted */

  labelNode(idNode * ident);

  //@}

  /** @brief Destroy a labelNode.
   * 
   *  @warning Do not use the destructors to delete AST nodes. Instead,
   *  rely on the node garbage collector. */

  virtual ~labelNode();

  /** @name Accessors
   *
   *  Methods to get and set fields in the class. */
  //@{

  inline string & name() { return _name; }
  inline void name(string name) { _name = name; }

  inline goto_list & references() { return _references; }
  inline const goto_list & references() const { return _references; }

  //@}

  // -- Undeclared

  bool is_undeclared() const;

  /** @name AST Traversal
   *
   *  Methods to uniformly traverse the AST. See the documentation in the Node class. */
  //@{

  virtual void visit(Visitor * the_visitor);
  virtual void walk(Walker & the_walker);
  virtual Node * change(Changer & the_changer, bool redispatch = false);

  //@}

  // -- Dataflow

  virtual void dataflow(FlowVal * v, FlowProblem & fp);

  // -- Clone

  virtual Node * clone() const { return new labelNode ( *this ); }

  // -- Output

  virtual void output_stmt(output_context & ct, Node * par);
};

/** @brief Case statement
 *  @ingroup AST
 *
 *  This class represents a case within a switch statement. Like the
 *  labelNode, it has a child statement. However, the label is an
 *  expression. The language requires the expression to be a compile-time
 *  constant expression, but there are currently no checks for this. In
 *  addition, this node keeps a reference to the enclosing switchNode.
 *
 *  The defaultNode has been folded into this class. If the case has no
 *  expression, then it is the default.
 *
 *  The @ref NodeType is Case */

class caseNode : public targetNode
{

private:

  /** @brief the label expression
   *
   *  This should be a compile-time constant expression */

  TREE exprNode * _expr;

  /** @brief the containing switch statement
   *
   *  Unless explicitly specified, this field is initially null. After
   *  parsing, these references are computed by calling the
   *  set_container_walker. */

  REF switchNode * _container;

public:

  /** @brief Create a new case statement
   *
   *  The new statement has the given label expression, child statement and
   *  containing switch statement.
   *
   *  @param expr the label expression, or null for the "default" case
   *  @param stmt the child statement
   *  @param container the containing switch statement
   *  @param coord the location of the construct in the source file. */

  caseNode(exprNode * expr, stmtNode * stmt, switchNode * container,
	   const Coord coord = Coord::Unknown);

  /** @brief Create a new case statement without a switch
   *
   *  The new statement has the given label expression and child statement,
   *  but no explicit switchNode container. This is computed later using
   *  the set_container_walker.
   *
   *  @param expr the label expression
   *  @param stmt the child statement
   *  @param coord the location of the construct in the source file. */

  caseNode(exprNode * expr, stmtNode * stmt,
	   const Coord coord = Coord::Unknown);

  /** @brief Destroy a caseNode.
   * 
   *  @warning Do not use the destructors to delete AST nodes. Instead,
   *  rely on the node garbage collector. */

  virtual ~caseNode();

  /** @name Accessors
   *
   *  Methods to get and set fields in the class. */
  //@{

  inline exprNode * expr() const { return _expr; }
  inline exprNode * get_expr() { exprNode * out = _expr; _expr = 0; return out; }
  inline void expr(exprNode * expr) { _expr = expr; }

  inline switchNode * container() const { return _container; }
  inline void container(switchNode * container) { _container = container; }

  //@}

  /** @name AST Traversal
   *
   *  Methods to uniformly traverse the AST. See the documentation in the Node class. */
  //@{

  virtual void visit(Visitor * the_visitor);
  virtual void walk(Walker & the_walker);
  virtual Node * change(Changer & the_changer, bool redispatch = false);

  //@}

  // -- Dataflow

  virtual void dataflow(FlowVal * v, FlowProblem & fp);

  // -- Clone

  virtual Node * clone() const { return new caseNode ( *this ); }

  // -- Output

  virtual void output_stmt(output_context & ct, Node * par);
};

/** @brief Selection statement
 *  @ingroup AST
 *
 *  This class provides a base class for all the control-flow selection
 *  statements. This includes the "if" statement, with and without an
 *  "else" and the "switch" statement. All three statements have a
 *  selection expression, and at least one child statement. For the if-else
 *  statements, we add another child statement to represent the false
 *  branch.
 *
 *  The @ref NodeType is determined by the subclass (If or Switch).  */

class selectionNode : public stmtNode
{

private:

  /** @brief the selection expression */

  TREE exprNode * _expr;

  /** @brief the child statement */

  TREE blockNode * _stmt;

public:

  /** @brief Create a new selection statement
   *
   *  The new selection statement has the given type, selection expression
   *  and child statement. This constructor is only called by the
   *  selectionNode subclass.
   *
   *  @param typ the kind of selection statement
   *  @param expr the selection expression
   *  @param stmt the child statement 
   *  @param coord the location of the construct in the source file. */

  selectionNode(NodeType typ, exprNode * expr, stmtNode * stmt,
		const Coord coord);

  /** @brief Destroy a selectionNode.
   * 
   *  @warning Do not use the destructors to delete AST nodes. Instead,
   *  rely on the node garbage collector. */

  virtual ~selectionNode();

  /** @name Accessors
   *
   *  Methods to get and set fields in the class. */
  //@{

  inline exprNode * expr() const { return _expr; }
  inline exprNode * get_expr() { exprNode * out = _expr; _expr = 0; return out; }
  inline void expr(exprNode * expr) { _expr = expr; }

  inline blockNode * stmt() const { return _stmt; }
  inline blockNode * get_stmt() { blockNode * out = _stmt; _stmt = 0; return out; }
  inline void stmt(blockNode * stmt) { _stmt = stmt; }

  //@}
};

/** @brief If-else statement
 *  @ingroup AST
 *
 *  This class represents an "if" statement. The selection expression
 *  serves as the condition and the child statement inherited from
 *  selectionNode is the true branch. It adds a child statement for the
 *  false branch. The false branch may be null, indicating that the
 *  statement has no "else" branch.
 *
 *  The @ref NodeType is If.  */

class ifNode : public selectionNode
{

private:

  /** @brief the false branch statement */

  TREE blockNode * _false_br;

  /** @brief the position of the "else" keyword in the source file */

  Coord _else_coord;

public:

  /** @brief Create a new if-else statement
   *
   *  The new if-else statement has the given condition expression, and
   *  true and false branches. Pass null for the false branch to create an
   *  if without an else.
   *
   *  @param expr the condition expression
   *  @param true_br the true branch statement
   *  @param false_br the false branch statement
   *  @param coord the location of the "if" keyword in the source file.
   *  @param coord the location of the "else" keyword in the source file. */

  ifNode(exprNode * expr, stmtNode * true_br, stmtNode * false_br,
	     const Coord if_coord = Coord::Unknown, 
	     const Coord else_coord = Coord::Unknown);

  /** @brief Destroy a ifNode.
   * 
   *  @warning Do not use the destructors to delete AST nodes. Instead,
   *  rely on the node garbage collector. */

  virtual ~ifNode();

  /** @name Accessors
   *
   *  Methods to get and set fields in the class. */
  //@{

  inline blockNode * true_br() const { return selectionNode::stmt(); }
  inline blockNode * get_true_br() { return selectionNode::get_stmt(); }
  inline void true_br(blockNode * true_br) { selectionNode::stmt(true_br); }

  inline blockNode * false_br() const { return _false_br; }
  inline blockNode * get_false_br() { blockNode * out = _false_br; _false_br = 0; return out; }
  inline void false_br(blockNode * false_br) { _false_br = false_br; }

  inline Coord else_coord() const { return _else_coord; }
  inline void else_coord(const Coord the_coord) { _else_coord = the_coord; }

  //@}

  /** @name AST Traversal
   *
   *  Methods to uniformly traverse the AST. See the documentation in the Node class. */
  //@{

  virtual void visit(Visitor * the_visitor);
  virtual void walk(Walker & the_walker);
  virtual Node * change(Changer & the_changer, bool redispatch = false);

  //@}

  // -- Dataflow

  virtual void dataflow(FlowVal * v, FlowProblem & fp);

  // -- Clone

  virtual Node * clone() const { return new ifNode ( *this ); }

  // -- Output

  virtual void output_stmt(output_context & ct, Node * par);
};

/** @brief Switch statement
 *  @ingroup AST
 *
 *  This class represents the switch statement. The child statement is
 *  typically a compound statement containing the cases. All cases should
 *  be contained somewhere in that subtree, although there is currently no
 *  check for this. It contains a list of the cases (including the default)
 *  that are within its scope. This list is initially empty, but is
 *  computed after parsing by the set_container_walker.
 *
 * The @ref NodeType is Switch. */

class switchNode : public selectionNode
{

private:

  /** @brief the list of cases
   *
   *  This list includes the default case. It is initially empty, but is
   *  computer after parsing by the set_container_walker. */

  REF target_list _cases;

  /** @brief a flag that is true if the switch has a default case */

  bool _has_default;

  /** @brief dataflow analysis entry value
   *
   *  This field is used by the dataflow analysis framework to hold the
   *  incoming flow value needed by each case statement. */

  FlowVal * _at_top;

public:

  /** @brief Create a new switch statement
   *
   *  The new switch statement has the given selection expression and the
   *  child statement that should contain the cases.
   *
   *  @param expr the selection expression
   *  @param stmt the child statement containing the cases
   *  @param coord the location of the construct in the source file. */

  switchNode(exprNode * expr, stmtNode * stmt,
	     const Coord coord = Coord::Unknown);

  /** @brief Destroy a switchNode.
   * 
   *  @warning Do not use the destructors to delete AST nodes. Instead,
   *  rely on the node garbage collector. */

  virtual ~switchNode();

  /** @name Accessors
   *
   *  Methods to get and set fields in the class. */
  //@{

  inline target_list & cases() { return _cases; }
  inline const target_list & cases() const { return _cases; }

  inline bool has_default() const { return _has_default; }
  inline void has_default(bool has_default) { _has_default = has_default; }

  inline FlowVal * at_top() const { return _at_top; }
  inline void at_top(FlowVal * at) { _at_top = at; }

  //@}

  /** @name AST Traversal
   *
   *  Methods to uniformly traverse the AST. See the documentation in the Node class. */
  //@{

  virtual void visit(Visitor * the_visitor);
  virtual void walk(Walker & the_walker);
  virtual Node * change(Changer & the_changer, bool redispatch = false);

  //@}

  // -- Dataflow

  virtual void dataflow(FlowVal * v, FlowProblem & fp);

  // -- Clone

  virtual Node * clone() const { return new switchNode ( *this ); }

  // -- Output

  virtual void output_stmt(output_context & ct, Node * par);
};

/** @brief Loop node
 *  @ingroup AST
 *
 *  This class provides a base class for all the kinds of loop statements.
 *
 *  The @ref NodeType is determined by the subclass (While, Do, or For)
 *  */

class loopNode : public stmtNode
{

private:  

  /** @brief the loop condition expression */

  TREE exprNode * _cond;

  /** @brief the loop body */

  TREE blockNode * _body;

  /** @brief loop head dataflow analysis value
   *
   *  This field is used by the dataflow analysis framework to hold the
   *  flow value at the head of the loop. */

  FlowVal * _at_loop_head;

  /** @brief loop tail dataflow analysis value
   *
   *  This field is used by the dataflow analysis framework to hold the
   *  flow value at the end (tail) of the loop. */

  FlowVal * _at_loop_tail;

public:

  /** @brief Create a new loop
   *
   *  The new loop has the given type, condition expression and body. This
   *  constructor is only called by the subclasses (whileNode, doNode, and
   *  forNode).
   *
   *  @param typ the type of loop (While, Do, or For)
   *  @param cond the loop condition
   *  @param body the loop body
   *  @param coord the location of the construct in the source file. */

  loopNode(NodeType typ, exprNode * cond, stmtNode * body, const Coord coord);

  /** @brief Create an empty loop
   *
   *  The new loop has no condition expression or body
   *
   *  @param typ the type of loop (While, Do, or For)
   *  @param coord the location of the construct in the source file. */

  loopNode(NodeType typ, const Coord coord);

  /** @brief Destroy a loopNode.
   * 
   *  @warning Do not use the destructors to delete AST nodes. Instead,
   *  rely on the node garbage collector. */

  virtual ~loopNode();

  /** @name Accessors
   *
   *  Methods to get and set fields in the class. */
  //@{

  inline exprNode * cond() const { return _cond; }
  inline exprNode * get_cond() { exprNode * out = _cond; _cond = 0; return out; }
  inline void cond(exprNode * cond) { _cond = cond; }

  inline blockNode * body() const { return _body; }
  inline blockNode * get_body() { blockNode * out = _body; _body = 0; return out; }
  inline void body(blockNode * body) { _body = body; }

  // -- Dataflow

  inline FlowVal * at_loop_head() const { return _at_loop_head; }
  inline void at_loop_head(FlowVal * ae) { _at_loop_head = ae; }

  inline FlowVal * at_loop_tail() const { return _at_loop_tail; }
  inline void at_loop_tail(FlowVal * ae) { _at_loop_tail = ae; }

  //@}
};

/** @brief While loop statement
 *  @ingroup AST
 *
 *  This class represents the while loop statement (condition at the top of
 *  the loop).
 *
 *  This @ref NodeType is While. */

class whileNode : public loopNode
{

private:

public:

  /** @brief Create a new while loop
   *
   *  The new while loop has the given condition expression and body.
   *
   *  @param cond the loop condition
   *  @param body the loop body
   *  @param coord the location of the construct in the source file. */

  whileNode(exprNode * cond, stmtNode * body,
	    const Coord coord = Coord::Unknown);

  /** @brief Destroy a whileNode.
   * 
   *  @warning Do not use the destructors to delete AST nodes. Instead,
   *  rely on the node garbage collector. */

  virtual ~whileNode();

  /** @name AST Traversal
   *
   *  Methods to uniformly traverse the AST. See the documentation in the Node class. */
  //@{

  virtual void visit(Visitor * the_visitor);
  virtual void walk(Walker & the_walker);
  virtual Node * change(Changer & the_changer, bool redispatch = false);

  //@}

  // -- Dataflow

  virtual void dataflow(FlowVal * v, FlowProblem & fp);

  // -- Clone

  virtual Node * clone() const { return new whileNode ( *this ); }

  // -- Output

  virtual void output_stmt(output_context & ct, Node * par);
};

/** @brief Do-while loop statement
 *  @ingroup AST
 *
 *  This class represents a do-while loop statement (condition at the
 *  bottom of the loop).
 *
 *  The @ref NodeType is Do.  */

class doNode : public loopNode
{

private:

  /** @brief the location of the "while" keyword in the source file */

  Coord _while_coord;

public:

  /** @brief Create a new do-while loop
   *
   *  The new do-while loop has the given condition expression and body.
   *
   *  @param cond the loop condition
   *  @param body the loop body
   *  @param coord the location of the "do" keyword in the source file.
   *  @param coord the location of the "while" keyword in the source file. */

  doNode(stmtNode * body, exprNode * cond,
	 const Coord coord = Coord::Unknown,
	 const Coord while_coord = Coord::Unknown);

  /** @brief Destroy a doNode.
   * 
   *  @warning Do not use the destructors to delete AST nodes. Instead,
   *  rely on the node garbage collector. */

  virtual ~doNode();

  /** @name Accessors
   *
   *  Methods to get and set fields in the class. */
  //@{

  inline Coord while_coord() const { return _while_coord; }
  inline void while_coord(const Coord while_coord) { _while_coord = while_coord; }

  //@}

  /** @name AST Traversal
   *
   *  Methods to uniformly traverse the AST. See the documentation in the Node class. */
  //@{

  virtual void visit(Visitor * the_visitor);
  virtual void walk(Walker & the_walker);
  virtual Node * change(Changer & the_changer, bool redispatch = false);

  //@}

  // -- Dataflow

  virtual void dataflow(FlowVal * v, FlowProblem & fp);

  // -- Clone

  virtual Node * clone() const { return new doNode ( *this ); }

  // -- Output

  virtual void output_stmt(output_context & ct, Node * par);
};

/** @brief For loop statement
 *  @ingroup AST
 *
 *  This class represents the for-loop statement (three expressions, plus
 *  the body). The condition expression still functions as the condition
 *  (the middle expression). It also adds two expressions, one for the
 *  initial condition (the first expression) and one for the "next"
 *  operation (the third expression). The decomposition looks like this:
 *
 *  @code
 *  for ( i = 0; i < N ; i++ ) {
 *    ...blah...
 *  }
 *  @endcode
 *
 *  The initial expression is "i = 0", the condition expression is "i < N"
 *  and the next expression is "i++". When any of these components are
 *  missing, the corresponding data members are null.
 *
 *  The @ref NodeType is For.  */

class forNode : public loopNode
{

private:

  /** @brief the initial expression (or null, if there is none) */

  TREE exprNode * _init;

  /** @brief the next expression (or null, if there is none) */

  TREE exprNode * _next;

  // FlowVal * _loop_val;
  // FlowVal * _break_val;
  // FlowVal * _continue_val;

public:

  /** @brief Create a new for loop
   *
   *  The new for loop has the given expressions and body. To leave out any
   *  of the expressions, pass null.
   *
   *  @param init the initial expression (or null)
   *  @param cond the condition expression (or null)
   *  @param next the next expression (or null)
   *  @param coord the location of the construct in the source file. */

  forNode(exprNode * init, exprNode * cond, exprNode * next,
	  stmtNode * body, const Coord coord = Coord::Unknown);

  /** @brief Destroy a forNode.
   * 
   *  @warning Do not use the destructors to delete AST nodes. Instead,
   *  rely on the node garbage collector. */

  virtual ~forNode();

  /** @name Accessors
   *
   *  Methods to get and set fields in the class. */
  //@{

  inline exprNode * init() const { return _init; }
  inline exprNode * get_init() { exprNode * out = _init; _init = 0; return out; }
  inline void init(exprNode * init) { _init = init; }

  inline exprNode * next() const { return _next; }
  inline exprNode * get_next() { exprNode * out = _next; _next = 0; return out; }
  inline void next(exprNode * next) { _next = next; }

  //@}

  /** @name AST Traversal
   *
   *  Methods to uniformly traverse the AST. See the documentation in the Node class. */
  //@{

  virtual void visit(Visitor * the_visitor);
  virtual void walk(Walker & the_walker);
  virtual Node * change(Changer & the_changer, bool redispatch = false);

  //@}

  // -- Dataflow

  virtual void dataflow(FlowVal * v, FlowProblem & fp);

  // -- Clone

  virtual Node * clone() const { return new forNode ( *this ); }

  // -- Output

  virtual void output_stmt(output_context & ct, Node * par);
};

/** @brief Jump node
 *  @ingroup AST
 *
 *  This class provides a base class for the statements that jump to
 *  another location unconditionally. This includes goto statements, break
 *  statements, and continue statements.
 *
 *  The @ref NodeType is determined by the subclass (Goto, Break Continue)
 *  */

class jumpNode : public stmtNode
{

private:

public:

  /** @brief Create a new jump statement
   *
   *  The new jump statement has the given type. This constructor should
   *  only be used by the subclasses (gotoNode, breakNode and
   *  continueNode).
   *
   *  @param typ the type of jump statement (Goto, Break, Continue).
   *  @param coord the location of the construct in the source file. */

  jumpNode(NodeType typ, const Coord coord);

  /** @brief Destroy a jumpNode.
   * 
   *  @warning Do not use the destructors to delete AST nodes. Instead,
   *  rely on the node garbage collector. */

  virtual ~jumpNode();

};

/** @brief Goto statement
 *  @ingroup AST
 *
 *  This class represents the unconditional goto statement. It includes a
 *  reference that can be used to record the target label. Typically, it is
 *  created only with the name of the target label, and later the actual
 *  label reference can be computed with the goto_label_walker. 
 *
 *  The @ref NodeType is Goto.
 *
 *  @see goto_label_walker */

class gotoNode : public jumpNode
{

private:

  /** @brief the target label
   *
   *  Unless explicitly specified, this field is initially null. It can be
   *  computed by calling the goto_label_walker. */

  REF labelNode * _label;

  /** @brief the name of the targete label */

  string _name;

public:

  /** @brief Create a new goto statement
   *
   *  The new goto statement automatically targets the given label, and
   *  takes its name from that label.
   *
   *  @param label the target label
   *  @param coord the location of the construct in the source file. */

  gotoNode(labelNode * label, const Coord coord = Coord::Unknown);

  /** @brief Create a new goto statement from an identifier
   *
   *  The new goto statement takes its name from the input identifier. The
   *  target label reference remains null. (Was ResolveGoto from
   *  procedure.c)
   *
   *  @param ident the identifier providing the name on the goto
   *  @param coord the location of the construct in the source file. */

  gotoNode(idNode * ident, const Coord coord);

  /** @brief Destroy a gotoNode.
   * 
   *  @warning Do not use the destructors to delete AST nodes. Instead,
   *  rely on the node garbage collector. */

  virtual ~gotoNode();

  /** @name Accessors
   *
   *  Methods to get and set fields in the class. */
  //@{

  inline labelNode * label() const { return _label; }
  void label(labelNode * label);

  inline string & name() { return _name; }
  inline void name(string name) { _name = name; }

  //@}

  /** @name AST Traversal
   *
   *  Methods to uniformly traverse the AST. See the documentation in the Node class. */
  //@{

  virtual void visit(Visitor * the_visitor);
  virtual void walk(Walker & the_walker);
  virtual Node * change(Changer & the_changer, bool redispatch = false);

  //@}

  // -- Dataflow

  virtual void dataflow(FlowVal * v, FlowProblem & fp);

  // -- Clone

  virtual Node * clone() const { return new gotoNode ( *this ); }

  // -- Output

  virtual void output_stmt(output_context & ct, Node * par);
};

/** @brief Continue statement
 *  @ingroup AST
 *
 *  This class represents the continue statement. It has a reference to the
 *  containing loop, which is initially set to null. It is computed after
 *  parsing by the set_container_walker.
 *
 *  The @ref NodeType is Continue
 *
 *  @see set_container_walker */

class continueNode : public jumpNode
{

private:

  /** @brief a reference the containing loop
   *
   *  Initiallly, this reference is null, but is computed after parsing by
   *  the set_container_walker. */

  REF loopNode * _container;

public:

  /** @brief Create a new continue statement within a loop
   *
   *  The new continue statement automatically has the given containing
   *  loop. Pass null if the containing loop is unknown.
   *
   *  @param container the containing loop, or null.
   *  @param coord the location of the construct in the source file. */

  continueNode(loopNode * container, const Coord coord = Coord::Unknown);

  /** @brief Create a new continue statement without a loop
   *
   *  The new continue statement has no containing loop reference. It can
   *  be computed using the set_container_walker.
   *
   *  @param coord the location of the construct in the source file. */

  continueNode(const Coord coord = Coord::Unknown);

  /** @brief Destroy a continueNode.
   * 
   *  @warning Do not use the destructors to delete AST nodes. Instead,
   *  rely on the node garbage collector. */

  virtual ~continueNode();

  /** @name Accessors
   *
   *  Methods to get and set fields in the class. */
  //@{

  inline loopNode * container() const { return _container; }
  inline void container(loopNode * container) { _container = container; }

  //@}

  /** @name AST Traversal
   *
   *  Methods to uniformly traverse the AST. See the documentation in the Node class. */
  //@{

  virtual void visit(Visitor * the_visitor);
  virtual void walk(Walker & the_walker);
  virtual Node * change(Changer & the_changer, bool redispatch = false);

  //@}

  // -- Dataflow

  virtual void dataflow(FlowVal * v, FlowProblem & fp);

  // -- Clone

  virtual Node * clone() const { return new continueNode ( *this ); }

  // -- Output

  virtual void output_stmt(output_context & ct, Node * par);
};
  
/** @brief Break statement
 *  @ingroup AST
 *
 *  This class represents the break statement. It has a reference to the
 *  containing loop or switch statement, which is initially set to null. It
 *  is computed after parsing by the set_container_walker.
 *
 *  The @ref NodeType is Break
 *
 *  @see set_container_walker */

class breakNode : public jumpNode
{

private:

  /** @brief a reference the containing statement
   *
   *  A break statement may be contained in a loop, or a switch
   *  statement. Initiallly, this reference is null, but is computed after
   *  parsing by the set_container_walker. */

  REF stmtNode * _container;

public:

  /** @brief Create a new break statement within a statement
   *
   *  The new break statement automatically has the given containing
   *  statement. Pass null if the containing statement is unknown.
   *
   *  @param container the containing statement, or null.
   *  @param coord the location of the construct in the source file. */

  breakNode(stmtNode * container, const Coord coord = Coord::Unknown);

  /** @brief Create a new break statement without a statement
   *
   *  The new break statement has no containing statement reference. It can
   *  be computed using the set_container_walker.
   *
   *  @param coord the location of the construct in the source file. */

  breakNode(const Coord coord = Coord::Unknown);

  /** @brief Destroy a breakNode.
   * 
   *  @warning Do not use the destructors to delete AST nodes. Instead,
   *  rely on the node garbage collector. */

  virtual ~breakNode();

  /** @name Accessors
   *
   *  Methods to get and set fields in the class. */
  //@{

  inline stmtNode * container() const { return _container; }
  inline void container(stmtNode * container) { _container = container; }

  //@}

  /** @name AST Traversal
   *
   *  Methods to uniformly traverse the AST. See the documentation in the Node class. */
  //@{

  virtual void visit(Visitor * the_visitor);
  virtual void walk(Walker & the_walker);
  virtual Node * change(Changer & the_changer, bool redispatch = false);

  //@}

  // -- Dataflow

  virtual void dataflow(FlowVal * v, FlowProblem & fp);

  // -- Clone

  virtual Node * clone() const { return new breakNode ( *this ); }

  // -- Output

  virtual void output_stmt(output_context & ct, Node * par);
};

/** @brief Return statement
 *  @ingroup AST
 *
 *  This class represents the return statement. It is classified as a jump
 *  because control leaves the procedure unconditionally.
 *
 *  The @ref NodeType is Return.
 *  */

class returnNode : public jumpNode
{

private:

  /** @brief the return value, or null if there is none */

  TREE exprNode * _expr;

  /** @brief a reference to the enclosing procedure */

  REF procNode * _proc;

public:

  /** @brief Create a new return statement
   *
   *  The new return statement has the given return value expression and
   *  containing procedure. For return statements with no return value,
   *  pass null for the expression.
   *
   *  @param expr the return value expression, or null
   *  @param proc the enclosing procedure
   *  @param coord the location of the construct in the source file. */

  returnNode(exprNode * expr, procNode * proc,
	     const Coord coord = Coord::Unknown);

  /** @brief Destroy a returnNode.
   * 
   *  @warning Do not use the destructors to delete AST nodes. Instead,
   *  rely on the node garbage collector. */

  virtual ~returnNode();

  /** @name Accessors
   *
   *  Methods to get and set fields in the class. */
  //@{

  inline exprNode * expr() const { return _expr; }
  inline exprNode * get_expr() { exprNode * out = _expr; _expr = 0; return out; }
  inline void expr(exprNode * expr) { _expr = expr; }

  inline procNode * proc() const { return _proc; }
  inline void proc(procNode * proc) { _proc = proc; }

  //@}

  /** @name AST Traversal
   *
   *  Methods to uniformly traverse the AST. See the documentation in the Node class. */
  //@{

  virtual void visit(Visitor * the_visitor);
  virtual void walk(Walker & the_walker);
  virtual Node * change(Changer & the_changer, bool redispatch = false);

  //@}

  // -- Dataflow

  virtual void dataflow(FlowVal * v, FlowProblem & fp);

  // -- Clone

  virtual Node * clone() const { return new returnNode ( *this ); }

  // -- Output

  virtual void output_stmt(output_context & ct, Node * par);
};

/** @brief Attribute
 *  @ingroup AST
 *
 *  -- Holds extra non-ANSI attributes
 *  */

class attribNode : public stmtNode
{

private:

  TREE exprNode * _arg;
  string _name;

public:

  // -- Constructors

  attribNode(const char * name, exprNode * arg,
	     const Coord coord = Coord::Unknown);

  //    Was ConvertIdToAttrib in ast.c

  attribNode(idNode * id, exprNode * arg);

  /** @brief Destroy a attribNode.
   * 
   *  @warning Do not use the destructors to delete AST nodes. Instead,
   *  rely on the node garbage collector. */

  virtual ~attribNode();

  /** @name Accessors
   *
   *  Methods to get and set fields in the class. */
  //@{

  inline string & name() { return _name; }
  inline void name(string name) { _name = name; }

  inline exprNode * arg() const { return _arg; }
  inline exprNode * get_arg() { exprNode * out = _arg; _arg = 0; return out; }
  inline void arg(exprNode * arg) { _arg = arg; }

  //@}

  /** @name AST Traversal
   *
   *  Methods to uniformly traverse the AST. See the documentation in the Node class. */
  //@{

  virtual void visit(Visitor * the_visitor);
  virtual void walk(Walker & the_walker);
  virtual Node * change(Changer & the_changer, bool redispatch = false);

  //@}

  // -- Dataflow

  virtual void dataflow(FlowVal * v, FlowProblem & fp) {}

  // -- Clone

  virtual Node * clone() const { return new attribNode ( *this ); }

  // -- Output

  virtual void output_stmt(output_context & ct, Node * par);
};

/** @brief Text
 *  @ingroup AST
 *
 *  -- Random text (could be used to store comments)
 *  */

class textNode : public Node
{

private:

  string _text;
  bool _start_new_line;

public:

  // -- Constructors

  textNode(const char * text, bool start_new_line,
	   const Coord coord = Coord::Unknown);

  /** @brief Destroy a textNode.
   * 
   *  @warning Do not use the destructors to delete AST nodes. Instead,
   *  rely on the node garbage collector. */

  virtual ~textNode();

  /** @name Accessors
   *
   *  Methods to get and set fields in the class. */
  //@{

  inline string & text() { return _text; }
  inline void text(string text) { _text = text; }

  inline bool start_new_line() const { return _start_new_line; }
  inline void start_new_line(bool start_new_line) { _start_new_line = start_new_line; }

  //@}

  /** @name AST Traversal
   *
   *  Methods to uniformly traverse the AST. See the documentation in the Node class. */
  //@{

  virtual void visit(Visitor * the_visitor);
  virtual void walk(Walker & the_walker);
  virtual Node * change(Changer & the_changer, bool redispatch = false);

  //@}

  // -- Dataflow

  virtual void dataflow(FlowVal * v, FlowProblem & fp) {}

  // -- Clone

  virtual Node * clone() const { return new textNode ( *this ); }

  // -- Output

  virtual void output(output_context & ct, Node * par);
};

// ------------------------------------------------------------
//   Template functions
// ------------------------------------------------------------

template< class T >
void dataflow_forward_list(list< T > & l, FlowVal * v, FlowProblem & fp)
{
  for (list< T >::iterator p = l.begin();
       p != l.end();
       ++p)
    (*p)->dataflow(v, fp);
}

template< class T >
void dataflow_reverse_list(list< T > & l, FlowVal * v, FlowProblem & fp)
{
  for (list< T >::reverse_iterator p = l.rbegin();
       p != l.rend();
       ++p)
    (*p)->dataflow(v, fp);
}

template< class T >
void output_list(list< T > & l, output_context & ct, Node * par)
{
  for (list< T >::iterator p = l.begin();
       p != l.end();
       ++p)
    (*p)->output(ct, par);
}

template< class T >
void output_delim_list(list< T > & l, output_context & ct, Node * par, char delim)
{
  for (list< T >::iterator p = l.begin();
       p != l.end();
       ++p)
    {
      if (p != l.begin()) {
	ct << delim;
	ct.space();
	ct.continue_line();
      }
      (*p)->output(ct, par);
    }
}

template< class T >
void delete_list(list< T > & l)
{
  for (list< T >::iterator p = l.begin();
       p != l.end();
       ++p)
    delete (*p);
}

#endif // CBZ_AST_H
