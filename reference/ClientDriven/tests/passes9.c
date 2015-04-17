//simplest flow sensitivity test
//take a pointer, and change what it points to

#include <stdio.h>

int main() {
  int x = 3;
  int y = 4;
  int *ptr;
  ptr = &x;
  *ptr = 1;
  ptr = &y;
  *ptr = 2;

}
