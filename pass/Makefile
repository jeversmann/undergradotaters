CXXFLAGS = $(shell llvm-config --cxxflags) -fno-rtti
CXX = g++

all: broadway-pass.so

BroadwayPass.o: BroadwayPass.cpp BroadwayPass.h DataFlowAnnotator.h Makefile DataFlow.h Utils.h BroadwayAST.h BroadwayAST.cpp
	$(CXX) $(CXXFLAGS) BroadwayPass.cpp -c -o $@

BroadwayAST.o: BroadwayAST.cpp BroadwayAST.h Makefile Utils.h 
	$(CXX) $(CXXFLAGS) BroadwayAST.cpp -c -o $@

broadway-pass.so: BroadwayPass.o BroadwayAST.o
	$(CXX) -shared $^ -o $@

broadway: broadway-pass.so
	opt -load ./broadway-pass.so -broadway-pass llvm/input/double_free.bc -o broadway_out.bc

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
