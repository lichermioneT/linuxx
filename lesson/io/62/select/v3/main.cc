#include "select.hpp"

std::string transaction(const std::string& request)
{
  return request;
}

int main()
{
  
  select_server s(transaction);
  s.init();
  s.start();




  return 0;
}
