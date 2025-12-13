#pragma once
#include <string>
#include <iostream>
#include <functional>
#include <cstring>
#include "log.hpp"
using namespace std;

void serverIO(int sock)
{
  char buffer[1024];
  while (true)
  {
    ssize_t n = read(sock, buffer, sizeof(buffer) - 1); // 目前当做字符串
    if (n > 0)
    {
      // 目前我们把我们读到的数据当做字符串，截至目前
      buffer[n] = 0;
      cout << "recv message : " << buffer << endl;

      std::string outbuffer = buffer;
      outbuffer += "server[echo]";

      write(sock, outbuffer.c_str(), outbuffer.size());
    }
    else if (n == 0)
    {
      // 客户端退出了
      logMessage(NORMAL, "client quit, me to");
      break;
    }
  }

  close(sock);
}

const std::string oper = "+-*/%";
class task
{
  using func_t = std::function<void(int)>;

public:
  task() {}
  task(int sock, func_t func):_sock(sock), _callbacl(func)
  {
  }

  void operator()()
  {
    _callbacl(_sock);
  }

private:
  int _sock;
  func_t _callbacl;
};

class savetask
{
  typedef std::function<void(const std::string &)> func_t; // 函数模板吧？
public:
  savetask() {}
  savetask(const std::string &message, func_t func) : _message(message), _func(func)
  {
  }

  void operator()()
  {
    _func(_message);
  }

private:
  std::string _message;
  func_t _func;
};



void Save(const std::string &message)
{
  const std::string target = "./lag.txt";
  FILE *fp = fopen(target.c_str(), "a+");

  if (!fp)
  {
    std::cerr << "fopen error" << std::endl;
  }

  fputs(message.c_str(), fp);
  fputs("\n", fp);
  fclose(fp);
}
