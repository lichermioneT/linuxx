#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>

int main()
{


  int a = 10;
  
  pid_t id = fork();

  if(id < 0)
  {
    printf("子进程创建失败\n");
  }
  else if (id == 0)
  {
    a = 1000;
    printf("子进程 %d\t , %p \n", a, &a);
  }
  else 
  { 
    a = 100;
    printf("父进程 %d\t, %p \n", a, &a);
  }












  return 0;
}
