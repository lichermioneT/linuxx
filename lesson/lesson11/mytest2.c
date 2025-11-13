#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <assert.h>

int main()
{
  pid_t id = fork();
  assert(id >= 0);
    
  if(id == 0)
  {
    while(1)
    {
      printf("I AM PORCESS, ppid : %d, pid : %d id : %d \n", getppid(), getpid(), id);
      sleep(2);
    }
  }
  else
  {
    while(1)
    {
      printf("I AM PORCESS, ppid : %d, pid : %d id : %d \n", getppid(), getpid(), id);
      sleep(2);
    }
  }
    return 0;
}

