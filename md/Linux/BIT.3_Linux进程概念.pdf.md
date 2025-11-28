# BIT.3_Linux进程概念.pdf

## 1冯诺依曼体系结构



![image-20251109114302528](C:\Users\LIC\AppData\Roaming\Typora\typora-user-images\image-20251109114302528.png)

**这里的存储器是？内存**。**内存掉电易失**

**磁盘，外设，永久性存储能力。**

**外设：输入设备**  

**外设：输出设备**

**磁盘和网卡  既是输入设备也是输出设备**

**为什么是外设？相对于：内存+CPU**

**运算器+控制器+其它=CPU**                     **存储器 内存**            **外设**

**很快**                                                          **快                          慢**

**计算的                                                     临时存储的            永久性存储**

**CPU其实很笨，只能被动接受别人的指令，别人的数据-->执行别人的指令，计算别人数据的目的。** 

**CPU必须先，认识别人的指令？ CPU有自己的指令集。 我们写代码，编译本质-->二进制可执行程序。实际上就是翻译成CPU的指令集。**

**所以编译器存在的意义，翻译我们的程序到机器指令。**

**CPU在读取和写入的时候，在数据层面，只和内存打交道。为了提高整机效率。**



**CPU里面的数据，不是天然就存在的。从外设拿数据那就太慢了。所以综合考虑就从内存拿数据。**

**CPU读取和写入只和内存打交道，提高整机效率。**

**内存里面不是天然就有数据的**

**例如：开机的时候加载 操作系统加载到内存。**

**内存可以看做一个大大的缓存，匹配CPU和外设的速度。**



**OS操作系统---》帮我们做决策。**

**内存**
 **I/O  读写就是IO的过程。**
 **外设**

**总结**

**CPU不和外设直接打交道，只是和内存直接打交道。**

**所以的外设，有数据需要载入，只能载入到内存中，内存写出，也一定是写到外设中。**

**总结： ----》》CPU不和外设直接打交道，只和内存直接打交道。**



**程序要运行必须加载到内存？**

**为什么要加载到内存？**

**CPU要执行我的代码，访问我的数据。**

**只能从内存中读取。(体系结构规定的){为什么这样子规定，效率的问题}**

**进度条：数据写到内存里面---->没有及时刷新到外设。**



![image-20251109144658146](C:\Users\LIC\AppData\Roaming\Typora\typora-user-images\image-20251109144658146.png)

**两个朋友发消息，冯诺依曼体系结构**

| 计算器1          | 计算器2      |
| ---------------- | ------------ |
| 键盘输入         | 网卡输入     |
| 输入到内         | 输入到内存   |
| CPU读取          | 输入到CPU    |
| 网卡和显示器输出 | 输入到显示器 |
|                  |              |

![image-20251109145408179](C:\Users\LIC\AppData\Roaming\Typora\typora-user-images\image-20251109145408179.png)

**发文件也是类似的理解**

![image-20251109145545367](C:\Users\LIC\AppData\Roaming\Typora\typora-user-images\image-20251109145545367.png)



## 2操作系统(Operator System)

**OS是一个进行软硬件的资源管理的软件。**

| **用户**                                               |
| ------------------------------------------------------ |
| **用户接口**                                           |
| **系统接口**                                           |
| **进程管理，文件系统管理 ，内存管理，驱动管理。（os)** |
| **驱动**                                               |
| **硬件**                                               |

**OS为什么要进行管理呢？**

**通过合理的管理软硬件资源，为用户提供良好的执行环境。** **良好：稳定，安全，高效的。**

**是什么？：OS是一个进行软硬件的资源管理的软件**

**为什么？：通过合理的管理软硬件资源，为用户提供良好的执行环境。** **良好：稳定，安全，高效的**

**怎么办？：管理**



**举个例子：理解管理者**

**校长：管理者，决策权利**

**辅导员：驱动齿，执行权利**

**我们：被管理者**

**管理：管理不需要面对面，只需要拿到被管理者的数据。对重大事情有决策的权力。**

