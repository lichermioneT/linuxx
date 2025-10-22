#include "comm.hpp"


int main()
{
  std::cout<< "cliend begin" <<std::endl;
  int rfd = open(NAME_PIPE, O_WRONLY);
  std::cout<< "cliend end" <<std::endl;
  if(rfd < 0) exit(1);
  
  char buffer[1024];
  while(true)
  {
    std::cout<< " please say# ";
    fgets(buffer, sizeof(buffer), stdin);
    if(strlen(buffer) > 0)
    {
      buffer[strlen(buffer) - 1] = 0;
    }
    ssize_t n =write(rfd,buffer, strlen(buffer));

    assert(n == strlen(buffer));
    (void)n;
  }
  close(rfd);
  return 0;
}
