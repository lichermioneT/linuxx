#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
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
    // execl:list,需要list可以执行程序的位置，可以执行程序终端的list
    //execl("/usr/bin/ls", "ls", "-a", "-l", "-h", NULL); // execl：l就是list,就和你终端是如何list一样的执行方法。注意需要NULL结尾的。
    //execl("/usr/bin/pwd", "pwd", NULL); // execl：l就是list,就和你终端是如何list一样的执行方法。注意需要NULL结尾的。
    //execl("/usr/bin/touch", "touch", "list.txt", NULL); // execl：l就是list,就和你终端是如何list一样的执行方法。注意需要NULL结尾的。
    execl("/usr/bin/top", "top", NULL); // execl：l就是list,就和你终端是如何list一样的执行方法。注意需要NULL结尾的。
    exit(1);
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
    printf("wait success exit_code:%d\n", WEXITSTATUS(status));
  }
  else if (WIFSIGNALED(status))
  {
    printf("wait success sig_num:%d\n", WTERMSIG(status));
  }
  return 0;
}
