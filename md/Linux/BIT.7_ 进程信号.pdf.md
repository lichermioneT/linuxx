# BIT.7_ 进程信号.pdf



## 1信号量是什么

**信号量是什么？本质是一个计数器，通常用来表示公共资源数量的多少问题。**

**公共资源：被多个进程同时可以访问的资源。**

**访问没有保护的公共资源的时候；数据不一致的问题。**

**让不同的进程看到同一份资源呢？因为我想通信，进程间实现协同--》进程独立性--》需要让进程看到同一份资源--》提出方法，引进新的问题！（数据不一致的    问题）**



**数据不一致的问题**

**我们未来将被保护起来的公共资源：临界资源，有大部分的资源是独立的，**

**资源（内存，网络，文件）是要被使用的，如何被进程使用呢？一定是该进程有对应的代码来访问这部分资源：临界区  非临界区 访问资源**



**保护策略：互斥和同步**

**互斥，要做就做完了，才能放开的。**

**两态**

**要么不做，要么做完，两态的这种情况：原子性**

**要么不做，要么做完，两态的这种情况：原子性**

**要么不做，要么做完，两态的这种情况：原子性**



**我为什么需要信号量？**

**共享资源：1作为一个整体使用，2划分成为一个一个的资源子部分**

**进程访问资源子部分---》先访问信号量---》---》成功就有权访问资源子部分----------》信号量**



![image-20251126152024224](C:\Users\LIC\AppData\Roaming\Typora\typora-user-images\image-20251126152024224.png)

![image-20251126152316526](C:\Users\LIC\AppData\Roaming\Typora\typora-user-images\image-20251126152316526.png)

**sem--**

**sem--:预定资源 所有的进程在访问公共资源之前，都必须先申请sem信号量-》申请信号量的前提是，先看到同一个信号量--》**

**信号量本身就是公共资源。--》信号量也要保证自己的安全》信号量保证自身的安全**



![image-20251126152833533](C:\Users\LIC\AppData\Roaming\Typora\typora-user-images\image-20251126152833533.png)



**p --  申请资源**

**v++ 释放资源**

**信号量 1是就是互斥信号量，二元信号量**



**IPC资源组织方式**



**本质上实现多态**




## 2硬件信号

**整个信号的生命周期**

**预备**

**信号产生**

**信号保存**

**信号处理**

![image-20251127091953887](C:\Users\LIC\AppData\Roaming\Typora\typora-user-images\image-20251127091953887.png)

**kill -l 信号的种类**

**1-31:普通信号**

**34-64:实时信号**



**识别信号**

**产生信号**

**1能够识别信号**

**2信号到来的时候，我们不一定马上处理这个信号**

​	**信号可以随时产生（异步），我们可能做着更重要的事情**

**3信号到来-------时间窗口（必须记住这个信号）---------信号被处理**

**4默认动作，自定义动作，忽略动作，**

![image-20251127092321116](C:\Users\LIC\AppData\Roaming\Typora\typora-user-images\image-20251127092321116.png)





**1信号是给进程发送的**

**进程是如何识别信号的 == 认识 + 动作**

**进程本身是程序员编写的属性和逻辑的集合---程序员编码完成的**

**进程收到信号的时候，可能正在执行更重要的代码，进程不一定会被立即执行的**

**进行本身要有对于信号的保存能力**

**进行在处理信号的时候：1.默认动作2.自定义动作3.忽略【信号被捕捉】**

![image-20251127092509900](C:\Users\LIC\AppData\Roaming\Typora\typora-user-images\image-20251127092509900.png)





**如果一个信号是发送给进行的，而进程要保存，那么应该保存在哪里呢，PCB里面，task_struct里面**

**如何保存呢？是否收到了指定信号【1,31】**

```c
 struct task_struct                                                                                                           
 {
    unsigned int signal;
    0000 0000 0000 0000 0000 0000 0000 0000
    比特位的内容代表是否收到内容，0无，1有
  }

```

**如何理解信号的发送？**

**发送信号的本质；就是修改PCB的信号位图。**

