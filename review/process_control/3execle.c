#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


int main()
{
  
  pid_t id = fork();
  
  if(id < 0)
  {
    perror(" fork failed ");
    return -1;
  }
  else if(id == 0)
  {
    char* envp[] = {"PATH=/bin", "CUSTOM_VAR=hello", NULL};
    printf("execle function \n");
    execle("/usr/bin/printenv", "printenv", "CUSTOM_VAR", NULL, envp);
    perror("execl failed");
    exit(1);
  }
  else 
  {
    int status = 0;
    pid_t id = wait(&status);
    printf("child is %d, exit code %d\n", id, WEXITSTATUS(status));
  }









  return 0;
}
