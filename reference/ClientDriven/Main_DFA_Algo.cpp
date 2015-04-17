// authors: Dustin, Parth

// TODO meta TODO: most of these comments are obselete ;)

// TODO general tasks [claim it]:
// (functionality)
//    - see if phis are placed when we make changes inside a proc
//    - fix and analyze tests
//    - phi placement/evaling has been changed from cbz... stress-test it.
//    - off-by-one deref levels for arg passing and ptr copying... hackish solns.
//
//    - array handling
//    - handle globals
// (CI)
//  - reanalyze_at from cbz is... never called.
//  - ci_loc, the magic master CI proc_loc, doesnt follow cbz's way..
//
//(FI)
//  - make a block tell its OrderedDefs that it's FI...
//
// (testing)
//    *** let's do constant prop.
//    - write and run tests
//    - [kurt?] Mem_Table->dump() could be wicked cool with graphviz
// (later)
//    - support backwards analyses too
// (small, i hope)
//    - pretty sure the is_new_def is just an optim. we can remove
//    - make sure changes are marked in deref
//    - make sure dominance testing is correct
//    - when we do ptr copies, dont gen_uses? or solve the common ancestor
//    thingy

#include "llvm/Support/CommandLine.h"
#include "llvm/Pass.h"
#include "llvm/Module.h"
#include "llvm/Instructions.h"
#include "model/ProcedureDB.h"
#include "model/Location.h"
#include "model/Mem_Table.h"
#include "ClientProblem.h"
#include "ConstantAnalysis.h"
#include "SplitPass.h"
#include "Utils.h"
#include "llvm/Analysis/MemoryBuiltins.h"
#include "llvm/Constants.h"
#include "llvm/Analysis/ValueTracking.h"
#include "llvm/Target/TargetData.h"
#include "llvm/Support/GetElementPtrTypeIterator.h"
#include "llvm/Analysis/CallGraph.h"
#include "AnnotationPass/AnnotationPass.h"

#define GEN_USES true
#define DONT_GEN_USES false

using namespace llvm;

// command line to toggle FI and CI
static cl::opt<bool> ENABLE_FI_flag("use-fi", cl::desc("Use Flow Insensitive pass 1"));
static cl::opt<bool> ENABLE_CI_flag("use-ci", cl::desc("Use Context Insensitive pass 1"));

bool ENABLE_FI = false;
bool ENABLE_CI = false;

namespace llvm_bdwy{
  
  // yay externs.
  int log_level = 0;

  
  struct Main_DFA_Algo : public ModulePass {
    static char ID;
    ProcedureDB procedures;
    Mem_Table memory;
    mem_block_ls_t debug_last_changes;  // debug
    ClientProblem *client;

    Main_DFA_Algo () : ModulePass(ID) {}

    virtual void getAnalysisUsage(AnalysisUsage &AU) const {
      AU.addRequired<SplitPass>();
      AU.addRequired<CallGraph>();
      // we don't transform
      AU.setPreservesAll();
    }

    virtual bool runOnModule (Module &m) {

      AnnotationPass ap;
      ap.runOnModule(m);

      if (ap.ready) {
        client = &ap.propAnalyses[0];
      } else {
        //client = new ClientProblem(); // an empty one that doesn't do anything
        client = new ConstantAnalysis(memory);      
      }

      // Share some singletons.
      Loc::proc_db = &procedures;
      Loc::memory_model = &memory;

      ENABLE_CI = ENABLE_CI_flag;
      ENABLE_FI = ENABLE_FI_flag;

      if (ENABLE_FI) {
        log() << "Flow Insensitive mode!\n";
      }
      if (ENABLE_CI) {
        log() << "Context Insensitive mode!\n";
      }

      // Start the locations with root...
      Function *root = m.getFunction("main");
      if (root == NULL) {
        log() << "Can't find main()\n";
        return false;
      }
      // even though pass 1 is CI, root has to be CS or we don't even have
      // BB_Locs... this is fine, nothing ever calls main hopefully...
      Proc_Loc *root_loc = new Proc_Loc(NULL, root, true);

      // set up worklists and such
      procedures.initialize(&m, root, getAnalysis<CallGraph>());

      // TODO mark all procedures for reanalysis. not 100% sure how all of that
      // gets called, so not adding it in just yet.

      // TODO apparently this is done until reanalysis is not required. but I'm
      // not sure why.

      // I don't think we use any of these, but analyze_proc always needs them.
      mem_block_ls_t extern_defs, extern_changes, mains_result;
      mem_use_ls_t extern_uses;

      setup_initial_globals(m);

      // since we start out in [CI], may have to actually reanalyze root? does
      // that every happen though?
      while (1) {
        analyze_proc(root_loc, extern_defs, extern_uses, extern_changes, mains_result);
        if (!procedures.is_reanalysis_required(root) || !ENABLE_CI) {
          break;
        }
      }

      log() << "\nConvergence!\n\n";
      memory.dump();
      log() << "\nClient's results...\n\n";
      client->analysis_done();

      // TODO update all d-u chains in the memory model. dunno why.

      graph(root_loc);

      return false;
    }

    void setup_initial_globals(Module& m) {
      for(Module::global_iterator i = m.global_begin(), e = m.global_end();
          i != e;
          i++) {
        log() << "Initializing global: " << *i;
        memory.create_global(&*i);
      }
    }

    // a special C.I. version somehow
    Mem_Def* ci_nearest_def_at (Mem_Block *block, Loc *loc) {
      // huzzah for naming scheme.
      log() << "seeking def for " << *block << "\n";
      log() << "location tree is: \n";
      loc->dump();
      //log() << "defs of block are\n";
      //block->dump_defs();
      //log() << "grabbing nearest def\n";
      Mem_Def *def = block->nearest_def_at(loc);
      //log() << "time to move up the stack\n";

      // move up call-stack until we find a reaching def if we didn't already
      ProcedureInfo *info = procedures.lookup(loc->get_function());
      while (!def && info->current_callsite) {
        // stop early if the block is local to this procedure
        if (block->owner_proc == info->proc) {
          return NULL;
        }

        // is there a def that reaches this callsite?
        def = block->nearest_def_at(info->current_callsite);
        // move up the call stack
        info = info->current_caller();
      }

      return def;
    }

