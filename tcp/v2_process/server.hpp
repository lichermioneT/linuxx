#pragma once 
#include "log.hpp"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <sys/wait.h>
#include <unistd.h>
#include <cstring>
#include <stdio.h>
#include <iostream>
#include <string>
using namespace std;
static const uint16_t gport = 8080;


class server
{
public:
  server(const uint16_t& port = gport) 
    :_sock(-1)
    ,_port(port)
  {}
  
  void init()
  {
    //1.创建socke
    _sock = socket(AF_INET, SOCK_STREAM, 0);
    if(_sock == -1)
    {
      cout<< "socket failed" << endl;
      logmessage(FATAL, "create socket error");
      exit(-1);
    }
    logmessage(NORMAL, "create socket success");
  
    //2.bind自己的网络信息
    struct sockaddr_in server;           // 设置进内核，最好清零一下的
    memset(&server, 0, sizeof(server));

    server.sin_family = AF_INET;         // AF_INET= PF_INET
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(_port);

    int n = bind(_sock, (struct sockaddr*)&server, sizeof(server));
    if(n < 0)
    {
      logmessage(FATAL, "bind failed");
      exit(-2);
    }

    //3.设置socket为监听状态的. TCP面向连接的
    n =  listen(_sock, 5); // 第二个参数不讲
    if(n < 0)
    {
      logmessage(FATAL, "listen failed");
      exit(-3);
    }
  }
    
  void start()
  {
    for(;;)
    {
      //1.server获取新链接的。来一个链接，分配一个file discriptor
      
      struct sockaddr_in client;
      socklen_t len = sizeof(client);
      int sock = accept(_sock, (struct sockaddr*)&client, &len);
      if(sock < 0)
      {
        logmessage(ERROR, "accept failed");
        continue;
      }
      logmessage(NORMAL, "accept a new link success");

      cout<< "sock:" << sock <<endl;
      
      // 子进程继承父进程的文件描述符号
      pid_t id = fork();
      if(id == 0) // child
      {
        close(_sock); // 子进程不需要监听的文件描述符号,子进程不需要监听的。
        if(fork() > 0) exit(0); // child exit

        serverio(sock); // grandchild gono  操作系统领养的孤儿进程的。
        close(sock);
        exit(0);
      }

      pid_t ret = waitpid(id, nullptr, 0); // wait child  马上就进行回收了的。
      if(ret > 0)
      {
        cout<< "wait success " << ret <<endl; 
      }
    
    }
  }

  void serverio(int sockfd)
  {
    char buffer[1024] = {0};
    while(true)
    {
      size_t n = read(sockfd, buffer, sizeof(buffer)-1);
      if(n > 0) 
      {
        //目前当字符串处理
        buffer[n] = 0;
        cout<< "recv message:" << buffer << endl;

        string outbuffer = buffer;
        outbuffer += "server[echo]";

        write(sockfd, outbuffer.c_str(), outbuffer.size()); // 写回去的。
      }
      else if(n == 0)
      {
        // 这里n==0代表clien退出了的。
        logmessage(NORMAL, "client quit");
        break;
      }
    }
  }




private: 
  int _sock;  // 用来监听新的链接  listen套接字
  uint16_t _port;
};
