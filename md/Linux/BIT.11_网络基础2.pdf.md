# BIT.11_网络基础2.pdf

## 1初始三次握手四次挥手

**TCP**

![image-20251218203906802](picture/image-20251218203906802.png)

**listen和accept 套接字**

**connnet 发起三次握手  建立是双方的OS自动完成的。 发起然后建立。**

**accept只是只是负责获取。**

**然后，双方read和write。**

**四次挥手，曾经先先建立。然后才断开链接。**



**建立链接一定是先记住一些什么东西。**

**三次握手是手段，建立链接是目的。**



**建立链接需要先描述，然后组织起来的。**

**链接OS完成的，需要花时间的。断开链接四次挥手。**

**建立链接双方协商，断开链接双方协商。**



**Uniform Resource Locator**

**URL**

**（统一资源定位符）**



**HTTP = HyperText Transfer Protocol**

**（超文本传输协议）**



## 2协议

**再谈协议**

**什么是协议：约定**



**传输一个结构化的数据呢**

**头像url,时间，昵称，消息---》打包变成一个报文，方便我们就那些整体发送。**

**多变一：序列化的过程。**



**网络发送**

**收到一个报文，反序列化**

**头像url,时间，昵称，消息**

**结论：业务结构数据在发送到网络中的时候，先序列化在发送，**

**收到的一定是序列字节流，要先进行反序列化，然后才能使用**



```c++
结构化数据进行建模 业务协议
class/struct message
{
  string url;
  string time;
  string nickname;
  string
  msg;
};
```

**TCP这里，你怎么保存你收到一个完整的报文？**

**协议**

![image-20251219093344816](picture/image-20251219093344816.png)



**我们自己手写一个**

**TCP是面向字节流的，明确报文和报文的边界。**

**TCP有自己的发送缓冲区，接收缓冲区。**



**应用层 (应用层缓冲区)**

**TCP层 (之间就是套接字)**

**我们调用的所有的发送函数，根本就不是把数据发送到网络中！**

**发送函数，本质是拷贝函数！(TCP协议自己决定什么时候发送)**

**接收函数，本质是拷贝函数！(TCP协议自己决定什么时候接收)**

**s-c:TCP发送的本质, 其实就是将数据从s的发送缓冲区，拷贝到c的接收缓冲区。**

**c-s:TCP发送的本质, 其实就是将数据从c的发送缓冲区，拷贝到s的接收缓冲区。**

**所以TCP是全双工的。(接收和发送的缓冲区分开的)**

**来不及缓冲区太多的数据了。如何解决**

**1.定长**

**2.特殊符号**

**3.自描述方式 [4字节特殊符号，数据的长度][真实数据]**

![image-20251219100137376](picture/image-20251219100137376.png)





**![image-20251219100155415](picture/image-20251219100155415.png)**





## 3复习

### socket

```c++
int socket(int domain, int type, int protocol);
```

- **返回值**：
  - 成功：返回一个非负整数的 socket 文件描述符（sockfd）。
  - 失败：返回 -1，并设置 errno（常见错误如 EAFNOSUPPORT、EPROTONOSUPPORT 等）。

每个参数的详细解释

1. **domain**（也叫 family 或 af）：
   - 指定通信域（协议族），决定 socket 使用哪种地址格式和底层协议栈。
   - 常见值：
     - **AF_INET**：IPv4 互联网协议（最常用，支持 TCP/UDP 等）。
     - **AF_INET6**：IPv6 互联网协议。
   - 这个参数决定了后续地址结构（如 struct sockaddr_in 对于 AF_INET）。
2. **type**（socket 类型）：
   - **SOCK_STREAM**：流式套接字，提供可靠的、面向连接的双向字节流（通常对应 TCP，顺序、无重复、无边界）。
   - **SOCK_DGRAM**：数据报套接字，提供无连接的、不可靠的消息传输（通常对应 UDP，保留消息边界，可能丢失或乱序）。

- 创建 TCP socket（IPv4）：
  ```c
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  ```

- 创建 UDP socket（IPv4）：
  ```c
  int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  ```

- 创建原始 ICMP socket：
  ```c
  int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
  ```



### bind

```c
int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
```

**作用**：  
将一个本地地址（IP + 端口）绑定到已创建的 socket 上。通常服务器端使用，让 socket 监听特定地址和端口；客户端也可绑定（可选，常用于指定源地址）。

- 成功返回 0，失败返回 -1 并设置 errno（常见：EADDRINUSE 端口已被占用）。

**参数说明**：

1. **sockfd**：  
   socket() 返回的 socket 文件描述符。

2. **addr**：  
   指向地址结构的指针，类型是 **struct sockaddr***（通用地址结构）。

3. **addrlen**：  
   addr 指向结构的长度，通常用 `sizeof(struct sockaddr_in)` 或 `sizeof(struct sockaddr_in6)`。

### 为什么需要强制类型转换？

bind() 的第二个参数是 **const struct sockaddr ***，这是一个**通用**的地址结构指针，设计用于兼容各种协议族（AF_INET、AF_INET6、AF_UNIX 等）。

但实际使用的地址结构是**具体**的：
- IPv4：`struct sockaddr_in`
- IPv6：`struct sockaddr_in6`
- Unix域：`struct sockaddr_un`

这些具体结构的前几个字节与 `struct sockaddr` 布局相同（有 sa_family 字段），但大小和内容不同。

**示例（IPv4）**：
```c
struct sockaddr_in serv_addr;
serv_addr.sin_family = AF_INET;
serv_addr.sin_port = htons(8080);
serv_addr.sin_addr.s_addr = INADDR_ANY;  // 0.0.0.0

bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
//                 ^^^^^^^^^^^^^^^^^^^^^^^ 强制转换
```

**强制转换的原因**：
- C语言类型检查严格，`struct sockaddr_in *` 不能直接赋值给 `struct sockaddr *`（即使内存布局兼容）。
- 系统调用（如 bind）只接受统一的 `struct sockaddr *`，所以必须**显式强制转换**告诉编译器：“我知道它们兼容，请允许传递”。

**不转换会报错**：
```c
bind(sockfd, &serv_addr, sizeof(serv_addr));  // 编译错误：类型不匹配
```

**总结一句话**：  
bind 用通用接口 struct sockaddr* 来支持多种地址类型，而我们实际用的是具体类型（如 sockaddr_in），所以必须强制转换来满足接口要求。这是 BSD Socket API 的经典设计（历史原因），现代 API（如 Windows 的 WSASocket）也有类似机制。

常见写法记住这句就行：
```c
bind(sockfd, (struct sockaddr *)&my_addr, sizeof(my_addr));
```

















































































