//authors: Dustin, Parth

#include "Defs.h"

using namespace llvm_bdwy;

namespace llvm {
  raw_ostream &operator<<(raw_ostream &os, const Mem_Def &def) {
    def.prettyprint(os);
    return os;
  }

  raw_ostream &operator<<(raw_ostream &os, const OrderedDefs &defs) {
    defs.prettyprint(os);
    return os;
  }
}

void Mem_Def::clear_points_to () {
  points_to->clear();
}

void Mem_Def::add_pointers (mem_block_ls_t *ls) {
  union_memblock_ls(points_to, ls);
}

void Mem_Def::prettyprint (raw_ostream &os) const {
  os << "Def(@ " << *where << ")";
}

void Mem_Def::graph(raw_os_ostream &os) const {
  // TODO
}

// TODO this is possibly not ever really needed, we already have add_pointers
void Mem_Def::merge_with (Mem_Def *md) {
  // TODO more to do here?
  union_memblock_ls(points_to, md->points_to);
}


////////////////////////////////////////////////////////////////////////////////
// OrderedDefs stuff

Mem_Def* OrderedDefs::find_def(Loc *loc) const {
  if (is_flow_sensitive) {
    for(def_ls_t_citr itr = defs.begin(); itr != defs.end(); itr++) {
      if((*itr)->where == loc) return *itr;
    }
    return NULL;
  } else {
    return (defs.size() == 0) ? NULL : defs[0];
  }
}

Mem_Def* OrderedDefs::find_strictly_dominating_def(Loc *loc) const {
  if (is_flow_sensitive) {
    for(def_ls_t_citr itr = defs.begin(); itr != defs.end(); itr++) {
      if(Loc::strictly_dominates((*itr)->where, loc)) return *itr;
    }
    return NULL;
  } else {
    return (defs.size() == 0) ? NULL : defs[0];
  }
}

Mem_Def* OrderedDefs::find_dominating_def(Loc *loc) const {
  if (is_flow_sensitive) {
    for(def_ls_t_citr itr = defs.begin(); itr != defs.end(); itr++) {
      if(Loc::dominates((*itr)->where, loc)) return *itr;
    }
    return NULL;
  } else {
    return (defs.size() == 0) ? NULL : defs[0];
  }
}

Mem_Def* OrderedDefs::make_def_at(Loc *loc, Mem_Block *owner, bool &is_new) {
  if (is_flow_sensitive) {
    is_new = false;
    Mem_Def* old = find_def(loc);
    if(old != NULL) return old;

    is_new = true;
    def_ls_t_itr itr = defs.begin();
    while(itr != defs.end()) {
      if(Loc::dominates((*itr)->where, loc)) break;
      itr++;
    }
    Mem_Def *new_def = new Mem_Def(loc, owner);
    defs.insert(itr, new_def);
    return new_def;
  } else {
    if (defs.size() == 0) {
      // first def
      is_new = true;
      // FI means no location
      Mem_Def *new_def = new Mem_Def(NULL, owner);
      defs.push_back(new_def);
      return new_def;
    } else {
      Mem_Def *old_def = defs[0];
      // to parth: your merge_with() was actually a no-op; the new def had no
      // points-to. something later will just add stuff to the points-to set i
      // think.
      is_new = false;   // there can only ever be one new def for this entire Mem_Block
      return old_def;
    }
  }
}

int OrderedDefs::size() const {
  return defs.size();
}

void OrderedDefs::prettyprint (raw_ostream &os) const {
  os << "[";
  for (def_ls_t_citr itr = defs.begin(); itr != defs.end(); itr++) {
    os << **itr;
    if ((itr + 1) != defs.end()) {
      os << ", ";
    }
  }
  os << "]";
}

void OrderedDefs::dump () const {
  int cnt = 0;
  for (def_ls_t_citr itr = defs.begin(); itr != defs.end(); itr++) {
    Mem_Def *def = *itr;
    // TODO [CI] still has bugs.
    if (def->where) {
      log() << cnt << ") At " << *(def->where) << ", it points to:\n";
    } else {
      if (!is_flow_sensitive) { //if FI
        log () << cnt << "). No location because flow-insensitive. It points to:\n";
      } else {
        log() << cnt << ") At TODO CI NULL, it points to:\n";
      }
    }
    log_push();
    log() << *(def->points_to) << "\n";
    log_pop();
    cnt++;
  }
}

void OrderedDefs::graph (raw_os_ostream &os) const {
  // TODO
}
