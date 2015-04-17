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
#include "copygenkill.h"
#include "copy_getdefs.h"

// get all stmts that are definitions.
// defines, provided as an empty map to the constructor, 
// will on exit map statements to the declarations of the variables 
// they define.

// return true if an expression contains a procedure call

// find out whether and what an expression statement defines

bool GetCopyDefsWalker::contains_call (exprNode *e) {
	if (!e) return false;
	switch (e->typ()) {
		case Call: return true;

		// search the left and right operands

		case Binary: {
			binaryNode *b = (binaryNode *) e;
			return contains_call (b->left()) 
			|| contains_call (b->right());
		}

		// search the operand
		case Unary: {
			unaryNode *u = (unaryNode *) e;
			return contains_call (u->expr());
		}

		// search the casted expression

		case Cast: {
			castNode *c = (castNode *) e;
			return contains_call (c->expr());
		}

		// none of these is a procedure call

		case Id:
		case Const:
		default: 
			return false;
	}
}

void GetCopyDefsWalker::handle_exprstmt (exprstmtNode *s) {

	exprNode *e = s->expr();
	if (!e) return;
	if (contains_call (e)) {
               
		ambiguous_defs->push_back (s);
		// don't return from here just yet;
		// this statement might define
		// even more stuff after the call
	}
        
	if (e->typ() == Binary) {
		binaryNode *b = (binaryNode *) e;
		if (b->op()->id() == '=') {
			exprNode *l = b->left();
			if (l->typ() == Unary) {
				unaryNode *u = (unaryNode *) l;
				if (u->op()->id() == Operator::INDIR) {

					// a pointer dereference is
					// an ambiguous definition

					ambiguous_defs->push_back (s);
					return;
				}
			} else if (l->typ() == Binary) {
				binaryNode *p = (binaryNode *) l;
				if (p->op()->id() == Operator::ARROW) {

					// this is another way to do
					// a pointer dereference

					ambiguous_defs->push_back (s);
					return;
				}
				if (p->op()->id() == Operator::Index) {
				  // an ambiguous ref for all intents
				  // and purposes
				  
				  ambiguous_defs->push_back (s);
				  return;
				}
			} else if (l->typ() == Id) {

				// id = something defines this id
                                
                               
				idNode *i = (idNode *) l;
				(*defines)[s] = i->decl();
                                
                               
                                        
                                // If it is copy statement
                                // x = y add it to the c_defines
                                
                                if(b->right()->typ() == Id){

                                        idNode *r_id = (idNode *)b->right();

                                        (*c_defines)[s] = i->decl();
                                        
                                        // get the definitions of the rhs id
                                        (*r_defines)[s] = (r_id->decl());
                                        
                                }
                                
			} else {
				// yikes, don't know what this is,
				// but I'm going to say it's an
				// ambiguous def conservatively.
				// this actually happens in 147.vortex
				//cerr << "unknown lvalue type? " << l->typ() << '\n';
				ambiguous_defs->push_back (s);
				return;
			}
		}

	}
}

void GetCopyDefsWalker::at_stmt (stmtNode *s, Order) {

	// if the statement is an expression statement, check
	// it for definitions

	if (s->typ() == Expr) {
		exprstmtNode *e = (exprstmtNode *) s;
		handle_exprstmt (e);
	}
}

copyGenKillWalker::copyGenKillWalker (
	// node2num - will map definition statements 
	// to bit positions in flow values

	map <stmtNode *, int> *n2n, 

	// defines - will map definition statements
	// to the variables (declNode's) they define

	map <stmtNode *, declNode *> *dfns, 
        
	// defs - will map a variable to a flow value
	// representing the set of all definitions
	// for that variable

	map <declNode *, defFlowVal *> *dfs,

	// num2node - will be an array mapping bit
	// positions (integers) to definition
	// statements
        
	stmtNode ***num2n,
	int *numdefs) : 
	Walker (Preorder, Subtree), 
	node2num(n2n),
        defines(dfns),
        defs(dfs),
        pn2n (num2n),
	pn (numdefs),
	_flowval(NULL),
	num2node(NULL) {

        
}

