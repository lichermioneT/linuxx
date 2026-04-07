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
static const int num  =  64;
using func_t = std::function<void(Connection*)>;

class Revent
{
public:
};

// 根据文件描述符进行封装一个对象，里面有文件描述符的
// 读写缓冲区
// 可以操作文件描述符的读写异常。
class Connection
{
public:
  int _sock;
  std::string _inbuffer; // 输入缓冲区
  std::string _outbuffer;// 输出缓冲区

  func_t _recver; // 从_sock读
  func_t _sender; // 从_sock写
  func_t _excepter; // 处理_sock异常事件的
  Tcpserver* _tsp; // ??

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

};

class Tcpserver 
{
private:
  uint16_t _port;
  Sock _sock;
  epoller _epoller;
  std::unordered_map<int, Connection*> _connections;
  struct epoll_event* _revs;
  int _num;
public:
  Tcpserver(uint16_t port)
    :_port(port)
    ,_revs(nullptr)
    ,_num(num) 
  {}

  ~Tcpserver()
  {}

  void init()
  {
    // 1.创建套接字
    _sock.Socket();
    _sock.Bind(_port);
    _sock.Listen();

    // 2.构建epoll模型的
    _epoller.Create(); 
    
    AddConnection(_sock.fd(), EPOLLIN | EPOLLET, std::bind(&Tcpserver::Accepter, this, std::placeholders::_1), nullptr , nullptr);

    _revs = new struct epoll_event[num];
    _num = num;
  }

  void Recver(Connection* conn)
  {
    char buffer[1024];
    while(true)
    {
      ssize_t s = recv(conn->_sock, buffer, sizeof(buffer) - 1, 0);
      if(s > 0)
      {
        buffer[s] = 0;
        conn->_inbuffer += buffer;
      }
      else if(s == 0)
      {
        if(conn->_excepter)
          conn->_excepter(conn);
      }
      else 
      {
        if(errno == EAGAIN || errno == EWOULDBLOCK)
        {
          break;
        }
        else if(errno == EINTR)
        {
          continue;
        }
        else 
        {
          if(conn->_excepter)
            conn->_excepter(conn);
        }
      }
    }
    std::cout<< conn->_sock << ":" << conn->_inbuffer.c_str() << std::endl;
  }
  void Sender(Connection* conn)
  {

  }
  void Excepter(Connection* conn)
  {


  }

  void Accepter(Connection* conn)
  {
    for(;;)
    {
      std::string clientip;
      uint16_t clientport;
      int err = 0;
      int sock = _sock.Accept(&clientip, &clientport, &err);
      if(sock > 0) 
      { 
          AddConnection(sock, EPOLLIN | EPOLLET, 
              std::bind(&Tcpserver::Recver, this, std::placeholders::_1),
              std::bind(&Tcpserver::Sender, this, std::placeholders::_1),
              std::bind(&Tcpserver::Excepter, this, std::placeholders::_1));

          std::cout<< "get a new link: " << clientip << ":" << clientport << std::endl;
      }
      else 
      {
        if(err == EAGAIN || err == EWOULDBLOCK)
          break;
        else if(err == EINTR)
          continue;
        else 
          break; // 出错了，暂时不处理的。
          
      }

    }
  }

  void AddConnection(int sock, uint32_t events, func_t recver, func_t sender, func_t excepter)
  {
    // 1.首先为sock创建对应的Connection,并初始化，并添加到connection
    // Listen套接字上树之前，先将文件描述符设置成非阻塞模式的
    if(events & EPOLLET)  
    {
      util::setNonBlock(_sock.fd());
    }

// 创建一个connet的对象
    Connection* conn = new Connection(sock);

// 设置回调方法,设置进一个connet对象里面去的。
    conn->Register(recver, sender, excepter);

    //2.其次将sock与它关系的事件写透式的注册到epoll里面，让epoll帮我们关系的
    //上树
    bool ret = _epoller.AddEvent(sock, events);
    assert(ret); 
    (void)ret;

    //3.添加到Connetion里面
    _connections.insert(std::pair<int, Connection*>(sock, conn));
    std::cout<< "add a new sock : " << sock << std::endl;
  }
  
  bool IsConnectExists(int sock)
  {
    auto iter = _connections.find(sock);
    return iter != _connections.end();
  }

  void loop(int timeout)
  {
    int n = _epoller.Wait(_revs, num, timeout); // 获取已经就绪的事件,返回值就是几个就绪的事件
    for(int i = 0; i < n; ++i)
    {
      int sock = _revs[i].data.fd;
      uint32_t events = _revs[i].events;

#if 0
      // 监听事件就绪了的
      if((events & EPOLLIN) && sock == _sock.fd())
      {
        _connections[sock]->_recver(_connections[sock]);
      }

      if((events & EPOLLIN) && sock != _sock.fd())
      {
        _connections[sock]->_recver(_connections[sock]);
      }
#else 
      // 异常事件，转换成读写事件的
      if(events & EPOLLERR) events |= (EPOLLIN | EPOLLOUT);
      if(events & EPOLLHUP) events |= (EPOLLIN | EPOLLOUT);

      // 读事件
      if((events & EPOLLIN) && IsConnectExists(sock) && _connections[sock]->_recver)
        _connections[sock]->_recver(_connections[sock]);          // 根据Key找到对应的value, value是指针的嘛，指针直接，你懂得。

      // 写事件
      if((events & EPOLLOUT) && IsConnectExists(sock) && _connections[sock]->_sender)
        _connections[sock]->_sender(_connections[sock]);
      //todo
#endif
    }
  }

// 事件派发器
  void Dispatcher()
  {
    int timeout = 1000;
    while(true)
    {
      loop(timeout); 
      std::cout<< "DEBUG" <<std::endl;
    }
  }
};
};
