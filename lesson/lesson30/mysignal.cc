#include <iostream>
#include <signal.h>
#include <unistd.h>
using namespace std;

void Count(int cnt)
{
  while(cnt)
  {
    printf("cnt: %d",cnt);
    fflush(stdout);
    cnt--;
    sleep(1);
    printf("\n");
  }
}

void hander(int signo)
{
  cout<< "get a signo : " << signo << "正在处理中" <<endl;
  Count(10);
}

int main()
{
  struct sigaction act,oact;
  act.sa_handler = hander;
  act.sa_flags = 0;

  sigemptyset(&act.sa_mask); // 正在处理某一种信号的时候，我们也想屏蔽其它信号，就可以添加到这个sa_mask中
  sigaddset(&act.sa_mask, 3);
  sigaction(SIGINT, &act, &oact);

  while(true)
  {
    sleep(1);
  }
  return 0;
}
