#pragma once 
#include "sock.hpp"
#include <poll.h>
#include <sys/epoll.h>
#include <string>
#include <iostream>
#include <functional>

namespace epoll_ns
{
static const uint16_t defaultip = 8080;
static const int size = 128;
static  const int defaulvalue = -1;
static  const int defaultnum = 64;
class epollServer
{
public:
  epollServer(int num = defaultnum, uint16_t port = defaultip)
    :_port(port)
    ,_listensock(-1)
    ,_epfd(-1)
    ,_revs(nullptr)
    ,_num(num)
  {
  }

  ~epollServer()
  {
    if(_listensock != defaulvalue) close(_listensock);
    if(_epfd != defaulvalue) close(_epfd);
    if(_revs) delete[] _revs;
  }

  void handerEvent(int n)
  {
    std::cout<< "handerEvent in" <<std::endl;
    for(int i = 0; i < n; i++)
    {
      
    }
    std::cout<< "handerEvent in" <<std::endl;
  }

  void init()
  {

    //1.创建socket
    _listensock = Sock::Socket();
    Sock::Bind(_listensock, _port);
    Sock::Listen(_listensock);
     
    //2.创建epoll模型
    _epfd = epoll_create(size);
    if(_epfd < 0)
    {
      std::cout<< "epoll模型创建失败" << std::endl;
      exit(-1);
    }

    //3.添加_listensock到epoll里面的
    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = _listensock;

    epoll_ctl(_epfd, EPOLL_CTL_ADD, _listensock, &ev);

    _revs = new struct epoll_event[_num];

    std::cout<< "init server success" << std::endl;
  }

  void start()
  {
    int timeout = 1000;
    for(;;)
    {
      //4.捞取就绪的事件。
      int n = epoll_wait(_epfd, _revs, _num, timeout);
      switch(n)
      {
        case 0:
          std::cout<< "timeout..." << std::endl;
          break;
        case -1:
          std::cout<< "epoll_wait failed..." << std::endl;
          break;
        default:
          std::cout<< "have event ready" << std::endl;
          handerEvent(n);
          break;
      }
    }

  }

private:
  uint16_t _port;
  int _listensock;
  int _epfd;
  struct epoll_event* _revs;
  int _num;
};



}
