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


#ifndef _CBZ_UTIL_H
#define _CBZ_UTIL_H

#include <string>
#include <list>
using namespace std;

// cbreeze utility stuff
namespace cbz_util
{

	// ----------------------------------------------------------------------
	// string maniuplation stuff

	// like sprintf but gives you a result in a string
	string string_format( const char * pFormat, ... );

	// handy string-replacement function
	void string_replace( string & str, const string & toFind, const string & toReplaceWith );

	// make a string lower-case
	void string_to_lower( string & str );

	// ----------------------------------------------------------------------
	// vector maniuplation stuff

	// copy a vector into another vector
	template <typename T>
	static void vector_copy( const vector<T> & copyFrom, vector<T> & copyTo ) 
	{
		// resize the destination and copy the source over
		size_t sz = copyFrom.size();
		copyTo.resize( sz );
		for ( int i = 0; i < (int)sz; ++i )
			copyTo[i] = copyFrom[i];
	}

	// find an element in a vector - returns array pos if found, -1 otherwise
	template <typename T>
	static int vector_find( const vector<T> & toSearch, const T & toFind ) 
	{
		// search the vector for this element
		size_t sz = toSearch.size();
		for ( int i = 0; i < (int)sz; ++i )
		{
			if ( toSearch[i] == toFind )
				return i;
		}

		// not found
		return -1;
	}

	// ----------------------------------------------------------------------
	// list maniuplation stuff

	// find an element in a list
	template < typename T, typename Allocator >
	bool list_find( const list<T, Allocator> & toSearch, const T & toFind )
	{
		list<T, Allocator>::const_iterator it = toSearch.begin();
		while ( it != toSearch.end() )
			if ( *(it++) == toFind )
				return true;
		return false;
	}

};

#endif

