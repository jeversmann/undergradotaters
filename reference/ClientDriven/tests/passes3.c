// test a pointer copy (and a phi, why not)

int main () {
  int data1 = 1;
  int data2 = 2;
  int *ptr = data1 == 3 ? &data1 : &data2;
  *ptr = 3;
  int *ptr_copy = ptr;
  *ptr_copy = 4;
}
