#include "tcpClient.hpp"
#include <memory>
using namespace std;

static void Usage(string proc)
{
  cout<< "Usage:\n \t  "<< proc << "  serverip serverpor \n\n";
}

// ./tcpclient serverip serverport
int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        Usage(argv[0]);
        exit(1);
    }

    string serverip = argv[1];
    uint16_t serverport = atoi(argv[2]);
    unique_ptr<TcpClient>  tcli(new TcpClient(serverip, serverport));
    tcli->initClient();
    tcli->start();

// 一般而言，tcp确实在查找的时候建立链接成功，只会有一条链接！
// 今天呢，我们做测试，clietn和server在一台机器上的。


    return 0;
}