copyGenKillWalker::~copyGenKillWalker (void) { }




void copyGenKillWalker::at_proc (procNode *p, Order) {
	int     i;
        int     j;
        defFlowVal *w;
	// get the definitions from this procedure
        
	defines->clear();
        c_defines.clear();
        c_defs.clear();
        rhs_defs.clear();
	GetCopyDefsWalker gdw (&c_defines,&rhs_defines, defines, & ambiguous_defs);
	p->walk (gdw);

	map <stmtNode *, declNode *>::iterator r;
        n = defines -> size();
	*pn = n;

	// num2node will map bit positions to statements.
	// bit positions go from 1..n, since 0
	// might be returned by the map on an unrecognized
	// statement
        
	num2node = new stmtNode *[n + 1];
	*pn2n = num2node;

	// get an empty flow value we can clone later

	_flowval = new defFlowVal (n, node2num, num2node);
	_flowval->to_top();
        
        //copy_union is the Union set of the all the copy flow values
        copy_union = new defFlowVal (n, node2num, num2node);
        copy_union -> to_top();
        
	// map stmtNode's to bit positions
	
        // loop goes thorugh all the definitions
        
	for (i=1,r=defines->begin(); r!= defines->end(); i++,r++) {
                
		(*node2num)[(*r).first] = i;

		// the i'th bit position represents this definition

		num2node[i] = (*r).first;

		// for the variable declaration (*p).second,
		// defs[(*r).second] will contain all
		// its definitions

		defFlowVal *v = (*defs)[(*r).second];
                //defFlowVal *w = NULL;
		// no such flow val?  make one.
		if (v == NULL) {
			v = (defFlowVal *) _flowval->clone();
			(*defs)[(*r).second] = v;
		}
                v->insert (i);
                
                /*if the statement is a copy statment*/
                // insert the bits for the rhs and lhs id defs
                map <stmtNode *, declNode *>  :: const_iterator pp;
                pp = c_defines.find((*r).first);
                if(pp !=  c_defines.end()){
                       
                        w = rhs_defs[rhs_defines[(*r).first]];
                        v = c_defs[(*r).second];
                        
                        // no such flow val?  make one.
                        if( v == NULL)
                                v = (defFlowVal *) _flowval->clone();
                       
                         if( w == NULL)
                                w = (defFlowVal *) _flowval->clone();
                        c_defs[(*r).second] = v;
                      
                        rhs_defs[rhs_defines[(*r).first]] = w;
                        copy_union -> insert(i);
                        v->insert (i);
                        w->insert(i);
                        
                }
	}

        

	// if the first block has a label, we need a preheader
	// to receive gen for the parameters and globals

	basicblockNode *b = (basicblockNode *) p->body()->stmts().front();
	if (b->stmts().size() && b->stmts().front()->typ() == Label) {

		// make an empty preheader for the procedure

		b = new basicblockNode (NULL, NULL);
		b->comment() = "preheader to generate parameters and globals";
	
		// link it to the first block in the procedure
	
		((basicblockNode *)(p->body()->stmts().front()))->
			preds().push_back (b);
		b->succs().push_back ((basicblockNode *) 
			p->body()->stmts().front());
		p->body()->stmts().push_front (b);
	}

	// make gen and kill sets for each statement,
	// giving them initially empty gen/kill sets

	stmt_list_p t;
	for (t=p->body()->stmts().begin(); t!=p->body()->stmts().end(); t++) {
		b = (basicblockNode *) *t;
		stmt_list_p r;
		for (r=b->stmts().begin(); r!=b->stmts().end(); r++) {
			defFlowVal *gen = (defFlowVal *) _flowval->clone();
			defFlowVal *kill = (defFlowVal *) _flowval->clone();
			(*r)->gen (gen);
			(*r)->kill (kill);
                        
                        /* Make one for the all the copy sets of gen and kill */
                        
                        defFlowVal *c_gen = (defFlowVal *) _flowval->clone();
			defFlowVal *c_kill = (defFlowVal *) _flowval->clone();
			(*r)->gen (c_gen);
			(*r)->kill (c_kill);
		}
	}

	// for every statement that is an ambiguous definition,
	// kill  all the copy definitions

	for (t=ambiguous_defs.begin(); t!=ambiguous_defs.end(); t++) {

                defFlowVal *w = (defFlowVal *) (*t)->kill();
                w->Union ( copy_union);
	}
} 


