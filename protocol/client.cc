#include "client.hpp"
#include <memory>
 
// 1.服务端的请求
// 2.请求的反序列化
// 3.构建相应
// 4.相应序列化
// 5.发送

int main(int argc, char* argv[])
{
  (void)argc;
  string serverip = argv[1]; 
  uint16_t serverport = atoi(argv[2]);

  unique_ptr<client> cli(new client(serverip, serverport));
  cli->init();
  cli->start();

  return 0;
}
