CXXFLAGS = $(shell llvm-config --cxxflags) -fno-rtti

all: constid.so live.so reach.so licm-pass.so dce-pass.so

DCEPass.o: DCEPass.cpp DCEPass.h DataFlowAnnotator.h Makefile DataFlow.h Utils.h
	$(CXX) $(CXXFLAGS) DCEPass.cpp -c -o $@

dce-pass.so: DCEPass.o
	$(CXX) -shared $^ -o $@

clean:
	rm -f *.o *~ *.so

input.bc: input.c Makefile
	clang -emit-llvm -c input.c
	opt -mem2reg -instnamer input.bc -o input.bc

dce: dce-pass.so test.bc
	opt -load ./dce-pass.so -dce-pass licm_out.bc -o dce_out.bc

# If the first argument is "run"...
ifeq (run,$(firstword $(MAKECMDGOALS)))
  # use the rest as arguments for "run"
  RUN_ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
  # ...and turn them into do-nothing targets
  $(eval $(RUN_ARGS):;@:)
endif

.PHONY: run
run : licm-pass.so dce-pass.so
	opt -load ./licm-pass.so -load ./dce-pass.so -loop-simplify -licm-pass -dce-pass $(RUN_ARGS) -o run_out.bc
