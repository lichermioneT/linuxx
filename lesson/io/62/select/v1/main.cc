#include "selectServer.hpp"
#include <memory>

using namespace std;
using namespace select_ns;

std::string transaction(const std::string& request)
{
  return request;
}

int main()
{
  unique_ptr<select_server> svr (new select_server(transaction));
  svr->init();
  svr->start();
  return 0;
}
