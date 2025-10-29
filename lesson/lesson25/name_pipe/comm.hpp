#pragma once 

#include <fcntl.h>
#include <unistd.h>
#include <cassert>
#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <cerrno>
#include <cstring>

#define NAME_PIPE "/tmp/mypipe.106"

bool createFifo(const std::string& path)
{
  umask(0);
  int n = mkfifo(path.c_str(), 0600);
  
  if(n == 0)
  {
    return true;
  }
  else 
  {
    std::cout<< "cerrno:" << errno << " err string " << strerror(errno) <<std::endl;
    return false;
  }
}

void removeFifo(const std::string& path)
{ 
  int n = unlink(path.c_str());
  assert(n == 0); // 意料之中assert
  (void)n;  // 意料之外if判断  (void)n防止被警告，小技巧
}
