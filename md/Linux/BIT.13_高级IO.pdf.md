# BIT.13_高级IO.pdf

## 重点

理解五种IO模型的基本概念, 重点是IO多路转接.

 掌握select编程模型, 能够实现select版本的TCP服务器.

 掌握poll编程模型, 能够实现poll版本的TCP服务器. 

掌握epoll编程模型, 能够实现epoll版本的TCP服务器. 

理解epoll的LT模式和ET模式. 

理解select和epoll的优缺点对比. 



## 什么是IO

**什么是IO？什么是高效的IO？**

**网络和系统都是从缓冲区里面拷贝数据。**

**read/recv**

**1.没有数据，阻塞住**

**2有数据，拷贝完成就返回了。**

**阻塞的本质：阻塞的本质就是进程等待某种资源就绪**

**IO的本质：等 + 数据拷贝。**

**高效的IO：数据拷贝是软件决定的，该是多少就是多少。  减少等的时间重就是高效的IO。**

![image-20260330192412289](picture/image-20260330192412289.png)

## IO的方式

**钓鱼：等 + 钓**

**1.张三：等 + 钓                                                                            阻塞**

**2.李四：边等 + 边学习 + 边钓鱼                                                     非阻塞**

**3.王五：边等鱼竿的铃铛 + 边学习  + 听消息                                   信号驱动**

**4.赵六：等一排鱼竿                                                                            多路转接/多路复用**

**5.田七：让小王钓鱼到指定目标就返回。 田七自己忙，小王钓鱼。    异步IO**

**等的比重低，旁观者看来就是高效的。**

**鱼：数据**

**河：内核**

**鱼鳔：就绪事件**

**鱼竿：文件描述符**

**钓鱼的动作：read/recv。**

![image-20260330193002841](picture/image-20260330193002841.png)

**五种IO模型**

****

**3,4,5效率是没有区别的。其它方面有区别的。4,5表现在可以做其它方面的事情。**

**6信号驱动一定等了。**

**3,4,5,6：都参与等了，参与IO的等待，这就是同步IO的。**

**田七：没有参与IO等待的过程，这就是异步IO。**

**阻塞和非阻塞：等待的方式不一样了的。**

**多路转接、多路复用为什么高效？减少等待的时间了。**

![image-20260330193711169](picture/image-20260330193711169.png)

阻塞

![image-20260330193840448](picture/image-20260330193840448.png)

**非阻塞IO**

![image-20260330193905917](picture/image-20260330193905917.png)

**信号驱动**

**![image-20260330193955157](picture/image-20260330193955157.png)**

**多路转接：一次等待多个文件描述符合。**

**select, poll epoll:只是负责等，只是负责等，只是负责等。 需要配合其它的IO函数。**

![image-20260330194026620](picture/image-20260330194026620.png)

**异步IO**

![image-20260330194334585](picture/image-20260330194334585.png)

![image-20260330194348115](picture/image-20260330194348115.png)

**同步IO和异步IO：参与等没有。**



```c++
       #include <unistd.h>
       #include <fcntl.h>

       int fcntl(int fd, int cmd, ... /* arg */ );


```

![image-20260406094322074](picture/image-20260406094322074.png)

## 非阻塞IO



