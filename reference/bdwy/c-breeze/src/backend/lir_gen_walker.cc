// ----------------------
//     Chuck Tsen
// Started June 25th 2002
//   code_gen_phase.cc
// Updated June 25th 2002
// ----------------------

// ********************************************************
// *							  *
// * This walker should generate straight line code in	  *
// * the form of a linked list as it traverses the tree.  *
// * I'm starting out simple, trying to spit out psuedo	  *
// * RISC assembly code for simple arithmetic operations  *
// * 							  *
// * Next step is to perform coercion (automatic type	  *
// * conversion) for operations where the type does not   *
// * match.						  *
// *							  *
// ********************************************************

#include "c_breeze.h"
#include "lir_gen_walker.h"
#include "LIR.h"


lir_gen_walker::lir_gen_walker() : 
	Walker ( Both, Subtree ),
	_alloc( _struct_data )
{
	_skip_next_goto = 0;
	_suespec_visit = false;
	_info._alloc = &_alloc;
	_info._insts = NULL;
	_info._proc = NULL;
	_info._unit = NULL;
	_info._structs = &_struct_data;
	_is_call = false;
	_ignore_decl = false;
	_call_res_type = vt_unknown;
	_condition_stmt = NULL;
}

lir_gen_walker::~lir_gen_walker(){
}

// determine if unary node is a ptr or a ref operator
bool lir_gen_walker::is_indir_or_deref(unaryNode *the_unary){
	
	if ( (the_unary->op()->id() == Operator::INDIR) || (the_unary->op()->id() == Operator::ADDRESS) ){
		return true;
	}

	return false;
}

string lir_gen_walker::type_string(typeNode *the_type){
	if (the_type->is_char()){
		return "char";
	}
	else if (the_type->is_int()){
		return "integer";
	}
	else if (the_type->is_float()){
		return "float";
	}
	return "<some type>";
}

void lir_gen_walker::at_unit(unitNode * the_unit, Order ord){

	// what order is this?
	if (ord == Preorder) {

		// kill instruction list for unit
		the_unit->instructions().clear();

		// save this node in case we need to know what unit we're in
		_info._unit = the_unit;
		_info._insts = &(the_unit->instructions());

		// reset sue stuff
		_struct_data.init();

		// read all sue specs for this unit
		suespec_walker specs( _struct_data );
		the_unit->walk( specs );
	}
	else 
	{
		// we are no longer at any unit
		_info._unit = NULL;
		_info._insts = NULL;
	}
}

void lir_gen_walker::at_block(blockNode *block, Order ord) 
{
	// keep track of blocks for structure info
	if ( ord == Preorder )
		_struct_data.enter_block( block );
	else
		_struct_data.leave_block();
}

void lir_gen_walker::at_proc( procNode * the_proc, Order ord){

	// what order is this?
	if (ord == Preorder) 
	{
		// reset our allocator
		_alloc.reset();

		// remember what proc we're in
		_info.set_proc( the_proc );

		// kill instruction list and block list for proc
		the_proc->instructions().clear();
		the_proc->lir_blocks().clear();

		// handle pointer arithmetic here before we allocate the proc, in case we add
		//	any temporary vars.
		ptr_arith_walker ptrArith;
		the_proc->walk( ptrArith );

		// traverse this thing to allocate storage for procedure variables
		// NOTE: this has to be done after we clear instructions because it may
		//	generate some.
		the_proc->set_initial_stack_local_offset( -1 * CBZ::ArchInfo.get_stack_extra_top() );
		storage_alloc_walker allocWalk( _alloc );
		the_proc->walk( allocWalk );

		// add a BeginProc instruction
		_info._insts->push_back( Lir_Class::BeginProc( the_proc ) );
	}
	else 
	{
		// add an EndProc instruction
		_info._insts->push_back( Lir_Class::EndProc( the_proc ) );

		// we are no longer at any proc
		_info.set_proc( NULL );

		// print out all of the instructions
#if 1
		cout << "Initial LIR instruction listing:" << endl;
		instruction_list::const_iterator it = the_proc->instructions().begin();
		int num = 0;
		while ( it != the_proc->instructions().end() )
			cout << num++ << " " << **(it++) << endl;
		cout << endl << endl;
#endif

		// try to restore instruction list for unit
		if ( _info._unit )
            _info._insts = &(_info._unit->instructions());
		else
			_info._insts = NULL;
	}
}

