#pragma once
#include <cstdint>
#include "net/socket.hpp"

void handlerHttp(int sock);

class server
{
public:
    server(uint16_t port);
    void init();
    void start();

private:
    Socket _listensock;
};

