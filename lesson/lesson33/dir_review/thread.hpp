#pragma once 
#include <string>
#include <iostream>
#include <pthread.h>
#include <functional>
#include <cassert>
#include <cstring>

class Thread
{
private:
  using func_t = std::function<void*(void*)>; // 类型别名的
  const int num = 1024;







};
