#include "udpServer.hpp"
#include <memory>
using namespace server;


// usage
// ./udpserver ip port
// getopt

void Usage(string proc)
{
  cout<< "Usage " << proc << "local_port" <<endl;
}

int main(int argc, char* argv[])
{

  if(argc != 2)
  {
    Usage(argv[0]);
    exit(USAGE_ERR);
  }

  uint16_t port = atoi(argv[1]);

  std::unique_ptr<udpServe> usvr(new udpServe(port));

  usvr->init();
  usvr->start();

  return 0;
}
