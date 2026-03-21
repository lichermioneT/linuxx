#include "server.hpp"
#include <memory>

// 1.socket注意是全局变量的
int main(int argc, char* argv[])
{
  if(argc != 2)
  {
    cout<< "参数输入有误" <<endl;           
    exit(1);
  }

  uint16_t port = atoi(argv[1]);
  server se(port);
  se.init();
  se.run();
  return 0;
}
