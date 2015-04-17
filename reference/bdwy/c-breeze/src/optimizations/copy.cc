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
 
#include "c_breeze.h"
#include "bits.h"
#include "copy.h"
#include "reaching.h"
#include "copygenkill.h"
using namespace std ;

/* Kaushik Lakshmanan : lakshman@ece.utexas.edu
   The code has a very close structure to reaching.cc
 */
output_context oc (cout) ;

Node* copyDefinitionsChanger::at_proc (procNode *p, Order ord) {
	if (ord == Postorder) {
		// clean up
                cout << "CLEANING UP " << endl;
		delete [] num2node;
		num2node = NULL;
		return p;
	}
        
        // clear all the maps for each proc
	node2num.clear();
	defines.clear();
	in.clear();
	out.clear();
	defs.clear();
        local_defs.clear();
        replace.clear();
        // walk through the procedure and set up all the copy definitions
        gcw = new copyGenKillWalker (& node2num, & defines, & defs, & num2node, & n);
	p->walk (*gcw);
        
        v = (*gcw).new_flowval();
	blockNode *b = p->body();
        
        // get the references rhs and copy lhs definition from the gcw class
        _c_defs  = (*gcw).get_cdefs();
        _rhs_defs  = (*gcw).get_rhsdefs();
                 
        stmt_list_p r;
        r = b->stmts().begin();
        basicblockNode *B = (basicblockNode *) *r;
        
        in[B] = (defFlowVal *) v->clone();
        
        out[B] = (defFlowVal *) B->gen()->clone();
        
        r++; 
        delete v;
        
        // From the Dragon book ( refer available expressions, and Copy propogation
        // Chapter 10 and section 10.7
        // Algorithm 10.6.
        // The iterative analysis is very similar to available expressions
        // for B != B1 do out[B] ]= U - c_kill[B]
        // Initial estimate is too large
        
        while ( r != b->stmts().end()){ // copy_union is a defFlowval of all copy statements
                defFlowVal * U = (defFlowVal *)gcw->getUnion() -> clone();
                basicblockNode *B = (basicblockNode *) *r;
                U  -> Difference((defFlowVal *) B->gen()->clone());
                out[B] = (defFlowVal *)U -> clone ();
                r++;
        }

        defFlowVal *oldout = gcw->new_flowval();
	
        bool change = true;
        
        // From the Dragon book
        // for all blocks B != B1 (intial block)
        // the algorithm is very similar to available expressions problem as mentioned
        // in the dragon book.

        while (change){
                change = false;
                r = b->stmts().begin();
                r++;
      
                while ( r != b->stmts().end()){
                        basicblockNode *B = (basicblockNode *) *r;
                        v = gcw->new_flowval();
                        in[B] = (defFlowVal *) v->clone();
                        //in[B] is empty initially
                        
			defFlowVal *inb = in[B];
			defFlowVal *outb = out[B];
                        
                        // in[B] = Intersection of all Predecessor Blocks of B
                        
                        basicblock_list_p q;
                        q=B->preds().begin();
                        inb->Union (out[*q]);
                        
                        q++;

                        while (q!=B->preds().end()){
                                inb -> Intersect( out [*q]);
                                q++;
                                 
                        }
                        
                        //oldout = out[B]
                        oldout->copy (outb);
                        // out[B] = c_gen[B] U (in [B] - c_kill[B])
                        
                        // v is a copy of in[B]
                        v->copy (inb);
			v->Difference (B->kill());
			v->Union (B->gen());
			outb->copy (v);
                        
                        r++;
                        // check if oldout and new calculated out are the same
                        
                        if (oldout->diff(v)) change = true;
                        
                }
                

        }
      
        return p;	
}


