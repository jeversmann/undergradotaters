//authors: Dustin, Parth

#include "Uses.h"
#include "Location.h"

using namespace llvm_bdwy;

namespace llvm {
  raw_ostream &operator<<(raw_ostream &os, const Mem_Use &use) {
    use.prettyprint(os);
    return os;
  }
}

void Mem_Use::prettyprint (raw_ostream &os) const {
  os << "Use(@ " << *where << ", rdef = " << reaching_def << ")";
}

////////////////////////////////////////////////////////////////////////////////
// Ordered Uses stuff

Mem_Use* OrderedUses::find_use(Loc *loc) const {
  if(loc) {
    use_map_citr itr = uses.find(loc);
    if(itr != uses.end()) {
      return (*itr).second;
    }
  }
  return NULL;
}

Mem_Use* OrderedUses::make_use_at(Loc *loc) {
  Mem_Use *res = find_use(loc);
  if(!res) {
    res = new Mem_Use(loc);
    uses.insert(use_map_pair(loc, res));
  }
  return res;
}

// TODO not used right now
/*pred_use_map* OrderedUses::make_merge_uses_at (BB_Loc *bb_loc) {
  // don't compute more than once
  if (merge_uses.count((Loc*) bb_loc)) {
    return merge_uses[(Loc*) bb_loc];
  }

  pred_use_map *pmap = new pred_use_map(); 
  merge_uses[(Loc*) bb_loc] = pmap;
  foreach_pred(pred, bb_loc->bb) {
    Mem_Use *use = new Mem_Use(bb_loc);
    (*pmap)[*pred] = use;
    // TODO hey look, no rdef.
  }

  return pmap;
}
*/

int OrderedUses::size () const {
  return uses.size();
}
