#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

int main()
{
  int sock = socket(AF_INET, SOCK_DGRAM, 0);
  if(sock == -1)
  {
    perror("socket failed");
    return -1;
  }

  struct sockaddr_in server;
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons(8080);

  int n = bind(sock, (struct sockaddr*)&server, sizeof(server));
  if(n == -1)
  {
    perror("bind failed");
    return -1;
  }

  char buffer[1024];
  struct sockaddr_in peer;
  socklen_t len = sizeof(peer);
  while(1)
  {
    n = recvfrom(sock, buffer, sizeof(buffer)-1, 0, (struct sockaddr*)&peer, &len);
    buffer[n] = 0;
    printf("receive message %s\n", buffer);
  }  






  return 0;
}
