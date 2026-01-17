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
#include <pthread.h>
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

  static void *readMessage(void *args)
  {
      int sockfd = *(static_cast<int*>(args));
      pthread_detach(pthread_self());

      char buffer[1024];
      struct sockaddr_in temp;
      socklen_t temp_len = sizeof(temp);
      
      while (true)
      {
          size_t n = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0, (struct sockaddr *)&temp, &temp_len);
          if (n >= 0)
          {
              buffer[n] = 0;
              cout << buffer << endl;
          }
      }
      return nullptr;
  }

  void run()
  {
    pthread_create(&_reader, nullptr, readMessage, (void*)&_sockfd);

    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(_serverip.c_str());
    server.sin_port = htons(_serverport);

    string message;
    char cmdlinne[1024];
    while(!_quit)
    {

      fprintf(stderr, "enter# ");
      fflush(stderr);
      fgets(cmdlinne, sizeof(cmdlinne), stdin);
      cmdlinne[strlen(cmdlinne)] = 0;
      message = cmdlinne;

      sendto(_sockfd, message.c_str(), message.size(), 0, (struct sockaddr*)&server, sizeof(server));
      
      // char buffer[1024];
      // struct sockaddr_in temp;
      // socklen_t temp_len = sizeof(temp);
      // size_t n = recvfrom(_sockfd, buffer, sizeof(buffer)-1, 0, (struct sockaddr*)&temp, &temp_len);
      // if(n>0) buffer[n] = 0;
      // cout<< buffer <<endl;
    }
  }
  ~udpclient()
  {}
private:
  string _serverip;
  uint16_t _serverport;
  int _sockfd;
  bool _quit;
  pthread_t _reader;
};
}
