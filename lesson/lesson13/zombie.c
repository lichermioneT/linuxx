#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<stdlib.h>

int main()
{

  pid_t id = fork();

  if(id == 0)
  {
    printf("我是子进程  ppid: %d  pid : %d \n", getppid(), getpid());
    sleep(5);
    exit(-1);
  }
  else 
  {

    while(1)
    {
      printf("我是父进程  ppid: %d  pid : %d \n", getppid(), getpid());
    }
  }
  return 0;
}
