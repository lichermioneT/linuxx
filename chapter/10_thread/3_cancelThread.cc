#include <iostream>    
#include <vector>    
#include <pthread.h>    
#include <unistd.h>    
#include <cstdio>    
#include <cstring>    
    
using namespace std;    
    
class ThreadData    
{    
public:    
    pthread_t tid;    
    int id;    
    char buffer[64];    
};    
    
// 线程执行函数    
void* start_routine(void* arg)    
{    
    ThreadData* td = static_cast<ThreadData*>(arg);    
    
    cout << "线程开始执行: " << td->buffer << endl;    
    
    // sleep 是取消点    
    sleep(10);    
    
    cout << "线程正常结束: " << td->buffer << endl;    
    
    // 把自己的参数地址返回给主线程    
    return arg;
} 

// 1.线程创建返回的是erron
// 2.线程取消返回的是erron
// 3.线程等待返回的是erron

int main()    
{    
    vector<ThreadData*> v;    
    
    // 1. 创建 10 个线程    
    for(int i = 0; i < 10; ++i)    
    {    
        ThreadData* td = new ThreadData();    
       td->id = i + 1;                                                                                                                                                                         
        snprintf(td->buffer, sizeof(td->buffer), "thread-%d", i + 1);    
    
        int n = pthread_create(&td->tid, nullptr, start_routine, td);    
        if(n != 0)    
        {    
            cerr << "pthread_create error: " << strerror(n) << endl;
            delete td;
            return 1;
        }

        v.push_back(td);
    }

    // 主线程先等 3 秒
    sleep(3);

    // 2. 取消前 5 个线程
    for(int i = 0; i < 5; ++i)
    {
        int n = pthread_cancel(v[i]->tid);
        if(n != 0)
        {
            cerr << "pthread_cancel error: " << strerror(n) << endl;
        }
        else
        {
            cout << "已发送取消请求: " << v[i]->buffer << endl;
        }
    }
   // 3. 等待所有线程，并拿到返回值
    for(int i = 0; i < 10; ++i)
    {
        void* ret = nullptr;
        int n = pthread_join(v[i]->tid, &ret);
        if(n != 0)
        {
            cerr << "pthread_join error: " << strerror(n) << endl;
            continue;
        }

        if(ret == PTHREAD_CANCELED)
        {
            cout << v[i]->buffer << " 被取消退出" << endl;
        }
        else
        {
            ThreadData* td = static_cast<ThreadData*>(ret);
            cout << td->buffer << " 正常返回, id = " << td->id << endl;
        }
    }

    // 4. 释放资源
    for(auto e : v)
    {
        delete e;
    }
    v.clear();

    return 0;
}


