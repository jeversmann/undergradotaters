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

#ifndef CBZ_IPCONSTANTS_H
#define CBZ_IPCONSTANTS_H

#include "pointers.h"
#include "ipanalysis.h"

// ------------------------------------------------------------
// ipConstant
// ------------------------------------------------------------

/** @brief A single constant value associated with one def of a variable.
 
    This class represents a single constant value associated with one
    def of a variable. It can hold either a constant value, or have the
    special lattice TOP value. The lattice BOTTOM is represented by a
    constant no_val (which is built into the constant class).
*/
class ipConstant : public analysisVal
{
  friend class ipConstantPropagation;
  friend class ipConstantsChanger;

private:

  constant _value;
  bool _top;
  bool _internal;

  ipConstant(constant & value);
  ipConstant();
  ipConstant(const ipConstant & other);
  virtual ~ipConstant();

  /** @defgroup IPCONSTANTS_H_FIELDS
      @brief Fields
  */
  /*@{*/

  void to_bottom();
  bool is_top() const;
  bool is_bottom() const;

  inline const constant & value() const { return _value; }
  inline constant & value() { return _value; }
  
  /*@}*/
  
public:

  inline void intern() { _internal = true; }

  void assign(const ipConstant * other);

  /// Comparison
  virtual bool diff(analysisVal * other) const;


  /** @brief Meet "^" operator.

      Must satisfy the following:
       - Associative: x ^ (y ^ z) = (x ^ y) ^ z
       - Commutative: x ^ y = y ^ x
       - Idempotent:  x ^ x = x
  */
  virtual void meet_with(const analysisVal * other);

  /** @defgroup IPCONSTANTS_H_COMP_FUNCS
      @brief Computational functions
  */
  /*@{*/
  virtual void binary_operator(const Operator * op,
			       const analysisVal * right_operand);

  virtual void unary_operator(const Operator * op);

  virtual void cast_operator(const typeNode * type);
  /*@}*/

  /// Output
  void print(ostream & o);

};

// ------------------------------------------------------------
// ipConstantPropagation
// ------------------------------------------------------------

typedef map< memoryDef *, ipConstant * > const_variables_map;
typedef const_variables_map::iterator const_variables_map_p;

typedef map< constNode * , ipConstant * > constants_map;
typedef constants_map::iterator constants_map_p;

typedef map< declNode * , ipConstant * > enums_map;
typedef enums_map::iterator enums_map_p;

typedef set< ipConstant * > ipconstant_set;
typedef ipconstant_set::iterator ipconstant_set_p;

#include "path.h"

/** @brief Controls data for management of constant propagation problem.

    This class controls the constant propagation algorithm by holding
    the current states of all objects, looking them up when needed, and
    setting their values at assignments.
*/
class ipConstantPropagation : public analysisProblem
{
private:

  const_variables_map _values;
  constants_map _constants;
  enums_map _enums;
  ipConstant * _top;
  ipConstant * _bottom;
  bool _debug;

  ipconstant_set _deleted;
  int _count;

public:

  ipConstantPropagation(bool debug = false)
    : _values(),
      _constants(),
      _top(new ipConstant()),
      _bottom(new ipConstant()),
      _debug(debug),
      _deleted(),
      _count(0)
  {
    _bottom->to_bottom();
  }

  /// Process an assignment, return true if the state of the defined object changes.
  virtual bool assignment(const Path * where,
			  memoryDef * left_hand_side,
			  analysisVal * right_hand_side,
			  bool is_strong_update);

  /** @defgroup IPCONSTANTS_H_LOOKUP
      @brief Lookup the flow value for an object

      IMPORTANT:

      -  Return a *copy* of any internal data structures because these analysisVal objects will be deleted.
      - If the given object cannot be found, return 0 and the analyzer will automatically use BOTTOM.
  */
  /*@{*/
  virtual analysisVal * lookup(memoryBlock * block, memoryUse * use);
  virtual analysisVal * lookup(constNode * con);
  virtual analysisVal * lookup(const string & field_name);
  /*@}*/

  /// Free: must be able to accept 0 as an argument
  virtual void free(analysisVal * to_free);

  /** @brief Return a *COPY* of the TOP flow value.

      Must satisfy : x ^ T = x
  */
  virtual analysisVal * top();

  /** @brief Return a *COPY* the BOTTOM flow value.

      Must satisfy : x ^ B = B
  */
  virtual analysisVal * bottom();

  /** @defgroup IPCONSTANTS_H_COMP_FUNC
      @brief Computational functions.

      Computational functions: produce a new analysisVal or
      transform one of the input vals according to the given
      operator. DO NOT DELETE any of the inputs (this is handled
      automatically). Optionally, return 0 to indicate a bad
      computation.
  */
  /*@{*/
  virtual analysisVal * binary_operator(const Operator * op,
					const analysisVal * left_operand,
					const analysisVal * right_operand);

  virtual analysisVal * unary_operator(const Operator * op,
				       const analysisVal * operand);

  virtual analysisVal * cast_operator(const typeNode * type,
				      const analysisVal * operand);

  /*@}*/

  /// Print some statistics
  void stats();

private:

  void allocate(ipConstant * ic) {
    _count++;
    /*
    cout << "Allocate : (" << _count << ") ";
    ic->print(cout);
    cout << endl;
    */
  }

  ipConstant * clone(analysisVal * to_clone);
};


#endif // CBZ_IPCONSTANTS_H
