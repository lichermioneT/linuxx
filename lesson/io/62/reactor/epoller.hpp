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
 void Create()
 {
   _epfd = epoll_create(size);
   if(_epfd  < 0)
   {
     perror("epoll_create");
     exit(-1);
   }
 }

// user->kernel
 bool AddEvent(int sock, uint32_t events)
 {
    struct epoll_event ev;
    ev.data.fd = sock;
    ev.events = events;
    
    int n = epoll_ctl(_epfd, EPOLL_CTL_ADD, sock, &ev);
    
    return n == 0;
 }

// kernel->user
  int Wait(struct epoll_event revs[], int num, int timeout)
  {
    int n = epoll_wait(_epfd, revs, num, timeout);
    return n;
  }

};
