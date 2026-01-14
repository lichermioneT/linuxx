#pragma once

#include <iostream>
#include <string>
#include <functional>
#include "sock.hpp"

namespace select_ns
{
    static const int defaultport = 8081;
    static const int fdnum = sizeof(fd_set) * 8;
    static const int defaultfd = -1;

    using func_t = std::function<std::string (const std::string&)>;

    class SelectServer
    {
    public:
        SelectServer(func_t f, int port = defaultport) : func(f), _port(port), _listensock(-1), fdarray(nullptr)
        {
        }
        void initServer()
        {
            _listensock = Sock::Socket();
            Sock::Bind(_listensock, _port);
            Sock::Listen(_listensock);
            fdarray = new int[fdnum];
            for (int i = 0; i < fdnum; i++)
                fdarray[i] = defaultfd;
            fdarray[0] = _listensock; // 不变了
        }
        void Print()
        {
            std::cout << "fd list: ";
            for (int i = 0; i < fdnum; i++)
            {
                if (fdarray[i] != defaultfd)
                    std::cout << fdarray[i] << " ";
            }
            std::cout << std::endl;
        }
        void Accepter(int listensock)
        {
            logMessage(DEBUG, "Accepter in");
            // 走到这里，accept 函数，会不会阻塞？？？1 0
            // select 告诉我， listensock读事件就绪了
            std::string clientip;
            uint16_t clientport = 0;
            int sock = Sock::Accept(listensock, &clientip, &clientport); // accept = 等 + 获取
            if (sock < 0)
                return;
            logMessage(NORMAL, "accept success [%s:%d]", clientip.c_str(), clientport);
            // sock我们能直接recv/read 吗？不能，整个代码，只有select有资格检测事件是否就绪
            // 将新的sock 托管给select！
            // 将新的sock托管给select的本质，其实就是将sock，添加到fdarray数组中即可！
            int i = 0;
            for (; i < fdnum; i++)
            {
                if (fdarray[i] != defaultfd)
                    continue;
                else
                    break;
            }
            if (i == fdnum)
            {
                logMessage(WARNING, "server if full, please wait");
                close(sock);
            }
            else
            {
                fdarray[i] = sock;
            }
            Print();
            logMessage(DEBUG, "Accepter out");
        }
        void Recver(int sock, int pos)
        {
            logMessage(DEBUG, "in Recver");

            // 1. 读取request
            // 这样读取是有问题的！
            char buffer[1024];
            ssize_t s = recv(sock, buffer, sizeof(buffer) - 1, 0); // 这里在进行读取的时候，会不会被阻塞？1， 0
            if (s > 0)
            {
                buffer[s] = 0;
                logMessage(NORMAL, "client# %s", buffer);
            }
            else if (s == 0)
            {
                close(sock);
                fdarray[pos] = defaultfd;
                logMessage(NORMAL, "client quit");
                return;
            }
            else
            {
                close(sock);
                fdarray[pos] = defaultfd;
                logMessage(ERROR, "client quit: %s", strerror(errno));
                return;
            }

            // 2. 处理request
            std::string response = func(buffer);

            // 3. 返回response
            // write bug
            write(sock, response.c_str(), response.size());

            logMessage(DEBUG, "out Recver");
        }
        // 1. handler event rfds 中，不仅仅是有一个fd是就绪的，可能存在多个
        // 2. 我们的select目前只处理了read事件
        void HandlerReadEvent(fd_set &rfds)
        {
            for (int i = 0; i < fdnum; i++)
            {
                // 过滤掉非法的fd
                if (fdarray[i] == defaultfd)
                    continue;

                // 正常的fd
                // 正常的fd不一定就绪了
                // 目前一定是listensock，只有这一个
                if (FD_ISSET(fdarray[i], &rfds) && fdarray[i] == _listensock)
                    Accepter(_listensock);
                else if(FD_ISSET(fdarray[i], &rfds))
                    Recver(fdarray[i], i);
                else{}
            }
        }

        void start()
        {
            for (;;)
            {
                fd_set rfds;
                // fd_set wfds;
                FD_ZERO(&rfds);
                int maxfd = fdarray[0];

                for (int i = 0; i < fdnum; i++)
                {
                    if (fdarray[i] == defaultfd)
                        continue;
                    FD_SET(fdarray[i], &rfds); // 合法 fd 全部添加到读文件描述符集中

                    if (maxfd < fdarray[i])
                        maxfd = fdarray[i]; // 更新所有fd中最大的fd
                }
                logMessage(NORMAL, "max fd is: %d", maxfd);
                // struct timeval timeout = {1, 0};
                // int n = select(_listensock + 1, &rfds, nullptr, nullptr, &timeout); // ??
                // 一般而言，要是用select，需要程序员自己维护一个保存所有合法fd的数组！
                int n = select(maxfd + 1, &rfds, nullptr, nullptr, nullptr); // ??
                switch (n)
                {
                case 0:
                    logMessage(NORMAL, "timeout...");
                    break;
                case -1:
                    logMessage(WARNING, "select error, code: %d, err string: %s", errno, strerror(errno));
                    break;
                default:
                    // 说明有事件就绪了,目前只有一个监听事件就绪了
                    logMessage(NORMAL, "have event ready!");
                    HandlerReadEvent(rfds);
                    // HandlerWriteEvent(wfds);
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
            if (fdarray)
                delete[] fdarray;
        }

    private:
        int _port;
        int _listensock;
        int *fdarray;
        func_t func;
    };
}