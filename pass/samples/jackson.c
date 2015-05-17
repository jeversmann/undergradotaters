int dance(int* name) {
  int bad = 5;
  int* terri = &bad;
  int* only = name;
  return (*terri**only);
}

int main() {
  int thriller = 4;
  int* name = &thriller;
  int mj = dance(name);
  int* ptr = &mj;
  return *ptr;
}

