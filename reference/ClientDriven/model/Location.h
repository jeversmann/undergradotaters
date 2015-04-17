#ifndef LLVM_BDWY_LOCATION
#define LLVM_BDWY_LOCATION

// authors: Dustin, Parth

/*
 * Locations are a key in the value |-> points-to set mapping. Think of them of
 * as (calling context, pointer name) pairs. Since they encode
 * context-sensitivity (or lack thereof), it doesn't suffice to just specify a
 * static line of code. If there is a clone-like instance of a Proc_Loc for some
 * procedure and there are also multiple instances of some other procedure that
 * calls it, then even more instances of this Proc_Loc will be created. Static
 * line of code would just be a fixed distance of one on the CFG.
 */

#include <stack>
#include "llvm/Support/raw_os_ostream.h"
#include "llvm/Function.h"
#include "llvm/Value.h"
#include "Blocks.h"
#include "ProcedureDB.h"

using namespace llvm;

namespace llvm_bdwy {

// TODO how to specify context-insensitive for a loc.?
// TODO let members creep private->public, or move more functionality here?

// Forward declarations
class Stmt_Loc;
class BB_Loc;
class Proc_Loc;
class Mem_Table;

typedef std::map<Value*, Stmt_Loc*> stmt_map_t;
typedef std::map<BasicBlock*, BB_Loc*>   bb_map_t;
typedef std::set<Mem_Block*> mem_block_ls_t;  // TODO potentially poorly named. set, not ls.

enum LocType { STMT_TYPE, BB_TYPE, PROC_TYPE, GLOBAL_TYPE };

class Loc {
  private:
    static void find_common_ancestor(Loc **l1, Loc **l2);

  public:
    int depth;  // depth in the inter-procedural CFG

    virtual LocType getValueID() const = 0;

    // need to make queries on the singleton
    static ProcedureDB *proc_db;
    static Mem_Table *memory_model;

    static bool dominates (Loc *l1, Loc *l2);
    static bool strictly_dominates (Loc *l1, Loc *l2);

    // all three types of locations can answer this pretty easily
    virtual Function* get_function () = 0;
    virtual Loc* get_parent () const = 0;

    // this is a bit of a hack
    virtual void prettyprint (raw_ostream &os) const = 0;
    virtual void graph (raw_os_ostream &os) const = 0;
    virtual void dump () const = 0;
};

class Proc_Loc : public Loc {
  private:
    bool is_context_sensitive;
    bb_map_t blocks;

  public:

    Stmt_Loc *parent;       // think of this as the call_site
    Function* func;         // the LLVM thing backing this
    Proc_Loc (Stmt_Loc *parent, Function *f, bool is_cs);

    Function* get_function ();
    Loc* get_parent () const;

    virtual LocType getValueID() const { return PROC_TYPE; }
    static inline bool classof(Proc_Loc const*) { return true; }
    static inline bool classof(Loc const *loc) {
      return loc->getValueID() == PROC_TYPE;
    }
    void prettyprint (raw_ostream &os) const;
    void graph (raw_os_ostream &os) const;
    void dump () const;

    BB_Loc* lookup_block (BasicBlock *bb);
    Stmt_Loc* get_last_stmt ();

    // the loc for main(); we all need to see this for a fast-path in dominance tests
    static Proc_Loc *root_loc;
};

class BB_Loc : public Loc {
  private:
    stmt_map_t statements;
  
  public:
    Proc_Loc *parent;
    BasicBlock* bb;         // the LLVM thing backing this
    mem_block_ls_t phis;    // when stuff changes elsewhere that could affect us...

    BB_Loc (Proc_Loc *proc, BasicBlock *block);

    Function* get_function ();
    Loc* get_parent () const;

    virtual LocType getValueID() const { return BB_TYPE; }
    static inline bool classof(BB_Loc const*) { return true; }
    static inline bool classof(Loc const *loc) {
      return loc->getValueID() == BB_TYPE;
    }
    void prettyprint (raw_ostream &os) const;
    void graph (raw_os_ostream &os) const;
    void dump () const;
    
    Stmt_Loc* lookup_stmt (Value *stmt);
    Stmt_Loc* last();
};

class Global_Loc : public Loc {
  public:
    GlobalVariable *decl; // declaration location
    Global_Loc (GlobalVariable *decl) : decl(decl) {}
    virtual LocType getValueID() const { return GLOBAL_TYPE; }
    Function* get_function() { return NULL;}
    Loc* get_parent() const { return NULL; }
    void prettyprint (raw_ostream &os) const { os << "Global_Loc(" << *decl << ")"; }
    void graph (raw_os_ostream &os) const;
    void dump() const;
    static inline bool classof(Global_Loc const*) { return true; }
    static inline bool classof(Loc const *loc) { return loc->getValueID() == GLOBAL_TYPE; }
};

class Stmt_Loc : public Loc {
  private:
    Proc_Loc *call_to;      // the recursion lies here...

  public:
    BB_Loc *parent;
    Instruction *expr;            // equivalent to this as a Value
    Stmt_Loc (BB_Loc *bb, Instruction *value);

    Function* get_function ();
    Loc* get_parent () const;
    Proc_Loc* make_call_to (Function *call_to, bool is_cs);
    Proc_Loc* get_call_to () { return call_to; }
    
    virtual LocType getValueID() const { return STMT_TYPE; }
    static inline bool classof(Stmt_Loc const*) { return true; }
    static inline bool classof(Loc const *loc) {
      return loc->getValueID() == STMT_TYPE;
    }
    void prettyprint (raw_ostream &os) const;
    void graph (raw_os_ostream &os) const;
    void dump () const;
};

}

// pretty-printing
namespace llvm {
  raw_ostream &operator<<(raw_ostream &os, const llvm_bdwy::Loc &loc);
}

#endif
