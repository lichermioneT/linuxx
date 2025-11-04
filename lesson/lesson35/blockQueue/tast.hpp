#pragma once 

#include <iostream>
#include <functional>


class task
{
  using func_t = std::function<int(int, int)>;
public:
  task(){}
  task(int x, int y, func_t func):_x(x), _y(y), _callbacl(func)
  {}
  
  int operator()()
  {
    int result = _callbacl(_x, _y);
    return result;
  }



private:
  int _x;
  int _y;
  func_t _callbacl;
};

