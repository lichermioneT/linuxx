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


int main()
{
  //1.创建套接字
  int sock = socket(AF_INET, SOCK_DGRAM, 0); 
  if(sock == -1)
  {
    return -1;
  }

  struct sockaddr_in server;
  memset(&server, 0, sizeof(server));
  server.sin_family = AF_INET;
  server.sin_port   = htons(8080);
  server.sin_addr.s_addr  = inet_addr("127.0.0.1");
  socklen_t len = sizeof(server);

  char buffer[1024];
  while(true)
  {
    cout<<"enter# ";
    cin>>buffer;
  int n = sendto(sock, buffer, sizeof(buffer), 0, (struct sockaddr*)&server, len);
  if(n > 0)
  {
    buffer[n] = 0;
  }
  cout<< buffer <<endl;
  }









  return 0;
}

