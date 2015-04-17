#include <stdlib.h>
#include <stdio.h>

// TODO test all the stuff mentioned on
// https://bitbucket.org/aluong/broadway/wiki/edit/Timeline
// TODO mark defs and uses everywhere

struct obj {
  int field1;
  char field2;
  int* field3;
};

// TODO figure out swap
/*int* swap (int **meta1, int **meta2) {
  int **tmp = meta1;
  meta1 = meta2;
  meta2 = tmp;
  return *meta1;
}*/

void set_without_return(int ***meta, int *ptr) {
  *meta = &ptr;
}

int* identity (int* ptr) {
  int **unnecessary = &ptr;
  return *unnecessary;
}

int main () {
  int data1 = 13, data2 = 42;
  int *ptr1, *ptr2, *ptr3;
  int **meta_ptr;

  // ptr1 -> { data1 }
  ptr1 = &data1;
  // ptr2 -> { data1 } (we shouldn't make another pass over this and have ptr2
  // get data2)
  ptr2 = ptr1;
  // ptr1 -> { data1, data2 }
  ptr1 = &data2;

  for (int i = 0; i < *ptr3; i++) {
    // ptr2 -> { data1, malloc at call-site blah }
    ptr2 = malloc(sizeof(int));
  }

  // TODO do we care about this? *ptr2 is either data1 or malloced stuff
  scanf("%d", ptr2);

  // TODO use some of the functions here, in the if or not. id, set without
  // return, etc

  if (*ptr2 > 5) {
    // meta_ptr -> { ptr2 }
    meta_ptr = &ptr2;
    // TODO
    // the result should be ptr3 -> { ...
    ptr3 = swap(meta_ptr, &ptr1);
    data1 = 17;
  } else {
    meta_ptr = &ptr1;
    ptr3 = swap(meta_ptr, &ptr2);
    data1 = 18; // force a phi!
  }

  struct obj* an_obj = malloc(sizeof(struct obj));
  an_obj->field2 = 'l';
  an_obj->field1 = 42;
  an_obj->field3 = ptr3;

  an_obj->field3[4] = 666;

  printf("%d %d %d %d %d %d %d\n", data1, data2, *ptr1, *ptr2, *ptr3, **meta_ptr, *(an_obj->field3));

  return 0;
}
