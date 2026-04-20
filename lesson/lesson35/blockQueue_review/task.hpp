#include <functional>

class task 
{
private:
  using func_t = std::function<int(int, int)>;

public:
  task()
  {}
  task(int x, int y, func_t func)
    :_x(x)
    ,_y(y)
    ,_func(func)
  {}

  int operator()()
  {
    int result = _func(_x, _y);
    return result;
  }

private:
  int _x;
  int _y;
  func_t _func;
};
