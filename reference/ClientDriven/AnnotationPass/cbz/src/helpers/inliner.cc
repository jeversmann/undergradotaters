// $Id: inliner.cc,v 1.7 2004/11/16 18:36:51 ans Exp $
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
#include "inliner.h"

/* Should only be used internally by the function inliner*/
class identify_inlinees:public Walker
{
   public:
      identify_inlinees(map<declNode *, procNode *> & m):
         Walker(Preorder, Subtree),
         _procmap(m) {};
      void at_threeAddr(threeAddrNode * ta, Order ord);
      inline set<threeAddrNode*> & callinline(){ return _callinline;}
      inline set<procNode *> & procs(){ return _procs;}
      inline set<procNode *> & inlined(){ return _inlined;}

   private:
      set<threeAddrNode *>  _callinline;
      map<declNode *, procNode *> & _procmap;
      set<procNode*> _procs;
      set<procNode*> _inlined;
};


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
void
fi::run()
{
#if DEBUG
cout << " in THIS function inliner " << endl;
#endif
   Linker l;
   unit_list_p up;

   /* first, be certain code is dismantled and CFG is correct*/
   for (up = CBZ::Program.begin(); up != CBZ::Program.end(); up++)
      cfg_changer::generate_cfg(*up);

   /* For the generic pass, we want the root of the callgraph to be
    * main.  So, we begin by finding main's location in the program
    */
   procNode * mainfunc = findmain();

   l.link(); /*linker connectes all procedure definitions with their
              *declaration - even across compilation units
              */

   callGraph cg(mainfunc, l); /*Creates a callgraph with main as the root node*/


   /**function_inline needs the map generated by
     *identify_inlinees.  By identifying call sites to 
     *be inlined in a separate pass, we avoid problems 
     *with recursion */
   identify_inlinees i_i(l.procedure_declarations());
   function_inline f(i_i.callinline(), l.procedure_declarations(), 
                     i_i.procs(), i_i.inlined());
   /* This inserts main as a function which has not yet had its callees inlined.
    */
   if(mainfunc)
      f.insert_proc(mainfunc);
   else
      perror("Whoops!  Null root to callGraph.\n");

   /* Use identify inlinees find functions whose callsites should be inlined*/
   for (up = CBZ::Program.begin(); up != CBZ::Program.end(); ++up)
      (*up)->walk(i_i);

   /*Do the inlining*/
   f.process(cg);
   
   /*Re-dismantle to be certain all variables have unique names*/
   for (up = CBZ::Program.begin(); up != CBZ::Program.end(); ++up)
      Dismantle::dismantle(*up);

}


/** This function is similar to the previous one.  It differs in that the i
 * previous one is used with the -fi phase; this one is meant to be called 
 * from user code.
 * The root argument is the root for the callgraph; if it is NULL, then main
 * is found and used.
 * It expects the code to be in a valid cfg.
 */
void
function_inline::inline_functions(procNode * root)
{
   Linker l;
   unit_list_p up;

   if(root == NULL)
   {
      root = findmain();
   }

   l.link(); 
   callGraph cg(root, l);

   if(!root)
      perror("oh no! null to callgraph");

   /**function_inline needs the map generated by
     *identify_inlinees.  By identifying call sites to 
     *be inlined in a separate pass, we avoid problems 
     *with recursion */
   identify_inlinees i_i(l.procedure_declarations());
   function_inline f(i_i.callinline(), l.procedure_declarations(), 
                     i_i.procs(), i_i.inlined());

   f.insert_proc(root);

   for (up = CBZ::Program.begin(); up != CBZ::Program.end(); ++up)
      (*up)->walk(i_i);

   f.process(cg);

   for (up = CBZ::Program.begin(); up != CBZ::Program.end(); ++up)
      Dismantle::dismantle(*up);

}

/*This procedure fins the procNode for the "main" function in a given
 * program
 */
