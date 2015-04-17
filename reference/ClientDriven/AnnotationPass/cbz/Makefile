include Makefile.inc

SRCLIBS = ast helpers main optimizations pointers contrib

update:
	for i in $(SRCLIBS); do \
	    (cd ./src/$$i; $(MAKE) update; ) \
	done
	# $(RANLIB) $(LIBCBZ)

rebuild:
	\rm -f $(LIBCBZ)
	for i in $(SRCLIBS); do \
	    (cd ./src/$$i; $(MAKE) rebuild; ) \
	done
	$(RANLIB) $(LIBCBZ)

headers:
	rm -f ./include/*.h
	for i in $(SRCLIBS); do \
	    (cd ./include; ./make_headers ../src/$$i) \
	done

depend:
	for i in $(SRCLIBS); do \
	    (cd ./src/$$i; $(MAKE) depend; ) \
	done

clean: 
	for i in $(SRCLIBS); do \
	    (cd ./src/$$i; $(MAKE) clean; ) \
	done
	\rm -f *.o *.a *~ *.tab.*
	(cd include; \rm -f *.h *~ )

clobber: 
	\rm -f $(LIBCBZ)
	for i in $(SRCLIBS); do \
	    (cd ./src/$$i; $(MAKE) clean; ) \
	done
	\rm -f *.o *.a *~ *.tab.* cbz
	(cd include; \rm -f *.h *~ )

cbz: $(LIBCBZ) $(EXTRA_OBJS)
	$(CXX) -o cbz $(EXTRA_OBJS) -L./lib -lc-breeze

