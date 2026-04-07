#pragma once 
#include <iostream>
#include <unistd.h>
#include <fcntl.h>

class util
{
public:
  static void setNonBlock(int fd)
  {
    int flag = fcntl(fd, F_GETFL);
    if(flag < 0)
    {
      perror("fcntl");
      return;
    }
    int ret = fcntl(fd, F_SETFL, flag | O_NONBLOCK);
    if(ret == -1)
    {
      perror("fcntl");
      return;
    }
  }
};


