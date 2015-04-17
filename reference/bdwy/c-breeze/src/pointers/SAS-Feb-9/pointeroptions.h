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

#ifndef CBZ_POINTEROPTIONS_H
#define CBZ_POINTEROPTIONS_H

#include <set>

typedef set< basicblockNode * > basicblock_set;
typedef basicblock_set::iterator basicblock_set_p;

typedef set< string > str_set;
typedef str_set::iterator str_set_p;

typedef set< declNode * > decl_set;
typedef decl_set::iterator decl_set_p;

typedef set< stmtNode * > stmt_set;
typedef stmt_set::iterator stmt_set_p;

// -- A walker to find callNodes

class findCallNodeWalker : public Walker
{
public:

  static callNode * find(stmtNode * s)
  {
    findCallNodeWalker fcnw;
    s->walk(fcnw);
    return fcnw.call;
  }

private:

  callNode * call;

public:

  findCallNodeWalker()
    : Walker(Preorder, Subtree),
      call(0)
  {}

  void at_call(callNode * the_call, Order ord)
  {
    call = the_call;
  }
};

// -- Manage flow sensitivity

class memoryBlock;

class FSKey
{
public:

  stmtNode * statement;
  declNode * decl;
  string field_name;

  FSKey(stmtNode * s, declNode * d, string & f)
    : statement(s),
      decl(d),
      field_name(f)
  {}

  bool operator==(const FSKey & other) {
    if ((statement == other.statement) &&
	(decl == other.decl) &&
	(field_name == other.field_name))
      return true;
    else
      return false;
  }
};

// typedef pair< stmtNode *, declNode * > flow_sensitive_pair;
typedef list< FSKey > flow_sensitive_set;
typedef flow_sensitive_set::iterator flow_sensitive_set_p;

typedef enum { Forward, Backward } Direction;

/** @brief Pointer options
 *
 * This class contains static flags that control the behavior of the
 * pointer analyzer. The actual definitions (and default values) reside in
 * pointers.cc. */

class pointerOptions
{
public:

  /** @brief Context-sensitivity control */

  static bool Context_insensitive;

  /** @brief Use multiplicity analysis */

  static bool Use_multiplicity;

  /** @brief Verbose output
   *
   * During program analysis, show every statement processed along with the
   * corresponding verbose output for either the pointer analyzer or
   * whatever other analysis is being run. */

  static bool Verbose;

  /** @brief Show memory blocks
   *
   * At the end of pointer analysis print out a list of all the
   * memoryblocks along with information about their properties, including
   * numbers of defs and uses. */

  static bool Show_memoryblocks;

  /** @brief Show procedures
   *
   * At the end of pointer analysis print out a list of all the procedures
   * and information about them. */

  static bool Show_procedures;
  
  /** @brief Pointer statistics */

  static bool Pointer_statistics;

  /** @brief Ignore struct/union fields
   *
   * The "dot" operator becomes a no-op. This also forces all updates to
   * the structure to be weak (as if the multiplicity were high). */

  static bool Ignore_fields;

  /** @brief One struct per type
   *
   * With this flag on, there is only one set of memory blocks for each
   * structure type. This emulates the situation in the type-system
   * world. */

  static bool One_struct_per_type;

  /** @brief Show the stack state */

  static bool Show_stack;

  /** @brief Flow-insensitive analysis
   *
   * This is implemented in the following way: (1) every object has only
   * one def that occurs at the main function (and therefore dominates
   * everything), (2) all assignments are additive, (3) we don't perform
   * any control-flow merges (phi functions), and (4) all heap allocations
   * are immediately Unbounded in size. */

  static bool Flow_insensitive;

  /** @brief Conditional analysis
   *
   * When set to true this flag causes the analyzer to skip branches when
   * it can ascertain the value of the condition. */

  static bool Conditional_analysis;

  /** @brief Bi-directional assignment
   *
   * This options is intended to emulate equality-based type analysis.  It
   * causes all assignments (including parameter passing) to include all of
   * the right-hand-side objects on the left hand side.
   *
   * For example, "y = x" becomes "{y,x} = x". */

  static bool Bidirectional_assignment;

  /** @brief Aggressive multiplicity
   *
   * With this option turned on, we use a trick that could be unsafe in
   * some cases, but it's very unlikely. The observation is that if a
   * memoryBlock is allocated and then a subsequent if-then-else frees it
   * on one branch, but not the other, then it is probably the case that
   * either way it is unallocated. For example:
   *
   *  f = fopen(...);
   *  if (f) {
   *    ...
   *    fclose(f);
   *  }
   *
   *  At the end of this code, the file is always closed because in the
   *  case that f is null, the file was never opened in the first place. */

  static bool Aggressive_multiplicity;

  /** @brief One string constant
   *
   * When set true, we only create a single object to represent all string
   * constants. Since we never look at the value, and we don't allow them
   * to be changed, this should not affect our results. */

  static bool One_string_constant;

  /** @brief Context-insensitive memory model
   *
   * When true, all objects, including variables and heap objects, are only
   * instantiated once for each calling context regardless of the analysis
   * mode. */

  static bool Context_insensitive_memory;

  /** @brief Use escape analysis
   *
   * In Pointers::record_external_inputs_and_outputs, use escape analysis
   * to prune out unreachable heap objects. */

  static bool Use_escape_analysis;

  /** @brief Verbose constants
   *
   * During constant propagation, show constant values being computed. */

  static bool Verbose_constants;

  /** @brief Verbose liveness
   *
   * During liveness analysis, show liveness computations. */

  static bool Verbose_liveness;

  /** @brief Show unknown procedures
   */

  static bool Show_unknown_procedures;

  /** @brief Show memory leaks
   *
   * With escape analysis and aggressive multiplicity on, report possible
   * memory leaks. */

  static bool Show_memory_leaks;

  /** @brief Monitor precision
   *
   * When true, the analysis engine monitors precision and builds a graph
   * of the causes and effects of precision loss. */

  static bool Monitor_precision;

  /** @brief Context sensitive procedures
   *
   * Populate this list with procedures that are to be treated context
   * sensitive. */

  static str_set Context_sensitive_procedures;

  /** @brief Flow-sensitive objects
   *
   * Populate this list with variables that are to be treated flow
   * sensitive. */

  static flow_sensitive_set Flow_sensitive_objects;

  /** @brief Flow-sensitive allocation objects
   *
   * Populate this list with allocation objects that need to be flow
   * sensitive in order to improve the4 multiplicity analysis. */

  static flow_sensitive_set Flow_sensitive_allocation_objects;

  /** @brief Path-sensitive branches
   *
   * Record the basic block that controls path sensitive information --
   * that is, the branch at the end of the basic block needs to be path
   * sensitive.*/

  static basicblock_set Path_sensitive_branches;

  /** @brief Verbose procedures
   *
   * Turn on verbose output when we're analyzing these procedures. */

  static str_set Verbose_procedures;
};


#endif // CBZ_POINTEROPTIONS_H

