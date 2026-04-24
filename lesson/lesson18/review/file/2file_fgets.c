#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define FILE_NAME "log.txt"

int main()
{
  FILE* fp  = fopen(FILE_NAME, "r");
  if(fp == NULL)
  {
    perror("fopen");
    return 1;
  }

  char buffer[64] = {0};
  while(fgets(buffer, sizeof(buffer) -1 , fp) != NULL)
  {
    buffer[strlen(buffer) - 1] = 0; 
    puts(buffer);
  }
  
  fclose(fp);
  return 0;
}
