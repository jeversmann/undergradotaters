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

#ifndef _arch_info_parser_H
#define _arch_info_parser_H

#include "arch_info.h"

class arch_info_parser
{
public:
///////////////////////////////
// object management

	// ctor/dtor
	arch_info_parser();
	~arch_info_parser();

public:
///////////////////////////////
// persistence methods

	// load architecture information from the specified file
	bool parse( const char * pFile, arch_info * pArchInfo );


private:
///////////////////////////////
// parsing helpers

	// initialize various things before parsing
	void init_parser();

	// get the next input character from the file
	// this basically preprocesses the file so the parser is more simple
	char get_char( bool skipWhitespace = false );
	char peek_char( bool skipWhitespace = false );

	// low-level character getter/ungetter routines
	char do_getc();
	void do_ungetc( char );

	// read a string from a file, returns false if none could be read
	// readAnyChars: if true, all characters are read until end of line
	//				 if false, only alphanumerics are read
	bool read_string( string & stringOut, bool readAnyChars = false, bool skipLeadingNewlines = false );

	// types of things we need to be able to load
	enum parse_type
	{
		type__unknown,
		type_reg,		// single register
		type_reg_list,	// list of registers
		type_string,	// string
		type_integer,	// integer
		type_boolean,	// boolean true/false
		type_vartype,	// lir_var_type value
		type_Lir2Asm,	// Lir2Asm record
	};

	// pointer-to-member typedefs for load types
	typedef arch_info::register_info*		arch_info::* reg_info_ptr;
	typedef arch_info::register_info_list	arch_info::* reg_info_list_ptr;
	typedef string							arch_info::* string_ptr;
	typedef int								arch_info::* int_ptr;
	typedef bool							arch_info::* bool_ptr;
	typedef lir_var_type					arch_info::* vartype_ptr;
	typedef arch_info::Lir2Asm_list			arch_info::* Lir2Asm_list_ptr;

	// subroutines for parsing various things
	bool parse_reg_item( string keyName, reg_info_ptr toRead );
	bool parse_reg_list_item( string keyName, reg_info_list_ptr toRead, bool isMasterList = false, bool allowTokens = false );
	bool parse_string_item( string keyName, string_ptr toRead );
	bool parse_integer_item( string keyName, int_ptr toRead );
	bool parse_bool_item( string keyName, bool_ptr toRead );
	bool parse_vartype_item( string keyName, vartype_ptr toRead );
	bool parse_Lir2Asm_item( string keyName, Lir2Asm_list_ptr toAppend );

	// structure defining something that we might need to parse
	struct thing_to_parse
	{
		thing_to_parse() : _parseType( type__unknown )
		{
		}

		// constructors for various types
		thing_to_parse( string keyName, reg_info_ptr toRead ) 
			: _keyName( keyName ), _parseType( type_reg )
		{
			cbz_util::string_to_lower( _keyName );			
			_data._reg = toRead;
		}
		thing_to_parse( string keyName, reg_info_list_ptr toRead, bool isMasterList = false ) 
			: _keyName( keyName ), _parseType( type_reg_list )
		{
			cbz_util::string_to_lower( _keyName );			
			_data._regList = toRead;
			_regIsMasterList = isMasterList;
		}
		thing_to_parse( string keyName, string_ptr toRead ) 
			: _keyName( keyName ), _parseType( type_string )
		{
			cbz_util::string_to_lower( _keyName );			
			_data._string = toRead;
		}
		thing_to_parse( string keyName, int_ptr toRead ) 
			: _keyName( keyName ), _parseType( type_integer )
		{
			cbz_util::string_to_lower( _keyName );			
			_data._int = toRead;
		}
		thing_to_parse( string keyName, bool_ptr toRead ) 
			: _keyName( keyName ), _parseType( type_boolean )
		{
			cbz_util::string_to_lower( _keyName );			
			_data._bool = toRead;
		}
		thing_to_parse( string keyName, vartype_ptr toRead ) 
			: _keyName( keyName ), _parseType( type_vartype )
		{
			cbz_util::string_to_lower( _keyName );			
			_data._vartype = toRead;
		}
		thing_to_parse( string keyName, Lir2Asm_list_ptr toAppend ) 
			: _keyName( keyName ), _parseType( type_Lir2Asm )
		{
			cbz_util::string_to_lower( _keyName );			
			_data._Lir2AsmList = toAppend;
		}

		// the name that identifies this information in the config file
		// e.g. RegsGprInt or Lir2Asm
		string		_keyName;

		// the function that should be used to parse this thing
		// e.g. parse_reg_list() or parse_Lir2Asm()
		parse_type	_parseType;

		// if we are parsing a register list, is it the master list?
		bool		_regIsMasterList;

		// pointer to the data that should be filled out by the parse function
		union 
		{
			reg_info_ptr		_reg;
			reg_info_list_ptr	_regList;
			string_ptr			_string;
			int_ptr				_int;
			bool_ptr			_bool;
			vartype_ptr			_vartype;
			Lir2Asm_list_ptr	_Lir2AsmList;
		} _data;
	};

	// a list of things that may need to be parsed
	typedef vector<thing_to_parse> thing_to_parse_array;

	// top-level things to parse
	thing_to_parse_array	_thingsToParse;


private:
///////////////////////////////
// member data

	// whether or not we are initialized
	bool				_initialized;

	// the architecture info object we are currently setting up
	arch_info *			_pArchInfo;

	// the file we are parsing to load the arch info
	FILE *				_pSpecFile;
	int					_currentLine;

	// used for getchar/peekchar stuff
#define NO_CHAR -2
	int					_lastChar;
	bool				_wasNewLine;

	// character classes
	enum char_class { WS, LINEBREAK, ALPHANUM, SPECIAL };
	char_class			_charClass[256];

	// map from mnemonic str to mnemonic
	typedef map<string, mnemonic> mnemonic_map;
	mnemonic_map		_mnemonicMap;

	// map from data type str to lir_var_type
	typedef map<string, lir_var_type> vartype_map;
	vartype_map			_varTypeMap;

};

#endif