    void graph(Proc_Loc *proc_loc)
    {
      std::filebuf fb;
      fb.open("graph.dot", std::ios::out);
      std::ostream output_graph(&fb);
      raw_os_ostream gout(output_graph);
      gout << "digraph G {\n";
      gout << "  node [shape=\"record\"];\n";
      gout << "  rankdir = \"LR\"\n";
      proc_loc->graph(gout);
      memory.graph(gout);
      gout << "}\n";
    }

    void analyze_proc (Proc_Loc *proc_loc, mem_block_ls_t &extern_defs,
                       mem_use_ls_t &extern_uses, mem_block_ls_t &extern_changes,
                       mem_block_ls_t &return_val)
    {
      log() << "Analyzing " << proc_loc->func->getName() << "()\n";
      log_push();

      ProcedureInfo *info = procedures.lookup(proc_loc->func);

      client->at_proc_entry(proc_loc, info);

      // collect defs and uses, so we can figure out external stuff
      mem_block_ls_t defs;
      mem_use_ls_t uses;
      // because the naming scheme sucks: the external versions are only used in
      // process_changes and stuff

      // I foresee issues with initializing to all blocks... one proc info, even
      // when C.S... hmm. recursion?
      info->add_all_blocks();

      // this is just simple IDFA.
      while (!info->worklist_empty()) {
        BasicBlock *bb = info->shift_worklist();
        BB_Loc *bb_loc = proc_loc->lookup_block(bb);
        log() << "------------------------ " << *bb_loc << " --------------------------\n";

        // accumulate the changes we observe
        // TODO i think it would be convenient to store lots of this as state of
        // this pass object
        mem_block_ls_t changes;

        client->at_bb_entry(bb_loc);

        // eval_phis() is just this:
        // merge in any information from other places that affects us
        // (if we're forward)
        foreach_memblock(block, bb_loc->phis) {
          log() << "Merging " << **block << "\n";
          // screw their weird old way.
          log_push();
          eval_phi(bb_loc, *block, defs, uses, changes);
          log_pop();

          client->at_merge(bb_loc, *block);
        }
        if (bb_loc->phis.size()) {
          log() << "Merged " << bb_loc->phis.size() << " phis\n\n";
        }

        Stmt_Loc *cur_stmt;

        // Visit each statement and record changes
        for (BasicBlock::iterator i = bb->begin(), e = bb->end(); i != e; ++i) {
          cur_stmt = bb_loc->lookup_stmt(i);
          // TODO consider a struct with all these common things to pass around.
          handle_statement(cur_stmt, changes, return_val, defs, uses);
        }

        client->at_bb_exit(bb_loc);

        // did anything happen?
        if (!changes.empty()) {
          // set up merge points
          log() << "\n";
          place_phis(bb_loc, changes);
          // If we were called from somewhere, place PHIs
          if(bb_loc->parent->parent) {
            place_callsite_phis(bb_loc->parent->parent, changes);
          }

          // manage external changes
          process_extern_changes(bb_loc, changes, extern_changes);

          int new_blocks = info->add_reachable_blocks(bb, NULL);

          log() << changes.size() << " changes, thus adding " << new_blocks
                << " BBs reachable from '" << bb->getName() << "'\n";

          // TODO er... do we reset changes?
        } else {
          log() << "\n";
        }
      }

      // [CI]
      record_extern_io(proc_loc, defs, extern_defs, uses, extern_uses);

      client->at_proc_exit(proc_loc, info, return_val);

      log_pop();
    }

    // this generates uses and computes reaching defs... pretty vital.
    void generate_uses (Loc *at, Ptr_Val *pval, mem_use_ls_t &uses) {
      if (pval->is_addr) {
        return;
      }

      // TODO maybe we take a PtrVal and ignore if stuff is_address. gotta check
      // the callers.

      foreach_memblock(mb, *(pval->ls)) {
        //log() << "making uses of " << **mb << " at " << *at << "\n";
        // find or create a use
        Mem_Use *use = (*mb)->use_at(at);
        uses.push_back(use);
        // Apparently this is a C.I. thing? reaching def could be in another
        // context, sure. so look for a non-local, possibly.
        Mem_Def *def = ci_nearest_def_at(*mb, at);
        use->reaching_def = def;
        log() << "(Used " << **mb << " at " << *at << ")\n";
        // TODO tmp
        if (!def) {
          log() << "... use something without an rdef!\n";
        }
      }
    }

    void process_extern_changes (BB_Loc *bb_loc, mem_block_ls_t& changes,
                                 mem_block_ls_t &extern_changes)
    {
      Function *caller = bb_loc->parent->func;

      foreach_memblock(memblock, changes) {
        if (procedures.is_in_scope(caller, *memblock)) {
          extern_changes.insert(*memblock);
        }
      }
    }

    void place_callsite_phis (Stmt_Loc* callsite, mem_block_ls_t& changes) {
      foreach_succ(succ, callsite->parent->bb) {
        BB_Loc* next_bbloc = callsite->parent->parent->lookup_block(*succ);

        foreach_memblock(memblock, changes) {
          Mem_Def *reaching_def = (*memblock)->current_def;
          if (reaching_def) {
            log() << "Placing callsite phi for " << **memblock << " with pts-to: "
                  << *(reaching_def->points_to) << "\n";
            next_bbloc->phis.insert(*memblock);
          } else {
            log() << "TODO: Check correctness -- Anyone using this here means we have a bug elsewhere, if this lacks an rdef prior to this. (globals have a default value, so impossible for this to be the first for anything)\n";
          }
        }
      }
    }

