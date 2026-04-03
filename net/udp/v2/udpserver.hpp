#pragma once
#include <iostream>
#include <string>
#include <cstring>
#include <cerrno>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
using namespace std;

class udpserver
{
public:
  udpserver(uint16_t port);
  ~udpserver();


public:
  void init();
  void start();

private:
  void createSocket();
  void Bind();


private:
  uint16_t _port;
  int _sock;
};
