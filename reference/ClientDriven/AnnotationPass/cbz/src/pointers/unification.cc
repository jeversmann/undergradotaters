
#include "unification.h"
#include "linker.h"
#include "memoryblock.h"
#include "memorymodel.h"
#include "location.h"

static bool debug = false;

//#define cx_vt

#ifdef cx_vt
int vt = 0, vt_e = 0; // debug
#endif

int UnificationBasedPtr::unify_points_to_max = 0;

void Unify_Pendings::insert(Unify_Pending *p, bool is_new) {
#define delete_and_return { \
  if(is_new) delete p; \
  return; \
}

  // check for similar p already in _set.
  if(!is_new && _set.find(p) != _set.end()) return; // quick check

  if(p->typ() == Unify_Pending::cjoin) {
    Unify_ECR *e1 = (Unify_ECR*) p->arg2(),
              *e2 = (Unify_ECR*) p->arg3();
    if( e1->root() == e2->root() ) delete_and_return;
    // peek: if both objects pointing to same object, there is nothing more to
    // do in the future.
    if( e1->type()->objTyp()==OBJECT &&
        e2->type()->objTyp()==OBJECT &&
        e1->type()->object()->alpha->tao()->type() ==
        e2->type()->object()->alpha->tao()->type() ) {
      /*std::set<Unify_ECR*> e2_parents = e2->type()->object()->p.parents();
      e1->type()->object()->p.parents().insert(e2_parents.begin(),
                                               e2_parents.end());*/
      delete_and_return;
    }

  } else if(p->typ() == Unify_Pending::join_ECR) {
    Unify_ECR *e1 = (Unify_ECR*) p->arg1(),
              *e2 = (Unify_ECR*) p->arg2();
    if( e1->root() == e2->root() ) delete_and_return;
  }

  cleanup();

  std::set<Unify_Pending*> temp_set = _set;
  for(Pendings_p s=temp_set.begin(); s!=temp_set.end(); s++)
    if((*s)->typ() == p->typ()) {
      bool d1 = (*s)->arg1() != p->arg1(),
           d2 = (*s)->arg2() != p->arg2(),
           d3 = (*s)->arg3() != p->arg3();
      switch(p->typ()) {
        case Unify_Pending::cjoin:
          if(d1) {
            Unify_Size *s1 = (Unify_Size*) (*s)->arg1(),
                       *s2 = (Unify_Size*) p->arg1();
            if(s1->is_top()==s2->is_top() && s1->size()==s2->size())
              d1 = false;
          }
          // cannot just check ECR etc. due to their respective pendings.
          if(d2 && ((Unify_ECR*)(*s)->arg2())->root()
                   == ((Unify_ECR*)p->arg2())->root())
            d2 = false;
          if(d3 && ((Unify_ECR*)(*s)->arg3())->root()
                   == ((Unify_ECR*)p->arg3())->root())
            d3 = false;

          if(!d1 && !d2 && !d3) {
            // temporarily add p to this, so that in recursion, would quickly
            // skip case where this->insert(p) is called again.
            _set.insert(p);
            Unify_ECR *es = (Unify_ECR*) (*s)->arg2(),
                      *ep = (Unify_ECR*) p->arg2();
            if(es != ep) {
              std::set<Unify_Pending*> S = ep->pending().set();
              for(Pendings_p q=S.begin(); q!=S.end(); q++)
                if(& es->pending()!=this && *q!=p) //prevent infinite recursion
                  es->pending().insert(*q,false);
            }
            es = (Unify_ECR*) (*s)->arg3();
            ep = (Unify_ECR*) p->arg3();
            if(es != ep) {
              std::set<Unify_Pending*> S = ep->pending().set();
              for(Pendings_p q=S.begin(); q!=S.end(); q++)
                if(& es->pending()!=this && *q!=p) //prevent infinite recursion
                  es->pending().insert(*q,false);
            }
            _set.erase(p); // erase temporary add.
          }
          break;

        case Unify_Pending::join_ECR:
          if(d1) {
            Unify_ECR *es = (Unify_ECR*) (*s)->arg1(),
                *ep = (Unify_ECR*) p->arg1();
            if(es->root() == ep->root() &&
               es->pending().set() == ep->pending().set())
              d1 = false;
          }
          if(d2) {
            Unify_ECR *es = (Unify_ECR*) (*s)->arg2(),
                     *ep = (Unify_ECR*) p->arg2();
            if(es->root() == ep->root() &&
               es->pending().set() == ep->pending().set())
              d2 = false;
          }
          /* do not try to merge in pendings as below, it cause much slowdown.
          if(!d1 && !d2) {
            // _set.insert(p);
            ECR *es = (ECR*) (*s)->arg1(),
                *ep = (ECR*) p->arg1();
            if(es != ep) {
              std::set<Unify_Pending*> S = ep->pending().set();
              for(Pendings_p q=S.begin(); q!=S.end(); q++)
                if(& es->pending()!=this && *q!=p) //prevent infinite recursion
                  es->pending().insert(*q);
            } 
            es = (ECR*) (*s)->arg2();
            ep = (ECR*) p->arg2();
            if(es != ep) {
              std::set<Unify_Pending*> S = ep->pending().set();
              for(Pendings_p q=S.begin(); q!=S.end(); q++)
                if(& es->pending()!=this && *q!=p) //prevent infinite recursion
                  es->pending().insert(*q);
            }
            _set.erase(p); // erase temporary add.
          } */
          break;
        case Unify_Pending::join_Lambda:
          if(d1 && ((Lambda*)(*s)->arg1()) == (Lambda*)p->arg1())
            d1 = false;
          if(d2 && ((Lambda*)(*s)->arg2()) == (Lambda*)p->arg2())
            d2 = false;
          if(!d1 && !d2) {
            _set.insert(p); // temporary add
            Lambda *ls = (Lambda*) (*s)->arg1(),
                   *lp = (Lambda*) p->arg1();
            if(ls != lp) {
              std::set<Unify_Pending*> S = lp->pending().set();
              for(Pendings_p q=S.begin(); q!=S.end(); q++)
                if(& ls->pending()!=this && *q!=p) //prevent infinite recursion
                  ls->pending().insert(*q,false);
            }
            ls = (Lambda*) (*s)->arg2();
            lp = (Lambda*) p->arg2();
            if(ls != lp) {
              std::set<Unify_Pending*> S = lp->pending().set();
              for(Pendings_p q=S.begin(); q!=S.end(); q++)
                if(& ls->pending()!=this && *q!=p) //prevent infinite recursion
                  ls->pending().insert(*q,false);
            }
            _set.erase(p); // erase temporary add.
          }
          break;
        case Unify_Pending::collapse:
          if(d1 && ((Unify_ECR*)(*s)->arg1())->root()
                   == ((Unify_ECR*)p->arg1())->root()) {
            d1 = false;
            _set.insert(p);
            Unify_ECR *es = (Unify_ECR*) (*s)->arg1(),
                      *ep = (Unify_ECR*) p->arg1();
            if(es != ep) {
              std::set<Unify_Pending*> S = ep->pending().set();
              for(Pendings_p q=S.begin(); q!=S.end(); q++)
                if(& es->pending()!=this && *q!=p) //prevent infinite recursion
                  es->pending().insert(*q,false);
            }
            _set.erase(p); // erase temporary add.
          }
          break;
        case Unify_Pending::makeunknown:
          if(d1) {
            Unify_Offset *os = (Unify_Offset*) (*s)->arg1(),
                         *op = (Unify_Offset*) p->arg1();
            if(os->value()==op->value()) {
              d1 = false;
              _set.insert(p); // temporary add
              std::set<Unify_Pending*> S = op->pending().set();
              for(Pendings_p q=S.begin(); q!=S.end(); q++)
                if(& os->pending() != this && *q!=p)//prevent infinite recursion
                  os->pending().insert(*q,false);
              _set.erase(p); // erase temporary add.
            }
          }
      }
      if(!d1 && !d2 && !d3) delete_and_return;
    }

  _set.insert(p);
  cleaned = false;
} // insert


void Unify_Pendings::cleanup() {
  if(cleaned) return;
  std::set<Unify_Pending*> temp_set = _set;
  for(Pendings_p s=temp_set.begin(); s!=temp_set.end(); s++)
    switch((*s)->typ()) {
      case Unify_Pending::cjoin: {
        Unify_ECR *e1 = (Unify_ECR*) (*s)->arg2(),
                  *e2 = (Unify_ECR*) (*s)->arg3();
        // if both objects pointing to same object, there is nothing more to do
        // in the future.
        if( e1->type()->objTyp()==OBJECT &&
            e2->type()->objTyp()==OBJECT &&
            e1->type()->object()->alpha->tao()->type() ==
            e2->type()->object()->alpha->tao()->type() ) {
          /*std::set<Unify_ECR*> e2_parents = e2->type()->object()->p.parents();
          e1->type()->object()->p.parents().insert(e2_parents.begin(),
                                                   e2_parents.end());*/
          _set.erase(*s);
        } else if( e1->root() == e2->root())
          _set.erase(*s);
        break;
      }
      case Unify_Pending::join_ECR:
        if( ((Unify_ECR*) (*s)->arg1())->root() ==
            ((Unify_ECR*) (*s)->arg2())->root())
          _set.erase(*s);
        break;
    }
  cleaned = true;
} // cleanup


void Unify_Pending::print() const {
  switch(_typ) {
    case makeunknown:
      cout << "make_unknown(" << _arg1 << ")"; break;
    case join_ECR:
      cout << "join_ECR(" << ((Unify_ECR*)_arg1)->type()->id()
           << "," << ((Unify_ECR*)_arg2)->type()->id() << ")";
      break;
    case join_Lambda:
      cout << "join_Lambda(" << ((Lambda*)_arg1)->id() << ","
           << ((Lambda*)_arg2)->id() << ")";
      break;
    case cjoin:
      cout << "cjoin(" << ((Unify_Size*)_arg1)->str() << ","
           << ((Unify_ECR*)_arg2)->type()->id() << ","
           << ((Unify_ECR*)_arg3)->type()->id() << ")";
      break;
    case collapse:
      cout << "collapse(," << ((Unify_ECR*)_arg1)->type()->id() << ")";
  }
} // Unify_Pending print



Alpha::Alpha() : _tao(T_bottom->ecr()),
                 _offset(new Unify_Offset(Unify_Offset::zero)) {}


bool Alpha::leq(Alpha *o, Unify_Size s) const {
  return _tao->type()->is_bottom() || _tao->type()->leq(o->_tao->type(),s)
                                   && _offset->leq(o->_offset);
} // leq

void Alpha::print() const {
  cout << "(" << _tao->type()->id() << "," << *_offset << ")";
}

int Lambda::id_count=0;

Lambda *Lambda::bottom() {
  Lambda *_bottom = new Lambda();
  return _bottom;
} // bottom


void Lambda::settype(int n, int m, Unify_ECR **t, Unify_ECR *r, bool ellipsis) {
  assert(_is_bottom);
  _is_bottom=false; _n=n; _m=m; _ellipsis=ellipsis; _taos=t; _taoR=r;
} // Lambda::settype


void Lambda::addArg(int n) {
  if(n <= 0 || _ellipsis) return;
  Unify_ECR **new_taos = (Unify_ECR**) malloc((n+_n) * sizeof(Unify_ECR*));
  for(int i=0; i<_n; i++) new_taos[i] = _taos[i];
  for(int i=_n; i<n+_n; i++) new_taos[i] = T_bottom->ecr();
  free(_taos);
  _taos = new_taos;
  _n += n;
}


bool Lambda::equal(Lambda *o) const {
  if(is_bottom() && o->is_bottom()) return true;
  if(is_bottom() || o->is_bottom()) return false;
  if(n()!=o->n() || m()!=o->m()) return false;
  for(int i=0; i<n(); i++)
    if(tao(i)->type() != o->tao(i)->type()) return false;
  if(m())
    if(taoR()->type() != o->taoR()->type()) return false;
  return true;
} // equal

void Lambda::print() const {
  cout << id();
  if(is_bottom()) {
    cout << "-L_bot";
    return;
  }
  cout << "-lambda(" << _n << ":";
  for(int i=0; i<n(); i++)
    cout << tao(i)->type()->id() << ",";
  if(_ellipsis) cout << "...";
  if(_m) cout <<" + "<< taoR()->type()->id();
  cout << ")";
}

int Unify_Size::sizeOf(typeNode *ty) {
  assert(ty);
  switch(ty->typ()) {
    case Prim: {
      basic_type bt = ((primNode*)ty)->basic();
      if(bt == basic_type::Void || bt == basic_type::Ellipsis)
        return -1;
      return bt.size();
    }
    case Ptr: return sizeof(int*); // TBD ???
    case Array: {
      arrayNode *a = (arrayNode*) ty;
      if(a->dim() && a->dim()->typ()==Const) {
        int dim = ((constNode*)a->dim())->value().Integer();
        return sizeOf(a->type()) * dim;
      }
      return sizeof(int*); // conservative? TBD?
    }
    case Enum: return sizeof(unsigned int); // ???
    case Struct:
    case Union:
    case sueSpec: {
      suespecNode *spec = (ty->typ()==sueSpec) ? (suespecNode*) ty
                                               : ((sueNode*)ty)->spec();
      decl_list fields = spec->fields();
      int r = 0;
      for(decl_list_p d=fields.begin(); d!=fields.end(); d++) {
        int f = sizeOf((*d)->type());
        if(ty->typ()==Struct) r += f;
        else if(r>f) r=f;
      }
      return r;
    }
    case Func: {
      long p1 = sizeof(&sizeOf),
           p2 = sizeof(&sizeOfAssign);
      assert(p1 == p2); // test
      return p1;
    }
    case Tdef: return sizeOf(((tdefNode*)ty)->def());
    default: cout << ty->typ() << endl; assert(false);
  }
} // sizeOf


int Unify_Size::sizeOfAssign(threeAddrNode *t, Linker &linker,
                             UnificationBasedPtr *analyzer) {
  if(!t || !t->lhs()) return 0;

#define size_operand(x,value) \
  /* could have use operandNode::type(), but that does not seem correct: does
   * not handle star(). */ \
  if(x->cast()) \
    value = Unify_Size::sizeOf(x->cast()); \
  else if(x->var()->typ() == Const) { \
    assert(!x->addr() && !x->star() && x->fields().empty()); \
    typeNode *ty = ((constNode*)x->var())->type(); \
    if(! x->index()) \
      value = Unify_Size::sizeOf(ty); \
    else { /* "some_str"[..] */ \
      assert(ty->typ() == Array); \
      value = Unify_Size::sizeOf(ty->type()); \
    } \
  } else { \
    /* precedence: star, field, index, addr. */ \
    typeNode *ty = ((idNode*)x->var())->decl()->type(); \
    int defers = 0; \
    if(x->star()) defers++; \
    if(! x->fields().empty()) { \
      assert(x->fields().back()->decl()); \
      ty = x->fields().back()->decl()->type(); \
      assert(ty); \
      defers=0; \
    } \
    if(x->index()) defers++; \
    if(x->addr()) defers--; \
    if(defers<0) value = sizeof(int*); /* ?? */ \
    else { \
      while(defers-->0) { \
        if(ty->typ()==Tdef) ty = ((tdefNode*)ty)->def(); \
        ty=ty->type(); \
      } \
      value = Unify_Size::sizeOf(ty); \
    } \
  }

  if(t->rhs1()->cast())
    return sizeOf(t->rhs1()->cast());
  if(t->op() && t->op()->id()==Operator::FUNC_CALL) {
    if(t->rhs1()->star()) { // call thru function pointer.
      // use lhs to determine value (may not be correct? TBD)
      int lhs;
      size_operand(t->lhs(), lhs)
      return lhs;
    } else {
      declNode *call = ((idNode*) t->rhs1()->var())->decl();
      procNode *proc = linker.lookup_procedure(call);
      if(!proc) proc = analyzer->synthetic_proc(call);
      if(! proc) {
        declNode *callee = ((idNode*)t->rhs1()->var())->decl();
        if(callee) {
          typeNode *type = callee->type()->follow_tdefs();
          if( type->typ() == Ptr ) {
            // call thru pointer. use lhs to determine value (correct? TBD)
            int lhs;
            size_operand(t->lhs(), lhs)
            return lhs;
          }
        }
      }
      if(! proc) return -1;
      return sizeOf( ((funcNode*)proc->decl()->type())->returns() );
    }

  } else if(! t->sizeof_type()) { // not function call, not sizeof()
    int rhs;
    size_operand(t->rhs1(), rhs)
    if(t->rhs2()) {
      assert(t->op());
      if(t->op()->is_comparison())
        rhs = sizeof(bool);
      else {
        assert(t->op()->is_arithmetic());
        int rhs2;
        size_operand(t->rhs2(), rhs2)
        if(rhs2 > rhs) rhs=rhs2; // take max ???
      }
    }
    return rhs;
  }
} // sizeOfAssign


string Unify_Size::str() const {
  if(_is_top) return "_top_";
  ostringstream ost;  ost << _size;
  return ost.str();
}

string Unify_Parents::str() const {
  if(_is_top) return "_top_";
  ostringstream ost;  ost << "{";
  for(set<Unify_ECR*>::const_iterator p=_parents.begin(); p!=_parents.end();
      p++) {
    if(p != _parents.begin()) ost << ",";
    ost << (*p)->type()->id();
  }
  ost << "}";
  return ost.str();
} // str


Unify_Structure::Unify_Structure(suespecNode *sue, EltMap _m, Unify_Size _s,
                                 Unify_Parents _p)
  : m(_m), s(_s), p(_p) {
  decl_list fields = sue->fields();
  assert(! fields.empty());
  if(sue->owner() == Struct) {
    for(decl_list_p f=fields.begin(); f!=fields.end(); f++) {
      declSet ds;  ds.insert(*f);
      fo.push_back(ds);
      fto.push_back((*f)->type());
    }
  } else {
    declSet ds;
    typeNode *type = NULL;
    for(decl_list_p d=fields.begin(); d!=fields.end(); d++) {
      ds.insert(*d);
      if(d==fields.begin())
        type = (*d)->type();
      else if(! UnificationBasedPtr::compatible_type((*d)->type(), type))
        type = NULL;
    }
    fo.push_back(ds);
    fto.push_back(type);
  }
} // Unify_Structure


