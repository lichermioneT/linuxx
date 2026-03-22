#include "client.hpp"





int main(int argc, char* argv[])
{
  if(argc != 3)
  {
    cout<< "./client ip port";
  }
 
  string ip = argv[1];
  uint16_t port = atoi(argv[2]);
  
  client cli(ip, port);
  cli.init();
  cli.start();
  


  return 0;
}
