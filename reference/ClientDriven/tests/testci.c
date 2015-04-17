// differentiate CS and CI

int* identity (int* in, int unused) {
  int *out = in;
  unused = 666; // shouldn't do anything
  *out = 333;
  return out;
}

int main () {
  int data1 = 111;
  int data2 = 222;
  int *ptr1 = &data1;
  int *ptr2 = &data2;

  int *clone1;
  int *clone2;

  clone1 = identity(ptr1, data1);
  clone2 = identity(ptr2, data2);

  // just change data2
  *clone2 = 444;
}