Unify_ECR *Unify_Structure::get(declNode *field, Unify_ECR *container) {
#define new_field_ECR(X, type, container) {\
    set<Unify_ECR*> ps;  ps.insert(container->root()); \
    Unify_Parents p(ps); \
    if(type) { \
      Unify_Size size(Unify_Size::sizeOf(type)); \
      X = (new UnifyType( new Unify_Blank(size,p)))->ecr(); \
    } else { \
      Unify_Size size; \
      X = (new UnifyType( new Unify_Blank(size,p)))->ecr(); \
    }}

  for(FieldOrder::iterator f=fo.begin(); f!=fo.end(); f++)
    if((*f).find(field) != (*f).end()) {
      if(! m[*f])
        new_field_ECR(m[*f], field->type(), container)
      return m[*f];
    }
  return NULL;
} // get(decl)

string Unify_Structure::map_str() {
  ostringstream ost;
  ost << "map: ";
  for(map<declSet,Unify_ECR*>::iterator p=m.begin(); p!=m.end(); p++) {
    assert(! p->first.empty());
    ost << "\n  <";
    if(p->first.size() > 10) {
      declNode *first = * p->first.begin();
      ost << first->name() << "-" << first->coord() << ",etc...";
    } else
      for(set<declNode*>::iterator d=p->first.begin(); d!=p->first.end(); d++)
        ost << (*d)->name() << ",";
    ost << ">|->";
    if(p->second) ost << p->second->type()->id();
    else ost << "NULL";
  }
  ost << endl;
  return ost.str();
} // map_str


string Unify_Structure::all_str() {
  ostringstream ost;
  ost << "FO: ";
  for(FieldOrder::iterator p=fo.begin(); p!=fo.end(); p++) {
    assert(! (*p).empty());
    ost << "\n  <";
    if(p->size() > 10) {
      declNode *first = * p->begin();
      ost << first->name() << "-" << first->coord() << ",etc...";
    } else
      for(set<declNode*>::iterator d=p->begin(); d!=p->end(); d++)
        ost << (*d)->name() << "-" << *d << "-" << (*d)->coord() << ",";
    ost << ">,";
  }
  ost << endl;
  return ost.str() + map_str();
} // all_str

// ==============================================================

int Unify_ECR::id_count=0;
set<Unify_ECR*> Unify_ECR::all_ECR;

Unify_ECR::Unify_ECR(declNode *v, procNode *p)
: _var(v), _proc(p), _parent(0), _root(this), _ndecestors(0), _id(id_count++) {
  all_ECR.insert(this);

  Unify_Size size(Unify_Size::sizeOf(v->type()));
  if(v->type()->typ() == Array && v->init() && v->init()->typ()==Initializer)
    size = Unify_Size( ((initializerNode*)v->init())->exprs().size() *
                       Unify_Size::sizeOf(v->type()->type()) );
  Unify_Parents par(false); // empty set of parents
  _type = new UnifyType( new Unify_Blank(size,par), this );
  _type->ecr(this);
}

Unify_ECR::Unify_ECR(UnifyType *t)
: _var(NULL), _proc(NULL), _parent(0), _root(this), _ndecestors(0),
  _id(id_count++), _type(t) {
  all_ECR.insert(this);
}

UnifyType *Unify_ECR::type() { if(_parent) return root()->type(); return _type;}

void Unify_ECR::type(UnifyType *t) {
  assert(t);
  if(_parent) { root()->type(t); return; }
  _type = t;
  assert(!_root || _root==this);
  _root = Union(t->ecr());
  assert(_root == this);
}

Unify_ECR * Unify_ECR::root() {
  if(! _parent) return this;
  if(! _root->_parent) return _root;
  return _root = _root->root();
} // root

Unify_ECR * Unify_ECR::Union(Unify_ECR *other) {
  if(!other) return root();
  Unify_ECR *r1 = root(),
      *r2 = other->root();
  if(r1 == r2) return r1;
  if( r1->_ndecestors < r2->_ndecestors ) {
    // make r1 point to r2
    r1->_parent = r1->_root = r2;
    r2->_children.insert(r1);
    r2->_ndecestors += r1->_ndecestors;
    return r2;
  } else {
    r2->_parent = r2->_root = r1;
    r1->_children.insert(r2);
    r1->_ndecestors += r2->_ndecestors;
    return r1;
  }
} // Union


decl_list Unify_ECR::all_decls() const {
  decl_list ds;
  if(_var) ds.push_back(_var);
  for(set<Unify_ECR*>::const_iterator c=_children.begin(); c!=_children.end();
      c++) {
    decl_list more = (*c)->all_decls();
    ds.merge(more);
  }
  return ds;
} // all_decls

#define huge_pending 1400

#ifdef cx_vt
bool check_28920 = false;
#define parentsOf(t) \
  ((t->objTyp()==OBJECT) ? t->object()->p.parents() : \
   (t->objTyp()==SIMPLE) ? t->simple()->p.parents() : \
   (t->objTyp()==STRUCTURE) ? t->structure()->p.parents() : \
   (t->objTyp()==BLANK) ? t->blank()->p.parents() : set<Unify_ECR*>() );
#endif

void Unify_ECR::print() {
#ifdef cx_vt
if(check_28920) {
  cout << id() << /*" r=" << root()->id() <<*/ "/" << *_type << " ";
  set<Unify_ECR*> P = parentsOf(_type);
  bool contain_25976 = false;
  for(set<Unify_ECR*>::iterator p=P.begin(); p!=P.end(); p++)
    if((*p)->type()->id() == vt) { contain_25976 = true; break; }
  cout << vt << " " << contain_25976 << endl;
  if(_parent) _parent->print();
  return;
}
#endif

  cout << /*"Tao-" << */id() << " r=" << root()->id() << "/" << *type();
  if(! _pending.empty())
    cout << " " << _pending.size() << " pendings";
  set<Unify_Pending*> P = _pending.set();
  if(!P.empty() && P.size() <= 30) {
    cout << " {";
    for(Pendings_p p=P.begin(); p!=P.end(); p++) {
      if(p!=P.begin()) cout << " ";
      if((*p)->is_served()) cout << "s-";
      cout << **p;
    }
    cout << "} ";
  }
if(P.size() >= huge_pending) { // debug
for(Pendings_p p=P.begin(); p!=P.end(); p++) {
  cout << " {" << **p << "} " << endl;
  if((*p)->typ() == Unify_Pending::cjoin)
    cout << "   " << * (Unify_ECR*) (*p)->arg3() << endl;
}
 cout << "end huge " << P.size() << " pendings\n";
exit(1);
}
}

// ==============================================================

int UnifyType::id_count=0;

Unify_Size UnifyType::size() const {
  switch(obj_typ) {
    case BOTTOM: return Unify_Size(0);
    case SIMPLE: return simple()->s;
    case STRUCTURE: return structure()->s;
    case OBJECT: return object()->s;
    case BLANK: return blank()->s;
  }
} // size


void UnifyType::print() const {
  if(is_bottom()) {
    cout << id() << ":T_bot";
    goto remaining;
  }
  cout << id() << ":tao";
  switch(obj_typ) {
    case SIMPLE: {
      Unify_Simple *s = _tao.simple;
      cout << "(simple " << * s->alpha << ",";
      cout << * s->lambda << "," << s->s.str() <<","<< s->p.str() <<")";
      break;
    }
    case OBJECT: {
      Unify_Object *o = _tao.object;
      cout << "(object " << *o->alpha << ",";
      cout << * o->lambda << "," << o->s.str() <<","<< o->p.str();
      cout <<")";
      break;
    }
    case STRUCTURE: {
      Unify_Structure *s = _tao.structure;
      cout << "(struct {";
      for(EltMap::iterator m=s->m.begin(); m!=s->m.end(); m++)
        if(m->second) {
          cout << "<";
          declSet decls = m->first;
          for(declSet::iterator d=decls.begin(); d!=decls.end(); d++) {
            if(d!=decls.begin()) cout << ",";
            cout << (*d)->name();
          }
          cout << "->" << m->second->type()->id() << ">,";
        }
      cout << "}," << s->s.str() << "," + s->p.str() << ")";
      break;
    }
    case BLANK: {
      Unify_Blank *b = _tao.blank;
      cout << "(blank " << b->s.str() << "," + b->p.str() << ")";
    }
  }

remaining:
  if(block()) cout << " block=" << block();
  if(! _procs.empty()) {
    cout << " " << _procs.size() << " procs";
    cout << "(";
    for(set<procNode*>::iterator p=_procs.begin(); p!=_procs.end(); p++)
      cout << (*p)->decl()->name() << ",";
    cout << ")";
  }
} // print

// ==============================================================
class UnificationBasedPtr::findVarAssign : public Walker {
  // find number of times a variable is assigned to. The rhs must be a single
  // operand without addr.
public:
  map<declNode*,int>  &_assignments;
  map<declNode*,bool> &_uses;

  findVarAssign(map<declNode*,int> &assignments, map<declNode*,bool> &uses)
  : _assignments(assignments), _uses(uses), Walker(Preorder,Subtree) {}

  void at_threeAddr(threeAddrNode *ta, Order) {
    if(ta->rhs1() && ta->rhs1()->var()->typ()==Id)
      _uses[((idNode*) ta->rhs1()->var())->decl()] = true;
    if(ta->rhs2() && ta->rhs2()->var()->typ()==Id)
      _uses[((idNode*) ta->rhs2()->var())->decl()] = true;
    for(operand_list_p a=ta->arg_list().begin(); a!=ta->arg_list().end(); a++)
      if((*a)->var()->typ() == Id)
      _uses[((idNode*)(*a)->var())->decl()] = true;

    if(!ta->lhs() || ta->lhs()->var()->typ() != Id) return;
    if(ta->lhs()->star() || ta->lhs()->index() || !ta->lhs()->fields().empty()){
      _uses[ ((idNode*)ta->lhs()->var())->decl() ] = true;
      return;
    }
    declNode *lhs = ((idNode*) ta->lhs()->var())->decl();

    bool rhs1_is_func = false;
    if(ta->rhs1() && ta->rhs1()->var()->typ()==Id) {
      operandNode *rhs1 = ta->rhs1();
      if(! rhs1->addr() && !rhs1->star() && !rhs1->index() &&
         rhs1->fields().empty() &&
         ((idNode*)rhs1->var())->decl()->no_tdef_type()->typ() == Func &&
         // explanation of below: see comment mergeOperand() or at_call().
         (!rhs1->cast() || rhs1->cast()->follow_tdefs()->typ()==Func))
        rhs1_is_func = true;
    }

    if(!ta->rhs1() || ta->rhs2() || ta->op() || ta->rhs1()->addr() ||
       ta->rhs1()->var()->typ() == Const || rhs1_is_func ||
       lhs->decl_location() != declNode::BLOCK ||
       lhs->storage_class() != declNode::NONE)
      _assignments[ lhs ] = 2; // not what we want.
    else
      _assignments[ lhs ] ++;
  }
  void at_operand(operandNode *opr, Order) {
    if(opr->var()->typ() != Id) return;
    if(opr->addr() && !opr->index() && !opr->star() && opr->fields().empty())
      _assignments[ ((idNode*)opr->var())->decl() ] = 2; // not what we want.
  }
  void at_conditiongoto(conditiongotoNode *c, Order) {
    if(c->left() && c->left()->typ()==Id)
      _uses[((idNode*) c->left())->decl()] = true;
    if(c->right() && c->right()->typ()==Id)
      _uses[((idNode*) c->right())->decl()] = true;
  }
};

// ==============================================================


void UnificationBasedPtr::at_suespec(suespecNode *sue, Order ord) {
  if(ord==Postorder /*|| sue->owner() == Enum*/) return; // ignore
  if(_visited_sue.find(sue) != _visited_sue.end()) return;
  _visited_sue.insert(sue);

  // There could be multiple suespecNode for same struct/union definition (e.g.
  // when defined in a .h file included by multiple .c files). We want only
  // unique suespecNode, and their fields. Hence the hack:
  for(set<suespecNode*>::iterator s=_unique_sue.begin(); s!=_unique_sue.end();
      s++)
    if((*s)->owner() == sue->owner() &&
       (*s)->coord().to_string() == sue->coord().to_string() &&
       (*s)->fields().size() == sue->fields().size()) {
      // found a duplicate. But the field list could be different due to (eg.)
      // different #ifdef values. Go through list to check.
      decl_list fields1 = (*s)->fields(),
                fields2 = sue->fields();
      bool all_same = true;
      for(decl_list_p f1=fields1.begin(), f2=fields2.begin();
          all_same && f1!=fields1.end(); f1++, f2++)
        // must check both name and coord.
        if((*f1)->name()!=(*f2)->name() ||
          (*f1)->coord().to_string()!=(*f2)->coord().to_string())
          all_same = false;
      if(all_same) {
        for(decl_list_p f1=fields1.begin(), f2=fields2.begin();
            f1!=fields1.end(); f1++, f2++) {
          assert((*f1)->name() == (*f2)->name() &&
                 (*f1)->coord().to_string() == (*f2)->coord().to_string());
          _unique_field_defn[*f2] = *f1;
        }
        return;
      }
    }
  _unique_sue.insert(sue);

  bool is_struct = (sue->owner() == Struct);
  decl_list pos;
  for(decl_list_p d=sue->fields().begin(); d!=sue->fields().end(); d++) {
    _unique_field_defn[*d] = *d;
    _field2sue[*d] = sue;
    if(! is_struct) pos.clear();
    pos.push_back(*d);
    _fieldpos[*d] = pos;
  }
} // at_suespec


void UnificationBasedPtr::at_proc(procNode *p, Order ord) {
  if(ord==Postorder) { cur_proc = NULL;  return; }
  _analyzed_proc.insert(p);

  findVarAssign fva(_assignments, _uses);
  p->walk(fva);
  _assignments[ p->return_decl() ] = 2; // exclude return var.
  _uses[ p->return_decl() ] = true;

  declNode *decl = p->decl();

  // p could be a forward declaration.
  procNode *actual_p = linker.lookup_procedure(decl);
  if(actual_p && p != actual_p)
    return; // if proceed will create ecr for wrong return_decl.

  cur_proc = p;
  if(_ecr[decl]) return; // already done

//cout << "at_proc " << p->decl()->name() << " @" << p->coord() << endl;
  Unify_ECR *tao0 = _ecr[decl] = new Unify_ECR(decl, cur_proc);
  Unify_Size s(Unify_Size::sizeOf(decl->type()));

  funcNode *func = (funcNode*) decl->type();
  bool returns = ! func->returns()->is_void();
  decl_list formals = func->args();
  int n=formals.size(), m=returns?1:0;
  bool contain_ellipsis = false;
  // if no arg, in formals there is still a decl with type=void.
  if(n==1 && formals.front()->type()->is_void()) n=0;
  else if(n>0 && (formals.back()->type()->is_ellipsis() ||
                  is_va_list(formals.back()))) {
    contain_ellipsis = true;
  }

  ensure_sim_obj(tao0, s);

  // note: _proctype[p] must point to the new type after ensure_sim_obj().
  // But in order for createProcBlock() to work correctly, since _proctype[p]'s
  // ecr_no_root() is the new root of tao0, we must have _ecr[decl] point to
  // this new root, so that during Memory.lookup_variable(), we can use
  // _proctype[p] instead of old _ecr[decl]'s type(). Also must make sure the
  // type's ecr is the root.
  _proctype[p] = tao0->type();
  _ecr[decl] = tao0->root();
  tao0->type()->ecr( tao0->root() );

  UnifyType *taoT = tao0->type();
  is_Sim_Obj(taoT);
  Unify_Size s0 = Sim_Obj_Size(taoT);
  Lambda *lambda0 = Sim_Obj_Lambda(taoT);
  taoT->procs().insert(p);

  if(! s.leq(s0)) expand(tao0);

  if(lambda0->is_bottom()) {
    Unify_ECR **ecrs = (Unify_ECR**) malloc(n * sizeof(Unify_ECR*));
    for(int i=0; i<n; i++)
      ecrs[i] = T_bottom->ecr();
    settype(lambda0,n,m,ecrs, returns ? T_bottom->ecr() : NULL,
            contain_ellipsis);
  } else
    assert(lambda0->ellipsis() == contain_ellipsis);

  decl_list_p f=formals.begin();
  Unify_ECR *last = NULL;
  Unify_Size last_size;
  for(int i=0; i<n; i++, f++) {
    _uses[*f] = true; // mark it as used.
    assert(! _ecr[*f]); // ??
    if(i==n-1 && contain_ellipsis) {
      assert((*f)->type()->is_ellipsis() || is_va_list(*f));
      if(last) {
        assert(last->type()->objTyp()==BLANK);
        Unify_Blank *b = last->type()->blank();
        _ecr[*f] = (new UnifyType(new Unify_Blank(b->s,b->p)))->ecr();
      } else {
        _ecr[*f] = (new UnifyType(
                     new Unify_Blank(Unify_Size(),Unify_Parents(true))))->ecr();
      }
      _ecr[*f]->var(*f);
      _ecr[*f]->proc(cur_proc);
    } else {
      last_size = Unify_Size(Unify_Size::sizeOf((*f)->type()));
      _ecr[*f] = last = new Unify_ECR(*f, cur_proc);
    }
    cjoin(last_size, lambda0->tao(i), _ecr[*f]);
  }

  if(returns) {
    declNode *ret = cur_proc->return_decl();
    assert(ret);
    typeNode *ret_type = ret->no_tdef_type();
    Unify_Size ret_size = Unify_Size(Unify_Size::sizeOf(ret_type));
    assert(! _ecr[ret]); // ??
    Unify_ECR *ret_tao = _ecr[ret] = new Unify_ECR(ret, cur_proc);
    if(ret_type->typ() != Prim || annotation_returns_object(cur_proc))
      // new: ensure pointer
      ensure_sim_obj(ret_tao, ret_size);
    cjoin(ret_size, ret_tao, lambda0->taoR());
  }
} // at_proc

