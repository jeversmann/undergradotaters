
#include "c_breeze.h"
#include "arch_info_parser.h"

// for handling errors when we parse the desc
#define PARSE_ERROR CBZFAIL

arch_info_parser::arch_info_parser() 
{
	// internal initial values
	_initialized = false;
	_pSpecFile = NULL;
	_lastChar = NO_CHAR;
	_wasNewLine = false;
}

arch_info_parser::~arch_info_parser() 
{
}

void arch_info_parser::init_parser()
{
	// only do this once
	if ( _initialized )
		return;

	////////////////////////////////////////////////
	// initialize character classes

	int i;

	// everyone starts special
	for ( i = 0; i < 256; ++i )
		_charClass[i] = SPECIAL;

	// setup alphanum
	for ( i = '0'; i <= '9'; ++i )
		_charClass[i] = ALPHANUM;
	for ( i = 'a'; i <= 'z'; ++i )
		_charClass[i] = ALPHANUM;
	for ( i = 'A'; i <= 'Z'; ++i )
		_charClass[i] = ALPHANUM;

	// setup whitespace and line breaks
	_charClass[' '] = WS;
	_charClass['\t'] = WS;
	_charClass['\n'] = LINEBREAK;


	////////////////////////////////////////////////
	// initialize top-level things to parse

	// top-level stuff
	_thingsToParse.push_back( thing_to_parse( "ArchName",		&arch_info::_archName ) );
	_thingsToParse.push_back( thing_to_parse( "AsmLineComment",	&arch_info::_asmLineComment ) );
	_thingsToParse.push_back( thing_to_parse( "AsmRegPrefixAdd",	&arch_info::_asmRegPrefixAdd ) );
	_thingsToParse.push_back( thing_to_parse( "AsmRegPrefixRemove",	&arch_info::_asmRegPrefixRemove ) );
	_thingsToParse.push_back( thing_to_parse( "AsmConstPrefix",	&arch_info::_asmConstPrefix ) );

	// register stuff
	_thingsToParse.push_back( thing_to_parse( "RegsAll",		&arch_info::_regsAll, true ) );
	_thingsToParse.push_back( thing_to_parse( "RegsGpr",		&arch_info::_regsGpr ) );
	_thingsToParse.push_back( thing_to_parse( "RegsFpr",		&arch_info::_regsFpr ) );
	_thingsToParse.push_back( thing_to_parse( "RegSp",			&arch_info::_regSp ) );
	_thingsToParse.push_back( thing_to_parse( "RegFp",			&arch_info::_regFp ) );
	_thingsToParse.push_back( thing_to_parse( "RegsParamFixed",	&arch_info::_regsParamFixed ) );
	_thingsToParse.push_back( thing_to_parse( "RegsParamFloat",	&arch_info::_regsParamFloat ) );
	_thingsToParse.push_back( thing_to_parse( "RegRetvalFixed",	&arch_info::_regRetvalFixed ) );
	_thingsToParse.push_back( thing_to_parse( "RegRetvalFloat",	&arch_info::_regRetvalFloat ) );
	_thingsToParse.push_back( thing_to_parse( "RegsCallerSave",	&arch_info::_regsCallerSave ) );
	_thingsToParse.push_back( thing_to_parse( "RegsCalleeSave",	&arch_info::_regsCalleeSave ) );
	_thingsToParse.push_back( thing_to_parse( "RegDataTypeGpr",	&arch_info::_regDataTypeGpr ) );
	_thingsToParse.push_back( thing_to_parse( "RegDataTypeFpr",	&arch_info::_regDataTypeFpr ) );

	// data size stuff
	_thingsToParse.push_back( thing_to_parse( "DataSizeShort",		&arch_info::_dataSizeShort ) );
	_thingsToParse.push_back( thing_to_parse( "DataSizeInt",		&arch_info::_dataSizeInt ) );
	_thingsToParse.push_back( thing_to_parse( "DataSizeLong",		&arch_info::_dataSizeLong ) );
	_thingsToParse.push_back( thing_to_parse( "DataSizeFloat",		&arch_info::_dataSizeFloat ) );
	_thingsToParse.push_back( thing_to_parse( "DataSizeDouble",		&arch_info::_dataSizeDouble ) );
	_thingsToParse.push_back( thing_to_parse( "DataSizePtr",		&arch_info::_dataSizePtr ) );

	// data alignment stuff
	_thingsToParse.push_back( thing_to_parse( "DataAlignChar",		&arch_info::_dataAlignChar ) );
	_thingsToParse.push_back( thing_to_parse( "DataAlignShort",		&arch_info::_dataAlignShort ) );
	_thingsToParse.push_back( thing_to_parse( "DataAlignInt",		&arch_info::_dataAlignInt ) );
	_thingsToParse.push_back( thing_to_parse( "DataAlignLong",		&arch_info::_dataAlignLong ) );
	_thingsToParse.push_back( thing_to_parse( "DataAlignFloat",		&arch_info::_dataAlignFloat ) );
	_thingsToParse.push_back( thing_to_parse( "DataAlignDouble",	&arch_info::_dataAlignDouble ) );
	_thingsToParse.push_back( thing_to_parse( "DataAlignPtr",		&arch_info::_dataAlignPtr ) );

	// stack management stuff
	_thingsToParse.push_back( thing_to_parse( "StackFrameMinSize",	&arch_info::_stackFrameMinSize ) );
	_thingsToParse.push_back( thing_to_parse( "StackExtraTop",		&arch_info::_stackExtraTop ) );
	_thingsToParse.push_back( thing_to_parse( "StackExtraBottom",	&arch_info::_stackExtraBottom ) );
	_thingsToParse.push_back( thing_to_parse( "StackAlign",			&arch_info::_stackAlign ) );
	_thingsToParse.push_back( thing_to_parse( "StackFormalsOffset",	&arch_info::_stackFormalsOffset ) );

	// arch_info::Lir2Asm stuff
	_thingsToParse.push_back( thing_to_parse( "LirEmulate3Address",	&arch_info::_emulate3Address ) );
	_thingsToParse.push_back( thing_to_parse( "Lir2Asm",			&arch_info::_Lir2Asm_records ) );

	////////////////////////////////////////////////
	// initialize mnemonic map thingy
	// NOTE: these map keys must be lower-case in order for us to do case-insensitive lookup
#define MNEMONIC( mnemonic ) \
{ \
	string temp( #mnemonic ); \
	cbz_util::string_to_lower( temp ); \
	_mnemonicMap[ temp ] = mn_##mnemonic; \
}
	MNEMONIC( NOP )
	MNEMONIC( ConvertType )
	MNEMONIC( Load )
	MNEMONIC( LoadImmediate )
	MNEMONIC( LoadStatic )
	MNEMONIC( Store )
	MNEMONIC( GetEffectiveAddr )
	MNEMONIC( Move )
	MNEMONIC( Add )
	MNEMONIC( Sub )
	MNEMONIC( Mul )
	MNEMONIC( Div )
	MNEMONIC( Mod )
	MNEMONIC( Neg )
	MNEMONIC( BitwiseOR )
	MNEMONIC( BitwiseAND )
	MNEMONIC( BitwiseXOR )
	MNEMONIC( BitwiseNOT )
	MNEMONIC( BitwiseShiftLeft )
	MNEMONIC( BitwiseShiftRight )
	MNEMONIC( BitwiseRotateRight )
	MNEMONIC( BitwiseRotateLeft )
	MNEMONIC( Compare )
	MNEMONIC( BranchEQ )
	MNEMONIC( BranchNE )
	MNEMONIC( BranchLT )
	MNEMONIC( BranchLE )
	MNEMONIC( BranchGT )
	MNEMONIC( BranchGE )
	MNEMONIC( Jmp )
	MNEMONIC( Call )
	MNEMONIC( Return )
	MNEMONIC( Label )
	MNEMONIC( DeclLocal )
	MNEMONIC( StaticDataString )
	MNEMONIC( StaticDataBinary )
	MNEMONIC( BeginProc )
	MNEMONIC( EndProc )
	MNEMONIC( BeginUnit )
	MNEMONIC( EndUnit )
#undef MNEMONIC

	////////////////////////////////////////////////
	// initialize lir_var_type map thingy
	// NOTE: these map keys must be lower-case in order for us to do case-insensitive lookup
#define VARTYPE( vartype ) \
{ \
	string temp( #vartype ); \
	cbz_util::string_to_lower( temp ); \
	_varTypeMap[ temp ] = vt_##vartype; \
}
	VARTYPE( char )
	VARTYPE( uchar )
	VARTYPE( short )
	VARTYPE( ushort )
	VARTYPE( int )
	VARTYPE( uint )
	VARTYPE( long )
	VARTYPE( ulong )
	VARTYPE( float )
	VARTYPE( double )
	VARTYPE( pointer )
#undef VARTYPE


	// we are now initialized
	_initialized = true;
}

bool 
arch_info_parser::parse( const char * pFileName, arch_info * pArchInfo )
{
	// initialize ourself
	_pArchInfo = pArchInfo;
	init_parser();

	// can we open it?
	_pSpecFile = fopen( pFileName, "rt" );
	if ( ! _pSpecFile )
	{
		fprintf( stderr, "ERROR: Can't load '%s' for reading.\n", pFileName );
		return false;
	}

	// reset parser state
	_lastChar = NO_CHAR;
	_currentLine = 1;

	// munch whitespace
	char c;
	while ( (c = peek_char(true)) != EOF && c == '\n' )
		get_char();

	// repeatedly parse a string and whatever goes with it
	string keyName;
	while ( read_string( keyName ) )
	{
		// we will be case-insensitive
		string keyNameLower( keyName );
		cbz_util::string_to_lower( keyNameLower );

		// can we find an action that goes with this?
		size_t sz = _thingsToParse.size();
		bool found = false;
		for ( int i = 0; i < (int)sz; ++i )
		{
			// is this the thing we're looking at?
			if ( _thingsToParse[i]._keyName == keyNameLower )
			{
				thing_to_parse & parseThing = _thingsToParse[i];

				// parse this thing
				bool res;
				switch ( parseThing._parseType )
				{
				case type_reg:		res = parse_reg_item(		keyName, parseThing._data._reg ); break;
				case type_reg_list:	res = parse_reg_list_item(	keyName, parseThing._data._regList, parseThing._regIsMasterList ); break;
				case type_string:	res = parse_string_item(	keyName, parseThing._data._string ); break;
				case type_integer:	res = parse_integer_item(	keyName, parseThing._data._int ); break;
				case type_boolean:	res = parse_bool_item(		keyName, parseThing._data._bool ); break;
				case type_vartype:	res = parse_vartype_item(	keyName, parseThing._data._vartype ); break;
				case type_Lir2Asm:	res = parse_Lir2Asm_item(	keyName, parseThing._data._Lir2AsmList ); break;
				default: assert( false ); break;
				}

				// success?
				if ( ! res )
					return false;

				// we found what we were looking for - go to the next thing in the file
				found = true;
				break;
			}
		}

		// did we find something?
		if ( ! found )
			PARSE_ERROR( ("unrecognized key name '%s'", keyName.c_str()) );

		// munch whitespace
		while ( (c = peek_char(true)) != EOF && c == '\n' )
			get_char();
	}

	return true;
}

char 
arch_info_parser::get_char( bool skipWhitespace ) 
{
	char c;

	// do we already have one?
	if ( _lastChar == NO_CHAR || (skipWhitespace && _charClass[_lastChar] == WS) )
		peek_char( skipWhitespace );

	// return what we have
	c = (char)_lastChar;
	if ( _lastChar != EOF )
		_lastChar = NO_CHAR;

	return c;
}

char 
arch_info_parser::peek_char( bool skipWhitespace ) 
{
	// do we need to get one?
	while ( _lastChar != EOF && (_lastChar == NO_CHAR || (skipWhitespace && _charClass[_lastChar] == WS)) )
	{
		char ch = do_getc();
		char ch2;
		switch ( ch )
		{
		case EOF:
			// we are finished
			_lastChar = EOF;
			break;

		case '/':

			// what's next?
			ch2 = do_getc();
			if ( ch2 == '*' )
			{
				// eat a block comment
				char ch3;
				while ( ch2 != EOF )
				{
					// is there a '*'?
					ch2 = do_getc();
					if ( ch2 != '*' )
						continue;

					// is there a '/'?
					ch3 = do_getc();
					if ( ch3 == '/' )
						break;

					// put this thing back
					do_ungetc( ch3 );
				}

				// sanity checks
				assert( ch2 == EOF || ch3 == '/' );

				// keep looking for real characters
				continue;
			}
			else if ( ch2 != '/' )
			{
				// this is not a line comment - put ch2 back and 
				//	give the first '/' to caller
				do_ungetc( ch2 );
				_lastChar = ch;
				break;
			}

            // if we are here, we have a c++-style line comment, so 
			//	just fall through to line comment handler

		case '#':

			// line comment - eat all characters up to newline
			while ( ((ch2 = do_getc()) != EOF) && ch2 != '\n');

			// sanity checks
			assert( ch2 == EOF || ch2 == '\n' );

			// go on and look for more interesting things
			continue;

		case '\\': // escape character

			// is next comment character?
			ch2 = do_getc();
			if ( ch2 == '#' )
			{
				// treat this as non-comment
				_lastChar = ch2;
				continue;
			}
			else
			{
				// pass on as normal
				do_ungetc( ch2 );
				_lastChar = ch;
				continue;
			}

			break;

		default:

			// the user probably wants to see this
			_lastChar = ch;
			break;

		}

	}

	return _lastChar;
}

char 
arch_info_parser::do_getc() 
{
	_wasNewLine = false;

	// gat a new char
	char c = fgetc( _pSpecFile );

	// is it an escape char?
	if ( c == '\\' )
	{
		// line continuation character?
		char c2 = fgetc( _pSpecFile );
		if ( c2 == '\n' )
		{
			// it was an escaped newline - eat the newline and give out next thing
			_currentLine++;
			return do_getc();
		}
		else
		{
			// put this back - it was not a newline
			ungetc( c2, _pSpecFile );
			return c;
		}
	}
	else if ( c == '\n' )
	{
		_currentLine++;
		_wasNewLine = true;
	}

	return c;
}

void 
arch_info_parser::do_ungetc( char c ) 
{
	// put it back
	ungetc( c, _pSpecFile );

	// fix line count if it was a newline
	if ( _wasNewLine )
	{
		_wasNewLine = false;
		_currentLine--;
	}
}

bool 
arch_info_parser::read_string( string & stringOut, bool readAnyChars, bool skipLeadingNewlines ) 
{
	// default to no results
	stringOut.clear();

	char c;

	// maybe skip leading newlines
	if ( skipLeadingNewlines )
		while ( (c = peek_char( true )) != EOF && c == '\n' )
			get_char();

	// read all the alphanums we can get in a row (skip preceding spaces)
	c = peek_char( true );
	while ( c != EOF && c != '\n' && (readAnyChars || _charClass[c] == ALPHANUM) )
	{
		// add this to the string and get the next character
		stringOut.append( 1, c );
		get_char();
		c = peek_char(); // don't skip whitespace - that would reformat the string
	}

	// we succeeded if our string was non-empty
	return (! stringOut.empty());
}

bool 
arch_info_parser::parse_reg_item( string keyName, reg_info_ptr toRead ) 
{
	// should have an equal sign first of all
	char c = get_char( true );
	if ( c != '=' )
		PARSE_ERROR( ("expected '=' after key '%s'", keyName.c_str()) );

	c = get_char( true );

	// it may be empty...
	if ( c == '\n' )
		return true;

	// now there should be an @ symbol to denote a register
	if ( c != '@' )
		PARSE_ERROR( ("expected '@' preceding register name") );

	// is there anything here for this item?  it's ok if there is not
	if ( peek_char( true ) == '\n' )
		return true;

	// now a string with the register name
	string regName;
	if ( !read_string( regName ) )
		PARSE_ERROR( ("expected register name after '@'") );

	// this thing really should already be in our list
	arch_info::register_info * info;
	if ( ! _pArchInfo->find_register_info( regName.c_str(), info ) )
		PARSE_ERROR( ("register '%s' is undefined - is it missing from the RegsAll list?", regName.c_str()) );

	// set the member variable for this guy
	_pArchInfo->*toRead = info;

	// should have line terminator now
	c = get_char( true );
	if ( c != '\n' )
		PARSE_ERROR( ("expected newline after register name", regName.c_str()) );

	return true;
}

bool 
arch_info_parser::parse_reg_list_item( string keyName, reg_info_list_ptr toRead, bool isMasterList, bool allowTokens ) 
{
	// clear out the list
	(_pArchInfo->*toRead).empty();

	// master list cannot have tokens
	assert( ! (isMasterList && allowTokens) );

	char c;

	// should have an equal sign first of all
	c = get_char( true );
	if ( c != '=' )
		PARSE_ERROR( ("expected '=' after key '%s'", keyName.c_str()) );

	// read a bunch of registers
	c = get_char( true );
	while ( c == '@' || (allowTokens && c == '$') )
	{
		// now a string with the register name
		string regName;
		if ( !read_string( regName ) )
			PARSE_ERROR( ("expected register name after '%c'", c) );

		// is this the master list?
		if ( isMasterList )
		{
			// add it to the list
			arch_info::register_info * info = new arch_info::register_info;
			info->_id = _pArchInfo->_nextRegId++;
			info->_name = regName;
			assert( (_pArchInfo->*toRead).size() == info->_id );
			(_pArchInfo->*toRead).push_back( info );

			// add it to the map
			_pArchInfo->_regMap[ regName ] = info;
		}
		else
		{
			// go find it in existing list
			arch_info::register_info * info;
			if ( ! _pArchInfo->find_register_info( regName.c_str(), info ) )
				PARSE_ERROR( ("register '%s' is undefined - is it missing from the RegsAll list?", regName.c_str()) );

			// add it to the list
			(_pArchInfo->*toRead).push_back( info );
		}

		// get the next
		c = get_char( true );
	}

	// whatever's left should be line terminator
	if ( c != '\n' )
		PARSE_ERROR( ("after '=', expected register list followed by newline") );

	return true;
}

bool 
arch_info_parser::parse_string_item( string keyName, string_ptr toRead ) 
{
	// should have an equal sign first of all
	char c = get_char( true );
	if ( c != '=' )
		PARSE_ERROR( ("expected '=' after key '%s'", keyName.c_str()) );

	// is there anything here for this item?  it's ok if there is not
	if ( peek_char( true ) == '\n' )
		return true;

	// read everything up to newline
	string str;
	c = get_char( true );
	while ( c != EOF && c != '\n' )
	{
		// add this to the string and get next char
		str.append( 1, c );
		c = get_char( false ); // preserve whitespace inside string
	}

	// save this thing
	_pArchInfo->*toRead = str;

	return true;
}

bool 
arch_info_parser::parse_integer_item( string keyName, int_ptr toRead ) 
{
	// should have an equal sign first of all
	char c = get_char( true );
	if ( c != '=' )
		PARSE_ERROR( ("expected '=' after key '%s'", keyName.c_str()) );

	// is there anything here for this item?  it's ok if there is not
	if ( peek_char( true ) == '\n' )
		return true;

	// read a string after the equal sign
	string str;
	if ( ! read_string( str ) )
		PARSE_ERROR( ("expected numeric constant after '='") );

	// any invalid chars?
	int value = atoi( str.c_str() );
	if ( value == 0 && str.find_first_not_of( "0123456789" ) != -1 )
		PARSE_ERROR( ("expected numeric constant after '='") );

	// whatever's left should be line terminator
	c = get_char( true );
	if ( c != '\n' )
		PARSE_ERROR( ("expected newline after integer constant") );

	// save this thing
	_pArchInfo->*toRead = value;

	return true;
}

bool 
arch_info_parser::parse_bool_item( string keyName, bool_ptr toRead ) 
{
	// should have an equal sign first of all
	char c = get_char( true );
	if ( c != '=' )
		PARSE_ERROR( ("expected '=' after key '%s'", keyName.c_str()) );

	// read a string after the equal sign
	string str;
	if ( ! read_string( str ) )
		PARSE_ERROR( ("expected boolean value after '='") );

	// this string has to be "true", "false", "yes", or "no"
	cbz_util::string_to_lower( str );
	if ( str == "true" || str == "yes" )
		_pArchInfo->*toRead = true;
	else if ( str == "false" || str == "no" )
		_pArchInfo->*toRead = false;
	else
		PARSE_ERROR( ("expected 'true'/'yes' or 'false'/'no' after '='") );

	// whatever's left should be line terminator
	c = get_char( true );
	if ( c != '\n' )
		PARSE_ERROR( ("expected newline after integer constant") );

	return true;
}

bool 
arch_info_parser::parse_vartype_item( string keyName, vartype_ptr toRead ) 
{
	// should have an equal sign first of all
	char c = get_char( true );
	if ( c != '=' )
		PARSE_ERROR( ("expected '=' after key '%s'", keyName.c_str()) );

	// read a string after the equal sign
	string typeStr;
	if ( ! read_string( typeStr ) )
		PARSE_ERROR( ("expected data type after '='") );

	// this string has to map to a variable type
	cbz_util::string_to_lower( typeStr );
	vartype_map::iterator it = _varTypeMap.find( typeStr );
	if ( it == _varTypeMap.end() )
		PARSE_ERROR( ("unrecognized variable type '%s'", typeStr.c_str()) );

	// whatever's left should be line terminator
	c = get_char( true );
	if ( c != '\n' )
		PARSE_ERROR( ("expected newline after integer constant") );

	// save it
	_pArchInfo->*toRead = (*it).second;

	return true;
}

bool 
arch_info_parser::parse_Lir2Asm_item( string keyName, Lir2Asm_list_ptr toAppend ) 
{
	// ignore line breaks and such
	char c;
    while ( (c = get_char( true )) == '\n' );

	// should have a bracket
	if ( c != '[' )
		PARSE_ERROR( ("expected '[' after '%s'", keyName.c_str()) );

	// read the different parts
	bool hasCode = false;
	arch_info::Lir2Asm lir2asm;
	lir2asm.reset();
	string keyStr;
	while ( read_string( keyStr, false, true ) )
	{
		string keyStrLower = keyStr;
		cbz_util::string_to_lower( keyStrLower );
		if ( keyStrLower == "lir" )
		{
			// parse a string here
			_pArchInfo->_tempParseString.clear();
			if ( ! parse_string_item( "lir", &arch_info::_tempParseString ) )
				return false;
			string mnemonicStr = _pArchInfo->_tempParseString;
			_pArchInfo->_tempParseString.clear();

			// look up the mnemonic
			string tempStr( mnemonicStr );
			cbz_util::string_to_lower( tempStr );
			mnemonic_map::iterator it = _mnemonicMap.find( tempStr );
			if ( it == _mnemonicMap.end() )
				PARSE_ERROR( ("unrecognized LIR instruction '%s'", mnemonicStr.c_str()) );

			// save the actual enumerated type of the mnemonic
			lir2asm._lirInstTypes.push_back( (*it).second );
		}
		else if ( keyStrLower == "datatype" )
		{
			// parse a string here
			_pArchInfo->_tempParseString.clear();
			if ( ! parse_string_item( "datatype", &arch_info::_tempParseString ) )
				return false;
			string typeStr = _pArchInfo->_tempParseString;
			_pArchInfo->_tempParseString.clear();

			// look up the variable type
			string tempStr( typeStr );
			cbz_util::string_to_lower( tempStr );
			vartype_map::iterator it = _varTypeMap.find( tempStr );
			if ( it == _varTypeMap.end() )
				PARSE_ERROR( ("unrecognized variable type '%s'", typeStr.c_str()) );

			// add this variable type to supported list for this record
			lir2asm._dataTypes.push_back( (*it).second );
		}
		else if ( keyStrLower == "converttotype" )
		{
			// parse a string here
			_pArchInfo->_tempParseString.clear();
			if ( ! parse_string_item( "converttotype", &arch_info::_tempParseString ) )
				return false;
			string typeStr = _pArchInfo->_tempParseString;
			_pArchInfo->_tempParseString.clear();

			// look up the variable type
			string tempStr( typeStr );
			cbz_util::string_to_lower( tempStr );
			vartype_map::iterator it = _varTypeMap.find( tempStr );
			if ( it == _varTypeMap.end() )
				PARSE_ERROR( ("unrecognized variable type '%s'", typeStr.c_str()) );

			// add this variable type to supported list for this record
			lir2asm._convertToTypes.push_back( (*it).second );
		}
		else if ( keyStrLower == "kill" )
		{
			// read a standard register list into class temporary list
			_pArchInfo->_tempRegList.clear();
			if ( ! parse_reg_list_item( "kill", &arch_info::_tempRegList, false, true ) )
				return false;

			// set this to be the arch_info::Lir2Asm thing's list
			cbz_util::vector_copy( _pArchInfo->_tempRegList, lir2asm._killRegs );
			_pArchInfo->_tempRegList.clear();
		}
		else if ( keyStrLower == "immed" )
		{
			// read true/false for this guy's immediate field
			_pArchInfo->_tempBool = false;
			if ( ! parse_bool_item( "immed", &arch_info::_tempBool ) )
				return false;
			lir2asm._immed = _pArchInfo->_tempBool ? 
				arch_info::Lir2Asm::Immed_Yes : 
				arch_info::Lir2Asm::Immed_No;
		}
		else if ( keyStrLower == "code" )
		{
			char c;

			// there is code for this template
			hasCode = true;

			// eat all whitespace including newlines
		    while ( (c = peek_char( true )) == '\n' || _charClass[c] == WS )
				get_char();

			// what's next?
			get_char();
			if ( c == '=' )
			{
				// just read a string (should be the code)
				string str;
				if ( read_string( str, true ) )
				{
					// this is the code for this guy
					lir2asm._codeTemplate.push_back( str );
				}

				// eat up to and including newline
				c = get_char( true );
				assert( c == EOF || c == '\n' );
				get_char();
			}
			else if ( c == '{' )
			{
				// read as many lines as we need
				while ( 1 )
				{
					// read all chars on this line (skip leading whitespace)
					string line;
					c = get_char( true );
					while ( c != EOF && c != '\n' && c != '}' )
					{
						line += c;
						c = get_char();
					}

					// add to list
					if ( ! line.empty() )
						lir2asm._codeTemplate.push_back( line );

					// are we done?
					if ( c == EOF || c == '}' )
						break;
				}
			}
			else
			{
				// we have no idea what to do here
				PARSE_ERROR( ("missing '=' or '{' after 'code'") );
			}
		}
		else
		{
			// we have no idea what this thing is
			PARSE_ERROR( ("unrecognized key string '%s'.", keyStr.c_str()) );
		}

	}

	// look for close bracket
    while ( (c = get_char( true )) == '\n' );
	if ( c != ']' )
		PARSE_ERROR( ("expected ']' after arch_info::Lir2Asm record") );

	// is there code for this template?  if not, don't add it to any lists.
	if ( ! hasCode )
		return true;

	// add this to the list
	_pArchInfo->_Lir2Asm_records.push_back( lir2asm );
	int addpos = (int)(_pArchInfo->_Lir2Asm_records.size()) - 1;
	assert( addpos >= 0 );

	// setup the map - we are mapping from LIR mnemonic to array index of this arch_info::Lir2Asm thingy
	size_t sz = lir2asm._lirInstTypes.size();
	for ( int i = 0; i < (int)sz; ++i )
	{
		// is there a vector for this guy?  if not, insert one
		arch_info::map_menmonic_to_record_set::iterator it = _pArchInfo->_Lir2Asm_mnemonicLookup.find( lir2asm._lirInstTypes[i] );
		if ( it == _pArchInfo->_Lir2Asm_mnemonicLookup.end() )
		{
			arch_info::map_menmonic_to_record_set::value_type toInsert( lir2asm._lirInstTypes[i], vector<int>() );
			it = _pArchInfo->_Lir2Asm_mnemonicLookup.insert( toInsert ).first;
		}

		// add this thing to the list
		it->second.push_back( addpos );
	}

	return true;
}

