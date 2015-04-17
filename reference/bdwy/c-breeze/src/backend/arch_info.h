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

#ifndef _ARCH_INFO_H
#define _ARCH_INFO_H

#include "instruction.h"

class arch_info
{
	// our parser is our friend
	friend class arch_info_parser;

public:
///////////////////////////////
// object management

	// ctor/dtor
	arch_info();
	~arch_info();

public:
///////////////////////////////
// persistence methods

	// load our architecture information from the specified file
	bool load_arch_info( const char * pFile );

	// reset this object
	void reset_info();

	// find out if we are valid right now
	bool is_valid() { return _valid; }

public:
///////////////////////////////
// public types

#define NO_REG ((unsigned int)-1)

	// information about a real register on the machine
	struct register_info
	{
		// constructor
		register_info() 
		{
			reset();
		}

		// more useful constructor
		register_info( unsigned int id, string name ) :
			_id( id ), _name( name ), _type( reg_unknown )
		{
		}

		// reset this register to known state
		void reset()
		{
			// set invalid values
			_id = NO_REG;
			_name.clear();
			_type = reg_unknown;
		}

		// find out if we are valid
		int is_valid() const
		{
			// we are valid if we have a number
			return ( _id != NO_REG );
		}

		// compare 2 of these
		bool operator ==( const register_info & r )
		{
			// numbers are all that matter
			return (_id == r._id);
		}

		// the type of this register
		Reg_type		_type;

		// a logical numeric identifier for this register.
		// this is unique among all real registers on the machine
		unsigned int	_id;

		// the actual name of this register
		string			_name;

		// get an equivalent Register object
		operator Register() const
		{
			// make a new one
			return Register( _id, _type );
		}
	};

	// some special psuedo-registers
	static register_info pseudo_reg_dest;
	static register_info pseudo_reg_src1;
	static register_info pseudo_reg_src2;

	// an array of real registers
	typedef vector<register_info*> register_info_list;
	typedef register_info_list::iterator register_info_list_p;

	// a map from register name to register info
	typedef map<string, register_info*> register_info_map;

public:
///////////////////////////////
// information access methods

	// general asm output stuff
	const char *				get_asm_line_comment() const;
	const char *				get_asm_const_prefix() const;

	// get register information
	const register_info_list &	get_all_regs() const;
	const register_info_list &	get_regs_gpr() const;
	const register_info_list &	get_regs_fpr() const;
	const register_info*		get_reg_sp() const;
	const register_info*		get_reg_fp() const;
	const register_info_list &	get_regs_param_fixed() const;
	const register_info_list &	get_regs_param_float() const;
	const register_info*		get_reg_retval_fixed() const;
	const register_info*		get_reg_retval_float() const;
	const register_info_list &	get_regs_caller_save() const;
	const register_info_list &	get_regs_callee_save() const;
	lir_var_type				get_reg_data_type_gpr() const;
	lir_var_type				get_reg_data_type_fpr() const;

	// get a Register from a register_info
	Register					get_register( const register_info * pInfo );

	// get the name of the given register, if it represents a real register
	// wantPrefixAdd : add extra prefixes specified in arch spec
	// wantPrefixRemove : remove extra prefixes specified in arch spec
	bool						get_reg_name( const Register & reg, string & name, 
									bool wantPrefixAdd = true, bool wantPrefixRemove = false );

	// get the register for a given real register index
	bool						get_reg_by_index( int realRegIndex, Register & reg );

	// read various data sizes
	unsigned int	get_data_size_short() const		{ return _dataSizeShort; }
	unsigned int	get_data_size_int() const		{ return _dataSizeInt; }
	unsigned int	get_data_size_long() const		{ return _dataSizeLong; }
	unsigned int	get_data_size_float() const		{ return _dataSizeFloat; }
	unsigned int	get_data_size_double() const	{ return _dataSizeDouble; }
	unsigned int	get_data_size_ptr() const		{ return _dataSizePtr; }
	unsigned int	get_data_size( lir_var_type vt ) const;

	// read various data alignments
	unsigned int	get_max_data_align() const;
	unsigned int	get_data_align_char() const		{ return _dataAlignChar; }
	unsigned int	get_data_align_short() const	{ return _dataAlignShort; }
	unsigned int	get_data_align_int() const		{ return _dataAlignInt; }
	unsigned int	get_data_align_long() const		{ return _dataAlignLong; }
	unsigned int	get_data_align_float() const	{ return _dataAlignFloat; }
	unsigned int	get_data_align_double() const	{ return _dataAlignDouble; }
	unsigned int	get_data_align_ptr() const		{ return _dataAlignPtr; }
	unsigned int	get_data_align( lir_var_type vt ) const;

	// get stack maintenance parameters
	unsigned int	get_stack_frame_min_size() const	{ return _stackFrameMinSize; }
	int				get_stack_extra_top() const			{ return _stackExtraTop; }
	int				get_stack_extra_bottom() const		{ return _stackExtraBottom; }
	unsigned int	get_stack_align() const				{ return _stackAlign; }
	int				get_stack_formals_offset() const	{ return _stackFormalsOffset; }

	// instruction-mode emulation
	bool emulate_3_address() const	{ return _emulate3Address; }

	// get the actual code for a given instruction, returned as an array of text lines
	bool get_code_for_instruction( const LirInst * pInst, vector<string> & lines );

