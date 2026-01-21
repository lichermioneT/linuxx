# SOCKET编程

# TCP 和 UDP 发送接收函数总结对比

## 一、核心函数概览

| 协议    | 发送函数             | 接收函数            | 是否需要地址参数         | 连接要求           |
| ------- | -------------------- | ------------------- | ------------------------ | ------------------ |
| **TCP** | `send()` / `write()` | `recv()` / `read()` | **不需要**（建立连接后） | 必须先建立连接     |
| **UDP** | `sendto()`           | `recvfrom()`        | **必须提供**             | 无需连接，直接发送 |

## 二、函数详细说明

### 1. TCP 相关函数

#### 发送函数
```c
// 主要发送函数
ssize_t send(int sockfd, const void *buf, size_t len, int flags);

// 等价函数（常用于已连接套接字）
ssize_t write(int sockfd, const void *buf, size_t len);
```

#### 接收函数
```c
// 主要接收函数
ssize_t recv(int sockfd, void *buf, size_t len, int flags);

// 等价函数
ssize_t read(int sockfd, void *buf, size_t len);
```

#### TCP 使用流程
```c
// 1. 客户端连接服务器
connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));

// 2. 发送数据（无需地址）
send(sockfd, "Hello", 5, 0);

// 3. 接收数据（无需地址）
char buffer[1024];
recv(sockfd, buffer, sizeof(buffer), 0);
```

### 2. UDP 相关函数

#### 发送函数
```c
// 主要发送函数（必须指定目标地址）
ssize_t sendto(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr, socklen_t addrlen);
```

#### 接收函数
```c
// 主要接收函数（可获取发送方地址）
ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags, struct sockaddr *src_addr, socklen_t *addrlen);
```

#### UDP 使用流程
```c
// 1. 发送数据（必须指定目标地址）
struct sockaddr_in server_addr;
// 填充 server_addr 信息...
sendto(sockfd, "Hello", 5, 0,  (struct sockaddr*)&server_addr, sizeof(server_addr));

// 2. 接收数据（可获取发送方地址）
struct sockaddr_in client_addr;
socklen_t addr_len = sizeof(client_addr);
char buffer[1024];
recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&client_addr, &addr_len);
```

## 三、关键差异总结

### 1. **连接模式**
- **TCP**：面向连接，先建立连接，然后通信
- **UDP**：无连接，直接发送数据报

### 2. **地址处理**
- **TCP**：
  - 只在 `connect()`（客户端）或 `accept()`（服务器）时指定地址
  - 建立连接后，所有通信都使用该连接，无需再指定地址
- **UDP**：
  - 每次 `sendto()` 必须指定目标地址
  - 每次 `recvfrom()` 可获得发送方地址

### 3. **套接字管理**
- **TCP**：
  - 服务器为每个客户端创建独立的套接字
  - 每个连接是独立的双向通信通道
- **UDP**：
  - 单个套接字可与多个客户端通信
  - 需要自己管理客户端地址信息

## 四、特殊用法

### 1. UDP 的 "伪连接"
```c
// UDP 也可以使用 connect()，但意义不同
connect(udp_sock, (struct sockaddr*)&addr, sizeof(addr));
// 之后可使用 send()/recv()，但仍然是 UDP 特性
send(udp_sock, data, len, 0);    // 只能发送到该地址
recv(udp_sock, buffer, len, 0);  // 只接收来自该地址的数据
```

### 2. TCP 获取对端信息
```c
// 获取已连接套接字的对端地址
struct sockaddr_in peer_addr;
socklen_t addr_len = sizeof(peer_addr);
getpeername(sockfd, (struct sockaddr*)&peer_addr, &addr_len);
```

## 五、选择建议

| 场景                       | 推荐协议 | 原因                   |
| -------------------------- | -------- | ---------------------- |
| 需要可靠传输（如文件传输） | TCP      | 保证数据完整性和顺序   |
| 实时性要求高（如视频流）   | UDP      | 延迟低，可容忍少量丢包 |
| 简单请求/响应              | UDP      | 无需维护连接状态       |
| 长时间数据交换             | TCP      | 连接管理更简单         |
| 多播/广播                  | UDP      | TCP 不支持多播         |

## 六、记忆口诀

**TCP：先握手，再通话，地址只记第一次**
- 连接建立后，`send()`/`recv()` 走天下

**UDP：无连接，自由身，每次都要说清楚**
- 谁发给谁？`sendto()`/`recvfrom()` 要地址

## 七、实战示例对比

```c
// TCP 服务器处理客户端
int client_sock = accept(server_sock, &client_addr, &addr_len);
// 后续通信只需 client_sock
recv(client_sock, buffer, size, 0);
send(client_sock, response, resp_len, 0);

// UDP 服务器处理客户端
struct sockaddr_in client_addr;
socklen_t addr_len = sizeof(client_addr);
recvfrom(server_sock, buffer, size, 0,  (struct sockaddr*)&client_addr, &addr_len);
// 回复时需要客户端地址
sendto(server_sock, response, resp_len, 0, (struct sockaddr*)&client_addr, addr_len);
```

## 总结表格

| 特性         | TCP                | UDP               |
| ------------ | ------------------ | ----------------- |
| **连接性**   | 面向连接           | 无连接            |
| **可靠性**   | 可靠传输           | 不可靠传输        |
| **发送函数** | `send()`/`write()` | `sendto()`        |
| **接收函数** | `recv()`/`read()`  | `recvfrom()`      |
| **地址参数** | 不需要（连接后）   | 必须提供          |
| **典型应用** | HTTP、FTP、SSH     | DNS、视频流、游戏 |

记住核心区别：**TCP 建立连接后固定通信对象，UDP 每次通信都要明确目标**。