**做决策需要依据的，依据就是根据数据。管理的本质就是对数据做管理。**



**管理的本质是对:被管理者的数据做管理。**

**管理者，对重大事有决策的能力。决策的依据就是数据。**



管理者：  做决策                                                                                                   校长      OS(操作系统)   软件
执行者：  做执行 1.执行上面的命令 2.和被管理者接触，拿到相应的数据。       辅导员    驱动 (磁盘驱动，显示器驱动) 软件
被管理者：行动产生数据                                                                                      学生      硬件 (磁盘，网卡， 显卡，键盘，显示器) 硬件



**被管理者的数据太多了，怎么办？**

**学生的信息种类都是一样的。**

![image-20251109152509652](C:\Users\LIC\AppData\Roaming\Typora\typora-user-images\image-20251109152509652.png)

**对学生做管理，变成对链表数据做管理。**

**先描述：抽象数据。 c结构体，c++类**

**后组织：数据结构。数据结构组织起来**

![image-20251109153218268](C:\Users\LIC\AppData\Roaming\Typora\typora-user-images\image-20251109153218268.png)

**OS管理硬件：**

**struct dev {struct dev* next, int type, int status}**

**管理的本质：对数据做管理  **
**管理的方法：先描述（语言），在组织  数据结构**

**OS管理硬件：一模一样的**

**软件管理软件，软件管理硬件。**



**OS需要保护自己的同时，还要为用户提供服务。**

**OS不相信任何人**

​	**1.保护自己，     不让任何人直接进入操作系统**

​	**2.对外提供服务，系统调用接口**

| 计算机的软硬件体系结构                                       |
| ------------------------------------------------------------ |
| **用户**                                                     |
| **系统调用接口(系统调用)  (本质上就是c式的接口)  (man syscalls) (strace ls)**   **实际上就是函数接口 ** |
| **(shell(指令操作)/lib(c/c++)(编程操作)/界面)**              |
| **操作系统(进程控制，文件系统，内存管理，驱动管理)**         |
| **驱动**                                                     |
| **硬件**                                                     |

![image-20251109155021406](C:\Users\LIC\AppData\Roaming\Typora\typora-user-images\image-20251109155021406.png)

​                                                               **计算机软硬件体系结构**

![image-20251109161027105](C:\Users\LIC\AppData\Roaming\Typora\typora-user-images\image-20251109161027105.png)

**我们学的c/c++: 学习用户接口和系统调用的接口。**



**1.冯诺依曼体系**

**2.操作系统本身理解**

**3.计算机体系结构**

## 3进程

**一个运行起来(加载到内存)的程序---进程**

**在内存中的程序的-----------------进程**



**写好的程序本质就是文件，在磁盘上放着。**

**那么多的程序，操作系统如何管理呢？**

**先描述:PCB。进程控制块。Process control block**

**后组织：![image-20251109162111691](C:\Users\LIC\AppData\Roaming\Typora\typora-user-images\image-20251109162111691.png)**









```c
struct task_struct {
    pid_t pid;                     // 进程号
    long state;                    // 进程状态
    int prio;                      // 优先级
    struct mm_struct *mm;          // 地址空间信息
    struct files_struct *files;    // 打开的文件信息
    struct signal_struct *signal;  // 信号处理相关
    struct task_struct *parent;    // 父进程
    struct list_head children;     // 子进程链表
    struct list_head tasks;        // 系统全局进程链表节点
    struct thread_struct thread;   // CPU 寄存器等上下文信息
    struct sched_entity se;        // 调度实体信息
};

```



**所谓对进程进行管，变成了对进程对应的PCB进行相关的管理。**

**对进程的管理-->对链表的增删查改**

**struct task_struct 内核结构体 --> 内核结构体的对象 ---> 将该结构体和你的代码和数据关联起来----> 先描述，后组织。**

**进程 == 内核数据结构 + 进程对应的磁盘代码**

**为什么会有PCB(struct task_struct)的结构体呢？**

​	**进制在调度运行的时候，就具有动态属性。**

