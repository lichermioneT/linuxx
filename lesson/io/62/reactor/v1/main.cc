#include "tcpserver.hpp"
#include <memory>

using namespace tcpserver;

bool cal(const Request &req, Response &resp)
{
    // req已经有结构化完成的数据啦，你可以直接使用
    resp.exitcode = OK;
    resp.result = OK;

    switch (req.op)
    {
        case '+':
            resp.result = req.x + req.y;
            break;
        case '-':
            resp.result = req.x - req.y;
            break;
        case '*':
            resp.result = req.x * req.y;
            break;
        case '/':
        {
            if (req.y == 0)
                resp.exitcode = DIV_ZERO;
            else
                resp.result = req.x / req.y;
        }
        break;
        case '%':
        {
            if (req.y == 0)
                resp.exitcode = MOD_ZERO;
            else
                resp.result = req.x % req.y;
        }
        break;
        default:
            resp.exitcode = OP_ERROR;
            break;
    }
    return true;
}

void calculate(Connection* conn)
{
  std::string onepPackage;
  while(ParseOnePackage(conn->_inbuffer, &onepPackage))
  {
    std::string reqStr;
    if(!deLength(onepPackage, &reqStr))
      return;

    std::cout<< "去掉正文的报头：\n" << reqStr << std::endl;

    Request req;
    if(!req.deserialize(reqStr))
      return;

    Response resp;
    cal(req, resp);
    
    std::string respStr;
    resp.serialize(&respStr);

    conn->_outbuffer = enLength(respStr);
    /*
     *std::cout<< "构建完整的报文响应\n" << send_string << std::endl;
     */

  }
  if(conn->_sender)
    conn->_sender(conn);
  // 如果没发送完毕，需要开启对sock的写事件关注，发完了，关闭对写事件的关系
  if(!conn->_outbuffer.empty())
    conn->_tsp->EnableReaderWrite(conn, true, true); 
  else 
    conn->_tsp->EnableReaderWrite(conn, true, false);
}

int main(int argc, char* argv[])
{
  if(argc != 2)
  {
    std::cout<< "usage: ../a.out port" << std::endl;
    return -1;
  }
  
  uint16_t port = atoi(argv[1]);

  std::unique_ptr<Tcpserver> tser(new Tcpserver(calculate, port));
  tser->init();    // 通过listensock添加新的链接
  tser->Dispatcher(); // 等待链接事件的就绪

  return  0;
}
