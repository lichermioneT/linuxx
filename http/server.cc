#include "net/socket.hpp"
#include "server.hpp"
#include <iostream>
#include <string>
#include "net/connection.hpp"
#include "http/http_request.hpp"
#include "http/http_response.hpp"
#include "http/http_protocol.hpp"
#include "http/http_parser.hpp"
#include "logic/router.hpp"

using namespace std;

server::server(uint16_t port, int sock)
    : _listensock(port)
    ,_con(sock)
{}

void server::init()
{
    _listensock.Init();
}

void server::start()
{
  _listensock.Start(handlerHttp);
}


