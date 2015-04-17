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

#ifndef CBZ_UDDUCHAINS_H
#define CBZ_UDDUCHAINS_H

#include "c_breeze.h"

/** @brief Data structure to store ud and du chains.
 *
 * This data structure provides a generic and simple way to represent ud
 * and du chains. The data structure is almost symmetric, so that each ud
 * chain corresponds to a du chain, and vice versa. Each def is
 * represented by a threeAddrNode, and each use is represented either by
 * idNode, or an operandNode. The use site, the statement where the use
 * is, is optionally stored as well.
 *
 * The exception to the above invariant is when the data structure is used
 * to store reaching defintiions, where an ambiguous definition is
 * represented by a NULL threeAddrNode.
 */

class udduChains {
public:
  typedef list<threeAddrNode*> threeAddr_list;

private:
  map<exprNode*,threeAddr_list> ud_chain;
  map<threeAddrNode*,expr_list> du_chain;
  map<threeAddrNode*,stmt_list> du_site_chain;

public:
  udduChains(void) {}

#define contain(L,x) (find(L.begin(),L.end(),x) != L.end())

  /** @brief Adds an ud/du chain, togather with an optional use site. */
  inline void add(exprNode *use, threeAddrNode *def, stmtNode *useSite=NULL) {
    if(!use) return;
    if(! contain(ud_chain[use], def))
      ud_chain[use].push_back(def);
    if(! contain(du_chain[def], use))
      du_chain[def].push_back(use);
    if(useSite && ! contain(du_site_chain[def], useSite))
      du_site_chain[def].push_back(useSite);
  }

  /** @brief Query all defs for a given use. */
  inline threeAddr_list defs(exprNode *use) const {
    if(use && ud_chain.find(use)!=ud_chain.end())
      return ud_chain.find(use)->second;
    return threeAddr_list();
  }

  /** @brief Query all uses for a given def. */
  inline expr_list uses(threeAddrNode *def) const {
    if(def && du_chain.find(def)!=du_chain.end())
      return du_chain.find(def)->second;
    return expr_list();
  }

  /** @brief Query all use sites for a given def. */
  inline stmt_list useSites(threeAddrNode *def) const {
    if(def && du_site_chain.find(def)!=du_site_chain.end())
      return du_site_chain.find(def)->second;
    return stmt_list();
  }

  /** @brief Clear all chains. */
  void reset() { ud_chain.clear();  du_chain.clear(); du_site_chain.clear(); }
};

#endif
