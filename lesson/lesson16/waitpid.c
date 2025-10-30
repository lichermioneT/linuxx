#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main()
{

  pid_t id = fork();
  
  if(id == 0)
  {
    // 子进程
    int cnt = 5;
    while(cnt)
    {
      printf("我是子进程：%d, 父进程：%d, cnt:%d\n", getpid(), getppid(),cnt--);
      sleep(1);
    }
    // 子进程的退出码
  exit(12); // 进程退出
  }

  // 父亲进程
  // pid_t ret = wait(NULL);
  int status = 0;
  pid_t ret = waitpid(id, &status, 0);
  if(id > 0)
  {
    printf("wait success: %d , status : %d , sig number : %d , exit : %d \n", ret, status, (status & 0x7F), (status >> 8) & 0xFF);
  }
  return 0;
}
