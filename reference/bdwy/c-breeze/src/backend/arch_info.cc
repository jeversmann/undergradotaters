
#include "c_breeze.h"
#include "arch_info.h"
#include "arch_info_parser.h"

// because I'm lazy
typedef arch_info::register_info		register_info;
typedef arch_info::register_info_list	register_info_list;

// these are needed so that we can parse Kill list in Lir2Asm record
register_info arch_info::pseudo_reg_dest( (unsigned int)-2, "dest" );
register_info arch_info::pseudo_reg_src1( (unsigned int)-4, "src1" );
register_info arch_info::pseudo_reg_src2( (unsigned int)-5, "src2" );


// for handling errors when we parse the desc
#define PARSE_ERROR( format ) \
	CBZ::Fail( __FILE__, __LINE__, \
		cbz_util::string_format( "error parsing architecture spec (near line %d): ", _currentLine ) ); \

arch_info::arch_info() 
{
	// internal initial values
	_nextRegId = 0;

	// init our state
	reset_info();
}

arch_info::~arch_info() 
{
}

void 
arch_info::reset_info() 
{
	// we are no longer valid
	_valid = false;

	// kill everything
	_asmLineComment.clear();
	_asmRegPrefixAdd.clear();
	_asmRegPrefixRemove.clear();
	_asmRegPrefixRemoveSplit.clear();
	_asmConstPrefix.clear();
	_regsAll.clear();
	_regMap.clear();
	_regsGpr.clear();
	_regsFpr.clear();
	_regSp = NULL;
	_regFp = NULL;
	_regsParamFixed.clear();
	_regsParamFloat.clear();
	_regRetvalFixed = NULL;
	_regRetvalFloat = NULL;
	_regDataTypeGpr = vt_unknown;
	_regDataTypeFpr = vt_unknown;
	_dataSizeLong = 8;
	_dataSizeInt = 4;
	_dataSizeShort= 2;
	_dataSizeDouble = 8;
	_dataSizeFloat = 4;
	_dataSizePtr = 4;
	_maxDataAlign = (unsigned int)-1;
	_dataAlignChar = 1;
	_dataAlignShort = 2;
	_dataAlignInt = 4;
	_dataAlignLong = 8;
	_dataAlignFloat = 4;
	_dataAlignDouble = 8;
	_dataAlignPtr = 4;
	_stackFrameMinSize = 0;
	_stackExtraTop = 0;
	_stackExtraBottom = 0;
	_stackAlign = 0;
	_stackFormalsOffset = 0;
	_emulate3Address = false;
	_Lir2Asm_records.clear();
	_Lir2Asm_mnemonicLookup.clear();
}

const char * 
arch_info::get_asm_line_comment() const
{
	return _asmLineComment.c_str();
}

const char * 
arch_info::get_asm_const_prefix() const
{
	return _asmConstPrefix.c_str();
}

const register_info_list & 
arch_info::get_all_regs() const
{
	return _regsAll;
}

const register_info_list & 
arch_info::get_regs_gpr() const
{
	return _regsGpr;
}

const register_info_list & 
arch_info::get_regs_fpr() const
{
	return _regsFpr;
}

const register_info* 
arch_info::get_reg_sp() const
{
	return _regSp;
}

const register_info* 
arch_info::get_reg_fp() const
{
	return _regFp;
}

const register_info_list & 
arch_info::get_regs_param_fixed() const
{
	return _regsParamFixed;
}

const register_info_list & 
arch_info::get_regs_param_float() const
{
	return _regsParamFloat;
}

const register_info* 
arch_info::get_reg_retval_fixed() const
{
	return _regRetvalFixed;
}

const register_info* 
arch_info::get_reg_retval_float() const
{
	return _regRetvalFloat;
}

const register_info_list & 
arch_info::get_regs_caller_save() const
{
	return _regsCallerSave;
}

const register_info_list & 
arch_info::get_regs_callee_save() const
{
	return _regsCalleeSave;
}

