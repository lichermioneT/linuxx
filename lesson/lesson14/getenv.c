#include <stdio.h>
#include <stdlib.h>

int main()
{

  char* msg = NULL;
  msg = getenv("USER");

  printf("%s \n", msg);


}
