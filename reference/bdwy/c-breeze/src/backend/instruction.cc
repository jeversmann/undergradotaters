// ------------- 
//  Chuck Tsen
// June 24, 2002
//  instruction.cc
// -------------
//
// ********************************************************
// *							  *
// * This is the implementation of instructions for the   *
// * class instruction, which keeps various information   *
// * about an instruction.				  *
// * 							  *
// * This information includes immediate size, register   *
// * bank of result register, result latency, and so on.  *
// *							  *
// * This implementation is similar to the cmcc compiler, *
// * developed at Carnegie Mellon University and	  *
// * in the paper,"Code Reuse in an Optimizing Compliler" *
// *							  *
// ********************************************************
#include "c_breeze.h"
#include "instruction.h"
#include <stdarg.h>

LirInst::LirInst()
{

	// initialize values
	instruction = mn_undefined;
	vtPrimary = vt_unknown;
	vtConvert = vt_unknown;
	targetInst = NULL;
	visited = false;

	// no contents yet
	opnd1_contents = NULL;
	opnd2_contents = NULL;
	dest_contents = NULL;
	memBase_contents = NULL;
	memOffset_contents = NULL;
	nodeExtra = NULL;
	memset( dataBinary, 0, sizeof(dataBinary) );

	// no block yet
	block = NULL;
}

LirInst::LirInst( mnemonic inst )
{
	// initialize values
	instruction = inst;
	vtPrimary = vt_unknown;
	vtConvert = vt_unknown;
	targetInst = NULL;
	visited = false;

	// no contents yet
	opnd1_contents = NULL;
	opnd2_contents = NULL;
	dest_contents = NULL;
	memBase_contents = NULL;
	memOffset_contents = NULL;
	nodeExtra = NULL;
	memset( dataBinary, 0, sizeof(dataBinary) );

	// no block yet
	block = NULL;
}

LirInst::~LirInst(){

}

// stringize an instruction
string LirInst::to_string() const
{
	InstructionInfo info = instruction_info[ instruction ];

	// make pretty format string for this thing
	string pretty( info._opndFormat );
	cbz_util::string_replace( pretty, "$dest", contents_str( dest_contents ) );
	cbz_util::string_replace( pretty, "$opnd1", contents_str( opnd1_contents ) );
	cbz_util::string_replace( pretty, "$opnd2", opnd2._is_const ? opnd2.to_string( false ) : contents_str( opnd2_contents ) );
	cbz_util::string_replace( pretty, "$base", contents_str( memBase_contents ) );
	cbz_util::string_replace( pretty, "$offset", memOffset._is_const ? memOffset.to_string( false ) : contents_str( memOffset_contents ) );
	cbz_util::string_replace( pretty, "$target", target );
	cbz_util::string_replace( pretty, "$ptype", LirVt::getVarTypeString( vtPrimary ) );
	cbz_util::string_replace( pretty, "$ctype", LirVt::getVarTypeString( vtConvert ) );

	// make ugly format string for this thing
	string ugly( info._opndFormat );
	cbz_util::string_replace( ugly, "$dest", dest.to_string() );
	cbz_util::string_replace( ugly, "$opnd1", opnd1.to_string() );
	cbz_util::string_replace( ugly, "$opnd2", opnd2.to_string( false ) );
	cbz_util::string_replace( ugly, "$base", memBase.to_string() );
	cbz_util::string_replace( ugly, "$offset", memOffset.to_string( false ) );
	cbz_util::string_replace( ugly, "$target", target );
	cbz_util::string_replace( ugly, "$ptype", LirVt::getVarTypeString( vtPrimary ) );
	cbz_util::string_replace( ugly, "$ctype", LirVt::getVarTypeString( vtConvert ) );

	// make the full string
	return cbz_util::string_format( "%-14s : %s", info._name, /*pretty.c_str(),*/ ugly.c_str() );
}

int LirInst::contents_compiler_temp_index = 1;

string LirInst::contents_str( declNode * pDecl )
{
	// any contents?
	if ( ! pDecl )
		return "??";

	// special?
	if ( pDecl == DATA_CONTENTS_STACKP )
		return "sp";
	else if ( pDecl == DATA_CONTENTS_FRAMEP )
		return "fp";
	else if ( pDecl == DATA_CONTENTS_REG_IN )
		return "in";
	else if ( pDecl == DATA_CONTENTS_REG_OUT )
		return "out";
	else if ( pDecl == DATA_CONTENTS_RETVAL )
		return "retval";

	// get its name
	return pDecl->name();
}

