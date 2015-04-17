// ----------------------------------------------------------------------
//
//  C-Breeze
//  C Compiler Framework
// 
//  Copyright (c) 2000 University of Texas at Austin
// 
//  Teck Bok Tok
//  Samuel Z. Guyer
//  Daniel A. Jimenez
//  Calvin Lin
// 
//  Permission is hereby granted, free of charge, to any person
//  obtaining a copy of this software and associated documentation
//  files (the "Software"), to deal in the Software without
//  restriction, including without limitation the rights to use, copy,
//  modify, merge, publish, distribute, sublicense, and/or sell copies
//  of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//  
//  The above copyright notice and this permission notice shall be
//  included in all copies or substantial portions of the Software.
//  
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
//  NONINFRINGEMENT.  IN NO EVENT SHALL THE UNIVERSITY OF TEXAS AT
//  AUSTIN BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
//  IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
//  OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//
//  We acknowledge the C-to-C Translator from MIT Laboratory for
//  Computer Science for inspiring parts of the C-Breeze design.
//
// ----------------------------------------------------------------------

#ifndef UNIFICATION_H
#define UNIFICATION_H

#include "c_breeze.h"

class Linker;

/* Algorithm source:
    Bjarne Steensgaard.  Points-to Analysis in Almost Linear Time.
    In Symposium on Principles of Programming Languages, pages 32--41, 1996
 */

class Unify_ECR;
class Unify_Offset;
class Unify_Size;

class UnifyType;
class UnificationBasedPtr;

class Unify_Pending;
class Unify_Pendings {
  std::set<Unify_Pending*> _set;
  bool cleaned;
public:
  Unify_Pendings() : cleaned(true) {}
  inline bool empty() const { return _set.empty(); }
  inline int size() const { return _set.size(); }
  inline void clear() { _set.clear(); }
  inline std::set<Unify_Pending*> set() const { return _set; }
  void insert(Unify_Pending *p, bool is_new);
  void cleanup();
}; // Unify_Pendings


//%{ /* Unify_ECR's */
class Unify_ECR {
protected:
  REF  declNode   *_var;
  REF  procNode   *_proc;
  REF  Unify_ECR  *_parent, *_root;
  int              _ndecestors;
  set<Unify_ECR*>  _children;
  Unify_Pendings   _pending;
  TREE int         _id;
  TREE UnifyType  *_type;

  static int       id_count;
  static set<Unify_ECR*> all_ECR;

public:
  Unify_ECR(declNode *v, procNode *p);
  Unify_ECR(UnifyType *t);

  inline declNode *var() const { return _var; }
  inline void var(declNode *d) { _var = d; }
  inline procNode *proc() const { return _proc; }
  inline void proc(procNode *p) { _proc=p; }
  inline Unify_ECR *parent() const { return _parent; }
  inline void parent(Unify_ECR *p) { _root = _parent = p; }
  Unify_ECR * root();
  inline Unify_Pendings & pending() { return _pending; }
  inline int id() const { return _id; }
  bool operator==(Unify_ECR &other) { return root()==other.root(); }
  Unify_ECR * Union(Unify_ECR *other);
  decl_list all_decls() const;
  UnifyType * type();
  void type(UnifyType *t);
  void print();
  inline friend ostream & operator<<(ostream &o, Unify_ECR &ecr) {
    ecr.print(); return o;
  }
  static set<Unify_ECR*> allECR() { return all_ECR; }
}; // Unify_ECR

//%}

//%{ /* misc */
class Unify_Offset {
public:
  typedef enum { zero, unknown } Value;
private:
  Value          _value;
  Unify_Pendings _pending;
public:
  Unify_Offset(Value v) : _value(v) {}
  inline Value value() const { return _value; }
  inline void value(Value v) { _value=v; }
  inline Unify_Pendings & pending() { return _pending; }
  inline bool leq(Unify_Offset *o) const
  { return _value==zero || _value==o->_value;}
  inline void print() const { cout << (_value==zero ? "zero" : "unknown"); }
  inline friend ostream & operator<<(ostream &o, const Unify_Offset &off) {
    off.print(); return o;
  }
};

