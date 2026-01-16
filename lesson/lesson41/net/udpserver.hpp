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
#include <functional>
using namespace std;

namespace server
{
  const static string defaultIp = "0.0.0.0"; //todo
  static const int gnum = 1024;
  enum {USAGE_ERR, SOCKET_ERR = 2, BIND_ERR};
  
  typedef function<void (string, uint16_t,string)> func_t;

class udpServe
{
public:
  udpServe(const func_t& cb, const uint16_t &port, const std::string &ip = defaultIp)
    :_callback(cb), _port(port),_ip(ip),_sockfd(-1)                 // 初始化服务器的 ip+port
  {}

  void init()
  {   
      // 返回值就是一个文件描述符, 就是一个open函数，类似的。
      // 打开一个文件描述符号
      
      //1.文件描述符。
      _sockfd = socket(AF_INET, SOCK_DGRAM, 0); //1.通信协议，2.字节流和数据报，3.协议 AF_INET IPV4  SOCK_DGRAM数据报。默认0
      if(_sockfd == -1)
      {
        cerr<< "socket error " << "  : " << strerror(errno) <<endl;
        exit(SOCKET_ERR);
      }
  
      // 把文件描述符和ip+port链接起来的 。
      //2.bind ip+port 
      struct sockaddr_in local; // 定义了一个变量
      bzero(&local, sizeof(local)); // 涉及结构体对其，然后填充零。
      
      // 三件套赋值
      local.sin_family  = AF_INET;
      local.sin_port = htons(_port); // 主机序列到网络序列 服务器绑定port.我是主机需要传输到网络里面去的。
      /*
       *local.sin_addr.s_addr = inet_addr(_ip.c_str());     // 1.string->uint32_t    2.htonl()
       */
      
      local.sin_addr.s_addr = htonl(INADDR_ANY); // 任意地址bind。

      int n = bind(_sockfd, (struct sockaddr*)&local, sizeof(local));
      if(n == -1)
      {
        cerr<< "bind error " << "  : " << strerror(errno) <<endl;
        exit(BIND_ERR);
      }
//UDP服务器正式完成
  }

  void start()
  {
// 服务器的本质就是一个死循环，常驻内存的进程。OS
  
    char buffer[gnum];
    for(;;)
    {
      // recvfrom 读取数据 知道谁发送的
      struct sockaddr_in peer;
      socklen_t len = sizeof(peer);      
      ssize_t s =recvfrom(_sockfd, buffer, sizeof(buffer)-1, 0, (struct sockaddr*)&peer, &len);
      //1.数据是什么 2.谁发的的
      
      if(s > 0)
      {
        buffer[s] = 0;
        string clientip = inet_ntoa(peer.sin_addr);  // 网络到主机 点分十进制
        uint16_t clientport = ntohs(peer.sin_port);
        string message = buffer;

         cout<< clientport << ":" <<  clientip << ":" << message <<endl; 
        _callback(clientip, clientport, message);
      }

    }


  }
  ~udpServe()
  {


  }
private:
  func_t _callback;
  uint16_t _port;
  std::string _ip; // 不建议绑定一款特定的ip
  int _sockfd;
};
}
