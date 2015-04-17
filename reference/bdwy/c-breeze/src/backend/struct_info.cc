// ------------- 
//  Chuck Tsen
// July 26, 2002
//  Struct_Info.cc
// -------------
//
// ********************************************************
// *							  *
// * This class will keep track of info in different	  *
// * structs for the code generator to use when		  *
// * determining how to use structs.  It could be 	  *
// * generalized to do parameters as well.		  *
// * described in the paper "Code-Reuse in an Optimizing  *
// * compiler."  					  *
// *							  *
// ********************************************************

// This picture (set tab to 8 spaces) models the data structure used 
// to keep track of structs.  Each struct is a head node of a linked 
// list, off of which nodes of items are appended.
//
// Struct_Member nodes encapsulate useful data about the struct
//
//	*********    *********		*********
//	*STRUCT1* -> *STRUCT2* -> .. -> *STRUCTN*
//	*********    *********		*********
//	   |		|		   |
//	-------      -------		-------
//	|item1|      |item1|		|item1|
//	-------      -------		-------
//	   |		|		   |
//	-------      -------		-------
//	|item2|	     |item2|		|item2|
//	-------      -------		-------
//	    		|		    
//	             -------	       
// 	             |itemM|
//	             -------

#include "c_breeze.h"
#include "struct_info.h"

Enc_Data::Enc_Data(){
	_ns_inner = NULL;
}

Enc_Data::~Enc_Data(){
}

void Enc_Data::init() {

	// clear all known blocks and make new outermost namespace
	_blocks.clear();
	delete _ns_inner;
	_ns_inner = new struct_namespace;
}

void Enc_Data::enter_block( blockNode * block ) {

	// have we seen this before?
	struct_namespace * found = NULL;
	block_ns_map_p old = _blocks.find( block );
	if ( old != _blocks.end() )
		found = old->second;

	// maybe setup new data for this guy
	if ( ! found )
	{
		found = new struct_namespace;
		found->block = block;
		found->outer = _ns_inner;
		_blocks[ block ] = found;
	}

	// this guy is now the innermost
	_ns_inner = found;
}

void Enc_Data::leave_block() {

	assert( _ns_inner && _ns_inner->outer );

	// leave innermost scope, exposing enclosing scope
	_ns_inner = _ns_inner->outer;
}

Struct_Data* Enc_Data::find_struct( string name ) {

	// traverse scopes from inner to outer
	struct_namespace * ns = _ns_inner;
	for ( ; ns != NULL; ns = ns->outer )
	{
		struct_info_map & structs = ns->structs;

		// try to find it
		struct_info_map_p found = structs.find( name );
		if ( found == structs.end() )
			continue;

		// see if it's defined
		Struct_Data* data = found->second;
		if ( ! data->_defined )
			define_struct( data );
		return data;
	}

	return NULL;
}

void Enc_Data::declare_struct(suespecNode *suespec)  {

	assert( _ns_inner );

	// make a new entry for this thing and put it in our map
	string & name = suespec->name();
	Struct_Data * sn = new Struct_Data( name );
	sn->_suespec = suespec;
	_ns_inner->structs[ name ] = sn;
}

void Enc_Data::define_struct(Struct_Data* sn) {

	// if we are already defining it then we are hosed - this thing has itself as a member
	if ( sn->_defining )
		CBZFAIL(("Can't define structure '%s' because it has itself as a member.", sn->name().c_str()));

	// we are busy defining this structure
	sn->_defining = true;

	// append items to structNode until exhaustion
	decl_list & fields = sn->_suespec->fields();
	decl_list::iterator it = fields.begin();
	for ( ; it != fields.end(); ++it )
		add_struct_member( *sn, *it );

	// we are finished defining the structure
	sn->_defining = false;
	sn->_defined = true;
}

// add item to bottom of itemlist of the current struct
void Enc_Data::add_struct_member(Struct_Data& sn, declNode *itemDecl) {

	// check for duplicates
	string name = itemDecl->name();
	if ( sn.find_member( name ) )
		CBZFAIL(("Structure '%s' already has a member named '%s'.", sn._name.c_str(), name.c_str()));

	// get the var type of the thing we're adding
	unsigned int size = 0;
	lir_var_type vt = LirVt::getVarType( itemDecl );
	switch (vt)
	{
	case vt_char:
	case vt_uchar:
		size = CBZ::ArchInfo.get_data_align_char();
		break;

	case vt_short:		
	case vt_ushort:		
		size = CBZ::ArchInfo.get_data_align_short();
		break;

	case vt_int:		
	case vt_uint:
		size = CBZ::ArchInfo.get_data_align_int();
		break;

	case vt_long:		
	case vt_ulong:		
		size = CBZ::ArchInfo.get_data_align_long();
		break;
	
	case vt_float:		
		size = CBZ::ArchInfo.get_data_align_float();
		break;

	case vt_double:		
		size = CBZ::ArchInfo.get_data_align_double();
		break;

	case vt_pointer:
		size = CBZ::ArchInfo.get_data_align_ptr();
		break;

	// if we find a struct field within a struct, C demands that it must be 
	// defined previously.  This makes our job easier in that we don't
	// have to keep a symbol table and only 1 pass is required.
	case vt_aggr:		
	{
		// get struct name
		assert( itemDecl->type()->typ() == Struct || itemDecl->type()->typ() == Union );
		string name = ((sueNode *)itemDecl->type())->spec()->name();

		// find this thing, and make sure it's defined
		Struct_Data* inner = find_struct( name );

		// update the struct size in multiples of max data align
		size = ((inner->_size / CBZ::ArchInfo.get_max_data_align()) + 1) * CBZ::ArchInfo.get_max_data_align();
		break;
	}

	default:	
		cout << "unknown data type passed to Enc_Data::add_struct_member()\n";
		break;
	}

	// create new itemNode
	Struct_Member *new_member = new Struct_Member(vt, name, sn._nextOffset);

	// update structure
	sn._nextOffset += size;
	sn._size += size;

	// add to list of members
	sn._members[ name ] = new_member;
}

// constructor - initialize all fields of struct_info
Struct_Data::Struct_Data(string s_name){

	_name 		= s_name;
	_suespec	= NULL;
	_defining	= false;
	_defined	= false;
	_size		= 0;
	_nextOffset	= 0;
}

Struct_Member* Struct_Data::find_member( string name ) const {

	struct_member_map::const_iterator found = _members.find( name );
	if ( found != _members.end() )
		return found->second;
	return NULL;
}


