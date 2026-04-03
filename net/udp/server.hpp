#pragma once
#include <iostream>
#include <string>
#include <cstring>
#include <cerrno>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
using namespace std;

class server
{
public:
  server(uint16_t port)
    :_port(port)
    ,_sock(-1)
  {}
  
  ~server()
  {
    if(_sock >= 0 )
    {
      close(_sock);
    }
  }

// 1.创建有个网络通信的文件描述符
  bool CreateSocket()
  {
    _sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(_sock < 0)
    {
      
      cout<< "socket failed" <<endl;
      return false;
    }
  
    cout<< "socket success" <<endl;
    return true;
  }

// 2.网络文件描述符和服务器地址和端口绑定

  bool Bind()
  {
    struct sockaddr_in local;
    bzero(&local, sizeof(local));

    local.sin_family = AF_INET;
    local.sin_addr.s_addr = htonl(INADDR_ANY);
    local.sin_port = htons(_port);
    
    if(bind(_sock, (struct sockaddr*)&local, sizeof(local)) < 0)
    {
      cout<< "bind success" <<endl;
      return false;
    }
    
    return true;
  }

// 3.UDP服务器就可以通信了的。
  bool Recv()
  {
    char buffer[1024] = {0};
    for(;;)
    {
      struct sockaddr_in client;
      bzero(&client, sizeof(client));
      socklen_t len = sizeof(client);
      
      char clientip[INET_ADDRSTRLEN];

      ssize_t s = recvfrom(_sock, buffer, sizeof(buffer) - 1, 0, (struct sockaddr*)&client, &len);
      if(s > 0)
      {
        buffer[s] = 0;
        
        if(inet_ntop(AF_INET, &client.sin_addr, clientip, INET_ADDRSTRLEN) == nullptr)
        {
          cout<< "inet_ntop failed" <<endl;
          return false;
        }
        
        cout<<"客户端[" << clientip << "]" << ":" << "["  << ntohs(client.sin_port) << "]" << "message:" << buffer <<endl;
          
        string message = "信息已经被接收到了\n";
        sendto(_sock, message.c_str(), message.size(), 0, (struct sockaddr*)&client, sizeof(client)); // 目标地址的信息
      }
    
    }

  }

private:
  uint16_t _port;
  int _sock;
};