#define is_dismantler_tmp(name) ((name).find("__T") == 0)


void UnificationBasedPtr::at_decl(declNode *decl, Order ord) {
  if(ord==Preorder || inside_call_func)
    return;
  // also want to skip formals defined in typedef,extern,etc. how: for valid
  // decl, it's _ecr must already be created by at_proc. So, always skip.
  declNode::Decl_location loc = decl->decl_location();
  if(loc == declNode::FORMAL) return;

  // _ecr[decl] already created for proc's formals args and return_decl().
  // (see at_proc())
  if(_ecr.find(decl) != _ecr.end()) return;

  if(loc == declNode::ENUM) {
    _uses[decl] = true;
    assert(_unique_field_defn[decl]);
    if(decl != _unique_field_defn[decl]) {
      declNode *unique = _unique_field_defn[decl];
      at_decl(unique, Postorder);
      assert(_ecr[unique]);
      _ecr[decl] = _ecr[unique];
      return;
    }
  }

  bool is_synthetic;
  declNode *real = linker.lookup_symbol(cur_unit, decl->name(),is_synthetic);

  // deal with func declaration without definition.
  if(real && real!=decl && decl->type()->typ() == real->type()->typ() &&
     (loc == real->decl_location() || real->decl_location()==declNode::PROC) ||
     // note: need above because possible that real!=decl yet not EXTERN (below)
     decl->storage_class() == declNode::EXTERN ||
     decl->type()->typ()==Func && (!cur_proc || loc==declNode::BLOCK)) {
    // process the actual definition now, if not already.
    if(real && real!=decl && decl->type()->typ() == real->type()->typ() &&
       (loc == real->decl_location() || real->decl_location()==declNode::PROC)){
      at_decl(real, Postorder);
      if(_ecr.find(real) != _ecr.end()) {
        _ecr[decl] = _ecr[real];
      }
      return;
    } else if(decl->type()->typ() == Func) {
      // create synthetic procNode for it.
      procNode *new_proc = create_synthetic_proc(decl);
      procNode *old_cur_proc = cur_proc;
      at_proc(new_proc, Preorder);
      cur_proc = old_cur_proc;
      return;
    }
  }

  if(loc == declNode::TOP && decl->name()!="" /* not abstract decl */ )
    _uses[decl] = true;

  if ((loc == declNode::BLOCK /*|| loc == declNode::FORMAL*/ ||
       loc == declNode::ENUM ||
       loc == declNode::TOP /*|| loc == declNode::PROC*/) &&
      decl->storage_class() != declNode::TYPEDEF /*&&
      !(is_va_list(decl) || decl->type()->typ() == Prim &&
        (((primNode*)decl->type())->is_void() ||
         ((primNode*)decl->type())->is_ellipsis()))*/) {
    // note: proc decl are handled in at_proc().
    if( _assignments[decl] != 1 && _uses[decl] ) {
/*cout << "decl " << decl->name() << "@" << decl->coord() << " loc="
     << decl->decl_location() << " sc=" << decl->storage_class()
     << " type=" << decl->type()->typ() << endl;*/
assert(decl->name() != "");
      _ecr[decl] = new Unify_ECR(decl,cur_proc);
//cout << "at_decl " << decl <<" "<< decl->name() <<" @"<< decl->coord() <<endl;
      if(is_dismantler_tmp(decl->name())) _dismantler_tmp++;

      if(decl->no_tdef_type()->typ() == Array &&
         decl->decl_location() != declNode::SU) {
        // inspired by memoryModel::generate_su_field()
        Unify_Size s(Unify_Size::sizeOf(decl->type()));
        ensure_sim_obj(_ecr[decl], s);
      }

      if(loc==declNode::BLOCK && is_va_list(decl)) {
        // make decl points to the va_list formal parameter of cur proc.
        assert(cur_proc);
        decl_list formals = ((funcNode*)cur_proc->decl()->type())->args();
        assert(formals.size()>=1);
        declNode *last_arg = formals.back();
        assert(last_arg->type()->is_ellipsis() || is_va_list(last_arg));
        assert(_ecr[last_arg]);

        Unify_Size s(Unify_Size::sizeOf(decl->type()));
        ensure_sim_obj(_ecr[decl], s);
        join(Sim_Obj_Alpha(_ecr[decl]->type())->tao(), _ecr[last_arg]);
      }

      if(decl->init() && decl->init()->typ() == Initializer)
        at_initializer((initializerNode*) decl->init(),
                       decl->no_tdef_type(),
                       _ecr[decl]);

      else if(decl->no_tdef_type()->typ() == Array) {
#define ensure_parent_child(child, parent, child_type) { \
  switch(child->type()->objTyp()) { \
    case BOTTOM: { \
      Unify_Size child_size(Unify_Size::sizeOf(child_type)); \
      settype(child, new UnifyType(new Unify_Blank(child_size, \
                                                   Unify_Parents(false))));\
      /* fall through */ \
    } \
    case BLANK: child->type()->blank()->p.parents().insert(parent); break; \
    case SIMPLE: child->type()->simple()->p.parents().insert(parent); break; \
    case STRUCTURE: child->type()->structure()->p.parents().insert(parent); \
                    break; \
    case OBJECT: child->type()->object()->p.parents().insert(parent); \
  } }

        typeNode *curtype = decl->no_tdef_type();
        Unify_ECR *tao = _ecr[decl];
        while (curtype && (curtype->typ() == Array)) {
          exprNode *size = ((arrayNode*)curtype)->dim();
          curtype = curtype->no_tdef_type();
          Unify_Size array_size;
          if(size && size->typ()==Const)
            array_size = Unify_Size(((constNode*)size)->value().Integer()
                                    * Unify_Size::sizeOf(curtype));
          ensure_sim_obj(tao, array_size);
          Unify_ECR *child_tao = Sim_Obj_Alpha(tao->type())->tao();
          ensure_parent_child(child_tao, tao, curtype)

          if(curtype && curtype->typ() == Array)
            tao = child_tao;
        }
      }

      if(loc == declNode::TOP) annotation_init_global(decl);
#ifdef cx_vt
if(decl->name() == "switches") {
cout << "at_decl " << decl->name() << " " << *_ecr[decl] << endl;
debug = true;
vt = _ecr[decl]->type()->id();
vt_e = Sim_Obj_Alpha(_ecr[decl]->type())->tao()->type()->id();
}
#endif
    }
  }
} // at_decl


void UnificationBasedPtr::at_threeAddr(threeAddrNode *ta, Order ord) {
  if(ord==Postorder) {
    inside_call_func = false;
    return;
  }

/*if(! ta->coord().is_unknown()) {
string c = ta->coord().to_string();
int line = ta->coord().line();
if(c.find("vpath.c") < c.length())
  debug = (line>=500 && line<=918);
else debug=false;
} */

//if(debug) {
//cout << "at_threeAddr @" << ta->coord();
//cout << "  "; output_context oc(cout); ta->output(oc,NULL); cout << endl;
//}

  if(ta->op() && ta->op()->id() == Operator::FUNC_CALL) {
    Unify_Size s = Unify_Size(Unify_Size::sizeOfAssign(ta, linker, this));
    operandNode *call = ta->rhs1();
    assert(call->var()->typ() == Id);
    string call_name = ((idNode*)call->var())->name();
    if(! call->star() && (call_name=="malloc" || call_name=="calloc"))
      at_allocation(ta,s);
    else
      at_call(ta,s);
    inside_call_func = true;

  } else if(! ta->sizeof_type()) { // not function call, not sizeof()
    Unify_Size s = Unify_Size(Unify_Size::sizeOfAssign(ta, linker, this));
    assert(ta->lhs());
    if(ta->rhs1())
      mergeOperand(ta->lhs(), ta->rhs1(), s);
    if(ta->rhs2()) {
      mergeOperand(ta->lhs(), ta->rhs2(), s);
      // extra work if two rhs
      Unify_ECR *tao = ecr(ta->lhs(), s);
      if(tao) {
        ensure_sim_obj(tao, s);
        UnifyType *t = tao->type();
        is_Sim_Obj(t);
        Alpha *alpha = Sim_Obj_Alpha(t);
        if(alpha->offset()->value() == Unify_Offset::zero)
          make_unknown(alpha->offset());
      }
    }
  }
//print_ecr();
} // at_threeAddr



void UnificationBasedPtr::mergeOperand(operandNode *lhs, operandNode *rhs,
                                       Unify_Size s) {
  bool rhs_is_str = false;
  if(rhs->var()->typ() == Const) {
    return;
    rhs_is_str = ((constNode*)rhs->var())->value().is_str();
  }
  assert(! lhs->addr());

  Unify_ECR *lhs_ecr = NULL;
  if(!lhs->index() && !lhs->star() && lhs->fields().empty()) {
    // simple id on the lhs
    declNode *lhs_decl = ((idNode*) lhs->var())->decl();
    if(! _uses[lhs_decl]) return;
    if(_assignments[lhs_decl] == 1) {
      assert(lhs_decl->decl_location() == declNode::BLOCK);
      assert(_ecr.find(lhs_decl) == _ecr.end());
      assert(! rhs->addr());
      assert(rhs->var()->typ() == Id || rhs_is_str);
      _ecr[lhs_decl] = ecr(rhs, s);
      if(_ecr[lhs_decl]) {
        if(! _ecr[lhs_decl]->var() ||
           is_dismantler_tmp(_ecr[lhs_decl]->var()->name())) {
          _ecr[lhs_decl]->var( lhs_decl );
          if(! _ecr[lhs_decl]->proc() &&
             lhs_decl->decl_location()==declNode::BLOCK)
            _ecr[lhs_decl]->proc( cur_proc );
        }
      } else
        _ecr.erase(lhs_decl);
      return;
    } else
      lhs_ecr = ecr(lhs, s);
  } else
    lhs_ecr = ecr(lhs, s);
  assert(lhs_ecr);

  if(rhs->var()->typ() != Id && !rhs_is_str) return; // cannot merge

  bool rhs_is_func = false;
  if(! rhs->addr() && !rhs->star() && !rhs->index() && rhs->fields().empty() &&
     ((idNode*)rhs->var())->decl()->no_tdef_type()->typ() == Func &&
     // if there is a cast to something other than Func, do not treat as
     // Func. For example, if callee is f(char *tag) and all it wants to do
     // with tag is to, say, store in some record, we want to treat actual
     // argument of tag as a normal pointer.
     (!rhs->cast() || rhs->cast()->follow_tdefs()->typ()==Func))
       rhs_is_func = true;

  if(rhs->addr() || rhs_is_str || rhs_is_func) {
    // case "..=&(y..)"
    ensure_sim_obj(lhs_ecr, s);
    if(rhs_is_func) rhs->addr(true); // temporary
    Alpha *rhs_alpha = alpha(rhs, s);
    if(rhs_is_func) rhs->addr(false); // set back
    Unify_Size s1 = Sim_Obj_Size(lhs_ecr->type());
    if(! s.leq(s1)) expand(lhs_ecr);
    join(rhs_alpha, Sim_Obj_Alpha(lhs_ecr->type()));

  } else {
    // case not "..=&y"
    Unify_ECR *rhs_ecr = ecr(rhs, s);
    cjoin(s, rhs_ecr, lhs_ecr);
  }
} // mergeOperand


Unify_ECR *UnificationBasedPtr::ecr(operandNode *opr, Unify_Size s,
                                    Unify_Offset **offset) {
  if(!opr || opr->var()->typ() != Id) return NULL;
  assert(! opr->addr());
  declNode *d = ((idNode*)opr->var())->decl();
  // if(d->decl_location() == declNode::ENUM) return NULL;
//if(debug) {
//cout << "ecr("; output_context oc(cout); opr->output(oc,NULL); cout << ")\n";
//}
  Unify_ECR *E = ecr1(d);
  if(!E) return NULL;

  /* recall precedence: star, field, index, addr. */ \

  if(opr->star() && opr->fields().empty()) {
    // case when opr has star. Do not compute if has fields because E is
    // computed in different way.
    if(!opr->index())
      ensure_sim_obj(E, s);
    else { // dereference twice.
      idNode *id = (idNode*) opr->var();
      Unify_Size s1(Unify_Size::sizeOf(id->decl()->type()->type()));
      ensure_sim_obj(E, s1);
    }
    is_Sim_Obj(E->type());
    Alpha *alpha2 = Sim_Obj_Alpha(E->type());
    unless_zero(alpha2->offset(), alpha2->tao());
    E = alpha2->tao();
    if(offset) *offset = alpha2->offset();
  }
//cout << "star done\n";

  if(! opr->fields().empty()) {
    indexNode *index = opr->index();
    opr->index(NULL); // temporarily
    id_list fields = opr->fields();
    opr->fields().clear(); // temporarily
    typeNode *cast = opr->cast();
    opr->cast(NULL); // temporarily

    int base_size;
    size_operand(opr, base_size)
    Unify_Offset *offset2 = NULL;
    Unify_ECR *base = ecr(opr, base_size, &offset2);
    if(!offset2) offset2 = new Unify_Offset(Unify_Offset::zero);
    opr->fields() = fields; // set back
    opr->cast(cast);
//cout << "b4 field, base " << *base << endl;

    for(id_list_p f=fields.begin(); f!=fields.end(); f++) {
      /* Size base_Size(base_size);
      ensure_sim_obj(base, base_Size);
      opr_alpha = Sim_Obj_Alpha(base->type());
      ECR *tao2 = opr_alpha->tao(); */
      // in paper, compute tao2 from above because it uses "y->n" and base
      // is y. Here, we use '.' instead of "->", so tao2 is base.
      Unify_ECR *tao2 = base;
//cout << "field " << (*f)->name() << " base " << *base << endl;
//cout << "  tao2 " << *tao2 << endl;
      Unify_ECR *next_tao2 = NULL;
      Unify_Offset *next_offset2 = NULL;
      suespecNode *sue = field2sue((*f)->decl());
      assert(sue);
      if(offset2->value() == Unify_Offset::unknown) {
        collapse(tao2);
        next_tao2 = tao2->type()->object()->alpha->tao();//this & below are new
        if(! next_tao2)
          new_field_ECR( next_tao2, 0, tao2 )
        if(next_tao2 == tao2) break; // no point looping
        next_offset2 = new Unify_Offset(Unify_Offset::unknown);
      } else {
        declSet F;  F.insert((*f)->decl());
        unless_zero(offset2, tao2);
        if(tao2->type()->is_bottom() /* new */ ||
           tao2->type()->objTyp() == BLANK) {
          ensure_struct_obj(tao2, sue);
          /* wrong, cannot do make_compatible() and get() here, because tao2
           * now could be OBJECT due to its pendings being "served".
          make_compatible(F, st, tao2->root(), true);
          //opr_alpha= new Alpha(st->m[(*f)->decl()], new Offset(Offset::zero));
          next_tao2 = st->get(unique_field_defn((*f)->decl()), tao2);
          assert(next_tao2);
          next_offset2 = new Unify_Offset(Unify_Offset::zero); */
        } // else
        if(tao2->type()->objTyp() == STRUCTURE) {
          Unify_Structure *st = tao2->type()->structure();
          make_compatible(F, st, tao2, true);
          //opr_alpha= new Alpha(st->m[(*f)->decl()], new Offset(Offset::zero));
          next_tao2 = st->get(unique_field_defn((*f)->decl()), tao2);
          assert(next_tao2);
          next_offset2 = new Unify_Offset(Unify_Offset::zero);
        } else {
          ensure_struct_obj(tao2, sue);
          Unify_Size struct_size(Unify_Size::sizeOf(sue));
          promote(tao2, struct_size);
          // return opr_alpha;
          next_tao2 = tao2->type()->object()->alpha->tao();//this/below are new
          if(! next_tao2)
            new_field_ECR( next_tao2, 0, tao2)
          if(next_tao2 == tao2) break; // no point looping
          next_offset2 = new Unify_Offset(Unify_Offset::unknown);
        }
      }
      if(next_tao2) {
        base = next_tao2; // opr_alpha->tao();
        offset2 = next_offset2;
      }
      base_size = Unify_Size::sizeOf((*f)->decl()->type());
    }

    opr->index(index); // set back
    E = base; // opr_alpha->tao();
  }

  if(opr->index()) {
    // same as star?
    ensure_sim_obj(E, s);
    is_Sim_Obj(E->type());
    Alpha *alpha2 = Sim_Obj_Alpha(E->type());
    unless_zero(alpha2->offset(), alpha2->tao());
    if(offset && (*offset)->value() != Unify_Offset::unknown) {
      // note: do not explicitly set alpha2->offset() to be unknown. only
      // offset of opr need to be set to unknown if index is non-zero or
      // if alpha2->offset() is non-zero.
      // TBD: in broadway, when opr is not Const, can use constants (see
      // ptr analyzer) to check value.
      if(alpha2->offset()->value() == Unify_Offset::unknown ||
         opr->index()->typ()!=Const ||
         ((constNode*)opr->index())->value().Integer() != 0)
        (*offset)->value(Unify_Offset::unknown);
    }
    E = alpha2->tao();
  }

//if(debug) {
//output_context oc(cout);
//cout << "ecr("; opr->output(oc,NULL); cout << ") is " << *E << endl;
//}
  return E;
} // ecr


