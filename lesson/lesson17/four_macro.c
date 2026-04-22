#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

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
      printf("子进程，ppid:%d, pid:%d, cnt:%d\n", getppid(), getpid(), cnt);
      --cnt;
      sleep(1);
    }

    return 12;
  }

  int status = 0;
  int ret = waitpid(id, &status, 0);
  if(ret < 0)
  {
    perror("waitpid");
    return 1;
  }
  
  if(WIFEXITED(status))
  {
    printf("%d正常退出，退出码:%d\n", ret, WEXITSTATUS(status));
  }
  else if (WIFSIGNALED(status))
  {
    printf("%d信号退出，退出信号:%d\n", ret,WTERMSIG(status));
  }
  else if (WIFSTOPPED(status))
  {
    printf("%d暂停，暂停信号:%d\n", ret,WSTOPSIG(status));
  }

  return 0;
}
