#pragma once
    
#include <netinet/in.h>    
#include <strings.h>    
#include <sys/types.h>    
#include <sys/socket.h>    
#include <arpa/inet.h>    
    
#include <cstring>    
#include <functional>    
#include <unistd.h>    
#include <stdlib.h>    
#include <cerrno>    
#include <iostream>    
#include <string>    
using namespace std;  



class client
{
public:
  client(const string& ip, const uint16_t& port)
    :_ip(ip)
    ,_port(port)
    ,_socketfd(-1)
  {}

  void init()
  {
// 1.客户端创建套接字。套接字使用的通信协议 和 通信类型。然后摸就是零。
   _socketfd = socket(AF_INET, SOCK_DGRAM, 0);
   if(_socketfd == -1)
   {
     cout<< "socket failed" <<endl;
   }
  }

  void start()
  {
    char buffer[1024] = {0};
    for(;;)
    {
      
     cout<< "请输入:";
     cin>>buffer;
     int send_len = strlen(buffer);

     struct sockaddr_in server;
     socklen_t len = sizeof(server);
     bzero(&server, sizeof(server));
    
     server.sin_family = AF_INET;
     server.sin_addr.s_addr = inet_addr(_ip.c_str());
     server.sin_port = htons(_port);
// 2.客户端不需要显示的bind，OS会自动帮你bind，socket和你的ip+port信息的。
// 3.send的时候给你bind滴。sendto.send的消息之外还有,自己的信息需要发送过去的呢。
     sendto(_socketfd, buffer, send_len, 0, (struct sockaddr*)&server, len);
      
     char outbuffer[1024] = {0};
     struct sockaddr_in server_info;
     socklen_t len_in =  sizeof(server_info);
     recvfrom(_socketfd, outbuffer, sizeof(buffer) - 1, 0, (struct sockaddr*)&server_info, &len_in);
     cout<< outbuffer <<endl;

    }
  }







private:
  string _ip;
  uint16_t _port;
  int _socketfd;
};
