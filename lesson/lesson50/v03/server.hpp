#pragma once 
#include "protocal.hpp"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <unistd.h>
#include <cstring>
#include <stdio.h>
#include <iostream>
#include <string>
#include <functional>
using namespace std;
static const uint16_t gport = 8080;

using func_t = std::function<bool(const httprequest&, httpresponse&)>;

class server
{
public:
  server(func_t func, const uint16_t& port = gport) 
    :_func(func)
    ,_sock(-1)
    ,_port(port)
  {}
  
  void init()
  {
    //1.创建socket
    _sock = socket(AF_INET, SOCK_STREAM, 0);
    if(_sock == -1)
    {
      cout<< "socket failed" << endl;
      exit(-1);
    }
  
    //2.bind自己的网络信息
    struct sockaddr_in server;           // 设置进内核，最好清零一下的
    memset(&server, 0, sizeof(server));

    server.sin_family = AF_INET;         // AF_INET= PF_INET
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(_port);

    int n = bind(_sock, (struct sockaddr*)&server, sizeof(server));
    if(n < 0)
    {
      exit(-2);
    }

    //3.设置socket为监听状态的. TCP面向连接的
    n =  listen(_sock, 5); // 第二个参数不讲
    if(n < 0)
    {
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
        continue;
      }
  
      pid_t id = fork();
      if(id == 0)
      {
        close(_sock);
        if(fork() > 0)exit(0);
        close(_sock);
        handerhttp(sock);
        close(sock);
        exit(0);
      }
    
    }
  }

  void handerhttp(int sock)
  {
    //1.读一个完整的请求
    //2.反序列化
    //3.请求
    //4.序列化
    //5.发送
    char buffer[4096] = {0};
    httprequest req;
    httpresponse resp;
    size_t n = recv(sock, buffer, sizeof(buffer) - 1, 0); // 假设一个完整的报文信息
    if(n > 0)
    {
      buffer[n] = 0;
      req.inbuffer = buffer;
      req.parse();

      _func(req, resp);

      send(sock, resp.outbuffer.c_str(), resp.outbuffer.size(), 0);
    }
  }

private: 
  func_t _func;
  int _sock;  // 用来监听新的链接  listen套接字
  uint16_t _port;
};