// at the declaration nodes, we need to allocate space
void lir_gen_walker::at_decl(declNode *the_decl, Order ord){

	// suespec does it's own thing, we don't want to interfere.
	if (_suespec_visit || ord == Postorder)
		return;

	// ignore extra decls for calls - these don't make any sense
	if ( _ignore_decl )
		return;

	// call function to allocate space
	Matcher::match_declaration(_info, the_decl);
}

// this function does pattern matching for all assignment statements ( a = b, a = !b, a = b + c )
void lir_gen_walker::at_binary(binaryNode *the_binary, Order ord){

	if (ord == Preorder){
		// skip anything that's not an assignment
		if (the_binary->op()->id() != '=')
			return;

		// no condition anymore
		_condition_stmt = NULL;

		// is this thing assigning the results of a test?  if so, we need to handle this later
		if ( the_binary->right()->typ() == Binary && ((binaryNode*)the_binary->right())->op()->is_comparison() ){
			// handle this later as a conditional
			_condition_stmt = the_binary;
			return;
		}
		else if ( the_binary->right()->typ() == Unary && ((unaryNode*)the_binary->right())->op()->id() == '!' ){
			// handle this later as a conditional
			_condition_stmt = the_binary;
			return;
		}

		// assigning the result of some binary operator
		if (the_binary->right()->typ() == Binary  && (((binaryNode *)the_binary->right())->op()->id() != Operator::ARROW)){ 
			Matcher::match_binop_asgmt(_info, the_binary);
		}
		
		// assigning the result of some unary operator
		else if (the_binary->right()->typ() == Unary && (!is_indir_or_deref((unaryNode *)the_binary->right())) ){
			Matcher::match_unop_asgmt(_info, the_binary);
		}

		// assigning the result of a cast
		else if (the_binary->right()->typ() == Cast){
			Matcher::match_cast(_info, the_binary);
		}

		// assigning something direct
		else if (	(the_binary->right()->typ() == Const) || 			// direct assignement of immediate
					(the_binary->right()->typ() == Id) || 				// direct assignment of variable
					(is_indir_or_deref((unaryNode *)the_binary->right())) || 		// direct assignment of ptr or ref
					(the_binary->right()->typ() == Binary && ((binaryNode *)the_binary->right())->op()->id() == Operator::ARROW) ) {
			Matcher::match_direct_asgmt(_info, the_binary);		
		}
	}
	else {
		// is this a function call?  if so, store the result in the specified place
		if ( the_binary->op()->id() == '=' && the_binary->right()->typ() == Call ) {

			// where did the return value go?  (if anywhere...)
			Register regRet;
			if ( ! Register::getRegRetVal( _call_res_type, regRet ) )
			{
				// if we are going to assign it there really should be a return value....
				assert( false );
				return;
			}

			// write the return value into the correct place
			declNode * pRetDecl = DATA_CONTENTS_RETVAL;
			Matcher::match_simple_store( _info, the_binary->left(), regRet, pRetDecl, _call_res_type );

			// kill this to make debugging easier
			_call_res_type = vt_unknown;
		}
	}
}

void lir_gen_walker::at_goto(gotoNode *the_goto, Order ord){

	if (ord == Postorder)
		return;
	
	if (!_skip_next_goto){
		Matcher::match_goto(_info, the_goto);
	}
	_skip_next_goto = 0;
}

void lir_gen_walker::at_label(labelNode *the_label, Order ord){

	if (ord == Postorder)
		return;

	Matcher::match_label(_info, the_label);
}

