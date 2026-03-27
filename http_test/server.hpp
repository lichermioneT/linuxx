#pragma once
#include <cerrno>
#include <sstream>

#include <sys/types.h>    
#include <sys/socket.h>    
#include <netinet/in.h>    
#include <arpa/inet.h>    
#include "protocal.hpp"
    
#include <unistd.h>    
#include <cstring>    
#include <stdio.h>    
#include <iostream>    
#include <string>    
#include <functional>

using func_t = std::function<bool(const request&, response&)>;
using namespace std;

class server
{
public:
   server(uint16_t port, func_t func)
     :_port(port)
     ,_sockfd(-1)
     ,_func(func)
    {}

public:
  void init()
  {
// 1.创建套接字。
    _sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(_sockfd == -1)
    {
      cout<< "socket failed" <<endl;
      exit(-1);
    }

    int opt = 1;
    setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
// 2.bind套接字，设置进内核里面
// 运行四次挥手期间，可以用使用该端口。(TIME_WAIT期间的)
    struct sockaddr_in local;
    
    memset(&local, 0, sizeof(local));
    local.sin_family = AF_INET;
    local.sin_addr.s_addr  = INADDR_ANY;
    local.sin_port = htons(_port);
  
    int n = bind(_sockfd, (struct sockaddr*)&local, sizeof(local));
    if(n < 0)
    {
      cout<< "bin failed" <<endl;
      exit(-2);
    }

// 3.开始监听客户端的链接
    n = listen(_sockfd, 5);
    if(n < 0)
    {
      cout<< "listen failed" <<endl;
      exit(3);
    }
  }

// 服务器的本质就是一个死循环的
  void start()
  {
    while(true)
    {
      struct sockaddr_in client;
      socklen_t len = sizeof(client);
      int socket = accept(_sockfd, (struct sockaddr*)&client, &len);
      if(socket < 0)
      {
        continue;
      }

      pid_t id = fork();
      if(id == 0)
      {
        close(_sockfd);
        if(fork() > 0) exit(0);

        handerHttp(socket);

        close(socket);
        exit(0);
      }
    }
  }
  

void handerHttp(int sock)
{
    static string inbuffer; // 简化写法，实际最好每个连接一个buffer
    string http_request;

    request req;
    response resp;

    if (ReadOneRequest(sock, inbuffer, &http_request))
    {
        cout << "收到一个完整请求:\n";
        req.inbuffer = http_request;
        req.parse();
    }
    else
    {
        cout << "读取失败，或者连接关闭" << endl;
    }
    

    _func(req, resp); // 回调函数

    send(sock, resp.outbuffer.c_str(), resp.outbuffer.size(), 0);
}
private:
  uint16_t _port;
  int _sockfd;
  func_t _func;
};
