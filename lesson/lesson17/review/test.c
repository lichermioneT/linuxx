#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <assert.h>

#define NUM 10
typedef void (*func_t)(); 

void task1()
{
  printf("task1 \n");
}

void task2()
{
  printf("task2 \n");
}

void task3()
{
  printf("task3 \n");
}

func_t handerTask[NUM];

void loadTask()
{
  memset(handerTask, 0, sizeof(handerTask));
  handerTask[0] = task1;
  handerTask[1] = task2;
  handerTask[2] = task3;
}



int main()
{
  
  pid_t id = fork();
  assert(id != -1);

  if(id == 0)
  {
    int cnt = 10;
    while(cnt)
    {
      printf("child runing, pid : %d, ppid : %d, cnt : %d \n", getpid(), getppid(), cnt--);
      sleep(1);
    }
    exit(10);
  }
  
  loadTask();
  int status = 0;
  while(1)
  {
  int ret = waitpid(id, &status, WNOHANG); // 非阻塞等待，
  if(ret == 0)
  {
    // waitpid调用成功，子进程没有退出
    // 子进程没有退出，我的waitpid没有等待失败，仅仅是是检测到了子进程没有退出，
    printf("正在等待中、\n");  
    for(int i = 0; handerTask[i] != NULL; i++)
    {
      handerTask[i]();
    }

    sleep(1);
  }
  else if(ret > 0)
  {
    // waitpid调用成功，子进程退出了
    printf("wait sucess \n");
    break;
  }
  else 
  {
    printf("等待失败了、\n");
    break;
  }
  }





  return 0;
}
