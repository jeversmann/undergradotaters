CXXFLAGS = $(shell llvm-config --cxxflags) -fno-rtti
CXX = clang -fcolor-diagnostics

all: f-meta.so

FMetaPass.o: FMetaPass.cpp FMetaPass.h Makefile
	$(CXX) $(CXXFLAGS) FMetaPass.cpp -c -o $@

f-meta.so: FMetaPass.o
	$(CXX) -shared $^ -o $@
clean:
	rm -f *.o *~ *.so
