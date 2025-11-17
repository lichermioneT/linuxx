#include <unistd.h>
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
    int cnt = 10;
    while(cnt)
    {
      printf("我是子进程：%d, 父进程的:%d , %d \n", getpid(), getppid(), cnt--);
      sleep(1);
    }
  }
  sleep(15);
  pid_t ret = wait(NULL);
  if(id > 0)
  {
    printf("wait success :%d \n", ret);
  }
  return 0;
}
