#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>


int main()
{
  
  int a = 10;
  
  pid_t id = fork();

  if(id < 0)
  {
    perror("fork failed");
    return -1;
  }
  else if(id > 0)
  {
    a = 100;
    printf("我是父进程，我的a是：%d \n", a);
  }
  else  
  {
    a = 1000;
    printf("我是子进程，我的a是：%d \n", a);
  }










  return 0;
}
