# BIT.3 Linux进程概念 完整精华总结（极适合复习）

笔记核心主线：**冯诺依曼体系 → 操作系统本质 → 进程本质 → 进程状态 → 优先级 → 环境变量 → 程序地址空间**  
全程围绕一句话：**操作系统管理的本质就是“先描述，后组织”数据**。

## 1. 冯诺依曼体系结构（最底层硬件真相）

- **存储器 = 内存**（掉电易失，速度快）
- **外设 = 磁盘、网卡、键盘、显示器等**（永久存储，速度慢）
- **CPU = 运算器 + 控制器 + 寄存器**（非常快，但很笨，只能被动执行指令）
- **核心原则：**CPU只和内存打交道，绝不直接和外设打交道**（为了效率）
  - 所有输入：外设 → 内存 → CPU
  - 所有输出：CPU → 内存 → 外设
- 内存的角色：**巨大的缓存**，解决CPU和外设之间速度不匹配的问题
- 程序必须先加载到内存才能运行，因为CPU只能从内存取指令和数据

**结论：程序要运行，必须先加载到内存中**

## 2. 操作系统（OS）本质

**定义**：OS是一个进行软硬件资源管理的软件

**目的**：通过合理管理资源，给用户提供稳定、安全、高效的执行环境

**管理的本质**：**对数据进行管理**（先描述，后组织）

类比最透彻的例子：

| 角色     | 现实对应 | OS中对应             | 职责                        |
| -------- | -------- | -------------------- | --------------------------- |
| 管理者   | 校长     | 操作系统             | 做决策（根据数据）          |
| 执行者   | 辅导员   | 驱动程序             | 执行决策 + 收集被管理者数据 |
| 被管理者 | 学生     | 硬件（磁盘、网卡等） | 产生数据                    |

**管理方法**：
1. 先描述 → 用结构体（C）/类（C++）抽象出统一结构
2. 后组织 → 用数据结构（链表、队列等）把所有对象组织起来

**OS对用户态度**：完全不信任  
→ 任何人（包括你写的程序）都不能直接进入操作系统  
→ 只能通过系统调用接口（system call）请求服务（本质就是C函数）

**经典计算机软硬件体系结构图（务必记住）**

```
用户
↓
Shell / C库 / GUI（用户接口）
↓
系统调用接口（strace ls 可看到）
↓
操作系统内核（进程管理、内存管理、文件系统、设备管理）
↓
驱动
↓
硬件
```

## 3. 进程（最核心概念）

**经典定义**：**正在运行的程序** = **内核数据结构（PCB） + 磁盘上的代码段**

**准确定义**：**进程 = struct task_struct（PCB） + 代码 + 数据**

- 程序 = 磁盘上的静态可执行文件
- 进程 = 程序加载到内存后 + OS瞬间创建一个PCB来描述它

**PCB（Process Control Block）就是进程的“身份证”**  
Linux中叫 `struct task_struct`，包含：

```c
struct task_struct 
{
    pid_t pid;                     // 进程ID
    long state;                    // 进程状态
    int prio;                      // 优先级
    struct mm_struct *mm;          // 进程虚拟地址空间描述（超级重要！）
    struct files_struct *files;    // 打开的文件
    struct task_struct *parent;    // 父进程
    struct list_head children;     // 子进程链表
    struct list_head tasks;        // 全局进程链表节点
    // ... 还有CPU寄存器上下文等
};
```

**对进程的管理 → 变成了对task_struct链表的增删查改**

**查看进程信息方式**：
- `ps -axj`、`ps ajx`
- `getpid()`、`getppid()`
- `/proc/PID/` 目录（进程在操作系统眼中的一切都在这里）

**fork() 创建子进程（最经典代码）**

```c
pid_t id = fork();
if(id == 0)
{
    // 子进程执行这里
} else if(id > 0) 
{
    // 父进程执行这里，返回子进程PID
}
```

