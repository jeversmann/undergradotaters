SHELL=/bin/sh

# Configuration

CXX     = g++
LD      = $(CXX)
INCLUDES= -I. -I./heaplayers-0.2 -I$(CBZHOME)/include
YACC    = bison
LEX     = flex
AR	= ar
RANLIB	= ranlib
LIBCBZ = $(CBZHOME)/lib/libc-breeze.a

ifeq ($(OPT), 1)
CFLAGS = -O4
endif

ifeq ($(OPT), 3)
CFLAGS = -O4 -fomit-frame-pointer -funroll-loops -DCBZ_HEAPLAYERS
endif

ifeq ($(OPT), 2)
CFLAGS = -O4 -fomit-frame-pointer -funroll-loops
endif

ifeq ($(EXP), 1)
CFLAGS = -DOLD_DOMINATOR_TEST -g
endif

ifeq ($(DEBUG), 1)
CFLAGS = -g 
endif

ifeq ($(DEBUG), 2)
CFLAGS = -g -DCBZ_HEAPLAYERS
endif

ifeq ($(PROF), 1)
CFLAGS = -O2 -pg
endif

ifeq ($(PROF), 2)
CFLAGS = -O4 -pg
endif

ifeq ($(PROF), 3)
CFLAGS = -O2 -pg -DOLD_DOMINATOR_TEST
endif

# Object code to build

OBJ = 	location.o \
	memoryaccess.o \
	memoryblock.o \
	memorymodel.o \
	pointers.o \
	pointervalue.o \
	proceduredb.o \
	worklist.o \
	dfpreds.o \
	constants.o \
	liveness.o \
	precision.o

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

test: $(OBJ) test.cc
	$(CXX) $(INCLUDES) $(CFLAGS) test.cc $(OBJ) -o test -L$(CBZHOME)/lib -lc-breeze

# Implicit rule for compiling all C++ code

.cc.o:
	$(CXX) $(INCLUDES) $(CFLAGS) -c $< -o $@

# rebuild the dependencies.  Note: Ignores standard includes on
# purpose, on the assumption that they won't change in between builds,
# and we want to only record dependencies for our files, so that this
# makefile will work on different platforms that store the standard
# headers in different places.  
depend:
	makedepend -Y -- $(CFLAGS) -- $(OBJ:%.o=%.cc)
# DO NOT DELETE

location.o: location.h allocation.h
memoryaccess.o: memoryaccess.h location.h allocation.h
memoryblock.o: memorymodel.h location.h allocation.h memoryblock.h
memoryblock.o: pointers_common.h memoryaccess.h
memorymodel.o: memorymodel.h location.h allocation.h memoryblock.h
memorymodel.o: pointers_common.h memoryaccess.h
pointers.o: pointers.h location.h allocation.h memorymodel.h memoryblock.h
pointers.o: pointers_common.h memoryaccess.h mergepoints.h proceduredb.h
pointers.o: dfpreds.h worklist.h ipanalysis.h pointervalue.h handle.h
proceduredb.o: proceduredb.h pointers_common.h location.h allocation.h
proceduredb.o: dfpreds.h memoryaccess.h memoryblock.h worklist.h
worklist.o: worklist.h
dfpreds.o: dfpreds.h pointers_common.h
constants.o: constants.h pointers.h location.h allocation.h memorymodel.h
constants.o: memoryblock.h pointers_common.h memoryaccess.h mergepoints.h
constants.o: proceduredb.h dfpreds.h worklist.h ipanalysis.h pointervalue.h
constants.o: handle.h
