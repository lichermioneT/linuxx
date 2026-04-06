#include "epollServer.hpp"
#include <memory>

using namespace std;
using namespace epoll_ns;

std::string transaction(const std::string& request)
{
  return  "i a epoll_ns" + request;
}

int main()
{
  unique_ptr<epollServer> svr (new epollServer(transaction));
  svr->init();
  svr->start();

  return 0;
}