**PCB内核维护的数据结构对象，PCB的管理者是OS,只有OS有权利修改PCB**

**未来我们学习多少种发送信号的方式，本质上都是通过OS向目标进程发送信号**

**os提供 发送处理信号的相关系统调用**

**kill---底层调用了对应的系统调用。**

![image-20251127094134475](C:\Users\LIC\AppData\Roaming\Typora\typora-user-images\image-20251127094134475.png)

![image-20251127094800384](C:\Users\LIC\AppData\Roaming\Typora\typora-user-images\image-20251127094800384.png)



```c++
#include <signal.h>
typedef void (*sighandler_t)(int);
sighandler_t signal(int signum, sighandler_t handler);
```



```c++
#include <iostream>
#include <unistd.h>
#include <signal.h>
using namespace std;

void hander(int signo)
{
  cout<<"进程捕捉到了一个信号，信号的编号是：" << signo <<endl;
  // exit(0);
}

int main()
{
  // signal函数的调用，并不是hander函数的调用
  // 仅仅是对2号信号的捕捉方法，并不代表该方法调用了
  // 一般这个方法不会执行,除非收到信号
  signal(2, hander);

  while(true)
  {
    cout<< "i am process " << endl;
    sleep(2);
  }
  return 0;
}
```



**复习**

**识别信号**

**合适时机处理**

**保存信号**

**信号的处理动作**

**PCB的属性--关于进程的信号**

**复习**



**1键盘发送**

**2kill发送**

```c++
  #include <iostream>    
  #include <cstdio>    
  #include <string>    
  #include <sys/types.h>    
  #include <unistd.h>    
  #include <signal.h>    
  #include <stdlib.h>    
  using namespace std;    
      
  void hander(int signo)    
  {    
    cout<<"进程捕捉到了一个信号，信号的编号是：" << signo <<endl;    
    // exit(0);    
  }    
      
  // 使用方法    
  static void Usage(const string& proc)    
  {    
    cout<< "\nUage: " << proc << " pid signo \n" <<endl;    
  }    
     
  int main(int agrc, char* agrv[])    
  {    
      
    if(agrc != 3)
    {
      Usage(agrv[0]);
      exit(1);
    }
  
    pid_t id = atoi(agrv[1]);
    int signo = atoi(agrv[2]);
    
    int n = kill(id, signo);
    if(n != 0)
    {
      perror("kill fail");
    }
	
    return 0;
}

```

```c++
#include <iostream>                                                    
#include <sys/types.h>    
#include <unistd.h>       
    
int main()    
{       
  while(true)    
  {    
    std::cout<< "我是一个进程 pid :" << getpid() <<std::endl;    
    sleep(1);    
  }    
  return 0;    
}
```



![image-20251127101342574](C:\Users\LIC\AppData\Roaming\Typora\typora-user-images\image-20251127101342574.png)

**kill接口的使用**

```c++
#include <sys/types.h>
#include <signal.h>
int kill(pid_t pid, int sig);
```





**raise接口**

**自己给自己发送信号**

```c++
  
  
#include <iostream>                                                    
#include <sys/types.h>    
#include <unistd.h>       
    
int main()    
{       
    int cnt = 0;    
      
    while(cnt < 10)    
    {    
      printf("cnt : %d \n", cnt++);    
      sleep(1);    
      /*    
       *if(cnt >= 5) abort();    
       */    
      
      if(cnt >= 5) raise(3);    
    }    
       
  return 0;    
}
      
```

```c++
 #include <signal.h>
 int raise(int sig);
```



**abort**

```c++
#include <iostream>                                                    
#include <sys/types.h>    
#include <unistd.h>       
    
int main()    
{       
    int cnt = 0;    
      
    while(cnt < 10)    
    {    
      printf("cnt : %d \n", cnt++);    
      sleep(1);    
      /*    
       *if(cnt >= 5) abort();    
       */    
      
      if(cnt >= 5) raise(3);    
    }    
       
  return 0;    
}
      
```



```c++
 #include <stdlib.h>
 void abort(void);
```





