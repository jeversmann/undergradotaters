/*
 *  Tainted Analysis: 
 * 	FI: Tainted: block1, block3
 * 	1 error
 */

int read(char* buffer_ptr, int size);

char* strcpy(char* src, const char* dst);

void syslog(char* msg);

int main()
{ 
  char block1[255];
  char block2[255];
  char block3[255];
  
  char* logMessage1;
  char* logMessage2;
  char* fileContents;
  
  fileContents = block1;
  
  read(fileContents, 255);
  
  logMessage1 = block2;
  
  syslog(logMessage1);
  
  logMessage2 = block3;
  
  strcpy(fileContents, logMessage2);
  
  syslog(logMessage2);
}
