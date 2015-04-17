// ------------- 
//  Chuck Tsen
// June 24, 2002
//  instruction.h
// -------------
//
// ********************************************************
// *							  *
// * This is the base class of instructions, which keeps  *
// * bookkeeping information about a particular 	  *
// * instructional information such as size of immediate, *
// * register bank of result register, commutability, the *
// * result register, the result latency, and so on.	  *
// * 							  *
// * This implementation is similar to the cmcc compiler, *
// * developed at Carnegie Mellon University and	  *
// * in the paper,"Code Reuse in an Optimizing Compliler" *
// *							  *
// ********************************************************

#ifndef __INSTRUCTION_H__
#define __INSTRUCTION_H__

#include <c_breeze.h>
#include "register.h"
#include "lir_var_type.h"

// pseudo-RISC instruction mnemonics
enum mnemonic {
	mn_undefined = 0,

	/* data manipulation instructions */

	mn_ConvertType,			// convert the contents of $opnd1 from $ptype to $ctype and place result in $dest
	mn_Load,				// read a value from memory ($base+$offset) into $dest register
	mn_LoadImmediate,		// read immediate value $opnd2 into $dest register
	mn_Store,				// store the value from $opnd1 to memory ($base+$offset)
	mn_GetEffectiveAddr,	// compute effective address of ($base+$offset) and store in $dest
	mn_Move,				// move register $opnd1 into register $dest
	mn_LoadStatic,			// loads the compile-time static data with label $target into register $dest
							//	(used for loading things like string constants or floating-point values)

	/* arithmetic instructions */

	mn_Add,					// store ($opnd1 + $opnd2) in $dest
	mn_Sub,					// store ($opnd1 - $opnd2) in $dest
	mn_Mul,					// store ($opnd1 * $opnd2) in $dest
	mn_Div,					// store ($opnd1 / $opnd2) in $dest
	mn_Mod,					// store ($opnd1 % $opnd2) in $dest
	mn_Neg,					// store (-1 * $opnd1) in $dest
	
	/* bitwise manipulation instructions */

	mn_BitwiseOR,			// store ($opnd1 | $opnd2) in $dest
	mn_BitwiseAND,			// store ($opnd1 & $opnd2) in $dest
	mn_BitwiseXOR,			// store ($opnd1 ^ $opnd2) in $dest
	mn_BitwiseNOT,			// store (! $opnd1) in $dest
	mn_BitwiseShiftLeft,	// store ($opnd1 << $opnd2) in $dest
	mn_BitwiseShiftRight,	// store ($opnd1 >> $opnd2) in $dest
	mn_BitwiseRotateRight,	// store ($opnd1 rotated left by $opnd2 bits) in $dest
	mn_BitwiseRotateLeft,	// store ($opnd1 rotated right by $opnd2 bits) in $dest

	/* flow control instructions */
	//
	// NOTE: It is guaranteed that all conditional branch instructions will be 
	//	immediately preceded by a compare instruction.
	//

	mn_Compare,				// compare $opnd1, $opnd2, store result in arch-specific manner
	mn_BranchEQ,			// branch to $target if previous comparison yielded ==
	mn_BranchNE,			// branch to $target if previous comparison yielded !=
	mn_BranchLT,			// branch to $target if previous comparison yielded <
	mn_BranchLE,			// branch to $target if previous comparison yielded <=
	mn_BranchGT,			// branch to $target if previous comparison yielded >
	mn_BranchGE,			// branch to $target if previous comparison yielded >=
	mn_Jmp,					// branch to $target
	mn_Label,				// declare label $target

	/* procedure call stuff */

	mn_Call,				// call function $target
	mn_Return,				// return from procedure $target with stack size $stacksize
	mn_CallPre,				// for handling caller-save registers - instruction inserted before call arguments are setup

	/* procedure control instructions */
	// NOTE: architecture code template is responsible for setting up and tearing down 
	//	the stack frame, including saving/restoring old stack and frame pointers.  stack
	//	frame is usually setup by BeginProc and torn down by Return.

