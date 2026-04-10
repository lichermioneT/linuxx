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
// 凭什么你是服务器？
// 1.服务器知名端口号
// 2.监听套接字
// 3.epoll句柄
// 4.epoll数组
// 5.epoll数组大小
// 6.回调函数
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

// 服务器初始化
// 1._listensock套接字
// 2.epoll模型
// 3.套接字上树
// 4.创建第三方维护数组的
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

//4.第三方维护的数组
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
// n == 0 规定的时间没有返回的
        case 0:
          std::cout<< "timeout..." << std::endl; // 期望时间没有返回的
          break;
// n == -1 出现了问题的
        case -1:
          std::cout<< "epoll_wait failed..." << std::endl; // 出错了的
          break;
// n == number 有几个返回了的
        default:
          std::cout<< "have event ready" << std::endl; // 有事件就绪了的, 放到你的数组里面了的
// n个就行时间传递给事件的处理接口的
          handerEvent(n);
          break;
      }
    }
  }

  void handerEvent(int readyNum)
  {
    std::cout<< "handerEvent in" <<std::endl;
// epoll_wait是有序的n个的。只需要进行n个遍历即可的
    for(int i = 0; i < readyNum; ++i)
    {
      uint32_t events = _revs[i].events; 
      int sock = _revs[i].data.fd;

// 监听套接字就上树
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
// 普通套接字就处理业务的
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