**ps -axj | head -1 && ps ajx | grep a.out **

**getpid()**

**/proc/id: 进程目录文件**

![image-20251109164334548](C:\Users\LIC\AppData\Roaming\Typora\typora-user-images\image-20251109164334548.png)

**进程的程序加载到内存**



**getpid()**

**getppid():终端运行的进程，一般都是当前bash分配一个子进程。**

![image-20251109164817115](C:\Users\LIC\AppData\Roaming\Typora\typora-user-images\image-20251109164817115.png)

**子进程出问题，不会影响父进程的。进程的独立性**

**fork():创建子进程**

**fork(): 子进程返回0，父进程返回子进程id，失败返回-1**

**fork之后代码共享，if分流**

**两个死循环和判断同时执行，证明fork之后是两个执行流。**

**通过返回值不同，让父子进程执行不同的代码。**

```c
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <assert.h>

int main()
{
  pid_t id = fork();
  assert(id >= 0);
    
  if(id == 0)
  {
    while(1)
    {
      printf("I AM PORCESS, ppid : %d, pid : %d id : %d \n", getppid(), getpid(), id);
      sleep(2);
    }
  }
  else
  {
    while(1)
    {
      printf("I AM PORCESS, ppid : %d, pid : %d id : %d \n", getppid(), getpid(), id);
      sleep(2);
    }
  }
    return 0;
}

```





## 4进程状态

**冯诺依曼体系：CPU只和内存打交道。外设也是只和内存打交道。缓存。**

**计算机是软件和硬件高度配合的**

 **读 输入-----内存----CUP**
 **写 CPU------内存----输出**

**CPU只能被动接收数据，指令。**

**CPU已经内置了指令。**

**二进制可执行程序---CUP认识的指令**

**控制CPU执行的**



**OS：是一个软硬件资源管理的软件。**

**OS对下软硬件资源**

**OS对上为用户提供良好的执行环境。**

**管理的本质就是：先描述，后组织。**

**管理者和被管理者不一定要接触。**

**管理的本质就是管理数据。**

**执行者：1被管理者的数据交给管理者 2执行指令。**



![image-20251109180931007](C:\Users\LIC\AppData\Roaming\Typora\typora-user-images\image-20251109180931007.png)

**管理的本质：先描述，后组织**

**先描述数据：结构体，类和对象。抽象出来**

**后组织数据：数据结构。链接起来的**

**OS维护一个数据结构-----维护对应的对象。 c struct ， cpp class**



**OS不相信任何人。**

**shell, c/c++ lib ,界面, 指令集合**



 **进程---操作系统里面的数据只能通过系统接口访问**

**什么是进程？**

**程序就是一段二进制代码,就是一个文件 **

**描述进程的PCB**

**管理每一个PCB，用数据结构组织起来。**

**程序加载带内存，操作系统瞬间创建一个PCB**

**PCB里面有进程的所有信息。 描述进程的全部信息。**

**进程 == 内核数据结构 + 进程对应的磁盘代码**

**PCB**

![image-20251109183114243](C:\Users\LIC\AppData\Roaming\Typora\typora-user-images\image-20251109183114243.png)



**fork创建子进程**

**进程状态**

 **进程状态**
 **1.运行**
 **2.新建**
 **3.就绪**
 **4.挂起**
 **5.阻塞**
 **6.等待**
 **7.停止**
 **8.挂机**
 **9.死亡**
 **都是操作系统的说法，太抽象了，不是具体的一款操作系统。**



![image-20251109191450337](C:\Users\LIC\AppData\Roaming\Typora\typora-user-images\image-20251109191450337.png)

![image-20251109191517066](C:\Users\LIC\AppData\Roaming\Typora\typora-user-images\image-20251109191517066.png)

**运行队列：放要执行的进程**

**进程的状态：进程内部的属性，放在PCB里面。不同的场景，不同的状态，等待不同的资源。**

**阻塞状态：PCB放到不同的队列里面。等待着**

