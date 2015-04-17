void createZeroMatrix(int * data, int w, int h);

void createIdentityMatrix(int * data, int w, int h);

void createDenseMatrix(int * data, int w, int h);

void copyMatrix(int * srcData, int * destData);

void multMatrices(int * m1, int * m2, int * result);

void invertMatrix(int * m);

void foo(int x, int* m) {
  
  if (x == 0) createZeroMatrix(m, 10, 10);
  if (x == 1) createIdentityMatrix(m, 10, 10);
}

int* bar(int* m1, int* m2) {
  
  if (m1[0] == 0) {
    invertMatrix(m1);
    return m1;
  } else return m2;
}

int main() {
  
  int block1[100];
  int block2[100];
  int block3[100];
  int block4[100];
  int block5[100];
  
  int * m1 = block1;
  int * m2 = block2;
  int * m3 = block3;
  int * m4 = block4;
  int * m5 = block5;
  int * m6;

  foo(1, m1);
  createZeroMatrix(m2, 10, 10);
  
  multMatrices(m1, m2, m3);
  
  invertMatrix(m3);
  
  createDenseMatrix(m4, 10, 10);
  
  m6 = bar(m4, m3);
  
  multMatrices(m6, m2, m5);
  
  copyMatrix(m6, m3);

  invertMatrix(m3);
}