```c++
#include "util.hpp"    
#include <vector>    
#include <cstdio>    
#include <functional>    
    
using func_t = std::function<void()>;    
    
#define INIT(cbs) do{\    
      cbs.push_back(printLog);\    
      cbs.push_back(download);\    
      cbs.push_back(executeSql);\    
    }while(0)    
    
#define EXEC_OTHER(cbs) do{\    
      for(auto const& cb : cbs) cb();\    
}while(0)    
    
    
int main()    
{    
  std::vector<func_t> cbs;    
  INIT(cbs);    
    
// 1.设置成非阻塞模式的    
//   读数据的时候，没有就立即返回的。    
  setNoBlock(0);    
    
  char buffer[1024] = {0};    
    
// 2.有点类似轮询的场景的    
  while(true)    
  {    
    printf(">>>>");    
    fflush(stdout);    
    ssize_t s = read(0, buffer, sizeof(buffer) - 1);    
    if(s > 0)    
    {    
      buffer[s] = 0;    
      std::cout<< "echo#" << buffer << std::endl;    
    }    
    else if(s == 0)    
    {    
      std::cout<< "read end" << std::endl;    
      break;    
    }    
    else    
    {    
      // s==-1    
      // 1.当我不输入的时候，底层没有数据，算错误吗？不算错误，只不过以错误的形式返回了    
      // 2.那我如何区：是真的错了，还是底层没有数据？(单纯返回值不能够区分的，还需要根据错误码进行判断的)    
      /*    
       *std::cout<< "s:" << s << " erron:"<< errno <<std::endl; // errno是11，临时资源没有准备好的    
       */    
      /*    
       *std::cout<< "s:" << s << " erron:"<< strerror(errno) <<std::endl;    
       */    
    
      /*    
       *std::cout<< "EAGAIN:" << EAGAIN << " EWOULDBLOCK:" << EWOULDBLOCK << std::endl; // 两个都是11的。    
       */                                                                                                                                                                                                                                                      
// 非阻塞返回值是-1的，还需要根据错误码判断，    
    
       std::cout<< "s:" << s << " erron:"<< errno <<std::endl;    
    
// 1.没有错误的,按照错误的方式返回，需要根据错误码进行判读的    
       if(errno == EAGAIN || errno == EWOULDBLOCK)
       {
         std::cout<< "我没有错，只是没有数据而已" << std::endl;
         EXEC_OTHER(cbs);
       }
// 2.需要重新读取数据的,读取被中断了，需要继续读取数据的。
       else if(errno == EINTR)
       {
          continue;
       }
// 3.真正的错误了。
       else 
       {
        std::cout<< "s:" << s << " erron:"<< errno <<std::endl; // 真正的错误了
        break;
       }
      sleep(3);
    }
  }
  return 0;
}
```

```c++
#pragma once     
    
#include <iostream>    
#include <unistd.h>    
#include <fcntl.h>    
#include <cstring>    
#include <cerrno>    
    
void setNoBlock(int fd)    
{    
//1.先获取文件描述符的状态标志    
  int f1 = fcntl(fd, F_GETFL);    
  if(f1 < 0)    
  {    
    std::cerr << "fcntl:" << strerror(errno) << std::endl;    
    return;    
  }    
    
//2.通过按位或追加一个标志    
  fcntl(fd, F_SETFL, f1 | O_NONBLOCK);    
    
//3.严谨一点的这里可以判断返回的信息    
}    
    
void printLog()                                                                                                                                                                                                                                                
{    
  std::cout<< "this is a log" << std::endl;    
}    
    
void download()    
{    
  std::cout<< "this is a download" << std::endl;    
}    
    
void executeSql()    
{    
  std::cout<< "this is a executeSql" << std::endl;    
} 
```

**非阻塞IO，读取的返回值，还需结合错误码进行判断的。**

**EAGAIN  EWOULDBLOCK : 阻塞了，本次没有数据而已**

**ENTER: 数据可能被信号中断了，还需进行再次的读取。缓冲区还有数据的。**

**其它情况就是错了的。**



## Select

**IO = 等 + 拷贝**

**select只是负责等待，可以一次等待多个fd, select本身没有拷贝的能力，拷贝需要read, write来完成的。**