**挂起：内核数据结构还在，code和data还在磁盘的**

![image-20251109191629994](C:\Users\LIC\AppData\Roaming\Typora\typora-user-images\image-20251109191629994.png)

![image-20251109193251666](C:\Users\LIC\AppData\Roaming\Typora\typora-user-images\image-20251109193251666.png)



**Linux内核源代码：进程状态**

![image-20251109183904911](C:\Users\LIC\AppData\Roaming\Typora\typora-user-images\image-20251109183904911.png)



![image-20251109194140097](C:\Users\LIC\AppData\Roaming\Typora\typora-user-images\image-20251109194140097.png)

**R运行状态（running）: 并不意味着进程一定在运行中，它表明进程要么是在运行中要么在运行队列里面。**



**kill  -19  暂停状态**

**kill -18 恢复暂停**

**kill -9 杀死进程**

![image-20251109201712780](C:\Users\LIC\AppData\Roaming\Typora\typora-user-images\image-20251109201712780.png)

**前台进程，后台进程。 带不带+**

**深度睡眠:D。disk sleep 磁盘睡眠**

![image-20251109203625094](C:\Users\LIC\AppData\Roaming\Typora\typora-user-images\image-20251109203625094.png)

**dd指令**

**复习**

**运行，阻塞，挂起。不同的状态，等待不同的资源。**

**运行队列 PCB在等待队列里面。**

**挂起你不一定看得到的**

![image-20251109204237965](C:\Users\LIC\AppData\Roaming\Typora\typora-user-images\image-20251109204237965.png)

**小t可以是暂停，就是在调试。**

**trace stop**

![image-20251111202307958](C:\Users\LIC\AppData\Roaming\Typora\typora-user-images\image-20251111202307958.png)

**D状态：一般是高IO**

**僵尸状态：**

![image-20251111202517909](C:\Users\LIC\AppData\Roaming\Typora\typora-user-images\image-20251111202517909.png)



**进程被创建处理---完成任务---1.要知道它完成的怎么样 2.可以不关心结果。**

**Linux你可以不用，但是我必须保存的。**

**进程退出的时候，不能立即释放进程对应的资源，保存一段时间，让父进程或者OS来进行读取。** **资源PCB**

**进程终止 exit（）函数**

**僵尸状态是一个问题暂时我们不解决的！ 进程控制的时候解决。  不回收僵尸状态--可能会造成内存泄漏**

**wait或者waitpid函数进行回收**

**僵尸状态是指：一个进程已经执行完毕并退出，但其在进程表中的条目仍然存在，等待父进程读取其退出状态信息的状态。**



**孤儿进程**

**父进程被杀掉，父进程的父进程回收资源了的。**

**父进程先退出， 然后子进程被操作系统1号进程领养了。**

**1.孤儿进程一定存在**

![image-20251111204403293](C:\Users\LIC\AppData\Roaming\Typora\typora-user-images\image-20251111204403293.png)



**精灵进程，守护进程。**



## 5优先级

**了解范畴**

**1.什么叫做优先级**

**优先级是有执行的权利，只不过是谁先完成后完成的过程。**   **权限是谁可以做什么的问题**

**2.为什么会存在优先级**

**为什么存在优先级？因为资源太少了。**

**3.Linux的优先级**

**优先级本质就是PCB里面的整数。**



**linux支持进程优先级调整的**

![image-20251111204858236](C:\Users\LIC\AppData\Roaming\Typora\typora-user-images\image-20251111204858236.png)

**数字越小，优先级越高。**

**top r 输入pid   【-20， 19】**

**每次都是默认的：80 +- ni**



![image-20251111205759749](C:\Users\LIC\AppData\Roaming\Typora\typora-user-images\image-20251111205759749.png)



**竞争性：资源太少了，想运行就得竞争。**

**独立性：QQ退出了，不会影响微信的**     **野指针模拟实现**

**并行：多个CPU执行，多个 进程。**

**并发：多个进程轮流到CPU执行。 看起来，大家都在执行。**   **时间片轮转的策略。进程不断切换**

