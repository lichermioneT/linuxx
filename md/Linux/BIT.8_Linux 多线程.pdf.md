# BIT.8_Linux 多线程.pdf

## 1线程

![image-20251202085959658](E:/gitee/lin/linuxx/md/Linux/picture/image-20251202085959658.png)



**进程 = 内核数据结构 + 进程对应的代码和数据**

**线程：进程内的一个执行流。(一般定义)**

**1.OS都太宏观了，太抽象了。**

**2.具体化，Linux**



**如何看待虚拟内存？虚拟内存里面决定了进程能够看待的"资源" 主体资源，**

**只是创建PCB(进程控制块) 通过mm_struct看到同一块资源**

**因为通过虚拟地址空间+页表的方式进行进程的资源划分，单个"进程"的执行 力度，一定比之前的进程细**



**OS真的存在，那么应该如何管理起来呢？怎么管理？先描述，后组织。**

**CPU只关注指令，不关注你是进程还是线程。**



**描述**

**一定要为线程设计专门的数据结构，表示线程对象。TCB(thread control block) 线程控制块。(windows，单独的线程块) (Linux, 复用PCB-->TCB)**

**线程被执行：调度(id, 状态， 优先级， 上下文，栈) 单纯调度的时候，进程和线程有很多地方是重叠的。**

**linux工程师，我们不想给Linux线程专门设计数据机构，直接复用线程PCB， 用PCB来标识Linux内部的线程**



**组织**

**1.线程在进程内部运行，线程在进程地址空间内运行，有地址空间的一部分数据。**



**进程的新概念：内核视角 ： 承担分配系统资源的基本实体。**

**在linux中，线程，CPU调度的基本单位**

**一个进程内部的执行流！----》线程。**

**task_struct轻量级进程！**





![image-20251202092605407](picture/image-20251202092605407.png)

**linux内核中有没有真正意义上的线程？严格意义上没得，Linux使用进程PCB来模拟线程的，是一套完全属于自己的一套线程方案。**

**cpu的视角，每一个PCB都是轻量级进程。**

**linux线程是CPU调度的基本单位，而进程是承担分配系统资源的基本单位**

**进程用来申请资源，线程用来伸手向进程要资源。**



**好处**

**简单，维护成本大大降低---可靠，高效！**

**缺点**

**线程并行的运行各种程序：电影边下载边观看。 在一个进程里面。**

**linux无法直接提供创建线程的系统调用接口，而只能给我们提供轻量级进程的接口！**

**-lpthread 线程的库**

**OS只认线程**

**用户级别线程库--->原生线程库**





**线程间通信那简直就是很简单的！！！**

**线程一旦被创建，几乎所有的资源都是被所有线程共享的！**

**线程也一定要有自己的私有属性内容！什么资源是私有的呢？**

**1.pcb里面的属性**

**2.线程切换，上下文**

**3.每一个线程都要有自己独立的栈结构：**



**动静态库！！！**

```c++
#include <iostream>    
#include <pthread.h>    
#include <unistd.h>    
#include <cassert>    
#include <cstdio>    
using namespace std;    
int g_val = 10;
    
void fun()    
{    
  cout<< "this is a new method" <<endl;    
}    
    
// 新线程，    
void* thread_routine(void* agrs)                                                                                                         
{    
  const char* name = (const char*)agrs;    
  while(true)    
  {    
     cout << "我是新线程， 我真正运行 ！" << "name : " << name <<endl;    
     sleep(2);    
  }    
  return nullptr;    
}    
    
int main()    
{    
  pthread_t tid;    
  int n = pthread_create(&tid, nullptr, thread_routine, (void*)"thread_one");    
  assert(0 == n);    
  (void)n;    
    
  // tid是地址    
   char buffer[64];    
   // 主线程    
   snprintf(buffer, sizeof(buffer), "0x%zx \n", tid);    
      while(true)    
   {    
     cout << "我是主线程， 我真正运行! " << "tid : " << tid << endl;;    
     sleep(2);
   }
  return 0;
}
```

```makefile
mythread:mythread.cc                                                                                                                              
  g++ -o $@ $^ -lpthread    
    
.PHONY:clean    
clean:    
  rm -rf mythread  
```

**ps -aL**