procNode * findmain()
{
   unit_list_p up;
   procNode * mainfunc=NULL;
   for (up = CBZ::Program.begin(); up != CBZ::Program.end(); ++up)
   {
      unitNode * unit = *up;

      for (def_list_p q = unit->defs().begin(); q != unit->defs().end(); ++q)
      {
         defNode * def = *q;

         if (def->typ() == Proc)
         {
            procNode * pr = (procNode *) def;
            if (pr->decl()->name() == "main")
            {
               mainfunc=pr;
            }
         }
      }
   }

   return mainfunc;
}


void
function_inline::insert_proc(procNode * m)
{
   _procs.insert(m);
}

/* This function calls the function inliner on callsites which are marked to be
 * marked to be inlined.  This marking is done by the identify_inlinees pass
 */
void
function_inline::process(callGraph & cg)
{

   int size = 0;  /* In an attempt to be certain everything that should be 
                   * inlined is inlined, we first inline the callsites of 
                   * procedures with the smallest number of calls.  This
                   * variable keeps track of where we are - as this number 
                   * increases, more procedures become eligible to have their 
                   * callsites inlined.
                   */
   bool changed = false;  /* We have a set of procedure nodes whose callsites
                           * need to be inlined.  This variable keeps track of 
                           * whether any of these nodes had their callsites
                           * inlined and were removed from the set.
                           */
#if DEBUG
   cout <<"in process" << endl;
#endif

   /*Procs is the set of procedues which need their callsites inlined*/
   while(!_procs.empty())
   {
      /*while loop since iterator is not always incremented*/
      proc_set_p p = _procs.begin();
      while ( p!= _procs.end())
      {

         /* Find the current procNode in the callGraph, and get its             
          * callgraphNode
          */
         callGraphNode * cgn = cg.lookup(*p);

         /*If this call graph node has the number of calls we are looking
          *for or its calls have already had their calls inlined, then...
          */
         if(cgn->calls().size()==size || already_inlined_calls(cgn))
         {
#if DEBUG
            cout << "going to function inliner "<< (*p)->decl()->name() <<endl;
#endif
            /*go to function inliner at procNode...*/
            (*p)->change(*this);
#if DEBUG
            cout << "inserting" << (*p)->decl()->name() << endl;
#endif
            /* insert procedure into the set of procedurse with their calls
             * already inlined
             */
            _inlined.insert(*p);

            /* Continue through set of procedures-needing-their-calls-inlined
             * but erase this one out of the set
             */
            proc_set_p tmp = p;
            p++;
            _procs.erase(tmp);


            /*If we were previously allowing the inlining of function whose
             * calls did not yet have all of their callsites inlined, go back to
             * inlining only those with no callsites or whose calls have already
             * had their calls inlined
             * Otherwise, set changed to be true - this prevents us from 
             * infinite-looping
             */
            if(size!=0)
            {
               p = _procs.begin();
               size = 0;
            }
            else
               changed = true;
         }
         /*if this procedure cannot yet have its calls inlined, continue..*/
         else
            p++;
      }
      /* If nothing changed, increase the size*/
      if(!changed)
         size++;
      else
        changed=false;
   }

#if DEBUG
   cout << "leaving process" << endl;
#endif
}

/* This function checks to see if the procedure's calls have already had
 * their calls inlined.  If so, it returns true, otherwise, it returns
 * false
 */
bool
function_inline::already_inlined_calls(callGraphNode * cgn)
{ 

#if DEBUG
   cout << "in already_inlined_calls"<<endl;
#endif

   /*First, get the list of calls...*/
   const callgraph_edge_list & cgel = cgn->calls();

   /*If this list is empty, this procedure is self-contained, return true*/
   if(cgel.begin()==cgel.end())
   {
#if DEBUG
      cout << "empty list, returning true"<< endl;
#endif
      return true;
   }

   /* Iterate thourgh the list of procedurse called*/
   for(callgraph_edge_list_cp p = cgel.begin(); p != cgel.end(); p++)
   {
      callGraphNode * checkinline = *p;
   
      /*get the procedure related to the callGraphNode*/
      procNode * procinline = checkinline->proc();
#if DEBUG
      cout << procinline->decl()->name() << endl;
#endif

      /* If this procedure is not int he set of precedures already inlined,
       * return false.
       */
      if(_inlined.find(procinline)==_inlined.end())
      {
#if DEBUG
cout <<"returning false" << endl;
#endif
         return false; 
}
   }

   /*All must have been found, return true*/
#if DEBUG
cout <<"returning true" << endl;
#endif
   return true;
}

