#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
int main()
{

  while(0)
  {
    printf("我是一个进程 我的id:%d, 我的父进程的id: %d \n", getpid(), getppid());
    sleep(1);
  }

  printf("当前bash的id:%d\n", getppid());
  printf("当前bash的子进程的id:%d\n", getpid());
  
  // fork之后代码共享
  // 同一个变量，在不被修改的情况下，竟然有不同的内容
  pid_t id = fork();
  // printf("我是一个进程 我的id:%d, 我的父进程的id: %d , %d \n", getpid(), getppid(),id);
  
  if(id == 0)
  {
      printf("我是子进程 我的id:%d, 我的父进程的id: %d , %d \n", getpid(), getppid(),id);
  }
  else if(id > 0)
  {

    printf("我是父亲进程 我的id:%d, 我的父进程的id: %d , %d \n", getpid(), getppid(),id);
  }
  else 
  {
    ;
  }

  return 0;
}
