#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <iostream>
#include <string>
#include <cstring>

class Sock
{
// 1.创建监听套接字
public:
  static int Socket()
  {
    int listensock = socket(AF_INET, SOCK_STREAM, 0);
    if(listensock == -1)
    {
      std::cerr<< "socket error : " << strerror(errno) << std::endl;
      return -1;
    }

    int opt = 1; 
    int n = setsockopt(listensock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    if(n == -1)
    {
      std::cerr<< "setsockopt error : " << strerror(errno) << std::endl;
      close(listensock);
      return -1;
    }
    
    return listensock;
  }

// 2.绑定套接字
  static bool Bind(int listensock,  uint16_t port)
  {
    struct sockaddr_in local;
    bzero(&local, sizeof(local));

    local.sin_family = AF_INET;
    local.sin_addr.s_addr = htonl(INADDR_ANY);
    local.sin_port  = htons(port);

    int n = bind(listensock,(struct sockaddr*)&local, sizeof(local));
    if(n == -1)
    {
      std::cerr<< "bind error : " << strerror(errno) << std::endl;
      return false;
    }

    return true;
  }

// 3.开始监听套接字
  static bool Listen(int listensock)
  {
    int n = listen(listensock, 128);
    if(n == -1)
    {
      std::cerr<< "listen error : " << strerror(errno) << std::endl;
      return false; 
    }

    return true;
  }

// 4.获取新的链接到来
  static int Accept(int listensock, std::string* ip, uint16_t* port)
  {
    struct sockaddr_in peer;
    memset(&peer, 0, sizeof(peer));
    socklen_t len = sizeof(peer);

    int sock = accept(listensock, (struct sockaddr*)&peer, &len);
    if(sock == -1)
    {
      std::cerr<< "accept errno : " << strerror(errno) <<std::endl; 
      return -1;
    }

    char ip_str[INET_ADDRSTRLEN] = {0} ;
    
    const char* p = inet_ntop(AF_INET, &peer.sin_addr, ip_str, sizeof(ip_str));
    if(p == nullptr)
    {
      std::cerr<< "inet_ntop errno : " << strerror(errno) <<std::endl;
      close(sock);
      return -1;
    }
    
    *ip = ip_str;
    *port = ntohs(peer.sin_port);

    return sock;
  }
};
