// $Id: allocation.h,v 1.7 2003/08/07 23:14:22 pnav Exp $
// ----------------------------------------------------------------------
//
//  C-Breeze
//  C Compiler Framework
// 
//  Copyright (c) 2003 University of Texas at Austin
// 
//  Samuel Z. Guyer
//  Adam Brown
//  Teck Bok Tok
//  Paul Arthur Navratil
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

#ifdef CBZ_HEAPLAYERS

#include "heaplayers.h"
#include "stlallocator.h"

// --- Main heap

class PointersHeap : public UniqueHeap< ZoneHeap < mallocHeap, 1024*1024 > > {};

class FreelistFunHeap : public FreelistHeap<ZoneHeap<mallocHeap, 64 * 1024> > {};

// --- Type-specific allocators for STL

class Location;
typedef STLAllocator< Location *, FreelistFunHeap > location_alloc;

class memoryBlock;
typedef STLAllocator< memoryBlock *, FreelistFunHeap > memoryblock_alloc;

class memoryDef;
typedef STLAllocator< memoryDef *, FreelistFunHeap > memorydef_alloc;

class memoryUse;
typedef STLAllocator< memoryUse *, FreelistFunHeap > memoryuse_alloc;

typedef STLAllocator< basicblockNode *, FreelistFunHeap > basicblock_alloc;


#else

template <class T>
class PerClassHeap {};

class PointersHeap {};
class FreelistFunHeap {};

class Location;
typedef std::allocator< Location * > location_alloc;

class memoryBlock;
typedef std::allocator< memoryBlock * > memoryblock_alloc;

class memoryDef;
typedef std::allocator< memoryDef * > memorydef_alloc;

class memoryUse;
typedef std::allocator< memoryUse * > memoryuse_alloc;

typedef std::allocator< basicblockNode * > basicblock_alloc;

#endif // 