```c++
int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);
// 参数1
// nfds:监视多个fd中，最大fd的值加一

// 参数234
// 剩下的四个参数都是输入输出型参数
// struct timeval* timeout
    // 	nullptr:阻塞等待。
	// 	struct timeval timeout = {0,0} 非阻塞
	// 	struct timeval timeout = {5,0} 5s以内阻塞式(5s以内都可以返回的)，超过5s，非阻塞返回一次
	//  还可以定制 多少时间以内进行返回的。多少时间内阻塞。

// 参数5
    //  struct timeval
    // { 
    //  time_t tv_sec;
    //  suseconds_t tv_sec; 
    //}

// 返回值 > 0 有几个文件描述符准备就绪了
// 返回值 ==0 超时返回 本次没有检测到就绪的
// 返回值 < 0 调用失败

// 参数234
// 只关心读，写，异常事件
// fd_set位图结构，表示文件描述集合
// fd_set:结构体套数组

// 文件描述符就是数组的下标
// 输入：表示用户告诉内核，你要帮我关系一下，我给你的集合中所有的fd的读事件. 哪些fd上的读事件内核需要你关系一下。
// 0000 0000：比特位的位置 表示fd的数值，比特位的内容，表示是否关关心。

// 0010 0010:只是关系1和5的读事件
// 输出：内核告诉用户，你所关系的多个fd中，哪些已经就绪了 0000 0010. 1号文件描述符已经好了的

// 输入输出型  位图结构
// 比特位的位置，表示fd的数值，比特位的内容，表示哪些fd上面对应的事件已经就绪了。
// 让用户和内核之间相互沟通，互相知晓对方要的和关心的。

// 内核和用户相互 交互
// 读，写，异常事件类似的。

// 四个函数操作 文件描述符
void FD_CLR(int fd, fd_set *set);  // 清除
int  FD_ISSET(int fd, fd_set *set); // 判断在不在
void FD_SET(int fd, fd_set *set); // 设置进位图
void FD_ZERO(fd_set *set);       // 清空位图结构的
```

**1.了解select基本概念和接口介绍**

​	**select直接监视多个文件描述符，程序会在select等待，直到监视的文件描述符返回**

​	**select未来只关心：读，写，异常事件。**

**2.代码**

​	**listen套接字首先交给select, listensock的连接就绪事件 == 读事件就绪的。**

**3.总结**

![image-20260331223752895](picture/image-20260331223752895.png)

**1.select能够同时等待的文件fd是有上限的，除非重新修改内核，否则无法解决的。 fd_set是一个位图结构，有类型就是有大小的。**

​	**企业也是有上限的。 select上限1024的。**

​	**内核设置是： 65535个文件的**

**2.必须借助第三方数组，来维护合法的fd**

**3.select大部分是输入输出型参数，调用select前，需要重写设置所有的fd,调用之后，我们还需要检查所有的fd,者带来了的遍历的成本--用户**

**4.select为什么第一个参数是fd + 1呢？  遍历这个数组的 [0, fd] 查一遍给你的。 确定遍历范围--内核层面。**

**5.select采用位图， 用户--内核  内核--用户。 来回进行拷贝的，拷贝的成本的问题。输入输出型参数的问题。**



## poll

**1.解决select的fd有上限的问题**

**2.每次调用都需要重新设置关心的fd.**

```c++
int poll(struct pollfd *fds, nfds_t nfds, int timeout);
// struct pollfd*
// 动态数组， new/malloc出来的

// 数组长度的

// timeout 单位毫秒，纯输入型的
// > 在timeout时间内返回，否则 非阻塞返回一次的。
// = 非阻塞
// < 阻塞等待
// 返回值同select

struct pollfd 
{
    int fd;
    short events;
    short revents;
};

// 输入看：fd + events。 请求需要关系哪些事件的
// 输出看：fd + revents. 你要关系的fd上面的events中哪些事件已经就绪啦
// 1.输入输出参数分离了. poll不需要重新设置的
	
// 2.解决select等待文件有上限的。
```



**event的事件。宏值，设置进内核的。**

**一个宏，一个比特位。**

**POLLINT POLLOUT : 读写事件的就绪。**

![image-20260401103845412](picture/image-20260401103845412.png)

**poll的缺点：遍历问题的。**



## epoll

