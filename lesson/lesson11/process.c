#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
int main()
{
#if 0
  printf("当前bash的id:        %d\n", getppid());
  printf("当前bash的子进程的id:%d\n", getpid());
#else
  // fork之后代码共享
  // 同一个变量，在不被修改的情况下，竟然有不同的内容
  
  int a = 1000;
// 前面的代码，只有父进程
  pid_t id = fork();
  if(id == -1)
  {
    perror("fork");
    return 1;
  }
  // printf("我是一个进程 我的id:%d, 我的父进程的id: %d , %d \n", getpid(), getppid(),id);
  
// 后面父子进程都有，只要有共享代码修改，就会发生写实拷贝
  while(1)
  {
    if(id == 0)
    { 
      a = 0;
      printf("我是子进程  我的ppid:%d,  我的id: %d, a : %d, a的地址 : %p, id == %d\n", getppid(), getpid(), a, &a, id);
      sleep(2);
    }
    else if(id > 0)
    {
      a = 1;
      printf("我是父进程  我的ppid:%d,  我的id: %d, a : %d, a的地址 ：%p, id == %d\n", getppid(), getpid(), a, &a, id);
      sleep(2);
    }
  }
  
// 父进度的pid就是当前终端
// echo $$ 当前终端的PID

// 通过fork的返回值进行分流
#endif
  return 0;
}
