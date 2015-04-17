/*
 *  Tainted Analysis: 
 * 	FI: Tainted: block1, block2, block3
 * 	1 error
 */

int read(char* buffer_ptr, int size);

char* strcpy(char* src, const char* dst);

void syslog(char* msg);

char * foo(char* a, char* b) 
{
  if (*a == 'a') return a;
  else return b;
}

int main()
{ 
  char block1[255];
  char block2[255];
  char block3[255];
  char block4[255];
  
  char* logMessage1;
  char* logMessage2;
  char* fileContents;
  
  fileContents = block1;
  read(fileContents, 255);
  
  logMessage1 = foo(block2, fileContents);
  logMessage2 = foo(block2, block3);
  
  syslog(logMessage2);
  
  strcpy(logMessage1, logMessage2);
  
  syslog(logMessage2);
}


  