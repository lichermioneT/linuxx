#pragma once

#include <iostream>
#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "log.hpp"

namespace server
{
enum {USAGE_ERR = 1, SOCKET_ERR, BIND_ERR, LISTEN_ERR};
static const uint16_t gport = 8080;
static const int gbacklog = 5;

class TcpServer
{
public:
    TcpServer(const uint16_t& port = gport)
        : _listensock(-1), _port(port)
    {}

    void initServer()
    {
        // 1. 创建套接字
        _listensock = socket(AF_INET, SOCK_STREAM, 0);
        if(_listensock < 0)
        {
            logMessage(FATAL, "create socket failed");
            exit(SOCKET_ERR);
        }
        logMessage(NORMAL, "create socket success");

        // 2. 绑定IP和PORT
        struct sockaddr_in local;
        memset(&local, 0, sizeof(local));
        local.sin_family = AF_INET;
        local.sin_port = htons(_port);             // 正确：htons
        local.sin_addr.s_addr = htonl(INADDR_ANY); // 绑定到任意网卡

        if(bind(_listensock, (struct sockaddr*)&local, sizeof(local)) < 0)
        {
            logMessage(FATAL, "bind failed");
            exit(BIND_ERR);
        }
        logMessage(NORMAL, "bind success");

        // 3.设置socket，为监听状态
        if(listen(_listensock, gbacklog) < 0)
        {
            logMessage(FATAL, "listen failed");
            exit(LISTEN_ERR);
        }
        logMessage(NORMAL, "listen success");
    }

    void start()
    {
        // TODO: listen + accept
        for(;;)
        {
            // 4.server获取新链接
            // accept返回值为什么也是文件描述符
            // sock和client进行通信的fd
            sockaddr_in peer;
            socklen_t len = sizeof(peer);
            int sock = accept(_listensock, (struct sockaddr*)&peer, &len);
            if(sock < 0)
            {
                logMessage(ERROR, "accept error, next");
                continue;
            }

            logMessage(NORMAL,"accept a new link success");
            cout<< "sock: " << sock <<endl;

            //5.用sock,进行通信。
            serverIO(sock);
        }
    }

    void serverIO(int sock)
    {
        
    }


private:
    int _listensock; // 不是用来通信的，用来监听链接，获取新链接
    uint16_t _port;
};

}
