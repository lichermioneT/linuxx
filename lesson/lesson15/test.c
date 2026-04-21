#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

 int g_val = 10;
int main()
{
  pid_t id = fork();
  if(id == -1)
  {
    perror("fork");
    return 1;
  }


  if(id == 0)
  {
    int cnt = 10;
    while(cnt)
    {
      printf("子进程，ppid:%d, pid:%d, g_val:%d, &g_val:%p\n", getppid(), getpid(), g_val, &g_val);
      g_val += 3;
      cnt--;
      sleep(1);
    }
    
    exit(0);
  }
  else 
  {

    int cnt = 20;
    while(cnt)
    {
      printf("父进程，ppid:%d, pid:%d, g_val:%d, &g_val:%p\n", getppid(), getpid(), g_val, &g_val);
      g_val += 2;
      cnt--;
      sleep(1);
    }

    int status = 0;
    int ret = waitpid(id, &status, 0);
    if(ret > 0)
    {
        printf("wait success, pid : %d, exit_code:%d sig_number %d\n", ret,(status>>8)&0xFF, (status&0X7F));
    }
    else 
    {
      perror("waitpid");
      return 1;
    }
  }
  return 0;
}
