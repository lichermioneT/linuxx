#include "selectServer.hpp"
#include <memory>

using namespace std;
using namespace select_ns;



int main()
{
  unique_ptr<select_server> svr (new select_server());
  svr->init();
  svr->start();

  return 0;
}
