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

#include "c_breeze.h"
#include "dummy_reg_alloc.h"

// because I'm lazy
typedef dummy_reg_alloc::vreg_info vreg_info;
typedef dummy_reg_alloc::rreg_info rreg_info;

dummy_reg_alloc::dummy_reg_alloc() 
{
	_rregsGprCount = 0;
	_rregsFprCount = 0;
}

dummy_reg_alloc::~dummy_reg_alloc() 
{
}

void 
dummy_reg_alloc::setup_rregs() 
{
	int i, sz;

	// setup info for gprs
	const arch_info::register_info_list & regsGpr = CBZ::ArchInfo.get_regs_gpr();
	_rregsGprCount = (int)regsGpr.size();
	_rregsGpr.resize( CBZ::ArchInfo.get_all_regs().size(), 0 );
	for ( i = 0; i < _rregsGprCount; ++i )
	{
		int num = regsGpr[i]->_id;

		// make room for it
		if ( num >= (int)_rregsGpr.size() )
			_rregsGpr.resize( num + 1, 0 );

		// setup info for this one
		rreg_info * rreg = new rreg_info;
		rreg->pInfo = regsGpr[i];
		rreg->reg = CBZ::ArchInfo.get_register( rreg->pInfo );

		// add this to the list in the right place
		_rregsGpr[ num ] = rreg;
	}

	// setup info for fprs
	const arch_info::register_info_list & regsFpr = CBZ::ArchInfo.get_regs_fpr();
	_rregsFprCount = (int)regsFpr.size();
	_rregsFpr.resize( CBZ::ArchInfo.get_all_regs().size(), 0 );
	for ( i = 0; i < _rregsFprCount; ++i )
	{
		int num = regsFpr[i]->_id;

		// make room for it
		if ( num >= (int)_rregsFpr.size() )
			_rregsFpr.resize( num + 1, 0 );

		// setup info for this one
		rreg_info * rreg = new rreg_info;
		rreg->pInfo = regsFpr[i];
		rreg->reg = CBZ::ArchInfo.get_register( rreg->pInfo );

		// add this to the list in the right place
		_rregsFpr[ num ] = rreg;
	}

	// next is zero
	_lastRealRegFixed = -1;
	_lastRealRegFloat = -1;
}

inline bool 
dummy_reg_alloc::is_real_reg( const Register & reg ) 
{
	int num = reg.num();
	return ( num >= 0 && num < (int)CBZ::ArchInfo.get_all_regs().size() );
}

vreg_info* 
dummy_reg_alloc::get_vreg( const Register & vreg ) 
{
	int num = vreg.num();

	// sanity check
	if ( is_real_reg( vreg ) )
		return NULL;
	assert( num >= (int)CBZ::ArchInfo.get_all_regs().size() );

	// make sure array is big enough
	if ( num >= (int)_vregs.size() )
		_vregs.resize( num+1, 0 );

	// need to allocate an entry for it?
	vreg_info * info = _vregs[num];
	if ( ! info )
	{
		// add this to the list in the right place
		info = new vreg_info;
		info->reg = vreg;
		_vregs[num] = info;
	}

	return info;
}

rreg_info* 
dummy_reg_alloc::get_rreg( const Register & rreg ) 
{
	int num = rreg.num();

	// sanity checks
	if ( ! is_real_reg( rreg ) )
		return NULL;
	assert( num >= 0 && num < (int)CBZ::ArchInfo.get_all_regs().size() );

	// return its info
	switch ( rreg.type() )
	{
	case reg_gpr: return _rregsGpr[num];
	case reg_fpr: return _rregsFpr[num];
	default: assert( false ); return NULL;
	}
}


