// $Id: worklist.cc,v 1.10 2003/08/07 23:14:33 pnav Exp $
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

#include "c_breeze.h"
#include "worklist.h"

int workList::get_next_block()
{
  // When is_empty, return 0

  if (is_empty())
    return 0;

  // Search down the list for the next set bit

  while ( ! _worklist.test(_cur_position) ) {
    _cur_position++;
    if (_cur_position == _max_size)
      _cur_position = 0;
  }

  // Find the corresponding basic block and reset the bit

  _worklist.reset(_cur_position);
  _cur_size--;

  // Result

  int result = _cur_position;

  // Always go to the next block

  _cur_position++;
  if (_cur_position == _max_size)
    _cur_position = 0;

  // cout << "WORKLIST: get_next_block, cur_size = " << _cur_size <<
  //  " cur_position = " << _cur_position << endl;

  return result;
}

bool workList::is_empty() const
{
  return _cur_size == 0;
}

void workList::add_block(int position)
{
  // Only add it if its not already there

  if ( ! _worklist.test(position) ) {
    _worklist.set(position);
    _cur_size++;
  }

  // This seems reasonable: go back to earlier blocks right away

  // IT DOESN'T WORK. WHY?
  // if (position < _cur_position)
  //  _cur_position = position;
  
  // cout << "WORKLIST: add_block " << position << ", cur_size = " << _cur_size <<
  //  " cur_position = " << _cur_position << endl;
}

void workList::remove_block(int position)
{
  if (_worklist.test(position) ) {
    _worklist.reset(position);
    _cur_size--;
  }
}


void workList::add_all_blocks()
{
  _worklist.set();
  _cur_position = 0;
  _cur_size = _max_size;

  //  cout << "WORKLIST: add_all_blocks, cur_size = " << _cur_size << endl;
}      

void workList::skip_current_block()
{
  // When is_empty, return 0

  if (is_empty())
    return;

  _cur_position++;
  if (_cur_position == _max_size)
    _cur_position = 0;
}

