#pragma once
#include "sock.hpp"
#include <string>
#include <functional>
#include <iostream>

using func_t = std::function<std::string(const std::string&)>;
static const uint16_t defaultport = 8080;
static const int fdnum = sizeof(fd_set)*8;
static const int defaultfd = -1;

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
      _listensock = defaultfd;
    }

    if(_fdarray)
    {
      delete[] _fdarray;
    }
  }

public: 
  void init()
  {
    _listensock = Sock::Socket();
    Sock::Bind(_listensock, _port);
    Sock::Listen(_listensock);

    _fdarray = new int[fdnum];
    for(int i = 0; i < fdnum; ++i)
    {
      _fdarray[i] = defaultfd;
    }

    _fdarray[0] = _listensock;
  }

  void start()
  {
    for(;;)
    {
      fd_set rfds;
      FD_ZERO(&rfds);
      
      struct timeval timeout = {3, 0};
      int maxfd = _fdarray[0];

      for(int i = 0; i < fdnum; ++i)
      {
        if(_fdarray[i] == defaultfd)
          continue;
      
        FD_SET(_fdarray[i], &rfds);

        if(maxfd < _fdarray[i])
          maxfd = _fdarray[i];
      }
      
      int n = select(maxfd + 1, &rfds, nullptr, nullptr, &timeout);
      switch(n)
      {
        case  0:
          std::cout<< "timeout....规定时间没有事件就绪的" << std::endl;
          break;
        case -1:
          std::cout<< "select error " << errno << strerror(errno) <<std::endl;
          break;
        default:
          handerEvent(rfds);
          std::cout<< "有新的事件就绪了" << std::endl;
          break;
      }
    }
  }

private:
  void handerEvent(fd_set& rfds)
  {
    for(int i = 0; i < fdnum; ++i)
    {
      if(_fdarray[i] == defaultfd)
        continue;

      if(FD_ISSET(_fdarray[i], &rfds) && _fdarray[i] == _listensock)
      {
        Accepter(_fdarray[i]);
      }
      else if(FD_ISSET(_fdarray[i], &rfds))
      {
        // 处理就绪的时间
        Recver(_fdarray[i], i);
      }
      else 
      {
        // errno
      }
    }
  }

  void Accepter(int listensock)
  {
    std::string clientip;
    uint16_t clientport;
    
    int sock = Sock::Accept(listensock, &clientip, &clientport);
    if(sock < 0)
    {
      perror("Accepter");
      return;
    }

    int i = 0;
    for(i = 0; i < fdnum; ++i)
    {
      if(_fdarray[i] != defaultfd)
      {
        continue;
      }
      else 
      {
        break;
      }
    }

    if(i == fdnum)
    {
      std::cout<< "已经到了服务器的极限了" <<std::endl;
      close(sock);
    }
    else 
    {
      _fdarray[i] = sock;
      std::cout<< "新的链接正式建立了 sock :" << sock << std::endl;
    }
  }

  void Recver(int sock, int pos)
  {
    char buffer[1024];
    ssize_t s = recv(sock, buffer, sizeof(buffer) - 1, 0);
    if(s > 0)
    {
      buffer[s] = 0;
    }
    else if(s == 0)
    {

    }
    
    std::cout<< buffer << std::endl;

    (void)pos;
  }


};
