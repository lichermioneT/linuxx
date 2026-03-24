#include "httpserver.hpp"
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
    // for test
    cout << "----------------------http start---------------------------" << endl;  // 浏览器的请求
    cout << req.inbuffer << std::endl;                                              // 总体数据
    std::cout << "method: " << req.method << std::endl;                             // 请求的方法
    std::cout << "url: " << req.url << std::endl;                                   // 请求的连接
    std::cout << "httpversion: " << req.httpversion << std::endl;                   // 请求的版本
    std::cout << "path: " << req.path << std::endl;                                 // 请求的路径
    cout << "----------------------http end---------------------------" << endl;

// 开始构建返回了
    std::string respline = "HTTP/1.1 200 OK\r\n";
    std::string respheader = "Content-Type: text/html\r\n";

    std::string respblank = "\r\n";
    // std::string body = "<html lang=\"en\"><head><meta charset=\"UTF-8\"><title>for test</title><h1>hello world</h1></head><body><p>北京交通广播《一路畅通》“交通大家谈”节目，特邀北京市交通委员会地面公交运营管理处处长赵震、北京市公安局公安交通管理局秩序处副处长 林志勇、北京交通发展研究院交通规划所所长 刘雪杰为您解答公交车专用道6月1日起社会车辆进出公交车道须注意哪些？</p></body></html>";
     std::string body = R"(
                        <html lang="en">
                        <head>
                            <meta charset="UTF-8">
                            <title> test </title>
                            <h1>hello world</h1>
                        </head>
                        <body>
                            <p>北京交通广播《一路畅通》"交通大家谈"节目，特邀北京市交通委员会地面公交运营管理处处长赵震、北京市公安局公安交通管理局秩序处副处长 林志勇、北京交通发展研究院交通规划所所长 刘雪杰为您解答公交车专用道6月1日起社会车辆进出公交车道须注意哪些？</p>
                        </body>
                        </html>
                        )";
    resp.outbuffer += respline;
    resp.outbuffer += respheader;
    resp.outbuffer += respblank;
    resp.outbuffer += body;

    return true;
}

// ./httpServer 8080
int main()
{

    uint16_t port = 8080;
    unique_ptr<HttpServer> httpsvr(new HttpServer(Get, port));

    httpsvr->initServer();
    httpsvr->start();
    return 0;
}
