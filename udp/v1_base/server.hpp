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


static const string defaultIP = "0.0.0.0";

class server
{
public:
  server(const uint16_t& port, const string& ip = defaultIP)
    :_port(port)
    ,_ip(ip) 
    ,_socketfd(-1)
    {}
  
  void init()
  {

// 1.创建socket;
    _socketfd = socket(AF_INET, SOCK_DGRAM, 0); // 创建一个文件描述符，文件描述符号类型和通信的方法
    if(_socketfd == -1)
    {
      cout<< "socket failed" <<endl;
      exit(1);
    }

// 2.将ip和port设置进内核，还是将数据进行清零一下的
    struct sockaddr_in server;
    memset(&server, 0, sizeof(server)); // 设置进内核注意清零的
    server.sin_family = AF_INET;
    /*
     *server.sin_addr.s_addr = htonl(inet_addr(_ip.c_str()));
     */
    
    // 服务器不建议绑定固定的ip地址。下面才是主流的写法
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(_port); 
//2.1.bind进内核里面去的。注意len需要取地址的。
//    将套接字和ip+port关联起来的。
    socklen_t len = sizeof(server);  
    bind(_socketfd, (struct sockaddr*)&server, len);
  }

  void run()
  {
    for(;;)
    {
      struct sockaddr_in client;
      socklen_t len = sizeof(client);
      char buffer[1024];
//3.udp的接收数据是recvfrom函数。
//  从套接字读取数据，放到哪儿，空间大小，0，
//  谁发给你的信息需要知道，传入一个输出型的参数。
//  知道数据类型和它的大小，内核需要填充数据信息的。
  
      string message;
      ssize_t s = recvfrom(_socketfd, buffer, sizeof(buffer) - 1, 0 , (struct sockaddr*)&client, &len);
      if(s > 0)
      {
        buffer[s] = 0;
        cout<< "客户端ip: " << client.sin_addr.s_addr << "客户端port: " << client.sin_port << "消息:" << buffer <<endl;
       
        message += buffer;
        message += ":这个消息已经收到了";
        sendto(_socketfd, message.c_str(), message.size(), 0, (struct sockaddr*)&client, sizeof(client));
      }
    }
  }

private:
  uint16_t _port; // 服务端的端口
  string _ip;     // 服务端的IP
  int _socketfd;  // 套接字，(文件描述符号)
};

