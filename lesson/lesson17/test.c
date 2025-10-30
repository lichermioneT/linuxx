#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <assert.h>
#include <string.h>


#define NUM 10

typedef void(*func_t)(); // 函数指针

func_t hander_task[NUM];

void task1()
{
  printf("hander_task 1 \n");
}

void task2()
{
  printf("hander_task 2 \n");
}

void task3()
{
  printf("hander_task 3 \n");
}

void load()
{
  memset(hander_task,0,sizeof(hander_task));
  hander_task[0] = task1;
  hander_task[1] = task2;
  hander_task[2] = task3;
}


int main()
{

  pid_t id = fork();

  if(id < 0)
  {
    return -1; 
  }
  else if(id == 0)
  {
    int cnt = 3;
    while(cnt)
    {
      printf("I AM CHILD, ID IS %d , PPID IS %d \n", getpid(), getppid());
      sleep(1);
      cnt--;
    }
  }

  load();
  int status  = 0;
  
  while(1)
  {
    pid_t ret = waitpid(id, &status, WNOHANG);

    if(ret == 0)
    {
      printf("child is running ....\n");
      for(int i = 0; hander_task[i] != NULL; i++)
      {
        hander_task[i]();
      }
    }
    else if(ret > 0)
    {
      printf("wait success, exit code : %d , sig : %d \n", (status>>8)&0xFF, status & 0x7F);
      sleep(1);
      break;
    }
    else 
    {
      printf("wait failed \n");
      break;
    }
  }











}