Unify_ECR *UnificationBasedPtr::ecr1(declNode *d) {
  Unify_ECR *E = _ecr[d];
  if(!E) {
    _ecr.erase(d); // clear empty entry.
    // this could happen if the var is actually a function or a global.
    if(d->type()->typ() == Func) {
      procNode *proc = linker.lookup_procedure(d);
      assert(proc); // ??
      procNode *old_cur_proc = cur_proc; // save
      at_proc(proc, Preorder);
      cur_proc = old_cur_proc; // set back
    } else if(_assignments[d] == 1 || !_uses[d])
      return NULL;
    else {
      assert(d->decl_location() == declNode::TOP ||
             d->decl_location() == declNode::ENUM);
      at_decl(d, Postorder);
    }
    E = _ecr[d];
    assert(E);
  }
  return E->root();
} // ecr1


Alpha *UnificationBasedPtr::alpha(operandNode *opr, Unify_Size s) {
//cout << "alpha("; output_context oc(cout); opr->output(oc,NULL); cout <<")\n";
  if(opr->var()->typ() == Const) {
    return NULL;
    constNode *con = (constNode*) opr->var();
    assert(con->value().is_str());
    if(! _string_alpha[con]) {
      int len = strlen(con->value().Str());
      Unify_ECR *E = (new UnifyType( new Unify_Blank(Unify_Size(len),
                                     Unify_Parents(false))))->ecr();
      _string_alpha[con] = new Alpha(E, new Unify_Offset(Unify_Offset::zero));
    }
    return _string_alpha[con];
  }

  assert(opr->var()->typ() == Id);
  assert(opr->addr());
  opr->addr(false); // temporarily

  Unify_Offset * offset = new Unify_Offset(Unify_Offset::zero);
  Unify_ECR *E = ecr(opr, s, &offset);
  Alpha *alpha = new Alpha(E, offset);

  opr->addr(true); // set back

//cout << "alpha("; opr->output(oc,NULL); cout << ") is " << *alpha << endl;
  return alpha;
} // alpha


void UnificationBasedPtr::at_allocation(threeAddrNode *ta, Unify_Size s) {
  if(_alloc[ta]) return;  // already handled.
  operandNode *lhs = ta->lhs();
  assert(lhs && lhs->var()->typ()==Id);
  declNode *lhs_decl = ((idNode*)lhs->var())->decl();
  Unify_ECR *x = _ecr[lhs_decl];
  if(! x) {
    // could be due to no uses of it. Force creation.
    _uses[lhs_decl] = true;
    _assignments[lhs_decl] = 2;
    bool i = inside_call_func;
    inside_call_func = false; // temporarily
    _ecr.erase(lhs_decl);
    at_decl(lhs_decl, Postorder);
    x = _ecr[lhs_decl];
    assert(x);
    inside_call_func = i;
  }
  ensure_sim_obj(x,s);
  UnifyType *t = x->type();
  is_Sim_Obj(t);
  Unify_Size s1 = Sim_Obj_Size(t);
  if(! s.leq(s1)) expand(x);
  Alpha *alpha1 = Sim_Obj_Alpha(t);
  if(alpha1->tao()->type()->is_bottom())
    settype(alpha1->tao(),
      new UnifyType(new Unify_Blank(Unify_Size(), Unify_Parents(false))));
  _alloc[ta] = alpha1->tao();
} // at_allocation


void UnificationBasedPtr::at_call(threeAddrNode *ta, Unify_Size s_m) {
  operandNode *call = ta->rhs1();
  assert(call->var()->typ() == Id);
  typeNode *call_expr_type = call->type()->follow_tdefs();
  bool is_func_ptr =
    call->star() || !call->fields().empty() || call->index() ||
    ((idNode*)call->var())->decl()->type()->follow_tdefs()->typ() == Ptr;
  declNode *callee = NULL;

  Unify_ECR *tao0;

  if(!is_func_ptr) {
    assert(call_expr_type->typ() == Func);
    Unify_Size s_c(Unify_Size::sizeOf(call_expr_type));
    tao0 = ecr(call, s_c);
    callee = ((idNode*)call->var())->decl();
    ensure_sim_obj(tao0, s_c);
  } else {
    Unify_Size s_c(Unify_Size::sizeOf(call_expr_type));
    tao0 = ecr(call, s_c);
    // need to dereference tao0 again to get the function.
    //if(tao0->type()->objTyp()==SIMPLE || tao0->type()->objTyp()==OBJECT)
    if(call_expr_type->typ() == Ptr) {
      call_expr_type = call_expr_type->type()->follow_tdefs();
      ensure_sim_obj(tao0, s_c);
      tao0 = Sim_Obj_Alpha(tao0->type())->tao();
    }
    assert(call_expr_type->typ() == Func);
    Unify_Size s_c1(Unify_Size::sizeOf(call_expr_type));
    ensure_sim_obj(tao0, s_c1);
  }

  UnifyType *taoT = tao0->type();
  is_Sim_Obj(taoT);
  Unify_Size s0 = Sim_Obj_Size(taoT);
  Lambda *lambda0 = Sim_Obj_Lambda(taoT);

  operand_list args = ta->arg_list();
  bool returns = ta->lhs();
  int n=args.size(), m=returns?1:0;

  bool is_synthetic_callee = false;
  procNode *proc = NULL;
  if(callee) {
    proc = linker.lookup_procedure(callee);
    if(!proc) {
      proc = _synthetic_proc[callee];
      assert(proc);
      is_synthetic_callee = true;
      // if(_skip_proc.find(proc) != _skip_proc.end()) return;
    }
  }

  if(!proc && lambda0->is_bottom()) {
    /*
    static bool func_ptr_error = false;
    if(!func_ptr_error) {
      cout << "Warning: call by function pointer, cannot resolve callee.\n";
      func_ptr_error = true;
    }*/
    _ptr_call[ta] = taoT;
    return;
  }
  if(!proc) _ptr_call[ta] = taoT; // update

  if(lambda0->is_bottom()) {
    assert(proc);
    funcNode *func = (funcNode*) proc->decl()->type();
    decl_list formals = func->args();
    int n=formals.size(), m=func->returns()->is_void()?0:1;
    Unify_ECR **ecrs = (Unify_ECR**) malloc(n * sizeof(Unify_ECR*));
    for(int i=0; i<n; i++)
      ecrs[i] = T_bottom->ecr();
    bool contain_ellipsis =
      !formals.empty() && 
      (is_va_list(formals.back()) || formals.back()->type()->is_ellipsis());
    settype(lambda0,n,m,ecrs, returns ? T_bottom->ecr() : NULL,
            contain_ellipsis);
  }

  if(is_func_ptr) {
    // is the target malloc?
    set<procNode*> procs = taoT->procs();
    for(set<procNode*>::iterator p=procs.begin(); p!=procs.end(); p++)
      if((*p)->decl()->name()=="malloc" || (*p)->decl()->name()=="calloc") {
        at_allocation(ta, s_m);
        break;
      }
  }

  // when procedure is synthetic...
  if(is_synthetic_callee) {
    // conservatively merge all operands. ??
    if(n>1) {// else what to do ??
      //cout << "Warning: unknown definition for procedure " << callee->name()
      //     << ", optimistically skipped call site's parameters.\n";
      // if(proc) _skip_proc.insert(proc);
      // note: we still want to run ecr()/alpha() on the arguments.
    }
    if(lambda0->n() < n)
      // we need more argument.
      lambda0->addArg(n - lambda0->n());
  }

  if(n == lambda0->n() || n+1>=lambda0->n() && lambda0->ellipsis() ||
     taoT->procs().size()>1) ; //okay
  else {
    cout << "Warning: more argument at callsite at " << ta->coord()
         << " than callee";
    if(callee) cout << " definition at " << callee->coord();
    cout << endl;
  }

  int i=0;
  for(operand_list_p arg=args.begin(); arg!=args.end(); arg++) {
    bool arg_is_str = false,
         arg_is_func = false;
    if((*arg)->var()->typ() == Const)
      ; //arg_is_str = ((constNode*)(*arg)->var())->value().is_str();
    else if(! (*arg)->addr()) {
      // no & in argument, but still possible it is a function ptr.
      typeNode *cast = (*arg)->cast();
      (*arg)->cast(NULL); // temporarily
      typeNode *arg_type = (*arg)->type()->follow_tdefs();
      (*arg)->cast(cast);
/*if(arg_type->typ() == Func && (*arg)->cast() &&
   ((*arg)->cast()->follow_tdefs()->typ()!=Ptr ||
    (*arg)->cast()->no_tdef_type()->typ()!=Func)) {
  cout << ta->coord() << endl;
  cout << "here. debug cast.\n";
  assert(false);
}*/
      if(arg_type->typ() == Func &&
         // hack: if there is a cast to something other than ptr to Func, do
         // not treat as Func. For example, if callee is f(char *tag) and all
         // it wants to do with tag is to, say, store in some record, we want
         // to treat actual argument of tag as a normal pointer.
         (!(*arg)->cast() ||
            (*arg)->cast()->follow_tdefs()->typ()==Ptr &&
            (*arg)->cast()->follow_tdefs()->no_tdef_type()->typ()==Func))
        arg_is_func = true;
    }

    if(i<lambda0->n() && ((*arg)->var()->typ() == Id || arg_is_str)) {
      Unify_Size s_i = Unify_Size(Unify_Size::sizeOf((*arg)->type()));
      Unify_ECR *lambda_tao = NULL;
      lambda_tao = lambda0->tao(i);

      if((*arg)->addr() || arg_is_str || arg_is_func) {
        if(arg_is_func) (*arg)->addr(true); // temporary
        // case "..=&y"
        Alpha *arg_alpha = alpha(*arg, s_i);
        if(lambda_tao) {
          ensure_sim_obj(lambda_tao, s_i);
          Unify_Size s1 = Sim_Obj_Size(lambda_tao->type());
          if(! s_i.leq(s1)) expand(lambda_tao);
          join(arg_alpha, Sim_Obj_Alpha(lambda_tao->type()));
        }
        annotation_call_arg(proc, i, (*arg)->type(), arg_alpha);

      } else {
        // case not "..=&y"
        Unify_ECR *arg_ecr = ecr(*arg, s_i);
        if(arg_ecr && lambda_tao) {
          cjoin(s_i, arg_ecr, lambda_tao);
        }
        annotation_call_arg(proc, i, (*arg)->type(), arg_ecr);
      }
    }
    if(arg_is_func) (*arg)->addr(false); // set back

    if(i+1 < lambda0->n()) i++;
    else if(! lambda0->ellipsis()) break; // error. don't do further.
  }

  if(returns) {
    if(lambda0->m()==0) {
      cout << "Warning: callsite at " << ta->coord() << " expecting return "
           << "value (due to cfg?) when callee has no return value";
      if(proc) cout << "(definition at " << proc->coord() << ")";
      cout << ".\n";

    } else {
      assert(ta->lhs()->var()->typ() == Id);
      declNode *ret = ((idNode*)ta->lhs()->var())->decl();
      Unify_ECR *ret_tao = NULL;
      if(_ecr.find(ret) != _ecr.end())
        ret_tao = _ecr[ret];
      if(ret->no_tdef_type()->typ() != Prim ||
         proc && annotation_returns_object(proc) ||
         !lambda0->taoR()->type()->is_bottom() &&
         lambda0->taoR()->type()->objTyp()!=BLANK) {
        // new: ensure pointer
        Unify_Size ret_size =
          Unify_Size(Unify_Size::sizeOf(ret->no_tdef_type()));
        if(ret_tao)
          ensure_sim_obj(ret_tao, ret_size);
        annotation_ret_val(proc, lambda0->taoR(), cur_unit);
      }
      Unify_Size s_m = Unify_Size(Unify_Size::sizeOfAssign(ta, linker, this));
      if(ret_tao)
        cjoin(s_m, lambda0->taoR(), ret_tao);
    }
  }
} // at_call


void UnificationBasedPtr::at_initializer(initializerNode *init,
                                         typeNode *init_type,
                                         Unify_ECR *init_ecr) {
  // inspired by memoryModel::generate_array_elements_for()
  typeNode * curtype = init_type->follow_tdefs();
  NodeType typ = curtype->typ();
  if(typ!=Struct && typ!=Union && typ!=Array) return; // nothing to do
  Unify_ECR *tao = init_ecr;
  assert(tao);
  expr_list inits;
  inits.push_back(init);
//cout << "at_initializer @" << init->coord() << " tao " << *tao << endl;

  while (curtype && (curtype->typ() == Array)) {
    // -- Move down in the initializer structure (probably not the most
    // efficient algorithm).

    expr_list new_inits;
    for (expr_list_p ip = inits.begin(); ip != inits.end(); ++ip) {
      exprNode * expr = (*ip);
      if (expr->typ() == Initializer) {
        initializerNode * initializer = (initializerNode *) expr;
        new_inits.insert(new_inits.end(), initializer->exprs().begin(),
                         initializer->exprs().end());
      }
    }

    inits = new_inits;
    curtype = curtype->no_tdef_type();
    Unify_Size array_size(inits.size() * Unify_Size::sizeOf(curtype));
    ensure_sim_obj(tao, array_size);
    Unify_ECR *child_tao = Sim_Obj_Alpha(tao->type())->tao();
    ensure_parent_child(child_tao, tao, curtype)

    if(curtype && curtype->typ() == Array)
      tao = child_tao;
  }

  if (curtype && ! inits.empty()) {

    if (curtype->typ() == Struct || curtype->typ() == Union) {
      // Initialize a struct, as if "a.f = elt". Here, treat "a" as it has
      // offset zero, since it is actually "a[i]" which we know points to a
      // valid starting address of a struct. With this, can skip check on
      // type(offset2)==unknown below.
      Unify_ECR *tao2;
      if(curtype == init_type) // the init is struct init, no array.
        tao2 = init_ecr;
        // ignore unless_zero
      else {
        Alpha *alpha2 = Sim_Obj_Alpha(tao->type()); // array element a[i]
        tao2 = alpha2->tao();
        Unify_Offset *offset2 = alpha2->offset();
        unless_zero(offset2, tao2);
      }

      suespecNode * spec = ((sueNode*)curtype)->spec();
      at_suespec(spec, Preorder); // make sure
      decl_list & fields = spec->fields();

      // make sure same field of all elements are joined.
      map<declNode*,Unify_ECR*> rhs_unique;

      for (expr_list_p ip = inits.begin(); ip != inits.end(); ++ip) {
        exprNode * expr = (*ip);
        if (expr->typ() != Initializer) continue;
        initializerNode * initializer = (initializerNode *) expr;
        expr_list & field_inits = initializer->exprs();

        // -- Loop over the fields and the field inits together
        decl_list_p fp = fields.begin();
        expr_list_p fip = field_inits.begin();
        while((fp != fields.end()) && (fip != field_inits.end())) {
          declNode * field_decl = (*fp);
          exprNode * field_init = (*fip);

          // note: different treatment as in
          // memoryModel::generate_array_elements_for(), which assumes the
          // expression is a compiler-time static constant. Here we also handle
          // ADDRESS operator, useful if the expression is address of a func.
          idNode * id = 0;
          Unify_ECR *rhs_ecr = NULL;
          bool rhs_addr = false;
          typeNode *rhs_type = NULL;
          if (field_init->typ() == Cast) { // 20030903
            rhs_type = ((castNode*) field_init)->type();
            field_init = ((castNode*) field_init)->expr();
          }
          if (field_init->typ() == Id)
            id = (idNode *) field_init;
          else if (field_init->typ() == Unary) {
            unaryNode * un = (unaryNode *) field_init;
            if (un->expr()->typ() == Id) 
              id = (idNode *) un->expr();
            rhs_addr = (un->op()->id() == Operator::ADDRESS);
          } else if(field_init->typ() == Const && 0 &&
                    ((constNode*)field_init)->value().is_str()) {
            constNode *con = (constNode*) field_init;
            if(! _string_alpha[con]) {
              int len = strlen(con->value().Str());
              Unify_ECR *E = (new UnifyType( new Unify_Blank(Unify_Size(len),
                                             Unify_Parents(false))))->ecr();
              _string_alpha[con] = new Alpha(E,
                                          new Unify_Offset(Unify_Offset::zero));
            }
            rhs_ecr = _string_alpha[con]->tao();
            rhs_addr = true;
          }
          if(id && id->decl() /*&&
             id->decl()->decl_location() != declNode::ENUM*/) {
            typeNode *field_type = field_decl->type();
            if(field_type->typ() == Struct ||
               field_type->typ() == Union ||
               field_type->typ() == Enum)
              at_suespec(((sueNode*)field_type)->spec(), Preorder);
            rhs_ecr = ecr1(id->decl());
            if(id->decl()->type()->typ() == Func)
              rhs_addr = true; //treat func as addr
          }

          if(rhs_ecr) {
            declSet F;  F.insert(field_decl);
            Unify_Structure *st;
            if(tao2->type()->is_bottom() /* new */ ||
               tao2->type()->objTyp() == BLANK) {
              suespecNode *sue = field2sue(field_decl);
              assert(sue);
              ensure_struct_obj(tao2, sue);
              // make_compatible(F, st, tao2, true);
            }
            if(tao2->type()->objTyp() == STRUCTURE) {
              st = tao2->type()->structure();
              make_compatible(F, st, tao2, true);
            } else assert(false); // possible?
            Unify_ECR *field_ecr = st->get(unique_field_defn(field_decl),
                                           tao2);
            assert(field_ecr);

            if(! rhs_addr) {
              Unify_Size rhs_size(Unify_Size::sizeOf(rhs_type? rhs_type
                                                             : id->type()));
              cjoin(rhs_size, rhs_ecr, field_ecr);
              // make sure all init for same field are joined.
              if(!rhs_unique[field_decl]) rhs_unique[field_decl] = rhs_ecr;
              else join(rhs_unique[field_decl], rhs_ecr);
            } else {
              Unify_Size size(sizeof(int*)); // ??? address size
              ensure_sim_obj(field_ecr, size);
              UnifyType *field_t = field_ecr->type();
              if(! size.leq(Sim_Obj_Size(field_t)))
                expand(field_ecr);
              Alpha *rhs_alpha = new Alpha(rhs_ecr, new Unify_Offset(
                                                      Unify_Offset::zero));
              join(rhs_alpha, Sim_Obj_Alpha(field_ecr->type()));
              delete rhs_alpha;
            }

          } else if(tao2->type()->is_bottom() ||
                    tao2->type()->objTyp() == BLANK) {
            // there are many reasons there is no rhs_ecr.
            suespecNode *sue = field2sue(field_decl);
            assert(sue);
            ensure_struct_obj(tao2, sue);
          }

          fp++;
          fip++;
        }
      }

    } else {
//cout << "Array\n";
      // Initialize an array, as if "*array = elt";
      Alpha *alpha3 = Sim_Obj_Alpha(tao->type()); // array element
      Unify_ECR *tao3 = alpha3->tao();
      unless_zero(alpha3->offset(), tao3);

      Unify_ECR *rhs_unique=NULL; // make sure all elements are joined.

      for (expr_list_p p = inits.begin(); p != inits.end(); ++p) {
        exprNode * the_init = (*p);
        idNode * id = 0;
        Unify_ECR *rhs_ecr = NULL;
        bool rhs_addr = false;
        typeNode *rhs_type = NULL;

        if (the_init->typ() == Cast) { // 20030903
          rhs_type = ((castNode*) the_init)->type();
          the_init = ((castNode*) the_init)->expr();
        }
        if (the_init->typ() == Id) id = (idNode *) the_init;
        else if (the_init->typ() == Unary) {
          unaryNode * un = (unaryNode *) the_init;
          if (un->expr()->typ() == Id)
            id = (idNode *) un->expr();
          rhs_addr = (un->op()->id() == Operator::ADDRESS);
        } else if(the_init->typ() == Const && 0 &&
                  ((constNode*)the_init)->value().is_str()) {
          constNode *con = (constNode*) the_init;
          if(! _string_alpha[con]) {
            int len = strlen(con->value().Str());
            Unify_ECR *E = (new UnifyType( new Unify_Blank(Unify_Size(len),
                                           Unify_Parents(false))))->ecr();
            _string_alpha[con] = new Alpha(E,
                                        new Unify_Offset(Unify_Offset::zero));
          }
          rhs_ecr = _string_alpha[con]->tao();
          rhs_addr = true;
        }
        if (id && id->decl() /*&&
            id->decl()->decl_location() != declNode::ENUM*/) {
          rhs_ecr = ecr1(id->decl());
          if(id->decl()->type()->typ() == Func)
            rhs_addr = true; //treat func as addr
        }

        if (rhs_ecr) {
          if(! rhs_addr) {
            Unify_Size rhs_size(Unify_Size::sizeOf(rhs_type? rhs_type
                                                           : id->type()));
            cjoin(rhs_size, rhs_ecr, tao3);
            if(!rhs_unique) rhs_unique = rhs_ecr;
            else join(rhs_unique, rhs_ecr);
          } else {
            Unify_Size size(sizeof(int*)); // ??? address size
            ensure_sim_obj(tao3, size);
            UnifyType *tao3_t = tao3->type();
            if(! size.leq(Sim_Obj_Size(tao3_t)))
              expand(tao3);
            Alpha *rhs_alpha = new Alpha(rhs_ecr, new Unify_Offset(
                                                    Unify_Offset::zero));
            join(rhs_alpha, Sim_Obj_Alpha(tao3->type()));
            delete rhs_alpha;
          }

        } else if(curtype->follow_tdefs()->typ() == Ptr &&
                  tao3->type()->is_bottom()) {
          // there are many reasons there is no rhs_ecr. ensure tao3 is a
          // pointer type.
          Unify_Size size(sizeof(int*)); // ??? address size
          ensure_sim_obj(tao3, size);
          Unify_ECR *child = Sim_Obj_Alpha(tao3->type())->tao();
          ensure_parent_child(child, tao3, curtype)
        }
      } // inits
    } // Array
  }
} // at_initializer


