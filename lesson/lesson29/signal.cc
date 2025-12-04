#include <iostream>
#include <vector>
#include <signal.h>
#include <unistd.h>
using namespace std;

#define BLOCK_SIGNAL 2
#define MAX_SIGNUM 31

static void show_pendings(const sigset_t& pendings)
{
  for(int signo = MAX_SIGNUM; signo >= 1; signo--)
  {
      if(sigismember(&pendings, signo))
      {
        cout<< "1";
      }
      else  
      {
        cout << "0";
      }
  }
  cout<<endl;
}

static vector<int> sigarr = {2, 3};

static void myhander(int signo)
{
    cout<<  signo << "号信号已经被抵达！！"<<endl;
}

int main()
{

  for(const auto&  sig : sigarr) signal(sig,myhander); // 捕捉了信号

// 1.屏蔽指定的信
  sigset_t block, oblock, pendings;

// 1.1初始化
  sigemptyset(&block);
  sigemptyset(&oblock);
  sigemptyset(&pendings);

// 1.2添加屏蔽信号
  // sigaddset(&block, BLOCK_SIGNAL);
  // 9号信号应该不能打，这样子OS就太垃圾了
  for(auto & e : sigarr)
  {
    sigaddset(&block, e);
  }

// 1.3开始屏蔽, 设置进内核(进程)
  sigprocmask(SIG_SETMASK, &block, &oblock);
 
// 遍历打印pending信号集
  int cnt = 10;
  while(true)
  {
    //2.1初始化
    sigemptyset(&pendings);
    //2.2获取
    sigpending(&pendings);
    //2.3打印
    show_pendings(pendings);
    sleep(1);

// 解除屏蔽信号
    if(cnt-- == 0)
    {
      sigprocmask(SIG_SETMASK, &oblock, &block); // 一旦对特定信号解除屏蔽，一般OS要立马抵达一个信号
      cout<< "恢复对信号的屏蔽，不屏蔽任何信号" <<endl;
    }
  }
  return 0;
}
