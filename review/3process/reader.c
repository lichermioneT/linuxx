#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main()
{
  printf("%s\n", getenv("MY_NAME"));
  printf("%s\n", getenv("MY_LEVEL"));

  printf("pid:%d, ppid:%d\n", getpid(), getppid());
  return 0;
}
