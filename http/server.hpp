#pragma once
#include <iostream>
#include <sys/socket.h>   // socket(), bind(), listen(), accept(), send(), recv() 等核心函数
#include <sys/types.h>
#include <netinet/in.h>   // struct sockaddr_in, htons(), htonl() 等网络字节序转换
#include <arpa/inet.h>    // inet_pton(), inet_ntop() 等 IP 地址转换函数
#include <unistd.h>       // close(), read(), write() 等系统调用
#include <fcntl.h>        // fcntl() 用于设置非阻塞等文件控制选项
#include <errno.h>        // errno 全局变量，用于错误处理
#include <string.h>       // memset(), strerror() 等
#include <stdio.h>        // perror(), printf() 等
#include <stdlib.h>       // exit(), atoi() 等
#include <string>
#include <cstdint>   // for uint16_t
using namespace std;


class server
{
public:
  server(uint16_t port = uint16_t())
    :_sockfd(-1)
    ,_port(port)
  {}
   
  void init()
  {
// 1.socket
    _sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(_sockfd < 0)
    {
      cout<< "socket failend" << strerror(errno) <<endl;
      exit(-1);
    }
    
// 2.bind
    struct sockaddr_in local;
    memset(&local, 0, sizeof(local));

    local.sin_family = AF_INET;
    local.sin_addr.s_addr = htonl(INADDR_ANY);
    local.sin_port = htons(_port);
  
    int n = bind(_sockfd, (struct sockaddr*)&local, sizeof(local));
    if(n < 0)
    {
      cout<< "bind failend" << strerror(errno) <<endl;
      exit(-1);
    }

// 3.listen
    n = listen(_sockfd, 5);
    if(n < 0)
    {
      cout<< "listen failend" << strerror(errno) <<endl;
      exit(-1);
    }
  }

  void start()
  {
// 4.accept;
    while(true)
    {
      struct  sockaddr_in client;
      socklen_t len = sizeof(client);
      int sock = accept(_sockfd, (struct sockaddr*)&client, &len);
      
      if(sock < 0)
      {
        continue;
      }

      pid_t id = fork();
      
      if(id == 0)
      {
        close(_sockfd);

        if(fork() > 0) exit(0);
        close(_sockfd);

        handerhttp(sock);

        close(sock);
        exit(0);
      }
    }
  }








private:
  int _sockfd;
  uint16_t _port;
};
