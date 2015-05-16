CXXFLAGS = $(shell llvm-config --cxxflags) -fno-rtti
CXX = g++

all: dce-pass.so f-meta.so broadway-pass.so

DCEPass.o: DCEPass.cpp DCEPass.h DataFlowAnnotator.h Makefile DataFlow.h Utils.h
	$(CXX) $(CXXFLAGS) DCEPass.cpp -c -o $@

dce-pass.so: DCEPass.o
	$(CXX) -shared $^ -o $@

BroadwayPass.o: BroadwayPass.cpp BroadwayPass.h DataFlowAnnotator.h Makefile DataFlow.h Utils.h
	$(CXX) $(CXXFLAGS) BroadwayPass.cpp -c -o $@

broadway-pass.so: BroadwayPass.o
	$(CXX) -shared $^ -o $@

dce: dce-pass.so
	opt -load ./dce-pass.so -dce-pass licm_out.bc -o dce_out.bc

broadway: broadway-pass.so
	opt -load ./broadway-pass.so -broadway-pass licm_out.bc -o broadway_out.bc

FMetaPass.o: FMetaPass.cpp FMetaPass.h Makefile DataFlow.h Utils.h
	$(CXX) $(CXXFLAGS) FMetaPass.cpp -c -o $@

f-meta.so: FMetaPass.o
	$(CXX) -shared $^ -o $@

# Since run is taken
f: f-meta.so
	opt -load ./f-meta.so  -fmeta-pass llvm/input/double_free.bc -o llvm/output/double_free.bc

clean:
	rm -f *.o *~ *.so

# If the first argument is "run"...
ifeq (run,$(firstword $(MAKECMDGOALS)))
  # use the rest as arguments for "run"
  RUN_ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
  # ...and turn them into do-nothing targets
  $(eval $(RUN_ARGS):;@:)
endif

.PHONY: run
run : broadway-pass.so
	opt -load ./broadway-pass.so -broadway-pass $(RUN_ARGS) -o run_out.bc
