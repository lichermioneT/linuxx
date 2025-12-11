#pragma once 

#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>

namespace Client 
{
class udpClient
{
public:
    udpClient(const std::string& serverip, const uint16_t& serverport) // 目标服务器的ip和port
        : _sockfd(-1)
        , _serverip(serverip)
        , _serverport(serverport)
        , _quit(false)
    {}

    void initClient()
    {
        // 创建socket
        _sockfd = socket(AF_INET, SOCK_DGRAM, 0); 
        if(_sockfd == -1)
        {
            std::cerr << "socket error: " << errno << " : " << strerror(errno) << std::endl; 
            exit(1);
        }
        std::cout << "socket success: " << _sockfd << std::endl;
        // UDP客户端一般不需要手动bind。不需要显示的bind port。
        // 客户端的端口号只需要保证唯一性。是谁不重要的。
    }

    static void* readMessage(void* args)
    {
        int sockfd = *(static_cast<int*>(args));
        pthread_detach(pthread_self());

        while(true)
        {
            char buffer[1024];
            struct sockaddr_in temp;
            socklen_t temp_len = sizeof(temp);

            ssize_t n = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0, (struct sockaddr*)&temp, &temp_len);
            if(n > 0)
            {
                buffer[n] = '\0';
                std::cout << buffer << std::endl;
            }
            else if(n == 0)
            {
                continue; // 对端关闭
            }
            else
            {
                perror("recvfrom error");
                break;
            }
        }
        return nullptr;
    }

    void run()
    {
        pthread_create(&_reader, nullptr, readMessage, &_sockfd);

// 填充对方服务器的信息。
        struct sockaddr_in server;
        std::memset(&server, 0, sizeof(server));
        server.sin_family = AF_INET;
        server.sin_addr.s_addr = inet_addr(_serverip.c_str());
        server.sin_port = htons(_serverport);

        char cmdline[1024];

        while(!_quit)
        {
            std::cerr << "Enter # ";
            std::cerr.flush();

            if(fgets(cmdline, sizeof(cmdline), stdin) == nullptr)
            {
                _quit = true;
                break;
            }

            size_t len = strlen(cmdline);
            if(len > 0 && cmdline[len - 1] == '\n')
                cmdline[len - 1] = '\0';

            if(strlen(cmdline) == 0)
                continue;

            sendto(_sockfd, cmdline, strlen(cmdline), 0, (struct sockaddr*)&server, sizeof(server)); // 将数据放松到目标服务器。
        }
    }

    ~udpClient()
    {
        _quit = true;
        if(_sockfd != -1)
            close(_sockfd);
    }

private:
    int _sockfd;
    std::string _serverip; // 目标服务器的ip
    uint16_t _serverport;  // 目标服务器的port
    bool _quit;
    pthread_t _reader;
};
  
} // namespace Client