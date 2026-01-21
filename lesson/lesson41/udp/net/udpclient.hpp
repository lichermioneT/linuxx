#pragma once 
#include <iostream>
#include <string>
#include <cerrno>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstdlib>
#include <arpa/inet.h>
#include <netinet/in.h>
using namespace std;


namespace client 
{
class udpclient
{
public:
  udpclient(const string& serverip, const uint16_t& serverport)
    :_serverip(serverip)
    ,_serverport(serverport)
    ,_sockfd(-1)
    ,_quit(false)
  {}

  void init()
  {
      //1.文件描述符。
      _sockfd = socket(AF_INET, SOCK_DGRAM, 0); //1.通信协议，2.字节流和数据报，3.协议 AF_INET IPV4  SOCK_DGRAM数据报。默认0
      if(_sockfd == -1)
      {
        cerr<< "socket error " << "  : " << strerror(errno) <<endl;
        exit(1);
      }
      //客户端必须bind，不需要显示的bind。 存在即可。
      //写服务器是一家公司，客户端是无数家公司。
      //os帮你合理安排端口资源
      //什么时候bind
  }

  void run()
  {
    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(_serverip.c_str()); // in_addr_t internet address(const char*) 主机到网络字节序号
    server.sin_port = htons(_serverport);                  // 同样这里也是主机字节序到网络字节序 

    string message;
    while(!_quit)
    {
      cout<<"message enter#";
      cin>>message;

      //UDP协议需要使用sendto, 每次发送带上服务端的信息
      sendto(_sockfd, message.c_str(), message.size(), 0, (struct sockaddr*)&server, sizeof(server)); // udp不是面向连接的，所以每次都必须指明地址的。
      /*
       *简单说：因为 UDP 是无连接的，操作系统不知道你要把数据发给谁，所以每次发送都必须明确指定接收方的地址信息。
       */
    }
  }

  ~udpclient()
  {}

private:
  string _serverip;
  uint16_t _serverport;
  int _sockfd;
  bool _quit;
};
}
