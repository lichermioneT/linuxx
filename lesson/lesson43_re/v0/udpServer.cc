#include "udpServer.hpp"
#include <memory>
using namespace server;

int main()
{

  std::unique_ptr<udpServe> usvr(new udpServe(0));
  usvr->init();
  usvr->start();








  return 0;
}