	// get the dependency information for an instruction
	bool get_instruction_kill_regs( const LirInst * pInst, register_info_list & killRegs );

	// find out if type conversion is necessary between two lir types
	bool types_need_conversion( lir_var_type vtsrc, lir_var_type vtdest );

	// find out if the architecture can handle an immediate for opnd2 of the given instruction
	bool instruction_supports_immediate( mnemonic instruction, lir_var_type vt, const constant & c );


private:
///////////////////////////////
// internal types

	// the lines of text in a code template
	typedef vector<string> code_template;

	/*
		The following is a list of tokens that may be used in instruction 
		template code:

		$dest 		- destination operand
		$opnd1		- source operand 1
		$opnd2		- source operand 2
		$base		- base register for loads/stores
		$offset		- offset register or constant for loads/stores
		$target		- branch/call target, label name
		$stacksize  - the size of the local stack frame
	*/
 
	// a Lir2Asm record (maps from LIR to assembler code)
	struct Lir2Asm
	{
		// reset this guy
		void reset()
		{
			_lirInstTypes.clear();
			_killRegs.clear();
			_codeTemplate.clear();
			_immed = Immed_NA;
		}

		// the set of instruction types that this record applies to
		vector<mnemonic>	_lirInstTypes;

		// data types
		vector<lir_var_type>	_dataTypes;
		vector<lir_var_type>	_convertToTypes;

		// register dependency information
		register_info_list	_killRegs;

		// code template for this instruction
		code_template		_codeTemplate;

		// the Immed flag for this template, if specified
		enum
		{
			Immed_NA = -1,	// value not specified
			Immed_No = 0,	// not immediate
			Immed_Yes = 1	// immeidate
		} _immed;
	};

	// a vector of Lir2Asm things
	typedef vector<Lir2Asm>	Lir2Asm_list;

private:
///////////////////////////////
// helper methods

	// init static variables
	static void do_init_static();

	// find a register in our set of all registers
	// returns nonzero if the register is found, zero otherwise
	bool find_register_info( const char * name, register_info *& regInfoFound ) const;

	// find the Lir2Asm block for a given instruction
	// returns zero if the instruction is not supported, nonzero otherwise
	bool get_Lir2Asm_for_instruction( const LirInst * pInst, Lir2Asm ** ppLir2Asm );
	bool get_Lir2Asm_for_instruction( mnemonic inst, lir_var_type vtsrc, lir_var_type vtdest, bool isImmed, Lir2Asm ** ppLir2Asm );

	// make replacements in the given code template for the given instruction
	void make_template_replacements( const LirInst * pInst, Lir2Asm * pLir2Asm, 
			code_template & outputTemplate );

private:
///////////////////////////////
// member data

	// whether or not we currently have valid data
	bool			_valid;

	// the next logical register ID to use
	unsigned int	_nextRegId;

	// the name of the architecture
	string			_archName;

	// asm formatting stuff
	string			_asmLineComment;
	string			_asmRegPrefixAdd;
	string			_asmRegPrefixRemove;
	vector<string>	_asmRegPrefixRemoveSplit;
	string			_asmConstPrefix;

	// register information
	register_info_list	_regsAll;
	register_info_map	_regMap;

	// different usages of registers
	register_info_list	_regsGpr;
	register_info_list	_regsFpr;
	register_info*		_regSp;
	register_info*		_regFp;
	register_info_list	_regsParamFixed;
	register_info_list	_regsParamFloat;
	register_info*		_regRetvalFixed;
	register_info*		_regRetvalFloat;
	register_info_list	_regsCallerSave;
	register_info_list	_regsCalleeSave;
	lir_var_type		_regDataTypeGpr;
	lir_var_type		_regDataTypeFpr;

	// data size information
	int				_dataSizeLong;
	int				_dataSizeInt;
	int				_dataSizeShort;
	int				_dataSizeDouble;
	int				_dataSizeFloat;
	int				_dataSizePtr;

	// data alignment information
	mutable int		_maxDataAlign;
	int				_dataAlignChar;
	int				_dataAlignShort;
	int				_dataAlignInt;
	int				_dataAlignLong;
	int				_dataAlignFloat;
	int				_dataAlignDouble;
	int				_dataAlignPtr;

	// stack management information
	int				_stackFrameMinSize;
	int				_stackExtraTop;
	int				_stackExtraBottom;
	int				_stackAlign;
	int				_stackFormalsOffset;

	// emulation mode
	bool			_emulate3Address;

	// the set of Lir2Asm records for this arch
	// NOTE: you should NOT change the order of the records of this array
	//	under any circumstances.  if you do, you will mess up the 
	//	mnemonic->Lir2Asm map below.
	Lir2Asm_list	_Lir2Asm_records;

	// a map from instruction mnemonic to a set of array indices of Lir2Asm
	//	records that might be able to handle that instruction.
	typedef map<mnemonic, vector<int> > map_menmonic_to_record_set;
	map_menmonic_to_record_set _Lir2Asm_mnemonicLookup;

	// some temporaries used when parsing Lir2Asm records
	// NOTE: see parse_Lir2Asm_item() for usage of this
	string				_tempParseString;
	register_info_list	_tempRegList;
	bool				_tempBool;

};

#endif

