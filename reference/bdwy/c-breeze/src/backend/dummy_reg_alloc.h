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
//  Paul Navratil
//  Charles Nevill
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

#ifndef _DUMMY_REG_ALLOC_H
#define _DUMMY_REG_ALLOC_H

#include "reg_alloc.h"

//
// dummy register allocator - assumes all variables stored in stack
//	and fixes up virtual register references to be real references.
//
class dummy_reg_alloc : public reg_alloc
{
public:

	// ctor/dtor
	dummy_reg_alloc();
	~dummy_reg_alloc();

	// allocate registers for a procedure
	void allocate(procNode *proc);

public:
///////////////////
// data structures

	// forward defines
	struct rreg_info;
	struct vreg_info;

	// status of a particular virtual register
	struct vreg_info
	{
		// ctor
		vreg_info() : pRealReg(0), reclaimed(false)
		{
		}

		// a register object representing this register
		Register reg;

		// real register to which it's currently allocated
		rreg_info * pRealReg;

		// whether or not the real register we used for this has been reclaimed
		bool reclaimed;
	};

	// status of a particular real register
	struct rreg_info
	{
		// ctor
		rreg_info() : pInfo(0), pVirtualReg(0)
		{
		}

		// info about this register
		arch_info::register_info * pInfo;

		// a register object representing this register
		Register reg;

		// virtual register currently living in this real register
		//  or NULL if unoccupied.
		vreg_info * pVirtualReg;
	};


protected:
///////////////////
// helpers

	// setup real register info
	void setup_rregs();

	// find out if a register is real (otherwise it's virtual)
	bool				is_real_reg( const Register & reg );

	// get info for a virtual register
	vreg_info*			get_vreg( const Register & vreg );

	// get info for a real register
	rreg_info*			get_rreg( const Register & rreg );

	// verify that a particular virtual register already has a real register
	//	and change the virtual to match the real
	// returns false if the virtual register does not have a real register
	//	associated with it or if one could not be allocated.
	bool				fix_allocated_virtual( Register & reg, const LirInst & inst );

	// allocate a real register for a virtual register.
	// returns false one could not be allocated.
	bool				allocate_real_for_virtual( Register & reg, declNode * pDecl, const LirInst & inst );


protected:

	// info on each virtual register - indexed by register number, 
	//	NULL if never seen before
	vector<vreg_info*>	_vregs;

	// info on each virtual register - indexed by register number, 
	//	NULL if never seen before
	vector<rreg_info*>	_rregsGpr;
	int					_rregsGprCount;
	vector<rreg_info*>	_rregsFpr;
	int					_rregsFprCount;

	// most recently-allocated real register
	int					_lastRealRegFixed;
	int					_lastRealRegFloat;

};


class dummy_reg_alloc_walker : public Walker
{
public:
  // ctor
  dummy_reg_alloc_walker();

  // handle various things
  void at_unit(unitNode *the_unit, Order ord);
  void at_proc(procNode *the_proc, Order ord);

};

#endif