lir_var_type 
arch_info::get_reg_data_type_gpr() const
{
	return _regDataTypeGpr;
}

lir_var_type 
arch_info::get_reg_data_type_fpr() const
{
	return _regDataTypeFpr;
}

bool 
arch_info::get_reg_name( const Register & reg, string & name, bool wantPrefixAdd, bool wantPrefixRemove ) 
{
	// check bounds first
	int num = reg.num();
	if ( num < 0 || num >= (int)_regsAll.size() )
		return false;

	// return its name, after prefix munging
	name = _regsAll[num]->_name;
	if ( wantPrefixAdd )
		name = _asmRegPrefixAdd + name;
	if ( wantPrefixRemove )
		for ( size_t i = 0; i < _asmRegPrefixRemoveSplit.size(); ++i )
			if ( name.find( _asmRegPrefixRemoveSplit[i] ) == 0 )
				name.erase( 0, _asmRegPrefixRemoveSplit[i].size() );
	return true;
}

bool 
arch_info::get_reg_by_index( int realRegIndex, Register & reg )
{
	// check bounds first
	if ( realRegIndex < 0 || realRegIndex >= (int)_regsAll.size() )
		return false;

	// just grab it by pointer
	reg = get_register( _regsAll[realRegIndex] );
	return true;
}

unsigned int 
arch_info::get_data_size( lir_var_type vt ) const
{
	switch ( vt )
	{
	case vt_char:	case vt_uchar:	return 1;
	case vt_short:	case vt_ushort:	return get_data_size_short();
	case vt_int:	case vt_uint:	return get_data_size_int();
	case vt_long:	case vt_ulong:	return get_data_size_long();
	case vt_float:					return get_data_size_float();
	case vt_double:					return get_data_size_double();
	case vt_pointer:				return get_data_size_ptr();

	default:
		// we have no idea how big this is
		CBZFAIL(( "ERROR: can't determine data size for low-level type %d.", vt ));
		assert( false );
		return 0;
	}
}

unsigned int
arch_info::get_max_data_align() const
{
	// did we already compute it?
	if ( _maxDataAlign != -1 )
		return _maxDataAlign;

	// compute it
	_maxDataAlign = _dataAlignChar;
	if ( _maxDataAlign < _dataAlignShort )
		_maxDataAlign = _dataAlignShort;
	if ( _maxDataAlign < _dataAlignInt )
		_maxDataAlign = _dataAlignInt;
	if ( _maxDataAlign < _dataAlignLong )
		_maxDataAlign = _dataAlignLong;
	if ( _maxDataAlign < _dataAlignFloat )
		_maxDataAlign = _dataAlignFloat;
	if ( _maxDataAlign < _dataAlignDouble )
		_maxDataAlign = _dataAlignDouble;
	if ( _maxDataAlign < _dataAlignPtr )
		_maxDataAlign = _dataAlignPtr;

	return _maxDataAlign;
}

unsigned int 
arch_info::get_data_align( lir_var_type vt ) const
{
	switch ( vt )
	{
	case vt_char:		case vt_uchar:	return get_data_align_char();
	case vt_short:		case vt_ushort:	return get_data_align_short();
	case vt_int:		case vt_uint:	return get_data_align_int();
	case vt_long:		case vt_ulong:	return get_data_align_long();
	case vt_float:						return get_data_align_float();
	case vt_double:						return get_data_align_double();
	case vt_pointer:					return get_data_align_ptr();
	case vt_aggr:						return get_max_data_align();

	default:
		// we have no idea how to align this
		CBZFAIL(( "ERROR: can't determine alignment for low-level type %d.", vt ));
		assert( false );
		return 0;
	}
}

