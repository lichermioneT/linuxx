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
void handlerMessage(int sockfd, string clientip, uint16_t clientport, string message)
{
    // 就可以对message进行特定的业务处理，而不关心message怎么来的 ---- server通信和业务逻辑解耦！
    // 婴儿版的业务逻辑
    string response_message = message;
    response_message += " [server echo] ";

    // 开始返回
    struct sockaddr_in client;
    bzero(&client, sizeof(client));

    client.sin_family = AF_INET;
    client.sin_port = htons(clientport);
    client.sin_addr.s_addr = inet_addr(clientip.c_str());

    sendto(sockfd, response_message.c_str(), response_message.size(), 0, (struct sockaddr*)&client, sizeof(client)); // 消费返回去
}

int main(int argc, char* argv[])
{
  if(argc != 2)
  {
    Usage(argv[0]);
    exit(USAGE_ERR);
  }

  uint16_t port = atoi(argv[1]);
  std::unique_ptr<udpServe> usvr(new udpServe(handlerMessage, port));

  usvr->initserve();
  usvr->start();

  return 0;
}
