#pragma once 
#include "log.hpp"
#include "protocol.hpp"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <functional>
#include <sys/wait.h>
#include <unistd.h>
#include <cstring>
#include <stdio.h>
#include <iostream>
#include <string>
using namespace std;
static const uint16_t gport = 8080;




// req输出型参数
// resp输出型参数
typedef function<bool(const request& req, response& resp)> func_t;

  void handerEnter(int sock, func_t func)
  {
    string inbuffer;
    while(true)
    {
     //1.读取:contentlen\r\nx op y\r\n
     //1.怎么保证是一个完整的数据请求呢？
// 不能保证，明确报文的边界信息的。
    string req_text, req_str;                // 服务端读取一个明确的请求来的
    if(!recvRequset(sock, inbuffer,  &req_text)) return;
    if(!delength(req_text, &req_str)) return;

    // 2.反序列化
    // 2.1得到一个结构化的请求对象
    request req;                          // req用来接收已经，序列化的数据 
    if(!req.deserialize(req_text)) return; // 反序列化失败了，直接返回了的。

    // 3.计算处理，req.x req.op, req.y; 业务逻辑处理的。
    // 3.1得到一个结构化的响应  
    response resp; 
    func(req,resp);

    // 4.对响应response,进行序列化
    // 4.1得到一个字符串
    string resp_str;
    resp.serialize(&resp_str);
    
    // 5.发送响应的
    string send_string = enlength(resp_str);

    send(sock, send_string.c_str(), send_string.size(), 0);
  
    }
  }
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
    
  void start(func_t func)
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
        /*
         *if(fork() > 0) exit(0); // child exit
         *serverio(sock); // grandchild gono  操作系统领养的孤儿进程的。
         */
          
        handerEnter(sock, func);

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
