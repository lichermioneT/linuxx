#include "udpServer.hpp"
#include <memory>

using namespace std;
using namespace Serve;


static void Usage(string proc)
{
  cout<< "Usage:\n\t"<< proc << "  local_port\n\n";
}

// ./udpServe ip port 
int main(int argc, char* argv[])
{
  if(argc != 2)
  {
    Usage(argv[0]);
    exit(USAGE_ERR);
  }

  uint16_t port = atoi(argv[1]);
  //string ip = argv[1];

  std::unique_ptr<udpServe> usvr(new udpServe(port));
  usvr->initserve();
  usvr->start();










  return 0;
}
