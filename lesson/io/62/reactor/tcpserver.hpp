#pragma once
#include "sock.hpp"
#include <assert.h>
#include "util.hpp"
#include <string>
#include <iostream>
#include "epoller.hpp"
#include <functional>
#include <unordered_map>

namespace tcpserver
{
class Connection;
class Tcpserver;
static const uint16_t defaultport = 8080;
using func_t = std::function<void(Connection*)>;

class Connection
{
public:
  Connection(int sock)
    :_sock(sock)
    ,_tsp(nullptr)
  {}
  ~Connection(){}

  void Register(func_t r, func_t s, func_t e)
  {
    _recver = r;
    _sender = s;
    _excepter = e;
  }

public:
  int _sock;
  std::string _inbuffer; // 输入缓冲区
  std::string _outbuffer;// 输出缓冲区

  func_t _recver; // 从_sock读
  func_t _sender; // 从_sock写
  func_t _excepter; // 处理_sock异常事件的

  Tcpserver* _tsp; // ??
};

class Tcpserver 
{
private:
  uint16_t _port;
  Sock _sock;
  epoller _epoller;
  std::unordered_map<int, Connection*> connections;

public:
  Tcpserver(uint16_t port)
    :_port(port)
  {}

  ~Tcpserver()
  {
  }

  void init()
  {
    // 1.创建套接字
    _sock.Socket();
    _sock.Bind(_port);
    _sock.Listen();

    // 2.构建epoll模型的
    _epoller.Create(); 
    
    AddConnection(_sock.fd(), EPOLLIN | EPOLLET, , , ,);
  }

  void AddConnection(int sock, uint32_t events, func_t recver, func_t sender, func_t excepter)
  {
    // 1.首先为sock创建对应的Connection,并初始化，并添加到connection
    // Listen套接字上树之前，先将文件描述符设置成非阻塞模式的
    if(events & EPOLLET)  util::setNonBlock(_sock.fd());
    Connection* conn = new Connection(sock);

    // 设置回调方法
    conn->Register(recver, sender, excepter);

    //2.其次将sock与它关系的事件写透式的注册到epoll里面，让epoll帮我们关系的
    bool ret = _epoller.AddEvent(sock, events);
    assert(ret); 
    (void)ret;

    //3.添加到Connetion里面
    connections.insert(std::pair<int, Connection*>(sock, conn));
  }

  void Dispatch()
  {
    while(true)
    {

    }
  }
};
};
