// $Id: phase.cc,v 1.5 2003/08/07 23:14:03 pnav Exp $
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

// -- The registry..

int Phases::_count = 0;
int Phases::_cur_order = 1;
phase_vec * Phases::_phases = 0;

// ------------------------------------------------------------
// Constructor
// ------------------------------------------------------------

Phases::Phases(const char * name, Phase * new_phase, const char * desc)
{
  if (! _count++)
    _phases = new phase_vec();
  
  string nm = string(name);
  phase_vec_p p = find_if(_phases->begin(), _phases->end(), Equal_phases(nm));
  if ( p != _phases->end() )
    cerr << "Phase name \"" << name << "\" is already registered." << endl;
  else
    _phases->push_back(new Phase_entry(name, new_phase, desc));
}

// ------------------------------------------------------------
// Destructor
// ------------------------------------------------------------

Phases::~Phases()
{
  if (! --_count) {
    for (phase_vec_p p = _phases->begin();
	 p != _phases->end();
	 ++p)
      delete (*p);

    delete _phases;
  }
}

// ------------------------------------------------------------
// Phase operations...
// ------------------------------------------------------------

// phase_flag checks the current command-line argument to see if
// it refers to one of the registered phases. If so, it gives
// the phase a chance to get any extra arguments needed.

bool Phases::phase_flag(str_list_p & arg_p)
{
  string arg = (*arg_p);
  bool dis = false;
  str_list_p next;
  bool was_phase_flag = false;

  if (arg[0] == '-') {

    // Remove the leading "-"

    arg.erase(0,1);

#if 0
    // Check for disabling argument

    if (arg.compare("no-",3) == 0) {
      dis = true;
      arg.erase(0,3);
    }
#endif

    // Find the phase by name

    phase_vec_p p = find_if(_phases->begin(), _phases->end(),
			    Equal_phases(arg));

    if (p != _phases->end()) {

      was_phase_flag = true;
      ++arg_p;

      if (dis)
	(*p)->disable();
      else {
	(*p)->order(++_cur_order);
	(*p)->phase()->get_flags(arg_p);
      }
    }
  }

  return was_phase_flag;
}

void Phases::run_all()
{
  stable_sort(_phases->begin(), _phases->end(), Less_phases());

  for (phase_vec_p p = _phases->begin();
       p != _phases->end();
       ++p) {
    if ((*p)->is_enabled())
      (*p)->phase()->run();
  }
}

void Phases::next_phase(string & the_name)
{
  phase_vec_p p = find_if(_phases->begin(), _phases->end(),
			  Equal_phases(the_name));

  if (p != _phases->end())
    (*p)->order(++_cur_order);
}

void Phases::disable_phase(string & the_name)
{
  phase_vec_p p = find_if(_phases->begin(), _phases->end(),
			  Equal_phases(the_name));

  if (p != _phases->end())
    (*p)->disable();
}
