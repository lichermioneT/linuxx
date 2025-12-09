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
#include <ifaddrs.h>

using namespace std;

namespace Serve 
{
static const  string defaultIp = "0.0.0.0";  // 默认IP地址
static const  int gnum = 1024; // 读取数据的缓冲区

enum {USAGE_ERR = 1, SOCKET_ERR, BIND_ERR};

void showLocalIPs()
{
    struct ifaddrs *addrs, *tmp;
    getifaddrs(&addrs);
    tmp = addrs;

    while (tmp)
    {
        if (tmp->ifa_addr && tmp->ifa_addr->sa_family == AF_INET)
        {
            struct sockaddr_in *p = (struct sockaddr_in *)tmp->ifa_addr;
            string ip = inet_ntoa(p->sin_addr);
            cout << tmp->ifa_name << ": " << ip << endl;
        }
        tmp = tmp->ifa_next;
    }
    freeifaddrs(addrs);
}


  class udpServe
  {
  public:
    udpServe(const uint16_t& port, const std::string& ip = defaultIp) // 服务器的端口号，和IP地址
      :_port(port)
      ,_ip(ip)
      ,_sockfd(-1)
    {}

    void initserve()
    {
      // int socket(int domain, int type, int protocol); 
      // domain     本地还是网络通信,           协议族，什么寻址方式  
      // type       我们的socket提供的能力类型  套接字类型
      // protocol   一般缺省0，（TCP/UDP通信）  协议类型
      // int        file descriptor,文件描述符
      // 创建一个网络“文件”，后续所有网络通信都通过这个文件描述符进行。

// 1.创建套接字（打开一个网络文件）
      _sockfd = socket(AF_INET, SOCK_DGRAM, 0);  // AF_INET:IPV4, SOCK_DGRAM:UDP, 
      if(_sockfd == -1)
      {
        cerr<< " socket error " << errno << " : " <<  strerror(errno) <<endl;
        exit(SOCKET_ERR);  // 失败了，我们就不玩了。
      }
      
      cout<< "socket success: " << " : 网络文件描述符：" << _sockfd <<endl;

//2 创建成功了 绑定ip和port
// bind() 的作用：把一个 socket 绑定到一个 IP + 端口。
// 这个端口由我来监听（接收数据）

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
      
      // 结构体之间的相互赋值？？ c语言是不能的。cpp可以的。

      struct sockaddr_in local;             // 定义了一个变量，栈，用户,这里结构体不省略的。
      bzero(&local, sizeof(local));         // 数据清零。 准备工作完成的。 void bzero(void *s, size_t n);
  
      local.sin_family = AF_INET;         // 协议                                                                         协议
      local.sin_port   = htons(_port);    // 你给别人发消息，你的port和ip要不要发送给对方 。要的 。网络序列                    端口
      local.sin_addr.s_addr = inet_addr(_ip.c_str());       // 1.string-->uint32_t, 2.htonl(); inet_addr()函数 。         IP地址 

// in_addr_t inet_addr(const char *cp);
// inet_addr() 用于 把点分十进制的 IPv4 地址字符串转换成网络字节序的 32 位整数。

      // 建议写法
      // local.sin_addr.s_addr = htonl(INADDR_ANY); // 说白了全零,任意地址bind，服务器的真实写法
      
      int n = bind(_sockfd, (struct sockaddr*)&local, sizeof(local));
      if(n == -1)
      {
        cerr<< " bind error " << errno << " : " <<  strerror(errno) <<endl;
        exit(BIND_ERR);  // 失败了，我不跟你玩了的
      }


     showLocalIPs();
      //UDP Serve的预备工作完成。
    }

// 127.0.0.1本地环回。服务器代码的测试
// 
    void start()
    {
      // 服务器的本质就是一个死循环
      // OS就是一个
      char buffer[gnum];        
      for(;;)
      {
        struct sockaddr_in peer;    // 输出型参数，谁发送的信息。
        socklen_t len = sizeof(peer);
        ssize_t s = recvfrom(_sockfd, buffer, sizeof(buffer) - 1, 0, (struct sockaddr*)&peer, &len); // 0阻塞等待
// 阻塞等待，直到有一个 UDP 报文到达，然后把内容复制到 buffer，并把“发送者的地址”放入 peer 中。

        //1.数据是什么。前面
        //2.谁发的      后面
// ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags, struct sockaddr *src_addr, socklen_t *addrlen);
// 最后两个数据，输出型参数。谁发的数据。        
        if(s > 0)
        {
          buffer[s] = 0;
          string clientip = inet_ntoa(peer.sin_addr);               // 网络序列，int->点分十进制IP。客户端的ip地址值
          uint16_t clientport = ntohs(peer.sin_port);               // 网络序列到主机              客户端的port。
          string message = buffer;                                  // 数据

          // cout<< clientip << " [" << clientport << "] #" << message <<endl;
          cout << "[Client] IP=" << clientip 
               << " Port=" << clientport 
               << " Message=\"" << message << "\""
               << endl;
        }
      }
    }

    ~udpServe()
    {
    }
  private:
    uint16_t _port;    // 服务端口号
    std::string _ip;   // 实际上，一款网络服务器，不建议指明一个IP。
    int _sockfd;       // 网络文件描述符
  };
}