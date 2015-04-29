#include "simple.h"

void simple(int arg) {
  int b = arg + 4;
  simple(b);
}
