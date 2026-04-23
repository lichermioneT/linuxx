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
    char* const argv1_[] = {"ls", "-a", "-l", "-h", NULL};
    char* const argv2_[] = {"pwd", NULL};
    char* const argv3_[] = {"top", NULL};
    execvp("top", argv3_);
    execvp("pwd", argv2_);
    execvp("ls", argv1_);
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
