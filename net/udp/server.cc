#include "server.hpp"


int main()
{

  server ser(8080);
  ser.CreateSocket();
  ser.Bind();
  ser.Recv();


  return 0;
}