![image-20251202101614419](picture/image-20251202101614419.png)

**调度的标识符 LWP**

**单进程lwp == PID**



![image-20251202095215120](picture/image-20251202095215120.png)



**与进程之间的切换相比，线程之间的切换需要操作系统做的工作要少很多**

**少呢些？：**

**切换页表**

**虚拟地址空间**

**0**

**切换PCB**

**上下文**

**cache切换不用太更新，进程切换全部更新**

**cpu的cache：硬件级别的缓存，局部性原理。先从cache里面读取。cache里面缓冲很多热点数据。线程的共享数据基本上都在cache里面。cache基本上不用切换>    的。进程切换，cache全部切换。**



**线程也一定要有自己的私有属性内容！什么资源是私有的呢？**

**1.pcb里面的属性**

**2.线程切换，上下文**

**3.每一个线程都要有自己独立的栈结构：**

**进程地址空间只有一个啊，怎么保存线程私有呢？**



**计算密集型 CPU资源**

**IO密集型   访问磁盘 显示 器网络**



## 2线程控制

**PCB模拟线程**

**TCB---->mm_struct**

**linux无法直接提供创建线程的系统调用接口，而只能给我们提供轻量级进程的接口！**



![image-20251202203110647](picture/image-20251202203110647.png)



```c++
#include <pthread.h>
#include <cstdlib>
#include <vector>
#include <unistd.h>
#include <string>
#include <iostream>
using namespace std;

void* start_routine(void* args)
{
    string name = static_cast<const char*>(args);
    while(true)
    {
        cout<< "this is a new thread, name is : " << name <<endl;
        sleep(1);
        int* p = nullptr;
        *p = 100;
    }
}

int main()
{

    pthread_t id = 0;
    pthread_create(&id, nullptr, start_routine, (void*)"thread new");

    while(true)
    {
        cout<< "main stream " <<endl;
        sleep(1);
    }
    return 0;
}
```



![image-20251202205159394](picture/image-20251202205159394.png)



**信号是发给线程的！**

**所以主线程，也就退出了**

**每个线程发送信号**

**进程不会的，独立性。独立的内核数据结构。**



**线程进程组合**

![image-20251202210729681](picture/image-20251202210729681.png)

**fork()---->底层是---->clone().**

**线程底层也是clone()**

**vfork() 轻量级进程的，一般不用了解的，一般是库在使用的。**



**线程控制**

**创建线程一般不用设置属性，除非您是高手，很了解的Linux操作系统。**



**线程创建**

```c++
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

// 可重入函数的
void* start_routine(void* args)
{   
    // 每个线程独立的栈帧结构的
    ThreadData* td = static_cast<ThreadData*>(args);
    int cnt = 10;  
    while(cnt)
    {
      cout<< td->namebuffer << "---cnt : " << cnt <<endl;
      cnt--;
      sleep(1);
    }
    
    // 证明了独立的栈结构
    // int cnt = 10
    // cout<< "&cnt " << &cnt <<endl;
    
    delete td;
  return nullptr;
}

int main()
{
// 创建一批线程
  vector<ThreadData*> threads;
#define NUM 10
  for(int i = 0; i < NUM; i++)                                                // 循环创建是个线程
  {
    ThreadData* td = new ThreadData();                                         // 创建一个存放线程信息的对象
    td->number = i + 1;                                                        // 线程id
    snprintf(td->namebuffer, sizeof(td->namebuffer), "%s : %d", "thread", i+1); // 线程name
    pthread_create(&td->tid, nullptr, start_routine, td);                      // 创建线程，并且把线程的信息给执行的函数 td拷贝给线程
    threads.push_back(td);                                                     // 指针放到数组里面去
  } 

  for(auto& iter : threads) // 打印线程对象的信息
  {
    cout<<" create thread:" << iter->namebuffer << " : " << iter->tid << " success" <<endl;
  }
    
    while(true)
    {
        cout<< "main stream " <<endl;
        sleep(1);
    }
}
```



**线程终止**

**如果是exit,会让整个进程退出了，所以不能调用它。**

**pthread_exit()**

**线程函数里面放return **



**线程等待问题**

**线程的等待问题！如果不等待，会造成类似僵尸进程的问题--内存泄漏**

**线程必须被等待**

