#pragma once

#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>

namespace server
{
enum {USAGE_ERR = 1};
static const uint16_t gport = 8080;

class TcpServer
{
public:
    TcpServer(const uint16_t& port = gport):_sock(-1),_port(gport)
    {}
    void initServer()
    {
        //1.创建套接字
        _sock = socket(AF_INET, SOCK_STREAM, 0);
        if(_sock < 0)
        {

        }
    }
    void start()
    {

    }


private:
    int _sock;      // 服务器需要套接字
    uint16_t _port; // 服务器需要指定端口信息、

};

}







