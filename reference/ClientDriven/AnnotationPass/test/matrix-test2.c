void createZeroMatrix(int * data, int w, int h);

void createIdentityMatrix(int * data, int w, int h);

void createDenseMatrix(int * data, int w, int h);

void copyMatrix(int * srcData, int * destData);

void multMatrices(int * m1, int * m2, int * result);

void invertMatrix(int * m);

int main() {
  
  int block1[100];
  int block2[100];
  int block3[100];
  
  int * m1 = block1;
  int * m2 = block2;
  int * m3 = block3;

  createIdentityMatrix(m1, 10, 10);
  createZeroMatrix(m2, 10, 10);
  createZeroMatrix(m3, 10, 10);
  
  int x = 5;
  
  if (x < 2) m2 = m1;
  
  copyMatrix(m2, m3);

  invertMatrix(m3);
}