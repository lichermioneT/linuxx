#include <iostream>
#include <cstdio>
#include <string>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
using namespace std;

void sig(int number)
{
  cout<< "捕捉到信号:"<< number <<endl;
}


int main()
{
  signal(2,sig);
    
  while(true)
  {
    sleep(1);
    cout<< "a process pid:%d" << getpid() <<endl;
  }


  return 0;
}
