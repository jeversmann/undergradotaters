// ------------- 
//  Chuck Tsen
// July 26, 2002
//  Struct_Info.h
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
//
// This picture (set tab to 8 spaces) models the data structure used 
// to keep track of structs.  Each struct is a head node of a linked 
// list, off of which nodes of items are appended.
//
// Item nodes encapsulate useful data about the struct
//
//	*********    *********		*********
//	*STRUCT1* -> *STRUCT2* -> ...	*STRUCTN*
//	*********    *********		*********
//	   |		|		   |
//	-------      -------		-------
//	|Item1|   -> |Item1|   -> ...	|Item1|
//	-------      -------		-------
//	   |		|		   |
//	-------      -------		-------
//	|Item2|   -> |Item2|   -> ...	|Item2|
//	-------      -------		-------
//	    		|		    
//	             -------		       
// 	             |ItemM|                  
//	             -------

#ifndef __STRUCT_INFO__
#define __STRUCT_INFO__

#include "c_breeze.h"
#include "instruction.h"

// structure stuff forward declarations
class Struct_Data;
class Struct_Member;


class Enc_Data{

private:

	struct struct_namespace;

	typedef map<string, Struct_Data*> struct_info_map;
	typedef struct_info_map::iterator struct_info_map_p;

	typedef map<blockNode*, struct_namespace*> block_ns_map;
	typedef block_ns_map::iterator block_ns_map_p;

	struct struct_namespace
	{
		struct_namespace()
		{
			block = NULL;
			outer = NULL;
		}

		// block associated with this namespace
		blockNode * block;

		// enclosing namespace
		struct_namespace * outer;

		// struct info for this namespace
		struct_info_map structs;
	};

	// current innermost namespace
	struct_namespace * _ns_inner;

	// various blocks we've seen and their associated namespaces
	block_ns_map _blocks;

public:

	// ctor/dtor
	Enc_Data();
	~Enc_Data();

	// reset our info
	void init();

	// for tracking of namespaces - enter/leave a block
	void enter_block( blockNode * block );
	void leave_block();

	// find info on a particular structure
	Struct_Data* find_struct(string cmp_name);

	// declare a structure but don't try to define it yet
	void declare_struct(suespecNode *suespec);


protected:

	// find the suespec for a structure and read in its definition, if possible
	void define_struct(Struct_Data* sd);

	// add a member to a structure
	void add_struct_member(Struct_Data& sn, declNode *itemDecl);

};

class Struct_Data{

	friend class Enc_Data;
	
private:

	typedef map<string, Struct_Member*> struct_member_map;
	typedef struct_member_map struct_member_map_p;

	string				_name;			// the name of the struct
	suespecNode			*_suespec;		// suespec for this structure
	struct_member_map	_members;		// our members
	bool				_defining;		// flag set while we are defining this structure to prevent endless loops
	bool				_defined;		// flag set once this thing has been defined
	unsigned int		_nextOffset;	// next offset to be used when adding members
	unsigned int		_size;			// size to allocate at instantiation

public:

	// constructors
	Struct_Data();
	Struct_Data(string s_name);

	// destructor
	~Struct_Data();

	// accessors
	string			name() const { return _name; }
	unsigned int	size() const { return _size; }

	// find one of our members
	Struct_Member*	find_member( string name ) const;

};

class Struct_Member{

	friend class Enc_Data;
	
private:
	
	lir_var_type	_var;			// what type of variable is this?
	string			_name;			// what is it's name?
	unsigned int	_ofst;			// tells the displacement from base for how to find it 


public:
	
	Struct_Member() {	 			// default constructor
		_name = "\0";
		_ofst = 0;
	}		
	
	Struct_Member(lir_var_type vt, string vn, unsigned int l) {		// constructor - assign name and lir_var_type
		_var = vt;
		_ofst = l;
	} 
	
	~Struct_Member()	{				// deconstructor
	}

	// field accessors
	lir_var_type	getVarType()	{return _var;}
	string			getVarName()	{return _name;}
	unsigned int	getOffset()		{return _ofst;}

};

#endif //__STRUCT_INFO__
