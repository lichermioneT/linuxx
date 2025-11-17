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

using namespace std;

namespace Serve 
{
static const  string defaultIp = "0.0.0.0"; 
static const  int gnum = 1024;
using namespace std;

enum {USAGE_ERR = 1, SOCKET_ERR, BIND_ERR};
  class udpServe
  {
  public:
    udpServe(const uint16_t& port, const std::string& ip = defaultIp)
      :_port(port)
      ,_ip(ip)
      ,_sockfd(-1)
    {}

    void initserve()
    {
      // int socket(int domain, int type, int protocol); 
      // domain     本地还是网络通信  
      // type       我们的socket提供的能力类型
      // protocol   一般缺省0，（TCP/UDP通信）
      // int        file descriptor,文件描述符
      
      // 1.创建套接字
      _sockfd = socket(AF_INET, SOCK_DGRAM, 0);
      if(_sockfd == -1)
      {
        cerr<< " socket error " << errno << " : " <<  strerror(errno) <<endl;
        exit(SOCKET_ERR);
      }
      
      cout<< "socket success: " << " : " << _sockfd <<endl;

      //2 创建成功了 绑定ip和port
// 未来服务器要明确的port，不能随意改变的
      // int band(int sockfd, const struct sockaddr* addr, socket_t addrlen);
      // sockfd 
      // const struct sockaddr* addr
      // socklen_t addrlen
      // ## c语言的双井号
      // uint32_t ip  点分十进制风格ip,字符串，可读性号。
      // 192.169.1.00 整数风格ip 网络通信。
        
      // struct _ip 
      // {
      //    unsigned char p1; 
      //    unsigned char p2; 
      //    unsigned char p3; 
      //    unsigned char p4; 
      // }
      //
      
      // 结构体之间的相互赋值？？

      struct sockaddr_in local; // 定义了一个变量，栈，用户
      bzero(&local, sizeof(local)); 
      local.sin_family = AF_INET;
      local.sin_port   = htons(_port);   // 你给别人发消息，你的port和ip要不要发送给对方 。要的
      local.sin_addr.s_addr = inet_addr(_ip.c_str());       // 1.string-->uint32_t, 2.htonl(); inet_addr()函数
      // 建议写法
      //local.sin_addr.s_addr = htonl(INADDR_ANY); // 说白了全零,任意地址bind，服务器的真实写法
      
      
      int n = bind(_sockfd, (struct sockaddr*)&local, sizeof(local));
      if(n == -1)
      {
        cerr<< " bind error " << errno << " : " <<  strerror(errno) <<endl;
        exit(BIND_ERR); 
      }
     
      //UDP Serve的预备工作完成
    }

    void start()
    {
      // 服务器的本质就是一个死循环
      char buffer[gnum];        
      for(;;)
      {
        struct sockaddr_in peer;
        socklen_t len = sizeof(peer);
        ssize_t s = recvfrom(_sockfd, buffer, sizeof(buffer)-1, 0, (struct sockaddr*)&peer, &len);
        //1.数据是什么
        //2.谁发的
        if(s > 0)
        {
          buffer[s] = 0;
          string clientip = inet_ntoa(peer.sin_addr);               // 网络序列，int->点分十进制
          uint16_t clientport = ntohs(peer.sin_port);
          string message = buffer;

          cout<< clientip << "[" << clientport << "]#" << message <<endl;
        }
      }
    }
    ~udpServe()
    {

    }


  private:
    uint16_t _port;
    std::string _ip;  // 实际上，一款网络服务器，不建议指明一个IP
    int _sockfd;
  };
}

