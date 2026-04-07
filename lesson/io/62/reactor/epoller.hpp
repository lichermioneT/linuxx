#pragma once
#include <iostream>
#include <sys/epoll.h>
#include <unistd.h>

const static int defaultepfd = -1;
const static int size = 128;

class epoller
{
private:
  int _epfd;

public:
  epoller()
    :_epfd(-1)
  {}

  ~epoller()
  {
    if(_epfd != defaultepfd)
    {
      close(_epfd);
      _epfd = defaultepfd;
    }
  }

public:

// 创建epoll实例
 void Create()
 {
   _epfd = epoll_create(size);
   if(_epfd  == -1)
   {
     perror("epoll_create");
     exit(-1);
   }
 }

// user->kernel
// 链接上树
 bool AddEvent(int sock, uint32_t events)
 {
    struct epoll_event ev;
    ev.data.fd = sock;
    ev.events = events;
    
    int n = epoll_ctl(_epfd, EPOLL_CTL_ADD, sock, &ev);
    
    return n == 0;
 }

// kernel->user
// 就绪队列里面，拿文件描述符
  int Wait(struct epoll_event revs[], int num, int timeout)
  {
    int n = epoll_wait(_epfd, revs, num, timeout);
    return n;
  }
  
  bool Control(int sock, uint32_t event, int action)
  {
    int n = 0;
    if(action == EPOLL_CTL_MOD)
    {
      struct epoll_event ev;
      ev.events = event;
      ev.data.fd = sock;

      n = epoll_ctl(_epfd, action, sock, &ev);
    }
    else if(action == EPOLL_CTL_DEL)
    {
      n = epoll_ctl(_epfd, action, sock, nullptr);
    }
    else 
    {
      n = -1;
    }

    return n == 0;
  }

};
