#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

extern char** environ;

int main(int argc, char* argv[], char* envp[])
{
  printf("argc = %d\n", argc);
  for(int i = 0; i < argc; ++i)
  {
    printf("argv[%d] = %s\n", i, argv[i]);
  }

#if 1
  for(int i = 0; i < 10 && envp[i]; ++i)
  {
    printf("%s\n", envp[i]);
  }
#else 
 for(int i = 0; i < 10 && environ[i]; ++i)
 {
   printf("%s\n", environ[i]);
 }

#endif
  return 0;
}
