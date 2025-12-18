#include <iostream>
#include <memory>
#include "udpClient.hpp"
using namespace std;
using namespace client;
//./udpclient server_ip server_port
void Usage(string proc)
{
  cout<< "Usage " << proc << "server_ip  server_port" <<endl;
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
