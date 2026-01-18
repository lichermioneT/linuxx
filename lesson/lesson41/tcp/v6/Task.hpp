#pragma once

#include <iostream>
#include <string>
#include <cstdio>
#include <functional>
using namespace std; 

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
        close(sock);
    }

class Task
{
    using func_t = std::function<void(int)>;
public:
    Task()
    {}
    Task(int sock, func_t func)
    :_sock(sock), _callback(func)
    {}
    void operator()()
    {
        _callback(_sock);
    }
private:
    int _sock;
    func_t _callback;
};

