#include <iostream>
#include <unistd.h>
#include <cstring>
#include <cassert>
using namespace std;

int main()
{

  int fds[2] = {0};
  int r = pipe(fds);
  assert(r == 0);

  pid_t id = fork();
  assert(id >= 0);

  if(id == 0)
  {
    close(fds[1]);
    
    char buffer[1024];
    read(fds[0], buffer, sizeof(buffer)); // 子进程读取
    cout<< "I AM CHILD, receive msg :" << buffer <<endl;

    close(fds[0]);
  }
  else 
  {
    close(fds[0]);
    const char* msg = "hello from paret";
    write(fds[1], msg, strlen(msg) + 1);
    close(fds[1]);
  }

// read函数，将fd中的文件读取到buffer
// write函数，将buffer内容写入到fd里面











  return 0;
}
