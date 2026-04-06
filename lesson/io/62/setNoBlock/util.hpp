#pragma once 

#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <cerrno>

void setNoBlock(int fd)
{
//1.先获取文件描述符的状态标志
  int f1 = fcntl(fd, F_GETFL);
  if(f1 == -1)
  {
    std::cerr << "fcntl:" << strerror(errno) << std::endl;
    return;
  }

//2.通过按位或追加一个标志
  int ret = fcntl(fd, F_SETFL, f1 | O_NONBLOCK);

//3.严谨一点的这里可以判断返回的信息
  if(ret == -1)
  {
    std::cerr << "fcntl:" << strerror(errno) << std::endl;
    return;
  }
}

void printLog()
{
  std::cout<< "this is a log" << std::endl;
}

void download()
{
  std::cout<< "this is a download" << std::endl;
}

void executeSql()
{
  std::cout<< "this is a executeSql" << std::endl;
}
