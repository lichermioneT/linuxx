#include "util.hpp"
#include <vector>
#include <cstdio>
#include <functional>

using func_t = std::function<void()>;

#define INIT(cbs) do{\
      cbs.push_back(printLog);\
      cbs.push_back(download);\
      cbs.push_back(executeSql);\
    }while(0)

#define EXEC_OTHER(cbs) do{\
      for(auto const& cb : cbs) cb();\
}while(0)


int main()
{
  std::vector<func_t> cbs;
  INIT(cbs);

// 1.设置成非阻塞模式的
//   读数据的时候，没有就立即返回的。
  setNoBlock(0);

  char buffer[1024] = {0};

// 2.有点类似轮询的场景的
  while(true)
  {
    printf(">>>>");
    fflush(stdout);
    ssize_t s = read(0, buffer, sizeof(buffer) - 1);
    if(s > 0)
    {
      buffer[s] = 0;
      std::cout<< "echo#" << buffer << std::endl;
    }
    else if(s == 0)
    {
      std::cout<< "read end" << std::endl;
      break;
    }
    else 
    {
      // s==-1
      // 1.当我不输入的时候，底层没有数据，算错误吗？不算错误，只不过以错误的形式返回了
      // 2.那我如何区：是真的错了，还是底层没有数据？(单纯返回值不能够区分的，还需要根据错误码进行判断的)
      /*
       *std::cout<< "s:" << s << " erron:"<< errno <<std::endl; // errno是11，临时资源没有准备好的
       */
      /*
       *std::cout<< "s:" << s << " erron:"<< strerror(errno) <<std::endl;
       */

      /*
       *std::cout<< "EAGAIN:" << EAGAIN << " EWOULDBLOCK:" << EWOULDBLOCK << std::endl; // 两个都是11的。
       */
// 非阻塞返回值是-1的，还需要根据错误码判断，

       std::cout<< "s:" << s << " erron:"<< errno <<std::endl;

// 1.没有错误的,按照错误的方式返回，需要根据错误码进行判读的
       if(errno == EAGAIN || errno == EWOULDBLOCK)
       {
         std::cout<< "我没有错，只是没有数据而已" << std::endl;
         EXEC_OTHER(cbs);
       }
// 2.需要重新读取数据的,读取被中断了，需要继续读取数据的。
       else if(errno == EINTR)
       {
          continue;
       }
// 3.真正的错误了。
       else 
       {
        std::cout<< "s:" << s << " erron:"<< errno <<std::endl; // 真正的错误了
        break;
       }
      sleep(3);
    }
  }
  return 0;
}
