#pragma once
#include <iostream>
#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define NUM 1024
using namespace std;

class TcpClient
{
public:
    TcpClient(const std::string& serverip, uint16_t serverport)
        : _sock(-1), _serverip(serverip), _serverport(serverport)
    {
    }

    // 初始化客户端
    void initClient()
    {
        // 1. 创建套接字
        _sock = socket(AF_INET, SOCK_STREAM, 0);
        if (_sock < 0)
        {
            std::cerr << "socket create error" << std::endl;
            exit(2);
        }
        // 客户端无需 bind，只需要 connect
    }

    // 启动 client
    bool start()
    {
        struct sockaddr_in server;
        memset(&server, 0, sizeof(server));
        server.sin_family = AF_INET;
        server.sin_port = htons(_serverport);
        server.sin_addr.s_addr = inet_addr(_serverip.c_str());

        // connect
        if (connect(_sock, (struct sockaddr*)&server, sizeof(server)) != 0)
        {
            std::cerr << "socket connect error" << std::endl;
            return false;
        }

        // 成功连接后开始消息收发
        string msg;
        char buffer[NUM];

        while (true)
        {
            cout << "Enter # ";
            std::getline(std::cin, msg);

            // 写入服务器
            ssize_t s = write(_sock, msg.c_str(), msg.size());
            if (s <= 0)
            {
                cerr << "write failed" << endl;
                break;
            }

            // 读回显
            ssize_t n = read(_sock, buffer, sizeof(buffer) - 1);
            if (n > 0)
            {
                buffer[n] = '\0';
                cout << "Server回显# " << buffer << endl;
            }
            else if (n == 0)
            {
                cout << "Server 关闭连接" << endl;
                break;
            }
            else
            {
                cerr << "read failed" << endl;
                break;
            }
        }

        return true;
    }

    ~TcpClient()
    {
        if (_sock >= 0) close(_sock);
    }

private:
    int _sock;
    std::string _serverip;
    uint16_t _serverport;  // 修正类型
};
