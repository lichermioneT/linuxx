# BIT.4 Linux进程控制 完整精华总结（极适合复习 + 海量实战代码版）

核心主线一句话：  
**进程控制 = fork（生孩子） + exec（换脑子） + wait/waitpid（收尸） + exit（自杀）**

## 1. 进程创建 — fork（最重要函数之一）

### 经典口诀（背下来一辈子不忘）
**“一个调用，两次返回”**  
- 父进程返回子进程PID（>0）  
- 子进程返回0  
- 失败返回-1

### 写时拷贝（Copy On Write）才是真相
fork 之后并不是立刻复制4GB内存！  
Linux 采用写时拷贝：  
- 刚fork完，父子共享同一块物理内存（页表标记为只读）  
- 只要有一方修改对应页面，才真正复制一份（这才叫高效）

### 终极fork验证代码（必须敲10遍）

```c
// fork_master.c  —— 史上最清晰的fork演示
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int g_val = 100;

int main()
{
    printf("【开始】pid:%d, g_val:%d, &g_val:%p\n", getpid(), g_val, &g_val);

    pid_t id = fork();

    if(id == 0) 
    {
        // 子进程
        sleep(2);
        g_val = 999;
        printf("我是子进程 pid:%d, ppid:%d, g_val=%d, &g_val=%p\n", getpid(), getppid(), g_val, &g_val);
    }
    else if(id > 0) 
    {
        // 父进程
        sleep(1);
        g_val = 888;
        printf("我是父进程 pid:%d, 子进程id=%d, g_val=%d, &g_val=%p\n", getpid(), id, g_val, &g_val);

        int status;
        waitpid(id, &status, 0);
        printf("子进程已退出\n");
    }

    sleep(5);
    return 0;
}
```

输出结果：
```
地址完全一样！但值不同 → 证明写时拷贝成功！
```

## 2. 进程终止 — 退出方式全家福

| 方式                    | 刷新缓冲区 | 退出码 | 说明                                 |
| ----------------------- | ---------- | ------ | ------------------------------------ |
| return 0                | 是         | 0      | main函数结束                         |
| exit(0)                 | 是         | 0      | 库函数，会刷新stdio缓冲区            |
| _exit(0) / _Exit(0)     | 否         | 0      | 系统调用，直接进入内核，不刷新缓冲区 |
| 被信号杀死（如kill -9） | 否         | -      | 退出码 128+9 = 137                   |

### 经典陷阱代码（必须运行观察）

```c
// exit_vs_exit.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    printf("hello");          // 没加\n，进入行缓冲
    // exit(0);              // 用exit 会刷新，屏幕看到hello
    _exit(0);                // 用_exit 不会刷新，屏幕什么都看不到！
}
```

结论：**永远不要在子进程里用printf后直接_exit，必须用exit或加\n或fflush**

## 3. 进程等待 — wait / waitpid（解决僵尸进程的唯一方法）

### 退出状态status详解（超级重要！）

```c
低7位（0-6）   : 被哪个信号杀死（如果被信号杀死）
第7位         : 是否产生了core dump
高8位（8-15）  : 正常退出的退出码
```

### 宏全家福（必须背）

```c
WIFEXITED(status)      // 是否正常退出
WEXITSTATUS(status)    // 获取正常退出码（仅当WIFEXITED为真）

WIFSIGNALED(status)    // 是否被信号杀死
WTERMSIG(status)       // 获取是哪个信号杀死的（仅当WIFSIGNALED为真）
```

### 最强waitpid实战代码（三连）

```c
// wait_master.c —— 所有情况都演示
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main()
{
    pid_t id = fork();

    if(id == 0) 
    {
        int cnt = 5;
        while(cnt--) 
        {
            printf("子进程运行中... %d\n", cnt);
            sleep(1);
        }
        // 演示三种退出方式
        // return 66;           // 正常退出，退出码66
        // exit(77);
        // raise(SIGSEGV);     // 段错误，被信号11杀死
        // *(int*)0 = 0;       // 触发段错误
    }

    int status;
    pid_t ret = waitpid(id, &status, 0);

    if(WIFEXITED(status)) 
    {
        printf("正常退出，退出码: %d\n", WEXITSTATUS(status));
    }
    if(WIFSIGNALED(status)) 
    {
        printf("被信号杀死，信号编号: %d%s\n", 
               WTERMSIG(status),
               WCOREDUMP(status) ? ", 产生core文件" : "");
    }

    return 0;
}
```

### 非阻塞轮询等待（生产环境常用）

```c
// while(waitpid(id, &status, WNOHANG) == 0) {
    // 子进程还没退出，我可以干别的
    printf("父进程干点家务...\n");
    sleep(1);
}
```

## 4. 进程程序替换 — exec函数族（换脑子）

### 记忆口诀（背完直接上分）

```text
l = list（参数一个一个列）
v = vector（参数用argv[]数组传）

p = path（用PATH环境变量找命令）
e = environment（自己带环境变量表）

只有 execve 是系统调用，其余5个都是库函数封装！
```