// handy print operator for LirInst
ostream & operator << ( ostream& os, const LirInst & inst )
{
	// just stringize it
	os << inst.to_string();
	return os;
}

bool LirInst::has_dest( bool declDefines ) const
{
	// does it think it has a destination?
	if ( instruction_info[ instruction ]._hasDest )
	{
		// make sure it has a real destination
		return dest.isValid();
	}

	// is it a decl that lives in a register?
	if ( declDefines && instruction == mn_DeclLocal && 
			((declNode*)nodeExtra)->storage_location()._type == declNode::Storage_location::storageloc_register )
		return true;

	return false;
}

bool LirInst::has_opnd1() const
{
	return instruction_info[ instruction ]._hasOpnd1;
}

bool LirInst::has_opnd2( bool regOnly ) const
{
	return (instruction_info[ instruction ]._hasOpnd2 && (! regOnly || !opnd2._is_const));
}

bool LirInst::has_base() const
{
	return instruction_info[ instruction ]._hasBase;
}

bool LirInst::has_offset( bool regOnly ) const
{
	return (instruction_info[ instruction ]._hasOffset && (! regOnly || !memOffset._is_const));
}

bool LirInst::has_target() const
{
	return instruction_info[ instruction ]._hasTarget;
}

bool LirInst::should_emit() const
{
	return instruction_info[ instruction ]._shouldEmit;
}

Register LirInst::get_dest()
{
	// do we have a real destination?
	if ( dest.isValid() )
		return dest;

	// are we a decl that lives in a register?  if so we will pretend this instruction
	//	defines that register so that we are sure to have at least one definition of it.
	if ( instruction == mn_DeclLocal && ((declNode*)nodeExtra)->storage_location()._type == declNode::Storage_location::storageloc_register )
		return ((declNode*)nodeExtra)->storage_location()._register;

	// return junk register
	return Register();
}

// instruction information
InstructionInfo instruction_info[ mn__count ];

