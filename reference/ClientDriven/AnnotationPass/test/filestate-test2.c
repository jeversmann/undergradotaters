void openFile(int* fd, char* path);

void closeFile(int* fd);

void readFile(int* fd, char* buff, int size);

int main() {

  int fd1;
  int fd2;
  int* fdp = &fd1;
  char buff[255];

  openFile(fdp, "test");

  readFile(fdp, buff, 255);
  
  fdp = &fd2;
  
  readFile(fdp, buff, 255);

}