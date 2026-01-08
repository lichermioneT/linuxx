#pragma once
#include "server.h"
#include <arpa/inet.h>
#include <stdio.h>

int initListenFd()
{
  //1.创建监听fd
  int lfd = socket(AF_INET, SOCK_STREAM, 0);
  if(lfd == -1)
  {
    perror("socket");
    return -1;
  }


  return 0;
}

