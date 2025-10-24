#include <iostream>
#include <cstdio>
#include <string>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
using namespace std;

void hander(int signo)
{
  cout<<"进程捕捉到了一个信号，信号的编号是：" << signo <<endl;
  // exit(0);
}

// 使用方法
static void Usage(const string& proc)
{
  cout<< "\nUage: " << proc << " pid signo \n" <<endl;
}


int cnt = 0;
void catchSig(int signo)
{
  cout<< "获取到一个进程 信号编号是 ：" << signo << cnt <<endl;
  alarm(1);
}


int main(int agrc, char* agrv[])
{
 
  
  while(true)
  {
    int a[10];
    a[10000] = 106;
  }





// 软件条件产生信号
// 统计一秒左右，我们的计算机能够计算多少次 

/*
  signal(SIGALRM, catchSig);
  alarm(1); // 一次性闹钟
  while(true)
  {
    cnt++;
  }
*/


 //  产生信号的方式：硬件异常产生信号
 // 信号的产生，不一定非得用户显示的发送
 //
 // 为什么除零会终止进程？当前进程会收到来自OS系统的信号
 


// 
//  signal(SIGFPE, catchSig);
//  signal(SIGFPE, catchSig);

/*
    signal(11, catchSig);
 
  while(true)
  {
    cout<<"我在运行。。。"<<endl;
    sleep(1);
    int a = 10;
  //  a /= 0;
  //
  int* p = nullptr;
  *p = 0;
  }

*/


















  // signal函数的调用，并不是hander函数的调用
  // 仅仅是对2号信号的捕捉方法，并不代表该方法调用了
  // 一般这个方法不会执行
  // signal(2, hander);

  // 系统调用发送信号
  // kill 函数
  // kill(pid, sig)
  // ./pro pid signo 
 
  // kill()
  // raise()
  // abort()
  
/*
  int cnt = 0;

  while(cnt < 10)
  {
    printf("cnt : %d \n", cnt++);
    sleep(1);
    if(cnt >= 5) abort();
    //if(cnt >= 5) raise(3);
  }
*/

/*

  if(agrc != 3)
  {
    Usage(agrv[0]);
    exit(1);
  }

  pid_t id = atoi(agrv[1]);
  int signo = atoi(agrv[2]);
  
  int n = kill(id, signo);
  if(n != 0)
  {
    perror("kill fail");
  }

*/ 

/*  

  while(true)
  {
    cout<< "i am process " << endl;
    sleep(2);
  
}
*/ 
  return 0;
}
