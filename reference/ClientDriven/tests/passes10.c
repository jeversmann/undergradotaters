//flow-insensitivity with phi merge
#include <stdio.h>

int main() {
  int x = 3;
  int y = 4;
  int a = 5;
  int b = 6;

  int *ptr;
  ptr = &x;
  *ptr = 1;
  ptr = &y;
  *ptr = -1;

  if (*ptr < 0) {
    ptr = &a;
  } else {
    ptr = &b;
  }

  printf("%d\n", *ptr);
}

