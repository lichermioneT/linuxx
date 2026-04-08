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
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock == -1)
    {
      perror("socket");
      return -1;
    }
    
    std::cout<< "1.socket success" <<std::endl;
    int opt = 1;
    int n = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    if(n == -1)
    {
      perror("setsockopt");
      return -1;
    }
    return sock;
  }

  static void Bind(int sock, uint16_t port)
  {
    struct sockaddr_in local;
    bzero(&local, sizeof(local));

    local.sin_family = AF_INET;
    local.sin_addr.s_addr = htonl(INADDR_ANY);
    local.sin_port = htons(port);

    int n = bind(sock, (struct sockaddr*)&local, sizeof(local));
    if(n == -1)
    {
      perror("bind");
      return;
    }

    std::cout<< "2.bind success" << std::endl;
  }

  static void Listen(int sock)
  {
    int n = listen(sock, 128);
    if(n == -1)
    {
      perror("listen");
      return;
    }
    
    std::cout<< "3.listen success" << std::endl;
  }

  static int Accept(int listensock, std::string* clientip, uint16_t* clientport)
  {
    struct sockaddr_in peer;
    socklen_t len = sizeof(peer);
    memset(&peer, 0, sizeof(peer));

    int sock  = accept(listensock, (struct sockaddr*)&peer, &len);
    if(sock == -1)
    {
      perror("accept");
      return -1;
    }
    
    *clientip = inet_ntoa(peer.sin_addr);
    *clientport = ntohs(peer.sin_port);

    std::cout<< "accept success" << std::endl;
    
    return sock;
  }
};