class Alpha {
  Unify_ECR    *_tao;
  Unify_Offset *_offset;
public:
  Alpha(Unify_ECR *t, Unify_Offset *o) : _tao(t), _offset(o) { assert(t && o); }
  Alpha();
  inline Unify_ECR *tao() const { return _tao; }
  inline Unify_Offset *offset() const { return _offset; }
  bool leq(Alpha *o, Unify_Size s) const;
  bool equal(Alpha *o) { return _tao->type()==o->tao()->type() &&
                                _offset->value()==o->offset()->value(); }
  void print() const;
  inline friend ostream & operator<<(ostream &o, const Alpha &a) {
    a.print(); return o;
  }
};

typedef set<declNode*>    declSet;
typedef list<declSet>     FieldOrder;
typedef list<typeNode*>   FieldTypeOrder;
typedef map<declSet,Unify_ECR*> EltMap;

class Lambda {
  int            _n,_m;
  bool           _ellipsis; // contain ellipsis?
  Unify_ECR    **_taos, *_taoR; // array of tao
  bool           _is_bottom;
  int            _id;
  Unify_Pendings _pending;
  static int     id_count;
public:
  /*Lambda(int n, int m, ECR **t) : _n(n), _m(m), _taos(t), _is_bottom(false),
    _ellipsis(false), _id(id_count++) {}*/
  Lambda() : _n(0), _m(0), _ellipsis(false), _taos(NULL), _taoR(NULL),
             _is_bottom(true), _id(id_count++) {}
  inline int id() const { return _id; }
  inline bool is_bottom() const { return _is_bottom; }
  inline int n() const { return _n; }
  inline int m() const { return _m; }
  inline bool ellipsis() const { return _ellipsis; }
  inline Unify_ECR *tao(int i) const { assert(i<_n); return _taos[i]; }
  inline void tao(int i, Unify_ECR *t) { assert(i<_n); _taos[i]=t; }
  inline Unify_ECR *taoR() const { assert(0<_m); return _taoR; }
  inline void taoR(Unify_ECR *r) { assert(0<_m); _taoR=r; }
  inline Unify_Pendings & pending() { return _pending; }
  void settype(int n, int m, Unify_ECR **t, Unify_ECR *r, bool ellipse);
  void addArg(int plus_n);
  inline bool leq(Lambda *o) const { return is_bottom() || equal(o); }
  bool equal(Lambda *o) const;
  void print() const;
  inline friend ostream & operator<<(ostream &o, const Lambda &l) {
    l.print(); return o;
  }

  static Lambda * bottom();
}; // Lambda 

class Unify_Size {
  bool _is_top;
  int  _size;
public:
  Unify_Size(int size) : _is_top(size<=0 ? true : false), _size(size) {}
  Unify_Size() : _is_top(true), _size(0) {}
  inline bool is_top() const { return _is_top; }
  inline int size() const { return _size; }
  inline bool leq(Unify_Size o) const { return _size==o.size() || o.is_top(); }
  inline bool equal(Unify_Size o)
  { return _size==o.size() && _is_top==o.is_top(); }
  string str() const;
  static int sizeOf(typeNode *ty);
  static int sizeOfAssign(threeAddrNode *t, Linker &, UnificationBasedPtr *);
};

class Unify_Parents {
  set<Unify_ECR*> _parents;
  bool _is_top;
public:
  Unify_Parents(set<Unify_ECR*> parents) : _parents(parents), _is_top(false) {}
  Unify_Parents(bool top) : _is_top(top) {}
  inline bool is_top() const { return _is_top; }
  inline void top(bool t) { _is_top=t; }
  inline bool empty() const { return !_is_top && _parents.empty(); }
  inline bool equal(Unify_Parents o)
  { return _parents==o.parents() && _is_top==o.is_top(); }
  inline set<Unify_ECR*> & parents() { return _parents; }
  string str() const;
};
//%}

//%{ /* UnifyType's types */
typedef enum { BOTTOM, SIMPLE, STRUCTURE, OBJECT, BLANK } Object_Typ;

typedef struct Unify_Simple {
  Alpha *alpha;
  Lambda *lambda;
  Unify_Size s;
  Unify_Parents p;
  Unify_Simple(Alpha *a, Lambda *l, Unify_Size _s, Unify_Parents _p)
  : alpha(a), lambda(l), s(_s), p(_p) { }
} Unify_Simple;

