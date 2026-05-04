#include <iostream>
#include <sys/wait.h>
#include <sys/types.h>
#include <vector>
#include <cstdlib>
#include <unistd.h>
#include <cassert>
#include <string>
#include <sys/types.h>
#include <functional>
#include <ctime>
#include <unistd.h>

using namespace std;

#define PROCESS_NUM 5
#define Make_Seed() srand((unsigned long)time(nullptr)^getpid()^0x1112^rand()%1234)

typedef void(*func_t)(); // 函数指针 类型

void downLoadTask()
{
  cout<< getpid()<<":下载任务" <<endl;
}

void ioTask()
{
  cout<< getpid() <<":io任务" <<endl;
}

void flushTask()
{
  cout<<getpid() <<":刷新任务"<<endl;

}

// 1.放任务
void loadTaskFunc(vector<func_t>* out)
{
  assert(out);
  out->push_back(downLoadTask);
  out->push_back(ioTask);
  out->push_back(flushTask);
}




int main()
{
  Make_Seed();




  return 0;
}
