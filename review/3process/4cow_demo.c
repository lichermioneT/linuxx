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

int g_val = 100;

int main()
{
  
  int* p = (int*)malloc(sizeof(int));
  if(p == NULL)
  {
    perror("malloc");
    return 1;
  }

  *p = 200;
  
  int x = 300;

  pid_t id = fork();
  if(id == -1)
  {
    perror("fork");
    return 1;
  }

  if(id == 0)
  {
    g_val = 10;
    *p = 20;
    x = 30;
    
    int cnt =3;
    while(cnt)
    {
      printf("child: g_val:%d,%p, p:%d,%p, x:%d,%p\n", g_val, &g_val, *p, p, x, &x);
      sleep(1);
      --cnt;
    }

    return 12;
  }
  else if (id > 0)
  {
    g_val = 1;
    *p = 2;
    x = 3;
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
        printf("paret: g_val:%d,%p, p:%d,%p, x:%d,%p\n", g_val, &g_val, *p, p, x, &x);
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