void lir_gen_walker::at_if(ifNode *the_if, Order ord){

	if (ord == Postorder)
		return;

	// comparison operands
	Node *pCmp1 = NULL, *pCmp2 = NULL;

	// the comparison operator of interest
	Operator::OperatorID cmpOp = (Operator::OperatorID)0;

	// get the test expression
	exprNode * pIfExpr = the_if->expr();

	// see if the dismantler dismantled something into "var = test; if (var) {}"
	if ( pIfExpr->typ() == Id && _condition_stmt && _condition_stmt->left()->typ() == Id && 
		((idNode*)_condition_stmt->left())->decl() == ((idNode*)pIfExpr)->decl() )
	{
		// actual test is rhs of this binary
		pIfExpr = _condition_stmt->right();
	}

	// load the operands
	if ( pIfExpr->typ() == Binary )
	{
		binaryNode * pIfExprBin = (binaryNode*)pIfExpr;

		// operands are left and right side
		pCmp1 = pIfExprBin->left();
		pCmp2 = pIfExprBin->right();

		// comparison is whatever we have on the binary thing
		assert( pIfExprBin->op()->is_comparison() );
		cmpOp = (Operator::OperatorID)pIfExprBin->op()->id();
	}
	else if ( pIfExpr->typ() == Id )
	{
		// comparand 1 is whatever the id is
		// NOTE: comparison will implicitly be (cmp1 != 0)
		pCmp1 = pIfExpr;
	}
	else
	{
		// we can't handle this
		CBZFAIL(("Don't understand 'if' test expression with type %d", pIfExpr->typ()));
	}

	// match this thing
	Matcher::match_cond_branch(_info, cmpOp, pCmp1, pCmp2, ((gotoNode*)the_if->true_br()->stmts().front())->name());

	// flag goto node so it does not emit a jmp instruction
	_skip_next_goto = 1;
}

void lir_gen_walker::at_call(callNode *the_call, Order ord) 
{
	if (ord == Postorder)
	{
		_ignore_decl = false;
		return;
	}

	// add sequencing instruction
	_info._insts->push_back( new LirInst( mn_CallPre ) );

	// set some flags to help things out
	_is_call = true;
	_ignore_decl = true;

	// get first arguments into registers
	expr_list & args = the_call->args();
	expr_list::const_iterator regArgs = args.begin();
	int nextRegFixed = 0, nextRegFloat = 0;
	Register outReg;
	const arch_info::register_info_list & regsFixed = CBZ::ArchInfo.get_regs_param_fixed();
	const arch_info::register_info_list & regsFloat = CBZ::ArchInfo.get_regs_param_float();
	while ( regArgs != args.end() )
	{
		// what are we allocating?
		lir_var_type vt = LirVt::getVarType( *regArgs );
		if ( vt == vt_float || vt == vt_double )
		{
			// make sure we have another register
			if ( nextRegFloat >= (int)regsFloat.size() )
				break;

			// decide which one we'll use
			outReg = *(regsFloat[nextRegFloat++]);
		}
		else
		{
			// make sure we have another register
			if ( nextRegFixed >= (int)regsFixed.size() )
				break;

			// decide which one we'll use
			outReg = *(regsFixed[nextRegFixed++]);
		}

		// walk this argument to evaluate it
		call_arg_walker argwalk( _info, DATA_CONTENTS_REG_OUT, outReg );
		(*regArgs)->walk( argwalk );
		declNode * pArgDecl = NULL;
		Register argReg;
		argwalk.get_results( pArgDecl, argReg );

		// did we get the result into our desired register?
		if ( argReg.num() != outReg.num() )
		{
			// have to move it into place
			Matcher::match_move( _info, argReg, pArgDecl, outReg, DATA_CONTENTS_REG_OUT );
		}

		// do the next one
		regArgs++;
	}

	// did we do them all?
	if ( regArgs != args.end() )
	{
		// track argument offsets - start at the first offset above any 
		//	reserved extra space.
		int stackOffsetNext = CBZ::ArchInfo.get_stack_extra_bottom();

		// put remaining arguments onto the stack
		expr_list::const_iterator stackArgs = regArgs;
		for ( ; stackArgs != args.end(); ++stackArgs )
		{
			lir_var_type vt = LirVt::getVarType( *stackArgs );

			// align this offset
			unsigned int align = CBZ::ArchInfo.get_data_align( vt );
			stackOffsetNext = ((stackOffsetNext + align - 1) / align) * align;

			// get this arg into a register
			call_arg_walker argwalk( _info, DATA_CONTENTS_REG_OUT, outReg );
			(*stackArgs)->walk( argwalk );
			declNode * pArgDecl = NULL;
			Register argReg;
			argwalk.get_results( pArgDecl, argReg );

			// move it to the right stack location - somewhere above the stack pointer
			_info._insts->push_back( Lir_Class::Store( vt, argReg, pArgDecl, Register::getRegSp(), 
				DATA_CONTENTS_STACKP, stackOffsetNext, NULL ) );

			// adjust for next thing
			stackOffsetNext += CBZ::ArchInfo.get_data_size( vt );
		}

		// record how much space we used (don't add in the extra bottom space - 
		//	this will be added back in later)
		the_call->stack_arg_bytes( stackOffsetNext - CBZ::ArchInfo.get_stack_extra_bottom() );
	}
	else
	{
		// no stack argument bytes for this call
		the_call->stack_arg_bytes( 0 );
	}

	// no longer in a call
	_is_call = false;

	// save the return type (currently only support direct function calls)
	assert( the_call->name()->typ() == Id );
	_call_res_type = LirVt::getVarType( ((funcNode*)(((idNode*)the_call->name())->type()))->type() );

	// do the actual call
	Matcher::match_call( _info, the_call );

	// is it a void function or what?
}