**1.获取新线程的退出信息 ---可以不关心退出信息**

**2.回收新线程对应的PCB等内核资源，防止内存泄漏。--暂时无法查看**



**int pthread_join(pthread_t thread, void* * retval); // 用来获取线程函数结束时，返回的退出结果！**

**因为执行的函数的返回值是* ，为了能够接受到，所以就是星星 **



**线程的返回值问题**



**你想改什么变量，就必须传给函数它的地址。**

⭐ **最精确的人类语言解释**

✔ pthread 返回一个 “指针”，要把这个指针放到 ret 里

✔ 想修改 ret，必须传入 ret 的地址

✔ ret 的地址是 ThreadReturn**

✔ pthread_join 要的就是 void**

✔ 类型兼容，所以写 (void**)&ret

```c++
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

void* start_routine(void* args)
{

    ThreadData* td = static_cast<ThreadData*>(args);
    int cnt = 10;
    while(cnt)
    {
      cout<< td->namebuffer << "---cnt : " << cnt <<endl;
      cnt--;
      sleep(1);

      // exit(1); 不行的，终止进程的
      // pthread_exit(nullptr);  // 可以的
      // return nullptr;  // 可以的
    }

// 必须返回堆空间的地址
    ThreadReturn * tr = new ThreadReturn();
    tr->exit_code = 1;
    tr->exit_result = 106;

   return (void*)tr; // 右值

// 线程怎么没有退出信号呢？
// 退出信号就是线程的，
}

int main()
{
// 创建一批线程
  vector<ThreadData*> threads;
#define NUM 10
  for(int i = 0; i < NUM; i++)                                                // 循环创建是个线程
  {
    ThreadData* td = new ThreadData();                                         // 创建一个存放线程信息的对象
    td->number = i + 1;                                                        // 线程id
    snprintf(td->namebuffer, sizeof(td->namebuffer), "%s : %d", "thread", i+1); // 线程name
    pthread_create(&td->tid, nullptr, start_routine, td);                      // 创建线程，并且把线程的信息给执行的函数 td拷贝给线程
    threads.push_back(td);                                                     // 指针放到数组里面去
  } 

  for(auto& iter : threads)
  {

    ThreadReturn *ret = nullptr;
    pthread_join(iter->tid, (void**)&ret); // 默认线程成功，退出信号是进程关系的。

    // 返回值是 *
    // 需要操作 * 来接收数据
    // 所以这里需要& *的地址的
    // 这里需要操作 ret, ret是

    cout<< " join " << " : " << iter->namebuffer << " success number " << iter->number <<endl;
    cout<< "return result " << " exit_code : " << ret->exit_code << " exit_result : " << ret->exit_result <<endl; // 拿到返回的数据

    delete iter;  // 外面释放
  }

    while(true)
    {
        cout<< "main stream " <<endl;
        sleep(1);
    }
}
```



**线程取消**

**前提：线程已经跑起来才能取消的**

**线程被取消，返回值就是 -1**

```c++
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

void* start_routine(void* args)
{

    ThreadData* td = static_cast<ThreadData*>(args);
    int cnt = 10;
    while(cnt)
    {
      cout<< td->namebuffer << "---cnt : " << cnt <<endl;
      cnt--;
      sleep(1);

      // exit(1); 不行的，终止进程的
      // pthread_exit(nullptr);  // 可以的
      // return nullptr;  // 可以的
    }

// 必须返回堆空间的地址
    ThreadReturn * tr = new ThreadReturn();
    tr->exit_code = 1;
    tr->exit_result = 106;

   return (void*)100; // 右值

// 线程怎么没有退出信号呢？
// 退出信号就是线程的，
}

int main()
{
// 创建一批线程
  vector<ThreadData*> threads;
#define NUM 10
  for(int i = 0; i < NUM; i++)                                                // 循环创建是个线程
  {
    ThreadData* td = new ThreadData();                                         // 创建一个存放线程信息的对象
    td->number = i + 1;                                                        // 线程id
    snprintf(td->namebuffer, sizeof(td->namebuffer), "%s : %d", "thread", i+1); // 线程name
    pthread_create(&td->tid, nullptr, start_routine, td);                      // 创建线程，并且把线程的信息给执行的函数 td拷贝给线程
    threads.push_back(td);                                                     // 指针放到数组里面去
  } 

    sleep(3);
    for(size_t i = 0; i < threads.size() / 2; i++)
    {
      pthread_cancel(threads[i]->tid);
    }


  for(auto& iter : threads)
  {

    ThreadReturn *ret = nullptr;
    pthread_join(iter->tid, (void**)&ret); 

    // 返回值是 *
    // 需要操作 * 来接收数据
    // 所以这里需要& *的地址的
    // 这里需要操作 ret, ret是

    cout<< " join " << " : " << iter->namebuffer << " success number " << iter->number <<endl;
    cout<< "return result : " << (long long)ret <<endl;
    delete iter;  // 外面释放
  }



    while(true)
    {
        cout<< "main stream " <<endl;
        sleep(1);
    }
}
```



