#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int main()
{

  pid_t id = fork();
  assert(id >= 0);

  if(id == 0)
  {
    printf("I AM CHILD \n");
    sleep(5);
    exit(1);
  }
  else 
  {
    int status = 0;
    pid_t child_pid = wait(&status);
    printf("status : %x \n", status);
    printf("child_pid : %d \n", child_pid);
  }






  return 0;
}

