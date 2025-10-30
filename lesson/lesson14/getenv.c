#include <stdio.h>
#include <stdlib.h>

int main()
{
  printf("%s \n", getenv("HOME"));
  printf("%s \n", getenv("USER"));
  printf("%s \n", getenv("PWD"));
  printf("%s \n", getenv("PATH"));
}
