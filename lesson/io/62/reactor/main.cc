#include "tcpserver.hpp"
#include <memory>

using namespace tcpserver;

int main(int argc, char* argv[])
{
  if(argc != 2)
  {
    std::cout<< "usage: ../a.out port" << std::endl;
    return -1;
  }
  
  uint16_t port = atoi(argv[1]);

  std::unique_ptr<Tcpserver> tser(new Tcpserver(port));
  tser->init();    // 通过listensock添加新的链接
  tser->Dispatcher(); // 等待链接事件的就绪

  return  0;
}
