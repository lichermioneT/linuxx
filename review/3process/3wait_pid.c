#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdio.h>

void print_exit_code(int status)
{
  printf("exit_code:%d\n",WEXITSTATUS(status));
}

void print_term_signal(int status)
{
  printf("term_sig:%d\n", WTERMSIG(status));
}

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
    sleep(3);
    exit(17);
  }
  else if (id > 0)
  {
    while(1)
    {
      int status = 0;
      int ret = waitpid(id, &status, WNOHANG);
      if(ret > 0)
      {
        // 进程常见的信号和退出码退出的场景。
        // 回收是为了让父进程回收子进程的系统资源比如PCB,以及子进程的退出信息的。
        if(WIFEXITED(status))
        {
          print_exit_code(status);
        }
        else if (WIFSIGNALED(status))
        {
          print_term_signal(status);
        }
        break;
      }
      else if (ret == 0)
      {
        sleep(1);
        printf("child is running, continue wait...\n");
      }
      else 
      {
        perror("waitpid");
        return 1;
      }
    }
  }


  return 0;
}