typedef struct Unify_Structure {
  FieldOrder     fo;
  FieldTypeOrder fto;
  EltMap m;
  Unify_Size s;
  Unify_Parents p;
  Unify_Structure(suespecNode *sue, EltMap _m, Unify_Size _s, Unify_Parents _p);
  Unify_Structure(FieldOrder _fo, FieldTypeOrder _fto, EltMap _m, Unify_Size _s,
    Unify_Parents _p) : fo(_fo), fto(_fto), m(_m), s(_s), p(_p){}
  Unify_ECR *get(declNode *field, Unify_ECR *container);
  string map_str();
  string all_str();
} Unify_Structure;

typedef struct Unify_Object {
  Alpha *alpha;
  Lambda *lambda;
  Unify_Size s;
  Unify_Parents p;
  Unify_Object(Alpha *a, Lambda *l, Unify_Size _s, Unify_Parents _p)
  : alpha(a), lambda(l), s(_s), p(_p) {}
} Unify_Object;

typedef struct Unify_Blank {
  Unify_Size s;
  Unify_Parents p;
  Unify_Blank(Unify_Size _s, Unify_Parents _p) : s(_s), p(_p) {}
} Unify_Blank;
//%}

class memoryBlock;

//%{ /* actual type */
class UnifyType {
private:
  Unify_ECR     *_ecr;
  int            _id;
  static int     id_count;
  union {
    Unify_Simple    *simple;
    Unify_Structure *structure;
    Unify_Object    *object;
    Unify_Blank     *blank;
  } _tao;
  Object_Typ     obj_typ;
  memoryBlock   *_block;
  set<procNode*> _procs; // NEW: this type represent this set of procedures.

  bool _is_bottom;

public:
  UnifyType() : _id(id_count++), obj_typ(BOTTOM), _block(0), _is_bottom(true)
  { _ecr=new Unify_ECR(this); }
  UnifyType(Unify_Simple *s) : _id(id_count++),obj_typ(SIMPLE),_is_bottom(false)
  { _block=0; _tao.simple=s; _ecr=new Unify_ECR(this); }
  UnifyType(Unify_Structure *s) : _id(id_count++),obj_typ(STRUCTURE), _block(0),
   _is_bottom(false) { _tao.structure=s; _ecr=new Unify_ECR(this); }
  UnifyType(Unify_Object *o) : _id(id_count++),obj_typ(OBJECT),_is_bottom(false)
  { _block=0; _tao.object=o; _ecr=new Unify_ECR(this); }
  UnifyType(Unify_Blank *b) : _id(id_count++), obj_typ(BLANK), _is_bottom(false)
  { _block=0; _tao.blank=b; _ecr=new Unify_ECR(this); }
  UnifyType(Unify_Blank *b, Unify_ECR *ecr) : _id(id_count++), obj_typ(BLANK),
  _is_bottom(false) { _block=0; _tao.blank=b; _ecr=ecr; }

  inline Unify_ECR *ecr() const { if(_ecr) return _ecr->root(); return _ecr; }
  inline Unify_ECR *ecr_no_root() const { return _ecr; }
  inline void ecr(Unify_ECR *ecr) { _ecr=ecr; }
  inline bool is_bottom() const { return _is_bottom; }
  inline int id() const { return _id; }
  inline Object_Typ objTyp() const { return obj_typ; }
  inline memoryBlock *block() const { return _block; }
  inline void block(memoryBlock *b) { _block=b; }
  inline set<procNode*> & procs() { return _procs; }

  inline Unify_Simple *simple() const
  { assert(obj_typ==SIMPLE); return _tao.simple; }
  inline Unify_Structure *structure() const
  { assert(obj_typ==STRUCTURE); return _tao.structure; }
  inline Unify_Object *object() const
  { assert(obj_typ==OBJECT); return _tao.object; }
  inline Unify_Blank *blank() const
  { assert(obj_typ==BLANK); return _tao.blank; }

  inline bool leq(UnifyType *o, Unify_Size s) const
  { return this==o || s.leq(size()); }
  Unify_Size size() const;
  void print() const;
  inline friend ostream & operator<<(ostream &o, const UnifyType &t) {
    t.print(); return o;
  }

  static UnifyType *toTao(UnifyType *t);
  static inline UnifyType *bottom() { return new UnifyType(); }
}; // UnifyType


#define T_bottom UnifyType::bottom()
#define L_bottom Lambda::bottom()

//%}

