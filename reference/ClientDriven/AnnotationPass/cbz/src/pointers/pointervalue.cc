// $Id: pointervalue.cc,v 1.7 2003/08/07 23:14:31 pnav Exp $
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

#include "c_breeze.h"
#include "pointervalue.h"

// ------------------------------------------------------------
// pointerValue class
// ------------------------------------------------------------

int pointerValue::count = 0;

pointerValue::pointerValue()
  : blocks(),
    // value(0),
    constant_value(0),
    is_address(false),
    is_a_use(false),
    dereferenced()
{
  count++;
}

pointerValue::pointerValue(memoryBlock * mb, bool is_address)
  : blocks(),
    // value(0),
    constant_value(0),
    is_address(is_address),
    is_a_use(false),
    dereferenced()
{
  blocks.insert(mb);
  count++;
}
/** @brief Copy constructor
 */

pointerValue::pointerValue(const pointerValue & other)
  : blocks(other.blocks),
    constant_value(other.constant_value),
    is_address(other.is_address),
    is_a_use(other.is_a_use),
    dereferenced(other.dereferenced)
{
  count++;
}

 
pointerValue::~pointerValue()
{
  count--;
}
