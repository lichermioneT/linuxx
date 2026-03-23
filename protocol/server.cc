#include "server.hpp"
#include <memory>


// req:里面一定是我们的处理好的一个完整请求对象
// resp:根据req，进行业务处理，填充resp, 不需要管任何，序列化，反序列化，io等等的。
bool cal(const request& req, response& resp)
{
  resp.exotcode = 0;
  resp.result = 0;

  switch(req._op)
  {
    case '+' : resp.result = req._x + req._y; break;
    case '-' : resp.result = req._x - req._y; break;
    case '*' : resp.result = req._x * req._y; break;
    case '/' : 
               {
                  if(req._y == 0)
                  {
                    resp.exotcode = -1;
                  }
                  else 
                  {
                    resp.result = req._x / req._y;
                  }
                break;
               }

    case '%' : 
               {
                  if(req._y == 0)
                  {
                    resp.exotcode = -2;
                  }
                  else 
                  {
                    resp.result = req._x / req._y;
                  }
                break;
               }
    default:
               {
                 resp.exotcode = -3;
                 break;
               }
  }
  return true;
}

int main(int argc, char* argv[])
{
  
  uint16_t port = atoi(argv[1]);
  unique_ptr<server> ser(new server(port));
  
  ser->init();
  ser->start(cal);





  return 0;
}