// handy thingy that sets up instruction info
struct InstructionInfo_Setup
{
	InstructionInfo_Setup()
	{
#define INST( mnemonic, format, hd, ho1, ho2, hb, hof, ht, emit ) \
	instruction_info[ mn_##mnemonic ] = InstructionInfo( #mnemonic, format, hd, ho1, ho2, hb, hof, ht, emit );

/*	  mnemonic				pretty-printer format string			dest	opnd1	opnd2	base	offset	target  emit */
/*    --------------------- --------------------------------------- ------- ------- ------- ------- ------- ------- ---- */
INST( NOP,					"",										false,	false,	false,	false,	false,	false,	true )
INST( ConvertType,			"$dest {$ctype} <- $opnd1 {$ptype}",	true,	true,	false,	false,	false,	false,	true )
INST( Load,					"$dest <- $base[$offset]",				true,	false,	false,	true,	true,	false,	true )
INST( LoadImmediate,		"$dest <- $opnd2",						true,	false,	true,	false,	false,	false,	true )
INST( Store,				"$base[$offset] <- $opnd1",				false,	true,	false,	true,	true,	false,	true )
INST( GetEffectiveAddr,		"$dest <- addr($base[$offset])",		true,	false,	false,	true,	true,	false,	true )
INST( LoadStatic,			"$dest <- static($target)",				true,	false,	false,	false,	false,	false,	true )
INST( Move,					"$dest <- $opnd1",						true,	true,	false,	false,	false,	false,	true )
INST( Add, 					"$dest <- ($opnd1 + $opnd2)",			true,	true,	true,	false,	false,	false,	true )
INST( Sub, 					"$dest <- ($opnd1 - $opnd2)",			true,	true,	true,	false,	false,	false,	true )
INST( Mul, 					"$dest <- ($opnd1 * $opnd2)",			true,	true,	true,	false,	false,	false,	true )
INST( Div, 					"$dest <- ($opnd1 / $opnd2)",			true,	true,	true,	false,	false,	false,	true )
INST( Mod, 					"$dest <- ($opnd1 $ $opnd2)",			true,	true,	true,	false,	false,	false,	true )
INST( Neg, 					"$dest <- (-1 * $opnd1)",				true,	true,	false,	false,	false,	false,	true )
INST( BitwiseOR,			"$dest <- ($opnd1 | $opnd2)",			true,	true,	true,	false,	false,	false,	true )
INST( BitwiseAND, 			"$dest <- ($opnd1 & $opnd2)",			true,	true,	true,	false,	false,	false,	true )
INST( BitwiseXOR, 			"$dest <- ($opnd1 ^ $opnd2)",			true,	true,	true,	false,	false,	false,	true )
INST( BitwiseNOT, 			"$dest <- (! $opnd1)",					true,	true,	false,	false,	false,	false,	true )
INST( BitwiseShiftLeft,		"$dest <- ($opnd1 << $opnd2)",			true,	true,	true,	false,	false,	false,	true )
INST( BitwiseShiftRight,	"$dest <- ($opnd1 >> $opnd2)",			true,	true,	true,	false,	false,	false,	true )
INST( BitwiseRotateRight,	"$dest <- (<$opnd1 << $opnd2>)",		true,	true,	true,	false,	false,	false,	true )
INST( BitwiseRotateLeft,	"$dest <- (<$opnd1 >> $opnd2>)",		true,	true,	true,	false,	false,	false,	true )
INST( Compare,				"cmp $opnd1 $opnd2",					false,	true,	true,	false,	false,	false,	true )
INST( BranchEQ,				"(op1 == op2) ? $target",				false,	false,	false,	false,	false,	true,	true )
INST( BranchNE,				"(op1 != op2) ? $target",				false,	false,	false,	false,	false,	true,	true )
INST( BranchLT,				"(op1 <  op2) ? $target",				false,	false,	false,	false,	false,	true,	true )
INST( BranchLE,				"(op1 <= op2) ? $target",				false,	false,	false,	false,	false,	true,	true )
INST( BranchGT,				"(op1 >  op2) ? $target",				false,	false,	false,	false,	false,	true,	true )
INST( BranchGE,				"(op1 >= op2) ? $target",				false,	false,	false,	false,	false,	true,	true )
INST( Jmp,					"goto $target:",						false,	false,	false,	false,	false,	true,	true )
INST( Label,				"$target:",								false,	false,	false,	false,	false,	true,	true )
INST( Call,					"call '$target'",						true,	false,	false,	false,	false,	true,	true )
INST( Return,				"ret",									false,	false,	false,	false,	false,	false,	true )
INST( CallPre,				".precall",								false,	false,	false,	false,	false,	false,	false )
INST( BeginProc,			".begin $target",						false,	false,	false,	false,	false,	false,	true )
INST( EndProc,				".end $target",							false,	false,	false,	false,	false,	false,	true )
INST( BeginUnit,			".beginunit",							false,	false,	false,	false,	false,	false,	true )
INST( EndUnit,				".endunit",								false,	false,	false,	false,	false,	false,	true )
INST( StaticDataString,		".data string",							false,	false,	false,	false,	false,	false,	true )
INST( StaticDataBinary,		".data binary",							false,	false,	false,	false,	false,	false,	true )
INST( DeclLocal,			"var $target",							false,	false,	false,	false,	false,	false,	false )

#undef INST
	}
};

// instantiate one of these at global scope so instruction info will get setup
InstructionInfo_Setup setup;

const instruction_list & LirBlock::insts() const
{
	return _instructions;
}

void LirBlock::add_inst_before( LirInst * newInst, LirInst * otherInst )
{
	// fix blocks
	assert( otherInst->block == this );
	newInst->block = this;

	// find the one we're putting it before
	instruction_list_p it;
	for ( it = _instructions.begin(); it != _instructions.end(); ++it )
		if ( *it == otherInst )
		{
			_instructions.insert( it, newInst );
			return;
		}

	// if we get here we did not find otherInst in our list - that is bad
	assert( false );
}

void LirBlock::add_inst_after( LirInst * newInst, LirInst * otherInst )
{
	// fix blocks
	assert( otherInst->block == this );
	newInst->block = this;

	// find the one we're putting it after
	instruction_list_p it;
	for ( it = _instructions.begin(); it != _instructions.end(); ++it )
		if ( *it == otherInst )
		{
			_instructions.insert( ++it, newInst );
			return;
		}

	// if we get here we did not find otherInst in our list - that is bad
	assert( false );
}

void LirBlock::add_inst( LirInst * inst )
{
	// add to end of list
	inst->block = this;
	_instructions.push_back( inst );
}

void LirBlock::remove_inst( LirInst * inst )
{
	// sanity check
	assert( inst->block == this );

	// find it and remove it
	inst->block = NULL;
	instruction_list_p it = _instructions.begin();
	for ( ; it != _instructions.end(); ++it )
		if ( *it == inst )
		{
			_instructions.erase( it );
			return;
		}
}

void LirBlock::reset_visited()
{
	// to avoid infinite recursion
	if ( ! _visited )
		return;

	// reset our flag
	_visited = false;

	// reset all successors
	LirBlockSet_p it;
	for ( it = _succs.begin(); it != _succs.end(); ++it )
		(*it)->reset_visited();
}



