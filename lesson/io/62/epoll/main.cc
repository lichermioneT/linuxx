#include "epollServer.hpp"
#include <memory>

using namespace std;
using namespace epoll_ns;

std::string transaction(const std::string& request)
{
  return request;
}


int main()
{
  unique_ptr<epollServer> svr (new epollServer());
  svr->init();
  svr->start();

  return 0;
}