void copyGenKillWalker::at_basicblock (basicblockNode *b, Order) {

        map <declNode *,defFlowVal *> :: const_iterator pp;
	stmt_list_p r;
        
        defFlowVal *gen, *c_gen;
	defFlowVal *kill,*c_kill;
        
	// make gen and kill for each statement

	for (r=b->stmts().begin(); r!=b->stmts().end(); r++) {

		// d is the definition (or just a statement)

		stmtNode *d = *r;

		// gen and kill are new flowvals, initially empty
                
		c_gen =  (defFlowVal *) d->gen();
		c_kill = (defFlowVal *) d->kill();
                
		// v is the variable defined by definition d.

		declNode *v = (*defines)[d];
                declNode *c = c_defines[d];
                declNode *rhs = rhs_defines[d];
                
		if (v == NULL && c == NULL && rhs == NULL) {

			// nothing to do; d is not a definition,
			// or may be ambiguous and thus not
			// in defines.

		}
               
                else if( c != NULL || v != NULL){ // Any defines statement
                        
                        // if a copy definition eg: x = y
                        // kill all other copies of type x = w
                        // kill all copies of type  z = x
                        
                        pp = c_defs.find(c);
                        if(pp !=  c_defs.end()){
                                
                                // kill all the c_defs from the Universal set

                                c_kill -> Union (c_defs[c]);

                                // kill all the definitions of
                                // c occurs on the rhs also
                                
                                pp = rhs_defs.find(c);
                                
                                if(pp != rhs_defs.end())
                                           c_kill -> Union(rhs_defs[c]);
                                
                                // Donot kill yourself
                                
                                c_kill -> remove(d);
                                
                                
                        }
                        else{ //may be a plain definition or the decl occurs only on  the rhs
                              // y = expression then kill x = y
                                
                                pp = c_defs.find(v); 
                                
                                if(pp !=  c_defs.end())
                                        c_kill -> Union (c_defs[v]);
                                
                                pp = rhs_defs.find(v); 
                                if(pp !=  rhs_defs.end())
                                        c_kill -> Union (rhs_defs[v]);
                        }
                          
                                                                   
                }
                if (c != NULL){ /*If a copy statement generate*/
                        c_gen -> insert (d);
                        
                } 
        }
	

	// gen and kill are defined for each statement.
	// put them all together for gen and kill
	// for this block.

	c_gen = (defFlowVal *) _flowval->clone();
	c_kill = (defFlowVal *) _flowval->clone();

	// the (pre)header should be seen to generate all ambiguous
	// definitions, so that parameters and global variables are 
	// seen with definitions reaching into the procedure
        
	for (r=b->stmts().begin(); r!=b->stmts().end(); r++) {
		stmtNode *d = *r;

		// from Dragon book:
		// gen[S] = gen[S_2] U (gen[S_1] - kill[S_2])
		// kill[S] = kill[S_2] U (kill[S_1] - gen[S_2])
		// we think of the basic block as seen so far
		// as S_1, and the current statement (d) as S_2,
		// with the resulting current basic block so far
		// as S.

                c_gen->Difference (d->kill());
                
                c_gen->Union (d->gen());
                
		c_kill->Difference (d->gen());
                
		c_kill->Union (d->kill());

	}
      
	b->gen (c_gen);
	b->kill (c_kill);
        
         
}

defFlowVal *copyGenKillWalker::new_flowval (void) {
	defFlowVal *f = (defFlowVal *) _flowval->clone();
	f->to_top();
	return f;
}


