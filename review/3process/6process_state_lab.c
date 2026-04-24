#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>

int main()
{
  pid_t id[2] = {0};
  for(int i = 0; i < 2; ++i)
  {
    id[i] = fork();
    if(id[i] == -1)
    {
      perror("fork");
      return 1;
    }
  }
  // 1.注意fork返回的时候已经开始 写时拷贝了的
  if(id[0] == 0)
  {
    printf("子进程1 pid:%d\n", getpid());
    return 1;
  }

  if(id[1] == 0)
  {
    int cnt = 15;
    while(cnt)
    {
      printf("子进程2 ppid:%d, pid:%d\n", getppid(), getpid());
      --cnt;
      sleep(1);
    }

    exit(13);
  }

  sleep(8);
  int i = 0;
  while(i < 2)
  {
    int status = 0;
    pid_t ret = waitpid(-1, &status, 0);
    if(ret < 0)
    {
      perror("waitpid");
      return 1;
    }

    if(ret > 0)
    {
      ++i;
      if(WIFEXITED(status))
      {
        printf("ret:%d wait success，exit_code:%d\n", ret, WEXITSTATUS(status));
      }
      else if (WIFSIGNALED(status))
      {
        printf("ret%d  wait success, sig_num :%d\n", ret, WTERMSIG(status));
      }
    }
  }



  return 0;
}