/* Since we may be inlining callsites where the callee has not yet had its 
 * callsites inlined, we keep track of the callstack to prevent 
 * getting trapped by recursion of any sort
 */
Node *
function_inline::at_proc(procNode * p, Order ord)
{
#if DEBUG
   cout <<"inside at_proc" << endl;
#endif

   if(ord == Preorder)
      _callstack.push_front(p);
   else
      _callstack.pop_front();

#if DEBUG
   cout <<"leaving at_proc" << endl;
#endif

   return p;
}

/*We find call sites marked inline from the exprstmtNode because we
 *need the call site to properly inline the node.
 *This checks for a call site and then checks if the callNode is in
 *the set of nodes to be inlined.  If it is, the inliner is called.
 */
Node *
function_inline::at_threeAddr(threeAddrNode * ta, Order ord)
{

    /*Only want to do this Preorder...*/
    if(ord==Postorder)
       return ta;

#if DEBUG
    cout << "in threeAddr" << endl;
#endif
  
    /*return value*/
    stmtNode * s=NULL;


   /*Since the code has been dismantled, we have two cases to worry about.
    *1) a call by itself
    *2) a call as the rhs of an assignment statement
    */

   /*If the op is NULL, this is an assignment*/
   if(ta->op()==NULL)
      return ta;

   /*Do we have a callNode?*/
   if (ta->op()->id()==Operator::FUNC_CALL)
   {
      /* If it is a call node, find it's declaration and then get its
       * procedure declaration
       */
      operandNode * o = (operandNode *) ta->rhs1();
      indexNode * i = o->var();
   
      assert(i->typ()==Id); /*since we know this is a call, can't be a const*/
      idNode * call = (idNode *)i;
      declNode * decl = call->decl();
      procNode * proc = _procmap[decl];

      /*Was the linker able to find its body and is it set to be inlined?*/
      if (proc)
      {
         bool inlineit=false;

         /* if it is in this set, it should be an original callsite - not
          * one introduced by inlining
          */
         if(_callinline.find(ta)!=_callinline.end())
         {  
            //create local call stack, to be reset whenever at original callsite
            _localcallstack.clear();
            _localcallstack == _callstack; 
 
            inlineit = true;
         }
         else if(!inlocalstack(proc))
         {
            funcNode * func = (funcNode *) proc->decl()->type();
            if(func->type_qualifiers() & typeNode::INLINE)  
               inlineit = true;
         }

         if(inlineit)
         {
            _localcallstack.push_front(proc);
            /*if so, let's inline!*/
            s = inliner(call, proc, ta);
            /*We inlined, so return result from inliner*/
            return s;
         }
      }
   }

   /*no inlining, return original*/
#if DEBUG
   cout << " no inlining leaving threeAddr" << endl;
#endif
   return ta;
}

/*Checks if p is contained in the local stack*/
bool
function_inline::inlocalstack(procNode *p)
{
#if DEBUG
   cout << "in inlocalstack" << endl;
#endif
   for(int i = 0; i < _localcallstack.size(); i++)
   {
      if((_localcallstack[i])->decl()->name()==p->decl()->name())
         return true;
   }
   return false;
}

