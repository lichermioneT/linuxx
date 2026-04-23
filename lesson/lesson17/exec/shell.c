#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/wait.h>

#define LINE_SIZE 1024
#define ARGVS 64

int main()
{
while(1)
{
  printf("[root@lic exec]#:::");
  fflush(stdout);
  char line[LINE_SIZE];
  char* s = fgets(line, sizeof(line) - 1, stdin);
  assert(s != NULL);
  (void)s;

  /*
   *printf("line:%s\n", line);
   */
  line[strlen(line)-1] = 0; 

  char* arg[ARGVS] = {NULL};
  arg[0] = strtok(line, " ");
  
  int i = 1;
  while( arg[i++] = strtok(NULL, " "))
  {
    ;
  }

#if 0
  for(int i = 0; arg[i]; ++i)
  {
    printf("%d:%s\n", i, arg[i]);
  }
#endif

  if(strcmp(arg[0], "cd") == 0)
  {
    if(arg[1])
    {
      chdir(arg[1]);
    }
    continue;
  }

  if(strcmp(arg[0], "echo") == 0)
  {
    printf("%s\n", arg[1]);
    continue;
  }

  pid_t id = fork();
  if(id == -1)
  {
    perror("fork");
    return 1;
  }

  if(id == 0)
  {
    execvp(arg[0], arg);
    exit(1);
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
    printf("exit_code:%d\n", WEXITSTATUS(status));
  }
  else if (WIFSIGNALED(status))
  {
    printf("sig_num:%d\n", WTERMSIG(status));
  }

  }
  return 0;
}