bool 
arch_info::get_code_for_instruction( const LirInst * pInst, vector<string> & lines ) 
{
	// find the Lir2Asm for this instruction
	Lir2Asm * pLir2Asm = NULL;
	if ( ! get_Lir2Asm_for_instruction( pInst, &pLir2Asm ) || ! pLir2Asm )
		return false;

	// copy the lines
	cbz_util::vector_copy( pLir2Asm->_codeTemplate, lines );

	// make replacements
	make_template_replacements( pInst, pLir2Asm, lines );

	// do formatting - labels are left-justified, all others indented from left edge
	if ( pInst->instruction != mn_Label )
	{
		int i, sz = (int)lines.size();
		for ( i = 0; i < sz; ++i )
		{
			// add some space if it is not a label (assume anything with colon char is label)
			if ( lines[i].find( ':' ) == -1 )
				lines[i].insert( 0, 4, ' ' );
		}
	}

	return true;
}

bool 
arch_info::get_instruction_kill_regs( const LirInst * pInst, register_info_list & killRegs ) 
{
	// find the Lir2Asm for this instruction
	Lir2Asm * pLir2Asm = NULL;
	if ( ! get_Lir2Asm_for_instruction( pInst, &pLir2Asm ) || ! pLir2Asm )
		return false;

	// copy the register info
	cbz_util::vector_copy( pLir2Asm->_killRegs, killRegs );

	return true;
}

bool 
arch_info::types_need_conversion( lir_var_type vtsrc, lir_var_type vtdest ) 
{
	// get a lir2asm for it - if we have none, pretend we need conversion so we'll fail later
	Lir2Asm * l2a = NULL;
	if ( ! get_Lir2Asm_for_instruction( mn_ConvertType, vtsrc, vtdest, false, &l2a ) || ! l2a )
		return true;

	// are there any instructions for it?  if so, we need conversion.
	return (l2a->_codeTemplate.size() != 0);
}

bool 
arch_info::instruction_supports_immediate( mnemonic instruction, lir_var_type vt, const constant & c ) 
{
	// no strings as immediates
	if ( c.is_str() )
		return false;

	// get a lir2asm for it - if we have one, it's supported
	Lir2Asm * l2a = NULL;
	return ( get_Lir2Asm_for_instruction( instruction, vt, vt_unknown, true, &l2a ) && l2a != NULL );
}

bool
arch_info::find_register_info( const char * name, register_info *& regInfoFound ) const
{
	// find this one in our set of registers
	register_info_map::const_iterator it = _regMap.find( name );
	if ( it == _regMap.end() )
		return false;

	// give them what we found
	regInfoFound = (*it).second;
	return true;
}

Register 
arch_info::get_register( const register_info * pInfo ) 
{
	// the value we'll return
	Register ret;
	if ( ! pInfo )
		return ret;

	// copy id and type
	ret.num( pInfo->_id );
	ret.type( pInfo->_type );
	return ret;
}

bool
arch_info::get_Lir2Asm_for_instruction( const LirInst * pInst, Lir2Asm ** ppLir2Asm ) 
{
	// use other version
	return get_Lir2Asm_for_instruction( pInst->instruction, pInst->vtPrimary, pInst->vtConvert, pInst->opnd2._is_const, ppLir2Asm );
}

bool 
arch_info::get_Lir2Asm_for_instruction( mnemonic inst, lir_var_type vtsrc, lir_var_type vtdest, bool isImmed, Lir2Asm ** ppLir2Asm ) 
{
	// find list of possibles for this guy
	map_menmonic_to_record_set::const_iterator it = _Lir2Asm_mnemonicLookup.find( inst );
	if ( it == _Lir2Asm_mnemonicLookup.end() )
		return false;

	// find actual match
	const vector<int> & possibles = it->second;
	int sz = (int)possibles.size();
	for ( int i = 0; i < sz; ++i )
	{
		// sanity check - the values in possibles should be array indices
		assert( possibles[i] >= 0 && possibles[i] < (int)_Lir2Asm_records.size() );

		Lir2Asm & l2a = _Lir2Asm_records[ possibles[i] ];

		// this thing had better handle this instruction
		assert( -1 != cbz_util::vector_find( l2a._lirInstTypes, inst ) );

		// does this template specify an immediate?
		if ( l2a._immed != Lir2Asm::Immed_NA )
		{
			// make sure immediate-ness is matched
			if ( l2a._immed == Lir2Asm::Immed_No && isImmed )
				continue;
			else if ( l2a._immed == Lir2Asm::Immed_Yes && ! isImmed )
				continue;
		}

		// data types match? (if this thing listed no data types, it matches any type)
		if ( vtsrc != vt_unknown && l2a._dataTypes.size() > 0 && -1 == cbz_util::vector_find( l2a._dataTypes, vtsrc ) )
			continue;

		// conversion types match? (conversion types must be explicitly listed)
		if ( vtdest != vt_unknown && -1 == cbz_util::vector_find( l2a._convertToTypes, vtdest ) )
			continue;

		// this one matches
		*ppLir2Asm = &l2a;
		return true;
	}

	return false;
}