class Unify_Pending {
public:
  typedef enum { makeunknown, join_ECR, join_Lambda, cjoin, collapse } Typ;
private:
  Typ   _typ;
  void *_arg1, *_arg2, *_arg3;
  bool  _served;
public:
  Unify_Pending(Unify_ECR *e1, Unify_ECR *e2) : _typ(join_ECR), _arg1(e1),
                                      _arg2(e2), _arg3(NULL), _served(false) {}
  Unify_Pending(Lambda *l1, Lambda *l2) : _typ(join_Lambda), _arg1(l1),
                                      _arg2(l2), _arg3(NULL), _served(false) {}
  Unify_Pending(Unify_Offset *o) : _typ(makeunknown), _arg1(o), _arg2(NULL),
                                   _arg3(NULL), _served(false) {}
  Unify_Pending(Unify_ECR *e) : _typ(collapse), _arg1(e), _arg2(NULL),
                                _arg3(NULL), _served(false) {}
  Unify_Pending(Unify_Size &s, Unify_ECR *e1, Unify_ECR *e2) : _typ(cjoin),
              _arg1(new Unify_Size(s)), _arg2(e1), _arg3(e2), _served(false) {}
  ~Unify_Pending() { if(_typ==cjoin) delete (Unify_Size*) _arg1; }
  inline Typ typ() const { return _typ; }
  inline void *arg1() const { return _arg1; }
  inline void *arg2() const { return _arg2; }
  inline void *arg3() const { return _arg3; }
  inline bool is_served() const { return _served; }
  inline void served() { _served=true; }
  inline void un_served() { _served=false; }
  void print() const;
  inline friend ostream & operator<<(ostream &o, const Unify_Pending &p) {
    p.print(); return o;
  }
};

typedef set<Unify_Pending*>::iterator Pendings_p;
typedef set<UnifyType*>               UnifyTypes;

class memoryModel;
class stmtLocation;
class procLocation;
 
class UnificationBasedPtr : public Walker {
public:
  static UnificationBasedPtr *analyze_all(Linker &);

  inline Unify_ECR *ecr(declNode *decl) const {
    if(_ecr.find(decl) != _ecr.end()) return _ecr.find(decl)->second;
    return NULL;
  }
  inline Unify_ECR *ecr(threeAddrNode *alloc_site) const {
    if(_alloc.find(alloc_site) != _alloc.end())
      return _alloc.find(alloc_site)->second;
    return NULL;
  }
  inline UnifyType *proctype(procNode *p) { return _proctype[p]; }
  void createProcBlock(procNode *, memoryModel &, procLocation*);

  // annotation mark this stmt as it create obj
  virtual void mark_alloc(stmtNode *stmt, declNode *source_decl,
                          declNode *target_decl) {}

  void ensure_no_bottom(Unify_ECR *tao, declNode *decl,
                        UnifyType *parent); // new

  virtual bool isField(declNode *f, bool &from_annotation) const {
    from_annotation = false;
    return _unique_field_defn.find(f) != _unique_field_defn.end();
  }
  Unify_ECR *ecrField(UnifyType *container, declNode *field,
                      bool field_from_annotation);
  Unify_ECR *ecrDeref(UnifyType *container);
  static bool reachable(UnifyType *src, UnifyTypes & targets,
                        UnifyTypes & visited);

  inline Alpha *string_alpha(constNode *con) { return _string_alpha[con]; }
  set<Unify_ECR*> unique_ecr() const;
  inline procNode *synthetic_proc(declNode *d) { return _synthetic_proc[d]; }
  static bool compatible_type(typeNode *, typeNode *);

  void print_ecr();

  static int unify_points_to_max;

protected:
  TREE map<declNode*, Unify_ECR*>      _ecr;
  TREE map<procNode*, UnifyType*>      _proctype;
  TREE map<threeAddrNode*, Unify_ECR*> _alloc; // ecr for allocation sites
  TREE map<constNode*,Alpha*>          _string_alpha;
  //#tmp vars created by diamantler
  TREE int                             _dismantler_tmp;

  TREE map<declNode*, suespecNode*> _field2sue;
  TREE map<declNode*, decl_list>    _fieldpos;

  TREE set<suespecNode*>            _unique_sue, //want only unique suespecNode
                                    _visited_sue;
  TREE map<declNode*, declNode*>    _unique_field_defn;

  map<threeAddrNode*,UnifyType*>    _ptr_call;