**关于信号处理的行为理解：有很多的情况，进程收到大部分的信号。默认处理动作都是终止进程**

**信号的意义：信号的不同，代表不同的事件，但是对事件发生之后的处理动作可以一样的**



```c++
  #include <iostream>    
  #include <cstdio>    
  #include <string>    
  #include <sys/types.h>    
  #include <unistd.h>    
  #include <signal.h>    
  #include <stdlib.h>    
  using namespace std;    
      
  void catchSig(int signo)
  {
    cout<< "获取到一个进程 信号编号是 ：" << signo << endl;
  }


int main(int agrc, char* agrv[])    
 { 
    signal(SIGFPE, catchSig);
    
    while(true)
    {
      cout<<"我在运行。。。"<<endl;
      sleep(1);
      int a = 10;
	 a /= 0;
    }
    
    return 0;
}:
```

**除零一次，发送无数次信号捕捉。**

**CPU里面有很多状态寄存器，溢出标记位。**

**CPU运算异常**

**OS识别问题**

![image-20251127144110336](C:\Users\LIC\AppData\Roaming\Typora\typora-user-images\image-20251127144110336.png)



**一直进程切换，然后标志位异常。**

**进程收到信号不一定会影响进程退出，没有退出还有可能被调度**

**CPU内部寄存器只有一份，但是寄存器中的内容，属于当前进程的上下文**

**你有能力解决吗？不能的**

**当进程被切换的时候，就有无数次状态寄存器被保存和恢复的过程**

**每一次恢复的时候，就让OS识别到了CPU内部的状态寄存器的标志位是1**



**OS怎么知道我野指针了呢？**

**虚拟地址----页表+MMU----物理内存。MMU出现异常了，OS马上发送信号给出现异常的进程。**

![image-20251127145213598](C:\Users\LIC\AppData\Roaming\Typora\typora-user-images\image-20251127145213598.png)



```c++
  #include <iostream>    
  #include <cstdio>    
  #include <string>    
  #include <sys/types.h>    
  #include <unistd.h>    
  #include <signal.h>    
  #include <stdlib.h>    
  using namespace std;    
      
  void catchSig(int signo)
  {
    cout<< "获取到一个进程 信号编号是 ：" << signo << endl;
  }


int main(int agrc, char* agrv[])    
 { 
    signal(11, catchSig);
    
    while(true)
    {
      cout<<"我在运行。。。"<<endl;
      sleep(1);
      int* p = nullptr;
      p = nullptr
	  *p = 0;
    }
    
    return 0;
}:
```



## 3软件信号

**软件产生**

**软件条件产生异常**

**管道**

**读端关闭**

**写端开启---->OS不允许**

![image-20251127145724542](C:\Users\LIC\AppData\Roaming\Typora\typora-user-images\image-20251127145724542.png)**



**一秒钟计算+打印数据**

```c++
  #include <iostream>    
  #include <cstdio>    
  #include <string>    
  #include <sys/types.h>    
  #include <unistd.h>    
  #include <signal.h>    
  #include <stdlib.h>    
  using namespace std;    
      


int main(int agrc, char* agrv[])    
 { 
 	alarm(1); // 一次性闹钟
	int cnt = 0;
    while(true)
    {
        cout<< "cnt:" << cnt++ <<endl;
    }
    
    return 0;
}:
```



**只是计算，最后打印**

```c++
  #include <iostream>    
  #include <cstdio>    
  #include <string>    
  #include <sys/types.h>    
  #include <unistd.h>    
  #include <signal.h>    
  #include <stdlib.h>    
  using namespace std;    
      
  void hander(int signo)    
  {    
    cout<<"进程捕捉到了一个信号，信号的编号是：" << signo <<endl;    
    // exit(0);    
  }    
      
  // 使用方法    
  static void Usage(const string& proc)    
  {    
    cout<< "\nUage: " << proc << " pid signo \n" <<endl;    
  }    
      
      
  int  cnt = 0;    
  void catchSig(int signo)    
  {    
    cout<< "获取到一个进程 信号编号是 ：" << signo <<": cnt : "<< cnt<< endl;                                             //exit(0);
    alarm(1);
  }  

int main(int agrc, char* agrv[])    
{
     signal(SIGALRM, catchSig);
    alarm(1); // 一次性闹钟
    while(true)
    {
      cnt++;
    }
	
    return 0;
}  

```



