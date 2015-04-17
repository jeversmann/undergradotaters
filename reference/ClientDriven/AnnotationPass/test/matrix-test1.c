void createZeroMatrix(int * data, int w, int h);

void createIdentityMatrix(int * data, int w, int h);

void createDenseMatrix(int * data, int w, int h);

void copyMatrix(int * srcData, int * destData);

void multMatrices(int * m1, int * m2, int * result);

void invertMatrix(int * m);

int main() {
  
  int block1[100];
  
  int * m = block1;

  createZeroMatrix(m, 10, 10);

  invertMatrix(m);
}