//authors: Dustin, Parth

#include "Blocks.h"
#include "Location.h"
#include "Mem_Table.h"

using namespace llvm_bdwy;

int Mem_Block::synthetic_counter = 0;

// TODO use this less, try to do Mem_Def->add_pointers
void llvm_bdwy::union_memblock_ls (mem_block_ls_t *lhs, mem_block_ls_t *rhs) {
  foreach_memblock(block, *rhs) {
    lhs->insert(*block);
  }
}

//TODO: this O(n^2) solution is gross.
//How to improve? Maybe if we had Mem_Block_Ptr class we could override
//equals on..
bool llvm_bdwy::equal_memblock_ls (mem_block_ls_t *lhs, mem_block_ls_t *rhs) {
  // ensure equal size
  if (lhs->size() != rhs->size()) {
    return false;
  }

  foreach_memblock(left, *lhs) {
    bool match = false;
    foreach_memblock(right, *rhs) {
      if (*left == *right) {
        match = true;
        break;
      }
    }
    if (!match) {
      return false;
    }
  }

  // for every element in lhs, it found one in rhs
  // so sets are equal
  return true;
}

// print the name of a Value (either a symbol from the src code or its
// instruction)
std::string llvm_bdwy::name_val (Value* v) {
  if (v->getName().size() == 0) {
    std::string str_buf;
    raw_string_ostream buf(str_buf);
    buf << *v;
    return buf.str();
  } else {
    return v->getName();
  }
}

namespace llvm {
  // dump a list of memory blocks
  raw_ostream &operator<<(raw_ostream &os, const mem_block_ls_t &ls) {
    os << ls.size() << " MBs: [";
    
    for (mem_block_ls_t::iterator block = ls.begin(); block != ls.end(); ++block) {
      os << **block;
      mem_block_ls_t::iterator next = block;  // hopefully this copies properly
      next++;
      if (next != ls.end()) {
        os << ", ";
      }
    }
    os << "]";
    return os;
  }
  
  // dump one thing
  // TODO it may be convenient to make all of these things extend a base class
  // just to harness things like prettyprint
  raw_ostream &operator<<(raw_ostream &os, const Mem_Block &mb) {
    mb.prettyprint(os);
    return os;
  }

}

void Mem_Block::prettyprint (raw_ostream &os) const {
  os << getName() << "{" << defs->size() << " defs, " << uses.size() << " uses}";
}

std::string Mem_Block::getName () const {
  std::string str_buf;
  raw_string_ostream buf(str_buf);

  if (type == NORMAL_TYPE) {
    buf << "MB(" << name_val(decl) << ")";
  } else if (type == HEAP_TYPE) {
    buf << "MB(Heap " << synthetic_num << ")";
  } else if (type == STRUCT_TYPE) {
    buf << "MB(Struct field " << synthetic_num << ")";
  } else if (type == RETURN_TYPE) {
    buf << "MB(Return of " << owner_proc->getName() << ")";
  }

  return buf.str();
}

////////////////////////////////////////////////////////////////////////////////

Mem_Block::Mem_Block (Value *val, Function *local_to) : decl(val), owner_proc(local_to) {
  type = NORMAL_TYPE;
  is_multiple = isa<ArrayType>(val->getType()) ? true : false; 
  defs = new OrderedDefs();
  current_use = NULL;
  current_def = NULL;
  is_flow_sensitive = !ENABLE_FI;
  synthetic_num = 0;
}

// synthetic constructor for mallocs and struct fields
Mem_Block::Mem_Block (Function *local_to, MB_Type mem_type) : decl(NULL), owner_proc(local_to) {
  type = mem_type;
  // give an id just for prettyprinting
  synthetic_num = synthetic_counter++;

  if (type == HEAP_TYPE) {
    // memory on the heap could always represent multiple things
    is_multiple = true;
  }
  defs = new OrderedDefs();
  current_use = NULL;
  current_def = NULL;
}

// this actually creates defs
Mem_Def* Mem_Block::def_at (Loc *loc, bool &new_def) {
  Mem_Def* def = NULL;
  if(! is_array()) {
    def = defs->make_def_at(loc, this, new_def);
  } else {
    def = setup_array_def();
    new_def = false;
  }
  current_def = def;
  return def;
}

