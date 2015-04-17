#include <stdio.h>

void foo(int** pp, int*p) {
  **pp = 6;
  *pp = p;
}

int main() {
  int x = 3;
  int y = 4;

  int* px = &x;
  int* py = &y;

  int** pp = &px;
  // px should now point to y, with x now being 6
  foo(pp, py);

  // Should assign to y
  *px = 5;

  int* pwhat = *pp;

  // Prints (6,5)
  printf("Resulting (x,y): (%d,%d)\n", x, y);
}