**结论：外设真的很慢的！！！**



**如何理解闹钟就是用软件实现的**

**闹钟为什么就是软件呢？**

**实际上闹钟就是软件实现的**

**任何一个进程，都可以通过alarm系统掉用在内核中设置闹钟，OS可能会存在很多的闹钟，OS要不要管理这些闹钟呢？**

**先描述，后组织！**

![image-20251127153856944](C:\Users\LIC\AppData\Roaming\Typora\typora-user-images\image-20251127153856944.png)

![image-20251127153516493](C:\Users\LIC\AppData\Roaming\Typora\typora-user-images\image-20251127153516493.png)

![image-20251127154016030](C:\Users\LIC\AppData\Roaming\Typora\typora-user-images\image-20251127154016030.png)



**上面所说的所有信号产生，最终都要有OS来进行执行，为什么？OS是进程的管理者**

**信号的处理是否是立即处理的？在合适的时候**

**信号如果不是被立即处理，那么信号是否需要暂时被进程记录下来？记录在哪里最合适呢？**

**一个进程在没有收到信号的时候，能否能知道，自己应该对合法信号作何处理呢？**

**如何理解OS向进程发送信号？能否描述一下完整的发送处理过程？**



## 4核心转储

![image-20251127154242477](C:\Users\LIC\AppData\Roaming\Typora\typora-user-images\image-20251127154242477.png)



**退出时的核心问题，核心转储**

**云服务器上，默认是关闭的**

**ulimit -c 1024打开核心存储的文件**

**当进程出现异常的时候，我们将进程对应的时候，在内存中的数据转储到磁盘中，核心转储！**

![image-20251127155028545](C:\Users\LIC\AppData\Roaming\Typora\typora-user-images\image-20251127155028545.png)

**以pid命名的文件**

![image-20251127155145645](C:\Users\LIC\AppData\Roaming\Typora\typora-user-images\image-20251127155145645.png)



**事后调试**

**为什么要有核心转储的？**

**为什么崩溃，哪里崩溃？**

**用来支持调试**

**gdb core-file core.pid**



**学习信号的流程**

![image-20251127155524817](C:\Users\LIC\AppData\Roaming\Typora\typora-user-images\image-20251127155524817.png)



**静止捕捉9号信息**



**实际执行信号的处理动作称为信号递达(Delivery)**   **捕捉函数，或者终止进程**

**信号从产生到递达之间的状态,称为信号未决(Pending)。** **收到，没有进行处理的**

**进程可以选择阻塞 (Block )某个信号。**  **阻塞信号 block信号**

**被阻塞的信号产生时将保持在未决状态,直到进程解除对此信号的阻塞,才执行递达的动作.**

**注意,阻塞和忽略是不同的,只要信号被阻塞就不会递达,而忽略是在递达之后可选的一种处理动作。**



## 5阻塞信号

**预备工作  信号的发送 信号的保存 信号的抵达处理                                                                                                **

**今天保存和抵达**

```c
task_struct
{
	// pending位图
unsigned int pending = 0;  // 0000 0000  0000 0000  0000 0000  0000 0000 1.比特位的位置，信号编号。2.比特位的内容，是否，收到了对应的信号
	// block位图
unsigned int block = 0;    // 0000 0000  0000 0000  0000 0000  0000 0000 2.比特位的位置，信号编号。2.比特位的内容，是否，阻塞了对应的信号
hander_t hander[32];      //  函数指针数组，a数组的位置(下标)，信号的编号，b数组下标对应的内容，表示对应信号的处理方法

}
```

**发生信号的载体只能是OS，OS才有权利修改PCB**

**pending位图：信号处于未决状态。**