    // Follows the thesis much more than cbz.
    void place_phis (BB_Loc *bb_loc, mem_block_ls_t& changes) {
      // find where to place merge points: the dominance frontier
      // TODO pg 39 of the thesis: if a change occurs at the exit BB, need to
      // insert phis in the dom frontier of the call-site. we're, uh, not
      // doing that yet. look into proc db's "callers"?
      Proc_Loc *proc_loc = bb_loc->parent;
      bb_set_t *frontier = procedures.get_dom_frontier(proc_loc->func, bb_loc->bb);
      if (frontier->size() == 0) {
        // eh, what's the point?
        return;
      }

      log() << "Placing phis at " << frontier->size() << " BBs...\n";

      foreach_memblock(memblock, changes) {
        // TODO handle external changes. is this C.I. only?

        // find the reaching def. if it was inside a called proecedure, the
        // "current def" will be null and we have to find it properly.
        Mem_Def *reaching_def = (*memblock)->current_def;
        if (!reaching_def) {
          log() << "WARNING never seen this before, just curious\n";
          reaching_def = (*memblock)->last_def_at(bb_loc);
        }
        log() << "Placing phi for " << **memblock << " with pts-to: "
              << *(reaching_def->points_to) << "\n";

        foreach_bb_in_set(merge_bb, *frontier) {
          BB_Loc *merge_loc = proc_loc->lookup_block(*merge_bb);
          // screw that weird stuff they were doing.
          merge_loc->phis.insert(*memblock);
        }
      }
    }

    // merge for one Mem_Block
    void eval_phi (BB_Loc *bb_loc, Mem_Block *block, mem_block_ls_t &defs, mem_use_ls_t &uses,
                   mem_block_ls_t &changes)
    {
      // find or make the def of this block that will merge the changes
      bool is_new_def = false;
      Mem_Def *merge_def = block->def_at(bb_loc, is_new_def);
      if (is_new_def) {
        changes.insert(block);
      }
      defs.insert(block);

      // copy it so comparison works
      mem_block_ls_t *prev = new mem_block_ls_t(*(merge_def->points_to));

      // start over
      if (!ENABLE_FI) { //don't clear if FI
        //TODO: should be checking based upon each block; not upon global
        //ENABLE_FI
        merge_def->clear_points_to();
      }

      // so what def(s) of this block do we look at? one option is to just store
      // that in the list of phis. but isn't in the def that dominates each of
      // our predecessors?
      foreach_pred(pred, bb_loc->bb) {
        BB_Loc *pred_loc = bb_loc->parent->lookup_block(*pred);
        // TODO [CI] we would normally insert a use into uses here, but we're no
        // longer doing things their way. maybe find the use whose rdef is this?
        Mem_Def *rdef = block->last_def_at(pred_loc);
        if (rdef) {
          log() << "rdef of a pred " << (*pred)->getName() << " contains: "
                << *(rdef->points_to) << "\n";
          union_memblock_ls(merge_def->points_to, rdef->points_to);
        } else {
          log() << "pred lacks an rdef!\n";
        }
      }

      if (!equal_memblock_ls(merge_def->points_to, prev)) {
        changes.insert(block);
      }

      // TODO if not computing pointers, do what?
    }

    // Just delegate? The visitor class could help here, if we can send our
    // args.
    void handle_statement (Stmt_Loc *stmt_loc, mem_block_ls_t& changes,
                           mem_block_ls_t &return_val, mem_block_ls_t &defs,
                           mem_use_ls_t &uses)
    {
      // in cbz, they immediately go to eval_expr() here. but our IR isn't
      // quite like that, so...

      // TODO cbz is structured a bit differently; i don't think there's other
      // useful parameters?
      client->at_stmt_entry(stmt_loc);

      // this may be very dependent on the IR we're experiment with, which is
      // produced using clang -O0
      switch (stmt_loc->expr->getOpcode()) {
        // most of the action happens here
        case Instruction::Store:    handle_store(stmt_loc, changes, return_val, defs, uses); break;
        // evaluate calls immediately, use their results later from a STORE?
        case Instruction::Call:     handle_call(stmt_loc, changes, return_val,
                                                defs, uses);
                                    break;
        // just copy into the return val
        case Instruction::Ret:      handle_return(stmt_loc, return_val, defs, uses); break;


        case Instruction::PHI:
          // TODO clang isn't generating these yet.
          break;
        case Instruction::IntToPtr:
          break;

        // something's on the stack. don't do anything here, let another instr
        // that uses this evaluate it.
        case Instruction::Alloca:   break;
        // TODO will we ever load something and use it a way that isn't
        // store/return/call/etc?
        case Instruction::Load:           break;
        case Instruction::BitCast:        break;
        case Instruction::GetElementPtr:  break;
        // There's some stuff that we should be able to safely ignore...
        // control flow stuff, arithmetic assumed to be a no-op due to the array
        // model...
        case Instruction::Br:       break;
        case Instruction::ICmp:     break;
        case Instruction::Add:      break;

        // What's been forgotten?
        default:
          log() << "TODO: handle " << *(stmt_loc->expr) << "?\n";
          break;
      }

      client->at_stmt_exit(stmt_loc);
    }

    void handle_return (Stmt_Loc *stmt_loc, mem_block_ls_t &return_val, mem_block_ls_t &defs,
                        mem_use_ls_t &uses)
    {
      Value *returned = (cast<ReturnInst>(stmt_loc->expr))->getReturnValue();
      if (returned) {
        log() << "RETURN " << *returned << "\n";
        union_memblock_ls(&return_val, eval_expr(returned, stmt_loc, defs, uses, GEN_USES)->ls);
        log() << "Return vals so far: " << return_val << "\n";
      }
      client->at_return(stmt_loc, &return_val);
      log() << "\n";
    }

