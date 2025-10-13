#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

int main()
{

/*
  pid_t id = fork();
  if(id < 0)
  {
    perror("fork failed ");
    return -1;
  }
  else if (id == 0)
  {
    printf("I AM CHILD  ppid is %d, pid is %d \n", getppid(), getpid());
  }
  else 
  {
    printf("I AM PARENT ppid is %d, pid is %d \n", getppid(), getpid());
  }
*/

  pid_t id = fork();
  if(id < 0)
  {
    perror("fork failed ");
    return -1;
  }
  else if(id == 0)
  {
    sleep(3);
    exit(EXIT_SUCCESS);
  }
  else 
  {
    while(1)
    {
      sleep(1);
    }
  }















  return 0;
}
