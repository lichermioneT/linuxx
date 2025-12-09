#include "udpClient.hpp"
#include <memory>
#include <string>
using namespace Client;

// ./udpClient server_ip server_port
//             发给谁

static void Usage(string proc)
{
  cout<< "Usage:\n\t"<< proc << " server_ip  server_port \n\n";
}

int main(int argc, char* argv[])
{
  if(argc != 3)
  {
    Usage(argv[0]);
    exit(1);
  }
  
  string serverip = argv[1];            // 服务端的ip地址
  uint16_t serverport = atoi(argv[2]);  // 服务端的port号

  unique_ptr<udpClient> ucli(new udpClient(serverip, serverport)); // 发给

  ucli->initClient();
  ucli->run();

  return 0;
}
