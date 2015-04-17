#ifndef LLVM_BDWY_MEM_DU_BLOCKS
#define LLVM_BDWY_MEM_DU_BLOCKS

// authors: Dustin, Parth

#include "../Utils.h"
#include "Uses.h"
#include "Defs.h"
#include <string>
#include "llvm/BasicBlock.h"

/*
 * TODO explain big picture
 */

// TODO so few privates...

namespace llvm_bdwy {

// forward declarations
class OrderedDefs;
class Mem_Block;
class Mem_Def;
class Mem_Use;
class Loc;
class BB_Loc;

/*typedef std::vector<Mem_Def*> def_ls_t;
typedef std::vector<Mem_Def*>::iterator def_ls_t_itr;
typedef std::vector<Mem_Def*>::const_iterator def_ls_t_citr;*/

typedef use_map::iterator use_map_itr;
typedef use_map::const_iterator use_map_citr;
typedef use_map::value_type use_map_pair;

typedef std::map<Value*, Mem_Block*> map_struct_fields_t;
typedef std::set<Mem_Block*> mem_block_ls_t;

typedef pred_use_map::const_iterator pred_use_map_citr;
typedef pred_use_map::iterator pred_use_map_itr;


enum MB_Type { NORMAL_TYPE, HEAP_TYPE, STRUCT_TYPE, RETURN_TYPE };

// Anything that can point or be pointed to.
class Mem_Block {
  private:
    // just used for printing
    static int synthetic_counter;

    // TODO what's the order for the uses?
    int synthetic_num;   // just a debugging name for synthetics
    bool is_multiple;       // array or heap. assignments through here are never strong! TODO make that so!

    // "a def is never preceded by another def that dominates it"
    // - find reaching def with linear search: first def that dominates the
    //   current loc is the nearest reaching def.
    // - use this search also to find position for new mem def
    // These are only pointers to avoid some circular header nightmares.
    OrderedDefs *defs;
    OrderedUses uses;

    map_struct_fields_t contains;
    bool is_flow_sensitive; //TODO: there is some irregularity between this and Main_DFA_Algo. it says FI, we say FS. eventually standardize

    // TODO deal with arrays
    bool is_array() const { return false; }
    Mem_Def* setup_array_def() { return NULL; }
    MB_Type type;

  public:
    Value *decl;    // declaration if it's a normal value

    Mem_Block (Value *val, Function *local_to);
    // special synthetic version used for malloc/heap/return val stuff
    Mem_Block (Function *local_to, MB_Type mem_type);
    void prettyprint (raw_ostream &os) const;
    std::string getName () const;
    bool can_be_strong_update() const { return !is_multiple; }
    bool is_array_or_heap() const { return is_multiple; }
    OrderedDefs* get_defs() { return defs; } // only for debug

    // Used during expression evaluation. It's unclear whether this is an
    // optimization or a necessary semantic. Set by use_at() / def_at(). use
    // apparently doesn't have to be reset? but current_def SHOULD be reset when
    // we leave a procedure. TODO
    Mem_Def* current_def;
    Mem_Use* current_use;
    Function *owner_proc;   // for locals, lets ud detect non-local access. NULL for global.

    Mem_Def* def_at (Loc *loc, bool &new_def);
    Mem_Def* nearest_def_at (Loc *loc);

    // TODO: Who is in charge of this? dunno, somebody do it. how are Uses
    // ordered?
    Mem_Use* use_at (Loc *loc);

    Mem_Def* last_def_at (BB_Loc *bb_loc);
    Mem_Def* find_def_at (Loc *loc);

    void setup_merge_uses_at (BB_Loc *bb_loc, Mem_Def *reaching_def);
    mem_use_ls_t* merge_uses_at(BB_Loc *bb_loc);

    Mem_Block *struct_access (Value *idx, Mem_Block *parent);

    void set_current_def_use (Loc *loc);

    void dump_defs ();
    void dump_uses ();
    void graph (raw_os_ostream&);
};

// This _only_ exists because handle_assign needs to know if a list of MBs just
// had their pointer taken or not, to follow points-to sets further or not.
class Ptr_Val {
  public:
    mem_block_ls_t *ls;
    bool is_addr;

    // TODO maybe might as well stick some set wrapper functions in here

    Ptr_Val (mem_block_ls_t *blocks) : ls(blocks), is_addr(false) {}
};

// Utility functions. I'd love to put them in Utils.h, but...

void union_memblock_ls (mem_block_ls_t *lhs, mem_block_ls_t *rhs);
bool equal_memblock_ls(mem_block_ls_t *lhs, mem_block_ls_t *rhs);
std::string name_val (Value* v);

}

// pretty-printing
namespace llvm {
  raw_ostream &operator<<(raw_ostream &os, const llvm_bdwy::mem_block_ls_t &ls);
  raw_ostream &operator<<(raw_ostream &os, const llvm_bdwy::Mem_Block &mb);
  raw_ostream &operator<<(raw_ostream &os, const llvm_bdwy::OrderedDefs &defs);
}

#endif
