#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>

int main()
{

  pid_t id = fork();
  
  // fork两个返回值
  // 成功的话父亲拿到子进程的id,子进程的id=0;
  // 失败返回-1
  
 while(1)
 {
     
  if(id < 0)
  {
    perror("fork failed");
    return -1;
  }
  else if (id == 0)
  {
    printf("我是子进程  我的id:%d , 我的父进程id：%d\n", getpid(), getppid());
    sleep(2);
  }
  else 
  {
    printf("我是父进程  我的id:%d , 我的父进程id：%d\n", getpid(), getppid());
    sleep(2);
  }
 }






  return 0;
}
