#include "util.hpp"
#include <stdio.h>
#include <functional>
#include <vector>
using func_t = std::function<void()>;

#define INIT(V) do{\
        V.push_back(printlog);\
        V.push_back(download);\
        V.push_back(execuate);\
        }while(0);

#define EXEC_OTHER(cbs) do{\
    for(auto const& cb : cbs) cb();\
}while(0)


int main()
{
  std::vector<func_t> cbs;
  INIT(cbs);

  setNoneBlock(0);
  char buffer[1024] = {0};
  while(true)
  {
    /*
     *printf(">>> ");
     *fflush(stdout);
     */
    ssize_t s = read(0, buffer, sizeof(buffer) - 1);
    if(s > 0)
    {
      buffer[s] = 0;
      std::cout<< "echo#: "<< buffer;
    }
    else if(s == 0) 
    {
      std::cout<< "read end" <<std::endl;
      break;
    }
    else 
    {

      /*
       *std::cout<< "result : " << s <<" : " << strerror(errno) <<std::endl; // 值是-1,不输入的时候算错误吗
       */
      /*
       *std::cout<< "EAGAIN : " << EAGAIN << " EWOULDBLOCK : " << EWOULDBLOCK <<std::endl;
       */
      // 1.不算做错误，只不过是以错误的形式返回了。
      // 2.我如何区分是真的错了，还是底层没有数据？
      //    单纯返回值，无法区分的
      //
      
      if(errno == EAGAIN || errno == EWOULDBLOCK)
      {
        std::cout<< "我没错 只是没有数据" <<std::endl;
        EXEC_OTHER(cbs);
      }
      else if(errno == EINTR)
      {
        continue;
      }
      else 
      {
       std::cout<< "result : " << s <<" : " << strerror(errno) <<std::endl; // 值是-1,不输入的时候算错误吗
       break;
      }

    }

    sleep(1);
  }








  return 0;
}
