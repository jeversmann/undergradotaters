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

// **********************************************************************
//
//   THIS FILE IS OBSOLETE
//
//   It has been replaced by options.h
//
// **********************************************************************


#ifndef CBZ_POINTERS_COMMON_H
#define CBZ_POINTERS_COMMON_H

#include <set>

typedef set< basicblockNode * > basicblock_set;
typedef basicblock_set::iterator basicblock_set_p;

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

  static int Threshold;

  /** @brief Use multiplicity analysis */

  static bool Use_multiplicity;

  /** @brief Verbose output */

  static bool Verbose;
  
  /** @brief Pointer statistics */

  static bool Pointer_statistics;

  /** @brief Ignore struct/union fields */

  static bool Ignore_fields;

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

};


#endif // CBZ_POINTERS_COMMON_H