// ==============================================================

void UnificationBasedPtr::settype(Unify_ECR *e, UnifyType *t) {
  if(e->type() == t) return;
  if(e->type()->block()) {
    UnifyType *old_type = e->type();
    assert(! t->block() || old_type->block() == t->block()); // ??
    t->block(old_type->block());
    old_type->block(NULL);
    t->block()->unifyType(t);
  }
  e->type(t);

  set<Unify_Pending*> pending = e->pending().set();
  //e->pending().clear();
bool big = pending.size() > huge_pending;
if(big) cout << "settype " << pending.size() << " pendings\n";
int not_served = 0;
  for(Pendings_p a=pending.begin(); a!=pending.end(); a++) {
    if((*a)->is_served()) continue;
not_served++;
    if(finalizing) (*a)->served();
    switch((*a)->typ()) {
      case Unify_Pending::join_ECR:
if(big)
 cout<< "join_ECR "<< *(Unify_ECR*) (*a)->arg1() <<"+"<< * (Unify_ECR*) (*a)->arg2() <<endl;
        join((Unify_ECR*) (*a)->arg1(), (Unify_ECR*) (*a)->arg2());
        break;
      case Unify_Pending::join_Lambda:
if(big)
  cout << "join_Lambda " << *(Lambda*) (*a)->arg1() <<"+"
       << *(Lambda*) (*a)->arg2() << endl;
        join((Lambda*) (*a)->arg1(), (Lambda*) (*a)->arg2());
        break;
      case Unify_Pending::cjoin:
if(big)
  cout << "cjoin " << ((Unify_Size*)(*a)->arg1())->size() << " "
       << *(Unify_ECR*) (*a)->arg2() <<"+"<< *(Unify_ECR*) (*a)->arg3() << endl;
        cjoin(* (Unify_Size*) (*a)->arg1(), (Unify_ECR*) (*a)->arg2(),
              (Unify_ECR*) (*a)->arg3());
        break;
      default: cout << (*a)->typ() << endl; assert(false);
    }
  }
if(big) {
  cout << "not_served = " << not_served << endl;
  assert(false);
}
} // settype


void UnificationBasedPtr::settype(Lambda *l, int n, int m, Unify_ECR **t,
                                  Unify_ECR *r, bool ellipsis) {
  l->settype(n,m,t,r,ellipsis);
  set<Unify_Pending*> pendings = l->pending().set();
  //_pending.clear();
  for(Pendings_p p=pendings.begin(); p!=pendings.end(); p++) {
    switch((*p)->typ()) {
      case Unify_Pending::join_Lambda:
        join((Lambda*) (*p)->arg1(), (Lambda*) (*p)->arg2());
        break;
      default: assert(false);
    }
  }
} // settype(Lambda)


void UnificationBasedPtr::join(Alpha *a1, Alpha *a2, bool *recursion_detected) {
  if(a1 == a2) return;
  if(a1->offset()->value() == Unify_Offset::zero) {
    if(new_pending) {
      a1->offset()->pending() = a2->offset()->pending();
      a1->offset()->pending().insert( new Unify_Pending(a2->offset()),true );
    }
  } else if(a2->offset()->value() == Unify_Offset::zero)
    make_unknown(a2->offset());
  join(a1->tao(), a2->tao(), recursion_detected);
} // join


void UnificationBasedPtr::join(Unify_ECR *e1, Unify_ECR *e2,
                               bool *recursion_detected) {
  assert(e1); assert(e2);
  if(e1 == e2) return;

  if(e1->root()==e2->root()) {
    if(!finalizing) return;
    // during finalize, continue even if e1,e2 share same root, because we need
    // to make sure their pendings are merged into the root (see below).
    // note: can only do this during finalize, else serving pendings without
    // marking them as served could lead to infinite loop.
  }

  // prevent infinite recursion.
  static set<pair<UnifyType*,UnifyType*> > stack;
  pair<UnifyType*,UnifyType*> ee(e1->type(),e2->type());
  if(stack.find(ee) != stack.end()) {
//cout << "join recursion\n";
    if(recursion_detected) *recursion_detected = true;
    return; // recursion detected.
  }
  stack.insert(ee);
  if(recursion_detected) *recursion_detected = false;

static int ID = 0; // debug
int _id = ID++;
/*if(debug) {
cout << _id << " ";
cout << "join " << *e1 << " + " << *e2 << endl;
}*/
  bool force_no_pending = false;
  if(new_pending && finalizing)
    // we have already completed finalize, now in ecrDeref or ecrField.
    // we want to create new pending for bottom e1 only if e2 is also pending;
    // else do join immediately.
    force_no_pending = e1->type()->is_bottom() && e2->type()->is_bottom();

  if(new_pending && e1->type()->is_bottom() && !force_no_pending) {
    e1->pending().insert( new Unify_Pending(e1,e2),true );
  } else {
set<procNode*> e1_procs = e1->type()->procs(),
               e2_procs = e2->type()->procs();
    UnifyType *e1_type = e1->type(),
              *e2_type = e2->type(),
              *new_type = unify(e1_type,e2_type); // have to do this first
    if(new_type == NULL) return; // cannot unify
    if(e1->type() != e1_type || e2->type() != e2_type) {
      // e1 or e2's type has changed during call to unify(). We can no longer
      // trust new_type to be the correct final type of their joins.
      join(e1, e2);
      return;
    }

    bool e1_changed = new_type!=e1->type(),
         e2_changed = new_type!=e2->type();
    e1->root()->pending().cleanup();
    if(e1->root() != e2->root()) e2->root()->pending().cleanup();
    set<Unify_Pending*> e1_pendings = e1->root()->pending().set(),
                        e2_pendings = e2->root()->pending().set();
    Unify_ECR *e = e1->Union(e2);
    // if(! finalizing) {
      set<Unify_Pending*> new_pendings;
      if(e==e1)      new_pendings = e2_pendings;
      else if(e==e2) new_pendings = e1_pendings;
      else {
        new_pendings = e1_pendings;
        new_pendings.insert(e2_pendings.begin(), e2_pendings.end());
      }
      for(Pendings_p p=new_pendings.begin(); p!=new_pendings.end(); p++)
        e->pending().insert(*p,false);

      if(serve_again) { // new
        // we are in finalizing mode, though we may have temporarily set the
        // flag off in cjoin().
        if(e1_changed)
          for(Pendings_p p=e1_pendings.begin(); p!=e1_pendings.end(); p++)
            if((*p)->is_served()) {
              (*p)->un_served();
              more_pending.insert(*p);
            }
        if(e2_changed)
          for(Pendings_p p=e2_pendings.begin(); p!=e2_pendings.end(); p++)
            if((*p)->is_served()) {
              (*p)->un_served();
              more_pending.insert(*p);
            }
      }
    // }
    // wrong: e->type(e1->type());
    settype(e, new_type);
set<procNode*> e_procs = e->type()->procs();
for(set<procNode*>::iterator p=e1_procs.begin(); p!=e1_procs.end(); p++)
  if(e_procs.find(*p) == e_procs.end()) {
cout << _id << " ";
    cout << (*p)->decl()->name() << endl;
    cout << *e1 << "\n + " << *e2 << "\n -> " << *e << endl;
    cout << "new_type " << *new_type << endl;
    assert(false);
  }
for(set<procNode*>::iterator p=e2_procs.begin(); p!=e2_procs.end(); p++)
  if(e_procs.find(*p) == e_procs.end()) {
cout << _id << " ";
    cout << (*p)->decl()->name() << endl;
    cout << *e1 << "\n + " << *e2 << "\n -> " << *e << endl;
    cout << "new_type " << *new_type << endl;
    assert(false);
  }
/*if(debug) {
cout << _id << " ";
cout << "joined " << *e << endl;
}*/

  }
  stack.erase(ee);
} // join


void UnificationBasedPtr::join(Lambda *l1, Lambda *l2) {
  assert(l1 && l2);
  if(l1 == l2) return;
//cout << "join Lambdas " << *l1 << " + " << *l2 << endl;
  if(l1->is_bottom() && new_pending) {
    l1->pending().insert( new Unify_Pending(l1,l2),true );
  } else if(l2->is_bottom() && new_pending) {
    l2->pending().insert( new Unify_Pending(l1,l2),true );
  } else if(!l1->is_bottom() && !l2->is_bottom()) {
    // this join() is called from cjoin() and unify(). What happen there is
    // that two types of are cjoin/unify and the two types involves have Lambda
    // to be joined. Those two types are either SIMPLE or OBJECT, which means
    // they are pointers to some functions (l1 and l2). In C, it is possible
    // that function pointers may point to functions of different signatures.
    // So l1 and l2 need not have same n() and m().
    l2->addArg(l1->n() - l2->n());
    l1->addArg(l2->n() - l1->n());
    int t = l1->n() < l2->n() ? l1->n() : l2->n();
    for(int i=0; i<t; i++) {
      Unify_ECR *e1 = l1->tao(i),
                *e2 = l2->tao(i);
      if(! e1->type()->is_bottom())
        join(e1, e2);
      else if(! e2->type()->is_bottom())
        join(e2, e1);
      else {
        join(e1, e2);
        // rightfully, join both ways because we don't know which one will
        // become non-bottom first, and we want to make sure the pendings will
        // lead one to become non-bottom when the other is changed.  Instead of
        // call join() which will have no effect, explicitly add pending.
        e2->pending().insert( new Unify_Pending(e2,e1),true );
      }
      l1->tao(i,e1->root());  l2->tao(i,e1->root());
    }

    if(l1->m()>0 && l2->m()>0) {
      Unify_ECR *e1 = l1->taoR(),
                *e2 = l2->taoR();
      if(! e1->type()->is_bottom())
        join(e1, e2);
      else if(! e2->type()->is_bottom())
        join(e2, e1);
      else {
        join(e1, e2);
        e2->pending().insert( new Unify_Pending(e2,e1),true );
      }
      l1->taoR(e1->root());  l2->taoR(e1->root());
    }

    if(finalizing) {
      set<Unify_Pending*> pendings = l1->pending().set(),
                          more = l2->pending().set();
      pendings.insert(more.begin(), more.end());
      for(Pendings_p p=pendings.begin(); p!=pendings.end(); p++) {
        if((*p)->is_served()) continue;
        (*p)->served();
        switch((*p)->typ()) {
          case Unify_Pending::join_Lambda:
            join((Lambda*) (*p)->arg1(), (Lambda*) (*p)->arg2());
            break;
          default: assert(false);
        }
      }
    }

  } else if(!l1->is_bottom() || !l2->is_bottom()) {
    assert(! new_pending);
    Lambda *bot, *non_bot;
    if( l1->is_bottom() ) { bot = l1; non_bot = l2; }
    else { bot = l2; non_bot = l1; }
    Unify_ECR **taos = (Unify_ECR**) malloc(non_bot->n() * sizeof(Unify_ECR*));
    for(int i=0; i<non_bot->n(); i++) taos[i] = non_bot->tao(i);
    settype(bot, non_bot->n(), non_bot->m(), taos,
            non_bot->m()>0 ? non_bot->taoR() : NULL, non_bot->ellipsis());
  }
//cout << "joined Lambdas " << *l1 << " + " << *l2 << endl;
} // join


void UnificationBasedPtr::ensure_sim_obj(Unify_ECR *tao, Unify_Size &s) {
  if(tao->type()->is_bottom()) {
    Unify_Parents par(false); // empty set of parents
    settype(tao, new UnifyType(new Unify_Simple(new Alpha(), L_bottom,
                                                s, par)));
    return;
  }
  switch(tao->type()->objTyp()) {
    case BLANK: {
      Unify_Blank *b = tao->type()->blank();
      settype(tao, new UnifyType(new Unify_Simple(new Alpha(), L_bottom,
                                                  b->s, b->p)));
      if(! s.leq(b->s)) expand(tao);
      break;
    }
    case SIMPLE: {
      Unify_Simple *si = tao->type()->simple();
      if(! s.leq(si->s)) expand(tao);
      break;
    }
    case STRUCTURE: {
      Unify_Structure *st = tao->type()->structure();
      promote(tao, st->s);
    }
    // case OBJECT:
  }
} // ensure_sim_obj


void UnificationBasedPtr::ensure_no_bottom(Unify_ECR *tao, declNode *decl,
                                           UnifyType *parent) {
  if(! tao->type()->is_bottom()) return;

  Unify_Size s;
  if(decl && decl->type()) s = Unify_Size(Unify_Size::sizeOf(decl->type()));
  Unify_Parents p(false);
  if(parent) p.parents().insert(parent->ecr());
  settype(tao, new UnifyType(new Unify_Blank(s, p)));
} // ensure_no_bottom


Unify_Structure *UnificationBasedPtr::ensure_struct_obj(Unify_ECR *tao,
                                                        suespecNode *sue,
                                                        bool redo_pending) {
  assert(sue);
  Object_Typ typ = tao->type()->objTyp();
  if(typ == STRUCTURE) return tao->type()->structure();
  if(tao->type()->is_bottom() || typ == BLANK) {
    Unify_Structure *st;
    if(tao->type()->is_bottom()) {
      Unify_Size struct_size(Unify_Size::sizeOf(sue));
      st = new Unify_Structure(sue, EltMap(), struct_size,
                               Unify_Parents(false));
    } else {
      Unify_Blank *blank = tao->type()->blank();
      st = new Unify_Structure(sue, EltMap(), blank->s, blank->p);
    }
    UnifyType *tao_type = tao->type();
    UnifyType *st_type = new UnifyType(st);

    if(redo_pending) {
      typeNode *c_type = NULL;
      memoryBlock *m = tao->type()->block();
      if(m && m->decl())
        c_type = m->decl()->type();
      Unify_Size size;
      if(c_type) size = Unify_Size(Unify_Size::sizeOf(c_type));

      // reset pending so that they are processed again.
      set<Unify_Pending*> pending = tao->pending().set();
      for(Pendings_p a=pending.begin(); a!=pending.end(); a++) {
        (*a)->un_served();
        if((*a)->typ() == Unify_Pending::cjoin &&
           ! ((Unify_Size*) (*a)->arg1())->leq(size)) {
          // in cjoin(), above condition will lead to expand() instad of
          // join/settype, the desired result. set size to top to prevent this.
          size = Unify_Size();
          if(tao->type()->objTyp() == BLANK)
            // do this to ensure_sim_obj use new size
            tao->type()->blank()->s= size;
        }
      }
    }

    if(tao->type() != tao_type) // tao's type has changed.
      return ensure_struct_obj(tao, sue, redo_pending);
    settype(tao, st_type);
    return st;
  } else { // case SIMPLE and OBJECT
    Unify_Size struct_size(Unify_Size::sizeOf(sue));
    promote(tao, struct_size);
    return NULL;
  }
} // ensure_struct_obj