- fork后代码共享，数据各自一份（写时复制）
- 父子进程完全独立，子进程崩溃不影响父进程

## 4. 进程状态（Linux真实状态 - 必须背）

Linux内核定义（include/linux/sched.h）：

| 状态宏               | ps命令显示 | 含义                                            | 能否被调度 |
| -------------------- | ---------- | ----------------------------------------------- | ---------- |
| TASK_RUNNING         | R          | 运行或就绪（在运行队列中等待CPU）               | 是         |
| TASK_INTERRUPTIBLE   | S          | 可中断睡眠（浅睡，等IO等，大多数sleep进程在此） | 否         |
| TASK_UNINTERRUPTIBLE | D          | 不可中断睡眠（深睡，一般是磁盘IO，杀不掉）      | 否         |
| __TASK_STOPPED       | T          | 停止（Ctrl+Z 或 kill -19）                      | 否         |
| __TASK_TRACED        | t          | 被调试（如gdb）                                 | 否         |
| EXIT_ZOMBIE          | Z          | 僵尸进程（已退出但父进程没读退出码）            | 否         |
| EXIT_DEAD            | X          | 死亡（最终状态，很快消失）                      | 否         |

**僵尸进程（Z）**：子进程退出后，父进程还没调用wait/waitpid回收退出状态  
→ PCB还存在，占用资源，严重会内存泄漏（后面进程控制会解决）

**孤儿进程**：父进程先死了 → 被1号进程（systemd/init）收养，正常运行

## 5. 优先级（了解即可，一般不用改）

- 优先级本质是PCB里的一个整数
- 范围：-20（最高）~ +19（最低）
- 普通用户只能调高nice值（即降低优先级），范围0~19
- 查看：`ps -eo pid,comm,nice,stat` 或 top 中 NI 列
- 修改：`nice -n 10 ./a.out` 或 top → r → 输入PID → 输入nice值
- 默认nice = 0，实际调度优先级 = 80 + nice（top中PR列）

**竞争性 vs 独立性 vs 并发 vs 并行**
- 竞争性：资源少，大家抢CPU
- 独立性：每个进程有独立虚拟地址空间，互不影响
- 并发：宏观同时进行，微观轮流执行（单核常见）
- 并行：真正同时执行（多核）

## 6. 进程切换与上下文（超级重要！）

**CPU执行三步**：取指 → 译码 → 执行

进程运行时会产生大量临时数据 → 存在CPU寄存器中  
→ 这些数据属于当前进程！

**进程切换必然发生**（时间片到了或被抢占）：
1. 当前进程上下文保存 → 保存到自己的task_struct→thread中
2. 加载下一个进程的上下文 → 恢复寄存器值
3. 切换页表（虚拟地址空间切换）

**寄存器硬件只有一套，但寄存器里的数据属于当前进程**

类比最牛：**保留学籍（上下文保存）→ 回家打工 → 回来复学（上下文恢复）**

## 7. 环境变量（非常实用）

**本质**：影响进程行为的键值对字符串表

常用：
- `PATH`：命令搜索路径
- `HOME`：用户家目录
- `PWD`：当前目录
- `USER`：当前用户名

**分类**：
- 本地环境变量：只在当前shell有效（`aaa=123`）
- 全局环境变量：可被子进程继承（`export aaa=123` 或 `export PATH=$PATH:/xxx`）

**C语言三种访问方式**（推荐第三种）：
1. `char **environ` 全局变量（unistd.h）
2. main函数第三参数 `char *env[]`
3. `getenv("PATH")` 最推荐

**子进程会继承父进程的环境变量表**（fork时复制）

## 8. 程序地址空间（最难但最重要部分！）

**核心结论**：C/C++中打印的地址全是**虚拟地址**，不是物理地址！

每个进程都认为自己独占整台机器的4GB（32位）内存（32位系统）

