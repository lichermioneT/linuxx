# select

这段代码本质上是在实现一个 **基于 `select` 的多路复用 TCP 服务器**。

它现在已经完成了最核心的一步：

**能够监听新连接，并把新连接的 socket 交给 `select` 托管。**

但是它**还没有完成客户端数据读写处理**，也就是说：

- 现在能 `accept`
- 但还不能对普通客户端 socket 做 `recv`
- 也没有做连接关闭后的清理

所以这份代码更准确地说，是一个 **“select 版本服务器的半成品框架”**。

------

# 1. 先看这个类整体想干什么

你的 `select_server` 的设计思路是这样的：

1. 先创建监听套接字 `_listensock`
2. 用一个数组 `fdarray` 维护“所有合法 fd”
3. 每次循环都把 `fdarray` 里的 fd 重新加入 `fd_set`
4. 调用 `select`
5. `select` 返回后，检查哪些 fd 就绪了
6. 如果监听 socket 就绪，说明有新连接来了，就 `accept`
7. 把新连接 socket 放进 `fdarray`
8. 下一轮 `select` 就能继续监视这个新 socket

这就是 `select` 服务器最经典的写法。

------

# 2. 代码最上面的常量是什么意思

```cpp
static const uint16_t defaultport = 8080;
static const int fdnum = sizeof(fd_set)*8;
static const int defaultfd = -1;
```

## `defaultport = 8080`

默认监听端口。

如果你创建对象时不传端口：

```cpp
select_server svr;
```

它就监听 8080。

------

## `fdnum = sizeof(fd_set) * 8`

这个意思是：

- `fd_set` 本质上是一个“位图结构”
- 一个 bit 表示一个 fd 是否被监视
- `sizeof(fd_set)` 是字节数
- 乘 8 就是 bit 数，也就是最多能表示多少个 fd

在 Linux 下通常是 1024。

所以这里的含义是：

**select 一次最多能管理这么多个 fd**

这个写法在你的实验环境里一般能跑通，但更常见、可读性更好的写法是：

```cpp
static const int fdnum = FD_SETSIZE;
```

因为 `FD_SETSIZE` 就是系统定义的 `select` 能监视的最大 fd 数。

------

## `defaultfd = -1`

表示这个位置当前没有存合法 socket。

也就是 `fdarray` 里空位置的占位符。

------

# 3. 成员变量是什么意思

```cpp
uint16_t _port;
int _listensock;
int *fdarray;
```

## `_port`

服务器监听的端口。

------

## `_listensock`

监听 socket。

它的职责不是和客户端收发普通数据，而是专门负责：

- 接收新连接请求
- 调用 `accept`

你可以把它理解成“前台接待”。

------

## `fdarray`

这是这份代码最关键的东西。

它是程序员自己维护的一个“合法 fd 表”。

因为 `select` 有一个特点：

> 每次调用前，你都要把所有想监视的 fd 重新放进 `fd_set`

但是 `select` 调用后，`fd_set` 会被内核修改，只保留“就绪”的那些 fd。
所以你不能指望 `fd_set` 永久保存全部 fd。

因此必须自己维护一个数组：

```cpp
fdarray
```

它记录：

- 监听 fd
- 所有已经连接上的客户端 fd

然后每一轮循环：

- 从 `fdarray` 把所有合法 fd 放回 `rfds`
- 再调用 `select`

这就是你注释里说的：

> 一般而言，使用select，需要程序员自己维护一个保存所有合法fd的数组

------

# 4. 构造函数在干什么

```cpp
select_server(uint16_t port = defaultport)
  :_port(port)
  ,_listensock(-1)
  ,fdarray(nullptr)
{}
```

就是初始化：

- 端口号
- 监听 socket 先设为无效值
- `fdarray` 先设为空

这里只是对象创建，还没有真正创建 socket。

------

# 5. `init()` 的逻辑

这个函数是“服务器启动前准备”。

```cpp
void init()
{
    _listensock = Sock::Socket();
    Sock::Bind(_listensock, _port);
    Sock::Listen(_listensock);
```

这三步就是 TCP 服务器标准三件套：

## 第一步：创建 socket

```cpp
_listensock = Sock::Socket();
```

底层大概率就是：

```cpp
socket(AF_INET, SOCK_STREAM, 0)
```

