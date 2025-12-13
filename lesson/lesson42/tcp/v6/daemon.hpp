#pragma once
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void daemonSelf(const char* currPath = nullptr)
{
    // 1.屏蔽掉异常的信号，进程忽略掉异常的信号
    signal(SIGPIPE, SIG_IGN);

    // 2.如何让自己不是组长，setid()
    if (fork() > 0)
        exit(0); // 子进程--守护进程，精灵进程，本质就是孤儿进程

    // 子进程
    pid_t n = setsid(); // 设置SID
    assert(n != -1);

    // 3.守护进程是脱离终端的,关闭或者重定向以前进程默认打开的文件
    // /dev/null linux的垃圾桶
    int fd = open("/dev/null", O_RDWR);
    if(fd >= 0)
    {
        dup2(fd, 0);
        dup2(fd, 1);
        dup2(fd, 2);

        close(fd);
    }
    else
    {
        close(0);
        close(1);
        close(2);
    }

    // 4.进程的执行路径是否发生改变的(进程属性cwd)
    if(currPath) chdir(currPath);
}