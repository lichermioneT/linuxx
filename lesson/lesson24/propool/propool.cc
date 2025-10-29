#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <cassert>
using namespace std;

#define PROCESS_NUM 5

int main()
{
  
  // 创建一批子进程
  // 子进程id和写入的fd
  //
  for(int i = 0; i < PROCESS_NUM; i++)
  {
    int fds[2];
    int n = pipe(fds);
    assert(n == 0);
    (void)n;

    pid_t id = fork();

    if(id == 0)
    {
      // 处理任务
      close(fds[1]);

      exit(0);
    }

    close(fds[0]);
  }

  // 父进程控制子进程
  




  // 回收子进程












  return 0;
}