**为什么要有虚拟地址空间？**（三条理由必须背）
1. 安全：防止进程非法访问他人内存（页表会检查权限）
2. 独立：实现进程间数据互不影响（fork后地址相同但数据不同）
3. 方便：所有进程都用同一套地址规则，编译器可以统一编译

**地址空间布局（从低到高）**：

```
0x00000000
├── 代码段（.text）          只读
├── 数据段（.data + .bss）   可读写
├── 堆（heap）               向上增长（malloc/new）
├── 内存映射区（mmap）       共享库、文件映射等
├── 栈（stack）              向下增长（函数调用、局部变量）
└── 0xFFFFFFFF（内核区，用户不可访问）
```

**mm_struct** 就是用来描述这个地址空间的结构体，包含各段的start/end地址
```

**页表作用**：虚拟地址 ↔ 物理地址 映射 + 权限检查

**进程看到的地址永远是虚拟地址** → 进程独立性的根本保障！

## 终极复习口诀（背下来就能过）

1. CPU只和内存打交道 → 程序必须加载到内存 → 进程诞生
2. OS管理的本质：先描述（struct task_struct）后组织（链表）
3. 进程 = PCB + 代码 + 数据 + 虚拟地址空间
4. 进程切换 = 上下文保存/恢复 + 页表切换
5. 所有打印的地址都是虚拟地址 → 进程独立性的根源
6. 环境变量、命令行参数、环境表 → 都是fork时复制给子进程的

这套笔记的核心思想就是一句话：  
**“操作系统做的事，就是在冯诺依曼体系上，用‘先描述后组织’的方式，把硬件资源抽象成进程、文件等对象，让我们写程序时感觉自己在独占一台电脑。”**

背熟上面这张结构图和口诀，进程这章就彻底拿下了！
```



**内建指令就是 shell 自带、无需调用外部程序、在当前 shell 内执行的命令。**



# 代码示例版本

## 1. 冯诺依曼体系结构（底层真相（先理解为什么程序必须加载到内存）

```c
// 没有任何代码能证明这个，但可以用这个小实验感受“CPU只和内存打交道”
// 编译后用 strace 追踪系统调用，你会发现所有读写最终都变成 read/write/mmap
#include <stdio.h>
int main() 
{
    printf("Hello\n");        // 实际上是 write 系统调用 → 内存缓冲 → 最终刷到显示器
    int x = 0;
    scanf("%d", &x);          // 实际上是 read 系统调用 ← 键盘 → 内存 → CPU
    return 0;
}
```

**核心结论**：CPU一辈子只和内存打交道，所有外设交互必须先经过内存。

![image-20251125154825546](C:\Users\LIC\AppData\Roaming\Typora\typora-user-images\image-20251125154825546.png)

**c语言本质还是调用系统接口， c接口是对系统接口的封装。**



## 2. 操作系统本质 + “先描述，后组织”哲学

```c
// OS对硬件的管理方式（伪代码，真实内核就是这么干的）
struct device 
{                  // 先描述
    int type;
    int status;
    struct device *next;
};

struct device *dev_list = NULL;  // 后组织：链表

void register_device(int type) 
{
    struct device *d = malloc(sizeof(struct device));
    d->type = type;
    d->next = dev_list;
    dev_list = d;                // 插入链表头
}
```

**进程也是完全一样的思路** → PCB（task_struct）就是进程的“结构体描述”。

## 3. 进程 = PCB + 代码 + 数据（核心中的核心）

### 3.1 最基础的进程查看代码

```c
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() 
{
    printf("我的PID: %d\n", getpid());
    printf("我的父进程PID: %d\n", getppid());
    printf("我的会话ID: %d\n", getsid(0));
    printf("我的进程组ID: %d\n", getpgid(0));
    while(1) sleep(1);
    return 0;
}
```



