#pragma once 
#include <string>
#include <iostream>
#include <functional>
#include <cstring>

class task
{
  using func_t = std::function<int(int, int, char)>;
public:
  task(){}
  task(int x, int y, char op, func_t func):_x(x), _y(y), _op(op), _callbacl(func)
  {}
  
  std::string operator()()
  {
    int result = _callbacl(_x, _y, _op);
    char buffer[1024];
    snprintf(buffer, sizeof(buffer),"%d %c %d = %d", _x, _op, _y, result);
    return buffer;
  }

private:
  int _x;
  int _y;
  char _op;
  func_t _callbacl;
};

class savetask
{
  typedef std::function<void(const std::string&)> func_t; // 函数模板吧？
public:
  savetask(){}
  savetask(const std::string& message, func_t func):_message(message), _func(func)
  {}

  void operator()()
  {
    _func(_message);
  }
private:
  std::string _message;
  func_t _func;
};

void Save(const std::string& message)
{   
  const std::string target = "./lag.txt";
  FILE* fp = fopen(target.c_str(), "a+");

  if(!fp)
  {
    std::cerr<< "fopen error" <<std::endl;
  }
  
  fputs(message.c_str(), fp);
  fputs("\n", fp);
  fclose(fp);
}
