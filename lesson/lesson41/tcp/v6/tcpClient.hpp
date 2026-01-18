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
    {}

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
        //tcp和先udp都是一样的
        // 客户端无需显示 bind，只需要 connect
        // os自动随机管理端口资源
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
        // 发起链接给_listensock发消息，我们来到了的
        // 客户端就是发起请求的，accept就接收到，并创建一个新的文件描述符。
        if (connect(_sock, (struct sockaddr*)&server, sizeof(server)) != 0)
        {
            std::cerr << "socket connect error" << std::endl;
            return false;
        }
        else 
        {
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
        }
        return true;
    }

    ~TcpClient()
    {
        if (_sock >= 0) close(_sock);
    }
private:
    int _sock;
    std::string _serverip; // 服务器的数据
    uint16_t _serverport;  // 修正类型
};
