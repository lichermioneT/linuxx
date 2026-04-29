#include <iostream>
#include <cstdio>
#include <string>
#include <sys/wait.h>
#include <sys/types.h>
#include <cstring>
#include <unistd.h>
using namespace std;

int main()
{
  int fds[2];
  int n = pipe(fds);
  if(n == -1)
  {
    cerr<< "pipe:" << strerror(errno) <<endl;
    return 1;
  }

  pid_t id = fork();
  if(id == -1)
  {
    cerr<< "fork:" << strerror(errno) <<endl;
    return 1;
  }
  
  if(id == 0)
  {
    close(fds[0]);
    
    const char* s = "我是子进程，正在给你发消息";
    int cnt = 0;
    while(true)
    {
      char buffer[1024];
      snprintf(buffer, sizeof buffer, "hello->father %s:%d, pid:%d", s, cnt++, getpid());
      write(fds[1], buffer, strlen(buffer)); // 操作系统的给你的通信内存
      cout<< "cnt: " << cnt <<endl;
    }

    exit(0);
  }
  
  close(fds[1]);
  close(fds[0]);
  
  while(true)
  {
    sleep(4);
    char buffer[1024];
    ssize_t s = read(fds[0], buffer, sizeof(buffer) - 1); // 操作系统的缓存内进行拷贝的。
    buffer[s] = 0;
    cout<< "读取到子进程的信息: " << buffer << endl;
  }


  int status = 0;
  int ret = waitpid(id, &status, 0);
  if(ret == -1)
  {
    cerr<< "waitpid: " << strerror(errno) <<endl;
    return 1;
  }

  if(WIFEXITED(status))
  {
    cout<< "子进程的退出码:" << WEXITSTATUS(status) << endl;
  }
  else if(WIFSIGNALED(status))
  {
    cout<< "子进程的退出信号: " << WTERMSIG(status) << endl;
  }



  return 0;
}
