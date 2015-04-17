// $Id: inliner.h,v 1.7 2003/08/07 23:13:48 pnav Exp $
// ----------------------------------------------------------------------
//
//  C-Breeze
//  C Compiler Framework
// 
//  Copyright (c) 2003 University of Texas at Austin
// 
//  Samuel Z. Guyer
//  Adam Brown
//  Teck Bok Tok
//  Paul Arthur Navratil
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
#ifndef CBZ_INLINE_H
#define CBZ_INLINE_H

#include <c_breeze.h>
#include <ref_clone_changer.h>
#include <dismantle.h>
#include <linker.h>
#include <cfg.h>
#include <callgraph.h>
#include <deque>


using namespace std;
typedef set<procNode *>::iterator proc_set_p;

procNode * findmain();

/** This phase inlines all functions with type inline.  It can be
 * called from the command line with -fi.  It needs both the cfg
 * and a callgraph, so it calls both cfg_changer and callgraph_walker.
 * This phase is NOT idempotent.  It is idempotent assuming no 
 * recursive functions have been marked inline.
 *
 * The function inliner works in two passes.  The first identifies
 * callsites which can/should be inlined.  Under normal operation,
 * it currently only inlines the call sites of functions of type inline.
 * Hopefully, it will not be difficult to add other heuristics.
 * The second pass inlines all call sites identified by the first pass.
 *
 * After function inlining is complete, the code is re-dismantled 
 * to make certain all variables in each scope are unique.
 */

class function_inline:public Changer
{
   public:
      /* This constructor is really meant for internal use only.
       * To inline functions, please uses inline_functions.
       */
      function_inline(set<threeAddrNode *> & m,  
                      map<declNode *, procNode *> & m2, 
                      set<procNode *> & s1, set<procNode*> & s2):
         Changer(Both, Subtree, false), 
         _callinline(m),
         _procmap(m2), 
         _procs(s1), 
         _inlined(s2) {};

      /** Use this for non-command line inlining.  Root is the top of
        * the call graph.  If null, main will be used.  The code is
        * expected to be in cfg form.
        */
      static void inline_functions(procNode * root);

      /** This function identifies callsites and calls the inliner*/
      virtual Node * at_threeAddr(threeAddrNode * ta, Order ord);

      /** This function maintains the global call stack to handle
        * recursion and mutual recursion
        */
      virtual Node * at_proc(procNode * p, Order ord);
      void insert_proc(procNode * m);
      void process(callGraph & cg);



   private:
       set<threeAddrNode *> & _callinline;
       map<declNode *, procNode *> & _procmap;
       set<procNode*> & _procs;
       set<procNode*> & _inlined;
       bool already_inlined_calls(callGraphNode *);
       deque<procNode*> _callstack;
       deque<procNode*> _localcallstack;
       bool inlocalstack(procNode * p);
       stmtNode * inliner(idNode * call, procNode * p, 
                           threeAddrNode * callsite);


};

class fi: public Phase
{
   public:
      void run();
};

#endif
