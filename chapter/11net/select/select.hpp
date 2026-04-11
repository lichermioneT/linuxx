#include <sys/select.h>
#include <functional>
#include <string>
#include <termios.h>
#include "sock.hpp"

static const int fdnum = FD_SETSIZE;
static const int defaultfd = -1;

class Select 
{
private:
  using func_t = std::function<std::string(std::string)>;
private:
  int _listensock;
  int _port;
  int* _fdarray;
  func_t _func;

public:
  Select(func_t func, uint16_t port)
    :_listensock(-1)
    ,_port(port)
    ,_fdarray(nullptr)
    ,_func(func)
  {}
  
  ~Select()
  {
    if(_listensock >= 0) close(_listensock);
    if(_fdarray) delete[] _fdarray;
  }

void  init()
{
  _listensock = Sock::Socket();
  Sock::Bind(_listensock, _port);
  Sock::Listen(_listensock);

  _fdarray = new int[fdnum];

  for(int i = 0; i < fdnum; ++i)
  {
    _fdarray[i] = defaultfd; 
  }
}







};