**extend poll。增强版本的poll。和poll没有半毛钱的关心的。**

**1.直接快速认识poll的接口**

````c++
 int epoll_create(int size);
// int size   >0 就行了
// 
````

```c++
int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
// epfd
// op增，改，删除
//   EPOLL_ADD
//   EPOLL_MOD
//   EPOLL_DEL
// fd文件描述符的event事件
```

**事件**

![image-20260401152424078](picture/image-20260401152424078.png)

**事件**

![image-20260401152505532](picture/image-20260401152505532.png)

```c++
int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout);
// 返回值，select/poll一模一样的
// 2,3输出型参数,内核告诉用户，哪些文件描述符已经准备就绪了
// ms,同poll
```

**2.epoll底层的原理**

**OS怎么知道网络中有数据到来？ 输入设备网卡。  **

​	**硬件中断设备。 中断向量表(函数指针)。 **

![image-20260401154616251](picture/image-20260401154616251.png)



![image-20260401161330493](picture/image-20260401161330493.png)

![image-20260401160940339](picture/image-20260401160940339.png)

![image-20260401160857052](picture/image-20260401160857052.png)

![image-20260401160404714](picture/image-20260401160404714.png)

![image-20260401160428902](picture/image-20260401160428902.png)

**给每一个文件描述符注册一个回调方法，然后把红黑树的节点 到  对应的序列当中去。**

**两个数据结构。红黑树，就绪队列的。**



**3.只关心读取的epoll的server**



**4什么是事件就绪的**

**底层的  IO  条件就绪了，可以进行某种 IO行为了的，就是时间就绪的。**

**select poll epoll  等  IO就绪事件的通知机制**

**通知机制，有没有策略呢？**

**LT  ET事件。**



**张三：大哥下来没得哦，第三次了哦？   效率低   LT  水平触发**

**李四：要不要？我只给你一次。            效率高  ET  边缘触发**

**打电话，通知用户事件的到来的。**



**水平触发：只要底层有数据没读完，epoll会一直通知用户读数据 LT。没有读完不删除你的节点信息，所以会一直通知你的。**

**边缘触发：只要底层有数据没读完，epoll不再通知用户的，除非底层数据变化的时候（再次增加）才会通知你一次的。**



**默认是LT, 水平触发的。**



**ET模式。EPOLLET模式**

**数据从无到有，从有到多时候，才会 通知你一次。倒逼程序员将本轮的数据全部读完。**

**你怎么知道数据读取完毕呢？ 一次就绪就读取完毕了呢？**

**循环登场，一直读取，读完。**

**ET---模式 文件描述符是非阻塞的。**



**LT,可以是阻塞，可以是非阻塞的。**

**LT，可以模仿ET。**



**ET不仅仅体现在通知机制上**

**尽量让上层把数据取走--TCP--->给对方一个更大的缓冲区，让对方更新更大的窗口。 提供底层的数据发送效率，更好适配TCP。**

**TCP当中催促对方尽快读数据 push。 让底层数据就绪，再让上层知道。**



## Reactor

**基于ET模式下的Reactor, 处理所有的IO。**

**半同步 半异步 LINUX网络中， 最常用， 最频繁的一种网络IO设计模式。**



**以前代码的问题：本轮的数据如何读完了？ 读完了是一个完整的请求吗？ 每一个套接字对应一个缓冲区。然后根据上层协议判断是否是一个完整的报文信息的。**

**“我不主动傻等某个 socket，而是把所有 socket 交给 epoll 统一监听；哪个 socket 就绪了，我就调用它对应的处理函数。”**



**ET:就绪事件只会通知一次，文件描述符号设置非阻塞模式的。**

**reactor模式**

![image-20260407212339768](picture/image-20260407212339768.png)



**Reactor模式：保证事件就绪了，**

![image-20260407212535805](picture/image-20260407212535805.png)

**前摄式，**

**Preactor模式**



**师傅您可太强了啊：**

















