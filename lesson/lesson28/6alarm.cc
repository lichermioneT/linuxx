#include <iostream>
#include <cstdio>
#include <string>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
using namespace std;

int cnt = 0;

void catSing(int sig)
{
  cout<< "捕捉到信号:" << sig <<endl;
  cout<< "cnt:" << cnt <<endl;
  exit(23);
}


int main()
{
  signal(SIGALRM, catSing);
  alarm(1);

  while(true)
  {
    cnt++;
  }

  return 0;
}
