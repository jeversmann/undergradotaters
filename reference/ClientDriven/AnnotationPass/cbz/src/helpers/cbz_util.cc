// $Id: cbz_util.cc,v 1.3 2003/08/07 23:13:41 pnav Exp $
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
#include "stdarg.h"

// handy string formatter function
string 
cbz_util::string_format( const char * pFormat, ... )
{
	// format it
	va_list va;
	va_start( va, pFormat );
	const int bufsize = 200;
	char buf[ bufsize ];
#ifdef _WIN32
	_vsnprintf( buf, bufsize, pFormat, va );
#else 
	vsnprintf( buf, bufsize, pFormat, va );
#endif
	va_end( va );
	buf[bufsize-1] = 0;

	// make a string from it
	return string( buf );
}

// handy string-replacement function
void 
cbz_util::string_replace( string & str, const string & toFind, const string & toReplaceWith )
{
	// repeatedly find the thing to be replaced and replace it
	string::size_type i;
	while ( (i = str.find( toFind )) != string::npos )
	{
		// replace it
		str.replace( i, strlen( toFind.c_str() ), toReplaceWith );
	}
}

void 
cbz_util::string_to_lower( string & str ) 
{
	// convert the whole thing
	string::iterator pStr = str.begin();
	while ( pStr != str.end() )
	{
		*pStr = tolower( *pStr );
		pStr++;
	}
}
