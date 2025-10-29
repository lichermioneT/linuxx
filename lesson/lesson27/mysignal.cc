#include <iostream>
#include <unistd.h>
#include <signal.h>
using namespace std;

void hander(int signo)
{
  cout<<"进程捕捉到了一个信号，信号的编号是：" << signo <<endl;
  // exit(0);
}


int main()
{
 
  // signal函数的调用，并不是hander函数的调用
  // 仅仅是对2号信号的捕捉方法，并不代表该方法调用了
  // 一般这个方法不会执行
  signal(2, hander);

  while(true)
  {
    cout<< "i am process " << endl;
    sleep(2);
  }


  return 0;
}