**CPU任何一个时刻只能执行一个进程。**



**进程切换**

**pc(eip): 当前正在执行指令的下一条指令。**

**CPU：1取指令，2分析指令，3执行指令**



**当我们的进程在运行的时候，一定会产生很多的临时数据。这份数据属于当前进程！！**

**CPU虽然只有一套寄存器硬件，寄存器里面保存的数据，是属于当前进程的。**

**寄存器硬件 !=寄存器内的数据。**

**进程在运行的时候占CPU，进程不是占有到进程结束！！(while(1)) 死循环，那么就不能执行其它指令 了。**

**进程在运行的时候都有自己的时间片，(离开学校，保留学籍的过程，上下文保护。)(回到学校，恢复学籍，上下文恢复。)**

**保留就是为了恢复。**

**寄存器 (寄存，寄存，寄存进程的临时数据。)。**

**上下文：寄存器里面的数据**

**进程切换的时候，    要进程上下文保护。**

**当进程恢复的时候，要进程上下文恢复。**

**上下文数据：进程的上下文主要保存在该进程的“进程控制块”的数据结构中，而PCB通常存在于操作系统内核空间的内存里。**



**寄存器 被所有进程共享 **

![image-20251112115132338](C:\Users\LIC\AppData\Roaming\Typora\typora-user-images\image-20251112115132338.png)



**难学的东西是知识，简单的东西是信息。**



## 6环境变量

**在 **Linux 中，**环境变量（environment variables）** 是操作系统中用于存储系统运行环境配置信息的一种 **键值对（key-value pair）**，其主要作用是：用来影响进程的行为，例如查找命令路径、设定语言、用户目录、库路径等。

**Linux的指令也是可执行程序。**

