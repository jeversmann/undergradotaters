// force a weak update.

#include "stdio.h"

int main () {
  int data1, data2, *ptr;
  data1 = 111;
  data2 = 222;

  int in;
  scanf("%d", in);

  if (in > 0) {
    ptr = &data1;
    *ptr = 333;
  } else {
    ptr = &data2;
    *ptr = 444;
  }

  *ptr = 555;

  return 0;
}