// this is different from ci_nearest_def_at(), which wraps this.
Mem_Def* Mem_Block::nearest_def_at (Loc *loc) {
  Mem_Def* def = NULL;
  if(! is_array()) {
    def = defs->find_strictly_dominating_def(loc);
  } else {
    def = setup_array_def();
  }
  return def;
}

Mem_Def* Mem_Block::last_def_at (BB_Loc *bb_loc) {
  Mem_Def* def = NULL;
  if(! is_array()) {
    Stmt_Loc *last = bb_loc->last();
    def = defs->find_dominating_def(last);
  } else {
    def = setup_array_def();
  }
  return def;
}

// this doesn't create defs, unlike def_at()
Mem_Def* Mem_Block::find_def_at (Loc *loc) {
  if(! is_array()) {
    return defs->find_def(loc);
  } else {
    return setup_array_def();
  }
}

// this just fills in uses that don't have reaching defs yet?
// TODO not using this right now
/*mem_use_ls_t* Mem_Block::merge_uses_at (BB_Loc *bb_loc) {
  mem_use_ls_t *uses = new mem_use_ls_t();
  pred_use_map *pred_uses = this->uses.make_merge_uses_at(bb_loc);
  Proc_Loc *proc_loc = bb_loc->parent;
  Mem_Def *def, *dom_def;

  dom_def = defs->find_strictly_dominating_def(bb_loc);
  foreach_pred(pred, bb_loc->bb) {
    Mem_Use *use = (*pred_uses)[*pred];
    BB_Loc *pred_loc = proc_loc->lookup_block(*pred);

    // TODO in cbz there's of "hypothesis testing" involving searching for a
    // def, so it's quite unclear which of these actually ends up being the
    // case..
    def = last_def_at(pred_loc);
    def = use->reaching_def;
    def = dom_def;

    use->reaching_def = def;
    uses->push_back(use);
  }
  
  return uses;
}
*/

// TODO not using this right now
/*void Mem_Block::setup_merge_uses_at (BB_Loc *merge_loc, Mem_Def *reaching_def)
{
  pred_use_map *pred_uses = uses.make_merge_uses_at(merge_loc);
  DominatorTree *dom_tree = Loc::proc_db->get_domtree(merge_loc->parent->func);

  foreach_pred(pred, merge_loc->bb) {
    // make sure bb_loc->bb dominates this pred; that's CBZ's formulation of the
    // dom frontier
    if (dom_tree->dominates(merge_loc->bb, *pred)) {
      Mem_Use *use = (*pred_uses)[*pred];
      Mem_Def *old_def = use->reaching_def;
      if ((!old_def) || Loc::strictly_dominates(old_def->where, reaching_def->where)) {
        use->reaching_def = reaching_def;
        //use->search_for_def = false;      pretty sure this is just used for
        //                                  'hypothesis testing'
      }
      // looks like we're setting the good reaching_def we pass in...
    }
  }
}
*/

Mem_Use* Mem_Block::use_at (Loc *loc) {
  Mem_Use* use = uses.make_use_at(loc);
  if(is_array()) {
    setup_array_def();
  }
  current_use = use;
  return use;
}

Mem_Block* Mem_Block::struct_access (Value *idx, Mem_Block *parent) {
  // TODO could be more efficient; all the indices are small constant ints
  if (contains.count(idx)) {
    return contains[idx];
  } else {
    // lazily make it
    // use the version of mem blocks usually used for heap stuff without a
    // declaration
    log() << "Creating synthetic object for " << *parent << " which has decl " << *(parent->decl) << "\n";
    Mem_Block *field = Loc::memory_model->create_synthetic_obj(parent->owner_proc, STRUCT_TYPE);
    field->decl = parent->decl;

    contains[idx] = field;
    return field;
  }
}

// thin wrapper is thin. :|
void Mem_Block::dump_defs () {
  defs->dump();
}

void Mem_Block::set_current_def_use (Loc *loc) {
  current_def = defs->find_def(loc);
  current_use = uses.find_use(loc);
}

void Mem_Block::graph(raw_os_ostream &os) {
  defs->graph(os);  
}
