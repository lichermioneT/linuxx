#include "server.hpp"
#include <memory>


int main(int argc, char* argv[])
{
  
  uint16_t port = atoi(argv[1]);
  unique_ptr<server> ser(new server(port));
  
  ser->init();
  ser->start();





  return 0;
}