void copyDefinitionsChanger::make_ud_chains (exprNode *e) {
        
        map <declNode *, declNode *> ::iterator p;
        map <idNode *,idNode* > ::iterator x;
        
        if (!e) return;
        switch (e->typ()) {
	case Id: {

                idNode *lhs = (idNode *) e;
                
                
                p =  local_defs. find(lhs -> decl());
		// maybe unknown id, like a function identifier
                if(p == local_defs.end())
                        return;
                x = replace.find(lhs);
                
                declNode* rhs_decl = local_defs[lhs->decl()];
                idNode *rhs = new idNode((rhs_decl->name()).c_str(),Coord::Unknown);
		udChainAnnote *a = new udChainAnnote;
		bool has_ambiguous = false;
                binaryNode* b = new binaryNode ('=',lhs,rhs,Coord::Unknown);
                exprNode * e = b;
                exprstmtNode * exp = new exprstmtNode (e);
                stmtNode *t = exp;
                
                a->ud_chain().push_back (t);
                lhs ->annotations().push_front (a);
                
                // here we need to find all the uses of 'x'
                // if it is in the local_defs list
                // we need to replace that id?

                break;
        }
        case Binary: {
                binaryNode *b = (binaryNode *) e;
                // if the lhs of the binary node involves
                // some decl from the local_decl
                // we need to eliminate that.
                
		if (b->op()->id() != '=')
			make_ud_chains (b->left());
                else{

                        idNode * lhs_id = (idNode *) (b -> left());
                        idNode * rhs_id = (idNode *) (b -> right());
                        p = local_defs.find(lhs_id ->decl());
                      
                        // if lhs is being redifined and 
                        // if the lhs exists in the list of local
                        // copies , then the copy can no longer be
                        // propagated

                        // erase the entry in the table corresponding to
                        // the lhs of the expression x = y
                        
                        if(p != local_defs.end()){

                                local_defs.erase(p);
                        }

                        // go through the list and erase the entry
                        // if the rhs has been modified also
                        // for eg: x = y is erased if y is modified
                        
                        for(p = local_defs.begin(); p != local_defs.end(); p++){
                                if (((*p). second) == lhs_id-> decl()){

                                        local_defs.erase(p);
                                }
                                        
                        }
                        
                        // if a new copy generated
                        // add that to the local list of copies
                        
                        if(b ->right()->typ() == Id){
                                
                                idNode * rhs_id = (idNode *) (b -> right());
                                local_defs[lhs_id -> decl()] = rhs_id ->decl(); 
                        }
                       

                        break;
                        
                }
                        
		if (b->op()->id() != Operator::ARROW)
			make_ud_chains (b->right());
                break;
        }
        case Unary: {
                
                unaryNode *u = (unaryNode *) e;
		// an address computation isn't a
		// use in a real sense; it can't
		// e.g. be replaced by a copy,
		// it has to be the real thing,
		// and doesn't fetch the rvalue
		// of its argument.
		if (u->op()->id() == Operator::ADDRESS) break;
                make_ud_chains (u->expr());
                break;
        }
        case Call: {
                callNode *c = (callNode *) e;
		// function pointer?
		make_ud_chains (c->name());
		// all the args
		for (expr_list_p i=c->args().begin();
                     i!=c->args().end(); i++)
			make_ud_chains (*i);
                break;
        }
        default: ; // we don't care
        }
}


void copyDefinitionsChanger::make_ud_chains (stmtNode *s) {
        if (s->typ() == If) {
		ifNode *i = (ifNode *) s;
		make_ud_chains (i->expr());
	} else if (s->typ() == Expr) {
		exprstmtNode *ex = (exprstmtNode *) s;
		make_ud_chains (ex->expr());
	} else if (s->typ() == Return) {
		returnNode *r = (returnNode *) s;
		make_ud_chains (r->expr());
	}
}


// all the ids that need to be changed have an annotation list
// in them
// If the list is empty we donot bother changing the id

Node* copyDefinitionsChanger:: at_id (idNode * the_id, Order ord){
        
        map <idNode *,idNode * >::iterator pp;
        annote_list &a =  the_id -> annotations();
        if ( a .size () == 0)
                return the_id; // annote_list should not be empty
        
        udChainAnnote * ud = (udChainAnnote * ) (*(a. begin()));
        stmt_list &s =  ud -> ud_chain();
        if ( s.size() == 0)
                return the_id;
        
        //if an annotation exists then we return the rhs of the
        // annotation that was inserted
        
        stmtNode * stm= *(s.begin());
        exprNode * e = ((exprstmtNode *)stm) -> expr();
        binaryNode * b = (binaryNode*) e;
        return b -> right();
        
}

Node* copyDefinitionsChanger:: at_basicblock (basicblockNode * b, Order ord){
        
        stmt_list_p r;
        
                 local_defs.clear();
                 for(int j = 0; j < n; ++j){ // for all copies that are coming into the block
                         if (in[b] -> in (j)){
                                 // if they are a copy definition
                                
                                 exprstmtNode *e = (exprstmtNode *) (num2node[j]);
                                 binaryNode *t = (binaryNode *) e -> expr ();
                                                                  
                                 idNode *i_lhs = (idNode *) (t -> left());
                                 idNode *i_rhs = (idNode *) (t -> right());

                                 // for a copy of type x = y
                                 // create a table of mapping between x and y
                                 
                                 local_defs[i_lhs -> decl()]  = i_rhs ->decl();
                                
                         
                         }
                 }
                 // make ud chains for each statement
                 for (r = b->stmts().begin(); r != b->stmts().end(); r++) {
                         
                         make_ud_chains (*r);
                 }
                 
            
        return b;
        
        
}

class reachingPhase1 : public Phase {
public:

	void run () {
		unit_list_p n;
		copyDefinitionsChanger rdw;
         
		for (n=CBZ::Program.begin(); n!=CBZ::Program.end(); n++)
			(*n)->change (rdw);
	}
};

Phases reachingPhase1("copy", new reachingPhase1());





