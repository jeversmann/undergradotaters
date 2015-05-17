#include <malloc.h>

// Calculate the fibonacci number at num
int fib(int num) {
  // Make an array to memoize
  int *array;
  int resp;
  if (NULL == (array = malloc((num + 1) * sizeof(int)))) {
    return (-1);
  }

  if (num <= 1) {
    resp = 1; // We just know this
    free(array);
  } else {
    array[0] = 1;
    array[1] = 1;
  }

  for (int i = 2; i < num; i++) {
    array[i] = array[i - 1] + array[i - 2];
    resp = array[i]; // Last one will be the answer
  }

  free(array); // Clean up
  return resp;
}

int main() {
  // Fill an array with fib numbers

  int *array;
  if (NULL == (array = malloc(5 * sizeof(int)))) {
    return (-1);
  }

  array[0] = fib(6);
  array[1] = fib(17);
  array[2] = fib(10);
  array[3] = fib(3);
  array[4] = fib(19);

  for (int i = 0; i < 5; i++) {
    printf("%d ", array[i]);
  }

  return 0; // This worked
}
