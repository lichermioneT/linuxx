#include "server.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
  if(argc < 3)
  {
    printf("./a.out port path\n");
    return -1;
  }

  unsigned short port = atoi(argv[1]);
  
  // 切换路径
  chdir(argv[2]);


  // 初始化监听的套接字  
  int lfd = initListenFd(10000); // 建议大于 50000 

  // 启动服务程序

  epollRun(lfd);


  return 0;
}
