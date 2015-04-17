#include <stdio.h>

// Makes sure assigning to int* through int** works. Regression test.
int main() {
  int x = 3;
  int* px = &x;
  int** pp = &px;
  *pp = px;
}
