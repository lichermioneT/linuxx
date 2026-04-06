#pragma once 
#include "sock.hpp"
#include <string>
#include <iostream>
#include <functional>

namespace select_ns
{
static const uint16_t defaultport = 8080; 
static const int fdnum = sizeof(fd_set)*8;
static const int defaultfd = -1;
using func_t = std::function<std::string (const std::string&)>;

class select_server 
{
private: 
  uint16_t _port;
  int  _listensock;
  int *fdarray;      //  自己维护一个数组，存放fd。
  func_t _func;
public:
  select_server(func_t func, uint16_t port = defaultport)
    :_port(port)
    ,_listensock(-1)
    ,fdarray(nullptr)
    ,_func(func)
  {}
 
  ~select_server() 
  {
    if(_listensock >= 0) close(_listensock);
    if(fdarray) delete[] fdarray;
  }

  void init()
  {
    _listensock = Sock::Socket();
    Sock::Bind(_listensock, _port);
    Sock::Listen(_listensock);

    // fd_set是一种类型，必须有大小，而且是固定的
    // 所欲能够添加fd的个数是有上限的
    // fd_set:最大1024个

// 宏：FD_SETSIZE;
    fdarray = new int[fdnum]; // 多大啊？

// 清空数据滴
    for(int i = 0; i < fdnum; ++i) 
    {
      fdarray[i] = defaultfd; // defaultfd = -1;
    }
    
// 最开始只有一个文件描述符
// 服务启动之前，只存在_listensock的。
    fdarray[0] = _listensock; // 不变了的，固定的位置下标
  }

  void start()
  {
    for(;;)
    {
// 读文件描述符集合
      fd_set rfds;
      FD_ZERO(&rfds);
     
// 每次循环的时候3秒钟，返回一次的
      struct timeval time = {3, 0};

// 需要根据自己维护的数组里面找文件描述符
      int maxfd = fdarray[0];
      for(int i = 0; i < fdnum; ++i)
      {
// 跳过不合法的
        if(fdarray[i] == defaultfd) continue; // 非法文件描述符的。continue;

// 自己维护的文件描述符---设置进位图里面去的。自己需要select监听的文件描述符。
// 合法的fd ,添加到文件描述符集合里面去的  
        FD_SET(fdarray[i], &rfds); 

// 更新最大的maxfd      
        if(maxfd < fdarray[i]) maxfd = fdarray[i]; // 更新所有fd中最大的fd.
      }

      std::cout << "最大的文件描述符: " << maxfd << std::endl;
// struct timeval timeout = {3,0}; // 几秒钟之内通知我一声。注意这里必须写到for循环里面，不然变量一直存在里面的数据减到零，就是非阻塞了。
//  一般而言，使用select，需要程序员自己维护一个保存所以合法的fd的数组
// select开始监听我们给的文件描述符。
      int n = select(maxfd + 1, &rfds, nullptr, nullptr, &time);
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
         
// 这里我们是通过_listensock监听，来更新文件描述符的。
          handlerEvent(rfds); // 这里告诉我，哪些文件描述符已经就绪了的
          std::cout<< "get a new link" << std::endl;    
          sleep(1);
          break;
      }
      // 业务处理的
    }
  }

// 这个函数不仅仅是一个仅有一个fd准备就绪的，可能存在多个的。
// 只是处理读事件的
  void handlerEvent(fd_set& rfds) // 这里就是已经就绪的文件描述符
  {  
// 遍历fd数组
   for(int i = 0; i < fdnum; ++i) 
   {
       // 过滤掉非法的fd.
      if(fdarray[i] == defaultfd) continue; // 这个位置的文件描述符没有准备好的，继续下一个的

// 1._listensock已经准备好了的。
//   新的文件描述符托管给select滴。
      if(FD_ISSET(fdarray[i], &rfds) && fdarray[i] == _listensock)
      {
        Accepter(_listensock); // listensock读事件已经准备好了的
      }
// 2.这里是处理其它的文件描述符的
      else if(FD_ISSET(fdarray[i], &rfds))  // 这里是处理其他的文件描述符的
      {
        Revcer(fdarray[i], i);
      }
// 3.这里既是出错了滴。
      else 
      {
      }
   }
  }

  void Accepter(int listensock)
  {
        // select告诉我，_listensock读事件准备就绪了的
        // 走到这里，accept不会阻塞的。
        std::string clientip;
        uint16_t clientport = 0;
     
// 这里一定不会阻塞的。
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
          std::cout<< "新的文件描述符已经来了sock:" <<  sock <<std::endl;
        }
        print();
  }

  void Revcer(int sock, int pos)
  {
    std::cout<< "in Revcer " << std::endl;

    // 这样子读取有问题的。 1.读完了？2.如何反序列化。
    char buffer[1024] = {0};
    ssize_t s = recv(sock,buffer, sizeof(buffer) - 1, 0); // 这里在进行读取的时候会不会被阻塞的呢？肯定不会的，读事件就绪的
    if(s > 0)
    {
      buffer[s] = 0;
      std::cout<< "client#" << buffer <<std::endl;
    }
    else if(s == 0)
    {
      close(sock);
      fdarray[pos] = defaultfd;  // 把它清理出去的
      std::cout<< "clientip quit" << std::endl;
      return;
    }
    else  // 出现错误了的
    {
      close(sock);
      fdarray[pos] = defaultfd; 
      std::cout<< "clientip errno" << std::endl;
      return;
    }

    // 处理
    std::string respose = _func(buffer);

    // write写事件，太麻烦了的
    // 怎么判断写事件就绪了？ select的第三个参数，也需要维护的，
    write(sock, respose.c_str(), respose.size());
    std::cout<< "out Revcer " << std::endl;
  }

  void print()
  {
    for(int i = 0; i < fdnum; ++i)
    {
      if(fdarray[i] != defaultfd)
        std::cout<< "fd list:" << fdarray[i] << std::endl;
    }
  }
};
}
