#include <pthread.h>
#include <cstdlib>
#include <vector>
#include <unistd.h>
#include <string>
#include <iostream>
using namespace std;


// 当结构体使用
// 里面存放线程的序号，id, 名字
class ThreadData
{
public:
  int number;
  pthread_t tid;
  char namebuffer[64];
};

class ThreadReturn
{
public: 
  int exit_code;
  int exit_result;
};

// 1.现在被几个执行流调用呢？10个。函数是什么状态？ 可重入状态 多个执行流同时执行
// 2.该函数是可重入函数吗？是的。
// 3.函数内部定义的变量，局部变量，具有临时属性，今天多线程任然合适。 每一个线程都有自己的独立的栈结构。
void* start_routine(void* args)
{
  // exit(0); eixt用来终止进程的，任何一个执行流调用exit，整个进程退出
  // 一个线程出现异常，会影响其它线程的
  // 进程信号，信号是发给整体发给进程的！
    ThreadData* td = static_cast<ThreadData*>(args);
    int cnt = 1;
    while(cnt)
    {
      // cout<< "new thread  create success : " << td->namebuffer << "cnt : " << cnt-- <<endl;
      //  cout<< "cnt : "  << cnt << "---&cnt : " << &cnt <<endl;
      cout<< td->namebuffer <<endl;
      cnt--;
      sleep(1);
    }

  // 如何终止
  //  delete td;
  
  // 线程函数的返回值！！
  //  pthread_exit(nullptr);
  //  return nullptr;  // 线程函数结束，return的时候，就算结束了。
  //  return (void*)td->number; // void* ret = (void*)td->number;
  //  return (void*)106;
  //  pthread_exit((void*)106);
  
  ThreadReturn * tr = new ThreadReturn();
  tr->exit_code = 1;
  tr->exit_result = 106;
 
  // 不能直接创建对象，然后返回对象的地址，因为这个栈帧结束了 
  return (void*)tr; // 右值
}



int main()
{

// 创建一批线程
  vector<ThreadData*> threads;
#define NUM 10
  for(int i = 0; i < NUM; i++) // 循环创建是个线程
  {
   // pthread_t tid;
   ThreadData* td = new ThreadData(); // 创建一个存放线程信息的对象
   td->number = i + 1;                // 线程id
   snprintf(td->namebuffer, sizeof(td->namebuffer), "%s: %d", "thread", i+1); // 线程name
   pthread_create(&td->tid, nullptr, start_routine, td); // 创建线程，并且把线程的信息给执行的函数 

   threads.push_back(td); // 指针放到数组里面去
   // char namebuffer[64];
   //  snprintf(namebuffer, sizeof(namebuffer), "%s : %d", "thread", i);
   // pthread_create(&tid, nullptr, start_routine, (void*)"thread one");
   //  pthread_create(&tid, nullptr, start_routine, namebuffer);
   // sleep(1);
  }

/*
  for(auto& iter : threads) // 打印线程对象的信息
  {
    cout<<" create thread:" << iter->namebuffer << " : " << iter->tid << "success" <<endl;
  }

*/ 
  sleep(5);
// 线程是可以被cancel的，线程要被取消，前提是这个线程已经跑起来了
// 线程被取现，退出码是-1 宏
  for(auto& iter : threads)
  {
    pthread_cancel(iter->tid);
  }
 
/*
// 等待线程
  for(auto& iter : threads)
  {
    //void* ret = nullptr;
    ThreadReturn *ret = nullptr;
    // 线程退出的退出码,怎么没有看到呢？ 线程出异常，收到信号，整个进程都会退出！
    // pthread_join:默认就会认为函数会调用成功，不考虑异常的问题 ，异常是你进程该考虑的问题。
    pthread_join(iter->tid, (void**)&ret); // void** retp; *ret = void*  二级指针操作一级指针  ret = void*
    cout<< " join " << " ： " << iter->namebuffer << " success number " << iter->number <<endl;
    delete iter; // 外部释放
  }
  
  cout<< " main thread quit " <<endl;
*/


/*
  while(true)
  {
      cout<< "main thread" <<endl;
      sleep(1);
  }
*/ 

/*
 *
  pthread_t id;
  pthread_create(&id, nullptr, start_routine, (void*) "pthread_one");

  while(true)
  {
      cout<< "main thread" <<endl;
      sleep(1);
  }

*/
  return 0;
}