void 
dummy_reg_alloc::allocate(procNode *proc) 
{
	// setup real registers
	setup_rregs();

	//
	// traverse all of this guy's instructions and replace virtual registers
	//	with real registers.
	//
	// we are assuming that all virtual registers are defined once and used
	//	a small number of times, all very close to their definition point.  
	//	if these assumptions fail, we will fail to allocate.
	//

	instruction_list & insts = proc->instructions();
	instruction_list_p it = insts.begin();
	for ( ; it != insts.end(); ++it )
	{
		LirInst & inst = **it;

		// all uses should already have registers
		if ( inst.has_opnd1() && ! fix_allocated_virtual( inst.opnd1, inst ) )
			return;
		if ( inst.has_opnd2() && ! fix_allocated_virtual( inst.opnd2._reg, inst ) )
			return;
		if ( inst.has_base() && ! fix_allocated_virtual( inst.memBase, inst ) )
			return;
		if ( inst.has_offset() && ! fix_allocated_virtual( inst.memOffset._reg, inst ) )
			return;

		// allocate register for def
		if ( inst.has_dest() && ! allocate_real_for_virtual( inst.dest, inst.dest_contents, inst ) )
			return;
	}
}

bool 
dummy_reg_alloc::fix_allocated_virtual( Register & reg, const LirInst & inst ) 
{
	// make sure we need to fixup
	if ( is_real_reg( reg ) )
		return true;

	// get info for this guy - make sure we have a real register for it
	vreg_info * info = get_vreg( reg );
	if ( ! info->pRealReg || info->reclaimed )
	{
		// why is it gone?
		if ( info->reclaimed )
		{
			// it was defined but we alraady had to reuse the real register for something
			//	else, so the value is gone
			cerr << "Register allocation error:" << endl <<
				"Instruction: " << endl << 
				"    " << inst << endl << 
				"uses virtual register whose real register " << endl <<
				"    " << info->pRealReg->reg.to_string() <<
				"was reclaimed because of register pressure." << endl;
		}
		else
		{
			// it was never defined so we don't know what register it's in
			cerr << "Register allocation error:" << endl <<
				"Instruction: " << endl << 
				"    " << inst << endl << 
				"uses register that was never defined." << endl;
		}
		return false;
	}

	// change virtual to real
	reg = info->pRealReg->reg;
	return true;
}

bool 
dummy_reg_alloc::allocate_real_for_virtual( Register & reg, declNode * pDecl, const LirInst & inst ) 
{
	// if this already is real then we're good
	if ( is_real_reg( reg ) )
		return true;

	// get info for it
	vreg_info * vinfo = get_vreg( reg );
	if ( vinfo->pRealReg )
	{
		// this already has a real register?  what the heck?
		cerr << "Register allocation error:" << endl <<
			"Virtual register defined by instruction: " << endl << 
			"    " << inst << endl << 
			"already has a real register...?" << endl;
		return false;
	}

	assert( pDecl > DATA_CONTENTS__SPECIAL );

	// what kind of thing is this?
	lir_var_type vt = LirVt::getVarType( pDecl );
	bool isfloat = (vt == vt_float || vt == vt_double);
	if ( (! isfloat && _rregsGprCount < 1) ||
			(isfloat && _rregsFprCount < 1) )
		CBZFAIL(("Can't allocate register for var type %s because architecture has no "
			"registers for data of this type.", LirVt::getVarTypeString( vt )));

	// find the next non-NULL one in the list
	int & last = isfloat ? _lastRealRegFloat : _lastRealRegFixed;
	vector<rreg_info*> & regs = isfloat ? _rregsFpr : _rregsGpr;
	int next = last;
	do 
	{
		next++;
		if ( next >= (int)regs.size() )
			next = 0;
	} while ( regs[next] == 0 && next != last );
	assert( next != last );
	last = next;

	// allocate this one (reclaim old register if necessary)
	rreg_info * rinfo = regs[next];
	if ( rinfo->pVirtualReg )
		rinfo->pVirtualReg->reclaimed = true;
	rinfo->pVirtualReg = vinfo;
	vinfo->pRealReg = rinfo;
	reg = rinfo->reg;

	return true;
}

dummy_reg_alloc_walker::dummy_reg_alloc_walker() : Walker( Preorder, NodeOnly )
{
}

void dummy_reg_alloc_walker::at_unit(unitNode *the_unit, Order ord)
{
  // process all defs
  const def_list & defs = the_unit->defs();
  def_list::const_iterator it = defs.begin();
  while ( it != defs.end() )
    (*(it++))->walk( *this );
}

void dummy_reg_alloc_walker::at_proc(procNode *the_proc, Order ord)
{
  // allocate regs for this guy
  dummy_reg_alloc dra;
  dra.allocate( the_proc );
}
