#include "client.hpp"



//1.服务端
//  1.socket
//  2.bind
//  3.recvfrom
//  4.sendto
//
//2.客户端
//  1.socket
//  2.sendto
//  3.recvfrom

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
