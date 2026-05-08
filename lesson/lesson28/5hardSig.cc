#include <iostream>
#include <cstdio>
#include <string>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
using namespace std;

void catSing(int sig)
{
  cout<< "捕捉到信号:" << sig <<endl;
}

int main()
{
  signal(SIGFPE, catSing);

  for(int i = 0; i < 10; ++i)
  {
    cout<< "a process pid:" << getpid() <<endl;
    sleep(1);
    int a = 10;
    a /= 0;
  }

  return 0;
}
