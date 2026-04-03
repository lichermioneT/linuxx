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
    _sock = socket(AF_INET, SOCK_STREAM, 0);
    if(_sock < 0)
    {
      cout<< "socket failed" <<endl;
      return false;
    }
    
    return true;
  }


  bool Connect()
  {
    struct sockaddr_in peer;
    peer.sin_family = AF_INET;
    if(inet_pton(AF_INET, _ip.c_str(), &peer.sin_addr.s_addr) < 0)
    {
      cout<< "inet_pton failde" << endl;
      return false;
    }
    peer.sin_port = htons(_port);
    
    if(connect(_sock, (struct sockaddr*)&peer, sizeof(peer)) < 0)
    {

      perror("connect failed");
      return false;
    }
    
    char buffer[1024] = {0};

    while(1)
    {
      cout<<"请输入";
      cin>>buffer;

      send(_sock, buffer, sizeof(buffer), 0);
    }
    
  }


private:
  string _ip;
  uint16_t _port;
  int _sock;
};
