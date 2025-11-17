#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>


int main()
{
  pid_t id = fork();
  if(id == 0)
  {
    int cnt = 5;
    while(cnt)
    {
      printf("我是子进程：%d, 父进程的:%d , %d \n", getpid(), getppid(), cnt--);
      sleep(1);
    }
    exit(10);
  }
  sleep(10);
  int status = 0;
  // 15-8退出状态
  // 7core dump
  // 6-0终止信号
  pid_t ret = waitpid(id, &status, 0);
  if(id > 0)
  {
    printf("wait success : ret : %d,  sig number : %d, child exit code : %d  \n", ret, (status & 0x7F), (status>>8)&0xFF);
  }
  return 0;
}