**任何语言在linux使用 线程，都必须使用Linux的线程库的。**

```c++
#include <iostream>
#include <unistd.h>
#include <thread>

void thread_run()
{
  while(true)
  {
    std::cout<< "我是新线程" <<std::endl;
    sleep(1);
  }
}

int main()
{
  std::thread t1(thread_run);

  while(true)
  {
    std::cout<< "我是主线程" << std::endl;
    sleep(1);
  }

  t1.join();
  return 0;
}

```



**线程分离**

**线程是可以等待的，等待的时候，join等待，阻塞式等待，如果我们不想等待呢？**

**pthread_self()**

```c++
#include <iostream>
#include <cstring>
#include <string>
#include <pthread.h>
#include <cstdio>
#include <unistd.h>
using namespace std;


string changeId(const pthread_t& thread_id)
{
  char tid[128];
  snprintf(tid, sizeof(tid), "0x%zx", thread_id);
  return tid;
}

void* start_routine(void* args)
{
  string threadname = static_cast<const char*>(args);
  pthread_detach(pthread_self()); // 设置自己为分离状态
  int cnt = 5;
  while(cnt)
  {
    char tid[28];
    snprintf(tid, sizeof(tid), "0x%zx", pthread_self());
    cout<< threadname << "running..." << changeId(pthread_self()) <<endl;
    cout<< cnt <<endl;
    sleep(1);
    cnt--;
  }
  return nullptr;
}

int main()
{

  pthread_t tid;
  pthread_create(&tid, nullptr, start_routine, (void*)"thread 1");
  string main_id = changeId(pthread_self());
  pthread_detach(tid);

  cout<< " main thread run ...." << "new thread id : "  << changeId(tid) << " main_id : " << main_id <<endl;

  while(true)
  {
    //todo main
  }

  // join和detach不能共存
  // int n = pthread_join(tid, nullptr);
  // cout<< "result " << n << " n " << strerror(n) <<endl;

  return 0;
}
```



**线程的独立栈**

**原生线程库，可能要存在多个线程--你用这些接口创建了线程，别人也可以同时在用的**

**原生线程库，要不要对线程管理呢？ 要**

**先描述：线程的属性比较少，**

**组织：**



**Linux方案：用户级别线程，用户关心的线程属性在库中，内核提供线程执行流的调度**

**Linux用户级线程：内核轻量级进程=1；1**



**用户级线程id究竟是什么？**

**就是库里面创建的结构体对象**

**id就是结构体对象的地址**



![image-20251203095315041](picture/image-20251203095315041.png)

**线程的栈，在线程库里面的栈。**

**共享区里面存放了，线程的地址。**

```c++
#include <iostream>
#include <cstring>
#include <string>
#include <pthread.h>
#include <cstdio>
#include <unistd.h>
using namespace std;

// 添加__thread,可以将一个内置类型设置为线程局部存储
// 每个线程都有一份资源的
__thread int g_val = 128;

// 共享资源
int shared = 44;
string changeId(const pthread_t& thread_id)
{
  char tid[128];
  snprintf(tid, sizeof(tid), "0x%zx", thread_id);
  return tid;
}

void* start_routine(void* args)
{
  string threadname = static_cast<const char*>(args);
  int cnt = 5;
  while(cnt)
  {
    char tid[28];
    snprintf(tid, sizeof(tid), "0x%zx", pthread_self());
    cout<< threadname << "running..." << changeId(pthread_self()) <<endl;
    cout<< cnt <<endl;
    cout<< "g_val: " << g_val <<endl; 
    g_val++;
    sleep(1);
    cnt--;
  }
  return nullptr;
}

int main()
{

  pthread_t tid;
  pthread_create(&tid, nullptr, start_routine, (void*)"thread 1");
  string main_id = changeId(pthread_self());
  pthread_detach(tid);

  cout<< " main thread run ...." << "new thread id : "  << changeId(tid) << " main_id : " << main_id <<endl;

  while(true)
  {
    //todo main
    cout<< "g_val: " << g_val <<endl; 
    sleep(1);
  }

  return 0;
}

```



