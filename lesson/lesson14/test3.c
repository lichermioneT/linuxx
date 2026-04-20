#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
  pid_t id = fork();
  if(id == -1)
  {
    perror("fork");
    return 1;
  }
  
  if(id == 0)
  {
    while(1)
    {
      printf("子进程: %d,%d\n", getppid(), getpid());
      sleep(2);
    }
  }
  else 
  {
    while(1)
    {
      printf("父进程: %d, %d\n", getppid(), getpid());
      sleep(2);
    }
  }

  return 0;
}
