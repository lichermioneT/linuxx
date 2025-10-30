#include <stdio.h>


int main(int agrc, char* argv[], char* env[])
{
  for(int i = 0; env[i] != NULL; i++)
  {
    printf("%d : %s \n", i, env[i]);
  }

  return 0;
}