    void handle_store (Stmt_Loc *stmt_loc, mem_block_ls_t& changes,
                       mem_block_ls_t &return_val, mem_block_ls_t &defs, mem_use_ls_t &uses)
    {
      StoreInst *si = (StoreInst*) stmt_loc->expr;

      Value *src = si->getValueOperand();
      Value *dst = si->getPointerOperand();

      log() << "STORE " << *si << "\n";
      log_push();

      // it looks like taking an address always happens when the rhs is an
      // ALLOCA.
      // and dereferences happen on the lhs or rhs at a load. (they can be
      // handled the same whether on lhs or rhs). They should be chased until an
      // alloca.
      // so does this do the right thing in both cases?

      Ptr_Val *lhs = eval_expr(dst, stmt_loc, defs, uses, DONT_GEN_USES);
      Ptr_Val *rhs = eval_expr(src, stmt_loc, defs, uses, GEN_USES);

      log() << "Dest (addr? " << lhs->is_addr << "): " << *(lhs->ls) << "\n";
      log() << "Source (addr? " << rhs->is_addr << "): " << *(rhs->ls) << "\n";

      // TODO this is maybe a hack too.
      // It's hard to distinguish pointer copies 'ptr1 = ptr2' from things like
      // '*ptr = value'. Go go heuristics!
      // TODO naturally, we now need to test '*ptr1 = *ptr2' where both are
      // pointers to pointers, and various combinations.
      if (isa<PointerType>(*(src->getType()))) {
        // so basically we have normal LHS vars = some rhs things that aren't
        // marked as addresses.
        // so... mark them as addresses so handle_assign doesn't chase points_to
        // sets another level.
        log() << "Pointer copy detected.\n";
        rhs->is_addr = true;
      }

      // no additive
      handle_assign(stmt_loc, lhs->ls, rhs, defs, uses, changes, false);

      log() << "Calling client->at_assignment with rhs of " << *(rhs->ls) << "\n";
      client->at_assignment(stmt_loc, lhs->ls, rhs, src);

      // do a bit of extra work to easily see new changes
      mem_block_ls_t new_changes;
      foreach_memblock(this_time, changes) {
        if (debug_last_changes.count(*this_time) == 0) {
          new_changes.insert(*this_time);
          //log() << "resultant defs of " << **this_time << "\n";
          //(*this_time)->dump_defs();
        }
      }
      log() << "New changes: " << new_changes << " (" << changes.size() << " total)\n";
      debug_last_changes.clear();
      union_memblock_ls(&debug_last_changes, &changes);

      log_pop();
      log() <<"\n";
    }

    void handle_assign (Loc *loc, mem_block_ls_t *lhs, Ptr_Val *rhs, mem_block_ls_t &defs,
                        mem_use_ls_t &uses, mem_block_ls_t& changes, bool additive)
    {
      // In LLVM as in CBZ, pretty much everything is like an assignment. Thus:
      // 1) eval RHS: generate Mem_Uses, find reaching defs, collect points-to
      //    sets.
      // 2) eval LHS: generate Mem_Defs, tag with stmt_loc, transferring sets

      if (lhs->size() == 0) {
        log() << "WARNING no destinations for a store\n";
      }
      bool unique_lhs = lhs->size() == 1;

      foreach_memblock(left, *lhs) {
        // lookup or make the def
        bool is_new;
        Mem_Def *left_def = (*left)->def_at(loc, is_new);
        defs.insert(*left);

        // easy change to detect
        if (is_new) {
          changes.insert(*left);
        }

        // remember the last value just to see if we've changed. copy it.
        mem_block_ls_t *prev_pts = new mem_block_ls_t(*(left_def->points_to));

        // strong or weak update? lhs must have one thing and it can't be a MB
        // with "multiplicity" that represents multiple runtime things (heap
        // stuff, arrays)
        bool strong_update = unique_lhs && (*left)->can_be_strong_update();

        if (strong_update) {
          // start over.
          if (!ENABLE_FI) { // don't clear the points to set when doing FI!!
            //TODO: should be checking based upon each block; not upon global
            //ENABLE_FI
            left_def->clear_points_to();
          }
        } else {
          // weak updates have to accumulate points-to info, not replace
          // ensure we have a previous def (even if CI)
          Mem_Def *prev_def = ci_nearest_def_at(*left, loc);
          if (prev_def) {
            left_def->add_pointers(prev_def->points_to);
          }

          left_def->is_weak = true;
          // left_def->set_prev(prev_dev)

          // create a Use here, to make certain client analyses work.
          Mem_Use *use = (*left)->current_use;
          if (!use) {
            use = (*left)->use_at(loc);
          }
          uses.push_back(use);
          use->reaching_def = prev_def;
          use->is_weak = true;
        }

        // include previous values, aka don't clear. used in CI settings usually
        if (additive) {
          left_def->add_pointers(prev_pts);
          // TODO set additive on def, if we care
        }

        if (rhs->is_addr) {
          left_def->add_pointers(rhs->ls);
        } else {
          // gotta merge points-to sets for the reaching defs
          foreach_memblock(right, *(rhs->ls)) {
            // the reaching def should already exist from generate_uses
            Mem_Use *right_use = (*right)->current_use;
            if (!right_use) {
              right_use = (*right)->use_at(loc);
              // won't have a reaching def...
            }

            Mem_Def *reaching_def = right_use->reaching_def;
            if (reaching_def) {
              left_def->add_pointers(reaching_def->points_to);
            } else {
              // TODO is this ok?
              log() << "something lacks a reaching def...\n";
            }
          }
        }

        if (!equal_memblock_ls(left_def->points_to, prev_pts)) {
          changes.insert(*left);
        }
      }
    }

