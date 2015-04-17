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

#ifndef CBZ_PHASE_H
#define CBZ_PHASE_H



// ------------------------------------------------------------
// Phase: override the run() method
// ------------------------------------------------------------

class Phase
{
public:

  virtual void run() =0;

  virtual void get_flags(str_list_p & arg) { };
};

// ------------------------------------------------------------
// Phase Registry
// ------------------------------------------------------------

// -- Phase entry

class Phase_entry
{
private:

  string _name;
  int _order;
  Phase * _phase;

public:

  Phase_entry(const char * name, Phase * phase)
    : _name(string(name)),
      _order(0),
      _phase(phase)
  {}

  ~Phase_entry() { delete _phase; }

  inline int order() const { return _order; }
  inline void order(int order) { _order = order; }

  inline void disable() { _order = 0; }
  inline bool is_enabled() const { return _order != 0; }

  inline string & name() { return _name; }
  inline const string & name() const { return _name; }
  inline void name(string & name) { _name = name; }

  inline Phase * phase() const { return _phase; }
};

typedef vector< Phase_entry * > phase_vec;
typedef phase_vec::iterator phase_vec_p;

// -- Phase_entry comparator for sorting

class Less_phases
{
public:
  int operator()(const Phase_entry * ph1, const Phase_entry * ph2)
  { return ph1->order() < ph2->order(); }
};

class Equal_phases
{
private:

  string & _name;

public:

  Equal_phases(string & name)
    : _name(name)
  {}

  bool operator()(const Phase_entry * ph)
  { return ph->name() == _name; }
};

// -- Registry class
// Each use of the constructor puts a new entry into the registry.

class Phases
{

private:

  // -- The registry..

  static int _count;
  static int _cur_order;
  static phase_vec * _phases;

public:

  // -- Constructor registers a new phase

  Phases(const char * name, Phase * new_phase);

  ~Phases();

  // -- Call the phases

  static void run_all();

  // -- Handle command line argument

  static bool phase_flag(str_list_p & arg);

  // -- Set priority

  static void next_phase(string & name);
  static void disable_phase(string & name);
  
};



#endif // CBZ_PHASE_H
