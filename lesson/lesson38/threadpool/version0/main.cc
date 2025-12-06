#include "thread.hpp"
using namespace std;

void* run(void* args)
{ 
    while(true)
    {
      std::string message = (const char*)args;
      cout<< "message = " << message <<endl;
    }
    return nullptr;
}

int main()
{

  ThreadNs::Thread t1(run, (void*)"thread 1  run");
  ThreadNs::Thread t2(run, (void*)"thread 2  run");

  t1.start();
  t2.start();


  t1.join();
  t2.join();

}





