得到一个监听 fd。

------

## 第二步：绑定端口

```cpp
Sock::Bind(_listensock, _port);
```

把这个 socket 和本机某个端口绑定起来。

------

## 第三步：开始监听

```cpp
Sock::Listen(_listensock);
```

告诉内核：

> 这个 socket 现在要变成监听 socket，等待客户端发起连接。

------

## 第四步：申请 `fdarray`

```cpp
fdarray = new int[fdnum];
for(int i = 0; i < fdnum; ++i) fdarray[i] = defaultfd;
```

这里做了两件事：

1. 开一个数组，大小是 `fdnum`
2. 全部初始化成 `-1`

含义就是：

- 现在数组里每个位置都为空

------

## 第五步：把监听 fd 放进去

```cpp
fdarray[0] = _listensock;
```

这一步非常重要。

表示：

- `fdarray[0]` 永远留给监听 socket
- 后面的 `fdarray[1] ~ fdarray[fdnum-1]` 才放客户端 socket

所以这里有一个默认约定：

> 下标 0 是监听 fd 的固定位置

这会让后面的逻辑更清晰。

------

# 6. `print()` 是干什么的

```cpp
void print()
{
    for(int i = 0; i < fdnum; ++i)
    {
        if(fdarray[i] != defaultfd)
            std::cout << "fd list:" << fdarray[i] << std::endl;
    }
}
```

它只是把当前 `fdarray` 中所有有效 fd 打印出来。

比如：

- 监听 fd 是 3
- 新连上来两个客户端 fd 是 4 和 5

那么输出可能是：

```cpp
fd list:3
fd list:4
fd list:5
```

这相当于调试用，方便你看当前有哪些连接被 `select` 托管了。

------

# 7. `start()` 是核心：事件循环

这个函数是整个服务器的“大脑”。

------

## 外层死循环

```cpp
for(;;)
```

服务器要一直工作，所以它会不断循环。

------

## 每次循环先定义一个 `fd_set`

```cpp
fd_set rfds;
FD_ZERO(&rfds);
```

这里的 `rfds` 就是“读事件集合”。

意思是：

> 我这一轮只关心哪些 fd 可读

`FD_ZERO` 就是先清空集合。

------

## 重新把 `fdarray` 中的 fd 加进去

```cpp
int maxfd = fdarray[0];

for(int i = 0; i < fdnum; ++i)
{
    if(fdarray[i] == defaultfd)
        continue;
    FD_SET(fdarray[i], &rfds);

    if(maxfd < fdarray[i]) maxfd = fdarray[i];
}
```

这里非常关键。

因为 `select` 要求你传进去：

- 哪些 fd 需要检查
- 最大 fd 是多少

所以这里遍历 `fdarray`：

### 1）过滤掉无效项

```cpp
if(fdarray[i] == defaultfd)
    continue;
```

`-1` 不是合法 fd，跳过。

------

### 2）把合法 fd 放到 `rfds`

```cpp
FD_SET(fdarray[i], &rfds);
```

这表示：

> 我希望 `select` 帮我检查这个 fd 是否可读

------

### 3）维护最大 fd

```cpp
if(maxfd < fdarray[i]) maxfd = fdarray[i];
```

因为 `select` 第一个参数是：

```cpp
maxfd + 1
```

不是“fd 的个数”，而是“最大 fd + 1”。

这是 `select` 的历史设计。

------

# 8. `select()` 调用是什么意思

```cpp
int n = select(maxfd + 1, &rfds, nullptr, nullptr, nullptr);
```

含义是：

- 检查 `rfds` 里的这些 fd
- 看哪些 fd 已经“读就绪”
- 写事件不关心，所以传 `nullptr`
- 异常事件不关心，所以传 `nullptr`
- 超时时间也不设置，所以传 `nullptr`

这意味着：

> **阻塞等待，直到至少有一个 fd 可读**

------

## 返回值 `n` 的含义

### `n == 0`

超时

但你这里传的是 `nullptr`，没有设置超时，所以理论上这里不会返回 0。

也就是说：

```cpp
case 0:
    std::cout<< "timeout...." << std::endl;
    break;
```

这一支在当前代码里其实走不到。

只有你把下面这个打开才有意义：