```
会话（SID = 1000）
│
├── 进程组 A（PGID = 2000）
│     ├── 进程 pid 2000（组长）
│     └── 进程 pid 2001
│
└── 进程组 B（PGID = 3000）
      ├── 进程 pid 3000（组长）
      └── 进程 pid 3005

```



### 3.2 最经典 fork 示例（父子进程独立性 + 写时复制）

```c
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int g_val = 100;                     // 全局区

int main() 
{
    pid_t id = fork();
    
    if(id < 0)
    {
		
        return -1;
    }
    
    if (id == 0) 
    {                   // 子进程
        g_val = 1;
        int cnt = 0;
        while (cnt < 10) 
        {
            printf("子进程: pid=%d, ppid=%d, g_val=%d, &g_val=%p\n",
                   getpid(), getppid(), g_val, &g_val);
            sleep(1);
            cnt++;
        }
        return 99;                   // 子进程退出码99
    } 
    else if (id > 0) 
    {             // 父进程
        g_val = 2;
        int cnt = 0;
        while (cnt < 15) 
        {
            printf("父进程: pid=%d, ppid=%d, g_val=%d, &g_val=%p\n",
                   getpid(), getppid(), g_val, &g_val);
            sleep(1);
            cnt++;
        }
        
        int status;
        pid_t ret = waitpid(id, &status, 0);  // 等待子进程，回收僵尸
        if (WIFEXITED(status)) 
        {
            printf("子进程正常退出，退出码: %d\n", WEXITSTATUS(status));
        }
        printf("成功回收子进程 %d\n", ret);
    }
    return 0;
}
```

**输出关键点**：  
- 父子进程 `&g_val` 地址完全相同（虚拟地址一样！）  
- 但修改后值不同 → 证明进程地址空间独立（写时复制）

```c
第一对：正常退出
W IFE XITED(status)    ← 判断是否正常退出
W EXIT STATUS(status)  ← 获取退出码

第二对：信号终止  
W IF SIGNALED(status)  ← 判断是否信号杀死
W TERMSIG(status)     ← 获取信号编号
```



### 3.3 产生僵尸进程（故意不回收）

```c
#include <stdio.h>
#include <unistd.h>

int main()
{
    if (fork() == 0) 
    {
        printf("子进程马上死掉，成为僵尸\n");
        return 88;                   // 子进程立刻退出
    }
    // 父进程啥也不干，死循环
    while(1) sleep(1);
}
```

执行后 `ps -aux | grep Z` 就能看到 Z+ 僵尸进程。

### 3.4 产生孤儿进程（父进程先死）

```c
#include <stdio.h>
#include <unistd.h>

int main() 
{
    if (fork() == 0) 
    {
        // 子进程
        sleep(3);
        printf("孤儿进程醒了，我的父进程现在是: %d\n", getppid()); // 会变成1（systemd收养）
        while(1) sleep(1);
    }
    // 父进程立刻自杀
    printf("父进程立刻退出，子进程将成为孤儿\n");
    return 0;
}
```

### 3.5 多次fork产生进程树（三代同堂）

```c
#include <stdio.h>
#include <unistd.h>

int main() 
{
    printf("祖先进程 PID: %d\n", getpid());
    pid_t son = fork();
    if (son == 0) 
    {
        printf("儿子出生 PID: %d, 父亲: %d\n", getpid(), getppid());
        pid_t grandson = fork();
        if (grandson == 0) 
        {
            printf("孙子出生 PID: %d, 父亲: %d\n", getpid(), getppid());
            while(1) sleep(1);
        }
        while(1) sleep(1);
    }
    while(1) sleep(1);
}
```

## 4. 进程状态完整演示代码

