// $Id: hash_set_ex.h,v 1.3 2003/08/07 23:13:45 pnav Exp $
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
//  Charles Nevill
//  Paul Navratil
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

#ifndef _HASH_SET_EX_H
#define _HASH_SET_EX_H

#include <set>
#include <memory>
using namespace std;

// because the template is really ugly - just define it
#define TEMPL template < class Key, class Traits, class Allocator >
#define TARGS < Key, Traits, Allocator >

// a better set class
template < class Key, class Traits=less<Key>, class Allocator=allocator<Key> >
class hash_set_ex : public set TARGS
{
public:
	// ctors
	hash_set_ex( ) {}
	explicit hash_set_ex( const Traits& _Comp ) :							set TARGS( _Comp ) {}
	explicit hash_set_ex( const Traits& _Comp, const Allocator& _Al ) :		set TARGS( _Comp, _Al ) {}
	hash_set_ex( const hash_set_ex& _Right ) :								set TARGS( _Right ) {}

	// set union operators
	hash_set_ex		operator |  ( const hash_set_ex & rhs ) const;
	hash_set_ex&	operator |= ( const hash_set_ex & rhs );
	hash_set_ex		operator |  ( const Key & single ) const;
	hash_set_ex&	operator |= ( const Key & single );

	// set intersection operators
	hash_set_ex		operator &  ( const hash_set_ex & rhs ) const;
	hash_set_ex&	operator &= ( const hash_set_ex & rhs );
	hash_set_ex		operator &  ( const Key & single ) const;
	hash_set_ex&	operator &= ( const Key & single );

	// set subtraction operators
	hash_set_ex		operator -  ( const hash_set_ex & rhs ) const;
	hash_set_ex&	operator -= ( const hash_set_ex & rhs );
	hash_set_ex		operator -  ( const Key & single ) const;
	hash_set_ex&	operator -= ( const Key & single );

	// copy operator
	hash_set_ex&	operator =	( const hash_set_ex & rhs );

	// comparison operators
	bool			operator ==	( const hash_set_ex & rhs ) const;
	bool			operator !=	( const hash_set_ex & rhs ) const;

	// handy typedef
	typedef typename hash_set_ex::iterator hash_set_ex_p;
	typedef typename hash_set_ex::const_iterator hash_set_ex_cp;
};

// set union operators
TEMPL
hash_set_ex TARGS	hash_set_ex TARGS::operator |  ( const hash_set_ex TARGS & rhs ) const
{
	// just insert both of our sets
	hash_set_ex TARGS output( *this );
	output.insert( rhs.begin(), rhs.end() );
	return output;
}
TEMPL
hash_set_ex TARGS&	hash_set_ex TARGS::operator |= ( const hash_set_ex TARGS & rhs )
{
	// insert all of their set
	insert( rhs.begin(), rhs.end() );
	return (*this);
}
TEMPL
hash_set_ex TARGS	hash_set_ex TARGS::operator |  ( const Key & single ) const
{
	// insert our set and the one other thing
	hash_set_ex TARGS output( *this );
	output.insert( single );
	return output;
}
TEMPL
hash_set_ex TARGS&	hash_set_ex TARGS::operator |= ( const Key & single )
{
	// insert the one other thing
	insert( single );
	return (*this);
}

// set intersection operators
TEMPL
hash_set_ex TARGS	hash_set_ex TARGS::operator &  ( const hash_set_ex TARGS & rhs ) const
{
	// insert each thing in our set if it's in theirs
	hash_set_ex TARGS output;
	hash_set_ex_cp it = begin();
	while ( it != end() )
	{
		// add this if it's in rhs as well
		if ( rhs.find( *it ) != rhs.end() )
			output.insert( *it );

		// next element
		++it;
	}
	return output;
}
TEMPL
hash_set_ex TARGS&	hash_set_ex TARGS::operator &= ( const hash_set_ex TARGS & rhs )
{
	// insert each thing in our set if it's in theirs
	hash_set_ex TARGS temp;
	hash_set_ex_p it = begin();
	while ( it != end() )
	{
		// if this is in both sets, add it to output
		if ( rhs.find( *it ) != rhs.end() )
			temp.insert( *it );

		++it;
	}

	// copy over from temp
	*this = temp;
	return (*this);
}
TEMPL
hash_set_ex TARGS	hash_set_ex TARGS::operator &  ( const Key & single ) const
{
	// if this is in our set return it otherwise empty
	hash_set_ex TARGS output;
	if ( find( single ) != end() )
		output.insert( single );
	return output;
}
TEMPL
hash_set_ex TARGS&	hash_set_ex TARGS::operator &= ( const Key & single )
{
	// if this is in our set, set us to it, otherwise empty
	bool in = (find( single ) != end());
	clear();
	if ( in )
		insert( single );
	return (*this);
}

// set subtraction operators
TEMPL
hash_set_ex TARGS	hash_set_ex TARGS::operator -  ( const hash_set_ex TARGS & rhs ) const
{
	// copy us and remove all of their things
	hash_set_ex TARGS output( *this );
	hash_set_ex_cp it = rhs.begin();
	while ( it != rhs.end() )
		output.erase( *(it++) );
	return output;
}
TEMPL
hash_set_ex TARGS&	hash_set_ex TARGS::operator -= ( const hash_set_ex TARGS & rhs )
{
	// remove all of their things
	hash_set_ex_cp it = rhs.begin();
	while ( it != rhs.end() )
		erase( *(it++) );
	return (*this);
}
TEMPL
hash_set_ex TARGS	hash_set_ex TARGS::operator -  ( const Key & single ) const
{
	// copy us and remove the one thing
	hash_set_ex TARGS output( *this );
	output.erase( single );
	return output;
}
TEMPL
hash_set_ex TARGS&	hash_set_ex TARGS::operator -= ( const Key & single )
{
	// remove the one thing
	erase( single );
	return (*this);
}

// copy operator
TEMPL
hash_set_ex TARGS&	hash_set_ex TARGS::operator =	( const hash_set_ex TARGS & rhs )
{
	// copy us over
	clear();
	insert( rhs.begin(), rhs.end() );
	return (*this);
}

// comparison operator
TEMPL
bool				hash_set_ex TARGS::operator ==	( const hash_set_ex TARGS & rhs ) const
{
	// we are equal if we have the same size and every element in rhs is in us
	if ( size() != rhs.size() )
		return false;
	hash_set_ex_cp it = rhs.begin();
	while ( it != rhs.end() )
		if ( find( *(it++) ) == end() )
			return false;
	return true;
}

// comparison operator
TEMPL
bool				hash_set_ex TARGS::operator !=	( const hash_set_ex TARGS & rhs ) const
{
	// are we equal?
	return ( *this == rhs ? false : true );
}

#undef TEMPL
#undef TARGS

#endif // _HASH_SET_EX_H
