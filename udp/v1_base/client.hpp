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

     sendto(_socketfd, buffer, send_len, 0, (struct sockaddr*)&server, len);
    }
  }







private:
  string _ip;
  uint16_t _port;
  int _socketfd;
};
