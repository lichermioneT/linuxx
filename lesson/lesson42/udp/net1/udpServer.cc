#include "udpServer.hpp"
#include <unordered_map>
#include <memory>
#include <fstream>

using namespace std;
using namespace Serve;

static void Usage(string proc)
{
  cout<< "Usage:\n \t  "<< proc << "  local_port\n\n";
}

// ./udpServe port 
//  getopt()

unordered_map<string, string> dict;
const std::string dictTxt="./dict.txt";



int main(int argc, char* argv[])
{
  if(argc != 2)
  {
    Usage(argv[0]);
    exit(USAGE_ERR);
  }

  uint16_t port = atoi(argv[1]);
  std::unique_ptr<udpServe> usvr(new udpServe(handlerMessage, port));
  initDict();
//   debugPrint();

  usvr->initserve();
  usvr->start();

  return 0;
}
