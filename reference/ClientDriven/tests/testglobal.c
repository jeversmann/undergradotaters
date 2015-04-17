int global1 = 5;
int global2 = 10;

int main() {
  int x = 32;
  int* p = &global1;
  *p = x;
  p = &x;
  *p = global2;
  global2 = 500;
}
// x should have a def at x=32, *p = global2
// global1 should have a def at decl, and at *p = x
// global2 should have a def at decl, and at global2 = 500
