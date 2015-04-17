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

#ifndef CBZ_POINTERVALUE_H
#define CBZ_POINTERVALUE_H

#include "pointeroptions.h"
#include "location.h"
#include "memorymodel.h"
#include "memoryblock.h"
#include "ipanalysis.h"
#include "handle.h"

typedef pair< memoryBlock *, memoryUse * > pointer_use;
typedef set< pointer_use > pointer_use_set;
typedef pointer_use_set::iterator pointer_use_set_p;

/** @brief Pointer value class
 *
 *  This class holds a temporary pointer value, which consists of a set of
 *  target memory blocks, and a boolean that indicates whether it's an
 *  "address-of" pointer. We need this because expressions such as "&y"
 *  don't have storage associated with them. */

class pointerValue
{
public:

  static int count;

public:

  /** @brief Memory blocks
   *
   * The objects that make up the "value" of the pointer analysis lattice
   * element. */

  memoryblock_set blocks;

  /** @brief Constant value
   *
   * For constant propagation and constant folding */

  const constant * constant_value;

  /** @brief Is address?
   *
   * True if this represents the address of the memory blocks. */

  bool is_address;

  /** @brief Is a use?
   *
   * True if the objects will be used. */

  bool is_a_use;

  /** @brief Dereferenced pointers
   *
   * Keep a list of the pointers that are dereferenced in order to get this
   * value. */

  memoryblock_set dereferenced;

public:

  /** @brief Defaut constructor */

  pointerValue();

  /** @brief Single memory block constructor */

  pointerValue(memoryBlock *mb, bool is_address = false);

  /** @brief Copy constructor
   */

  pointerValue(const pointerValue & other);

  /** @brief Destructor
   */

  ~pointerValue();

  /** @brief Copy pointers */

  inline void copy_pointers_from(const pointerValue & other) {
    blocks = other.blocks;
    is_address = other.is_address;
    dereferenced = other.dereferenced;
  }

  /** @brief Reinitialize the value */

  inline void initialize_pointers() {
    blocks.clear();
    is_address = false;
    dereferenced.clear();
  }

  /** @brief Add blocks from another pointer */

  inline void add_blocks_from(const pointerValue & other) {
    blocks.insert(other.blocks.begin(), other.blocks.end());
    is_address = other.is_address;
    dereferenced.insert(other.dereferenced.begin(),
			other.dereferenced.end());
  }

  /** @brief Is assignable?
   * 
   * Is the given pointerValue an L-value */

  inline bool is_lvalue() { return ( ! is_address && ! blocks.empty()); }
};

typedef list< pointerValue > pointervalue_list;
typedef pointervalue_list::iterator pointervalue_list_p;

#endif // CBZ_POINTERVALUE_H
