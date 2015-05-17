int* dance(int* name) {
  int bad = 5;
  int* terri = &bad;
  return *terri;
}

int main() {
  int thriller = 4;
  int* name = &thriller;
  int* mj = dance(name);
  return *mj;
}

