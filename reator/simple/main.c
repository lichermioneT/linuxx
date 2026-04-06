#include "server.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char* argv[])
{
  if(argc < 3)
  {
    printf("./a.out port path\n");
    return -1;
  }

  unsigned short port = atoi(argv[1]);
  
  // 切换路径
  if(chdir(argv[2]) == -1)
  {
    perror("chdir");
    return -1;
  }

  // 初始化监听的套接字  
  int lfd = initListenFd(port); // 建议大于 50000 

  // 启动服务程序

  epollRun(lfd);


  return 0;
}
