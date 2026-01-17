#include "udpserver.hpp"
#include <memory>

using namespace std;
using namespace server;

static void usage()
{
  cout<< "usage ：" << "./udpserver local_port" <<endl;
}

void handlerMessage(string clientip, uint16_t clientport, string message)
{
  cout<<"hello client" <<endl;
}


int main(int agrc, char* argv[])
{
  if(agrc != 2)
  {
    usage();
    exit(1);
  }

  uint16_t port = atoi(argv[1]);

  std::unique_ptr<udpServe> usvr(new udpServe(handlerMessage, port));
  usvr->init();
  usvr->start();

  return 0;
}
