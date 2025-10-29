#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <assert.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
  
  pid_t id = fork();
  
  if(id == 0)
  {

    int cnt = 5;
    while(cnt)
    {
      printf("child running,pid:%d, ppid:%d,cnt:%d\n", getpid(),getppid(), cnt--);
      sleep(1);
    }
    
    exit(12);
  }
  
  int status = 0;
  int ret = waitpid(id,&status, 0);
  // 1.OS获取子进程的僵尸状态
  // 2.获取子进程的退出结果
  if(ret > 0)
  {
    //    printf("wait success, exit code: %d, sig: %d \n", (status>>8)&0xFF, status & 0x7F);
    if(WIFEXITED(status))
    {
      printf("exit code: %d\n",  WEXITSTATUS(status));
    }
    else 
    {
      printf("退出失败\n");
    }
  }

  return 0;
}
