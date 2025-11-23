#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main()
{

  chdir("/root/linuxx");
  pid_t id = fork();

  if(id == 0)
  {
    while(1)
    {
      printf("running.... id : %d, pid : %d\n", getpid(), getppid());
      sleep(2);
    }
  }

  wait(NULL);
  



  return 0;
}
