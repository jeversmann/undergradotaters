// make a call. return something and also have side-effects.

int* identity (int* in, int unused) {
  int *out = in;
  unused = 666; // shouldn't do anything
  *out = 444;
  return out;
}

int main () {
  int data = 111;
  int *ptr = &data;

  // '1 level' of deref... so we do data = 222
  *ptr = 222;

  // '2 levels' of deref... but we need to mangle it to 1, when the expr is a
  // rhs? I can't decide. Expanding the points-to sets of a normal int (data in
  // this case) is a no-op anyway...
  int copy_of_data = *ptr;

  int *clone_ptr;
  // '1 level' of deref for ptr... but actually we just want 'ptr' immediately,
  // aka, the alloca.
  clone_ptr = identity(ptr, data);

  *clone_ptr = 333;
}
