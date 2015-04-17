#ifndef LLVM_BDWY_MEM_USES
#define LLVM_BDWY_MEM_USES

// authors: Dustin, Parth

#include "../Utils.h"

namespace llvm_bdwy {

/*
 * TODO explain big picture
 */
// TODO so few privates...

// forward declarations
class Mem_Block;
class Mem_Def;
class Mem_Use;
class Loc;
class BB_Loc;

typedef std::vector<Mem_Use*> mem_use_ls_t;
typedef std::map<Loc*, Mem_Use*> use_map; 

typedef std::map<BasicBlock*, Mem_Use*> pred_use_map;

typedef std::map<Loc*, pred_use_map*> merge_use_map;
typedef merge_use_map::iterator merge_use_map_itr;
typedef merge_use_map::const_iterator merge_use_map_citr;
typedef merge_use_map::value_type merge_use_map_pair;

class OrderedUses {
  public:
    Mem_Use* find_use(Loc *loc) const;
    Mem_Use* make_use_at(Loc *loc);
    pred_use_map* make_merge_uses_at(BB_Loc *loc);
    int size() const;

  private:
    use_map uses;
    merge_use_map merge_uses;
};

// This should result from LOADs (implicit or explicit).
class Mem_Use {
  private:

  public:
    Mem_Def* reaching_def;
    Loc* where;
    bool is_weak;

    Mem_Use (Loc *at) : reaching_def(NULL), where(at), is_weak(false) {}
    void prettyprint (raw_ostream &buf) const;
};

// pretty-printing
namespace llvm {
  raw_ostream &operator<<(raw_ostream &os, const Mem_Use &use);
}

}

#endif
