#pragma once
#include "protocol.hpp"
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
using hander_t = std::function<void(const std::string& package)>;

class Revent
{
public:
};

// 一个套接字一个对象。
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
  Connection(int sock, Tcpserver* tsp)
    :_sock(sock)
    ,_tsp(tsp)
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
  func_t _func;

public:
  Tcpserver(func_t func, uint16_t port)
    :_port(port)
    ,_revs(nullptr)
    ,_num(num) 
    ,_func(func)
  {}

  ~Tcpserver()
  {}

public:
  void init()
  {
    // 1.创建套接字
    _sock.Socket();
    _sock.Bind(_port);
    _sock.Listen();
  
    util::setNonBlock(_sock.fd());

    // 2.构建epoll模型的
    _epoller.Create(); 
    
    // 3.Listensock套接字bind的是Accepter函数，他只是管新新链接的到来。
    // 4.它的Connect对象注册的方法有Accepter, nullptr, nullptr。
    AddConnection(_sock.fd(), EPOLLIN | EPOLLET, std::bind(&Tcpserver::Accepter, this, std::placeholders::_1), nullptr , nullptr);

    _revs = new struct epoll_event[num];
    _num = num;
  }

// 事件派发器
  void Dispatcher()
  {
    int timeout = 1000;
    while(true)
    {
      loop(timeout); 
    }
  }

private:
  void AddConnection(int sock, uint32_t events, func_t recver, func_t sender, func_t excepter)
  {
    // 1.首先为sock创建对应的Connection,并初始化，并添加到connection
    // Listen套接字上树之前，先将文件描述符设置成非阻塞模式的
    if(events & EPOLLET)  
    {
      util::setNonBlock(sock);
    }

// 创建一个connet的对象
    Connection* conn = new Connection(sock, this);

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
      // _connections[_sock.fd()]->Accepter(_connections[sock]== (Connection* conn))
      if((events & EPOLLIN) && IsConnectExists(sock) && _connections[sock]->_recver)
        _connections[sock]->_recver(_connections[sock]);                            // 根据Key找到对应的value, value是指针的嘛，指针直接，你懂得。

      // 写事件
      if((events & EPOLLOUT) && IsConnectExists(sock) && _connections[sock]->_sender)
        _connections[sock]->_sender(_connections[sock]);
      //todo
#endif
    }
  }

  void Accepter(Connection* conn)
  {
    for(;;)
    {
      std::string clientip;
      uint16_t clientport;
      int err = 0;
      int sock = _sock.Accept(&clientip, &clientport, &err);
      if(sock >= 0) 
      { 
          AddConnection(sock, EPOLLIN | EPOLLET | EPOLLOUT, 
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

  void Recver(Connection* conn)
  {
    char buffer[1024];
    while(true)
    {
      ssize_t s = recv(conn->_sock, buffer, sizeof(buffer) - 1, 0);
// 1.有数据可以读的
      if(s > 0)
      {
        buffer[s] = 0;
        conn->_inbuffer += buffer;
        std::cout<< conn->_inbuffer << std::endl;
        _func(conn);
      }
// 2.对方关闭且数据已经读完了的
      else if(s == 0)
      {
        if(conn->_excepter)
          conn->_excepter(conn);
          break;
      }
      else 
      {
// 3.没有数据的，但是对方还在的
// 链接还在的
        if(errno == EAGAIN || errno == EWOULDBLOCK)
        {
          break;
        }
// 4.被信号中断了的
        else if(errno == EINTR)
        {
          continue;
        }
// 5.真正出现错误了的
        else 
        {
          if(conn->_excepter)
          {
            conn->_excepter(conn);
            break;
          }
        }
      }
    }
    /*
     *if(!conn->_inbuffer.empty())
     *  std::cout<< conn->_sock << ":" << conn->_inbuffer.c_str() << std::endl;
     */
  /*
   *std::string onePackage;
   *while(!ParseOnePackage(conn->_inbuffer, &onePackage))
   *{
   *    _hander(onePackage);
   *}
   */

    /*
     *std::cout<< conn->_inbuffer << std::endl;
     *_func(conn);
     */

  }

  void Sender(Connection* conn)
  {
    while(true)
    {
      ssize_t s = send(conn->_sock, conn->_outbuffer.c_str(), conn->_outbuffer.size(), 0);
      if(s > 0)
      {
        if(conn->_outbuffer.empty()) 
        {
          EnableReaderWrite(conn, true, false); 
          break;
        }
        else 
        {
          conn->_outbuffer.erase(0, s);
        }
      }
      else 
      {
        if(errno == EAGAIN || errno == EWOULDBLOCK)
          break;
        else if (errno == EINTR) 
          continue;
        else 
        {
          if(conn->_excepter)
          {
            conn->_excepter(conn);
            break;
          }
        }
      }
    }
  }

  void Excepter(Connection* conn)
  {
    std::cout<< "DEBUG" << "Excepter begin" << std::endl;
    _epoller.Control(conn->_sock, 0, EPOLL_CTL_DEL); // 下树
    close(conn->_sock);   // 先下树，在关闭文件描述符
    _connections.erase(conn->_sock); // 删除容器
    
    std::cout<< "DEBUG" << "Excepter end" <<  std::endl;
    delete conn;
  }
  
  bool IsConnectExists(int sock)
  {
    auto iter = _connections.find(sock);
    return iter != _connections.end();
  }

  void EnableReaderWrite(Connection* conn, bool readable, bool writeable)
  {
    uint32_t event = (readable ? EPOLLIN : 0) | (writeable ? EPOLLOUT : 0) | EPOLLET;
    _epoller.Control(conn->_sock, event, EPOLL_CTL_MOD);
  }

};
};
