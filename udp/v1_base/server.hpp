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
    
    _socketfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(_socketfd == -1)
    {
      cout<< "socket failed" <<endl;
      exit(1);
    }

    struct sockaddr_in server;
    memset(&server, 0, sizeof(server)); // 设置进内核注意清零的
    server.sin_family = AF_INET;
    /*
     *server.sin_addr.s_addr = htonl(inet_addr(_ip.c_str()));
     */
    
    // 服务器不建议绑定固定的ip地址。下面才是主流的写法
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(_port); 

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

      ssize_t s = recvfrom(_socketfd, buffer, sizeof(buffer) - 1, 0 , (struct sockaddr*)&client, &len);
      if(s > 0)
      {
        buffer[s] = 0;
        cout<< "客户端ip: " << client.sin_addr.s_addr << "客户端port: " << client.sin_port <<endl;
        cout<< "消息：" << buffer <<endl;
      }
    }
  }




private:
  u_int16_t _port;
  string _ip;
  int _socketfd;
};