void UnificationBasedPtr::expand(Unify_ECR *e) {
  /*settype(e, unify(e->type(), new UnifyType(new Unify_Blank(Unify_Size(),
                                                    Unify_Parents(false)))));*/
  UnifyType *T = e->type(), // remember
            *U = unify(T, new UnifyType(new Unify_Blank(Unify_Size(),
                                                        Unify_Parents(false))));
  while(T != e->type()) // has e->type() changed?
    U = unify(T = e->type(), U);
  settype(e, U);
} // expand


void UnificationBasedPtr::promote(Unify_ECR *e, Unify_Size &s) {
  if(e->type()->objTyp() == OBJECT && s.leq(e->type()->object()->s))
    return;
  /*settype(e, unify(e->type(),
                   new UnifyType(new Unify_Object(new Alpha(), L_bottom,
                                                  s, Unify_Parents(false)))));
  */
  UnifyType *T = e->type(), // remember
            *U = unify(T, new UnifyType(new Unify_Object(new Alpha(), L_bottom,
                                                    s, Unify_Parents(false))));
  while(T != e->type()) // has e->type() changed?
    U = unify(T = e->type(), U);
  settype(e, U);
} // promote


void UnificationBasedPtr::collapse(Unify_ECR *e) {
  if(e->type()->objTyp() == OBJECT) {
    Unify_Size s;
    if(s.leq(e->type()->object()->s)) return;
  }
  /*settype(e, unify(e->type(),
                   new UnifyType(new Unify_Object(new Alpha(), L_bottom,
                                                  Unify_Size(),
                                                  Unify_Parents(true)))));*/

  UnifyType *T = e->type(), // remember
            *U = unify(T, new UnifyType(new Unify_Object(new Alpha(), L_bottom,
                                                        Unify_Size(),
                                                        Unify_Parents(true))));
  while(T != e->type()) // has e->type() changed?
    U = unify(T = e->type(), U);
  settype(e, U);
} // collapse


void UnificationBasedPtr::make_unknown(Unify_Offset *o) {
  if(o->value() == Unify_Offset::zero) return; // note: pendings are served.
  o->value(Unify_Offset::unknown);
  set<Unify_Pending*> pending = o->pending().set();
  //o->pending().clear();
  for(Pendings_p a=pending.begin(); a!=pending.end(); a++) {
    if((*a)->is_served()) continue;
    if(finalizing) (*a)->served();
    switch((*a)->typ()) {
      case Unify_Pending::collapse:
        collapse((Unify_ECR*) (*a)->arg1());
        break;
      case Unify_Pending::makeunknown:
        if((Unify_Offset*) (*a)->arg1() != o)
          make_unknown((Unify_Offset*) (*a)->arg1());
        break;
    }
  }
} // make_unknown

void UnificationBasedPtr::unless_zero(Unify_Offset *o, Unify_ECR *tao) {
  if(o->value() == Unify_Offset::zero) {
    if(new_pending)
      o->pending().insert( new Unify_Pending(tao),true );
  } else
    collapse(tao);
} // unless_zero

void UnificationBasedPtr::cjoin(Unify_Size &s, Unify_ECR *e1, Unify_ECR *e2) {
  if(!e1 || !e2) return;
  if(e1 == e2 || e1->root()==e2->root()) return;

  // prevent infinite recursion.
  static set<pair<UnifyType*,UnifyType*> > stack;
  pair<UnifyType*,UnifyType*> ee(e1->type(),e2->type());
  if(stack.find(ee) != stack.end()) {
//cout << "cjoin recursion\n";
    return; // recursion detected.
  }
  stack.insert(ee);

  // peek: if both objects pointing to same object, there is nothing more to do
  // in the future.
  bool force_no_pending = e1->type()->objTyp()==OBJECT &&
                          e2->type()->objTyp()==OBJECT &&
                          e1->type()->object()->alpha->tao()->type() ==
                          e2->type()->object()->alpha->tao()->type();

  if(new_pending && !force_no_pending)
    e1->pending().insert( new Unify_Pending(s,e1,e2),true );

  if(e1->type()->is_bottom()) {
    stack.erase(ee);
    return;
  }
/*if(debug)
cout << "cjoin " << *e1 << "+" << *e2 << endl;*/

  Object_Typ e2_typ = e2->type()->objTyp();

  switch(e1->type()->objTyp()) {
    case BLANK: {
      Unify_Blank *b1 = e1->type()->blank();
      if(! s.leq(b1->s))
        expand(e1);
      else if(e2->type()->is_bottom())
        settype(e2, new UnifyType(new Unify_Blank(s, Unify_Parents(false))));
      else if(! s.leq(e2->type()->size()))
        expand(e2);
      break;
    } // BLANK

    case SIMPLE: {
      Unify_Simple *sim1 = e1->type()->simple();
      if(! s.leq(sim1->s))
        expand(e1);
      else if(e2->type()->is_bottom())
        settype(e2, new UnifyType(new Unify_Simple(sim1->alpha, sim1->lambda,
                                                   s, Unify_Parents(false))));
      else {
        switch(e2->type()->objTyp()) {
          case BLANK: {
            Unify_Blank *b2 = e2->type()->blank();
            settype(e2, new UnifyType(new Unify_Simple(sim1->alpha,sim1->lambda,
                                                       b2->s, b2->p)));
            if(! s.leq(b2->s))
              expand(e2);
            break;
          }
          case SIMPLE: {
            Unify_Simple *sim2 = e2->type()->simple();
            join(sim1->alpha, sim2->alpha);
            join(sim1->lambda, sim2->lambda);
            if(! s.leq(sim2->s))
              expand(e2);
            break;
          }
          case STRUCTURE:
            promote(e2, e2->type()->structure()->s);
            if(!finalizing && !serve_again) break; // new
          case OBJECT: {
            Unify_Object *o = e2->type()->object();
            // note: does not check if o has size top and empty parents.
            join(sim1->alpha, o->alpha);
            join(sim1->lambda, o->lambda);
          }
        }
      }
      break;
    } // SIMPLE

    case STRUCTURE: {
      Unify_Structure *st1 = e1->type()->structure();
      if(! s.leq(st1->s))
        expand(e1);
      else if(e2->type()->is_bottom())
        settype(e2, new UnifyType(
                      new Unify_Structure(st1->fo, st1->fto, st1->m, s,
                                          Unify_Parents(false))));
      else {
        switch(e2->type()->objTyp()) {
          case BLANK: {
            Unify_Blank *b2 = e2->type()->blank();
            settype(e2, new UnifyType(
                          new Unify_Structure(st1->fo, st1->fto, st1->m, b2->s,
                                              b2->p)));
            if(! s.leq(b2->s))
              expand(e2);
            break;
          }
          case SIMPLE:
            promote(e2, e2->type()->simple()->s);
            if(finalizing || serve_again) goto STRUCTURE_OBJECT; // new
            break;
          case STRUCTURE: {
            Unify_Structure *st2 = e2->type()->structure();
            bool all_compatible = true;
            if(s.leq(st2->s)) {
              // check all x in dom(st1->m)
              for(EltMap::iterator x=st1->m.begin(); x!=st1->m.end(); x++)
                if(! make_compatible(x->first, st2, e2))
                { all_compatible = false; break; }
            } else all_compatible = false;
            if(all_compatible)
              for(EltMap::iterator x=st1->m.begin(); x!=st1->m.end(); x++) {
                declNode *one_field = unique_field_defn(* x->first.begin());
                Unify_Size s_x(Unify_Size::sizeOf(one_field->type()));
                cjoin(s_x, x->second, st2->get(one_field, e2));
              }
            else
              expand(e2);
            break;
          }
          case OBJECT: {
STRUCTURE_OBJECT:
            // note: does not check if e2 has size top and empty parents.
            for(EltMap::iterator x=st1->m.begin(); x!=st1->m.end(); x++) {
              Unify_Size s_x(Unify_Size::sizeOf((*x->first.begin())->type()));
              cjoin(s_x, x->second, e2);
            }
          }
        }
      }
      break;
    } // STRUCTURE

    case OBJECT: {
      Unify_Object *o1 = e1->type()->object();
      if(! s.leq(o1->s)) expand(e1); // new
      //if(o1->s.is_top() && o1->p.empty()) {
      // note: does not check if o1,o2 has size top and empty parents.
        if(finalizing || serve_again) // new
          promote(e2,s); // ensure e2 is OBJECT
        // bool do_join = false;
        Unify_Object *o2 = NULL;
        if(e2->type()->objTyp()==OBJECT) {
          o2 = e2->type()->object();
          // if(o2->s.is_top() && o2->p.empty()) do_join = true;
        }
        if(/*do_join ||*/ o2 /*&& finalizing*/) {
          bool f = finalizing;
          finalizing = false; // so that new pending can be generated.
          if(! s.leq(o2->s)) expand(e2); // new
          join(o1->alpha, o2->alpha);
          join(o1->lambda, o2->lambda);
          finalizing = f;

        } /*else if(finalizing) {
          //settype(e2, unify(e1->type(), e2->type())); // new
          join(e1, e2); // new
        } */ else
          promote(e2,s);

      /*} else if(finalizing) {
        settype(e2, unify(e1->type(), e2->type())); // new
      } */
    } // OBJECT
  }

  if(serve_again && e2_typ != e2->type()->objTyp()) { // new
    // e2 has been promoted, and we are in finalize mode. Any pending in e2 may
    // need to be served again. Need to get all pendings from e2's root.
    set<Unify_Pending*> pending = e2->root()->pending().set();
    for(Pendings_p p=pending.begin(); p!=pending.end(); p++)
      if((*p)->is_served()) {
        (*p)->un_served();
        more_pending.insert(*p);
      }
  }

/*if(debug)
cout << "  after cjoin " << *e1 << "+" << *e2 << endl;*/
  stack.erase(ee);
} // cjoin


UnifyType *UnificationBasedPtr::unify(UnifyType *t1, UnifyType *t2) {
  if(t1 == t2) return t1;

#define return_unify(t) { \
  UnifyType *T = (t)->ecr()->type(); /* need to do (?) this because t could have
                                  a new root type as we recurse into joins. */ \
  memoryBlock *block = t1->block() ? t1->block() : t2->block(); \
  if(! T->block()) T->block(block); \
  else assert(T->block()==block); \
  \
  if(t1 != T) T->procs().insert(t1->procs().begin(), t1->procs().end()); \
  if(t2 != T) T->procs().insert(t2->procs().begin(), t2->procs().end()); \
  \
  return T; \
}

  if(t1->block() && t2->block()) {
    cout << "cannot unify " << *t1 << " + " << *t2 << endl;
    cout << "  " << *t1->ecr() << " + " << *t2->ecr() << endl;
    cout << "  " << t1->block()->name() << " + " << t2->block()->name() << endl;
    assert(false);
    return NULL; // do not unify. or what? TBD?
  }

  Object_Typ ty1 = t1->objTyp(),
             ty2 = t2->objTyp(),
             result_ty;
  if(ty1 == BOTTOM) return_unify( t2 )
  if(ty2 == BOTTOM) return_unify( t1 )

#define max_Lambda(t1, ty1, t2, ty2, l) \
  if(t1->is_bottom() || ty1==STRUCTURE || ty1==BLANK) { \
    is_Sim_Obj(t2); \
    l = Sim_Obj_Lambda(t2); \
  } else if(t2->is_bottom() || ty2==STRUCTURE || ty2==BLANK) { \
    is_Sim_Obj(t1); \
    l = Sim_Obj_Lambda(t1); \
  } else { \
    l = Sim_Obj_Lambda(t1); \
    Lambda *l2 = Sim_Obj_Lambda(t2); \
    if(l->leq(l2)) l=l2; \
    else if(l2->leq(l)) ; \
    else join(l,l2); \
  }

#define sizeOfType(t,ty) \
  ((ty==OBJECT) ? t->object()->s : \
   (ty==SIMPLE) ? t->simple()->s : \
   (ty==STRUCTURE) ? t->structure()->s : \
   (ty==BLANK) ? t->blank()->s : Unify_Size())

#define max_Size(t1, ty1, t2, ty2, s) \
  if(t1->is_bottom()) \
    s = sizeOfType(t2,ty2); \
  else if(t2->is_bottom()) \
    s = sizeOfType(t1,ty1); \
  else { \
    s = sizeOfType(t1,ty1); \
    Unify_Size s2 = sizeOfType(t2,ty2); \
    if(s.is_top()) ; \
    else if(s2.is_top()) s=s2; \
    else if( s.size() < s2.size() ) s=s2; \
  }

#define parentsOfType(t,ty) \
  ((ty==OBJECT) ? t->object()->p : \
   (ty==SIMPLE) ? t->simple()->p : \
   (ty==STRUCTURE) ? t->structure()->p : \
   (ty==BLANK) ? t->blank()->p : Unify_Parents(true))

#define max_Parents(t1, ty1, t2, ty2, p) \
  if(t1->is_bottom()) \
    p = parentsOfType(t2,ty2); \
  else if(t2->is_bottom()) \
    p = parentsOfType(t1,ty1); \
  else { \
    Unify_Parents p1 = parentsOfType(t1,ty1), \
                  p2 = parentsOfType(t2,ty2); \
    for(set<Unify_ECR*>::iterator P=p1.parents().begin(); \
        P!=p1.parents().end(); P++)\
      p.parents().insert((*P)->root()); \
    for(set<Unify_ECR*>::iterator P=p2.parents().begin(); \
        P!=p2.parents().end(); P++)\
      p.parents().insert((*P)->root()); \
    if(!p1.is_top() || ! p2.is_top()) p.top(false); \
  }

  if(ty1==OBJECT || ty2==OBJECT || ty1==STRUCTURE && ty2==SIMPLE ||
     ty2==STRUCTURE && ty1==SIMPLE)
    result_ty = OBJECT;
  else if(ty1==SIMPLE || ty2==SIMPLE)
    result_ty = SIMPLE;
  else if(ty1==STRUCTURE || ty2==STRUCTURE)
    result_ty = STRUCTURE;
  else if(ty1==BLANK || ty2==BLANK)
    result_ty = BLANK;
  else result_ty = BOTTOM;

