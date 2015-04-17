
include ../../Makefile.inc

# Object code to build

OBJ = 	callgraph.o \
	cbz_util.o \
	dismantle-control.o \
	dismantle-expr.o \
	dismantle-flatten.o \
	dismantle-init.o \
	dismantle-label.o \
	dismantle-static.o \
	dismantle.o \
	enum_value_walker.o \
	goto_label_walker.o \
	id_lookup_walker.o \
	inliner.o \
	linker.o \
	name_mangle_walker.o \
	ref_clone_changer.o \
	scope_walker.o \
	semcheck.o \
	set_container_walker.o \
	symbol.o \
	tree_checker.o

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
