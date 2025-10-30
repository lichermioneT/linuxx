#include <iostream>
#include <signal.h>

#define BLOCK_SIGNAL 2

int main()
{

// 1.屏蔽指定的信号
  sigset_t block, oblock;
// 1.1初始化
  sigemptyset(&block);
  sigemptyset(&oblock);
// 1.2添加屏蔽信号
  sigaddset()
// 1.2开始屏蔽
  sigprocmask(SIG_SETMASK, &block, &oblock);






  return 0;
}