    // and by arithmetic, we just mean a binaryish op that we should examine the
    // operands of
    bool is_arithmetic (Value *val) {
      Instruction *instr = dyn_cast<Instruction>(val);
      if (!instr) {
        return false;
      }
      
      switch (instr->getOpcode()) {
        case Instruction::Add:
        case Instruction::Sub:
        case Instruction::Mul:
        case Instruction::SDiv:
        case Instruction::And:
        case Instruction::Or:
        case Instruction::Xor:
          return true;
        default:
          return false;
      }
    }

    // TODO maybe always generate_uses() on the results?
    Ptr_Val* eval_expr (Value *instr, Stmt_Loc *context, mem_block_ls_t &defs, mem_use_ls_t &uses,
                        bool gen_uses)
    {
      log() << "Evaluating expression: " << *instr << " in " << *context << "\n";
      log_push();
      // The context is where the expression is used. instr as a Value will be
      // where it was defined.
      Ptr_Val *result = new Ptr_Val(new mem_block_ls_t());

      if (isa<AllocaInst>(instr) || isa<GlobalVariable>(instr)) {
        // the thesis calls these "identifiers."

        Mem_Block *block;
        if(isa<AllocaInst>(instr)) {
          // why look up by the procedure?
          block = memory.lookup_var(context->parent->parent, instr);
        } else {
          block = memory.lookup_global(instr);
        }

        // this is us taking an address.
        result->is_addr = true;
        // return a set containing just this one block
        result->ls->insert(block);

        if (gen_uses) {
          generate_uses(context, result, uses);
        }

        client->at_addr_taken(context, block, gen_uses);

        log_pop();
        return result;
      } else if (LoadInst *li = dyn_cast<LoadInst>(instr)) {
        // a dereference. thesis calls this "star_operator".

        // chase the thing we're loading until we hit a concrete value (I think
        // always an alloca or similar) and count the levels. Then look up that
        // block and follow/expand its points-to sets that many times.
        int lvls = 0;
        Value *current = li;
        while (isa<LoadInst>(current)) {
          lvls++;
          current = (cast<LoadInst>(current))->getPointerOperand();
        }
        // TODO i think only an alloca should be the base case... or GEP
        if (!isa<AllocaInst>(current) && !isa<GetElementPtrInst>(current)) {
          log() << "what's at the bottom of a load-chain? " << *current << "\n";
        }

        //log() << "dereffing " << lvls << " of " << *current << "\n";
        result->ls = dereference(current, context, defs, uses, lvls);

        if (gen_uses) {
          generate_uses(context, result, uses);
        }

        client->at_dereference(context, result->ls, gen_uses);

        log_pop();
        return result;
      } else if (BitCastInst *bi = dyn_cast<BitCastInst>(instr)) {
        // we just want to detect malloc, mostly.
        if (isMalloc(bi)) {
          result->ls->insert(memory.lookup_heap_object(context));

          if (gen_uses) {
            generate_uses(context, result, uses);
          }
          log_pop();
          return result;
        } else {
          // TODO make a test that produces these
          log() << "bitcast without malloc?\n";
          log_pop();
          return result;  // empty
        }
      } else if (GetElementPtrInst *gepi = dyn_cast<GetElementPtrInst>(instr)) {
        // struct access. this is originally "dot_operator."
        // follow containment links lazily.

        log() << "handling GEP " << *gepi << "\n";

        // TODO actually, sometimes i'm seeing one index. always use the last
        // index is our rule?

        // GEPs encompass both array and struct accesses. According to the docs,
        // it could have any number of 'indices', but I only ever observe 1 or
        // 2, and all but the last are a no-op (always accesses the first
        // PointerOperand thing). So if you do foo->field[4].field, it gets
        // split into a sequence of GEPs, and eval_expr()ing the pointer operand
        // always does the right thing.

        if (gepi->getNumIndices() > 2) {
          log() << "^ this GEP has lots of indices\n";
        }

        // TODO Why generate uses? CBZ seems to.
        log_push();
        mem_block_ls_t *operand = (eval_expr(
          gepi->getPointerOperand(), context, defs, uses, GEN_USES)
        )->ls;
        log_pop();

        // the last index seems to be the useful one
        User::op_iterator idx_iter = gepi->idx_begin();
        gep_type_iterator gep_type = gep_type_begin(gepi);

        // TODO I can't do gep_type_end() - 1? do it the dumb way.
        for (uint i = 1; i < gepi->getNumIndices(); ++i) {
          idx_iter++;
          gep_type++;
        }

        Value *idx = *idx_iter;
        if (isa<StructType>(*gep_type)) {
          // struct access
          foreach_memblock(mb, *operand) {
            // this also constitutes a use, even if we're in between? alright.
            Mem_Use *use = (*mb)->current_use;
            if (!use) {
              log() << "struct deref use wont have a reaching def!\n";
              use = (*mb)->use_at(context);
            }
            uses.push_back(use);

            // lazily make the field and accumulate results
            Mem_Block *field = (*mb)->struct_access(idx, *mb);

            result->ls->insert(field);

            client->at_struct_access(context, *mb, field);
          }
        } else {
          // array access
          // assume these are no-ops.
          foreach_memblock(mb, *operand) {
            // TODO bad!
            if (!(*mb)->is_array_or_heap()) {
              log() << "GEP array on something we havent marked as such\n";
            }
            client->at_array_access(context, *mb);
            result->ls->insert(*mb);
          }
          // just copy.
        }

        if (gen_uses) {
          generate_uses(context, result, uses);
        }
        log_pop();
        return result;
      } else if (CallInst *calli = dyn_cast<CallInst>(instr)) {
        log() << "Using return val from a call to " << calli->getCalledFunction()->getName() << "\n";

        // make a use of the return value that we should have just made a def
        // for in the previous instruction
        Mem_Block *ret = procedures.get_return(calli->getCalledFunction());
        mem_block_ls_t tmp_ls;
        tmp_ls.insert(ret);
        generate_uses(context, new Ptr_Val(&tmp_ls), uses);
        // once we determine generate_uses DEFINITELY sets current_use, then we
        // can take out the paranoia.
        Mem_Use *use = ret->current_use;
        if (!use) {
          log() << "Making a use that we won't have a reaching def for!\n";
          use = ret->use_at(context);
        }

        // we've already done the work in handle_call(), just pass it on
        Mem_Def *def = use->reaching_def;
        union_memblock_ls(result->ls, def->points_to);

        if (gen_uses) {
          generate_uses(context, result, uses);
        }
        log_pop();
        return result;
      } else if (PHINode *phi = dyn_cast<PHINode>(instr)) {
        // straightforward... merge possibilities. reg2mem pass is the
        // alternative, but it's hard and it's late.
        bool is_addr = false;
        for (PHINode::block_iterator bb = phi->block_begin(); bb != phi->block_end(); ++bb) {
          Value *choice = phi->getIncomingValueForBlock(*bb);
          Ptr_Val *val = eval_expr(choice, context, defs, uses, GEN_USES);
          if (is_addr != val->is_addr) {
            if (bb == phi->block_begin()) {
              is_addr = val->is_addr;
            } else {
              // these better all be addresses? TODO
              log() << "phi gave some addresses, some pts-to sets!\n";
            }
          }
          union_memblock_ls(result->ls, val->ls);
          result->is_addr = is_addr;
        }

        log() << "LLVM phi evaluated to " << *(result->ls) << "\n";

        if (gen_uses) {
          generate_uses(context, result, uses);
        }
        log_pop();
        return result;
      } else if (is_arithmetic(instr)) {
        // arithmetic on expressions should actually just be actual computation
        // in the llvm model. array acceses show up more accurately as GEPs.
        log() << "Evaluating operands of expr " << *instr << "\n";
        log_push();

        // just eval all the operands to generate uses or whatever, and return
        // empty.
        Instruction *instr_ops = dyn_cast<Instruction>(instr);
        for (User::op_iterator oper = instr_ops->op_begin(); oper != instr_ops->op_end(); ++oper)
        {
          eval_expr(*oper, context, defs, uses, gen_uses);
        }

        log_pop();
        log_pop();
        return result;  // empty
      } else if (isa<Constant>(instr)) {
        // TODO is this too broad?
        log_pop();
        return result;  // empty
      } else if (isa<Argument>(instr)) {
        // we'll have a def on this Mem_Block at our callsite
        Mem_Block *block = memory.lookup_var(context->parent->parent, instr);
        // par->par is our proc_loc, it's parent is our callsite
        Loc *callsite = context->parent->parent->parent;
        // TODO or should we use ci_nearest_def_at or another? specifically
        // looking up our callsite makes sense to me.
        Mem_Def *def = block->find_def_at(callsite);

        union_memblock_ls(result->ls, def->points_to);
        log() << "Recovered argument " << *instr << " that points to " << *(result->ls) << "\n";

        if (gen_uses) {
          // TODO not sure this makes sense. it's like a pointer copy.
          //generate_uses(context, result, uses);
        }
        log_pop();
        return result;
      } else {
        // TODO many things probably just return empty...
        log() << "WEIRD EXPR " << *instr << "\n";
        log_pop();
        return result;  // empty
      }
    }

