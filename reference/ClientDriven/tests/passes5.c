#include <stdio.h>

// nest pointer-dereferences enthusiastically!

int main () {
  int data = 13;

  int *ptr1 = &data;
  int **ptr2 = &ptr1;
  int ***ptr3 = &ptr2;
  int ****ptr4 = &ptr3;

  printf("what is it? %d\n", ****ptr4);

  ****ptr4 = 22;

  printf("and now %d\n", data);
}
