#pragma once
#include <iostream>
#include <string>
#include <cerrno>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstdlib>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>

using namespace std;

namespace client
{
class udpclient
{
public:
    udpclient(const string& serverip, const uint16_t& serverport)
        : _serverip(serverip),
          _serverport(serverport),
          _sockfd(-1),
          _quit(false)
    {}

    void init()
    {
        _sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (_sockfd == -1)
        {
            cerr << "socket error: " << strerror(errno) << endl;
            exit(1);
        }
    }

    static void* readMessage(void* args)
    {
        udpclient* self = static_cast<udpclient*>(args);
        pthread_detach(pthread_self());

        char buffer[1024];
        struct sockaddr_in peer;
        socklen_t len = sizeof(peer);

        while (!self->_quit)
        {
            ssize_t n = recvfrom(self->_sockfd,
                                 buffer,
                                 sizeof(buffer) - 1,
                                 0,
                                 (struct sockaddr*)&peer,
                                 &len);
            if (n > 0)
            {
                buffer[n] = 0;
                cout << "[server] " << buffer << endl;
            }
        }
        return nullptr;
    }

    void run()
    {
        pthread_create(&_reader, nullptr, readMessage, this);

        struct sockaddr_in server;
        memset(&server, 0, sizeof(server));
        server.sin_family = AF_INET;
        server.sin_addr.s_addr = inet_addr(_serverip.c_str());
        server.sin_port = htons(_serverport);

        char cmdline[1024];

        while (!_quit)
        {
            fprintf(stderr, "enter# ");
            fflush(stderr);

            if (!fgets(cmdline, sizeof(cmdline), stdin))
                continue;

            size_t len = strlen(cmdline);
            if (len <= 1)   // 只敲回车
                continue;

            cmdline[len - 1] = 0; // 去掉 '\n'

            if (strcmp(cmdline, "quit") == 0)
            {
                _quit = true;
                break;
            }
            sendto(_sockfd, cmdline, strlen(cmdline), 0, (struct sockaddr*)&server, sizeof(server));
        }
    }

    ~udpclient()
    {
        _quit = true;
        if (_sockfd >= 0)
            close(_sockfd);
    }

private:
    string _serverip;
    uint16_t _serverport;
    int _sockfd;
    bool _quit;
    pthread_t _reader;
};
}