void lir_gen_walker::at_return(returnNode *the_return, Walker::Order ord)
{
	if (ord == Postorder)
		return;

	// do whatever we need to
	Matcher::match_return( _info, the_return );
}

suespec_walker::suespec_walker( Enc_Data & encData ) : 
	Walker( Both, Subtree ),
	_encData( encData )
{
}

void suespec_walker::at_block( blockNode * block, Order ord ) 
{
	if ( ord == Preorder )
		_encData.enter_block( block );
	else
		_encData.leave_block();
}

void suespec_walker::at_suespec( suespecNode * suespec, Order ord )
{
	if ( ord != Preorder )
		return;

	// declare it for now - it can be demand-defined later
	_encData.declare_struct( suespec );
}

ptr_arith_walker::ptr_arith_walker() : Walker( Both, Subtree )
{
	_stmt = NULL;
}

void ptr_arith_walker::at_block(blockNode *block, Order ord) 
{
	// keep track of blocks
	if ( ord == Preorder )
		_blocks.push_back( block );
	else
		_blocks.pop_back();
}

void ptr_arith_walker::at_stmt(stmtNode *stmt, Order ord) 
{
	// keep track of statement
	_stmt = (ord == Preorder) ? stmt : NULL;
}

void ptr_arith_walker::at_binary(binaryNode *the_binary, Order ord)
{
	if ( ord != Preorder )
		return;

	// what operation is it?
	Operator * op = the_binary->op();
	if ( op->id() != '-' && op->id() != '+' )
		return;

	// operand types?
	exprNode * left = the_binary->left();
	exprNode * right = the_binary->right();
	lir_var_type vtl = LirVt::getVarType( left );
	lir_var_type vtr = LirVt::getVarType( right );

	// we care only if one is pointer and the other is integral
	if ( ! ((vtl == vt_pointer && LirVt::isTypeFixed( vtr ) && vtr != vt_pointer) ||
			(vtr == vt_pointer && LirVt::isTypeFixed( vtl ) && vtl != vt_pointer)) )
		return;

	// which do we convert?
	exprNode * ptrNode = NULL;
	exprNode * opndNode = NULL;
	void (binaryNode::*setFn)( exprNode * );
	if ( vtr == vt_pointer )
	{
		// convert left side
		opndNode = left;
		ptrNode = right;
		setFn = &binaryNode::left;
	}
	else
	{
		// convert right side
		opndNode = right;
		ptrNode = left;
		setFn = &binaryNode::right;
	}

	// find out how big the pointed-to type is
	assert( ptrNode->type()->typ() == Ptr );
	lir_var_type vtPointed = LirVt::getVarType( ptrNode->type()->type() );
	unsigned int size = CBZ::ArchInfo.get_data_size( vtPointed );

	// if pointed-to size is 1, we can quit here.
	if ( size == 1 )
		return;

	// is operand a constant?  if so we can just munge it in-place.
	if ( opndNode->typ() == Const )
	{
		// grab the constant
		constNode * cn = (constNode*)opndNode;
		constant c = cn->value();
		assert( c.basic().is_arithmetic() );
		unsigned long value = c.Integer();

		// multiply by size of pointed-to type
		value *= size;

		// put it back into the smallest thing not smaller than old size
		lir_var_type vtNew = LirVt::getVarType( opndNode );
		while ( vtNew <= vt_ulong )
		{
			// figure out how bit this data type is
			char bytes = CBZ::ArchInfo.get_data_size( vtNew );
			assert( bytes <= 4 ); // assuming 32-bit long on compiler machine
			unsigned long highbit = 0x80;
			for ( int i = 1; i < bytes; ++i )
				highbit <<= 8;

			// will the value fit here?
			if ( highbit > value )
				break;
		}

		// use this as the new type
		switch ( vtNew )
		{
		case vt_char:	c.SChar( (char)value ); break;
		case vt_uchar:	c.UChar( (unsigned char)value ); break;
		case vt_short:	c.SShort( (short)value ); break;
		case vt_ushort:	c.UShort( (unsigned short)value ); break;
		case vt_int:	c.SInt( value ); break;
		case vt_uint:	c.UInt( value ); break;
		case vt_long:	c.SLong( value ); break;
		case vt_ulong:	c.ULong( value ); break;
		default:		assert( false ); break;
		}

		// setup new constant
		cn->value( c );
		return;
	}

	// we have to do some math here
	assert( _blocks.size() > 0 );
	blockNode * block = _blocks.back();

	// a new temporary - gets old value * size
	idNode * newVar = CBZ::get_temp_var( block, opndNode->type() );
	constNode * sizeConst = new constNode( constant( size ) );
	exprNode * mult = new binaryNode( '*', opndNode, sizeConst );
	exprNode * asgn = new binaryNode( '=', newVar, mult );
	exprstmtNode * asgnStmt = new exprstmtNode( asgn );

	// put this right before the previous statement
	stmt_list & stmts = block->stmts();
	stmtNode * stmt = _stmt;
	if ( ! stmt )
		stmt = *stmts.begin();
	stmt_list_p it = stmts.begin();
	for ( ; it != stmts.end(); ++it )
		if ( *it == stmt )
		{
			stmts.insert( it, asgnStmt );
			break;
		}

	// fixup old expr to reference the temp and not the old thing
	(the_binary->*setFn)( newVar );
}