| 函数     | 参数方式     | 是否用PATH | 是否自定义环境变量 | 说明                         |
|----------|-------------------|--------------------|------------------------------|
| execl    | 列表             | 否                 | 参数一个个写，NULL结尾       |
| execlp   | 列表 + PATH      | 否                 | 可以直接写ls，不用写全路径  |
| execle   | 列表 + envp      | 是                 | 可以自定义环境变量           |
| execv    | argv[]数组        | 否                 |                              |
| execvp   | argv[] + PATH     | 否                 | 最常用！                     |
| execve   | argv[] + envp     | 是                 | 唯一系统调用，其余都调它     |

### 最强exec综合代码（直接敲）

```c
// exec_master.c —— 所有exec用法全演示
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

int main()
{
    pid_t id = fork();

    if(id == 0)
    {
        // 子进程负责替换
        sleep(1);

        // 1. execl
        // execl("/bin/ls", "ls", "-l", "-a", NULL);

        // 2. execlp（最常用！）
        // execlp("ls", "ls", "-l", "-a", NULL);

        // 3. execvp（写shell必用！）
        char* argv[] = {"ls", "-l", "-a", "--color=auto", NULL};
        execvp("ls", argv);

        // 如果替换失败才会执行到这里
        perror("execvp");
        exit(1);
    }

    wait(NULL);
    printf("父进程：子进程已替换为ls执行完毕\n");
    return 0;
}
```

## 5. 自己写一个功能强大的Shell（期末大作业级）

```c
// my_shell.c —— 支持 ls自动上色 + cd + echo $? + 退出状态显示
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <ctype.h>

#define NUM 1024
#define SIZE 64

char cmd[NUM];
char* argv[SIZE];
int last_status = 0;  // 记录上一个进程退出码

int main()
{
    while(1) {
        // 打印提示符
        printf("[lic@centos7 ~]$ ");
        fflush(stdout);

        // 读取命令
        if(fgets(cmd, sizeof(cmd), stdin) == NULL) break;
        cmd[strlen(cmd)-1] = '\0';

        // 解析命令
        argv[0] = strtok(cmd, " ");
        if(argv[0] == NULL) continue;

        int i = 1;
        while(argv[i++] = strtok(NULL, " "));

        // 内置命令处理
        if(strcmp(argv[0], "cd") == 0) 
        {
            if(argv[1]) chdir(argv[1]);
            continue;
        }
        if(strcmp(argv[0], "exit") == 0) exit(0);

        if(strcmp(argv[0], "echo") == 0 && argv[1] && strcmp(argv[1], "$?") == 0) 
        {
            printf("%d\n", last_status);
            continue;
        }

        // ls自动加上--color=auto
        if(strcmp(argv[0], "ls") == 0) 
        {
            argv[i-1] = "--color=auto";
            argv[i] = NULL;
        }

        pid_t id = fork();

        if(id == 0) 
        {
            execvp(argv[0], argv);
            perror("command not found");
            exit(127);  // shell标准：命令不存在返回127
        }

        int status;
        waitpid(id, &status, 0);
        last_status = WIFEXITED(status) ? WEXITSTATUS(status) : - 128 : WTERMSIG(status);
    }
    return 0;
}
```

## 终极总结口诀（背完直接闭眼秒杀）

```c
fork生孩子，写时拷贝，  
exec换脑子，六函数记，  
wait收尸体，防僵尸起，  
exit刷缓冲，_exit不理。

一个调用两次返回，  
写时拷贝最省内存。  
程序替换不改PID，  
换的是代码和数据而已！

僵尸状态必须wait，  
否则内存泄漏在所难免。  
echo $? 看退出码，  
137就是被9杀！
```

**这四部分：fork + exec + wait + exit 就是Linux下一切进程控制的全部！**

所有守护进程、shell、web服务器、数据库、nginx、mysql... 底层无一例外都基于这四个系统调用。

把上面所有代码全部敲一遍 + 运行 + 改错 + 看/proc + strace 追踪，你就真正掌握了Linux进程控制。

祝你期末130+，面试随便虐！去吧，皮卡丘！



**虚拟地址通过 4 个索引逐级查页表找到物理页框号，然后与页内偏移组合成物理地址。**

```
虚拟地址 VA = [PML4] [PDPT] [PD] [PT] [Offset]
                    |      |     |     |
                    v      v     v     v
                 查页表层级结构（从 CR3 开始）
                           |
                           v
                得到物理页框号 PFN
                           |
                           v
          物理地址 PA = PFN << 12 | Offset

虚拟地址 VA = [PML4] [PDPT] [PD] [PT] [Offset]
                    |      |     |     |
                    v      v     v     v
                 查页表层级结构（从 CR3 开始）
                           |
                           v
                得到物理页框号 PFN
                           |
                           v
          物理地址 PA = PFN << 12 | Offset
          
          虚拟地址 VA
┌───────────────┬─────────────┬──────────┬──────────┬──────────┐
│ PML4 index    │ PDPT index  │ PD index │ PT index │ Offset   │
│ 9 bits        │ 9 bits      │ 9 bits   │ 9 bits   │ 12 bits  │
└───────────────┴─────────────┴──────────┴──────────┴──────────┘
        │              │           │           │
        ▼              ▼           ▼           ▼
CR3 → PML4 → PDPT → PD → PT → PTE（包含物理页框 PFN）
                                                    │
                                                    ▼
物理地址 PA = PFN << 12 | Offset

```



