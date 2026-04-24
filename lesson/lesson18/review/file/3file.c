#include <stdio.h>
#include <unistd.h>

#define FILE_NAME "log.txt"

int main()
{
  FILE* fp  = fopen(FILE_NAME, "a");
  if(fp == NULL)
  {
    perror("fopen");
    return 1;
  }

  int cnt = 5;
  while(cnt)
  {
    fprintf(fp, "%s:%d\n", "hello lic", cnt--);
  }
  
  fclose(fp);
  return 0;
}
