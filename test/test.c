#include <unistd.h>
#include <stdio.h>
#include <wait.h>
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
      printf("子进程。。。。。、id:%d\n", getpid());
      sleep(2);
    }
    return 2;
  }


  int status = 0;
  wait(&status);
  if(WIFSTOPPED(status))
  {
    printf("%d\n", WSTOPSIG(status));
  }

  
  return 0;
}
