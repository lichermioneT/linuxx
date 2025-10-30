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
  //

// 假如是一个进程的话，怎么会一个进程同时执行if else if else 语句呢！
// 所以这里是两个进程执行的代码，
// fork之后，子进程父进程谁先执行，不知道的
     
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
    printf("\n");
    sleep(5);
  }
  return 0;
}
