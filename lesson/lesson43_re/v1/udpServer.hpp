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
#include <netinet/in.h>

using namespace std;
namespace server
{
  static const std::string defaultIp = "0.0.0.0";
  static const int gnum = 1024;
  enum {USAGE_ERR,SOCKET_ERR = 2, BIND_ERR};

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
      // 一般只需要绑定port, ip绑定0.0.0.0。
      // sockfd文件描述符
      struct sockaddr_in local;
      bzero(&local, sizeof(local));
      local.sin_family = AF_INET;
      local.sin_port   = htons(_port);                              // 给别人发消息，你的port和ip发不发？ 发送的。 字节序列 host to net short
      // local.sin_addr.s_addr =  inet_addr(_ip.c_str());           // 1.string->uint32_t 2.htonl()
      local.sin_addr.s_addr =  htonl(INADDR_ANY);                   // 任意地址bind

      int n = bind(_socket, (struct sockaddr*)&local, sizeof(local));
      if(n == -1)
      {
        cerr<< " bind error " << errno << strerror(errno) <<endl;
      }
// UDP服务器就此完成了。
// 服务器的端口是众所周知的

    }

    void start()
    {
// 服务器的本质就是一个死循环。常驻内存进程。OS netstat
      char buffer[gnum] = {0};
      for(;;)
      {
        // ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags, struct sockaddr *src_addr, socklen_t *addrlen);
        // 消息本身和消息的发送者。
        struct sockaddr_in peer;
        socklen_t len = sizeof(peer);
        ssize_t s = recvfrom(_socket, buffer, sizeof(buffer)-1,  0, (struct sockaddr*)&peer, &len);
        //1.数据是什么，2.谁发的
        if(s > 0)
        {
          buffer[s]  = 0;
          string clientip = inet_ntoa(peer.sin_addr); // 1.网络序列 2.int->电分十进制。
          uint16_t clientport = ntohs(peer.sin_port);
          string message = buffer;
          cout<< clientip << "[" << clientport << "]# " << message <<endl;
        }
      }

    }
    ~udpServe()
    {
    }

  private:
    uint16_t _port;
    std::string _ip;  // 实际上一款服务器，不建议指明一个IP地址。
    int _socket;
  };

}
