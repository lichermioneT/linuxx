#include <stdio.h>

int main()
{

  FILE* fp = fopen("test.txt", "r");
  
  int c = 0;
  //while( (c = fgetc(fp)) !=  EOF)
  while( (c = fgetc(stdin)) !=  EOF)
  {
    fputc(c, stdout);
  }
  







  return 0;
}
