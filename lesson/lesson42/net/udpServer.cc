#include "udpServer.hpp"
#include <memory>
using namespace Serve;

int main()
{

  std::unique_ptr<udpServe> usvr(new udpServe());


  usvr->initserve();
  usvr->start();








  return 0;
}
