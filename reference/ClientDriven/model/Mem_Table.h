#ifndef LLVM_BDWY_MEM_TABLE
#define LLVM_BDWY_MEM_TABLE

// authors: Dustin, Parth

#include "../Utils.h"
#include "Location.h"
#include "Blocks.h"

/*
 * Map (Location, Value) pairs to Mem_Blocks and lazily create them.
 */

namespace llvm_bdwy {

typedef std::pair<Loc*, Value*> mem_block_key;
typedef std::map<mem_block_key, Mem_Block*> map_blocks_t;
typedef std::map<Loc*, Mem_Block*> heap_map_t;
typedef std::map<Value*, Mem_Block*> global_map_t;

class Mem_Table {
  private:
    map_blocks_t blocks;
    global_map_t globals;
    heap_map_t heap_model;
    // only track these for debugging.
    mem_block_ls_t struct_ls;
    mem_block_ls_t return_ls;
    
  public:
    // TODO general Loc?
    Mem_Block* lookup_var (Proc_Loc *loc, Value *value);

    Mem_Block* create_global (GlobalVariable *decl);
    Mem_Block* lookup_global (Value *decl);

    Mem_Block* lookup_heap_object (Stmt_Loc *loc);

    Mem_Block* create_synthetic_obj (Function *f, MB_Type type);

    // yo dawg i heard you like debugging, so I'm gonna spam yo terminal
    void dump();
    void graph(raw_os_ostream&);
};

}

#endif