## 3线程同步与互斥

```c++
#pragma once 
#include <string>
#include <iostream>
#include <pthread.h>
#include <functional>
#include <cassert>
#include <cstring>

class Thread;

class context
{
public:
  Thread* this_;
  void* args_;
public:
  context():this_(nullptr), args_(nullptr){}
  ~context(){}
};

class Thread
{
public:
  typedef std::function<void*(void*)> func_t;  // function pointer
  const int num = 1024;
public: 
  Thread(func_t func, void* args = nullptr, int number = 0):func_(func), args_(args)
  {
    char buffer[num];
    snprintf(buffer, sizeof(buffer), "thread-%d", number);         // thread-1,2,,n;
    name_ = buffer;                                                // thread name

    context* ctx = new context();     // store class address and agrs
    ctx->this_ = this;
    ctx->args_ = args_;

    int n = pthread_create(&tid_, nullptr, start_routine, ctx);     // TODO
    assert(n == 0);                                                 // assert 意料只中， if意料之外
    (void)n;
  }

  // 类内创建线程，执行对应的方法，方法static this pointer
  static void* start_routine(void* agrs)   // 缺省参数
  {
    context* ctx = static_cast<context*>(agrs);
    
    void* ret = ctx->this_->run(ctx->args_);
    delete ctx;
    return ret;
    // 静态不能调用成员方法，成员变量。
  }

  void join()
  {
    int n = pthread_join(tid_, nullptr);
    assert(n == 0);
    (void)n;
  }

  void* run(void* args)
  {
    return func_(args);
  }

  ~Thread()
  {
    // do nothing
  }

private:
  std::string name_;  // thread name
  func_t func_;       // thread function
  void* args_;        // thread args
  pthread_t tid_;     // thread tid
};

```



```c++
#include <iostream>
#include <memory>
#include "Thread.hpp"
#include <cstring>
#include <string>
#include <pthread.h>
#include <cstdio>
#include <unistd.h>
using namespace std;

// 间接猪跑
// pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; // 定义锁
// 需要多个线程交叉执行，
// 交叉执行的本质，调度器尽可能频繁发生线程调度与切换
// 线程切换：时间片到了，来了优先级跟高的线程。线程等待的时候。
// 线程是在什么时候检查上面的问题呢？内核态--》用户态。线程对调度状态进行检查，如果可以，就直接发生线程切换。

int tickets = 10000;
void* getTickets(void* args)
{
  std::string user_name = static_cast<const char*>(args);
  while(true)
  {
    if(tickets > 0)
    {
      // 进来睡觉了，前面的已经修改了ticket了
      usleep(1000); // 1=1000=1000 000
      std::cout<< user_name << "真正抢票 " << tickets <<std::endl;
      tickets--; // 修改数据
    }
    else 
    {
      break;
    }
  }
  return nullptr;
}

int main()
{
  std::unique_ptr<Thread> thread1(new Thread(getTickets, (void*)"hello lic1", 1));
  std::unique_ptr<Thread> thread2(new Thread(getTickets, (void*)"hello lic2", 2));
  std::unique_ptr<Thread> thread3(new Thread(getTickets, (void*)"hello lic3", 3));
  std::unique_ptr<Thread> thread4(new Thread(getTickets, (void*)"hello lic4", 4));

  thread1->join();
  thread2->join();
  thread3->join();
  thread4->join();
  return 0;
}

```

**为什么出现问题了**

**ticket只剩最后一张了**

**判断的本质：1读取内存的数据到CPU里面，2进行判断**

