#include "server.hpp"



int main()
{

  server ser(8080);
  ser.init();
  ser.start();
  return 0;
}