```cpp
struct timeval timeout = {3,0};
select(maxfd + 1, &rfds, nullptr, nullptr, &timeout);
```

那时候才可能 3 秒超时返回 0。

------

### `n == -1`

出错

```cpp
case -1:
    std::cout<< "select error:" << errno << strerror(errno) << std::endl;
    break;
```

表示 `select` 调用失败。

------

### `n > 0`

说明有 fd 就绪了

```cpp
default:
    handlerEvent(rfds);
    std::cout<< "get a new link" << std::endl;
    break;
```

此时 `rfds` 里面保留的是“已经可读”的那些 fd。

注意：

**调用完 `select` 后，`rfds` 已经不是原来的“待检测集合”，而是“结果集合”。**

------

# 9. `handlerEvent(rfds)` 的逻辑

这是当前代码里真正处理事件的地方。

但你现在只处理了一个事件：

> 监听 socket 可读

------

## 先检查监听 fd 是否就绪

```cpp
if(FD_ISSET(_listensock, &rfds))
```

这句话意思是：

> `_listensock` 这个 fd 是否在本轮 `select` 结果中可读？

对于监听 socket 来说，“可读”不表示有普通数据，而表示：

> **有新的连接到达了，可以 accept 了**

------

## 然后调用 `accept`

```cpp
std::string clientip;
uint16_t clientport = 0;

int sock = Sock::Accept(_listensock, &clientip, &clientport);
if(sock < 0) return;
```

这一步是从内核的“已完成连接队列”里取出一个新连接。

返回值 `sock` 就是：

> 和这个客户端通信用的“已连接 socket”

注意区分：

- `_listensock`：监听 socket，只负责接客
- `sock`：已连接 socket，负责和某个具体客户端通信

这是 TCP 服务器必须分清的两个角色。

------

## 为什么这里 `accept` 不会阻塞？

因为前面已经被 `select` 告诉你：

```cpp
_listensock 可读
```

对于监听 socket，这意味着：

> 当前至少有一个连接已经准备好了

所以这时调用 `accept` 是安全的，一般不会阻塞。

这也是你注释里说的：

> 走到这里，accept不会阻塞的

这是对的。

------

## 打印新连接信息

```cpp
std::cout<< "新链接获取成功：ip:" << clientip.c_str() << "port" << clientport << std::endl;
```

表示新客户端连接上来了。

------

## 为什么这里不能直接 `recv`？

你代码里这一句注释很关键：

```cpp
// 这里能够直接recv/read吗？ 不能的。整个代码，只有select有资格检查事件是否就绪的。
```

这个思路基本正确。

更准确地说：

- 你可以直接对新 socket `recv`
- 但**不能保证此时一定有数据**
- 如果这个 socket 是阻塞的，直接 `recv` 可能阻塞住整个服务器

因为：

- 监听 socket 可读，只说明“连接建立好了”
- 不说明“客户端已经发送业务数据了”

所以正确做法是：

1. 新连接 `accept`
2. 放到 `fdarray`
3. 下一轮 `select` 再帮你检查这个客户端 socket 是否可读
4. 真正可读时再 `recv`

这就是“事件驱动”的思想。

------

## 把新连接加入 `fdarray`

```cpp
int i = 0;
for(i = 0; i < fdnum; ++i)
{
    if(fdarray[i] != defaultfd) continue;
    else break;
}
```

这里是在找数组里第一个空位。

逻辑是：

- 遇到不是 `-1` 的位置，说明已经被占用了，继续找
- 遇到 `-1`，说明这个位置空着，可以放新 socket

------

## 如果满了怎么办

```cpp
if(i == fdnum)
{
    std::cout<< "服务器已经承受住了，已经满了" << std::endl;
    close(sock);
}
```

说明数组里没有空位了。

也就是说：

- `select` 当前托管的 fd 数量已经到上限了
- 没法再接纳这个客户端

所以关闭这个新连接。

这个处理思路是对的。

------

## 否则加入数组

```cpp
else
{
    fdarray[i] = sock;
}
```

这一步非常核心：

> 把新连接正式交给 `select` 托管

下一轮 `start()` 再次遍历 `fdarray` 时，这个新 `sock` 就会被 `FD_SET` 加到 `rfds` 中。

于是以后客户端发数据时，`select` 就能检测到它的读事件。

