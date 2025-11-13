#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>


int main()
{

  pid_t id = fork();

  if(id == 0)
  {
    while(1)
    {
      printf("%d \n", getpid());
      sleep(3);
    }
  }
  else if(id > 0)
  {
    printf("%d \n", getpid());
    sleep(2);
    exit(1);
  }
  else 
  {
    ;
  }






  return 0;
}
