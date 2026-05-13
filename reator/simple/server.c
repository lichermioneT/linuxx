#include "server.h"
#include <ctype.h>
#include <dirent.h>
#include <sys/sendfile.h>
#include <assert.h>
#include <sys/stat.h>
#include <strings.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <pthread.h>

struct FdInfo
{
  int fd;
  int epfd;
  pthread_t tid;
};

int initListenFd(unsigned short port)
{
// 1.创建监听的fd
  int lfd = socket(AF_INET, SOCK_STREAM, 0);
  if(lfd == -1)
  {
    perror("socket");
    return -1;
  }

 //2.端口复用
  int opt = 1;
  int ret = setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
  if(ret == -1)
  {
    perror("setsockopt");
    return -1;
  }
  
// 3.bind函数
  struct sockaddr_in addr;
  bzero(&addr, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = htonl(INADDR_ANY);
  addr.sin_port = htons(port);               // 不建议写死了，函数参数传进来的

  ret = bind(lfd, (struct sockaddr*)&addr, sizeof(addr));
  if(ret == -1)
  {
    perror("bind");
    return -1;
  }

// 4.listen  全链接长度,存储已经握手成功的，没有被accept走的数据了。
  ret = listen(lfd, 128);
  if(ret == -1)
  {
    perror("listen");
    return -1;
  }

  return lfd;
}


int epollRun(int lfd)
{
// 1.创建实例, 以后操作都行这个句柄的。
  int epfd = epoll_create(1);
  if(epfd == -1)
  {
    perror("epoll_create");
    return -1;
  }

// 2.lfd上树
// struct epoll_event 
// {
//   uint32_t events,   // 事件类型 EPOLLIN
//   epoll_data_t data  // 用户数据，一般是fd或者指针。
//  }
//
  struct epoll_event ev;
  ev.data.fd = lfd;
  ev.events = EPOLLIN; // EPOLLIN关系它的读事件的。

  int ret = epoll_ctl(epfd,  EPOLL_CTL_ADD, lfd, &ev);
  if(ret == -1)
  {
    perror("epoll_ctl");
    return -1;
  }

// 3.检查
  struct epoll_event evs[1024];
  int size = sizeof(evs) / sizeof(struct epoll_event);

  while(1)
  {
    int num = epoll_wait(epfd, evs, size, -1); // -1 一直阻塞，
    for(int i = 0; i < num; ++i)
    {
      struct FdInfo* info = (struct FdInfo*)malloc(sizeof(struct FdInfo));
      int fd = evs[i].data.fd;
      info->epfd = epfd;
      info->fd  = fd;

      if(fd == lfd)
      {
        // 监听文件描述符，建立新的链接accept(); 内核告诉我们客户端已经来建立链接了的
        // 通过listenfd，把新来的文件描述符添加到epoll树上去的。 epoll树的文件描述符越来越多的
// 1.添加文件描述符
        // acceptClient(lfd, epfd);              
        pthread_create(&info->tid, NULL, acceptClient, info);
      }
      else 
      {
        // 主要是读数据的。服务器的任务。
// 2.已经在epoll树上文件，准备好了的，(lfd除外，它在上面的。)
        //recvHttpRequest(fd, epfd);
        pthread_create(&info->tid, NULL, recvHttpRequest, info);
      }
    }
  }

}

void* acceptClient(void* arg)
{
  struct FdInfo* info  = (struct FdInfo*)arg;
  // 1.建立链接
  int cfd = accept(info->fd, NULL, NULL);
  if(cfd == -1)
  {
    perror("accept");
    return NULL;
  }

  // 2.设置文件描述符的属性，非阻塞
  int flag = fcntl(cfd, F_GETFL);
  flag |= O_NONBLOCK;
  fcntl(cfd, F_SETFL, flag);

  // 3.添加到epoll模型里面去的
  struct epoll_event ev;
  ev.data.fd = cfd;
  ev.events = EPOLLIN | EPOLLET;
  int ret = epoll_ctl(info->epfd, EPOLL_CTL_ADD, cfd, &ev);
  if(ret == -1)
  {
    perror("epoll_ctl");
    return NULL;
  }

  printf("accept threadid: %ld", info->tid);
  free(info);

    return NULL;
}

void* recvHttpRequest(void* arg)
{
  struct FdInfo* info  = (struct FdInfo*)arg;
  printf("开始接收数据了...\n");

  int len = 0;
  int total = 0;
  char tmp[1024] = {0};
  char buffer[4096] = {0};

  while((len = recv(info->fd, tmp, sizeof tmp, 0)) > 0)
  {
    if((total + len < (int)sizeof buffer))
    {
       memcpy(buffer + total, tmp, len);
    }

    total += len;
  }

  printf("数据接收完毕\n");

  // 判断数据是否接收完毕的。
  if(len == -1 && errno == EAGAIN)
  {
    // 解析请求的
    char* pt = strstr(buffer, "\r\n");
    int reqLen = pt - buffer;
    buffer[reqLen] = '\0';
    parseRequestLine(buffer, info->fd);
  }
  else if(len == 0)
  {
    // 客户端已经断开链接
    epoll_ctl(info->epfd, EPOLL_CTL_DEL, info->fd, NULL);
    close(info->fd);
  }
  else 
  {
    perror("recv");
  }

  printf("recvMsg threadid: %ld", info->tid);
  free(info);
  return NULL;
}

int parseRequestLine(const char* line, int cfd)
{
  // 解析请求行 sscanf(); 
  // get /xxx/1.jpg http/1.1
  char method[12];
  char path[1024];

  sscanf(line, "%[^ ] %[^ ]", method, path);
  printf("method:%s, path:%s\n", method, path);

  if(strcasecmp(method, "get") != 0)
  {
    return -1;
  }
  
  decodeMsg(path, path);
  // 处理客户端请求的静态资源(目录或者文件)
  const char* file = NULL;
  if(strcmp(path, "/") == 0)
  {
    file = "./";
  }
  else 
  {
    file = path + 1; // 指针往后移动一个字符的
  }

  struct stat st;
  int ret = stat(file, &st);
  if(ret == -1)
  {
    // 文件不存在，回复404
    sendHeadMsg(cfd, 404, "Not Found", getFileType(".html"), -1);
    sendFile("404.html", cfd);
    close(cfd);

    return 0;
  }

  if(S_ISDIR(st.st_mode))
  {
    // 把这个目录中的内容发送给客户端
    /*
     *sendHeadMsg(cfd, 200, "OK", getFileType(".html"), -1);
     */
    sendHeadMsg(cfd, 200, "OK", getFileType(".html"), -1);
    sendDir(file, cfd);
    close(cfd);
  }
  else 
  {
    // 文件的内容发送给客户端的。
    sendHeadMsg(cfd, 200, "OK", getFileType(file), st.st_size);
    sendFile(file, cfd);
    close(cfd);
  }

  close(cfd);
  return 0;
}

int sendFile(const char* fileName, int cfd)
{
  //TCP面向连接的流式字节。读一部分，发一部分。
  int fd = open(fileName,  O_RDONLY); 
  assert(fd > 0);
  
#if 0
  while(1)
  {
    char buffer[1024];
    int len = read(fd, buffer, sizeof buffer);
    if(len > 0)
    {
      send(cfd,  buffer, len, 0);
      usleep(10); // 这非常重要的，接收和发送速度匹配的。 接收端
    }
    else if (len == 0)
    {
      break;
    }
    else 
    {
      perror("read");
    }
  }
#else 
  // 系统函数
  off_t offset = 0;
  off_t size = lseek(fd, 0, SEEK_END); // 
  lseek(fd, 0, SEEK_SET);

  while(offset < size)
  {
    ssize_t ret = sendfile(cfd, fd, &offset, size);
    printf("ret = %zd, offset = %ld\n", ret, (long)offset);
    
    if(ret == -1 && errno == EAGAIN)
    {
      printf("没有数据了....\n");
    }
  }

#endif
  close(fd);
  return 0;
}


int sendHeadMsg(int cfd, int status, const char* descr, const char* type, int length)
{
  char buf[1024] = {0};
// 1.状态行
  sprintf(buf, "HTTP/1.1 %d %s\r\n", status, descr);


// 2. 响应头
  sprintf(buf +  strlen(buf), "Content-Type: %s\r\n", type);
  if(length >= 0)
  {
    sprintf(buf + strlen(buf), "Content-Length: %d\r\n", length);
  }
  sprintf(buf + strlen(buf), "Connection: close\r\n");

// 3.响应头结束，需要有一个空行的
  sprintf(buf + strlen(buf),"\r\n");

// 4.get请求没有响应的正文的。

  send(cfd, buf, strlen(buf), 0);
  return 0;
}

const char* getFileType(const char* name)
{
    // a.jpg a.mp4 a.html
    // 自右向左查找‘.’字符，如不存在返回NULL
    const char* dot = strrchr(name, '.');
    if (dot == NULL)
      return "text/plain; charset=utf-8"; // 纯文本
    if (strcmp(dot, ".html") == 0 || strcmp(dot, ".htm") == 0)
      return "text/html; charset=utf-8";
    if (strcmp(dot, ".jpg") == 0 || strcmp(dot, ".jpeg") == 0)
      return "image/jpeg";
    if (strcmp(dot, ".gif") == 0)
      return "image/gif";
    if (strcmp(dot, ".png") == 0)
      return "image/png";
    if (strcmp(dot, ".css") == 0)
      return "text/css";
    if (strcmp(dot, ".au") == 0)
      return "audio/basic";
    if (strcmp(dot, ".wav") == 0)
      return "audio/wav";
    if (strcmp(dot, ".avi") == 0)
    return "video/x-msvideo";
    if (strcmp(dot, ".mov") == 0 || strcmp(dot, ".qt") == 0)
      return "video/quicktime";
    if (strcmp(dot, ".mpeg") == 0 || strcmp(dot, ".mpe") == 0)
      return "video/mpeg";
    if (strcmp(dot, ".vrml") == 0 || strcmp(dot, ".wrl") == 0)
      return "model/vrml";
    if (strcmp(dot, ".midi") == 0 || strcmp(dot, ".mid") == 0)
      return "audio/midi";
    if (strcmp(dot, ".mp3") == 0)
      return "audio/mpeg";
    if (strcmp(dot, ".ogg") == 0)
      return "application/ogg";
    if (strcmp(dot, ".pac") == 0)
      return "application/x-ns-proxy-autoconfig";

    return "text/plain; charset=utf-8";
}

//
/*
 *int sendDir(const char* dirName, int cfd)
 *{
 *  char buf[4096] = {0};
 *  sprintf(buf, "<html><head><title>%s</title></head><body><table>", dirName);
 *  struct dirent** namelist;
 *  int num = scandir(dirName, &namelist, NULL, alphasort);
 *  for(int i = 0; i < num; ++i)
 *  {
 *    // 名字取出来 namelist是一个指针数组的
 *    char* name = namelist[i]->d_name; 
 *    struct stat st;
 *    char subPath[1024] = {0};
 *    sprintf(subPath, "%s/%s", dirName, name);
 *    stat(subPath, &st);
 *    if(S_ISDIR(st.st_mode))
 *    {
 *      sprintf(buf + strlen(buf), "<td><a href=\"%s/\">>%s</a></td><td>%ld</td></tr>", name, name, st.st_size);
 *    }
 *    else 
 *    {
 *      sprintf(buf + strlen(buf), "<td><a href=\"%s\">>%s</a></td><td>%ld</td></tr>", name, name, st.st_size);
 *    }
 *    send(cfd, buf, strlen(buf), 0);
 *    memset(buf, 0, sizeof buf);
 *
 *    free(namelist[i]);
 *  }
 *  sprintf(buf, "</table></body></html>");
 *  send(cfd, buf, strlen(buf), 0);
 *  free(namelist);
 *  return 0;
 *}
 *
 */
int sendDir(const char* dirName, int cfd)
{
    char buf[4096];

    // 1. 页面开头
    int n = snprintf(buf, sizeof(buf),
        "<!DOCTYPE html>"
        "<html lang=\"zh-CN\">"
        "<head>"
        "<meta charset=\"UTF-8\">"
        "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
        "<title>目录索引 - %s</title>"
        "<style>"
        "body {"
        "  font-family: Arial, \"Microsoft YaHei\", sans-serif;"
        "  background: #f5f7fa;"
        "  margin: 0;"
        "  padding: 30px;"
        "  color: #333;"
        "}"
        ".container {"
        "  max-width: 900px;"
        "  margin: 0 auto;"
        "  background: white;"
        "  border-radius: 12px;"
        "  box-shadow: 0 4px 20px rgba(0,0,0,0.08);"
        "  padding: 24px;"
        "}"
        "h1 {"
        "  margin-top: 0;"
        "  font-size: 28px;"
        "  color: #222;"
        "}"
        ".path {"
        "  color: #666;"
        "  margin-bottom: 20px;"
        "  font-size: 14px;"
        "}"
        "table {"
        "  width: 100%;"
        "  border-collapse: collapse;"
        "  overflow: hidden;"
        "}"
        "th, td {"
        "  padding: 12px 14px;"
        "  border-bottom: 1px solid #eee;"
        "  text-align: left;"
        "}"
        "th {"
        "  background: #f0f3f8;"
        "  color: #444;"
        "}"
        "tr:hover {"
        "  background: #f9fbff;"
        "}"
        "a {"
        "  color: #1677ff;"
        "  text-decoration: none;"
        "}"
        "a:hover {"
        "  text-decoration: underline;"
        "}"
        ".dir { color: #d48806; font-weight: bold; }"
        ".file { color: #1677ff; }"
        ".footer {"
        "  margin-top: 18px;"
        "  color: #888;"
        "  font-size: 13px;"
        "}"
        "</style>"
        "</head>"
        "<body>"
        "<div class=\"container\">"
        "<h1>目录索引</h1>"
        "<div class=\"path\">当前路径：%s</div>"
        "<table>"
        "<tr><th>名称</th><th>类型</th><th>大小</th></tr>",
        dirName, dirName);

    send(cfd, buf, n, 0);

    // 2. 扫描目录
    struct dirent** namelist = NULL;
    int num = scandir(dirName, &namelist, NULL, alphasort);
    if(num < 0)
    {
        const char* msg = "<tr><td colspan=\"3\">无法读取目录</td></tr>";
        send(cfd, msg, strlen(msg), 0);
    }
    else
    {
        for(int i = 0; i < num; ++i)
        {
            char* name = namelist[i]->d_name;

            // 可选：跳过当前目录 "."
            if(strcmp(name, ".") == 0)
            {
                free(namelist[i]);
                continue;
            }

            struct stat st;
            char subPath[1024] = {0};
            snprintf(subPath, sizeof(subPath), "%s/%s", dirName, name);

            if(stat(subPath, &st) == -1)
            {
                free(namelist[i]);
                continue;
            }

            char sizeBuf[64] = {0};

            if(S_ISDIR(st.st_mode))
            {
                strcpy(sizeBuf, "-");
                n = snprintf(buf, sizeof(buf),
                    "<tr>"
                    "<td><a class=\"dir\" href=\"%s/\">📁 %s/</a></td>"
                    "<td>目录</td>"
                    "<td>%s</td>"
                    "</tr>",
                    name, name, sizeBuf);
            }
            else
            {
                formatSize(st.st_size, sizeBuf, sizeof(sizeBuf));
                n = snprintf(buf, sizeof(buf),
                    "<tr>"
                    "<td><a class=\"file\" href=\"%s\">📄 %s</a></td>"
                    "<td>文件</td>"
                    "<td>%s</td>"
                    "</tr>",
                    name, name, sizeBuf);
            }

            send(cfd, buf, n, 0);
            free(namelist[i]);
        }

        free(namelist);
    }

    // 3. 页面结尾
    n = snprintf(buf, sizeof(buf),
        "</table>"
        "<div class=\"footer\">Simple HTTP Server Directory Listing</div>"
        "</div>"
        "</body>"
        "</html>");

    send(cfd, buf, n, 0);
    return 0;
}

void formatSize(off_t size, char* out, int len)
{
    if(size < 1024)
    {
        snprintf(out, len, "%ld B", (long)size);
    }
    else if(size < 1024 * 1024)
    {
        snprintf(out, len, "%.2f KB", size / 1024.0);
    }
    else if(size < 1024LL * 1024 * 1024)
    {
        snprintf(out, len, "%.2f MB", size / (1024.0 * 1024));
    }
    else
    {
        snprintf(out, len, "%.2f GB", size / (1024.0 * 1024 * 1024));
    }
}

// 将字符转换为整形数
// 16-10 进制的转换的
int hexToDec(char c)
{
    if (c >= '0' && c <= '9')
        return c - '0';
    if (c >= 'a' && c <= 'f')
        return c - 'a' + 10;
    if (c >= 'A' && c <= 'F')
        return c - 'A' + 10;

    return 0;
}

// 解码
// to 存储解码之后的数据, 传出参数, from被解码的数据, 传入参数
void decodeMsg(char* to, char* from)
{
    for (; *from != '\0'; ++to, ++from)
    {
        // isxdigit -> 判断字符是不是16进制格式, 取值在 0-f
        // Linux%E5%86%85%E6%A0%B8.jpg
        if (from[0] == '%' && isxdigit(from[1]) && isxdigit(from[2]))
        {
            // 将16进制的数 -> 十进制 将这个数值赋值给了字符 int -> char
            // B2 == 178
            // 将3个字符, 变成了一个字符, 这个字符就是原始数据
            *to = hexToDec(from[1]) * 16 + hexToDec(from[2]);

            // 跳过 from[1] 和 from[2] 因此在当前循环中已经处理过了
            from += 2;
        }
        else
        {
            // 字符拷贝, 赋值
            *to = *from;
        }

    }
    *to = '\0';
}