------

## 最后打印当前 fd 列表

```cpp
print();
```

方便调试。

------

# 10. 这份代码现在“已经做到的事情”

当前代码已经实现了：

## 第一阶段：监听和接收连接

- 创建监听 socket
- bind
- listen
- 使用 `select` 检测监听 fd
- 有新连接时 `accept`
- 把新连接放进 `fdarray`

也就是说：

**已经实现了“连接管理”的第一步。**

------

# 11. 这份代码还没做到的事情

这是你现在最需要看懂的地方。

当前代码最大的问题是：

## 只处理了监听 fd，没有处理客户端 fd

在 `handlerEvent(rfds)` 里，你只有：

```cpp
if(FD_ISSET(_listensock, &rfds))
```

也就是说，你只判断：

- 监听 socket 是否可读

但是对于已经连接上的客户端 socket，比如：

- `fdarray[1]`
- `fdarray[2]`
- `fdarray[3]`

你根本没有检查：

```cpp
FD_ISSET(fdarray[i], &rfds)
```

所以就算客户端发来数据，`select` 已经告诉你“这个客户端 socket 可读了”，你也没处理。

------

# 12. 正确的事件处理应该是什么样

你现在应该把 `handlerEvent()` 改成“两类事件都处理”：

## 第一类：监听 socket 可读

说明有新连接，执行 `accept`

## 第二类：普通客户端 socket 可读

说明客户端发数据了，执行 `recv`

------

## 逻辑应该像这样

### 先处理监听 fd

```cpp
if(FD_ISSET(_listensock, &rfds))
{
    // accept 新连接
}
```

### 再遍历所有客户端 fd

```cpp
for(int i = 1; i < fdnum; ++i)
{
    if(fdarray[i] == defaultfd) continue;

    if(FD_ISSET(fdarray[i], &rfds))
    {
        // 这个客户端可读，recv
    }
}
```

注意从 `1` 开始，因为 `0` 号位是监听 fd。

------

# 13. 客户端 fd 可读时意味着什么

当普通客户端 socket 可读时，通常有两种情况：

## 情况 1：读到正常数据

说明客户端发来了请求

比如：

```cpp
ssize_t s = recv(fdarray[i], buffer, sizeof(buffer)-1, 0);
if(s > 0)
{
    buffer[s] = 0;
    std::cout << "client say: " << buffer << std::endl;
}
```

------

## 情况 2：`recv` 返回 0

说明对端关闭连接

```cpp
if(s == 0)
{
    close(fdarray[i]);
    fdarray[i] = defaultfd;
}
```

这里一定要记住：

> 客户端断开后，要把 `fdarray[i]` 重新置成 `-1`

不然下一轮你还会把一个失效 fd 放进 `FD_SET`，程序就会出问题。

------

## 情况 3：`recv` 返回负数

说明读出错

这时一般也要关闭连接并清理。

------

# 14. 用一个完整运行过程帮你理解

假设服务器启动后：

- 监听 fd = 3
- `fdarray[0] = 3`

此时数组状态：

```cpp
[3, -1, -1, -1, ...]
```

------

## 第一次 `select`

客户端 A 发起连接。

监听 socket 可读。

于是：

```cpp
accept(_listensock)
```

得到新 fd，例如 4。

把它放到数组里：

```cpp
[3, 4, -1, -1, ...]
```

------

## 第二次 `select`

客户端 B 又发起连接。

监听 socket 再次可读。

`accept` 得到 fd 5。

数组变成：

```cpp
[3, 4, 5, -1, ...]
```

------

## 第三次 `select`

这次不是新连接，而是客户端 A 发数据了。

那么 `select` 返回后：

- 监听 fd 3 不一定就绪
- 客户端 fd 4 可读

但你当前代码只检查：

```cpp
FD_ISSET(_listensock, &rfds)
```

所以会漏掉 fd 4 的事件。

这就是为什么说你现在代码只能“接连接”，还不能“处理客户端通信”。

------

# 15. 你代码里的几个细节问题

下面这些点你要特别注意。

------

## 问题 1：析构函数条件写反了

你现在写的是：

```cpp
~select_server()
{
    if(_listensock < 0) close(_listensock);
    if(fdarray) delete[] fdarray;
}
```

这里有问题。

