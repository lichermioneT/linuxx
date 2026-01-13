#include "util.hpp"
#include <cstdio>
#include <vector>
#include <functional>
#include <sys/select.h>

using func_t = std::function<void()>;

#define INIT(v)                \
    do                         \
    {                          \
        v.push_back(printLog); \
    } while (0)

#define EXEC_OTHER(cbs)            \
    do                             \
    {                              \
        for (auto const &cb : cbs) \
            cb();                  \
    } while (0)

int main()
{
    fd_set;
    std::vector<func_t> cbs;
    INIT(cbs);

    setNonBlock(0);
    char buffer[1024];
    while (true)
    {
        // printf(">>> ");
        // fflush(stdout);
        ssize_t s = read(0, buffer, sizeof(buffer) - 1);
        if (s > 0)
        {
            buffer[s - 1] = 0;
            std::cout << "echo# " << buffer << std::endl;
        }
        else if (s == 0)
        {
            std::cout << "read end" << std::endl;
            break;
        }
        else
        {
            // 1. 当我不输入的时候，底层没有数据，算错误吗？不算错误，只不过以错误的形式返回了
            // 2. 我又如何区分，真的错了，还是底层没有数据？单纯返回值，无法区分！
            // std::cout << "EAGAIN: " << EAGAIN << " EWOULDBLOCK: " << EWOULDBLOCK << std::endl;

            if (errno == EAGAIN)
            {
                std::cout << "我没错, 只是没有数据" << std::endl;
                EXEC_OTHER(cbs);
            }
            else if (errno == EINTR)
            {
                continue;
            }
            else
            {
                std::cout << "s : " << s << " errno: " << strerror(errno) << std::endl;
                break;
            }
        }

        sleep(1);
    }
}