#include "HttpServer.hpp"
#include <memory>

using namespace std;
using namespace server;
       
void Usage(std::string proc)
{
    cerr << "Usage:\n\t" << proc << " port\r\n\r\n";
}
// 1. 服务器和网页分离，html
// 2. url -> / : web根目录

bool Get(const HttpRequest &req, HttpResponse &resp)
{
    cout << "----------------------http start---------------------------" << endl;
    cout << req.inbuffer << std::endl;
    cout << "----------------------http end---------------------------" << endl;
    return true;
}

// ./httpServer 8080
int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        Usage(argv[0]);
        exit(0);
    }
    uint16_t port = atoi(argv[1]);
    unique_ptr<HttpServer> httpsvr(new HttpServer(Get, port));


    httpsvr->initServer();
    httpsvr->start();
    return 0;
}