#pragma once 
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>

class Sock 
{
public:
  static int Socket()
  {
    int  sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0)
    {
      std::cout<< "socket failed" <<std::endl;
      exit(-1);
    }
    std::cerr<< "1. sock succdess" <<std::endl;
    
    int opt = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    return sock;
  }

  static void Bind(int sock, uint16_t port)
  {
    struct sockaddr_in local;
    memset(&local, 0, sizeof(local));

    local.sin_family = AF_INET;
    local.sin_addr.s_addr = htonl(INADDR_ANY);
    local.sin_port = htons(port);

    if(bind(sock, (struct sockaddr*)&local, sizeof(local)) < 0)
    {
      std::cerr<< "bind failed" << std::endl;
      exit(-1);
    }
    std::cout<< "2. bind succdess" <<std::endl;
  }

  static void Listen(int sock)
  {
    if(listen(sock, 128) < 0)
    {
      std::cerr<< "listen failed" << std::endl;
      exit(-1);
    }
    std::cout<< "3. listen succdess" <<std::endl;
  }

  static int Accept(int listensock, std::string* clientip, uint16_t* clientport)
  {
    struct sockaddr_in peer;
    socklen_t len = sizeof(peer);

    int sock = accept(listensock, (struct sockaddr*)&peer, &len);
    if(sock < 0)
    {
      std::cerr << "accept failend" << std::endl;
      exit(-1);
    }
    
    *clientip = inet_ntoa(peer.sin_addr);
    *clientport = ntohs(peer.sin_port);
    
    std::cout<< "4. accept succdess" <<std::endl;
    return sock;
  }
};
