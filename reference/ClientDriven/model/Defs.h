#ifndef LLVM_BDWY_MEM_DEFS
#define LLVM_BDWY_MEM_DEFS

// authors: Dustin, Parth

#include "Location.h"

namespace llvm_bdwy {

/*
 * TODO explain big picture
 */

// TODO perhaps uses and defs should know their owner.
// TODO so few privates...

// forward declarations
class Mem_Block;
class Mem_Def;
class Loc;

typedef std::vector<Mem_Def*> def_ls_t;
typedef std::vector<Mem_Def*>::iterator def_ls_t_itr;
typedef std::vector<Mem_Def*>::const_iterator def_ls_t_citr;

typedef std::set<Mem_Block*> mem_block_ls_t;

class OrderedDefs {
  public:
    OrderedDefs (bool is_fs) : is_flow_sensitive(is_fs) {}
    OrderedDefs () : is_flow_sensitive(!ENABLE_FI) {}
    Mem_Def* find_def(Loc *loc) const;
    // use for reaching def of regular use
    Mem_Def* find_strictly_dominating_def(Loc *loc) const;
    // use for reaching def of merge-point use
    Mem_Def* find_dominating_def(Loc *loc) const;
    Mem_Def* make_def_at(Loc* loc, Mem_Block *owner, bool &is_new);
    int size() const;
    void prettyprint (raw_ostream &buf) const;
    void dump () const;
    void graph (raw_os_ostream &os) const;
 
  private:
    std::vector<Mem_Def*> defs;
    bool is_flow_sensitive;
};

// Rather than duplicate the flow values (points-to sets) at every program point
// in our massive interprocedural mess, we just sparsely record where they
// change. This should result from STOREs.
class Mem_Def {
  private:

  public:
    Loc* where;
    mem_block_ls_t *points_to;   // anything established at this point
    // TODO: Determine if this should stay
    Mem_Block* owner; // WHAT IS THIS FOR? I DO NOT KNOW YET!
    bool is_weak;

    Mem_Def (Loc *loc, Mem_Block *owner) : where(loc), owner(owner), is_weak(false) {
      points_to = new mem_block_ls_t();
    }
    void clear_points_to ();
    void add_pointers (mem_block_ls_t *ls);
    void prettyprint (raw_ostream &buf) const;
    void merge_with (Mem_Def *md);
    void graph(raw_os_ostream&) const;
};

// pretty-printing
namespace llvm {
  raw_ostream &operator<<(raw_ostream &os, const Mem_Def &def);
  raw_ostream &operator<<(raw_ostream &os, const OrderedDefs &def);
}

}

#endif