/*cout << "unify " << *t1 << " + " << *t2 << " result_ty="
     << (result_ty==SIMPLE ? "SIMPLE\n" :
         result_ty==OBJECT ? "OBJECT\n" :
         result_ty==STRUCTURE ? "STRUCTURE\n" :
         result_ty==BLANK ? "BLANK\n" : "BOTTOM\n");*/

  if(result_ty==SIMPLE || result_ty==OBJECT) {
    Unify_Size s;
    max_Size(t1,ty1,t2,ty2,s)
    Alpha *alpha;
    bool recursion_detected;
    if(ty1!=SIMPLE && ty1!=OBJECT)
      alpha = Sim_Obj_Alpha(t2);
    else if(ty2!=SIMPLE && ty2!=OBJECT)
      alpha = Sim_Obj_Alpha(t1);
    else {
      Alpha *a1 = Sim_Obj_Alpha(t1),
            *a2 = Sim_Obj_Alpha(t2);
      if(a2->offset()->value() == Unify_Offset::unknown)
        join(a2,a1,&recursion_detected); //treat a2 as rhs
      else
        join(a1,a2,&recursion_detected);
      // note: obtain alpha below only after above joins. Pick the one that
      // contain the root of tao, so that it also contains the union of their
      // pendings.
      if(a1->tao()->root() == a1->tao())      alpha = a1;
      else if(a2->tao()->root() == a2->tao()) alpha = a2;
      else alpha = new Alpha(a1->tao()->root(), a1->offset());
    }
    Lambda *lambda;
    max_Lambda(t1,ty1,t2,ty2,lambda)
    Unify_Parents p(true);
    max_Parents(t1,ty1,t2,ty2,p);

    if(result_ty==SIMPLE) {
      if(ty1==SIMPLE) {
        Unify_Simple *s1 = t1->simple();
        if((s1->alpha->equal(alpha) || s1->alpha->tao()->type()->is_bottom()) &&
           s1->lambda->equal(lambda) && s1->s.equal(s) && s1->p.equal(p))
          return_unify( t1 )
      }
      if(ty2==SIMPLE) {
        Unify_Simple *s2 = t2->simple();
        if((s2->alpha->equal(alpha) || s2->alpha->tao()->type()->is_bottom()) &&
           s2->lambda->equal(lambda) && s2->s.equal(s) && s2->p.equal(p))
          return_unify( t2 )
      }
      if(ty1==SIMPLE && ty2==SIMPLE && recursion_detected) {
        // this happens only (?) when alpha is not joined from a1,a2 due to
        // recursion in join(). This happens when, e.g. we try to unify two
        // structures with pointer fields pointing back to the structures.
        t1->simple()->p = p;
        return_unify( t1 ) // arbitrary return, doesn't matter.
      }
      if(ty1==SIMPLE && !recursion_detected) {
        Unify_Simple *s1 = t1->simple();
        s1->lambda = lambda;
        s1->s = s;
        s1->p = p;
        return_unify( t1 )
      } else if(ty2==SIMPLE && !recursion_detected) {
        Unify_Simple *s2 = t2->simple();
        s2->lambda = lambda;
        s2->s = s;
        s2->p = p;
        return_unify( t2 )
      }
      Unify_Simple *sim = new Unify_Simple(alpha, lambda, s, p);
      return_unify( new UnifyType(sim) )

    } else { // result_ty is OBJECT
      Unify_ECR *field = NULL;
#define consolidate_struct_fields(struc,result) {\
    EltMap map = struc->m; \
    for(EltMap::iterator m=map.begin(); m!=map.end(); m++) \
      if(m->second) { \
        if(result) join(result, m->second); \
        else result = m->second; \
      } \
  }

#define consolidate_sim_obj_fields(alpha,result) {\
    if(result) join(result,alpha->tao()); \
    else result = alpha->tao(); \
  }

      if(ty1==STRUCTURE) consolidate_struct_fields(t1->structure(), field)
      else if(ty1==SIMPLE || ty1==OBJECT)
        consolidate_sim_obj_fields(Sim_Obj_Alpha(t1), field)
      if(ty2==STRUCTURE) consolidate_struct_fields(t2->structure(), field)
      else if(ty2==SIMPLE || ty2==OBJECT)
        consolidate_sim_obj_fields(Sim_Obj_Alpha(t2), field)

      if(ty1==OBJECT) {
        Unify_Object *o1 = t1->object();
        if((o1->alpha->equal(alpha) || o1->alpha->tao()->type()->is_bottom()) &&
           o1->lambda->equal(lambda) && o1->s.equal(s) && o1->p.equal(p))
          return_unify( t1 )
      }
      if(ty2==OBJECT) {
        Unify_Object *o2 = t2->object();
        if((o2->alpha->equal(alpha) || o2->alpha->tao()->type()->is_bottom()) &&
           o2->lambda->equal(lambda) && o2->s.equal(s) && o2->p.equal(p))
          return_unify( t2 )
      }
      if(ty1==OBJECT && ty2==OBJECT && recursion_detected) {
        // this happens only (?) when alpha is not joined from a1,a2 due to
        // recursion in join(). This happens when, e.g. we try to unify two
        // structures with pointer fields pointing back to the structures.
        t1->object()->p = p;
        return_unify( t1 ) // arbitrary return, doesn't matter.
      }
      if(ty1==OBJECT && !recursion_detected) {
        Unify_Object *o1 = t1->object();
        o1->lambda = lambda;
        o1->s = s;
        o1->p = p;
        return_unify( t1 )
      } else if(ty2==OBJECT && !recursion_detected) {
        Unify_Object *o2 = t2->object();
        o2->lambda = lambda;
        o2->s = s;
        o2->p = p;
        return_unify( t2 )
      }
      Unify_Object *obj = new Unify_Object(alpha, lambda, s, p);
      return_unify( new UnifyType(obj) )
    }

  } else if(result_ty==STRUCTURE) {
    Unify_Size s;
    max_Size(t1,ty1,t2,ty2,s)
    Unify_Parents p(true);
    max_Parents(t1,ty1,t2,ty2,p);

    EltMap m;
    FieldOrder fo;
    FieldTypeOrder fto;
    if(ty1!=STRUCTURE) {
      t2->structure()->s = s;
      t2->structure()->p = p;
      return_unify( t2 )
    } else if(ty2!=STRUCTURE) {
      t1->structure()->s = s;
      t1->structure()->p = p;
      return_unify( t1 )
    } else {
      Unify_Structure *s = new Unify_Structure(FieldOrder(), FieldTypeOrder(),
                                               EltMap(), Unify_Size(), p);
      merge_EltMap(t1, t2, s);
      return_unify( new UnifyType(s) )
    }


  } else if(result_ty==BLANK) {
    Unify_Size s;
    max_Size(t1,ty1,t2,ty2,s)
    Unify_Parents p(true);
    max_Parents(t1,ty1,t2,ty2,p);
    if(ty1==BLANK) {
      Unify_Blank *b1 = t1->blank();
      if(b1->s.equal(s) && b1->p.equal(p)) return_unify( t1 )
    }
    if(ty2==BLANK) {
      Unify_Blank *b2 = t2->blank();
      if(b2->s.equal(s) && b2->p.equal(p)) return_unify( t2 )
    }
    if(ty1==BLANK) {
      Unify_Blank *b1 = t1->blank();
      b1->s = s;
      b1->p = p;
      return_unify( t1 )
    } else {
      Unify_Blank *b2 = t2->blank();
      b2->s = s;
      b2->p = p;
      return_unify( t2 )
    }
    // return new UnifyType(new Blank(s, p));
  } else // both are BOTTOM
    return_unify( t1 )
} // unify


void UnificationBasedPtr::merge_EltMap(UnifyType *t1, UnifyType *t2,
                                       Unify_Structure *s) {
//cout << "merge_EltMap\n";
  /* The paper does not give details, but it appears that, say, the structure
   * definitions are respectively
   *   m1 = { int, int, int* }
   *   m2 = { int, int, float, float* }
   * then the result should be
   *   s->m  = { int, int, _everything_else_ }
   * i.e. the prefix is the common prefix of both m1,m2, then everything else
   * are merged.
   */

  assert(t1->objTyp()==STRUCTURE && t2->objTyp()==STRUCTURE);
  EltMap m1 = t1->structure()->m,
         m2 = t2->structure()->m;
  FieldOrder fo1 = t1->structure()->fo,
             fo2 = t2->structure()->fo;

  for(FieldOrder::iterator f1=fo1.begin(); f1!=fo1.end(); f1++) {
    make_compatible(*f1,s,t1->ecr(),true);
    if(! m1[*f1]) continue;
    declNode *one_field = * f1->begin();
    FieldOrder::iterator fo=s->fo.begin();
    while(fo!=s->fo.end() && fo->find(one_field)==fo->end())
      fo++;
    assert(fo!=s->fo.end());
    join(m1[*f1], s->m[*fo]);
  }

  for(FieldOrder::iterator f2=fo2.begin(); f2!=fo2.end(); f2++) {
    make_compatible(*f2,s,t2->ecr(),true);
    if(! m2[*f2]) continue;
    declNode *one_field = * f2->begin();
    FieldOrder::iterator fo=s->fo.begin();
    while(fo!=s->fo.end() && fo->find(one_field)==fo->end())
      fo++;
    assert(fo!=s->fo.end());
    join(m2[*f2], s->m[*fo]);
  }
} // merge_EltMap


bool UnificationBasedPtr::make_compatible(declSet n, Unify_Structure *s,
                                          Unify_ECR *container, bool force) {
  { // ensure we use the unique fields.
  declSet n1 = n; // make a copy
  for(declSet::iterator d=n1.begin(); d!=n1.end(); d++)
    if(_unique_field_defn[*d] != *d) {
      n.erase(*d);
      n.insert(_unique_field_defn[*d]);
    }
  }

  if(find(s->fo.begin(), s->fo.end(), n) != s->fo.end()) return true;
  container = container->root();

/*cout << "make_compatible ";
for(declSet::iterator d=n.begin(); d!=n.end(); d++)
  cout << (*d)->name() << "@" << (*d)->coord() << " ";
 cout << "\n  container:" << *container << endl; */

  assert(! n.empty());

  if(s->fo.empty()) { // not yet set
    suespecNode *sue = _field2sue[* n.begin()];
    decl_list fields = sue->fields();
    assert(! fields.empty());
    if(sue->owner() == Struct) {
      for(decl_list_p d=fields.begin(); d!=fields.end(); d++) {
        declSet ds;  ds.insert(*d);
        s->fo.push_back(ds);
        s->fto.push_back((*d)->type());
        assert(! s->m[ds]);
        new_field_ECR(s->m[ds], (*d)->type(), container)
      }
    } else {
      declSet ds;
      typeNode *type = NULL;
      for(decl_list_p d=fields.begin(); d!=fields.end(); d++) {
        ds.insert(*d);
        if(d==fields.begin()) type = (*d)->type();
        else if(! compatible_type((*d)->type(), type)) type = NULL;
      }
      s->fo.push_back(ds);
      s->fto.push_back(type);
      assert(s->m.empty());
      new_field_ECR(s->m[ds], type, container);
    }
    return true;
  }

  decl_list n_pos; // see comment below.
  for(declSet::iterator d=n.begin(); d!=n.end(); d++) {
//cout << (*d)->name() << " " << *d << " @" << (*d)->coord() << endl;
    if(d==n.begin()) n_pos = _fieldpos[*d];
    /*else if(_field2sue[*d] == _field2sue[* n.begin()]) {
      cout << s->all_str();
      for(FieldOrder::iterator fo=s->fo.begin(); fo!=s->fo.end(); fo++)
        cout << (*fo == n);
      cout << "\nn:";
      for(declSet::iterator d1=n.begin(); d1!=n.end(); d1++)
        cout << "  " << (*d1)->name() << " @" << (*d1)->coord() << endl;
      assert(false); // possible?
      n_pos = _fieldpos[*d];
    } else {
      if(n_pos.size() != _fieldpos[*d].size()) {
        cout << "n_pos:\n";
        for(decl_list_p d1=n_pos.begin(); d1!=n_pos.end(); d1++)
          cout << "    " << (*d1)->name() << "@" << (*d1)->coord() << endl;
        cout << "_fieldpos:\n";
        for(decl_list_p d1=_fieldpos[*d].begin(); d1!=_fieldpos[*d].end(); d1++)
          cout << "    " << (*d1)->name() << "@" << (*d1)->coord() << endl;
      }
      assert(n_pos.size() == _fieldpos[*d].size());
    }*/
    // above is wrong. the fields in n could come from same sue (they are
    // merged). Whether they belong to same sue or not, we want to know the
    // common prefix in each sue; hence we want n_pos to be the _fieldpos of
    // smallest size:
    else if(_fieldpos[*d].size() < n_pos.size())
      n_pos = _fieldpos[*d];
  }

/*{
  cout << "#n_pos=" << n_pos.size() << endl;
  cout << "b4 change. " << s->all_str();
  cout << "\nfield_type_order: ";
  for(FieldTypeOrder::iterator fto=s->fto.begin(); fto!=s->fto.end(); fto++) {
    if(*fto) {
      output_context oc(cout);
      (*fto)->output(oc,NULL); cout << ",";
    } else cout << "nil,";
  }
  cout << endl;
} */

  assert(s->fo.size() == s->fto.size());
  FieldOrder::iterator fo=s->fo.begin();
  FieldTypeOrder::iterator to=s->fto.begin();
  Unify_ECR *previously_merged = NULL;


  for(decl_list_p n1=n_pos.begin(); n1!=n_pos.end(); n1++) {

/*cout << "n1=" << (*n1)->name() << " fo={";
for(declSet::iterator d=fo->begin(); d!=fo->end(); d++) cout << (*d)->name() << ",";
 cout << "}\n"; */

#define insert_into_set(set, Dp) { \
  if(Dp == n_pos.back()) set->insert(n.begin(), n.end()); \
  else                   set->insert(Dp); \
  if(Dp != n_pos.front()) { \
    /* insert other fields preceding each field in n. */ \
    for(declSet::iterator N=n.begin(); N!=n.end(); N++) { \
      decl_list all_fields = _field2sue[*N]->fields(); \
      FieldOrder::iterator fo1 = s->fo.begin(); \
      for(decl_list_p f=all_fields.begin(); f!=all_fields.end(); f++) { \
        if(_fieldpos[*f].size() >= _fieldpos[Dp].size()) break; \
        else { \
          Unify_ECR *E = s->m[*fo1]; \
          s->m.erase(*fo1); \
          fo1->insert(*f); \
          s->m[*fo1] = E; \
          break; \
        } \
        if(++fo1==s->fo.end()) break; \
      } \
    } \
  } \
}

    bool merge = false;
    typeNode *field_type = (to==s->fto.end()) ? NULL : *to;
//cout << "  field_type " << field_type << endl;
    if(field_type == NULL || !compatible_type((*n1)->type(), field_type)) {
//cout << "!compatible_type\n";
      if(!force) return false;
      // merge
      declSet * ds;
      if(to!=s->fto.end()) {
        *to = NULL;
        ds = &(*fo);
      } else {
        s->fto.back() = NULL;
        ds = & s->fo.back();
      }
      if(s->m.find(*ds) != s->m.end()) {
        Unify_ECR *E = s->m[*ds];
        s->m.erase(*ds);
        insert_into_set(ds,*n1)
        s->m[*ds] = E;
      } else
        insert_into_set(ds,*n1)
      if(previously_merged) {
        if(s->m[*ds])
          join(s->m[*ds], previously_merged); // TBD: or use cjoin?
        else
          s->m[*ds] = previously_merged;
      } else if(! s->m[*ds])
        new_field_ECR(s->m[*ds], field_type, container)
      previously_merged = s->m[*ds];
    } else { // compatible types
      if(s->m.find(*fo) == s->m.end()) {
        insert_into_set(fo,*n1)
        new_field_ECR(s->m[*fo], field_type, container)
      } else {
        Unify_ECR *E = s->m[*fo];
        s->m.erase(*fo);
        insert_into_set(fo,*n1)
        s->m[*fo] = E;
      }
    }

    if(*n1 == n_pos.back() && ! previously_merged && fo!=s->fo.end()) {
      // we are about to finish this loop. Before that, need to check if
      // anything in n is in later fo, and if so, need to merge.
      // Check this now before fo is incremented.
      bool n_in_later_fo = false;
      FieldOrder::iterator fo1 = fo; fo1++; // next after fo
      while(!n_in_later_fo && fo1!=s->fo.end()) {
        for(declSet::iterator d=n.begin(); d!=n.end(); d++)
          if(fo1->find(*d) != fo1->end()) { n_in_later_fo=true; break; }
        fo1++;
      }
      if(n_in_later_fo)
        previously_merged = s->m[*fo]; // enable merge.
    }

    if(!previously_merged && fo != s->fo.end()) { fo++; to++; }
/*cout << "n1 done, m=";
for(EltMap::iterator m=s->m.begin(); m!=s->m.end(); m++)
  if(m->second) {
    cout << "<";
    declSet decls = m->first;
    for(declSet::iterator d=decls.begin(); d!=decls.end(); d++) {
      if(d!=decls.begin()) cout << ",";
      cout << (*d)->name();
    }
    cout << "->" << m->second->type()->id() << ">,";
  }
 cout << endl; */
  }


  if(previously_merged) {
    if(fo != s->fo.end()) {
//cout << "fo " << fo->size() << endl;
      while(s->fo.back() != *fo) {
        declSet back = s->fo.back();
        s->m.erase(*fo);
        if(s->m.find(back) != s->m.end()) { // merge previously_merged and back
          Unify_ECR *E2 = s->m[back];
          s->m.erase(back);
          join(previously_merged, E2);
          previously_merged = previously_merged->root();
        }
        (*fo).insert(back.begin(), back.end());
        s->m[*fo] = previously_merged;
        s->fo.pop_back();
        s->fto.pop_back();
      }
    }

    // if n is not the last field of its structure, need to merge all in.
    for(declSet::iterator N=n.begin(); N!=n.end(); N++) {
      if(_field2sue[*N]->fields().back() != *N) {
//cout << "N = " << (*N)->name() << endl;
        declSet & back = s->fo.back();
        assert(back.find(*N) != back.end());
        assert(previously_merged == s->m[back]);
        s->m.erase(back);
        decl_list fields = _field2sue[*N]->fields();
        for(decl_list::reverse_iterator f=fields.rbegin();
            f!=fields.rend() && *f!=*N; f++)
          back.insert(*f);
        s->m[back] = previously_merged;
      }
    }
  }

#define CX_INVARIANT
#ifdef CX_INVARIANT
{ int error = 0;
  if(s->fo.size() != s->fto.size()) error = 1; // unmatch size
  int pos = 1;
  for(fo=s->fo.begin(); !error && fo!=s->fo.end(); fo++, pos++) {
    if(s->m.find(*fo) != s->m.end() && !s->m[*fo]) error=2; // null entry
    for(declSet::iterator d=fo->begin(); !error && d!=fo->end(); d++) {
      if(_fieldpos[*d].size() < pos) error=3; // wrong _fieldpos
      decl_list all_fields = _field2sue[*d]->fields();
      if(_fieldpos[*d].size() > pos) { // *d is merged field.
        for(decl_list_p f=all_fields.begin(); !error && f!=all_fields.end();f++)
          if(_fieldpos[*f].size() >= pos &&
             fo->find(*f) == fo->end()) {
            error=4; // *f not inside fo, should be in same as *d.
            cout << "d " << (*d)->name() << " @" << (*d)->coord() << endl;
            cout << "f " << (*f)->name() << " @" << (*f)->coord() << endl;
          }
      }
      for(decl_list_p f=all_fields.begin(); !error && f!=all_fields.end();f++)
        if(_fieldpos[*f].size() < pos) { // *f is merged, possibly diff struct
          int pos1 = 1;
          FieldOrder::iterator fo1 = s->fo.begin();
          while(pos1 < _fieldpos[*f].size()) { fo1++; pos1++; }
          if(fo1->find(*f) == fo1->end()) { // *f is not in right s->fo
            error=5;
            cout << "f " << (*f)->name() << " @" << (*f)->coord() << endl;
          }
        }
    }
  }
  for(map<declSet,Unify_ECR*>::iterator m=s->m.begin(); !error && m!=s->m.end();
      m++) {
    if(! m->second) error=6; // empty entry
    else if(find(s->fo.begin(), s->fo.end(), m->first) == s->fo.end())
      error=7; // the declSet not in fo
  }
  if(error) {
    cout << s->all_str();
    cout << "error=" << error << endl;
    if(cur_proc) cout << "cur_proc @" << cur_proc->coord() << endl;
    assert(false);
  }
}
#endif

  return true;
} // make_compatible


