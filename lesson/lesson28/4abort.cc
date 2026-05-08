#include <iostream>
#include <cstdio>
#include <string>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
using namespace std;

int main()
{
  for(int i = 0; i < 10; ++i)
  {
    std::cout<< "i:" << i << std::endl;
    if(i == 4)
    {
      abort(); // 信号六
    }
  }

  return 0;
}
