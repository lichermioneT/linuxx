#include <iostream>
#include <unistd.h>
#include <thread>
using namespace std;

void run()
{
  sleep(1);
  cout<< "这是一个子线程" <<endl;
}

int main()
{ 
  thread t(run);
  cout<< "这是一个主线程" <<endl;

  t.join();

  return 0;
}