**要执行一个基本指令(程序），先找到这个程序。**

**echo $PATH**

**echo $HOME**

**添加环境变量**

**export  PATH=$PATH:/home/lic/lesson11/**

**which本质就是根据环境变量PATH查找数据的。**



**Linux的环境变量配置：**

**用户**

![image-20251112121052229](C:\Users\LIC\AppData\Roaming\Typora\typora-user-images\image-20251112121052229.png)



**系统配置**

**/etc/bashrc**

![image-20251112121302941](C:\Users\LIC\AppData\Roaming\Typora\typora-user-images\image-20251112121302941.png)



**echo $HOME**



**复习 **

![image-20251112122123797](C:\Users\LIC\AppData\Roaming\Typora\typora-user-images\image-20251112122123797.png)

**进程优先级一般不需要调整，除非你特别了解。或者你真的需要调整。**

**进程切换 ，CPU轮转的临时数据，在寄存器 里面的数据(进程的上下文)，上下文保护，上下文切换。**

**CPU:取指令，分析指令，执行指令。**

**进程切换**

**时间片轮转。执行进程。**

**并发：每个进程在时间片跑跑跑。**

**时间片单位，**

**CPU---PCB---Code&Data--address--reall address**

**例子：学籍保留**

**su -重新登录**

**su 切换用户**



**不同的环境变量，使用不同的场景。**

**user:环境变量的意义，可以表示当前那个用户在使用Linux**



```c
//  #include <stdlib.h>
//  char *getenv(const char *name);

#include <stdio.h>
#include <stdlib.h>
int main()
{
  printf("%s \n", getenv("HOME"));
  printf("%s \n", getenv("USER"));
  printf("%s \n", getenv("PWD"));
  printf("%s \n", getenv("PATH"));
  return 0;
}

```





**本地环境变量和全局环境变量**

**aaa=123**

**export aaa  导入全局环境变量**

**当前bash的全局环境变量，会被子进程继承下去的。**

**本地变量-- 只会在当前bash内有效的**



**aaa=123  定义局部环境**

**export aaa 设置全局环境变量**

**unset aaa 取消全局环境变量**



**export MYAVL=2222**

**ls 继承了PWD，所以不断切换目录，ls知道你当前在那个目录下面。**



**环境变量是如何被子进程继承？？**

**main函数参数和extern enviro **



**命令行参数**

**argc个数**

**argv内容**

```c
#include <stdio.h>
int main(int agrc, char* argv[])
{
  for(int i = 0; i < agrc; i++)
  {
    printf("argv[%d] : %s\n", i, argv[i]);
  }
  return 0;
}

```

![image-20251112131717945](C:\Users\LIC\AppData\Roaming\Typora\typora-user-images\image-20251112131717945.png)

![image-20251112131841274](C:\Users\LIC\AppData\Roaming\Typora\typora-user-images\image-20251112131841274.png)



**命令行选项**

```c
#include <stdio.h>
#include <string.h>

// ./mycmd -a -b -c
// ./mycmd -ab -bc -ac
// ./mycmd -abc

int main(int agrc, char* argv[])
{
  if(agrc != 2)
  {
    printf("Usage: \n%s [-a/-b/-c/-ab/-ac/-bc/-abc]\n", argv[0]);
    return 1;
  }

  if(strcmp("-a", argv[1]) == 0)
  {
    printf("功能a\n");
  }
  if(strcmp("-b", argv[1]) == 0)
  {
    printf("功能b\n");
  }
  
  if(strcmp("-c", argv[1]) == 0)
  {
    printf("功能c\n");
  }

  if(strcmp("-ab", argv[1]) == 0)
  {
    printf("功能ab\n");
  }

  if(strcmp("-ac", argv[1]) == 0)
  {
    printf("功能ac\n");
  }

  if(strcmp("-bc", argv[1]) == 0)
  {
    printf("功能bc\n");
  }
  if(strcmp("-abc", argv[1]) == 0)
  {
    printf("功能abc\n");
  }
  return 0;
}

```



**环境变量表**

```c
#include <stdio.h>
#include <string.h>

int main(int agrc, char* argv[], char* env[])
{
  for(int i = 0; env[i]; i++)
  {
    printf("%d : %s\n", i, env[i]);
  }
  return 0;
}
```



**extern char  environ**

```c
#include <stdio.h>
#include <string.h>
#include <unistd.h>

extern char** environ;
int main(int agrc, char* argv[], char* env[])
{
  
  for(int i = 0; environ[i]; i++)
  {
    printf("%d : %s \n", i, environ[i]);
  }
    
  return 0
}

```



**getenv()<u>**</u>

```c
#include <stdio.h>
#include <stdlib.h>
int main()
{
  printf("%s \n", getenv("HOME"));
  printf("%s \n", getenv("USER"));
  printf("%s \n", getenv("PWD"));
  printf("%s \n", getenv("PATH"));
  return 0;
}
```



**putenv()**

**#include <stdlib.h>
int putenv(char *string);**

**putenv不重要**





**#include <sys/types.h>**
**#include <sys/stat.h>**
**#include <unistd.h>**

**int stat(const char *pathname, struct stat *statbuf);**





## 7程序地址空间



![image-20251112231621135](C:\Users\LIC\AppData\Roaming\Typora\typora-user-images\image-20251112231621135.png)



**见见猪跑**

```c
#include <stdio.h>
#include <string.h>
#include <unistd.h>


extern char** environ;
int g_val = 100;

int main()
{

  pid_t id = fork();
  if(id < 0)
  {
    perror("fork failed");
    return 1;
  }
  else if(id == 0)
  {
    g_val = 1;
    while(1)
    {
      printf("我是子进程、 g_val：%d , &g_val : %p \n",  g_val, &g_val);
      sleep(1);
    }
  }
  else 
  {
    g_val = 2;
    while(1)
    {
      printf("我是父进程、 g_val：%d, &g_val  : %p \n",  g_val, &g_val);
      sleep(1);
    }
  }

    return 0;
}
```



![image-20251112232232806](C:\Users\LIC\AppData\Roaming\Typora\typora-user-images\image-20251112232232806.png)

**多进程读取同一个地址，怎么可能出现不同的结果？  进程的独立性，同一个地址，不同的数据。**

**地址没变----》这里的地址，绝对不是物理地址---》曾经我们学习语言级别的地址，绝对不是对应的物理地址。---》虚拟地址（线性地址）  c/c++打印出来的全部都是虚拟地址。**



**进程他会认为自己独占系统资源（实际上不是）。这是设计上的理念。**

**三son**

![image-20251112233030319](C:\Users\LIC\AppData\Roaming\Typora\typora-user-images\image-20251112233030319.png)



![image-20251112234206873](C:\Users\LIC\AppData\Roaming\Typora\typora-user-images\image-20251112234206873.png)



**复习**

**环境变量，全局属性，方便不同的场景使用**

**命令行参数，根据选项执行不同的功能。**

**mian函数表，环境变量表。推荐getenv()**

**echo 怎么能够打出局部变量**

**进程地址空间，是虚拟地址空间。**

**进程独立性，打印一个地址，然而是不同的数据。**

**进程地址空间，就是一种数据结构。每一个地址空间管理起来。mm_struct**

**复习**



**mm_struct{} 成员有哪些呢？**

**1.地址空间的描述的基本空间大小是字节
 2.32位---2^32个地址
 3.2^32 * 1字节 = 4GB的空间范围
 4.每一个字节唯一的地址**

**2^32个地址保证唯一性就行了。**

**32位的数据即可**

**43 默认就是2^32次方个地址。**

**unsigned int(32bits)**

**struct mm_struct
 {
 uint32_t code_start;
 uint32_t code_end;
 uint32_t data_start;
 uint32_t data_end;
 uint32_t heap_start;
 uint32_t heap_end;
 uint32_t stack_start;
 uint32_t stack_end;
 };**

**[start, end]---[起始地址，区域结束地址]**

**举个栗子**

![image-20251113113923267](C:\Users\LIC\AppData\Roaming\Typora\typora-user-images\image-20251113113923267.png)





**[start, end]---[起始地址，区域结束地址]**

**struct mm_struct* p =  (struct mm_struct*)malloc(sizeof(struct mm_struct));**

![image-20251113123910424](C:\Users\LIC\AppData\Roaming\Typora\typora-user-images\image-20251113123910424.png)

**66 -----堆栈可以调整------(本质就是修改end或者start)
 67 定义局部变量，malloc ,new---->扩大堆栈区域
 68 函数调用完毕，freee, delete-->缩小堆栈区域**



![image-20251113124115643](C:\Users\LIC\AppData\Roaming\Typora\typora-user-images\image-20251113124115643.png)



**页表**

![image-20251113125731626](C:\Users\LIC\AppData\Roaming\Typora\typora-user-images\image-20251113125731626.png)

**程序加载到内存里面**

**OS创建PCB--->里面有程序的信息。**

**内存IO一般是4KB为单位的。Page页**

**线性地址：数组就是线性地址。**



**在进一步**

![image-20251113130437519](C:\Users\LIC\AppData\Roaming\Typora\typora-user-images\image-20251113130437519.png)



**如何理解地址空间呢？**

**1.如果让进程直接访问物理内存，万一进程越界非法操作呢？非常不安全呢！(页表不仅仅只做映射，还会检查映射是否安全呢) 安全！所以进程都必须要遵守的。  安全的**

**2.地址空间的存在可以更方便，进程和进程的数据代码的解耦，保证了进程的独立性的特征。**

**3.让进程以同一的视角，来看待进程对应的代码和数据等各个区域，方便
编译器也以统一的视角进行编译代码。
规则一样的，编译完即可直接使用的。**



**编译代码的时候，使用不用的地址形式。**

![image-20251113160630115](C:\Users\LIC\AppData\Roaming\Typora\typora-user-images\image-20251113160630115.png)





![image-20251113160958202](C:\Users\LIC\AppData\Roaming\Typora\typora-user-images\image-20251113160958202.png)





![image-20251113161014941](C:\Users\LIC\AppData\Roaming\Typora\typora-user-images\image-20251113161014941.png)

































