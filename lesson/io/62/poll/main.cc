#include "pollServer.hpp"
#include <memory>

using namespace std;
using namespace poll_ns;

std::string transaction(const std::string& request)
{
  return request;
}


int main()
{
  unique_ptr<poll_server> svr (new poll_server(transaction));
  svr->init();
  svr->start();

  return 0;
}
