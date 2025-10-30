#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<stdlib.h>

int main()
{
  pid_t id = fork();
  if(id == 0)
  {

// 子进程退出就是僵尸状态，等待父进程回收他的资源和信息
// 事情办的怎么样，有些你可以不关心，有些你需要关系，但是OS是必须关系的
// wait
// waitpid登场
    printf("我是子进程  ppid: %d  pid : %d \n", getppid(), getpid());
    sleep(5);
    exit(-1);
  }
  else 
  {
    while(1)
    {
      printf("我是父进程  ppid: %d  pid : %d \n", getppid(), getpid());
      sleep(2);
    }
  }
  return 0;
}
