CXXFLAGS = $(shell llvm-config --cxxflags) -fno-rtti
CXX = g++

all: dce-pass.so broadway-pass.so

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
