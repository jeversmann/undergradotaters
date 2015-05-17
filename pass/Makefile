CXXFLAGS = $(shell llvm-config --cxxflags) -fno-rtti
CXX = g++ 

all: f-meta.so

FMetaPass.o: FMetaPass.cpp FMetaPass.h Makefile
	$(CXX) $(CXXFLAGS) FMetaPass.cpp -c -o $@

f-meta.so: FMetaPass.o
	$(CXX) -shared $^ -o $@
clean:
	rm -f *.o *~ *.so
run: f-meta.so
	opt -load ./f-meta.so  -fmeta-pass llvm/input/double_free.bc -o llvm/output/double_free.bc

basic-alias: 
	opt -basicaa llvm/input/double_free.bc -o llvm/output/double_free.bc

print-basic-alias:	
	opt -basicaa -print-alias-sets llvm/input/double_free.bc -o llvm/output/double_free.bc

AAPass.so: AAPass.o
	$(CXX) -shared $^ -o $@

AAPass.o: AAPass.cpp AAPass.h Makefile
	$(CXX) $(CXXFLAGS) AAPass.cpp -c -o $@

experimental-alias: AAPass.so
	opt -load ./AAPass.so -basicaa -myaa llvm/input/double_free.bc -o llvm/output/double_free.bc
print-experimental: AAPass.so
	opt -load ./AAPass.so -basicaa -myaa -print-alias-sets llvm/input/double_free.bc -o llvm/output/double_free.bc
