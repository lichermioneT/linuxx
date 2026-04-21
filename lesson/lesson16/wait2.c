#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
  pid_t id = fork(); 
  if(id == 0)
  {
    int cnt = 10;
    while(cnt)
    {
      printf("子进程ppid:%d,pid:%d, cnt=%d\n", getppid(), getpid(), cnt--);
      sleep(1);
    }
    
    exit(0);
  }
  
  sleep(15);
  pid_t ret = wait(NULL);
  if(ret > 0)
  {
    printf("wait success child id : %d\n", ret);
  }
  return 0;
}
