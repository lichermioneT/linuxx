#pragma once 

#include <iostream>
#include <string>
#include <cstring>
#include <strings.h>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

namespace Client 
{
using namespace std;  

class udpClient
{
public:
  udpClient(const string& serverip, const uint16_t& serverport)
    :_sockfd(-1)
    ,_serverip(serverip)
    ,_serverport(serverport)
    ,_quit(false)
  {
  }

  void initClient()
  {
    _sockfd = socket(AF_INET, SOCK_DGRAM,0);
    if(_sockfd == -1)
    {
     cerr<< "socket error: " << errno << " : " << strerror(errno) <<endl; 
     exit(1);
    }

    cout<< "socket success: " << " : " << _sockfd <<endl;
    //client要不要bind[必须要的]
    //client要不要显示的(程序员自己)bind[]
    //写服务器的是一家公司，写client是无数家公司。--由OS自动形成端口进行bind!--OS在什么时候，如何bind
  }

  void run()
  {
    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(_serverip.c_str());
    server.sin_port = htons(_serverport);

    string message;
    while(!_quit)
    {
      cout<< "Please Enter# ";
      cin>>message;
      sendto(_sockfd, message.c_str(), message.size(), 0,(struct sockaddr*)&server, sizeof(server));
    }
  }


  ~udpClient()
  {

  }


private:
  int _sockfd;
  string _serverip;
  uint16_t _serverport;
  bool _quit;
};
  









}