    mem_block_ls_t* dereference (Value *base, Stmt_Loc *context, mem_block_ls_t &defs,
                                 mem_use_ls_t &uses, int lvls)
    {
      // For each level, find the reaching defs and follow points-to edges

      mem_block_ls_t *current = eval_expr(base, context, defs, uses, GEN_USES)->ls;
      mem_block_ls_t *next = new mem_block_ls_t();

      for (int i = 0; i < lvls; ++i) {
        // this isn't in CBZ, but it makes sense that we're using these blocks
        // here, and we certainly do need to have their rdefs!
        generate_uses(context, new Ptr_Val(current), uses);

        foreach_memblock(mb, *current) {
          // this should have the side effect of looking up the reaching def, i
          // think
          Mem_Use *use = (*mb)->current_use;
          if (!use) {
            log() << "Making a use that we won't have a reaching def for!\n";
            use = (*mb)->use_at(context);
          }
          uses.push_back(use);

          Mem_Def *def = use->reaching_def;

          // TODO their code actually handles the case where there was no def.
          // i... don't quite trust that, though.
          if (def) {
            // merge in the possibilities
            union_memblock_ls(next, def->points_to);
          } else {
            log() << "WARNING no reaching def for a use in deref\n";
            // this shows up when you do things like scanf("blah", input)... the
            // IR dereferences input. shouldn't it be taking an address?
          }
        }

        // swap
        mem_block_ls_t *tmp = current;
        current = next;
        next = tmp;
        next->clear();
      }

      return current;
    }

    // TODO fill these out
    bool ignore_function (Function *f) {
      // va_start, va_end, delete, malloc
      return false;
    }

