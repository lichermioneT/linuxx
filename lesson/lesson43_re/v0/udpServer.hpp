#pragma once
#include <sys/socket.h>
#include <string>
#include <sys/types.h>
#include <cerrno>
#include <stdlib.h>
#include <arpa/inet.h>
#include <iostream>
#include <cstring>
#include <strings.h>

using namespace std;
namespace server
{
  const static std::string defaultIp = "0.0.0.0";
  enum {SOCKET_ERR = 2};

  class udpServe
  {
  public:
    udpServe(const uint16_t &port, const std::string &ip = defaultIp) : _port(port), _ip(ip), _socket(-1)
    {
    }

    void init()
    {
      // 1. int socket(int domain, int type, int protocol)
      // domain   本地还是网络
      // type     socket提供能力
      // protocol 0
      // return value : discriptor
      
      // 1.创建套接字
      _socket = socket(AF_INET, SOCK_DGRAM, 0);
      if(_socket == -1)
      {
        cerr<< " socket error " << errno << strerror(errno) <<endl;
        exit(SOCKET_ERR);
      }

      // 2 int bind(int sockfd, const struct sockaddr *addr,  socklen_t addrlen);
      // sockfd文件描述符
      struct sockaddr_in local;
      bzero(&local, sizeof(local));
      local.sin_family = AF_INET;
      
      
    }

    void start()
    {
    }
    ~udpServe()
    {
    }

  private:
    uint16_t _port;
    std::string _ip;
    int _socket;
  };

}
