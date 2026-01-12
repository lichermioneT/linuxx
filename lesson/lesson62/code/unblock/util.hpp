#pragma once 
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <cerrno>

void setNoneBlock(int fd)
{
  int f1 = fcntl(fd, F_GETFL); // 获取文件描述符的状态
  if(f1 == -1)
  {
    std::cerr<< "fcntl : " << strerror(errno) <<std::endl;
    return;
  }
  fcntl(fd, F_SETFL, f1 | O_NONBLOCK); // 添加新的状态，设置为非阻塞状态
}


void printlog()
{
  std::cout<< "this is a log"<<std::endl;
}

void download()
{
  std::cout<< "this is a download" <<std::endl;
}

void execuate()
{
  std::cout<< "this is a execuate "<<std::endl;
}

