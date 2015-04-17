/*	$Id: briggs_reg_alloc.h,v 1.8 2002/11/22 06:00:21 cnevill Exp $	*/
// register allocator code
// author: Paul Navratil
//				 pnav@cs.utexas.edu

#ifndef _BRIGGS_REG_ALLOC_H
#define _BRIGGS_REG_ALLOC_H

#include <list>
#include <map>
#include <set>
#include <vector>

#include <limits.h>
#include <string.h>
#include <stdlib.h>

#include "reg_alloc.h"

// def-use chain is a list of pointers to each LIR node for the id
// LIR translator should assign temp registers for each instruction
// assuming infinite registers.	
// Unifying registers will occur when webs are formed.

// symbol to identify web.	
// can be var id, register, or const val
struct Symbol {

	// ctors
	Symbol() {
		_type = sym_unknown;
		_id = NULL;
	}
	Symbol( declNode * id ) {
		_type = sym_id;
		_id = id;
	}
	Symbol( const Register & reg ) {
		_type = sym_reg;
		_id = NULL;
		_reg = reg;
	}
	Symbol( const constant & con ) {
		_type = sym_const;
		_id = NULL;
		_const = con;
	}

	// can be decl, register, or constant
	declNode * _id;
	Register _reg;
	constant _const;

	// what is it?
	enum {
		sym_unknown,
		sym_id,
		sym_reg,
		sym_const
	} _type;

	// compare two of these
	bool operator ==( const Symbol & rhs ) const;

	// get our type
	lir_var_type getLirVt() const;
};

// the type we're going to use for our defuse chain
typedef LirInst* DUChainDef;
typedef list<DUChainDef> DUChainDefs;
typedef DUChainDefs::iterator DUChainDefs_p;

// list of LIR instruction pointers that form a def-use chain
typedef LirInst* DuChainUse;
typedef list<DuChainUse> DUChainUses;
typedef DUChainUses::iterator DUChainUses_p;

class sdu
{
	Symbol sym;
	DUChainDef def;
	DUChainUses uses;

 public:
	sdu( Symbol sym, DUChainDef def ) :
		sym(sym), def(def)
		{}

	Symbol & getSymbol() { return sym; }
	DUChainDef & getDef() { return def; }
	DUChainUses & getUses() { return uses; }
};

typedef list<sdu*> sduList;
typedef sduList::iterator sduList_p;

class WebRecord 
{
	Symbol sym;
	DUChainDefs defs;
	DUChainUses uses;
	bool spill;
	Register reg;
	int disp;

 public:
	WebRecord(Symbol sym, Register reg, bool spill = false, int disp = -1)
		: sym(sym), spill(spill), reg(reg), disp(disp)
		{}

	bool intersectUses( DUChainUses &u);
	void unionDefs( DUChainDefs &d );
	void unionUses( DUChainUses &u );

	Symbol & getSym() { return sym; }
	DUChainDefs & getDefs() { return defs; }
	DUChainUses & getUses() { return uses; }
	Register & getReg() { return reg; }
	bool & getSpill() { return spill; }
};

typedef set<WebRecord*> WRSet;
typedef WRSet::iterator WRSet_p;

typedef vector<WebRecord*> WRVector;
typedef WRVector::iterator WRVector_p;

typedef set<int> int_set;
typedef int_set::iterator int_set_p;

class ListRecord 
{
	int webNum;
	int num_ints, color, disp;
	float spillCost;
	int_set adjNodes, removeAdj;

 public:
    static const int color_none;

	ListRecord( int web, float spc ):
		webNum(web), num_ints(0), color(color_none), disp(INT_MIN), spillCost(spc), adjNodes(), removeAdj()
		{ }

	void addAdjacentNode( int reg );
	void removeAdjacentNode( int reg );
	void removeAllAdjacencies();
	void incCost( float f ) { spillCost += f; }
	void decCost( float f ) { spillCost -= f; }

	int getWeb() { return webNum; }
	int numInts() { return num_ints; }
	float getSpillCost() { return spillCost; }
	const int_set & getAdjNodes() { return adjNodes; }
	const int_set & getRmAdjNodes() { return removeAdj; }
	int & getColor() { return color; }
};

typedef vector<ListRecord*> LRVector;
typedef LRVector::iterator LRVector_p;

typedef hash_set_ex<int> color_set;

class briggs_reg_alloc : public reg_alloc
{
	// number of registers available to the algorithm
	int num_regs, usable_regs;
	int regs_reserved;
	int num_webs;
	WRVector Symreg;

	// stack of nodes built in pruneGraph
	list<int> nodeStack;

	// color of each real register - indexed by color, maps to register
	vector<int> colorToRealReg;

	// liveness information
	inst_to_reg_id_map liveRegs;

	// adjacency information
	vector<vector<bool> > adjMatrix;
	LRVector adjLists;

	// current procedure of interest
	procNode * proc;

	// spill cost weights
	static const float useWt;
	static const float copyWt;
	static const float defWt;

 protected:
	// helpers
	template < class T > set< T > * copySet( set< T > * s );
	bool interfere( WebRecord *wr, int reg );
	bool liveAt( WebRecord *wr, DUChainDef def );
	bool nonStore( int reg_k, int reg_l, instruction_list_p instr );
	void deleteInstr( instruction_list_p instr );
	int	depth( instruction_list_p instr );
	void makeDuChains( sduList & DuChains );
	bool isSymReg( const Register & reg );
	void loadSymReg( Register & reg, declNode * contents, 
		instruction_list_p currentInstruction, instruction_list& insts );
	void removeUnavailableColors( ListRecord * node, color_set & colors );
	void changeWebRegister( WebRecord * web, const Register & newReg );

	// Briggs Algorithm functions
	// Taken from Muchnick 1997, Chapter 16 
	void makeWebs( sduList &DuChains );
	void buildAdjMatrix();
	bool coalesceRegisters();
	void buildAdjLists();
	void computeSpillCosts();
	void pruneGraph();
	void adjustNeighbors( int reg );
	bool assignRegisters();
	void modifyCode();
	void genSpillCode();

 public:
	// allocate registers for a procedure
	void allocate(procNode * proc);

};

class RegAllocWalker : public Walker
{
public:
	// ctor
	RegAllocWalker();

	// handle various things
	void at_unit(unitNode *the_unit, Order ord);
	void at_proc(procNode *the_proc, Order ord);

};

#endif
