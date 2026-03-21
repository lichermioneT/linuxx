#pragma  once 
#include <netinet/in.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <cstring>
#include <unistd.h>
#include <stdlib.h>
#include <cerrno>
#include <iostream>
#include <string>
#include <pthread.h>
using namespace std;

class client
{
public:
  client(const string& ip, uint16_t& port)
    :_socketfd(-1)
    ,_serverip(ip)
    ,_serverport(port)
  {}

  void init()
  {
    // 1.创建套接字
    _socketfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(_socketfd == -1)
    {
      cerr<< "socket error <<" << errno << " : " << strerror(errno) << endl;
      exit(-1);
    }
   
    // 2.需要bind的，不需要显示的bind的。
    // OS自动帮你bind的，帮你随机找一个port的。 有就行了的。 唯一性的就行的。
    // 在什么时候进行bind呢？ sendto的时候bind  
  }
  
  static void* readMessage(void* args)
  {
      int sockfd = *(static_cast<int*>(args));
      pthread_detach(pthread_self());
      while(true)
      {
        char buffer[1024];
        struct sockaddr_in temp;
        socklen_t temp_len = sizeof(temp);
        size_t n = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0, (struct sockaddr*)&temp, &temp_len);
        if(n > 0) buffer[n] = 0;
        cout<< "服务器的翻译结果：" << buffer <<endl;
      }

      return nullptr;
  }


  void run()
  {
    
    pthread_create(&_reader, nullptr, readMessage, (void*)&_socketfd);

    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(_serverip.c_str());
    server.sin_port = htons(_serverport);

    string message;
    char buffer[1024] = {0};
    while(1)
    {
      fprintf(stderr, "enter#");
      fflush(stderr);
      fgets(buffer, sizeof(buffer), stdin);
      buffer[strlen(buffer) - 1] = 0;

      message = buffer;
      sendto(_socketfd, message.c_str(), message.size(), 0, (struct sockaddr*)&server, sizeof(server));
      
    }
  }

  ~client()
  {}
private:
  int _socketfd;
  string _serverip;
  uint16_t _serverport;
  pthread_t _reader;
};