call_arg_walker::call_arg_walker( lir_gen_info & info, declNode * temp_var_decl, Register temp_var_reg ) :
	Walker( Preorder, NodeOnly ),
	_info( info )
{
	_arg_decl = temp_var_decl;
	_arg_reg = temp_var_reg;
}

void call_arg_walker::at_expr(exprNode *the_expr, Order)
{
	// load this into a register for later perusal
	_info._alloc->get_temp_var( the_expr->type(), _arg_reg, _arg_decl );
	lir_var_type type;
	Matcher::match_simple_load( _info, the_expr, _arg_reg, _arg_decl, &type );
}

void call_arg_walker::at_unary(unaryNode *the_unary, Order)
{
	// load this into a register for later perusal
	_info._alloc->get_temp_var( the_unary->type(), _arg_reg, _arg_decl );
	lir_var_type type;
	Matcher::match_simple_load( _info, the_unary, _arg_reg, _arg_decl, &type );
}

void call_arg_walker::at_binary(binaryNode *the_binary, Order)
{
	// load this into a register for later perusal
	_info._alloc->get_temp_var( the_binary->type(), _arg_reg, _arg_decl );
	lir_var_type type;
	Matcher::match_simple_load( _info, the_binary, _arg_reg, _arg_decl, &type );
}

void call_arg_walker::get_results( declNode *& arg_decl, Register & arg_reg )
{
	arg_decl = _arg_decl;
	arg_reg = _arg_reg;
}