// Assumes dismantled code. Originally from Sam Guyer.
stmtNode *
function_inline::inliner(idNode * call, procNode * proc,
			 threeAddrNode * callsite)
{

   /// -- Clone the procedure. Here we use the ref_clone because
   /// it preserves the internal references (that is, the local variable
   /// declaration pointers). That way we can use the name mangler to fix
   /// any conflicting variable names.

   procNode * dup_proc = (procNode *) ref_clone_changer::clone(proc, false);

   /// -- Remove the body...this is the return object. BUT, get the exit
   /// basic block first.

   basicblockNode * exit = dup_proc->exit();
   blockNode * body = dup_proc->get_body();

   /// -- Fix the parameter passing: remove each formal parameter from
   /// the funcNode, use the actual argument from the callNode as the
   /// initializer, and add it to the block declarations of the body.

   /// Formal arguments...

   funcNode * fd = (funcNode *) dup_proc->decl()->type();
   decl_list & formal_args = fd->args();

   /// Local declarations...

   decl_list & topdecls = body->decls();

   /// Clone the call so we can cannibalize the actual arguments...

   threeAddrNode * dup_callsite = (threeAddrNode *) 
                              ref_clone_changer::clone(callsite, false);
   operand_list & actual_args = dup_callsite->arg_list();


   if (!fd->is_void_args())
   {
      operand_list::reverse_iterator actuals;
      decl_list::reverse_iterator formals;
 
      for(actuals = actual_args.rbegin(), formals = formal_args.rbegin();
          actuals!=actual_args.rend() && formals!=formal_args.rend();
          actuals++, formals++)
      { 
	 declNode * one_arg = *formals;
	 operandNode * one_act = *actuals;

	 /// -- Use the initialize to pass the actual value

	 one_arg->init(one_act);

	 /// -- Important: set the decl location to BLOCK

	 one_arg->decl_location(declNode::BLOCK);

	 topdecls.push_front(one_arg);

      }
   }

   /// -- Assumption: last statement of the exit basic block is the
   /// return statement. This is true after the regular dismantle/CFG
   /// phase. Remove it.

   returnNode * ret = (returnNode *) exit->stmts().back();
   exit->stmts().pop_back();

   assert(ret->typ() == Return);

   /// -- If there is a returned value and a left-hand side at the
   /// callsite, then set up the appropriate assignment.
   if(proc->return_decl())
   {
#if DEBUG
      cout << "return_decl non-null for "<< proc->decl()->name()<<endl;
#endif
   }
   if (proc->return_decl() && callsite->lhs())//in threeAddr, lhs non-null means
                                              //an assignment
   {

      /// -- Get the returned value
      declNode * retDecl = dup_proc->return_decl();
      idNode * returned_id = new idNode(retDecl);

      /// -- Copy the left-hand side of the binary node

      threeAddrNode * bin = (threeAddrNode *) callsite;
      operandNode * lhs = (operandNode *) 
                            ref_clone_changer::clone(bin->lhs(), false);

      /// -- Create a new assignment statement and place it at the end of
      /// the basic block.

      operandNode * rhs = new operandNode(returned_id);
      threeAddrNode * assignment = 
         new threeAddrNode(lhs, rhs, call->coord());
      exit->stmts().push_back(assignment);
   }

   return body;

}


/* This is part of the first pass of the function inliner.  
 * At a callNode, we look to see
 * if the proc is of type inline.  If so, we make the function to
 * be inlined.  This phase also has an inline_all variable, typically
 * set to false, which can be set to true to inline all functions.
 */
void
identify_inlinees::at_threeAddr(threeAddrNode * call, Order ord)
{
#if DEBUG
output_context oc(cout);
cout << "processing " ;
call->output(oc, NULL);
#endif
   if (call->op() && call->op()->id()==Operator::FUNC_CALL)
   {
#if DEBUG
cout << endl << "it was a call!" << endl;
#endif

      operandNode * o = (operandNode *) call->rhs1();
      indexNode * i = o->var();
      assert(i->typ()==Id); /*since we know this is a call, can't be a const*/
      idNode * c = (idNode *)i; 
      declNode * decl = c->decl();
      procNode * proc = _procmap[decl];
      if (proc)
      {
         funcNode * func = (funcNode *) proc->decl()->type();
         if(func->type_qualifiers() & typeNode::INLINE)
         {
#if DEBUG
cout << "should be inlined" << endl;
#endif

            _callinline.insert(call);
            _procs.insert(proc);
         }
         else
         {
            _procs.insert(proc);
            //_inlined.insert(proc);
         }
      }

   }
}

