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

#ifndef CBZ_COPY_H
#define CBZ_COPY_H
#include "copygenkill.h"


/** @brief Performs global (intraprocedural) copy propagation.
    @ingroup Phases
*/
class copyDefinitionsChanger : public Changer {

public:

       
        copyDefinitionsChanger (void) : Changer ( Preorder, Subtree,false){
               gcw = NULL;
        }

        ~copyDefinitionsChanger(){
                if (gcw){
                        //clean up
                        delete gcw;
                        gcw = NULL;
                }
                        
        }
        
	Node* at_proc (procNode *, Order);
        Node* at_basicblock (basicblockNode *, Order);
        Node* at_id (idNode*, Order);
private:
       

	/** @brief make ud-chains for each use in this expression */
	void make_ud_chains (exprNode *);

	/** @brief make ud-chains for each use in this statement */
	void make_ud_chains (stmtNode *);

	/** @brief in and out sets for each basic block node */
	map <basicblockNode *,defFlowVal *> in, out;

	/** @brief array of pointers to stmtNode's maps bit positions
	    to definitions in the current procedure */
	stmtNode **num2node;

	/** @brief n is the number of definitions in the current procedure */
	int	n;

	/** @brief the current value  */
	defFlowVal *current_in;

	/** @brief maps def2initions (stmts) to bit positions */
	map <stmtNode *, int> node2num;	

	/** @brief maps a statement to the variable it defines */
	map <stmtNode *, declNode *> defines;

	/** @brief maps a variable to the definitions 
	    that (may) affect it */
	map <declNode *,defFlowVal *> defs;
        
        /** @brief reference to the c_defs
	    that was declared in copydenkillwalker */
        map <declNode *, defFlowVal *>* _c_defs;
        
        /** @brief reference to the rhs_defs
	    that was declared in copygenkillwalker */
        map <declNode *, defFlowVal *>* _rhs_defs;
        
        /** @brief local map for each basic block to keep track
	    of which copy is active at a time */
        map <declNode *, declNode *> local_defs;
        
        map <declNode *, declNode *> localrhs_defs;
        
        map <idNode *,idNode* > replace; 
       
        defFlowVal *v;
        
        // gcw is a reference to the genkillwalker
        copyGenKillWalker * gcw ;
        
};
            
#endif // CBZ_REACHING_H










