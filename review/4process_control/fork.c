#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>


int main()
{


  pid_t id = fork();
  if(id < 0)
  {
    perror("fork failed \n");
    return 1;
  }

//  printf("公共代码 \n");
  else if(id == 0)
  {
    printf("我是子进程 \n");
    printf("我的ppid是 %d, pid是 %d \n", getppid(),  getpid());
  }
  else 
  {
    printf("我是父进程 \n");
    printf("我的ppid是 %d, pid是 %d \n", getppid(),  getpid());
  }
  





  return 0;
}