void 
arch_info::make_template_replacements( const LirInst * pInst, Lir2Asm * pLir2Asm, code_template & outputTemplate ) 
{
	// generate strings for each various token
	string dest, op1, op2, base, offset, target, stacksize, str;
	if ( pInst->has_dest() )
		dest = pInst->dest.to_string();
	if ( pInst->has_opnd1() )
		op1 = pInst->opnd1.to_string();
	if ( pInst->has_opnd2( false ) )
		op2 = pInst->opnd2.to_string( false, true );
	if ( pInst->has_base() )
		base = pInst->memBase.to_string();
	if ( pInst->has_offset( false ) )
		offset = pInst->memOffset.to_string( false, false );
	if ( pInst->nodeExtra && pInst->nodeExtra->typ() == Proc )
		stacksize = cbz_util::string_format( "%d", ((procNode*)(pInst->nodeExtra))->stack_frame_size() );

	// always read a target and static data
	target = pInst->target;
	str = pInst->dataString;

	// make replacements in template
	int i, sz = (int)outputTemplate.size();
	for ( i = 0; i < sz; ++i )
	{
		// fix various parts of template
		cbz_util::string_replace( outputTemplate[i], "$dest", dest );
		cbz_util::string_replace( outputTemplate[i], "$opnd1", op1 );
		cbz_util::string_replace( outputTemplate[i], "$opnd2", op2 );
		cbz_util::string_replace( outputTemplate[i], "$base", base );
		cbz_util::string_replace( outputTemplate[i], "$offset", offset );
		cbz_util::string_replace( outputTemplate[i], "$target", target );
		cbz_util::string_replace( outputTemplate[i], "$stacksize", stacksize );
		cbz_util::string_replace( outputTemplate[i], "$string", str );

		// fixup any register names that were formatted @name
		int reg, szregs = (int)_regsAll.size();
		for ( reg = 0; reg < szregs; ++reg )
		{
			string handyName = '@' + _regsAll[reg]->_name;
			string realName;
			get_reg_name( *_regsAll[reg], realName, true, true );
			cbz_util::string_replace( outputTemplate[i], handyName, realName );
		}
	}


}