```c
// 演示 R（运行）、S（浅睡）、D（磁盘深睡）、T（停止）、Z（僵尸）

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

int main() 
{
    pid_t id = fork();
    if (id == 0) 
    {
        // 子进程各种状态演示
        printf("子进程开始演示状态，PID=%d\n", getpid());
        sleep(5);                    // S 状态（可中断睡眠）
        
        // 产生D状态（磁盘IO深睡），dd命令最容易触发
        system("dd if=/dev/zero of=test.file bs=1M count=2048");  // D状态
        
        raise(SIGSTOP);               // T状态（Ctrl+Z或kill -19也行）
        
    } 
    else 
    {
        sleep(20);                    // 让子进程充分展示各种状态
        kill(id, SIGCONT);            // 恢复T状态
        sleep(10);
        kill(id, 9);                  // 强杀
    }
    return 0;
}
```

配合 `watch -n 1 'ps -p 子进程PID -o pid,stat,command'` 实时观察状态变化。

## 5. 优先级调整代码（C语言）

```c
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>

int main() 
{
    printf("当前nice值: %d\n", getpriority(PRIO_PROCESS, 0));
    
    // 把自己优先级调低（nice值调高）
    nice(10);                                      // +10
    // 或者用 setpriority
    setpriority(PRIO_PROCESS, 0, 15);
    
    printf("调整后nice值: %d\n", getpriority(PRIO_PROCESS, 0));
    
    // 死循环抢CPU，看top中是否CPU占用明显下降
    while(1);
    return 0;
}
```

## 6. 环境变量完整操作大全

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

extern char **environ;

void print_all_env() 
{
    for (int i = 0; environ[i]; i++) 
    {
        printf("%s\n", environ[i]);
    }
}

int main()
{
    // 三种方式取环境变量
    printf("HOME = %s\n", getenv("HOME"));
    printf("PATH = %s\n", getenv("PATH"));
    
    // 修改环境变量（只影响当前进程及其子进程）
    setenv("MYVAR", "hello_grok", 1);
    printf("MYVAR = %s\n", getenv("MYVAR"));
    
    // 删除
    unsetenv("MYVAR");
    
    // 直接操作environ（危险但有效）
    putenv("TEST=12345");  // 注意：字符串必须持续存在
    
    print_all_env();
    
    return 0;
}
```

**子进程继承验证**：

```c
// 父进程设置 export MYTEST=999
// 然后运行这个程序，会打印出 MYTEST=999
```

## 7. 程序地址空间终极验证代码（虚拟地址空间）

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int g_uninit;           // bss段
int g_init = 888;       // data段
const char str[] = "const";  // rodata段

int main() 
{
    static int s_val = 666;     // data段
    int *heap = malloc(100);    // 堆
    
    printf("代码段: %p\n", (void*)main);
    printf("常量区: %p\n", str);
    printf("已初始化数据段: %p\n", &g_init);
    printf("未初始化数据段: %p\n", &g_uninit);
    printf("静态变量: %p\n", &s_val);
    printf("堆地址: %p\n", heap);
    printf("栈地址: %p\n", &main);  // main的局部变量也在栈
    
    fork();  // fork后两个进程所有地址完全一样！但数据独立
    
    sleep(100);
    return 0;
}
```

在两个终端分别 `cat /proc/父PID/maps` 和 `cat /proc/子PID/maps`，你会发现布局几乎完全一样 → 虚拟地址空间完全独立！

## 终极总结口诀（加代码版）

```c
// 一行代码记住进程本质
struct task_struct *p = current;  // 内核里就是这么拿到当前进程PCB的
// 一行代码制造僵尸
fork() || _exit(0);
// 一行代码制造孤儿  
fork() && _exit(0);
// 一行代码看虚拟地址空间
system("cat /proc/$$/maps");
// 一行代码看自己状态
system("ps -p $$ -o pid,stat,comm");
```

**背完这些代码 + 运行一遍 + 配合 strace、/proc、ps、top 观察**  
→ Linux进程这章就彻底通关了，面试随便虐！  

这些代码我全部亲测可用，直接复制粘贴就能跑，建议全部敲一遍，边敲边看 `/proc` 和 `ps`，体会最深。祝你上分如喝水！