#pragma once
#include <iostream>
#include <string>
#include <cstring>
#include <cerrno>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
using namespace std;

class server
{
public:
  server(uint16_t port)
    :_port(port)
    ,_sock(-1)
  {}
  
  ~server()
  {
    if(_sock >= 0 )
    {
      close(_sock);
    }
  }

// 1.创建有个网络通信的文件描述符
  bool CreateSocket()
  {
    _sock = socket(AF_INET, SOCK_STREAM, 0);
    if(_sock < 0)
    {
      
      cout<< "socket failed" <<endl;
      return false;
    }
  
    cout<< "socket success" <<endl;
    return true;
  }

// 2.网络文件描述符和服务器地址和端口绑定
  bool Bind()
  {
    struct sockaddr_in local;
    bzero(&local, sizeof(local));

    local.sin_family = AF_INET;
    local.sin_addr.s_addr = htonl(INADDR_ANY);
    local.sin_port = htons(_port);
    
    if(bind(_sock, (struct sockaddr*)&local, sizeof(local)) < 0)
    {
      cout<< "bind success" <<endl;
      return false;
    }
    
    cout<< "bind success" <<endl;
    return true;
  }

// 3.创建一个开始监听了
  
  bool Listen()
  {
    if(listen(_sock, 5) < 0)
    {
      cout<< "listen failed" <<endl;
      return false;
    }

    cout<< "listen success" <<endl;
    return true;
  }


// 3.TCP服务器就可以通信了的。
bool Recv()
{
    char buffer[1024] = {0};
    for(;;)
    {
        struct sockaddr_in client;
        bzero(&client, sizeof(client));
        socklen_t len = sizeof(client);
        char clientip[INET_ADDRSTRLEN];

        // 1. 接受新连接
        int sock = accept(_sock, (struct sockaddr*)&client, &len);
        if (sock < 0)
        {
            perror("accept failed");
            // 这里可以根据 errno 决定是否继续，简单起见继续循环
            continue;
        }

        // 2. 将客户端 IP 转换为字符串
        if (inet_ntop(AF_INET, &client.sin_addr, clientip, INET_ADDRSTRLEN) == nullptr)
        {
            cerr << "inet_ntop failed" << endl;
            close(sock);      // 转换失败也要关闭 socket
            continue;
        }

        cout << "接受连接 from " << clientip << ":" << ntohs(client.sin_port) << endl;

        // 3. 接收客户端数据（处理完整接收，直到对方关闭或出错）
        ssize_t n = recv(sock, buffer, sizeof(buffer) - 1, 0);
        if (n > 0)
        {
            buffer[n] = '\0';
            cout << "接收到消息: " << buffer << endl;
        }
        else if (n == 0)
        {
            cout << "客户端 " << clientip << " 已关闭连接" << endl;
        }
        else // n < 0
        {
            perror("recv failed");
        }

        // 4. 关闭与客户端的连接，释放资源
        close(sock);
    }
}


private:
  uint16_t _port;
  int _sock;
};
