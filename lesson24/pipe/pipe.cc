#include <iostream>
#include <unistd.h>
#include <cassert>
#include <sys/wait.h>
#include <cstring>
#include <cstdio>

using namespace std;

int main()
{

// #include 
// int pipe[int pipefd[2]];
// 创建一个管道文件
  
  int fds[2];
  int n = pipe(fds);
  
  assert(n == 0);
  
  // 0 1 2 3 4
  // 谁是读取
  // 谁是写入
  // [0]读，嘴巴，读书得
  // [1]写，钢笔，写的
/*
  cout<< "fds[0]: " << fds[0] <<endl;
  cout<< "fds[1]: " << fds[1] <<endl;
*/ 
  
  pid_t id = fork();
  assert(id >= 0);

  if(id == 0)
  {
    // 子进程通信代码
    close(fds[0]); // 子进程关闭读
    
    const char* s = "我是子进程，我正在给你发消息";
    int cnt = 0;
    while(true)
    {
       cnt++;
       char buffer[1024];
       snprintf(buffer, sizeof(buffer), "child->parent say: %s[%d][%d]\n", s, cnt, getpid()); // 都是子进程的信息
       write(fds[1], buffer, strlen(buffer));
       cout<< " cnt :" << cnt <<endl; // 写段写满的时候，在阻塞等待，等读端读取数据
    }

    close(fds[1]);
    exit(0);
  }

  close(fds[1]); // 关闭父进程的写
  while(true)
  {
    char buffer[1024];
    // 如果管道中没有数据了，读取端正在读， 默认会直接阻塞当前正在读取的进程！
    ssize_t s =read(fds[0], buffer, sizeof(buffer));
    sleep(1);
    if(s > 0) 
    {
      buffer[s] = 0;
      cout << "get message# " << buffer <<" mypid " << getppid() <<endl;
    }
    else if(s == 0) 
    {
      cout << "read:"<< s <<endl;
      break;
    }

    // 父进程没有进行sleep
  }



  n = waitpid(id, nullptr, 0);
  assert(n == id);
  close(fds[0]);

  return 0;
}
