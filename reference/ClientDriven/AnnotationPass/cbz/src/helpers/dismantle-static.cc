// ----------------------------------------------------------------------
//
//  C-Breeze
//  C Compiler Framework
// 
//  Copyright (c) 2000 University of Texas at Austin
// 
//  Samuel Z. Guyer
//  Daniel A. Jimenez
//  Calvin Lin
//  Adam Brown
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
//
// dismantle-static.cc
//

#include <sstream>
#include "c_breeze.h"
#include "dismantle.h"
#include "ref_clone_changer.h"

StaticToGlobalDismantle::StaticToGlobalDismantle(void) :
  Changer(Both, Subtree, false),
  _cur_unit(0),
  _cur_proc(0),
  _declareProc(false)
{}

class UpdateTdefChanger : public Changer {
public:
  UpdateTdefChanger(const map<typeNode *, string> & newNames) :
    Changer(Preorder, Subtree, false),
    _newNames(newNames)
  {}
  Node * at_tdef(tdefNode * the_tdef, Order ord) {
    if ( _newNames.find(the_tdef->def()) != _newNames.end() )
      the_tdef->name(_newNames[the_tdef->def()]);
    return the_tdef;
  }
private:
  map<typeNode *, string> _newNames;
};

Node * StaticToGlobalDismantle::at_unit(unitNode * the_unit, Order ord) {
  if ( ord == Preorder ) {
    _cur_unit = the_unit;
    _topLevelSuespecs.clear();
  } else {
    UpdateTdefChanger utc(_newTypedefName);
    the_unit->change(utc);
  }
  return the_unit;
}

void StaticToGlobalDismantle::rename_decl(declNode * to_rename,
					  procNode * the_proc) {
  if ( to_rename->storage_class() != declNode::EXTERN ) {
    if ( !to_rename->name().empty() ) {
      // rename the decl to ensure uniqueness at top level
      ostringstream ost;
      ost << DismantleUtil::new_id() << "_" << the_proc->decl()->name()
	  << "_" << to_rename->name();
      to_rename->name(ost.str());

      // update referring idNode's name()
      for ( id_list_p id = to_rename->ref_list().begin();
	    id != to_rename->ref_list().end();
	    id++ )
	(*id)->name(to_rename->name());

      if ( to_rename->storage_class() == declNode::TYPEDEF ) {
	_newTypedefName[to_rename->type()] = to_rename->name();

	if ( sueNode * the_sue = dynamic_cast<sueNode *>(to_rename->type()) ) {
	  // also need to rename the struct tag, if the struct was
	  // defined inside a procedure
	  suespecNode * the_suespec = the_sue->spec();
	  if (_topLevelSuespecs.find(the_suespec) == _topLevelSuespecs.end()) {
	    ostringstream sueost;
	    sueost << DismantleUtil::new_id() << "_" 
		   << the_proc->decl()->name()
		   << "_" << the_suespec->name();
	    the_suespec->name(sueost.str());
	  }
	}
      }
    } else {
      // this must be a declaration for a sue type w/o a typedef
      sueNode * the_sue = dynamic_cast<sueNode *>(to_rename->type());
      assert(the_sue);
      suespecNode * the_suespec = the_sue->spec();

      ostringstream ost;
      ost << DismantleUtil::new_id() << "_" << the_proc->decl()->name()
	  << "_" << the_suespec->name();
      the_suespec->name(ost.str());
    }
  } // end if ( ... != declNode::EXTERN )
}

Node * StaticToGlobalDismantle::at_proc(procNode * the_proc, Order ord) {
  if ( ord == Preorder ) {
    _cur_proc = the_proc;
    _declsToMove.clear();
    _declareProc = false;
  } else { // ord == Postorder
    assert(_blockStack.empty());
    def_list_p procIter = find(_cur_unit->defs().begin(),
			       _cur_unit->defs().end(),
			       the_proc);
    if ( _declareProc ) {
      declNode * proc_decl = 
	(declNode *) ref_clone_changer::clone(the_proc->decl(), false);
      proc_decl->decl_location(declNode::TOP);
      _cur_unit->defs().insert(procIter, proc_decl);
    }

    set<declNode *> moved;
    for ( map<declNode *, decl_list>::iterator p = _declsToMove.begin();
	  p != _declsToMove.end();
	  p++ ) {
      for ( decl_list_p q = p->second.begin(); q != p->second.end(); q++ ) {
	if ( moved.find(*q) == moved.end() ) {
	  _cur_unit->defs().insert(procIter, *q);
	  moved.insert(*q);
	  (*q)->decl_location(declNode::TOP);
	  rename_decl(*q, the_proc);
	}
      }
      _cur_unit->defs().insert(procIter, p->first);
      p->first->decl_location(declNode::TOP);
      rename_decl(p->first, the_proc);
    }
  }
  return the_proc;
}

