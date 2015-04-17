class codeFixup
{
public:

  // -- General unitNode fixup routine (includes dismantling and CFG)

  static void fix_unit(unitNode * Unit)
  {
    // -- Establish all the variable declarations, and goto-labels

    Unit->fixup();

    // -- Dismantle and put in CFG form

    cout << "    + Dismantle " << Unit->input_file() << endl;

    cfg_changer::generate_cfg (u, DEFAULT_DISMANTLER_FLAGS | INDEX_IS_PRIMITIVE);

    // -- Sem-check to compute types

    semcheck_walker::check(u, false);

    // -- Again (is this necessary?)

    Unit->fixup();
    
    // -- Then remove the unused definitions

    for (def_list_p dp = Unit->defs().begin();
	 dp != Unit->defs().end();
	 ++dp)
      {
	defNode * dn = (*dp);
	if (dn->typ() == Proc) {
	  procNode * pr = (procNode *) dn;

	  // -- Visit all block-level declarations and removed the unused
	  // ones

	  decl_list & decls = pr->body()->decls();
	  decl_list_p p = decls.begin();
	  decl_list_p temp;

	  while (p != decls.end()) {
	    declNode * decl = *p;
	    temp = p;
	    p++;
	    if (decl->references() == 0)
	      decls.erase(temp);
	  }

	  // -- Visit the basic blocks and remove any unused labels
	  
	  stmt_list & stmts = pr->body()->stmts();
	  stmt_list_p q = stmts.begin();
	  stmt_list_p temp2;

	  while (q != stmts.end()) {
	    stmtNode * stmt = *q;
	    temp2 = q;
	    q++;
	    if (stmt->typ() == Label) {
	      labelNode * label = (labelNode *) stmt;
	      if (label->references().empty())
		stmts.erase(temp2);
	    }
	  }
	}
      }
  }

  // -- More cleanup (includes removing unused stuff)

  static void cleanup(unitNode * Unit)
  {
    Unit->fixup();

    // -- Visit all statements and declarations, removing the ones not
    // needed.

    for (def_list_p dp = Unit->defs().begin();
	 dp != Unit->defs().end();
	 ++dp)
      {
	defNode * dn = (*dp);
	if (dn->typ() == Proc) {
	  procNode * pr = (procNode *) dn;
	  blockNode * body = pr->body();

	  // -- Removed unused variables

	  decl_list_p p = body->decls().begin();
	  while (p != body->decls().end()) {
	    declNode * var = *p;
	    typeNode * type = var->type();
	    assert(type);

	    decl_list_p cur = p;
	    ++p;

	    if ((var->references() == 0) &&
		(type->typ() != Func) &&
		(var->decl_location() == declNode::BLOCK) &&
		(var->storage_class() != declNode::EXTERN))
	      {
		body->decls().erase(cur);
	      }
	  }

	  // -- Remove empty statements

	  for (stmt_list_p q = body->stmts().begin();
	       q != body->stmts().end();
	       ++q)
	    {
	      if ((*q)->typ() == Block) {
		blockNode * block = (blockNode *)(*q);
		stmt_list_p w = block->stmts().begin();

		while (w != block->stmts().end()) {
		  stmtNode * s = *w;

		  stmt_list_p cur = w;
		  ++w;

		  if (s->typ() == Expr) {
		    exprstmtNode * ex = (exprstmtNode *)s;
		    if ( ! ex->expr())
		      block->stmts().erase(cur);
		  }
		}
	      }
	    }
	}
      }
  }
};
	    
