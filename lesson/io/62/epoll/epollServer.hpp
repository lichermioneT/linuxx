#pragma once 
#include "sock.hpp"
#include <poll.h>
#include <sys/epoll.h>
#include <string>
#include <string>
#include <iostream>
#include <functional>
using namespace std;

namespace epoll_ns
{
static const uint16_t defaultip = 8080;
static const int size = 128;
static  const int defaulvalue = -1;
static  const int defaultnum = 64;
using func_t = function<std::string (const string&)>;
class epollServer
{
private:
  uint16_t _port;
  int _listensock;
  int _epfd;
  struct epoll_event* _revs;
  int _num;
  func_t _func;
public:
  epollServer(func_t func, int num = defaultnum, uint16_t port = defaultip)
    :_port(port)
    ,_listensock(-1)
    ,_epfd(-1)
    ,_revs(nullptr)
    ,_num(num)
    ,_func(func)
  {}

  ~epollServer()
  {
    if(_listensock != defaulvalue) close(_listensock);
    if(_epfd != defaulvalue) close(_epfd);
    if(_revs) delete[] _revs;
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
    ev.data.fd = _listensock;  // 当事件就绪，我们知道是谁的

    epoll_ctl(_epfd, EPOLL_CTL_ADD, _listensock, &ev);

    _revs = new struct epoll_event[_num];

    std::cout<< "init server success" << std::endl;
  }

  void start()
  {
    int timeout = 1000;

    for(;;)
    {
      // 捞取就绪的事件。
      int n = epoll_wait(_epfd, _revs, _num, timeout);  // timeout时间内阻塞的
      switch(n)
      {
        case 0:
          std::cout<< "timeout..." << std::endl; // 期望时间没有返回的
          break;

        case -1:
          std::cout<< "epoll_wait failed..." << std::endl; // 出错了的
          break;

        default:
          std::cout<< "have event ready" << std::endl; // 有事件就绪了的, 放到你的数组里面了的
          handerEvent(n);
          break;
      }
    }
  }

  void handerEvent(int readyNum)
  {
    std::cout<< "handerEvent in" <<std::endl;
    for(int i = 0; i < readyNum; ++i)
    {
      uint32_t events = _revs[i].events; 
      int sock = _revs[i].data.fd;

      if(sock == _listensock && (events & EPOLLIN))
      {
        // Listen套接字的读事件就绪，获取新链接的
        string clientip;
        uint16_t port = 0;
        int fd = Sock::Accept(_listensock, &clientip, &port);
        if(fd == 0)
        {
          cout<< "新链接到了 Accept接收失败" <<endl;
          continue;
        }
        
        // 上树
        struct epoll_event ev;
        ev.events = EPOLLIN;
        ev.data.fd = fd;

        epoll_ctl(_epfd, EPOLL_CTL_ADD, fd, &ev);
      }
      else if(events & EPOLLIN)
      {
        // 普通的套接字就绪
        char buffer[1024] = {0};

        // 1.读完？2.协议？
        int n = recv(sock, buffer, sizeof(buffer) - 1, 0);
        if(n > 0)
        {
          buffer[n] = 0;
          cout<< "client#" << buffer <<endl;
          
          string response = _func(buffer);

          send(sock, response.c_str(), response.size(), 0); // 发送完了？
        }
        else if(n == 0) // 对方关闭了的
        {
          // 1.先删除，在关闭的
          epoll_ctl(_epfd, EPOLL_CTL_DEL, sock, nullptr);
          close(sock);
          cout<< "我也退出了 下树" <<endl;
        }
        else 
        {
          cout<< "error : " << strerror(errno) <<endl;
        }
      }
      else 
      {

      }
    }
    std::cout<< "handerEvent in" <<std::endl;
  }


};



}
