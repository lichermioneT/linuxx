#pragma once 
#include "sock.hpp"
#include <poll.h>
#include <string>
#include <iostream>
#include <functional>


namespace poll_ns
{

static const uint16_t defaultport = 8080; 
static const int defaultfd = -1;
static const int num = 2048;
using func_t = std::function<std::string (const std::string&)>;

class poll_server 
{
private: 
  uint16_t _port;
  int  _listensock;
  struct pollfd* _rfds;
  func_t _func;

public:
  poll_server(func_t func, uint16_t port = defaultport)
    :_port(port)
    ,_listensock(-1)
    ,_rfds(nullptr)
    ,_func(func)
  {}

  ~poll_server() 
  {
    if(_listensock >= 0) close(_listensock);

    if(_rfds) delete[] _rfds;
  }
 
  void init()
  {
    _listensock = Sock::Socket();
    Sock::Bind(_listensock, _port);
    Sock::Listen(_listensock);

    _rfds = new struct pollfd[num];
    for(int i = 0; i < num; ++i) 
    {
      ResetItem(i);
    }
    
    // 最开始只有一个文件描述符
    _rfds[0].fd = _listensock; // 不变了的，固定的位置下标
    _rfds[0].events = POLLIN;
    // 服务启动之前，只存在_listensock的。
  }

  void ResetItem(int i)
  {
    _rfds[i].fd = defaultfd;
    _rfds[i].events = 0;
    _rfds[i].revents = 0;
  }

  void print()
  {
    for(int i = 0; i < num; ++i)
    {
      if(_rfds[i].fd != defaultfd)
        std::cout<< "_rfds list:" << _rfds[i].fd << std::endl;
    }
  }

  void start()
  {
    int timeout = 1000;
    for(;;)
    {
      int n = poll(_rfds, num, timeout);
      switch(n)
      {
        case 0:
          std::cout<< "timeout...." << std::endl; // 在我等的这个时间段里面你没有进行返回的。
          break;
        case -1:
          std::cout<< "poll error:" << errno << strerror(errno) << std::endl;
          break;
        default:

          // 说明有事件就绪了，目前只有一个监听事件就绪了
          // 需要取走的
          handlerEvent(); // 这里告诉我，哪些文件描述符已经就绪了的
          break;
      }
      
      // 业务处理的
    }
  }

// 这个函数不仅仅是一个仅有一个fd准备就绪的，可能存在多个的。
  void handlerEvent() // 这里就是已经就绪的文件描述符
  {  
   for(int i = 0; i < num; ++i) 
   {
       // 过滤掉非法的fd.
      if(_rfds[i].fd == defaultfd) 
        continue; // 这个位置的文件描述符没有准备好的，继续下一个的

      if(!(_rfds[i].revents & POLLIN)) 
        continue; // 我们设置了读事件的关心

// _listensock的读事件就绪了，所以需要进行处理，添加新的文件描述符
      if(_rfds[i].fd == _listensock  && (_rfds[i].events & POLLIN))
      {
        Accepter(_listensock);
      }
      else if((_rfds[i].events & POLLIN) && (_rfds[i].events & POLLIN)) 
      {
        Revcer(i);
      }
      else 
      {
      }
    
      _rfds[i].revents = 0;
   }
  }

  void Accepter(int listensock)
  {
        // select告诉我，_listensock读事件准备就绪了的
        // 走到这里，accept不会阻塞的。
        std::string clientip;
        uint16_t clientport = 0;
      
        int sock = Sock::Accept(listensock, &clientip, &clientport); // Accept = 等 + 获取
        if(sock < 0)  
        {
          return;
        }

        std::cout<< "新链接获取成功：ip:" << clientip.c_str() << ":port" << clientport << std::endl;

        // 这里能够直接recv/read吗？ 不能的。整个代码，只有select有资格检查事件是否就绪的。
        // 底层可能没有数据的的。
        
        // 将新的sock托管给select！, 本质就是 将sock添加到fdarray数组中即可。
        int i = 0;
        for(i = 0; i < num; ++i)
        {
          if(_rfds[i].fd != defaultfd) 
            continue;
          else 
            break;
        }

        if(i == num)
        {
          std::cout<< "服务器已经承受住了，已经满了" << std::endl;
          close(sock);
        }
        else 
        {
          _rfds[i].fd = sock;
          _rfds[i].events = POLLIN;
          _rfds[i].revents = 0;
        }
        print();
  }

  void Revcer(int pos)
  {
    std::cout<< "in Revcer " << std::endl;

    // 这样子读取有问题的。 1.读完了？2.如何反序列化。
    char buffer[1024] = {0};
    ssize_t s = recv(_rfds[pos].fd, buffer, sizeof(buffer) - 1, 0); // 这里在进行读取的时候会不会被阻塞的呢？
    if(s > 0)
    {
      buffer[s] = 0;
      std::cout<< "client#" << buffer <<std::endl;
    }
    else if(s == 0)
    {
      close(_rfds[pos].fd);
      ResetItem(pos);
      std::cout<< "clientip quit" << std::endl;
      return;
    }
    else 
    {
      close(_rfds[pos].fd);
      ResetItem(pos);
      std::cout<< "clientip errno" << std::endl;
      return;
    }

    // 处理
    std::string respose = _func(buffer);

    // write写事件，太麻烦了的
    write(_rfds[pos].fd, respose.c_str(), respose.size());
    std::cout<< "out Revcer " << std::endl;
  }

};
}
