#include "client.hpp"
#include <memory>

static void usage(string proc)
{
  cout<< "Usage: "<< proc << "server_ip server_port" <<endl;
}

//1.创建socket
//2.发送信息
//3.接收返回的信息

// ./client serverip serverport
int main(int agrc, char* argv[])
{
  if(agrc != 3)
  {
    usage(argv[0]);
    exit(1);
  }
  
  string serverip = argv[1];
  uint16_t serverport = atoi(argv[2]);

  unique_ptr<client> ucli (new client(serverip, serverport));
  ucli->init();
  ucli->run();

  return 0;
}
