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

class client
{
public:
  client(const string& ip, uint16_t port)
    :_ip(ip)
    ,_port(port)
    ,_sock(-1)
  {}
  
  ~client()
  {
    if(_sock >= 0)
    {
      close(_sock);
    }
  }

  bool CreateSocket()
  {
    _sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(_sock < 0)
    {
      cout<< "socket failed" <<endl;
      return false;
    }
    
    return true;
  }


  bool Send()
  {
    char buffer[1024] = {0};
    char inbuffer[1024] = {0};
    struct sockaddr_in server;
    
    server.sin_family = AF_INET;
    if(inet_pton(AF_INET, _ip.c_str(), &server.sin_addr) < 0)
    {
      cout<< "网络字节序转换失败" <<endl;
      return false;
    }
    server.sin_port = htons(_port);

    for(;;)
    {
      cout<<"请输入:";
      cin>>buffer;

      sendto(_sock, buffer, sizeof(buffer) - 1, 0, (struct sockaddr*)&server, sizeof(server));

      recvfrom(_sock, inbuffer, sizeof(inbuffer) - 1, 0, nullptr, 0);
      cout<< inbuffer <<endl;
    }
  }

private:
  string _ip;
  uint16_t _port;
  int _sock;
};
