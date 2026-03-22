#include "client.hpp"
#include <memory>


// 1.tcp的服务端
//   1.socket
//   2.bind
//   3.listen, 监听总的socket,
//   4.accept  监听到的消息需要知道谁的，输入输出型参数的
//   5.revc/read 
//   5.send/write
// 2.tcp的客户端
//   1.socket
//   2.不需要显示的bind。
//   3.connect.建立链接
//   4.通信的。

int main(int argc, char* argv[])
{

  string serverip = argv[1]; 
  uint16_t serverport = atoi(argv[2]);

  unique_ptr<client> cli(new client(serverip, serverport));
  cli->init();
  cli->start();

  return 0;
}
