#pragma once

#include <netinet/in.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <cstring>
#include <functional>
#include <unistd.h>
#include <stdlib.h>
#include <cerrno>
#include <iostream>
#include <string>
using namespace std;

enum {USAGE_ERR, SOCKET_ERR = 2, BIND_ERR};

const static string defaultIp = "0.0.0.0";
class server
{
typedef function<void(int, string, uint16_t, string)> func_t;
public:
  server(func_t callback,const uint16_t& port, const string& ip = defaultIp)
    :_callback(callback)
    ,_socketfd(-1)
    ,_port(port)
    ,_ip(ip)
  {}
  
  void init()
  {
    // 1.file discriptor
    _socketfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(_socketfd == -1)
    {
      cerr<< "socket error <<" << errno << " : " << strerror(errno) << endl;
      exit(SOCKET_ERR);
    }

    // 2. bind:AF_INET + port+ip 
    struct sockaddr_in local; // 栈区定义一个变量。bind就是为了设置进内核里面的。
    bzero(&local, sizeof(local)); // 注意清零的

    local.sin_family = AF_INET;
    local.sin_port = htons(_port); // 你给你给别发信息，需不需要发送ip+port呢？ 大小端转换的。
    /*
     *local.sin_addr.s_addr = inet_addr(_ip.c_str());
     */
    local.sin_addr.s_addr = htonl(INADDR_ANY); // 全零的地址，任意地址bind。访问我主机就行(端口要对的)
    
    int n = bind(_socketfd, (struct sockaddr*)&local, sizeof(local));
    if(n == -1)
    {
      cerr<< "bind error <<" << errno << " : " << strerror(errno) << endl;
      exit(BIND_ERR);
    }
  }

  void start()
  {
    // 服务器的本质就是死循环的。
    
    char buff[1024] = {0};
    for(;;)
    {
       sleep(2); 
       struct sockaddr_in peer; 
       socklen_t len = sizeof(peer);
       // 这里-1我们把数据当做字符串的了。
       ssize_t s = recvfrom(_socketfd, buff, sizeof(buff) - 1, 0, (struct sockaddr*)&peer, &len); 
       if(s > 0)
       {
         buff[s] = 0;
         // 1.peer的数据是网络序列的。2.int还需转换成点分十进制的。
        string clientip = inet_ntoa(peer.sin_addr); 
        uint16_t clientport = ntohs(peer.sin_port);
        string message = buff;

        cout<< "客户端ip ：" << clientip << " 端口：" <<  clientport << " 发送的信息是：" << message <<endl;
        // 对数据进行处理的。
        
        _callback(_socketfd, clientip, clientport, message);

       }
       
    }
  }

  ~server()
  {}

private:
  func_t _callback;
  int _socketfd;
  uint16_t _port; // 服务器的端口号
  string _ip;     // 服务器的ip地址, 实际上一款网络服务器不建议指明一个IP地址。
};
