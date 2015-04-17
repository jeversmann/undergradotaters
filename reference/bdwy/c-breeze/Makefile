SHELL=/bin/sh

# Configuration

CXX     = /usr/local/bin/g++
LD      = $(CXX)
INCLUDES= -I. -I$(CBZHOME)/include

ifeq ($(OPT), 1)
CFLAGS = -O4
endif

ifeq ($(OPT), 2)
CFLAGS = -O4 -fomit-frame-pointer -funroll-loops
endif

ifeq ($(DEBUG), 1)
CFLAGS = -g
endif

ifeq ($(PROF), 1)
CFLAGS = -O0 -fno-default-inline -fno-inline -pg
endif

ifeq ($(PROF), 2)
CFLAGS = -O4 -pg
endif


YACC    = bison
LEX     = flex
AR	= ar
RANLIB	= ranlib
LIBCBZ = $(CBZHOME)/lib/libc-breeze.a

export CFLAGS

SRCLIBS = ast backend helpers main optimizations pointers contrib

update:
	for i in $(SRCLIBS); do \
	    (cd $(CBZHOME)/src/$$i; $(MAKE) update; ) \
	done
	$(RANLIB) $(LIBCBZ)

rebuild:
	\rm -f $(LIBCBZ)
	for i in $(SRCLIBS); do \
	    (cd $(CBZHOME)/src/$$i; $(MAKE) rebuild; ) \
	done
	$(RANLIB) $(LIBCBZ)

headers:
	rm -f ./include/*.h
	for i in $(SRCLIBS); do \
	    (cd ./include; ./make_headers $(CBZHOME)/src/$$i) \
	done

clean: 
	\rm -f $(LIBCBZ)
	for i in $(SRCLIBS); do \
	    (cd $(CBZHOME)/src/$$i; $(MAKE) clean; ) \
	done
	\rm -f *.o *.a *~ *.tab.*
	(cd include; \rm -f *.h *~ )

cbz:
	g++ -o cbz -L./lib -lc-breeze