ticket
ticket--
1.
进程1进来了，睡觉了，上下文切换，ticket>0
进程2进来了，睡觉了，上下文切换，ticket>0
进程3进来了，睡觉了，上下文切换，ticket>0
进程4进来了，睡觉了，上下文切换，ticket>0
2.
进程1醒了，读数据ticket = 1
进程2醒了，读数据ticket = -1
进程3醒了，读数据ticket = -2



![image-20251203111212090](picture/image-20251203111212090.png)

**变量进行--(或者++)**

**1 A加载1000到CPU里面**

**2 1000--**

**3 999写到内存里面**



**不幸的是3就被切换走了**



**1 B加载1000到CPU里面**

**2 1000--  **

**3 999写到内存里面**

**然后A读写到了还是1000了**



**加锁**

**我们定义的全局变量，没有保护的时候，往往是不安全的，像上面多个线程在交替执行，造成的数据安全问题，数据不一致问题。
多个执行流进行，安全访问的共享资源--临界资源    （小部分的代码）
我们把多个执行流中，访问临界资源的代码，临界区  （小部分的代码） 往往是线程代码的很小一部分
想让多个线程，串行访问共享资源--互斥
对一个资源进行访问的时候，要么不做，要么做完---原子性，（解释不是原子性的场景）(刚才的123，三条汇编语句)--一个对资源进行操作，如果只用一条汇编就能完成，原子性操作。反之不是原子的。 
反之：不是原子的，当前理解，方便表述。**

**原子性：原子是不可以分的。就是要做就做完，不做就不做。**



```c++
#include <iostream>
#include <memory>
#include "Thread.hpp"
#include <cstring>
#include <string>
#include <pthread.h>
#include <cstdio>
#include <unistd.h>
using namespace std;



// 间接猪跑
// pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; // 定义锁
// 需要多个线程交叉执行，
// 交叉执行的本质，调度器尽可能频繁发生线程调度与切换
// 线程切换：时间片到了，来了优先级跟高的线程。线程等待的时候。
// 线程是在什么时候检查上面的问题呢？内核态--》用户态。线程对调度状态进行检查，如果可以，就直接发生线程切换。

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

int tickets = 10000;
void* getTickets(void* args)
{
  std::string user_name = static_cast<const char*>(args);
  while(true)
  {
    pthread_mutex_lock(&lock);
    if(tickets > 0)
    {
      // 进来睡觉了，前面的已经修改了ticket了
      usleep(1000); // 1=1000=1000 000
      std::cout<< user_name << "真正抢票 " << tickets <<std::endl;
      tickets--; // 修改数据
    pthread_mutex_unlock(&lock);
    }
    else 
    {
      pthread_mutex_unlock(&lock);
      break;
    }
  }
  return nullptr;
}

int main()
{
  std::unique_ptr<Thread> thread1(new Thread(getTickets, (void*)"hello lic1", 1));
  std::unique_ptr<Thread> thread2(new Thread(getTickets, (void*)"hello lic2", 2));
  std::unique_ptr<Thread> thread3(new Thread(getTickets, (void*)"hello lic3", 3));
  std::unique_ptr<Thread> thread4(new Thread(getTickets, (void*)"hello lic4", 4));

  thread1->join();
  thread2->join();
  thread3->join();
  thread4->join();
  return 0;
}

```





```c++
#pragma once 
#include <string>
#include <iostream>
#include <pthread.h>
#include <functional>
#include <cassert>
#include <cstring>

class Thread;

class context
{
public:
  Thread* this_;
  void* args_;
public:
  context():this_(nullptr), args_(nullptr){}
  ~context(){}
};

class Thread
{
public:
  typedef std::function<void*(void*)> func_t;  // function pointer
  const int num = 1024;
public: 
  Thread(func_t func, void* args = nullptr, int number = 0):func_(func), args_(args)
  {
    char buffer[num];
    snprintf(buffer, sizeof(buffer), "thread-%d", number);         // thread-1,2,,n;
    name_ = buffer;                                                // thread name

    context* ctx = new context();     // store class address and agrs
    ctx->this_ = this;
    ctx->args_ = args_;

    int n = pthread_create(&tid_, nullptr, start_routine, ctx);     // TODO
    assert(n == 0);                                                 // assert 意料只中， if意料之外
    (void)n;
  }

  // 类内创建线程，执行对应的方法，方法static this pointer
  static void* start_routine(void* agrs)   // 缺省参数
  {
    context* ctx = static_cast<context*>(agrs);
    
    void* ret = ctx->this_->run(ctx->args_);
    delete ctx;
    return ret;
    // 静态不能调用成员方法，成员变量。
  }

  void join()
  {
    int n = pthread_join(tid_, nullptr);
    assert(n == 0);
    (void)n;
  }

  void* run(void* args)
  {
    return func_(args);
  }

  ~Thread()
  {
    // do nothing
  }

private:
  std::string name_;  // thread name
  func_t func_;       // thread function
  void* args_;        // thread args
  pthread_t tid_;     // thread tid
};

```



