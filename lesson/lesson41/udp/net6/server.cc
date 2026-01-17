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

  int sock = socket(AF_INET, SOCK_DGRAM, 0);
  if(sock == -1)
  {
    return -1;
  }

  struct sockaddr_in local;
  local.sin_family = AF_INET;
  local.sin_port = htons(8080);
  local.sin_addr.s_addr = htonl(INADDR_ANY);

  int n = bind(sock, (struct sockaddr*)&local, sizeof(local));
  if(n == -1)
  {
    return -1;
  }
  
  char buffer[1024];
  struct sockaddr_in peer;
  socklen_t len = sizeof(peer);
  while(true)
  {
  ssize_t s = recvfrom(sock, buffer, sizeof(buffer) - 1, 0,(struct sockaddr*)&peer, &len);
  if(s > 0)
  {
    buffer[s] = 0;
    cout<< buffer <<endl;
  }
  }







  return 0;
}
