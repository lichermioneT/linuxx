#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

int main()
{
  pid_t id = fork();
  if(id == -1)
  {
    perror("fork");
    return 1;
  }
  
// 提供一个简单，明确的信号，告诉子进程你是创建出来的孩子的
  if(id == 0) 
  { 
    int cnt = 5;
    while(cnt)
    {
      printf("child, ppid:%d, pid%d\n", getppid(), getpid());
      --cnt;
      sleep(1);
    }
  }
// 核心：需要知道子进程的pid，才能够进行回收的，方便管理的。
  else if (id > 0)
  {
    int cnt = 5;
    while(cnt)
    {
      printf("parent, ppid:%d, pid%d\n", getppid(), getpid());
      --cnt;
      sleep(1);
    }

    int status = 0;
    int ret = waitpid(id, &status, 0);
    if(ret == -1)
    {
      perror("waitpid");
      return 1;
    }
    
    if(WIFEXITED(status))
    {
      printf("wait suceess exit_code:%d\n", WEXITSTATUS(status));
    }
    else if (WIFSIGNALED(status))
    {
      printf("wait success sig_num:%d\n", WTERMSIG(status));
    }

  }
  return 0;
}
