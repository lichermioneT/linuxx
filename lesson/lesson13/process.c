#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
  pid_t id = fork(); 
  if(id < 0)
  {
    perror("fork");
  }
  else if(id == 0)
  {
    while(1)
    {
      printf("子进程\n");
    }
  }
  else 
  {
    sleep(4);
    printf("父进程\n");
    exit(1);
  }
  return 0;
}
