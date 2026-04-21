#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
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
    int cnt = 20;
    while(cnt)
    {
      printf("子进程，ppid:%d, pid:%d, cnt:%d\n", getppid(), getpid(), cnt--);
      sleep(1);

      /*
       *int* p = NULL;
       **p = 100;
       */
    }
  
    //exit(12);
    return 13;
  }

  sleep(7);
  int status = 0;
  pid_t ret = wait(&status);
  printf("wait success, pid : %d, exit_code:%d sig_number %d\n", ret,(status>>8)&0xFF, (status&0X7F));
  return 0;
}