Node * StaticToGlobalDismantle::at_block(blockNode * the_block, Order ord) {
  if ( ord == Preorder ) {
    _blockStack.push_front(the_block);
  } else { // ord == Postorder
    assert(_blockStack.front() == the_block);
    _blockStack.pop_front();
  }
  return the_block;
}

class RequiresGatherer : public Walker {
public:
  RequiresGatherer(void) :
    Walker(Preorder, Subtree)
  {}
  virtual void at_id(idNode * the_id, Order ord) {
    _decls.push_back(the_id->decl());
    the_id->decl()->walk(*this);
  }
  virtual void at_tdef(tdefNode * the_tdef, Order ord) {
    _types.push_back(the_tdef->def());
    the_tdef->def()->walk(*this);
  }
  virtual void at_sue(sueNode * the_sue, Order ord) {
    _types.push_back(the_sue);
  }
  bool requires(declNode * the_decl) {
    bool isReqdDecl = ( find(_decls.begin(), _decls.end(), the_decl)
			!= _decls.end() );
    bool isReqdTypedef = the_decl->storage_class() == declNode::TYPEDEF
      && ( find(_types.begin(), _types.end(), the_decl->type())
	   != _types.end() );
    bool isAbstract = the_decl->name().empty();
    bool matchingSuespec = false;
    if ( sueNode * decl_sue = dynamic_cast<sueNode *>(the_decl->type()) ) {
      suespecNode * decl_spec = decl_sue->spec();
      for ( list<typeNode *>::iterator p = _types.begin();
	    p != _types.end();
	    p++ ) {
	if ( sueNode * the_sue = dynamic_cast<sueNode *>(*p) )
	  if ( decl_spec == the_sue->spec() )
	    matchingSuespec = true;
      }
    }

    return ( isReqdDecl || isReqdTypedef || (isAbstract && matchingSuespec) );
  }
private:
  decl_list _decls;
  list < typeNode * > _types;
};

Node * StaticToGlobalDismantle::at_decl(declNode * the_decl, Order ord) {
  if ( ord == Preorder ) {
    if ( the_decl->decl_location() == declNode::TOP ) {
      // gather listing of struct's defined at the top level, so that
      // they don't get inadvertently renamed
      if ( sueNode * the_sue = dynamic_cast<sueNode *>(the_decl->type()) ) {
	_topLevelSuespecs.insert(the_sue->spec());
      }
    } else if ( the_decl->storage_class() == declNode::STATIC 
		&& the_decl->decl_location() == declNode::BLOCK ) {
      // ensure that the_decl appears in _declsToMove
      _declsToMove[the_decl].clear();

      RequiresGatherer rg;
      the_decl->walk(rg);

      _declareProc = _declareProc || rg.requires(_cur_proc->decl());

      decl_list_p iter = _declsToMove[the_decl].end();
      block_list_p p = _blockStack.begin();
      while ( p != _blockStack.end() ) {
	blockNode * cur_block = *p;
	p++;
	// determine if cur_block contains any Nodes that the_decl
	// depends upon.
	decl_list_p q = cur_block->decls().begin();
	while ( q != cur_block->decls().end() ) {
	  declNode * cur_decl = *q;
	  q++;

	  if ( rg.requires(cur_decl) ) {
	    cur_block->decls().remove(cur_decl);
	    _declsToMove[the_decl].insert(iter, cur_decl);
	  }
	}
	iter = _declsToMove[the_decl].begin();
      }
      return NULL; // remove this decl from original location
    }
  }
  return the_decl;
}
