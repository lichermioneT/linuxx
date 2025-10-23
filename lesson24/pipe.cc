#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <cassert>
using namespace std;

int main()
{

  int fd[2];
  int ret = pipe(fd);
  assert(ret == 0);
  if(ret != 0)
  {
    exit(1);
  }
  
  pid_t id = fork();
  assert(id >= 0);
  if(id < 0)
  {
    exit(1);
  }

  if(id == 0)
  {
    int cnt = 0;
    close(fd[0]);
    const char* msg = "我是子进程，正在在给你写信息";
    char buffer[1024];
    snprintf(buffer, sizeof(buffer),"%s, %d \n", msg,cnt++);
    write(fd[1],buffer, sizeof(buffer));
  }
    

  if(id > 0)
  {
    close(fd[1]);
    char buffer[1024];
    read(fd[0],buffer, sizeof(buffer));
    cout<< buffer <<endl;
  }



















  return 0;
}
