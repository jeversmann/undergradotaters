//authors: Dustin, Parth

#include "llvm/Support/raw_os_ostream.h"
#include "Mem_Table.h"
#include <utility>

using namespace llvm_bdwy;

// TODO this is supposed to operate on a general Loc, but I only ever see it
// called on Proc_Locs... unless I'm wrong...
Mem_Block* Mem_Table::lookup_var (Proc_Loc *loc, Value *value) {
  // is it already there? return it
  // TODO comparator for pairs.
  mem_block_key key(loc, value);

  if (blocks.count(key)) {
    return blocks[key];
  } else {
    // lazily make it.

    // TODO globals won't have a procedure to which they are local...
    Mem_Block *block = new Mem_Block(value, loc->func);
    blocks[key] = block;
    return block;
  }
}

Mem_Block* Mem_Table::create_global (GlobalVariable *decl) {
  Mem_Block *block = new Mem_Block(decl, NULL);
  Global_Loc *global_loc = new Global_Loc(decl);

  bool new_def;
  block->def_at(global_loc, new_def);

  globals[decl] = block;
  return block;
}

Mem_Block* Mem_Table::lookup_global (Value *decl) {
  return globals[decl];
}

//TODO: do we need to store any other info about heap object?
Mem_Block* Mem_Table::lookup_heap_object (Stmt_Loc *loc) {
  if (heap_model.count(loc)) {
    return heap_model[loc];
  } else {
    // lazily make it. call the constructor that doesn't require a value.
    Mem_Block* mb = new Mem_Block(loc->get_function(), HEAP_TYPE);
    mb->decl = loc->expr;
    heap_model[loc] = mb;
    return mb;
  }
}

Mem_Block* Mem_Table::create_synthetic_obj (Function *f, MB_Type type) {
  Mem_Block *mb = new Mem_Block(f, type);
  // we just want to track it for debugging
  if (type == STRUCT_TYPE) {
    struct_ls.insert(mb);
  } else {
    return_ls.insert(mb);
  }
  return mb;
}

void dump_name_block_map(std::multimap<StringRef, Mem_Block*> *map) {
  for(std::multimap<StringRef,Mem_Block*>::iterator i = map->begin(), e = map->end();
      i != e;
      i++) {
    StringRef name = (*i).first;
    Mem_Block *block = (*i).second;
    log() << "* " << name << ": " << *block << "\n";
    log_push();
    block->dump_defs();
    log_pop();
  }
}

void Mem_Table::dump() {
  std::multimap<StringRef, Mem_Block*> print_map;

  log() << "The main model has " << blocks.size() << " blocks:\n";
  log_push();
  for (map_blocks_t::iterator iter = blocks.begin(); iter != blocks.end(); iter++)
  {
    Loc *at = (iter->first).first;
    Mem_Block *mb = iter->second;
    print_map.insert(std::make_pair(at->get_function()->getName(), mb));
  }
  dump_name_block_map(&print_map);
  print_map.clear();
  log_pop();

  log() << "The heap has " << heap_model.size() << " blocks:\n";
  log_push();
  for (heap_map_t::iterator iter = heap_model.begin(); iter != heap_model.end(); iter++)
  {
    Loc *at = iter->first;
    Mem_Block *mb = iter->second;
    print_map.insert(std::make_pair(at->get_function()->getName(), mb));
  }
  dump_name_block_map(&print_map);
  print_map.clear();
  log_pop();

  log() << "The struct model has " << struct_ls.size() << " blocks:\n";
  log_push();
  foreach_memblock(mb, struct_ls) {
    print_map.insert(std::make_pair((*mb)->owner_proc->getName(), *mb));
  }
  dump_name_block_map(&print_map);
  print_map.clear();
  log_pop();

  log() << "The return value model has " << return_ls.size() << " blocks:\n";
  log_push();
  foreach_memblock(mb, return_ls) {
    print_map.insert(std::make_pair((*mb)->owner_proc->getName(), *mb));
  }
  dump_name_block_map(&print_map);
  print_map.clear();
  log_pop();

  log() << "The global value model has " << globals.size() << " variables:\n";
  log_push();
  for(global_map_t::iterator i = globals.begin(), e = globals.end();
      i != e;
      i++) {
    Value* decl = (*i).first;
    Mem_Block *block = (*i).second;
    print_map.insert(std::make_pair(decl->getName(), block));
  }
  dump_name_block_map(&print_map);
  print_map.clear();
  log_pop();

  // The raw_os_ostream object apparently will buffer, so it flushes and closes
  // the ostream in the dtor when it goes out of scope... Don't call close() or
  // nothing will be written.
}

void Mem_Table::graph(raw_os_ostream &os) {
  for(map_blocks_t::iterator iter = blocks.begin(); iter != blocks.end(); iter++) {
    Loc *at = (iter->first).first;
    Mem_Block *mb = iter->second;
    mb->graph(os);
  }
}
