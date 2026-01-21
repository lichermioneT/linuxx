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
  server.sin_addr.s_addr = inet_addr("127.0.0.1");
  server.sin_port = htons(8080);

  char buffer[1024];
  while(1)
  {
    if(fgets(buffer, sizeof(buffer), stdin)!= NULL)
    sendto(sock, buffer, sizeof(buffer)-1, 0, (struct sockaddr*)&server, sizeof(server));
  }  






  return 0;
}
