#pragma once
#include <cstdint>
#include "net/socket.hpp"
#include "net/connection.hpp"

void handlerHttp(int sock);

class server
{
public:
    server(uint16_t port, int sock);
    void init();
    void start();
    void connect();

private:
    Socket _listensock;
    Connection _con;
};

