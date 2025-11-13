#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{


  pid_t id = fork();
 
  if(id == 0)
  {
    while(1)
    {
      printf("I AM PORCESS, ppid : %d, pid : %d id : %d \n", getppid(), getpid(), id);
      sleep(2);
    }
  }
  else if(id > 0)
  {
    while(1)
    {
      printf("I AM PORCESS, ppid : %d, pid : %d id : %d \n", getppid(), getpid(), id);
      sleep(2);
    }
  }
  else 
  {
    ;
  }

/*
  while(1)
  {
    printf("hello process my ID :%d , my PID: %d \n", getpid(), getppid());
    sleep(1);
  }
*/ 




  return 0;
}
