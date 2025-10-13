#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(int agrc, char* argv[])
{
  
/*
  for(int i = 0; env[i]; i++)
  {
    printf("%s \n", env[i]); // %s是一个指针，指向的数据。
  }
*/


/*
  extern char** environ;
  for(int i = 0; environ[i]; i++)
  {
    printf("%s \n", environ[i]);
  }
*/


// 环境变量可以被子进程继承下去

  pid_t id = fork();
  if(id == 0)
  {
  extern char** environ;
  for(int i = 0; environ[i]; i++)
  {
    printf("%s \n", environ[i]);
  }
  }







  return 0;
}
