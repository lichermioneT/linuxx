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

/*
  printf("当前bash的id:%d\n", getppid());
  printf("当前bash的子进程的id:%d\n", getpid());
*/

  // fork之后代码共享
  // 同一个变量，在不被修改的情况下，竟然有不同的内容
  
  int a = 1000;
// 前面的代码，只有父进程
  pid_t id = fork();
  // printf("我是一个进程 我的id:%d, 我的父进程的id: %d , %d \n", getpid(), getppid(),id);
  
// 后面父子进程都有，只要有共享代码修改，就会发生写实拷贝
  if(id == 0)
  { 
      a = 0;
      printf("我是子进程 我的pid:%d, 我的id: %d, a : %d, a的地址 : %p\n", getppid(), getpid(), a, &a);
  }
  else if(id > 0)
  {
      a = 1;
    printf("我是父进程  我的pid:%d,  我的id: %d, a : %d, a的地址 ：%p\n", getppid(), getpid(), a, &a);
  }
  
  printf("zzz\n");

// 父进度的pid就是当前终端
// echo $$ 当前终端的PID

// 通过fork的返回值进行分流


  return 0;
}
