#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <string>
#include <cstring>

struct ThreadData
{
    int id;
    std::string name;
};

// 共享资源：总票数
int tickets = 20;

// 互斥锁：保护 tickets
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* start_routine(void* arg)
{
    ThreadData* td = static_cast<ThreadData*>(arg);

    while (true)
    {
// 1. 进入临界区前先加锁
        pthread_mutex_lock(&mutex);

// 2. 检查共享资源
        if (tickets <= 0)
        {
            pthread_mutex_unlock(&mutex);
            break;
        }

// 3. 对共享资源进行访问和修改
        std::cout << td->name << " 正在抢第 " << tickets << " 张票" << std::endl;
        --tickets;

// 4. 临界区结束，立刻解锁
        pthread_mutex_unlock(&mutex);

        // 模拟线程处理其它事情
        usleep(1000 * 100); // 100ms
    }

    std::cout << td->name << " 退出抢票" << std::endl;
    return nullptr;
}

int main()
{
    const int NUM = 4;
    pthread_t tids[NUM];
    ThreadData tds[NUM];

    for (int i = 0; i < NUM; ++i)
    {
        tds[i].id = i + 1;
        tds[i].name = "thread-" + std::to_string(i + 1);

        int n = pthread_create(&tids[i], nullptr, start_routine, &tds[i]);
        if (n != 0)
        {
            std::cerr << "pthread_create error: " << strerror(n) << std::endl;
            return 1;
        }
    }

    for (int i = 0; i < NUM; ++i)
    {
        int n = pthread_join(tids[i], nullptr);
        if (n != 0)
        {
            std::cerr << "pthread_join error: " << strerror(n) << std::endl;
            return 1;
        }
    }

    pthread_mutex_destroy(&mutex);
    std::cout << "所有票已经抢完，剩余票数: " << tickets << std::endl;

    return 0;
}
