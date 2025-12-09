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
    _sockfd = socket(AF_INET, SOCK_DGRAM, 0); // 服务端的套接字
    if(_sockfd == -1)
    {
      cerr<< "socket error: " << errno << " : " << strerror(errno) <<endl; 
      exit(1);
    }

    cout<< "socket success: " << " : " << _sockfd <<endl;

    //client要不要bind[必须要的]
    //client要不要明确的(程序员自己)bind[不需要（因为OS）]
    //写服务器的是一家公司，写client是无数家公司。--由OS自动形成端口进行bind!--OS在什么时候，如何bind。只要有端口号就行的。
    //什么时候绑定的。
  }

  void run()
  {
    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(_serverip.c_str());  // 服务器的IP地址
    server.sin_port = htons(_serverport);                   // 服务器的port号

    string message;
    while(!_quit)
    {
      cout<< "Please Enter# ";
      cin>>message;
      sendto(_sockfd, message.c_str(), message.size(), 0, (struct sockaddr*)&server, sizeof(server)); // 发送给谁的

      char buffer[1024];
      struct sockaddr_in temp;
      socklen_t temp_len=sizeof(temp);
      size_t n = recvfrom(_sockfd,buffer, sizeof(buffer)-1,0,(struct sockaddr*)&temp,&temp_len);
      if(n > 0) buffer[n] = 0;
      cout<<"服务器的翻译结果#"<<buffer<<endl;
    }
  }

  ~udpClient()
  {
  }

private:
  int _sockfd;          // 通过套接字发送
  string _serverip;     // 服务端的ip
  uint16_t _serverport; // 服务端port
  bool _quit;
};
  









}

