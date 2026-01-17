#pragma once

#include <iostream>
#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "log.hpp"
#include <unistd.h>
using namespace std;

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
        _listensock = socket(AF_INET, SOCK_STREAM, 0);  // TCP是面向字节流的 SOCK_STREAM
        if(_listensock < 0)
        {
            logMessage(FATAL, "create socket failed");
            exit(SOCKET_ERR);
        }
        logMessage(NORMAL, "create socket success");

// 2. 绑定IP和PORT
        struct sockaddr_in local;
        memset(&local, 0, sizeof(local));          // 置零 因为结构体对其，可能会填充的。
        local.sin_family = AF_INET;                // AF_INET = PF_INET
        local.sin_port = htons(_port);             // 正确：htons   主机转网络 这里是用户序列 需要主机转网络
        local.sin_addr.s_addr = htonl(INADDR_ANY); // 绑定到任意网卡 主机转网络

        if(bind(_listensock, (struct sockaddr*)&local, sizeof(local)) < 0)  // 设置近内核里面
        {
            logMessage(FATAL, "bind failed");
            exit(BIND_ERR);
        }
        logMessage(NORMAL, "bind success");

// 3.设置socket，为监听状态。 这里和udp不一样的。这里是面向连接的，必须连接的。
        // 监听者
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
            
            // return value file discriptor
            // 吃鱼的故事，招呼了人的小二。
/*
    总结一下： accept() 成功一次，就产生一个专门负责该连接的新 FD。
    原来的监听 FD 依然屹立不倒，继续等待下一个幸运儿。
*/
            int sock = accept(_listensock, (struct sockaddr*)&peer, &len);
            if(sock < 0)
            {
                logMessage(ERROR, "accept error, next");
                continue;
            }

            logMessage(NORMAL,"accept a new link success");
            cout<< "sock: " << sock <<endl;               //  print a new file discriptor

            //5.用new sock,进行通信。面向字节流的。后续全部都是文件操作。
            serverIO(sock); // callback function
            close(sock);    // 已经使用完的sock，必须关闭，要不然会导致,文件描述符泄漏。这里没有进行并发，一次只能链接一个的。
            // 后面高并发的
        }

    }

    void serverIO(int sock)
    {
        char buffer[1024];
        while(true)
        {
            ssize_t n = read(sock, buffer, sizeof(buffer) - 1); // 目前当做字符串 这里是读
            if(n > 0)
            {
                // 目前我们把我们读到的数据当做字符串，截至目前
                buffer[n] = 0;
                cout<< "recv message : " << buffer <<endl;

                std::string outbuffer = buffer;
                outbuffer += "  server[echo]";

                write(sock, outbuffer.c_str(), outbuffer.size()); // 这里是写回去的
            }
            else if(n == 0) 
            {
                // 客户端退出了
                logMessage(NORMAL, "client quit, me to");
                break;
            }
        }
    }

private:
    int _listensock;   // 不是用来通信的，用来监听链接，获取新链接
    uint16_t _port;    // 服务器指定端口号
};

}
