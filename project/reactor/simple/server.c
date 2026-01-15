#include "server.h"

int initListenFd(unsigned short port)
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

  //2.设置端口复用
  int opt = 1;
  int ret = setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
  if(ret == -1)
  {
    perror("setsockopt failed");
    return -1;
  }

  //3.绑定 
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);         // 大小端注意
  addr.sin_addr.s_addr = INADDR_ANY;   // 监听任意网卡的数据,这个宏就是零。零没有大小端之分
  ret = bind(lfd, (struct sockaddr*)&addr, sizeof(addr));
  if(ret == -1)
  {
    perror("bind failed");
    return -1;
  }
  
  //4监听函数
  ret = listen(lfd, 128);
  if(ret == -1)
  {
    perror("listen failed");
    return -1;
  }
 
  return lfd;
}

