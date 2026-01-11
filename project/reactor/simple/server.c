#include "server.h"

int initListenFd()
{
  //1.创建监听的fd
  //通信协议ipv4/6
  //流式的还是数据报的
  int lfd = socket(AF_INET, SOCK_STREAM, 0);
  if(lfd == -1)
  {
    perror("socket failed");
    return -1;
  }
    
 





}