	mn_BeginProc,			// start of procedure named $target, with stack size $stacksize.
	mn_EndProc,				// end of procedure named $target

	/* translation-unit control instructions */

	mn_BeginUnit,			// beginning of translation unit
	mn_EndUnit,				// end of translation unit

	/* static data stuff */
	//
	// for each of these things the template should emit the proper macros for encoding the data, such
	//	as '.ascii' for strings and '.long' for a 32-bit value.  for binary values, the data bytes are 
	//	encoded as strings in $b0 through $b7, where $b0 is defined to be the most significant byte of
	//	the binary data.
	//
	// the template should not emit a label for the static data - this will be handled automatically.
	//

	mn_StaticDataString,	// declaration of static string data $string (formatted with no quotes)
	mn_StaticDataBinary,	// declaration of static binary data with data type $type and data bytes $b0 through $b7

	/* miscellaneous instructions */

	mn_DeclLocal,			// declaration of block-local variable $target (virtual instruction, not emitted)
	mn_NOP,					// empty operation

	// placeholder - must always be last in enumeration
	mn__count
};

// information about instructions
struct InstructionInfo {

	InstructionInfo() {}
	InstructionInfo( const char * name, const char * opndFormat, bool hasDest, bool hasOp1, 
		bool hasOp2, bool hasBase, bool hasOffset, bool hasTarget, bool shouldEmit ) :
		_name( name ), _opndFormat( opndFormat ), 
		_hasDest( hasDest ), 
		_hasOpnd1( hasOp1 ), 
		_hasOpnd2( hasOp2 ), 
		_hasBase( hasBase ), 
		_hasOffset( hasOffset ),
		_hasTarget( hasTarget ),
		_shouldEmit( shouldEmit )
	{
	}

	// friendly name
	const char * _name;

	// operand format for pretty-printing an instruction
	//
	// format fields
	//
	// %dest - destination
	// %opnd1 - operand1
	// %opnd2 - operand2
	// %base - load-store base register
	// %offset - load-store offset register/constant
	// %target - branch target or label name (for Decl instruction, variable name)
	// %ptype - primary data type (for type conversions, the source type)
	// %ctype - data type to convert to (used for type conversions)
	//
	const char * _opndFormat;

	// whether or not the instruction has various things
	bool _hasDest;
	bool _hasOpnd1;
	bool _hasOpnd2;
	bool _hasBase;
	bool _hasOffset;
	bool _hasTarget;

	// whether or not the instruction should be emitted
	bool _shouldEmit;
};

// information about all instructions - keyed by instruction mnemonic
extern InstructionInfo instruction_info[ mn__count ];

// types of comparisons
// this ennumeration will be set at binary nodes if it is a conditional statement
// and it will be used when emitting conditional and branch instructions
enum Compare_type { 
		cmp_unknown,
		cmp_GT,		// cmp_greater cmp_than
		cmp_GE,		// cmp_greater cmp_or cmp_equal
		cmp_LE,		// cmp_less cmp_or cmp_equal
		cmp_LT,		// cmp_less cmp_than
		cmp_EQ,		// cmp_equal cmp_to
		cmp_NE		// cmp_not cmp_equal cmp_to		 
};


// Class LirInst contains all the useful information about an instruction
// Here we specify the source registers, the destination register, immediate values,
// mnuemonic for the instruction, symbolic addresses (var_name), comparison type (for
// conditionals), and the kind of instruction operands (Opnd_type)
// Most of these values are set in LIR.cc
// If we find that the code generator contains insufficient information about
// instructions or the machine-specific instruction set, then we can eliminate
// ambiguity by adding fields in this class.

class LirInst
{
	// for base+offset instructions, if the base register is the stack or frame pointer
#define DATA_CONTENTS_STACKP		((declNode*)4)
#define DATA_CONTENTS_FRAMEP		((declNode*)5)

	// for operations involving arch-defined input/output regs
#define DATA_CONTENTS_REG_IN		((declNode*)6)
#define DATA_CONTENTS_REG_OUT		((declNode*)7)

