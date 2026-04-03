#include "udpserver.hpp"


int main(int argc, char* argv[])
{
  if(argc != 2)
  {
    cout<< "usage: ./a.out port" <<endl;
    return -1;
  }
  
  uint16_t port = atoi(argv[1]);

  udpserver ser(port);
  ser.init();
  ser.start();
  
  return 0;
}
