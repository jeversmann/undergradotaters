
include ../../Makefile.inc

# Object code to build

OBJ = 	arraynode.o \
	ast.o \
	attribnode.o \
	basic_type.o \
	basicblocknode.o \
	binarynode.o \
	blocknode.o \
	breaknode.o \
	callnode.o \
	casenode.o \
	castnode.o \
	commanode.o \
	conditiongotonode.o \
	constant.o \
	constnode.o \
	continuenode.o \
	coord.o \
	declnode.o \
	defnode.o \
	donode.o \
	enumnode.o \
	exprnode.o \
	exprstmtnode.o \
	fornode.o \
	funcnode.o \
	gotonode.o \
	idnode.o \
	ifnode.o \
	indexnode.o \
	initializernode.o \
	jumpnode.o \
	labelnode.o \
	loopnode.o \
	node.o \
	operandnode.o \
	operators.o \
	primnode.o \
	procnode.o \
	ptrnode.o \
	returnnode.o \
	selectionnode.o \
	stmtnode.o \
	structnode.o \
	subdeclnode.o \
	suenode.o \
	suespecnode.o \
	switchnode.o \
	targetnode.o \
	tdefnode.o \
	ternarynode.o \
	textnode.o \
	threeaddrnode.o \
	typenode.o \
	unarynode.o \
	unionnode.o \
	unitnode.o \
	whilenode.o

# Make targets

update: $(LIBCBZ)

$(LIBCBZ): $(OBJ)
	$(AR) r $@ $?
	$(RANLIB) $@

rebuild: $(OBJ)
	$(AR) q $(LIBCBZ) $?

objects: $(OBJ)

clean:
	rm -f *.o *.a *~ *.tab.*
	rm -f .depend

# Implicit rule for compiling all C++ code

.cc.o:
	$(CXX) $(INCLUDES) $(CFLAGS) -c $< -o $@

# Additional file for dependencies
ifeq (.depend, $(wildcard .depend))
include .depend
endif

# rebuild the dependencies.  Note: Ignores standard includes on
# purpose, on the assumption that they won't change in between builds,
# and we want to only record dependencies for our files, so that this
# makefile will work on different platforms that store the standard
# headers in different places.  
depend:
	touch .depend
	$(MAKEDEP) -f .depend $(INCLUDES) -- $(CFLAGS) -- $(OBJ:%.o=%.cc)
