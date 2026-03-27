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

server::server(uint16_t port)
    : _listensock(port)
{}

void server::init()
{
    _listensock.Init();
}

void server::start()
{
  _listensock.Start(handlerHttp);
}

void handlerHttp(int sock)
{
  Connection con(sock);
  http_parser parser;
  
  while(true)
  {
    if(parser.getOneHttpRequest(con.InBuffer()))
    {
      continue;
    }
    else 
    {
      con.Read();
    }

  }
  std::cout<< "一个完整的报文如下:" << std::endl;
  std::cout<< parser._inbuffer << std::endl;
}