bool UnificationBasedPtr::compatible_type(typeNode *t1, typeNode *t2) {
  // easiest: the types must be the same.
  if(!t1 || !t2) return false;
  if(t1 == t2) return true;
  if(t1->typ() != t2->typ()) return false;
  switch(t1->typ()) {
    case Prim: return ((primNode*)t1)->basic() == ((primNode*)t1)->basic();
    case Struct:
    case Union: return ((sueNode*)t1)->spec() == ((sueNode*)t2)->spec();
    case Ptr: return compatible_type(t1->type(), t2->type());
    case Func: return true; // TBD???
    case Array: {
      arrayNode *a1 = (arrayNode*) t1,
                *a2 = (arrayNode*) t2;
      if(!compatible_type(a1->type(), a2->type())) return false;
      return (a1->dim() && a1->dim()->typ()==Const &&
              a2->dim() && a2->dim()->typ()==Const &&
              ((constNode*)a1->dim())->value().Integer() ==
              ((constNode*)a2->dim())->value().Integer());
    }
    case Tdef:
      return compatible_type(((tdefNode*)t1)->def(), ((tdefNode*)t2)->def());
    default: cout << t1->typ() << "@" << t1->coord() << " & "
                  << t2->typ() << "@" << t2->coord() << endl;
             assert(false);
  }
} // compatible_type



void UnificationBasedPtr::finalize() {
//cout << "finalize\n";

  bool completed_once = false;
  serve_again = true;
  do {
    // the default, original value, in case this loop is re-iterated.
    finalizing = false;
    new_pending = true;

  // deal with _ptr_call first.
//cout << "finalize deal with _ptr_call\n";
    map<threeAddrNode*,UnifyType*> ptr_call_set = _ptr_call;
    bool change = false;
    for(map<threeAddrNode*,UnifyType*>::iterator pc = ptr_call_set.begin();
        pc != ptr_call_set.end(); pc++) {
      threeAddrNode *the_call = pc->first;
      UnifyType *type = pc->second;
      assert(type);
      is_Sim_Obj(type); // still a Simple or Object ?
/*cout << "at_call again ? " << the_call->coord() << " " << *type << endl;
  cout << "  " << *type << endl;
  cout << "  " << *Sim_Obj_Lambda(type) << endl; */
      if(type->ecr()->type() != type || // type has changed
         ! Sim_Obj_Lambda(type)->is_bottom()) { // no longer a bottom
        Unify_Size s = Unify_Size(Unify_Size::sizeOfAssign(the_call, linker,
                                                           this));
/*cout << "at_call again " << the_call->coord() << " "
     << (type->ecr()->type() != type) << (! Sim_Obj_Lambda(type)->is_bottom())
     << endl;
if(type->ecr()->type() != type) cout << "  new type " << *type->ecr() <<endl;*/
        at_call(the_call, s);
        if(_ptr_call.find(the_call) == _ptr_call.end() ||
           _ptr_call[the_call] != type)
          change = true;
      }
    }
    if(completed_once && !change) break;

//debug = true;
//cout << "actual finalize\n";
    finalizing = true;
    new_pending = false;
    set<Unify_ECR*> all_ecr = Unify_ECR::allECR();
    for(set<Unify_ECR*>::iterator e=all_ecr.begin(); e!=all_ecr.end(); e++) {
      set<Unify_Pending*> pending = (*e)->pending().set();
      // more for Lambda pendings
      if((*e)->type()->objTyp()==SIMPLE || (*e)->type()->objTyp()==OBJECT) {
        set<Unify_Pending*> more= Sim_Obj_Lambda((*e)->type())->pending().set();
        pending.insert(more.begin(), more.end());
      }
repeat:
      for(Pendings_p p=pending.begin(); p!=pending.end(); p++) {
        if((*p)->is_served()) continue;
        (*p)->served();
        switch((*p)->typ()) {
          case Unify_Pending::collapse:
            collapse((Unify_ECR*) (*p)->arg1());
            break;
          case Unify_Pending::makeunknown:
            make_unknown((Unify_Offset*) (*p)->arg1());
            break;
          case Unify_Pending::join_ECR:
            join((Unify_ECR*) (*p)->arg1(), (Unify_ECR*) (*p)->arg2());
            break;
          case Unify_Pending::join_Lambda:
            join((Lambda*) (*p)->arg1(), (Lambda*) (*p)->arg2());
            break;
          case Unify_Pending::cjoin:
            cjoin(*(Unify_Size*) (*p)->arg1(), (Unify_ECR*)(*p)->arg2(),
                  (Unify_ECR*)(*p)->arg3());
            // delete (Unify_Size*) (*p)->arg1();
            break;
          default: assert(false);
        }
      }
      if(! more_pending.empty()) {
//cout << "finalize: " << more_pending.size() << " more pendings\n";
        pending = more_pending;
        more_pending.clear();
        goto repeat;
      }
      //(*e)->pending().clear();
    }

    completed_once = true;
  } while(true);

  new_pending = true;
} // finalize

// ==============================================================

Unify_ECR *UnificationBasedPtr::ecrField(UnifyType *container, declNode *field,
                                         bool field_from_annotation) {
  if(!container) return NULL;

  if(field_from_annotation && ! _unique_field_defn[field]) {
    if(container->objTyp() == OBJECT)
      return container->object()->alpha->tao();
    if(container->objTyp() != STRUCTURE) return NULL; // cannot do anything
    Unify_Structure *st = container->structure();
    string field_name = field->name(); // get rid of any prefix
    int pos = field_name.rfind(".");
    if(pos > 0) field_name = field_name.substr(pos+1, string::npos);

    declNode *unique_field = NULL;
    for(FieldOrder::iterator f=st->fo.begin(); f!=st->fo.end(); f++)
      for(declSet::iterator d=f->begin(); d!=f->end(); d++)
        if((*d)->name() == field_name) {
          if(! unique_field) unique_field = *d;
          else return NULL; // no unique field.
        }
    if(unique_field) _unique_field_defn[field] = unique_field;
  }

  field = _unique_field_defn[field];

  if(container->is_bottom() || container->objTyp() == BLANK) {
    suespecNode *sue = field2sue(field);
    assert(sue);
    Unify_Structure *st = ensure_struct_obj(container->ecr(), sue, true);

    if(container->ecr()->type() != container) {
      // changed. need to update.
      //memoryBlock *m = container->block();
      container = container->ecr()->type();
      /*if(m) { // m could be null if call from process_struct_rule().
        m->unifyType(new_container);
        new_container->block(m);
        container->block(NULL);
      }*/
    }
    declSet F;  F.insert(field);
    if(st && container->objTyp()==STRUCTURE)
      // !st happens when container->ecr()->type()!=container, when
      // container refers to a procedure. See createProcBlock.
      make_compatible(F, st, container->ecr(), true);
    // fall through
  }

  if(container->objTyp() == SIMPLE) {
    promote(container->ecr(), container->simple()->s);
    if(container->ecr()->type() != container) {
      //memoryBlock *m = container->block();
      container = container->ecr()->type();
      /*if(m) {
        m->unifyType(new_container);
        new_container->block(m);
        container->block(NULL);
      }*/
    }
  }

  if(container->objTyp() == OBJECT)
    return container->object()->alpha->tao();

  assert(container->objTyp() == STRUCTURE); // ???
  return container->structure()->get(field, container->ecr());
} // ecrField


Unify_ECR *UnificationBasedPtr::ecrDeref(UnifyType *container) {
  if(!container) return NULL;
  if(container->is_bottom() || container->objTyp() == BLANK) {
    // how is this possible: an array is never dereferenced except in
    // broadway's annotation.
    typeNode *c_type = NULL;
    memoryBlock *m = container->block();
    if(m && m->decl())
      c_type = m->decl()->type();
    Unify_Size size;
    if(c_type) size = Unify_Size(Unify_Size::sizeOf(c_type));

    // reset pending so that they are processed again.
    set<Unify_Pending*> pending = container->ecr()->pending().set();
    for(Pendings_p a=pending.begin(); a!=pending.end(); a++) {
      (*a)->un_served();
      if((*a)->typ() == Unify_Pending::cjoin &&
         ! ((Unify_Size*) (*a)->arg1())->leq(size)) {
        // in cjoin(), above condition will lead to expand() instad of
        // join/settype, the desired result. set size to top to prevent this.
        size = Unify_Size();
        if(container->objTyp() == BLANK)
          container->blank()->s = size; //do this to ensure_sim_obj use new size
      }
    }
    
    ensure_sim_obj(container->ecr(), size);
    if(container->ecr()->type() != container) {
      // changed. need to update.
      container = container->ecr()->type();
      /*m->unifyType(new_container);
      new_container->block(m);
      container->block(NULL);*/
    }
  }
  assert(container->objTyp() == SIMPLE || container->objTyp() == OBJECT); //???
  if(container->objTyp() == SIMPLE)
    return container->simple()->alpha->tao();
  else
    return container->object()->alpha->tao();
} // ecrDeref


void UnificationBasedPtr::createProcBlock(procNode *proc, memoryModel &Memory,
                                          procLocation *cur_loc) {
  declNode *p_decl = proc->decl();
  UnifyType *p_type = proctype(proc);
  Unify_ECR *proc_ecr = p_type->ecr_no_root(), // do not use ecr(proc->decl())
            *parent = proc_ecr->parent();
  UnifyType *org_type = proc_ecr->type();
  proc_ecr->parent(NULL);
  proc_ecr->type(p_type);
  memoryBlock *b = Memory.lookup_variable(cur_loc, p_decl, cur_loc->proc());
  proc_ecr->parent(parent);
  if(! parent) proc_ecr->type(org_type);
  assert(proc_ecr->type() == org_type);

  if(p_type->block()) assert(p_type->block() == b);
  else p_type->block(b);
} // createProcBlock


bool UnificationBasedPtr::reachable(UnifyType *src, UnifyTypes & targets,
                                    UnifyTypes & visited) {
  assert(src);
  if(visited.find(src) != visited.end()) return false;
  visited.insert(src);
  if(targets.find(src) != targets.end()) return true;
  if(src->is_bottom()) return false;
  switch(src->objTyp()) {
    case BLANK: return false;
    case SIMPLE:
    case OBJECT: {
      bool r = reachable(Sim_Obj_Alpha(src)->tao()->type(), targets, visited);
      if(r) targets.insert(src);
      return r;
    }
    case STRUCTURE: {
      EltMap map = src->structure()->m;
      for(EltMap::iterator m=map.begin(); m!=map.end(); m++)
        if(m->second && reachable(m->second->type(), targets, visited)) {
          targets.insert(src);
          return true;
        }
      return false;
    }
  }
} // reachable


// ==============================================================

set<Unify_ECR*> UnificationBasedPtr::unique_ecr() const {
  set<Unify_ECR*> r;
  for(map<declNode*,Unify_ECR*>::const_iterator m=_ecr.begin(); m!=_ecr.end();
      m++)
    if(m->second) r.insert(m->second->root());
  return r;
} // unique_ecr


void UnificationBasedPtr::print_ecr() {
  cout << "all Unify_ECR:\n";
  set<Unify_ECR*> all_ecrs=Unify_ECR::allECR(), vars;
  set<UnifyType*> type_printed;
  map<UnifyType*,decl_list> points_to; // what points to same obj?

  for(map<declNode*, Unify_ECR*>::iterator m=_ecr.begin(); m!=_ecr.end(); m++) {
#define print_one_ecr(E) \
  (E)->print(); \
  cout << endl;

    Unify_ECR *E = m->second;
    assert(E && m->first);
    if(m->first->decl_location() == declNode::TOP && E->var() != m->first) {
      assert(_ecr.find(E->var()) != _ecr.end());
      continue;
    }

    cout << "  ";
    if(m->first->decl_location() == declNode::TOP) cout << "(global)::";
    cout << m->first->name() << "(";
    if(E->proc() && E->proc()->decl()!=E->var())
      cout << E->proc()->decl()->name() << ".";
    if(E->var()->type() && E->var()->type()->is_ellipsis() ||
       is_va_list(E->var()))
      cout << "<...>: ";
    else
      cout << E->var()->name() <<"): ";
    print_one_ecr(E)
    type_printed.insert(E->type());

    vars.insert(E);
//debug  all_ecrs.erase(E);

    UnifyType *ty = E->type();
    if(ty->objTyp()==OBJECT || ty->objTyp()==SIMPLE) {
      if(ty->objTyp()==OBJECT)
        ty = ty->object()->alpha->tao()->type();
      else
        ty = ty->simple()->alpha->tao()->type();
      points_to[ty].push_back(m->first);
    }
  }

#ifdef cx_vt
for(set<Unify_ECR*>::iterator e=all_ecrs.begin(); e!=all_ecrs.end(); e++)
  if((*e)->type()->id() == vt_e) {
    check_28920 = true;
    (*e)->print();
    check_28920 = false;
  }
for(map<declNode*, Unify_ECR*>::iterator m=_ecr.begin(); m!=_ecr.end(); m++) {
  if(m->first->type() && m->first->type()->typ() == Array &&
     m->first->decl_location() != declNode::FORMAL) {
    Unify_ECR *parent = m->second;
    if(parent->type()->objTyp() != SIMPLE && parent->type()->objTyp() !=OBJECT)
      cout << m->first->name() << "@" << m->first->coord() << " " << *parent
           << endl;
    is_Sim_Obj(parent->type());
    Unify_ECR *child = Sim_Obj_Alpha(parent->type())->tao();
    set<Unify_ECR*> parents = parentsOf(child->type());
    bool contain_parent = false;
    for(set<Unify_ECR*>::iterator p=parents.begin(); p!=parents.end(); p++)
      if((*p)->type() == parent->type()) { contain_parent=true; break; }
if(m->first->name() == "switches")
  cout << "switches " << *parent << " child " << *child << " contain "
       << contain_parent << endl;
    if(!contain_parent) {
      vt_e = child->type()->id();
      vt = parent->type()->id();
      cout << *child << " does not contain parent " << *parent << endl;
      check_28920 = true;
      child->print();
      check_28920 = false;
    }
  }
}
#endif


  map<int,int> n_src; // statistics: how many objects have n pointers to it?
  // print vars that points to same object.
  for(map<UnifyType*,decl_list>::iterator p=points_to.begin();
      p!=points_to.end(); p++)
    if(p->second.size() > 1) {
      for(decl_list_p d=p->second.begin(); d!=p->second.end(); d++)
        cout << (*d)->name() << ",";
      cout << " point-to " << * p->first << endl;
      n_src[p->second.size()] ++;
    } else n_src[1]++;

  // print more types
  for(set<Unify_ECR*>::iterator e=all_ecrs.begin(); e!=all_ecrs.end(); e++) {
    if(type_printed.find((*e)->type()) != type_printed.end()) continue;
    type_printed.insert((*e)->type());
    cout << "  ";
    print_one_ecr(*e)
  }

  /* print root chains
  cout << "ECR roots: ";
  for(set<ECR*>::iterator e=vars.begin(); e!=vars.end(); e++) {
    ECR *p = *e;
    cout << "(";
    while(p) {
      cout << p->id();
      p = p->parent();
      if(p) cout << " ";
    }
    cout << ") ";
  }
  cout << endl;*/

  // print statistic: n_src
  cout << "#ptrs to one obj  #such objs\n"
       << "----------------  ----------\n";
  for(map<int,int>::iterator m=n_src.begin(); m!=n_src.end(); m++)
    cout << m->first << "\t\t\t" << m->second << endl;

  cout << "number of tmp vars created by dismantler = " << _dismantler_tmp
       << endl;
//exit(1);
} // print_ecr


bool UnificationBasedPtr::is_va_list(declNode * decl) {
  // -- Look for }variables whose type is typedef va_list

  if (decl->type() && (decl->type()->typ() == Tdef)) {
    string name = ((tdefNode *) decl->type())->name();
    if (name == "va_list")
    return true;
    if (name == "__builtin_va_alist_t")
      return true;
    if (name == "__gnuc_va_list")
      return true;
  }

  return false;
} // is_va_list


procNode *UnificationBasedPtr::create_synthetic_proc(declNode * decl) {
//cout << "new synthetic proc " << decl->name() << "@" << decl->coord() << endl;
  if(_synthetic_proc[decl]) return _synthetic_proc[decl];
  assert(decl->type() && decl->type()->typ()==Func);
  procNode *new_proc = _synthetic_proc[decl] = new procNode(decl, NULL);
  if(! ((funcNode*)decl->type())->returns()->is_void()) {
    declNode *ret = new declNode("__ret", declNode::NONE,
                                 ((funcNode*)decl->type())->returns(),
                                 NULL, NULL);
    new_proc->return_decl( ret );
  }
  return new_proc;
} // create_synthetic_proc

UnificationBasedPtr *UnificationBasedPtr::analyze_all(Linker &linker) {
  cbzTimer unify_timer;
  unify_timer.start();
  UnificationBasedPtr *U = new UnificationBasedPtr(linker);
  for(unit_list_p u=CBZ::Program.begin(); u!=CBZ::Program.end(); u++)
    (*u)->walk(*U);
  U->finalize();
  unify_timer.stop();
  double t(unify_timer);
  cout << "STAT-unification-time              " << t << endl;
  return U;
}

// ==============================================================

class testUnify : public Phase {
  void run() {
    CBZ::PrintLineOffset = true;
    Linker linker;
    linker.link();
    UnificationBasedPtr *U = UnificationBasedPtr::analyze_all(linker);
    cout << "UnificationBasedPtr done\n";
    U->print_ecr();

    /*
    set<ECR*> ecrs = U->unique_ecr();
    for(set<ECR*>::iterator e=ecrs.begin(); e!=ecrs.end(); e++) {
      cout << "ECR: ";
      decl_list vars = (*e)->all_decls();
      for(decl_list_p v=vars.begin(); v!=vars.end(); v++) {
        ECR *ve = U->ecr(*v);
        if(ve->proc()) cout << ve->proc()->decl()->name() << ".";
        cout << (*v)->name() << ", ";
      }
      cout << endl;
    }*/
    cout << "unify done\n";
  }
};

Phases unify_phase("unify", new testUnify());


// to-do
// - distinguishable components of struct. section~3, eg. in fig 1
