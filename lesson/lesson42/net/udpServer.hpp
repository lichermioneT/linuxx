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
const static string defaultIp = "0.0.0.0"; 
enum {SOCKET_ERR = 1};
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
      

      //2 创建成功了 绑定ip和port
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

      struct sockaddr_in local; // 定义了一个变量，栈，用户
      bzero(&local, sizeof(local)); 
      local.sin_family = AF_INET;
      local.sin_port   = htons(_port);   // 你给别人发消息，你的port和ip要不要发送给对方 。要的
      local.sin_addr.s_addr = _ip;       // 1.string-->uint32_t, 2.htonl();

      int n = bind(_sockfd, &local, )



    }

    void start()
    {

    }
    ~udpServe()
    {

    }


  private:
    uint16_t _port;
    std::string _ip;
    int _sockfd;
  };
}

