#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <assert.h>

int main()
{

  pid_t id = fork();
  assert(id != -1);
  
  int cnt = 10;
  while(cnt)
  {

  if(id == 0)
  {
    printf("I AM CHILD, PID IS %d, PPID IS %d \n", getpid(), getppid());
  }
  cnt--;
  }
  int status = 0;

  while(1)
  {
    pid_t ret = waitpid(id, &status, WNOHANG);

    if(ret == 0)
    {
      printf("CONTINUE WAITING....\n");
    }
    else if(ret > 0)
    {
      printf("wait suceesss, exit code : %d, sig : %d \n", (status>>8)&0xFF, status & 0x7F);
      break;
    }
    else 
    {
      printf("waitpid call failed \n");
      break;
    }
    sleep(1);
  }







  return 0;
}
