#pragma once
#include <iostream>
#include <string>
#include <cstring>
#include <cerrno>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
using namespace std;

class udpclient 
{
public:
  udpclient(const string& ip, uint16_t port);
  ~udpclient();
  void init();
  void start();

private:
  bool createSocket();
  void Send();

private:
  string _ip;
  uint16_t _port;
  int _sock;
};