**复习**

**原生线程库**

**线程id，线程库结构体对象的id**

**线程栈--库里面**

**共享资源**

**串行访问**

**原子性**



**1如何看待锁
  锁本身就是一个共享资源，全局变量要是被保护的，锁是用来保护全局资源，锁的安全谁来保护？
  pthread_mutex_lock，pthread_mutex_unlock;加锁的过程必须是安全的！,加锁过程其实是原子的！ 申请失败或者成功。
  如果申请成功，继续向后执行。如果申请暂时没有呢？执行流会怎么办？执行流会阻塞。trylock：非阻塞申请
  谁持有锁，谁进入临界区**



![image-20251203200653900](picture/image-20251203200653900.png)



![image-20251203200718379](picture/image-20251203200718379.png)



**线程1**
**线程2**
**线程3**
**如果线程1，申请锁成功，进入临界资源，正在访问临界资源，其它线程在做什么？？  阻塞等待**
**如果线程1，申请锁成功，进入临界资源，正在访问临界资源，我可不可以被切换呢？？绝对可以的！，其它线程不能进来**
  **当持有锁的线程被切走，是被抱着锁切走的，即便自己被切走了，其它线程依旧无法申请成功锁，也变变无法向后执行！**
  **直到我最终释放这个锁！**
**其它线程的角度，看待当前线程有锁的过程 ，就是原子的。**
  **对应其它线程而言，有意义的锁的状态，无非两种**

  **1申请锁前**
  **2释放锁后**

  **原子态，两态**
**未来，我们在使用锁的时候，一定要尽量保证临界区的粒度非常小！**
**加锁是程序员行为，必须做到要加都要加！**



**2如何理解加锁和解锁的本质:加锁的过程是原子的。**
  **swap或exchange指令,该指令的作用是把寄存器和内存单元的数据相交换 ,由于只有一条指令.**

**mutex_t 锁变量**
**lock:**
  **movb $0, %a1  //a1是寄存器  0--》a1(0放到寄存器)（0放到上下文）。**
  **xchgb %a1, mutex // 交换的本质：共享的数据，交换到我的上下文。**
  **if(al寄存器的内容 > 0)**
  **{**
    **return 0**
  **}**
  **else**
  **{**
    **挂起等待**
    **goto lock;**
  **}**
**unlock**
  **movb $1, mutex**
  **唤醒等待Mutex的线程**
  **return 0;**
**1cpu内寄存器只有一套被所有执行流共享**
**2cpu内寄存器的内容，是每个执行流私有的，运行时的上下文。**



![image-20251203202118291](picture/image-20251203202118291.png)



**死锁的四个必要条件！**
  **互斥**
  **请求与保持**
  **不剥夺**
  **环路等待条件**
**互斥条件：一个资源每次只能被一个执行流使用**
**请求与保持条件：一个执行流因请求资源而阻塞时，对已获得的资源保持不放**
**不剥夺条件:一个执行流已获得的资源，在末使用完之前，不能强行剥夺**
**循环等待条件:若干执行流之间形成一种头尾相接的循环等待资源的关系**

**能不用锁，就不用锁。除非不得已啊。**

**线程同步问题**

**同步：在保证数据安全的前提下，让线程能够按照某种特定的顺序访问临界资源，从而有效避免饥饿问**
**题，叫做同步**



**同步**

**同步：在保证数据安全的前提下，让线程能够按照某种特定的顺序访问临界资源，从而有效避免饥饿问**



****















