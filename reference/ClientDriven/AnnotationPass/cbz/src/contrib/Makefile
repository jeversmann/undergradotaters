
include ../../Makefile.inc

# Object code to build

OBJ = 	meta.o \
        nodeinfo.o \
        vcg.o \
        vcgast.o \
        vcgCCG.o

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