  unitNode           *cur_unit;
  procNode           *cur_proc;
  bool                inside_call_func;
  bool                finalizing, serve_again, new_pending;
  set<Unify_Pending*> more_pending;

  Linker                   &linker;
  map<declNode*,procNode*> _synthetic_proc;
  set<procNode*>           _analyzed_proc; //, _skip_proc;

  class findVarAssign; // find number of times a variable is assigned to.
  map<declNode*,int>  _assignments;
  map<declNode*,bool> _uses;  // any use for this var?

  UnificationBasedPtr(Linker &l) : Walker(Both,Subtree),
    _dismantler_tmp(0), cur_proc(0),
    inside_call_func(false), finalizing(false), serve_again(false),
    new_pending(true), linker(l) {}

  void at_unit(unitNode *u, Order) { cur_unit = u; }
  void at_proc(procNode *, Order);
  void at_decl(declNode *, Order);
  void at_suespec(suespecNode *, Order);
  void at_threeAddr(threeAddrNode *, Order);

  void at_allocation(threeAddrNode *, Unify_Size);
  void at_call(threeAddrNode *, Unify_Size);
  void at_initializer(initializerNode *init, typeNode *init_type,
                      Unify_ECR *init_ecr);
  void mergeOperand(operandNode *lhs, operandNode *rhs, Unify_Size);

  // condition to use alpha(): operand is addr().
  Alpha *alpha(operandNode *, Unify_Size);
  // condition to use ecr(): opposite of alpha() above.
  Unify_ECR *ecr(operandNode *, Unify_Size, Unify_Offset **offset=NULL);
  // similar to ecr(declNode*) but if not found, attempt to create.
  Unify_ECR *ecr1(declNode *decl);

  void join(Alpha *a1, Alpha *a2, bool *recursion_detected=NULL);
  void join(Unify_ECR *e1, Unify_ECR *e2, bool *recursion_detected=NULL);
  void join(Lambda *l1, Lambda *l2); // new
  void settype(Unify_ECR *e, UnifyType *t);
  void settype(Lambda *l, int n, int m, Unify_ECR **t, Unify_ECR *r,
               bool ellipse);
  void ensure_sim_obj(Unify_ECR *tao, Unify_Size &s);
  Unify_Structure *ensure_struct_obj(Unify_ECR *tao, suespecNode *sue, //new
                                     bool redo_pending = false);
  void expand(Unify_ECR *e);
  void promote(Unify_ECR *e, Unify_Size &s);
  void collapse(Unify_ECR *e);
  void make_unknown(Unify_Offset *o);
  void unless_zero(Unify_Offset *o, Unify_ECR *tao);
  void cjoin(Unify_Size &s, Unify_ECR *e1, Unify_ECR *e2);
  UnifyType *unify(UnifyType *t1, UnifyType *t2);
  bool make_compatible(declSet n, Unify_Structure *s, Unify_ECR *container,
                       bool force=false);
  void merge_EltMap(/*IN*/ UnifyType *t1, UnifyType *t2,
                    /*IN,OUT*/ Unify_Structure *);

  void finalize();

  static bool is_va_list(declNode * decl);
  procNode *create_synthetic_proc(declNode *);

  inline declNode *unique_field_defn(declNode *d)
  { return _unique_field_defn[d]; }
  inline suespecNode *field2sue(declNode *d)
  { return _field2sue[_unique_field_defn[d]]; }

  // to be overwritten by subclass
  virtual bool annotation_returns_object(procNode *proc) const { return false; }
  virtual void annotation_call_arg(procNode*, int arg, typeNode*, Unify_ECR*) {}
  virtual void annotation_call_arg(procNode*, int arg, typeNode*, Alpha*) {}
  virtual void annotation_ret_val(procNode *, Unify_ECR *taoR, unitNode *unit){}
  virtual void annotation_init_global(declNode *global) {}
}; // UnificationBasedPtr

#define is_Sim_Obj(t) assert(t->objTyp()==SIMPLE || t->objTyp()==OBJECT)
#define Sim_Obj_Size(t) (t->objTyp()==SIMPLE ? t->simple()->s : t->object()->s)
#define Sim_Obj_Lambda(t) \
  (t->objTyp()==SIMPLE ? t->simple()->lambda : t->object()->lambda)
#define Sim_Obj_Alpha(t) ((t->objTyp()==SIMPLE) ? t->simple()->alpha : t->object()->alpha)


#endif
