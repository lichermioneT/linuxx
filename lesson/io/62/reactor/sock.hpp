#pragma once 

#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>

const static int backlog = 32;
const static int defaultsock = -1;

class Sock 
{
private:
  int _listensock;
public:
  Sock()
    :_listensock(-1)
  {}

  ~Sock()
  {
    if(_listensock != defaultsock)
    {
      close(_listensock);
      _listensock = -1;
    }
  }

public:
  void Socket()
  {
    _listensock = socket(AF_INET, SOCK_STREAM, 0);
    if(_listensock < 0)
    {
      std::cout<< "_listensock failed" <<std::endl;
      exit(-1);
    }
    std::cout<< "sock succdess" <<std::endl;
    
    int opt = 1;
    setsockopt(_listensock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
  }

  void Bind(uint16_t port)
  {
    struct sockaddr_in local;
    memset(&local, 0, sizeof(local));

    local.sin_family = AF_INET;
    local.sin_addr.s_addr = htonl(INADDR_ANY);
    local.sin_port = htons(port);

    if(bind(_listensock, (struct sockaddr*)&local, sizeof(local)) < 0)
    {
      std::cout<< "bind failed" << std::endl;
      exit(-1);
    }
    std::cout<< "bind succdess" <<std::endl;
  }

  void Listen()
  {
    if(listen(_listensock, backlog) < 0)
    {
      std::cout<< "listen failed" << std::endl;
      exit(-1);
    }
    std::cout<< "listen succdess" <<std::endl;
  }

  int Accept(std::string* clientip, uint16_t* clientport)
  {
    struct sockaddr_in peer;
    socklen_t len = sizeof(peer);

    int sock = accept(_listensock, (struct sockaddr*)&peer, &len);
    if(sock < 0)
    {
      std::cout << "accept failend" << std::endl;
      exit(-1);
    }
    
    *clientip = inet_ntoa(peer.sin_addr);
    *clientport = ntohs(peer.sin_port);
    
    std::cout<< "accept succdess" <<std::endl;
    return sock;
  }

  int fd()
  {
    return _listensock;
  }
};