bool 
arch_info::load_arch_info( const char * pFileName ) 
{
	int i, sz;

	// reset ourself first
	reset_info();

	// let our parser do the work
	arch_info_parser parser;
	if ( ! parser.parse( pFileName, this ) )
		return false;

	/////////////////////////////
	// setup register stuff

	// setup type info for registers
	_regSp->_type = reg_stack_ptr;
	_regFp->_type = reg_frame_ptr;
	sz = (int)_regsGpr.size();
	for ( i = 0; i < sz; ++i )
		_regsGpr[i]->_type = reg_gpr;
	sz = (int)_regsFpr.size();
	for ( i = 0; i < sz; ++i )
		_regsFpr[i]->_type = reg_fpr;

	////////////////////
	// sanity checks

#define ARCHWARN( expr ) \
	cout << cbz_util::string_format expr << endl;

	if ( _regsAll.size() < 1 )					ARCHWARN(( "WARNING: architecture does not appear to have any registers!!" ));
	if ( _regsGpr.size() < 1 )					ARCHWARN(( "WARNING: architecture does not appear to have any integer registers!!" ));
	if ( ! _regSp || ! _regSp->is_valid() )		ARCHWARN(( "WARNING: architecture does not appear to have a stack pointer register!!" ));
	if ( ! _regFp || ! _regFp->is_valid() )		ARCHWARN(( "WARNING: architecture does not appear to have a frame pointer register!!" ));

	if ( _regDataTypeGpr == vt_unknown )	ARCHWARN(( "WARNING: invalid data type specified for general-purpose registers!!" ));
	if ( _regDataTypeFpr == vt_unknown )	ARCHWARN(( "WARNING: invalid data type specified for floating-point registers!!" ));

	if ( _dataSizeShort <= 0 )		ARCHWARN(( "WARNING: size of short integer (%d) appears to be ridiculous!!",	_dataSizeShort ));
	if ( _dataSizeInt <= 0 )		ARCHWARN(( "WARNING: size of integer (%d) appears to be ridiculous!!",		_dataSizeInt ));
	if ( _dataSizeLong <= 0 )		ARCHWARN(( "WARNING: size of long integer (%d) appears to be ridiculous!!", 	_dataSizeLong ));
	if ( _dataSizeFloat <= 0 )		ARCHWARN(( "WARNING: size of float (%d) appears to be ridiculous!!",			_dataSizeFloat ));
	if ( _dataSizeDouble <= 0 )		ARCHWARN(( "WARNING: size of double float (%d) appears to be ridiculous!!", 	_dataSizeDouble ));
	if ( _dataSizePtr <= 0 )		ARCHWARN(( "WARNING: size of pointer (%d) appears to be ridiculous!!",		_dataSizePtr ));

	// currently we don't handle the case that our data sizes and the target data sizes don't match
	if ( sizeof(short) != _dataSizeShort )	ARCHWARN(( "WARNING: compiler data size does not match target data size for type 'short'.  "
												"This is currently not handled properly by the backend." ));
	if ( sizeof(int) != _dataSizeInt )		ARCHWARN(( "WARNING: compiler data size does not match target data size for type 'int'.  "
												"This is currently not handled properly by the backend." ));
	if ( sizeof(long) != _dataSizeLong )	ARCHWARN(( "WARNING: compiler data size does not match target data size for type 'long'.  "
												"This is currently not handled properly by the backend." ));
	if ( sizeof(float) != _dataSizeFloat )	ARCHWARN(( "WARNING: compiler data size does not match target data size for type 'float'.  "
												"This is currently not handled properly by the backend." ));
	if ( sizeof(double) != _dataSizeDouble )	ARCHWARN(( "WARNING: compiler data size does not match target data size for type 'double'.  "
												"This is currently not handled properly by the backend." ));
	if ( sizeof(void*) != _dataSizePtr )	ARCHWARN(( "WARNING: compiler data size does not match target data size for type 'pointer'.  "
												"This is currently not handled properly by the backend." ));

	if ( _dataAlignChar <= 0 )		ARCHWARN(( "WARNING: alignment of character (%d) appears to be ridiculous!!",		_dataAlignChar ));
	if ( _dataAlignShort <= 0 )		ARCHWARN(( "WARNING: alignment of short integer (%d) appears to be ridiculous!!",	_dataAlignShort ));
	if ( _dataAlignInt <= 0 )		ARCHWARN(( "WARNING: alignment of integer (%d) appears to be ridiculous!!",			_dataAlignInt ));
	if ( _dataAlignLong <= 0 )		ARCHWARN(( "WARNING: alignment of long integer (%d) appears to be ridiculous!!",		_dataAlignLong ));
	if ( _dataAlignFloat <= 0 )		ARCHWARN(( "WARNING: alignment of float (%d) appears to be ridiculous!!",			_dataAlignFloat ));
	if ( _dataAlignDouble <= 0 )	ARCHWARN(( "WARNING: alignment of double float (%d) appears to be ridiculous!!",		_dataAlignDouble ));
	if ( _dataAlignPtr <= 0 )		ARCHWARN(( "WARNING: alignment of pointer (%d) appears to be ridiculous!!",			_dataAlignPtr ));

	if ( _stackFrameMinSize < 0 )	ARCHWARN(( "WARNING: stack frame minimum size (%d) appears to be ridiculous!!",	_stackFrameMinSize ));
	if ( _stackAlign < 0 )			ARCHWARN(( "WARNING: stack alignment value (%d) appears to be ridiculous!!",		_stackAlign ));

	if ( _stackFrameMinSize != 0 && _stackAlign != 0 && (_stackFrameMinSize % _stackAlign) != 0 )
									ARCHWARN(( "WARNING: stack frame minimum size is not a multiple of stack alignment!!" ));

	// make sure caller-save and callee-save are disjoint and that all regs are in one list or another
	register_info_list::iterator it = _regsAll.begin();
	for ( ; it != _regsAll.end(); ++it )
	{
		register_info * info = *it;
		if ( ! info )
			continue;

		// assume neither
		bool caller = false, callee = false;

		// see if it's a callee-save
		register_info_list::iterator calleeIt = _regsCalleeSave.begin();
		for ( ; calleeIt != _regsCalleeSave.end(); ++calleeIt )
			if ( info == *calleeIt )
			{
				callee = true;
				break;
			}

		// see if it's a caller-save
		register_info_list::iterator callerIt = _regsCallerSave.begin();
		for ( ; callerIt != _regsCallerSave.end(); ++callerIt )
			if ( info == *callerIt )
			{
				caller = true;
				break;
			}

		// it should be saved by someone (except fp, sp, retFixed, and retFloat)
		if ( ! caller && ! callee && info != _regFp && info != _regSp && info != _regRetvalFixed && info != _regRetvalFloat )
			ARCHWARN(( "WARNING: register %s is not caller-save or callee-save - is this the intent?", info->_name.c_str() ));

		// but not by both...
		if ( caller && callee )
			ARCHWARN(( "WARNING: register %s is both caller-save and callee-save - is this the intent?", info->_name.c_str() ));

		// fp and sp must be handled by arch template code - we do not save them for you
		if ( (caller || callee) && (info == _regFp || info == _regSp) )
			ARCHWARN(( "WARNING: register %s should be handled by architecture template because it is the frame/stack pointer.", info->_name.c_str() ));

		// retFixed and retFloat should probably be caller-save but for now they are neither
		if ( (caller || callee) && (info == _regRetvalFixed || info == _regRetvalFloat) )
			ARCHWARN(( "WARNING: return-value register %s is volatile and will not be saved even though marked as caller/callee save.", info->_name.c_str() ));
	}

#undef ARCHWARN

	////////////////////////////////////////////////
	// other stuff
	
	// split up prefix remove stuff - treat as comma-separated list
	if ( ! _asmRegPrefixRemove.empty() )
	{
		int len = _asmRegPrefixRemove.size() + 2;
		char * split = new char[ len ];
		strcpy( split, _asmRegPrefixRemove.c_str() );
		split[ len-1 ] = 0;
		char *p1 = split, *p2 = strchr(p1, ',');
		while ( (p2 = strchr(p1, ',')) )
		{
			// kill the comma and add what was before it to array
			*p2 = 0;
			_asmRegPrefixRemoveSplit.push_back( p1 );
			p1 = p2 + 1;
		}
		_asmRegPrefixRemoveSplit.push_back( p1 );
		delete [] split;
	}

	////////////////////////////////////////////////
	// show all of this to the user

	// title
	cout << endl;
	cout << "will compile for architecture: " << _archName.c_str() << endl;
	cout << endl;

	// we are valid now
	_valid = true;
	return true;
}
