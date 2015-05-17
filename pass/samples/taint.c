int recv(int socket);

int exec(int command);

int main() {
  int input = recv(15);
  int file = 0;
  if (input == 11) {
    file = recv(11);
  } else {
    file = 13;
  }
  return exec(file);
}
