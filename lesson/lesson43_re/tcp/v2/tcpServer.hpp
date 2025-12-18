#pragma once
#include <iostream>
#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/wait.h>
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
    TcpServer(const uint16_t& port = gport): _listensock(-1), _port(port)
    {}

    void initServer()
    {
// 1. 创建套接字
        _listensock = socket(AF_INET, SOCK_STREAM, 0); // TCP套接字
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
        local.sin_port = htons(_port);             // 正确：htons   port
        local.sin_addr.s_addr = htonl(INADDR_ANY); // 绑定到任意网卡 pi地址

        if(bind(_listensock, (struct sockaddr*)&local, sizeof(local)) < 0) // 交给操作系统
        {
            logMessage(FATAL, "bind failed");
            exit(BIND_ERR);
        }
        logMessage(NORMAL, "bind success");

// 3.设置socket，为监听状态
        // tcp是面向连接的
        if(listen(_listensock, gbacklog) < 0)  // gbacklog暂时说不清楚 listen等客户端
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
            // accept返回值为什么也是套接字
            // sock和client进行通信的fd
            // 吃鱼拉客
            sockaddr_in peer;
            socklen_t len = sizeof(peer);
            
            int sock = accept(_listensock, (struct sockaddr*)&peer, &len); // _listensock 不是用来通信的，用来监听链接，获取新链接
            if(sock < 0)
            {
                logMessage(ERROR, "accept error, next");
                continue;
            }

            logMessage(NORMAL,"accept a new link success");
            cout<< "sock: " << sock <<endl;               //  print a new file discriptor

//5.用new sock,进行通信。面向字节流的。后续全部都是文件操作。

            // dome1
            // serverIO(sock); // callback function
            // close(sock);  // 已经使用完的sock，必须关闭，要不然会导致,文件描述符泄漏。这里没有进行并发，一次只能链接一个的。

            //dome2
            // 多进程。
            pid_t id = fork();
            if (id == 0) // child process
            {
                // 子进程对外提供服务，不需要的文件描述符关闭了,节约文件描述符资源的
                close(_listensock);

                if(fork() > 0) exit(0); // 

                serverIO(sock); 
                close(sock);    
                exit(0);   // child process exit
            }

            // father
            // wait child process
            pid_t ret = waitpid(id, nullptr, 0);
            if(ret > 0)
            {
                cout<< "wait success:" << ret <<endl;
            }
        }
    }

    void serverIO(int sock)
    {
        char buffer[1024];
        while(true)
        {
            ssize_t n = read(sock, buffer, sizeof(buffer) - 1);  // 读数据 目前当做字符串
            if(n > 0)
            {
                // 目前我们把我们读到的数据当做字符串，截至目前
                buffer[n] = 0;
                cout<< "recv message : " << buffer <<endl;

                std::string outbuffer = buffer;
                outbuffer += "server[echo]";

                write(sock, outbuffer.c_str(), outbuffer.size()); // 写数据 (多路转接)
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
    uint16_t _port;
};

}
