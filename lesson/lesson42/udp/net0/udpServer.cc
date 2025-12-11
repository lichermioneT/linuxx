#include "udpServer.hpp"
#include <memory>

using namespace std;
using namespace Serve;

static void Usage(string proc)
{
  cout<< "Usage:\n \t  "<< proc << "  local_port\n\n";
}

// ./udpServe port 
//  getopt()

int main(int argc, char* argv[])
{
  // cout << "Program started, argc=" << argc << endl;  // 新增：确认 main 执行
  if(argc != 2)
  {
    Usage(argv[0]);
    exit(USAGE_ERR);
  }

  uint16_t port = atoi(argv[1]);
  // cout << "Port parsed: " << port << endl;  // 新增：确认参数
  // string ip = argv[1];

  std::unique_ptr<udpServe> usvr(new udpServe(port));
  usvr->initserve();
  usvr->start();

  return 0;
}
