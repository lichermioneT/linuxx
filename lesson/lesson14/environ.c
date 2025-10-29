#include <stdio.h>

extern char** environ;


int main()
{

  for(int i = 0; **environ; environ++, i++)
  {
    printf("%d : %s \n", i, *environ);
  }



  return 0;
}
