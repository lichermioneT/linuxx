#pragma once 
#include "sock.hpp"
#include <string>
#include <iostream>
#include <functional>
using namespace std;

namespace select_ns
{

static const uint16_t defaultport = 8080;
static const int fdnum =  FD_SETSIZE;
static const int defaultfd = -1;
using func_t = std::function<std::string (const std::string&)>;

class select_server
{
private:
	uint16_t _port;
  int _listensock;
  int* _fdarray;
  func_t _func;

public:
  select_server(func_t func, uint16_t port = defaultport)
    :_port(port)
    ,_listensock(-1)
    ,_fdarray(nullptr)
    ,_func(func)
  {}

  ~select_server()
  {
    if(_listensock >= 0)
    {
      close(_listensock);
      _listensock = -1;
    }

    delete[] _fdarray;
  }

  void init()
  {
// 1.三部曲，sock bind Listen
    _listensock = Sock::Socket();
    Sock::Bind(_listensock, _port);
    Sock::Listen(_listensock);

// 2.创建辅助数组
    _fdarray = new int[fdnum];
    for(int i = 0; i < fdnum; ++i)
    {
      _fdarray[i] = defaultfd;
    }

// 3.目前只有一个 _listensock 设置到第一位置
    _fdarray[0] = _listensock;
  }

  void start()
  {
    for(;;)
    {
// 读取的文件描述符集合 
      fd_set rfds;
      FD_ZERO(&rfds);

//    设置阻塞时间
      struct timeval time = {3, 0};
   
// 找到最大的文件描述符
      int maxfd = 0;
      for(int i = 0; i < fdnum; ++i)
      {
        // 非法的就直接跳过
        if(_fdarray[i] == defaultfd)
          continue;

// 这个文件描述符设置进：读事件里面
// fd_set rfds      
        FD_SET(_fdarray[i], &rfds);
        
        // 更新文件描述符
        if(maxfd < _fdarray[i])
          maxfd = _fdarray[i];
      }

      cout<< "最大文件描述符maxfd:" << maxfd <<endl;

      int n = select(maxfd + 1, &rfds, nullptr, nullptr, &time);
      switch(n)
      {
          case  0:
              cout<< "timeout....,  在你规定的时间内没有事件就绪的" <<endl;
              break;
          case -1:
              cout<< "select error:" << strerror(errno) <<endl;
              break;
          default:
              handerEvent(rfds);
              cout<< "已经有新的链接到来了" <<endl;
              break;
      }
    }
  }

void handerEvent(fd_set& rfds)
{
  //  rfds住里面全部都是已经准备好的 读事件
  // 遍历数组，看看三种情况
    
  for(int i = 0; i < fdnum; ++i)
  {
// 非法的
    if(_fdarray[i] == defaultfd)
      continue;

// _listensock准备好了的
    if(FD_ISSET(_fdarray[i], &rfds) && _fdarray[i] == _listensock)
    {
      Accepter(_listensock);
    }
    else if(FD_ISSET(_fdarray[i], &rfds))
    {
      Revcer(_fdarray[i],  i);
    }
    else 
    {
      continue; 
    }
  }
}

void Accepter(int listensock)
{
  // accept建立链接我们需要知道，对方的信息
  string clientip;
  uint16_t clientport = 0;
  
  int sock = Sock::Accept(listensock, &clientip, &clientport);
  if(sock < 0)
  {
    cout<< "获取新的链接失败" <<endl;
    return;
  }

  int i = 0;
  for(i = 0; i < fdnum; ++i)
  {
    if(_fdarray[i] == defaultfd)
      break;
  }

  if(i == fdnum)
  {
    std::cout<< "服务器已经承受不来了" <<endl;
    close(sock);
  }
// 添加到维护的数组里面去的。
  else 
  {
    _fdarray[i] = sock;
    std::cout<< "新的文件描述符已经来了sock:" << sock <<endl;
  }

  print();
}

void Revcer(int sock, int pos)
{
  cout<< "开始接收数据了" <<endl;

  char buf[1024] = {0};
  ssize_t s = recv(sock, buf, sizeof(buf) - 1, 0);
  if(s > 0)
  {
    buf[s] = 0;
    cout<< "client#" << buf <<endl;
  }
  else if(s == 0)
  {
    close(sock);
    _fdarray[pos] = defaultfd;
    std::cout<< "clien quit" <<endl;
    return;
  }
  else 
  {
    cout<< "出现问题了" <<endl;
    close(sock);
    _fdarray[pos] = defaultfd;
    return;
  }

  string respose = _func(buf);
  
  write(sock, respose.c_str(), respose.size());
  std::cout<< "数据处理完毕" <<endl;
}

void print()
{
  for(int i = 0; i < fdnum; ++i)
  {
    if(_fdarray[i] != defaultfd)
    {
      cout<< "fd list:" << _fdarray[i] <<endl;
    }
  }
}

};
























}
