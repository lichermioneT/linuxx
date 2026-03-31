#pragma once 

#include "sock.hpp"
#include <string>
#include <iostream>


namespace select_ns
{
static const uint16_t defaultport = 8080; 
static const int fdnum = sizeof(fd_set)*8;
static const int defaultfd = -1;
class select_server 
{
public:
  select_server(uint16_t port = defaultport)
    :_port(port)
    ,_listensock(-1)
    ,fdarray(nullptr)
  {}
 
  void print()
  {
    for(int i = 0; i < fdnum; ++i)
    {
      if(fdarray[i] != defaultfd)
        std::cout<< "fd list:" << fdarray[i] << std::endl;
    }
  }

  void handlerEvent(fd_set& rfds)
  {  
      if(FD_ISSET(_listensock, &rfds))
      {
        // select告诉我，_listensock读事件准备就绪了的
        // 走到这里，accept不会阻塞的。
        std::string clientip;
        uint16_t clientport = 0;
      
        int sock = Sock::Accept(_listensock, &clientip, &clientport); // Accept = 等 + 获取
        if(sock < 0)  return;

        std::cout<< "新链接获取成功：ip:" << clientip.c_str() << "port" << clientport << std::endl;

        // 这里能够直接recv/read吗？ 不能的。整个代码，只有select有资格检查事件是否就绪的。
        // 将新的sock托管给select！, 本质就是 将sock添加到fdarray数组中即可。
        //
        int i = 0;
        for(i = 0; i < fdnum; ++i)
        {
          if(fdarray[i] != defaultfd) continue;
          else break;
        }

        if(i == fdnum)
        {
          std::cout<< "服务器已经承受住了，已经满了" << std::endl;
          close(sock);
        }
        else 
        {
          fdarray[i] = sock;
        }

        print();
      }
    
  }

  void init()
  {
    _listensock = Sock::Socket();
    Sock::Bind(_listensock, _port);
    Sock::Listen(_listensock);

    // fd_set是一种类型，必须有大小，而且是固定的
    // 所欲能够添加fd的个数是有上限的
    // fd_set:最大1024个
    fdarray = new int[fdnum]; // 多大啊？
    for(int i = 0; i < fdnum; ++i) fdarray[i] = defaultfd;
    
    // 最开始只有一个文件描述符
    fdarray[0] = _listensock; // 不变了的，固定的位置下标

  }

  void start()
  {

    for(;;)
    {
      fd_set rfds;
      FD_ZERO(&rfds);
      int maxfd = fdarray[0];
      
      for(int i = 0; i < fdnum; ++i)
      {
        if(fdarray[i] == defaultfd)
          continue;
        FD_SET(fdarray[i], &rfds);

        if(maxfd < fdarray[i]) maxfd = fdarray[i]; // 更新所有fd中最大的fd.
      }


      // struct timeval timeout = {3,0}; // 几秒钟之内通知我一声。注意这里必须写到for循环里面，不然变量一直存在里面的数据减到零，就是非阻塞了。

      //  一般而言，使用select，需要程序员自己维护一个保存所以合法的fd的数组
      //   
      int n = select(maxfd + 1, &rfds, nullptr, nullptr, nullptr);
      switch(n)
      {
        case 0:
          std::cout<< "timeout...." << std::endl; // 在我等的这个时间段里面你没有进行返回的。
          break;
        case -1:
          std::cout<< "select error:" << errno << strerror(errno) << std::endl;
          break;
        default:
          // 说明有事件就绪了，目前只有一个监听事件就绪了
          // 需要取走的
          
          handlerEvent(rfds);
          std::cout<< "get a new link" << std::endl;    
          break;
      }
      
      // 业务处理的
    }
  }

  ~select_server() 
  {
    if(_listensock < 0) close(_listensock);
    if(fdarray) delete[] fdarray;
  }
private:
  uint16_t _port;
  int  _listensock;
  int *fdarray;
};
}
