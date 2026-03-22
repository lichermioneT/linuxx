#include "client.hpp"
#include <memory>



int main(int argc, char* argv[])
{

  string serverip = argv[1]; 
  uint16_t serverport = atoi(argv[2]);

  unique_ptr<client> cli(new client(serverip, serverport));
  cli->init();
  cli->start();

  return 0;
}
