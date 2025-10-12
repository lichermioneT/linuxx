#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main()
{

  pid_t id = fork();
  if(id == 0)
  {
    printf("我是子进程ppid:%d, pid:%d \n", getpid(), getppid());
    exit(42);
  }
  else 
  {
    int status = 0;
    pid_t ret = wait(&status);
    printf("child is %d , exit code %d \n",ret, WEXITSTATUS(status));
  }









  return 0;
}

