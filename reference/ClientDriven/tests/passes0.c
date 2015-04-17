// simplest possible test: point to something, then dereference it.

int main () {
  int data, *ptr;

  data = 111;
  ptr = &data;
  *ptr = 222;

  // this does nothing; it's equivalent to 'x = x'
  // but it does two levels of deref... to get the value.
  data = *ptr;

  return 0;
}
