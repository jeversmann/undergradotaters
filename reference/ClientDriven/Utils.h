#ifndef LLVM_BDWY_UTILS
#define LLVM_BDWY_UTILS

// STL includes

#include "llvm/Support/CFG.h"
#include <vector>
#include <map>
#include <set>

// Wacky macros to make code prettier. The types for these are defined
// elsewhere.

// TODO it's annoying to deref the iterater variable at each use...

// TODO either name the 'end' things differently to avoid clobbering in nested
// loops, or hopefully it's not expensive to call end() frequently.
#define foreach_func(func, module) for (Module::iterator func = module->begin(); func != module->end(); ++func)
#define foreach_pred(pred, bb) for (pred_iterator pred = pred_begin(bb); pred != pred_end(bb); ++pred)
#define foreach_succ(succ, bb) for (succ_iterator succ = succ_begin(bb); succ != succ_end(bb); ++succ)
#define foreach_bb(bb, func) for (Function::iterator bb = func->begin(); bb != func->end(); ++bb)
#define foreach_stmt(stmt, bb) for (BasicBlock::iterator stmt = bb->begin(); stmt != bb->end(); ++stmt)

#define foreach_memblock(block, ls) for (mem_block_ls_t::iterator block = (ls).begin(); block != (ls).end(); ++block)
#define foreach_bb_in_ls(bb, ls) for (bb_ls_t::iterator bb = (ls).begin(); bb != (ls).end(); ++bb)
#define foreach_bb_in_set(bb, ls) for (bb_set_t::iterator bb = (ls).begin(); bb != (ls).end(); ++bb)
#define foreach_use(use, ls) for (mem_use_ls_t::iterator use = (ls).begin(); use != (ls).end(); ++use)

extern bool ENABLE_FI, ENABLE_CI;

// Logging stuff

#include "llvm/Support/FormattedStream.h"
using namespace llvm;

namespace llvm_bdwy {
  // indent logging to make it easier to read when our analysis is recursive or
  // hierarchial in any sense. call log() instead of ferrs().

  extern int log_level;

  static void log_push() {
    log_level++;
  }

  static void log_pop() {
    log_level--;
    if (log_level < 0) {
      // fail
      log_level = 0;
    }
  }

  static raw_ostream& log() {
    // indent
    for (int i = 0; i < log_level; i++) {
      ferrs() << "  ";
    }
    return ferrs();
  }
}
#endif
