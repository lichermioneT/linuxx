#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>

int main()
{

  pid_t id = fork();
  
  // unistd posix操作系统访问的API
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
    printf("我是子进程 , 我的pid：%d, 我的id:%d \n", getppid(), getpid());
    printf("\n");
    sleep(5);
  }
  else 
  {
    printf("我是父进程 , 我的pid：%d, 我的id:%d\n", getppid(), getpid());
    sleep(5);
  }
 }
  
 





  return 0;
}
