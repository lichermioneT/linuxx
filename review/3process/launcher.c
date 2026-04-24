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

extern char** environ;

int main()
{
  char* env1 = "MY_NAME=lic";
  char* env2 = "MY_LEVEL=linux";
  putenv(env1);
  putenv(env2);

  pid_t id = fork();
  if(id == -1)
  {
    perror("fork");
    return 1;
  }

  if(id == 0)
  {
    //execl("./reader", "./reader", NULL); // l:命令行展开执行的方式，需要指定可执行程序的位置
    //execlp("ls", "ls", "-a", "-l", "-h", NULL); // p:path环境变量里面进程查找的，l:命令行展开执行的方式
    char* const envp[] = {"MY_NAME=lic", "MY_LEVEL=linux", NULL}; // 自己组织的环境变量的, 也可以putenv添加环境变量，然后用户environ变量
    execle("./reader", "./reader", NULL, envp);

    return 1;
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