如果 `_listensock < 0`，说明它本来就是无效 fd，不能 `close`。
应该写成：

```cpp
if(_listensock >= 0) close(_listensock);
```

正确版本：

```cpp
~select_server()
{
    if(_listensock >= 0) close(_listensock);
    if(fdarray) delete[] fdarray;
}
```

------

## 问题 2：`case 0` 当前不会进入

因为你 `select` 最后一个参数传的是：

```cpp
nullptr
```

这表示永久阻塞等待，没有超时。

所以：

```cpp
case 0:
```

理论上不会发生。

------

## 问题 3：`select error` 输出格式不太好

你写的是：

```cpp
std::cout<< "select error:" << errno << strerror(errno) << std::endl;
```

中间少个分隔。

更清晰些：

```cpp
std::cout << "select error: " << errno << " " << strerror(errno) << std::endl;
```

------

## 问题 4：`fdnum = sizeof(fd_set)*8` 能用，但建议写成 `FD_SETSIZE`

更标准：

```cpp
static const int fdnum = FD_SETSIZE;
```

因为它直接表达：

> select 的最大监视 fd 数

------

## 问题 5：只 accept 一个连接

如果监听 fd 就绪，内核里可能不止一个连接已经排队完成了。
你现在只 `accept` 一次。

这不是错，在 **LT 水平触发** 风格下还能继续工作，因为下轮 `select` 还会再次告诉你监听 fd 可读。
只是效率不算最高。

------

# 16. 为什么你的注释里说“只有 select 有资格检查事件是否就绪”

这句话的核心意思是：

> 在多路复用模型里，我们不要直接盲目对每个 socket 做阻塞式读写，而是统一由 `select` 先检查哪个 fd 就绪，再对那个 fd 操作。

这是非常重要的思想。

传统阻塞式服务器会这样：

- 对一个连接 `recv`
- 没数据就卡住
- 整个服务器就不能处理别的连接

而 `select` 模型是：

- 先一次性监视很多连接
- 哪个就绪就处理哪个
- 没就绪的不碰

所以服务器不会轻易被某一个 socket 卡死。

------

# 17. 这段代码在架构上的本质

你的代码其实已经体现了 `select` 服务器的三个核心角色：

## 1）监听 socket

负责接受新连接

## 2）`fdarray`

负责保存所有被托管的 fd

## 3）`select`

负责统一检测事件是否就绪

可以总结成一句话：

> `fdarray` 记录成员，`select` 负责点名，监听 fd 专门接新人，普通 fd 负责业务通信。

这个理解非常重要。

------

# 18. 你下一步应该补什么

如果你要把这份代码补完整，下一步就该做这三件事：

## 第一：在 `handlerEvent()` 中遍历普通客户端 fd

检查哪些客户端可读。

------

## 第二：对可读客户端执行 `recv`

- `>0`：处理数据
- `==0`：对端关闭
- `<0`：异常处理

------

## 第三：客户端关闭后清理 `fdarray`

```cpp
close(fdarray[i]);
fdarray[i] = defaultfd;
```

不清理的话，后续 `select` 会出问题。

------

# 19. 你这段代码可以这样理解成一句“流程话”

整个运行逻辑可以浓缩为：

**服务器先创建监听 socket，然后把监听 fd 放进 `fdarray`。每一轮循环都把 `fdarray` 中所有合法 fd 重新加入 `fd_set`，再调用 `select` 阻塞等待。若监听 fd 就绪，说明有新连接到来，调用 `accept` 获取新 socket，并把它加入 `fdarray`，使它以后也能被 `select` 监视。**

这就是你这段代码目前完整实现的逻辑。

------

# 20. 最后我给你一个最核心的结论

你这份代码的定位应该是：

## 已完成

- `select` 基本框架
- 监听 socket 管理
- 新连接接入
- 新连接加入 `fdarray`

## 未完成

- 普通客户端读事件处理
- 连接关闭处理
- 异常 fd 清理
- 真正的服务器业务逻辑

所以你现在不是“代码错了”，而是：

> **代码只写到了 select 服务器的第一阶段：连接接入层。**

------

你要的话，我下一条可以直接帮你把这份代码补成一个 **完整可通信的 select 回显服务器版本**，我会在你这个代码基础上继续改，并且逐段解释为什么这么改。