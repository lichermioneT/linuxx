#include "udpClient.hpp"
#include <memory>
#include <string>
using namespace Client;

// ./udpClient server_ip server_port
//             发给谁

static void Usage(string proc)
{
  cout<< "Usage:\n\t"<< proc << "  local_port\n\n";
}

int main(int argc, char* argv[])
{
  if(argc != 3)
  {
    Usage(argv[0]);
    exit(1);
  }
  
  string serverip = argv[1];
  uint16_t serverport = atoi(argv[2]);
  unique_ptr<udpClient> ucli(new udpClient(serverip, serverport));


  ucli->initClient();
  ucli->run();







  return 0;
}
