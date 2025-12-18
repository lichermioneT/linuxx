#include "tcpServer.hpp"
#include <memory>

using namespace server;
using namespace std;

static void Usage(string proc)
{
  cout<< "Usage:\n \t  "<< proc << " local_port\n\n";
}

// tcp和udp启动一模一样的
// ./tcpserver port 
int main(int argc, char* argv[])
{

  if(argc != 2)
  {
    Usage(argv[0]);
    exit(USAGE_ERR);
  }

  uint16_t port = atoi(argv[1]);
  unique_ptr<TcpServer> tsvr(new TcpServer());

  tsvr->initServer(); // 初始化
  tsvr->start();      // 启动服务器
  return 0;
}