**block位图：信号是否阻塞**

**hander_t： 信号处理动作**

![image-20251128094002484](C:\Users\LIC\AppData\Roaming\Typora\typora-user-images\image-20251128094002484.png)



```c
  // 信号抵达的伪代码
  // signo从1开始
  if((1<<(signo)-1) & pcb->block)
 {
     //signo信号是被阻塞，不抵达
 }
 else
 {
		 if((1<<(signo-1)) & pcb->pending)                                                                                   {
		  	// 信号抵达

		 }
}
```



**typedef void(*header_t)(int signo) // 函数指针**

**hander_t hander[32] = {0};         // 函数指针数组**



![image-20251128094515274](C:\Users\LIC\AppData\Roaming\Typora\typora-user-images\image-20251128094515274.png)



![image-20251128094721536](C:\Users\LIC\AppData\Roaming\Typora\typora-user-images\image-20251128094721536.png)



**今日重点: 信号的捕捉流程!!**

**信号产生的时候，不会立即处理，而是在合适的时候。从内核态返回用户态的时候，进行处理。**

**OS是一个软硬件管理的软件**

**用户态的：-》1.操作系统自身的资源(getpid(), waitpid()) 2.硬件资源(printf(), read(), write())**

**为了用户访问内核或者硬件资源，必须通过系统调用完成访问。 printf() 底层还是访问操作系统的接口, 访问硬件：显示器。 io/内存**



**内核态:用户态变成用户态的时候，调用系统调用。实际执行系统调用的进程，但身份其实是内核。 往往系统调用比较费时间。尽量避免频繁调用系统调用。**

**1.可见寄存器 :**

**2.不可见寄存器 :**

**凡是和当前进程强相关的，上下文数据。**

**CR3:0内核态，3用户态**

![image-20251128095906885](C:\Users\LIC\AppData\Roaming\Typora\typora-user-images\image-20251128095906885.png)

![image-20251128095147259](C:\Users\LIC\AppData\Roaming\Typora\typora-user-images\image-20251128095147259.png)

![image-20251128095211436](C:\Users\LIC\AppData\Roaming\Typora\typora-user-images\image-20251128095211436.png)



**内核级别页表只需存在一份就够了**

**内核空间都有一份的**

**我一直不太理解：我是一个进程，怎么跑到OS中执行方法呢？ 用户空间！**

**task_struct{} 曾经都是用户空间  用户级别页表， 每个用户都有自己独立的用户级别页表**

**内核级页表 内核级页表只需要有一份就够了  访问OS**

**每一个进程都有自己的地址空间(用户空间独占)， 内核空间（被映射到每一个进程3-4G)  进程要访问OS的接口，其实只需在自己的地址空间上跳转就可以了？**

**每个进程都有3-4g，都会共享一个内核级页表，无论进程如何切换，不会更改任何的3-4g**

**用户，凭什么能够访问内核的接口或者数据呢？ CR3寄存器，0内核态，3用户态。**

**系统调用接口，起始位置会帮你做的！ Int 80 陷入内核。3--0**



**信号产生的时候，不会立即处理，而是在合适的时候。从内核态返回用户态的时候，进行处理。**

**曾经我们一定是先进入内核态！ 系统调用，进程切换，**

![image-20251128101837490](C:\Users\LIC\AppData\Roaming\Typora\typora-user-images\image-20251128101837490.png)





![image-20251128102335873](C:\Users\LIC\AppData\Roaming\Typora\typora-user-images\image-20251128102335873.png)

 ![image-20251128102455980](C:\Users\LIC\AppData\Roaming\Typora\typora-user-images\image-20251128102455980.png)





![image-20251128102738240](C:\Users\LIC\AppData\Roaming\Typora\typora-user-images\image-20251128102738240.png)



```c++
#include <signal.h>
int sigemptyset(sigset_t *set);
int sigfillset(sigset_t *set);
int sigaddset (sigset_t *set, int signo);
int sigdelset(sigset_t *set, int signo);
int sigismember（const sigset_t *set, int signo); 

```











































