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
#include <cstring>
using namespace std;

namespace client
{
  class udpClient
  {
  public:
    udpClient(const string& serverip, const uint16_t& port) : _socket(-1),_serverip(serverip), _serverport(port),_quit(false) 
    {}

    void initClient()
    {
// 1.创建套接字
      _socket = socket(AF_INET, SOCK_DGRAM, 0);
      if (_socket == -1)
      {
        cerr << " socket error " << errno << strerror(errno) << endl;
        exit(1);
      }
// client不需要显示的绑定。客户端有一个端口号就行的。
// 服务器是一家公司，客户端是无数家公司。万一app打架，怎么办呢。 OS进行bind。OS在什么时候，

    }

    void run()
    {
      struct sockaddr_in server;
      memset(&server, 0, sizeof(server));
      server.sin_family = AF_INET;
      server.sin_addr.s_addr = inet_addr(_serverip.c_str());
      server.sin_port        = htons(_serverport);

      string message;
      while(!_quit)
      {
        cout<< "please enter#";
        cin>>message;
        sendto(_socket, message.c_str(), message.size(), 0, (struct sockaddr*)&server, sizeof(server));
      }
    }
    ~udpClient()
    {
    }

  private:
    int _socket;
    string _serverip;
    uint16_t _serverport;
    bool _quit;
  };

}
