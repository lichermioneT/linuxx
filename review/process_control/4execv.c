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

    char* argv[] = {"ls", "-a", "-l", "-h", "/", NULL};
    printf("execl function \n");
    execv("/bin/ls", argv);
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
