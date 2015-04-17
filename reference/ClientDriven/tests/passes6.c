#include <stdlib.h>
#include <stdio.h>

struct obj2 {
  int field1;
  int field2;
};

struct obj {
  int field1;
  char field2;
  int* field3;
  struct obj2 field4;
};

int main () {
  int numbers[5];
  numbers[0] = 111;
  numbers[1] = 222;
  numbers[2] = 333;

  struct obj an_obj;
  an_obj.field2 = 123;
  an_obj.field1 = 42;
  an_obj.field3 = numbers;

  an_obj.field3[3] = 444;
  an_obj.field4.field1 = 666;

  printf("%d %d %d\n", an_obj.field1, an_obj.field2, an_obj.field3[0]);

  return 0;
}
