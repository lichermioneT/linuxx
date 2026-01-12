#pragma once

#include <iostream>
#include "sock.hpp"

namespace select_ns
{
    static const int defaultport = 8081;
    static const int fdnum = sizeof(fd_set) * 8;
    static const int defaultfd = -1;

    class SelectServer
    {
    public:
        SelectServer(int port = defaultport) : _port(port), _listensock(-1), fdarray(nullptr)
        {}
        
        void Print()
        {
            std::cout << "fd list: ";
            for(int i = 0;i < fdnum;i++) 
            {
                if(fdarray[i] != defaultfd) std::cout << fdarray[i] << " ";
            }
            std::cout << std::endl;
        }
        void HandlerEvent(fd_set &rfds)
        {
            //? 目前一定是listensock，只有这一个
            if (FD_ISSET(_listensock, &rfds))
            {
                // 走到这里，accept 函数，会不会阻塞？？？1 0  不会的
                // select 告诉我， listensock读事件就绪了
                std::string clientip;
                uint16_t clientport = 0;
                int sock = Sock::Accept(_listensock, &clientip, &clientport); // accept = 等 + 获取
                if (sock < 0)
                    return;
                logMessage(NORMAL, "accept success [%s:%d]", clientip.c_str(), clientport);
                // sock我们能直接recv/read 吗？不能，整个代码，只有select有资格检测事件是否就绪
                // 将新的sock 托管给select！
                // 将新的sock托管给select的本质，其实就是将sock，添加到fdarray数组中即可！
                int i = 0;
                for(; i < fdnum; i++)
                {
                    if(fdarray[i] != defaultfd) 
                        continue;
                    else 
                        break;
                }
                if(i == fdnum)
                {
                    logMessage(WARNING, "server if full, please wait");
                    close(sock);
                }
                else
                {
                    fdarray[i] = sock;
                }
                Print();
            }
        }

        void initServer()
        {
            _listensock = Sock::Socket();
            Sock::Bind(_listensock, _port);
            Sock::Listen(_listensock);

            fdarray = new int[fdnum];
            for (int i = 0; i < fdnum; i++)
                fdarray[i] = defaultfd;   // defaultfd = -1;
            fdarray[0] = _listensock;     // 不变了
        }

// 链接就绪了，你再来叫我。
        void start()
        {
            for (;;)
            {
                fd_set rfds;         // 读文件描述符集合
                FD_ZERO(&rfds);      // 全部初始化为零的。
                int maxfd = fdarray[0];

                for (int i = 0; i < fdnum; i++)
                {
                    if (fdarray[i] == defaultfd)
                        continue;
                    FD_SET(fdarray[i], &rfds); // 合法 fd 全部添加到读文件描述符集中

                    if(maxfd < fdarray[i]) maxfd = fdarray[i]; // 更新所有fd中最大的fd
                }

                // struct timeval timeout = {1, 0};
                // int n = select(_listensock + 1, &rfds, nullptr, nullptr, &timeout); // ??
                // 一般而言，要是用select，需要程序员自己维护一个保存所有合法fd的数组！

                int n = select(maxfd + 1, &rfds, nullptr, nullptr, nullptr); // ??
                switch (n)
                {
                // 没有在特定时间，发生任何事情了。
                case 0:
                    logMessage(NORMAL, "timeout...");
                    break;
                case -1:
                    logMessage(WARNING, "select error, code: %d, err string: %s", errno, strerror(errno));
                    break;
                default:
                    // 说明有事件就绪了,目前只有一个监听事件就绪了
                    logMessage(NORMAL, "get a new link...");
                    HandlerEvent(rfds);
                    break;
                }
                // std::string clientip;
                // uint16_t clientport = 0;
                // int sock = Sock::Accept(_listensock, &clientip, &clientport); // accept = 等 + 获取
                // if(sock<0) continue;
                // // 开始进行服务器的处理逻辑
            }
        }
        ~SelectServer()
        {
            if (_listensock < 0)
                close(_listensock);
            if (fdarray) delete []fdarray;
        }

    private:
        int _port;
        int _listensock;
        int *fdarray;    // 用于保存 所有“合法、需要被监控的 fd”，这是 select 模型的本质要求。
    };
}