    void handle_call (Stmt_Loc *stmt_loc, mem_block_ls_t& changes,
                      mem_block_ls_t &return_val, mem_block_ls_t &defs,
                      mem_use_ls_t &uses)
    {
      CallInst *ci = (CallInst*) stmt_loc->expr;

      // eval() all the actuals/args.
      std::vector<mem_block_ls_t*> args;
      for (uint i = 0; i < ci->getNumArgOperands(); ++i) {
        Value *actual = ci->getArgOperand(i);
        Ptr_Val *result = eval_expr(actual, stmt_loc, defs, uses, GEN_USES);
        args.push_back(result->ls);
      }

      // Are we in a special case?
      bool be_conservative = false;
      Function* called_fxn = ci->getCalledFunction();
      if (called_fxn) {
        log() << "Analyzing a call to " << called_fxn->getName() << "\n";

        // seriously, don't do anything
        if (ignore_function(called_fxn)) {
          return;
        }

        // alright, now see if it's recursive
        if (procedures.is_current_call_recursive(called_fxn, stmt_loc->get_function())) {
          be_conservative = true;
        }
      } else {
        // TODO what to do, and what to do if there's more than one possible
        // called fxn?
        log() << "Indirect function call!\n";
        be_conservative = true; // TODO only if multiple targets
      }

      if (be_conservative) {
        // TODO assume all reachable block could be R/W... whoa.
        // conservative_proc_call()
        log() << "WARNING TODO conservative proc call stuff\n";
        return;
      }

      // Set up the context-sensitive location... well, maybe.
      bool is_cs = procedures.lookup(called_fxn)->is_context_sensitive;

      // this also sets the current caller/callsite
      Proc_Loc *callee_path = procedures.setup_call(stmt_loc, called_fxn);

      bool reanalysis_required = procedures.is_reanalysis_required(called_fxn);
      // TODO different for backwards-flowing, as usual

      mem_block_ls_t changed_inputs;

      log_push();

      // handle parameters, both actual and external
      pass_parameters(args, callee_path, changed_inputs, return_val, is_cs);
      // now the [CI] one.
      if (!is_cs) {
        pass_external_inputs(called_fxn, uses, changed_inputs);
      }

      // do we have any work to do? (C.I. only)
      bool skip_analysis = !is_cs && !reanalysis_required && changed_inputs.empty();

      if (skip_analysis) {
        log() << "WARNING TODO skipping analysis of a proc call\n";
        // still need the return value and all this other stuff
        // TODO
      } else {
        // diverging from cbz; I think this is an IR difference again.
        // whatever the return value from this analysis is, we may need to use
        // it in the very next instruction (x = foo()). we can call it a
        // mem_block with a def established here, then use it in the next line
        // to grab the result.

        Mem_Block *return_from_proc = procedures.get_return(called_fxn);
        bool new_return_def;
        // TODO how to do this [CI]ly?
        Mem_Def *return_def = return_from_proc->def_at(stmt_loc, new_return_def);

        // TODO ehh really we should call return_def->add_pointers() in there,
        // but this works...

        if (called_fxn->isDeclaration()) {
          // time to talk to broadway! TODO
          log() << "TODO ask bdwy for what happens here\n";

          // and fire a callback too
          client->at_lib_call(callee_path, args, return_def);

          log_pop();
          return;
        } else {
          analyze_proc(callee_path, defs, uses, changes, *(return_def->points_to));
          client->at_call(callee_path, args, changes, return_def);
        }

        // we'll make a use of this def if anything cares about the return value
        // from this call.

        // [CI] make external output visible
        if (!is_cs) {
          pass_external_outputs(called_fxn, defs, changes);
        }
      }

      // reset the current callsite
      procedures.return_from_call(called_fxn);

      log_pop();
    }

    void pass_parameters (std::vector<mem_block_ls_t*> &actuals, Proc_Loc *call_to,
                          mem_block_ls_t &changed_inputs, mem_block_ls_t &return_val, bool is_cs)
    {
      Function *f = call_to->func;

      // we don't care about these, ignore them, they're fake
      mem_block_ls_t defs;
      mem_use_ls_t uses;

      if (f->isVarArg()) {
        // TODO cbz uses an 'ellipsis object' that interacts with va_* calls...
        log() << "var args, uh oh\n";
        return;
      }

      // reuse this
      Ptr_Val *rhs_in = new Ptr_Val(NULL);
      // TODO I'm not sure why, but this makes examples run. Otherwise,
      // handle_assign chases points-to sets another level, when it shouldn't.
      rhs_in->is_addr = true;

      // conceptually, assign each actual to the corresponding formal. ignore
      // defs and uses, but record if any inputs are different.
      int idx = 0;
      for (Function::arg_iterator formal = f->arg_begin(); formal != f->arg_end(); ++formal)
      {
        mem_block_ls_t *actual = actuals[idx];
        rhs_in->ls = actual;
        idx++;

        // find the formal
        Mem_Block *left = memory.lookup_var(call_to, formal);
        // just make a set for it
        mem_block_ls_t lhs;
        lhs.insert(left);

        log() << "Assigning formal " << *left << " to actual " << *actual << "\n";

        // apparently CRITICAL: "current path" must be in context of the
        // procedure being called (our 'call_to'), and we're assuming no arg is
        // an address-of?

        // don't clear points-to sets if we're CI
        bool additive = !is_cs;

        // TODO does this really do what we want? :P
        // TODO also, are we passing in the correct Stmt_Loc, our caller?
        handle_assign(call_to->parent, &lhs, rhs_in, defs, uses, changed_inputs, additive);
      }
      log() << "\n";
    }

    // [CI] magic below.

    // visit all external inputs to f, merge reaching def from calling context,
    // make one def that reaches all uses inside f.
    void pass_external_inputs (Function *f, mem_use_ls_t &extern_uses, mem_block_ls_t &changes)
    {
      log() << "Passing external input for call to " << f->getName() << "\n";
      ProcedureInfo *info = procedures.lookup(f);
      Stmt_Loc *current_callsite = info->current_callsite;

      // put the merging def at the beginning of the first BB
      BB_Loc *merge_loc = info->ci_loc->lookup_block(&(f->front()));

      // they're nonlocal, we don't care.
      mem_block_ls_t defs;
      mem_use_ls_t uses;

      foreach_memblock(block_to_pass, *(info->extern_inputs)) {
        log() << "Passing external input for " << *block_to_pass << "\n";

        // make a use at the callsite
        Proc_Loc *attach_to = current_callsite->get_call_to();
        Mem_Use *use = (*block_to_pass)->use_at(attach_to);
        extern_uses.push_back(use);
        
        // find the rdef at the callsite
        Mem_Def *def = ci_nearest_def_at(*block_to_pass, current_callsite);
        use->reaching_def = def;

        mem_block_ls_t list_of_block;
        list_of_block.insert(*block_to_pass);

        // this is a weird redundant-looking assignment, but the rhs has the
        // rdef from the current context
        handle_assign(merge_loc, &list_of_block, new Ptr_Val(&list_of_block), defs,
                      uses, changes, true);
      }
    }