	// for function calls, the return value of the function
#define DATA_CONTENTS_RETVAL		((declNode*)8)

	// flag value
#define DATA_CONTENTS__SPECIAL		((declNode*)10)

public:
// instruction components

	// operands (register or constant)
	Register		opnd1;
	declNode *		opnd1_contents;
	reg_or_const	opnd2;
	declNode *		opnd2_contents;

	// destination register
	Register		dest;
	declNode *		dest_contents;

	// base+offset registers for loads/stores
	Register		memBase;
	declNode *		memBase_contents;
	reg_or_const	memOffset;
	declNode *		memOffset_contents;

	// instruction mnemonic
	mnemonic		instruction;

	// branch target, procedure name, variable name, or label name
	string			target;

	// contents for static variables
	// for strings, the string is formatted with no quotes
	// for binary, this is up to MAX_LIR_DATA_BYTES bytes, most significant byte first
	string			dataString;
#define MAX_LIR_DATA_BYTES 8
	char			dataBinary[MAX_LIR_DATA_BYTES];

	// operand types
	lir_var_type	vtPrimary; // primary instruction datatype
	lir_var_type	vtConvert; // for conversions, destination datatype

public:
// other handy stuff

	// the basic block to which we belong
	LirBlock *		block;

	// for BeginProc, EndProc, and Return, the procNode of the current procedure
	// for Call, the callNode of the function call
	// for DeclXxx, the declNode of the declaration
	// for LoadStatic, the declNode of the static variable
	Node *			nodeExtra;

	// for jumps/branches, the target instruction (if branch taken)
	LirInst *		targetInst;

	// for dataflow problems that need to process all instructions, a visited flag
	bool			visited;

	// for dataflow problems on per-instruction level, successors/predecessors of each
	//	instruction
	instruction_set	succs;
	instruction_set	preds;

public:
// primary interface

	// constructor/destructor
	LirInst();
	LirInst( mnemonic instruction );
	~LirInst();

	// for easier printing
	friend ostream & operator << ( ostream& os, const LirInst & inst );

	// stringize this thing
	string			to_string() const;

	// find out if this instruction should actually be included in assembly generation
	bool			should_emit() const;

	// find out if this instruction type has various things
	// declDefines - if true, declaration instructions for register variables
	//	are considered to define that register.
	// regOnly - if true, for components that can be a register or a constant, 
	//	return true only if the component is actually a register.
	bool			has_dest( bool declDefines = false ) const;
	bool			has_opnd1() const;
	bool			has_opnd2( bool regOnly = true ) const;
	bool			has_base() const;
	bool			has_offset( bool regOnly = true ) const;
	bool			has_target() const;

	// get our destination register - this is here so that if we are a decl of a variable
	//	that is allocated to a register, we can pretend to define that register
	Register		get_dest();

	// stringize the contents of some part of an instruction
	static string	contents_str( declNode * pDecl );
	static int		contents_compiler_temp_index;

};


// represents a basic block at LIR level
class LirBlock
{
public:
// primary interface

	// ctor
	LirBlock( int number )
	{
		// save number
		_number = number;

		// init other values
		_next = NULL;
		_depth = 0;
		_visited = false;
	}

	// access instruction list instructions
	const instruction_list & insts() const;
	void add_inst_before( LirInst * newInst, LirInst * otherInst );
	void add_inst_after( LirInst * newInst, LirInst * otherInst );
	void add_inst( LirInst * inst );
	void remove_inst( LirInst * inst );

	// reset visitation flag on this and all successors
	void reset_visited();

	// successors/predecessors
	LirBlockSet _preds;
	LirBlockSet _succs;

	// next block in linear block order
	LirBlock * _next;

	// dominators of this block
	LirBlockSet _doms;		// dominators

	// block number
	unsigned int _number;

	// loop depth
	unsigned int _depth;

	// visitation flag
	// NOTE: this is just for the convenience of people that need to traverse 
	//	the CFG and not worry about getting into an infinite loop.
	bool _visited;

protected:

	// instructions in this block
	instruction_list _instructions;
};


#endif // __INSTRUCTION_H

