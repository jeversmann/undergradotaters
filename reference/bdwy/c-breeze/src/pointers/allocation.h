
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

