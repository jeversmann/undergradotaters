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

#ifndef CBZ_MEMORYMODEL_H
#define CBZ_MEMORYMODEL_H

#include "pointeroptions.h"
#include "location.h"

class memoryBlock;

class memoryModel
{
public:

  typedef pair< Location *, declNode * > memorymodel_key;

  typedef map< memorymodel_key , memoryBlock *> memorymodel_map;
  typedef memorymodel_map::iterator memorymodel_map_p;
  typedef memorymodel_map::const_iterator memorymodel_map_cp;

  typedef map< procNode *, memorymodel_map > proc_memorymodel_map;
  typedef proc_memorymodel_map::iterator proc_memorymodel_map_p;
  typedef proc_memorymodel_map::const_iterator proc_memorymodel_map_cp;

  typedef pair< stmtNode *, declNode * > ci_memorymodel_key;

  typedef map< ci_memorymodel_key , memoryBlock *> ci_memorymodel_map;
  typedef ci_memorymodel_map::iterator ci_memorymodel_map_p;
  typedef ci_memorymodel_map::const_iterator ci_memorymodel_map_cp;


  typedef map< constNode *, memoryBlock *> string_constant_map;
  typedef string_constant_map::iterator string_constant_map_p;

private:

  /** @brief the memoryBlocks
   *
   *  All memoryBlocks are stored and managed by the memoryModel
   *  object. They are indexed by combining their declaration with a
   *  program location. A synthetic declaration is generated for dynamic
   *  objects. */

  TREE proc_memorymodel_map _memory_blocks;

  /** @brief Context-insensitive memory
   *
   * We use this map to store memoryBlocks that are indexed by the
   * stmtNode, not the location. This is only used in
   * Context_insensitive_memory mode. */

  TREE ci_memorymodel_map _ci_memory_blocks;

  /** @brief String constants
   *
   * Create a separate repository of string constants, indexed by their
   * constNodes */

  string_constant_map _string_constants;

  /** @brief a counter to generate unique names */

  int Counter;

  /** @brief special "null" object */

  memoryBlock * Null;

  /** @brief String constant object
   *
   * When we have the one-string-constant option trurned on, this holds a
   * pointer to that object. */

  memoryBlock * _string_constant;
  
public:

  /** @brief Create a new memory model
   */

  memoryModel();

  /** @brief Delete the memory model
   *
   *  This also deallocates all memoryBlock objects created in this
   *  model. */

  ~memoryModel();

  /** @brief Clear
   *
   *  Visit all memoryBlocks and call clear, preparing for reanalysis. */

  void clear();

  /** @brief Get the number of memory blocks. */

  int size();

  /** @brief Get the list of memoryBlocks */

  // const memorymodel_map & memory_blocks() const { return _memory_blocks; }

  /** @brief Return the null object. */

  memoryBlock * null() const { return Null; }

  /** @brief Look up a memoryBLock
   *
   *  Look up a memoryblock, returning "null" if it's not found. */

  memoryBlock * lookup(Location * location, declNode * decl);

  /** @brief Look up a CI memoryBLock
   *
   *  Look up a memoryblock, returning "null" if it's not found. */

  memoryBlock * lookup(stmtNode * location, declNode * decl);

  /** @brief Look up a variable
   *
   *  Create the memory block if it's not found. Also, when creating a new
   *  memory block, if it's a built-in array, then create objects for the
   *  elements. */

  memoryBlock * lookup_variable(Location * location,
				declNode * decl,
				procNode * local_to);

  /** @brief Look up or create a heap object
   *
   * Lookup a heap object based on the given location and Node key. If it's
   * not found, create a new one. The declaration can be NULL, in which
   * case a synthetic declNode will be created using the name. */

  memoryBlock * lookup_heap_object(const string & name,
				   stmtLocation * allocation_site,
				   stmtNode * allocation_stmt,
				   declNode * decl,
				   bool synthetic_decl);

  /** @brief Look up string constant
   *
   * String constants are indexed by their constNode. */

  memoryBlock * lookup_string_constant(constNode * the_string);

  /** @brief Generate a struct/union field
   *
   * Given a field declaration and the container object, generate an object
   * for the field. We use the type information from the field declaration
   * to detect and handle fields that are arrays.
   *
   * The field_decl can be ommited.
   *
   * The method actually generates a new unique declNode for the object. */

  memoryBlock * generate_su_field(const string & field_name,
				  declNode * field_decl,
				  memoryBlock * container);

  /** @brief Update def-use chains
   *
   *  At the end of pointer analysis, call this method to populate the
   *  definition uses on each memoryDef. Only call this once! */

  void update_def_use_chains();

  /** @brief Counter
   *
   * Get the counter and increment it. */

  int counter() { Counter++; return Counter; }

  /** @brief Print statistics */

  void stats(ostream & out);

  void print(ostream & o) const;

private:

  /** @brief Create a new memory object
   *
   * Create a new object that represents some entity in memory. These
   * objects are indexed by the location and Node given. For regular
   * variables, this Node will be the same as the declNode. However, for
   * heap objects and strings constants, we will use other kinds of nodes
   * to distinguish them. In addition, we need the declaration node for
   * built-in arrays, so that we can create the objects to represent the
   * elements.
   *
   * This is the only function that creates new memoryBlocks and stores
   * them in the memory model.
   *
   * The synthetic_decl flag indicates that the declNode passed was created
   * just to distinguish this object. */

  memoryBlock * create_memory_object(Location * location,
				     declNode * decl,
				     bool synthetic_decl,
				     memoryBlock * container,
				     procNode * local_to);

  /** @brief Create a CI memory block
   *
   */

  memoryBlock * create_memory_object(stmtNode * location,
				     declNode * decl,
				     bool synthetic_decl,
				     memoryBlock * container,
				     procNode * local_to);

  /** @brief Generate array elements
   *
   * For built-in arrays, we generate a single object for each dimension of
   * the array. For example,
   *
   * int A[3][4][5]
   *
   * Results in
   *
   *   A --> A_el_1 --> A_el_2 --> A_el_3
   *
   * That way A[1][3] evaluates to A_el_2, as does *(A[1]) and **A.
   *
   * This structure is represented by storing the element as a field of the
   * array object. We then add a special def that also causes the array to
   * point-to it's element. */

  void generate_array_elements_for(memoryBlock * array_object);


  /** @brief Initialize struct
   *
   * Assign static initializers to a newly created struct. */

  void initialize_struct(memoryBlock * struct_object,
			 sueNode * sue_type,
			 expr_list & inits);
};

#endif // 
