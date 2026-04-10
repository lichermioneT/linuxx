#include "server.h"
#include <stdio.h>

int main(int argc, char* argv[])
{
  if(argc != 3)
  {
    printf("usage: ./a.out port dir\n");
    return -1;
  }
  
  uint16_t port = atoi(argv[1]); 
  
  if(chdir(argv[2]) == 1)
  {
    perror("chdir");
    return -1;
  }


  int lfd = initListenFd(port);
  if(lfd == -1)
  {
    perror("initListenFd");
    return -1;
  }
  
  epollRun(lfd);

  return 0;
}
