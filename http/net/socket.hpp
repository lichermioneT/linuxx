#pragma once
#include <iostream>
#include <functional>
#include <string>
#include <cstdint>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

class Socket
{
public:

public:
    Socket(uint16_t port);
    ~Socket();

    void Init();
    void Start();

private:
    int CreateSocket();
    void Bind();
    void Listen(int backlog = 5);
    int Accept(std::string* client_ip = nullptr, uint16_t* client_port = nullptr);
    void SetReuseAddr();
    void Close();

private:
    int _sockfd;
    uint16_t _port;
};