    // pass_external_outputs - for C.I., visit all external output, propagate
    // last value to call sites. make use at callsite reached by the last def
    // inside proc.

    // propagate last value of all external outputs to callsites by making a use
    // at the callsite reached by the correct def inside the proc.
    void pass_external_outputs (Function *f, mem_block_ls_t &extern_defs, mem_block_ls_t &changes)
    {
      ProcedureInfo *info = procedures.lookup(f);
      Proc_Loc *proc_loc = info->ci_loc;

      Stmt_Loc *procs_last_stmt = proc_loc->get_last_stmt();

      foreach_memblock(block_to_pass, *(info->extern_outputs)) {
        // find the last def of this
        Mem_Def *def = (*block_to_pass)->nearest_def_at(procs_last_stmt);

        // all the callsites...
        for (callsite_map_t::iterator iter = info->callsites.begin();
             iter != info->callsites.end(); ++iter)
        {
          Stmt_Loc *callsite = iter->first;
          ProcedureInfo *caller = iter->second;

          // skip things inaccessible from caller
          if (!procedures.is_in_scope(caller->proc, *block_to_pass)) {
            continue;
          }

          bool reanalyze_callsite = false;

          if (callsite == info->current_callsite) {
            // just pass the value back to the caller

            // make the special use with the right rdef
            Mem_Use *use = (*block_to_pass)->use_at(callsite);
            use->reaching_def = def;

            mem_use_ls_t ignored_uses;

            // this does the right thing, says cbz's comments.
            // TODO but because we do our own weird phi magic now, i'm not so
            // sure.
            eval_phi(callsite->parent, *block_to_pass, extern_defs, ignored_uses, changes);
          } else {
            // see if the points-to set will be changed... but don't actually
            // change till we revisit the site
            reanalyze_callsite = true;

            Mem_Def *callsite_def = (*block_to_pass)->find_def_at(callsite);
            if (callsite_def) {
              Mem_Use *use = (*block_to_pass)->use_at(callsite);
              // TODO cbz reuses the 'def' from earlier, is that right?
              def = use->reaching_def;
              if (def && equal_memblock_ls(def->points_to, callsite_def->points_to)) {
                // nope, we can skip it actually
                reanalyze_callsite = false;
              }
            }
          }

          // do stuff for the client
          (*block_to_pass)->set_current_def_use(callsite);
          if (callsite == info->current_callsite) {
            mem_block_ls_t block_ls;
            block_ls.insert(*block_to_pass);
            // a weird assignment to be telling them. guess they can look up the
            // def.
            // TODO: Add this back with a src?
            //client->at_assignment(callsite, &block_ls, new Ptr_Val(&block_ls), );
          }

          if (reanalyze_callsite) {
            procedures.mark_for_reanalysis(f);
          }
        }

        // reset... this won't break anything, right?
        (*block_to_pass)->current_def = NULL;
        (*block_to_pass)->current_use = NULL;
      }
    }


    // after analyzing a procedure, find external uses and defs.
    // uses are extern if rdef is outside
    // defs are external if it could be used outside... <---- haha what?
    void record_extern_io (Proc_Loc *context, mem_block_ls_t &defs, mem_block_ls_t &extern_defs,
                           mem_use_ls_t &uses, mem_use_ls_t &extern_uses)
    {
      Function *f = context->get_function();
      ProcedureInfo *info = procedures.lookup(f);

      bool found_new_inputs = false;

      // find uses with rdefs outside
      foreach_use(use, uses) {
        Mem_Def *def = (*use)->reaching_def;
        // no rdef? certaintly couldn't be external. :P
        if (def) {
          Mem_Block *block = def->owner;
          if (block->owner_proc != f) {
            if (!info->is_prefix(context, def->where)) {
              if (info->add_extern_input(block)) {
                found_new_inputs = true;
              }

              // pass it up to the caller
              ProcedureInfo *cur_caller = info->current_caller();
              if (cur_caller && procedures.is_in_scope(cur_caller->proc, block)) {
                extern_uses.push_back(*use);
              }
            }
          }
        }
      }

      // find defs visible outside of the proc. reset cur_def/use to avoid rdef
      // errors?
      // TODO zero = ...
      foreach_memblock(block, defs) {
        // how do we know if something is accessible outside this proc?
        // if it's not local to current proc but is in scope "above it", then
        // it's extern
        if ((*block)->owner_proc != f && procedures.is_in_scope(f, *block)) {
          // special case of a 'trivial def'. only def is the merge of external
          // ins, it's not a real boy, pinocchio.
          // TODO ^ implement it, i dont think it hurts us much if we dont
          if (info->add_extern_output(*block)) {
            found_new_inputs = true;
          }

          // pass it up to the caller
          ProcedureInfo *cur_caller = info->current_caller();
          if (cur_caller && procedures.is_in_scope(cur_caller->proc, *block)) {
            extern_defs.insert(*block);
          }
        }

        // reset cur def/use - doesn't break stuff, right?
        (*block)->current_def = NULL;
        (*block)->current_use = NULL;
      }

      if (found_new_inputs && info->is_context_sensitive) {
        procedures.mark_for_reanalysis(f);
      }
    }
  };
  
  char Main_DFA_Algo::ID = 0;
  static RegisterPass<Main_DFA_Algo> X("main", "main DFA for pointer analysis");

}


