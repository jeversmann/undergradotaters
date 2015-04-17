void g(int* p, int x) {
  *p = x;
}

void f(int* p) {
  *p = 7;
  g(p, 123);
}

int main() {
  int x = 100;
  f(&x);
  g(&x, 500);
  return 0;
}
