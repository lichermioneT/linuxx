#include <stdlib.h>
#include <stdio.h>

int main()
{
  printf("%s\n", getenv("USER"));
  printf("%s\n", getenv("HOME"));
  printf("%s\n", getenv("PATH"));
  printf("%s\n", getenv("LOGNAME"));
  printf("%s\n", getenv("PWD"));

  return 0;
}
