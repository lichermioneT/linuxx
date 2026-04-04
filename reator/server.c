#include "server.h"
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
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = htonl(INADDR_ANY);
  addr.sin_port = htons(port);               // 不建议写死了，函数参数传进来的

  ret = bind(lfd, (struct sockaddr*)&addr, sizeof(addr));
  if(ret == -1)
  {
    perror("bind");
    return -1;
  }

  // 4.listen 
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
  // 1.创建实例
  int epfd = epoll_create(1);
  if(epfd == -1)
  {
    perror("epoll_create");
    return -1;
  }

  // 2.lfd上树
  struct epoll_event ev;
  ev.data.fd = lfd;
  ev.events = EPOLLIN;
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
      int fd = evs[i].data.fd;
      if(fd == lfd)
      {
        // 监听文件描述符，建立新的链接accept(); 内核告诉我们客户端已经来建立链接了的
        // 通过listenfd，把新来的文件描述符添加到epoll树上去的。 epoll树的文件描述符越来越多的
        acceptClient(lfd, epfd);              
      }
      else 
      {
        // 主要是读数据的。服务器的任务。
        recvHttpRequest(fd, epfd);
      }
    }
  }

}

int acceptClient(int lfd, int epfd)
{
  // 1.建立链接
  int cfd = accept(lfd, NULL, NULL);
  if(cfd == -1)
  {
    perror("accept");
    return -1;
  }

  // 2.设置文件描述符的属性，非阻塞
  int flag = fcntl(cfd, F_GETFL);
  flag |= O_NONBLOCK;
  fcntl(cfd, F_SETFL, flag);

  // 3.添加到epoll模型里面去的
  struct epoll_event ev;
  ev.data.fd = cfd;
  ev.events = EPOLLIN | EPOLLET;
  int ret = epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &ev);
  if(ret == -1)
  {
    perror("epoll_ctl");
    return -1;
  }

  return 0;
}

int recvHttpRequest(int cfd, int epfd)
{
  printf("开始接收数据了...\n");
  int len = 0;
  int total = 0;
  char tmp[1024] = {0};
  char buffer[4096] = {0};
  while((len = recv(cfd, tmp, sizeof tmp, 0)) > 0)
  {
    if((total + len < sizeof buffer))
    {
       memcpy(tmp + total, buffer + total, len);
    }

    total += len;
  }

  // 判断数据是否接收完毕的。
  if(len == -1 && errno == EAGAIN)
  {
    // 解析请求的
    char* pt = strstr(buffer, "\r\n");
    int reqLen = pt - buffer;
    buffer[reqLen] = '\0';
    parseRequestLine(buffer, cfd);
  }
  else if(len == 0)
  {
    // 客户端已经断开链接
    epoll_ctl(epfd, EPOLL_CTL_DEL, cfd, NULL);
    close(cfd);
  }
  else 
  {
    perror("recv");
  }
  return 0;
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
  
  // 处理客户端请求的静态资源(目录或者文件)
  char* file = NULL;
  if(strcmp(path, "/") == 0)
  {
    file = "./";
  }
  else 
  {
    file = path + 1;
  }

  struct stat st;
  int ret = stat(file, &st);
  if(ret == -1)
  {
    // 文件不存在，回复404
    sendHeadMsg(cfd, 404, "Not Found", getFileType(".html"), -1);
    sendFile("404.html", cfd);

    return 0;
  }

  if(S_ISDIR(st.st_mode))
  {
    // 把这个目录中的内容发送给客户端
    sendHeadMsg(cfd, 200, "OK", getFileType(".html"), -1);
    sendDir(file, cfd);
  }
  else 
  {
    // 文件的内容发送给客户端的。
    sendHeadMsg(cfd, 200, "OK", getFileType(file), st.st_size);
    sendFile(file, cfd);
  }

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
  int size = lseek(fd, 0, SEEK_END); // 
  sendfile(cfd, fd, NULL, size);

#endif
  return 0;
}


int sendHeadMsg(int cfd, int status, const char* descr, const char* type, int length)
{
  char buf[1024] = {0};
  // 状态行
  sprintf(buf, "http/1.1 %d %s\r\n", status, descr);
  // 响应头
  snprintf(buf, + strlen(buf), "content-type: %s\r\n", type);
  snprintf(buf, + strlen(buf), "content-length: %d\r\n\r\n", length);

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
int sendDir(const char* dirName, int cfd)
{
  char buf[4096] = {0};
  sprintf(buf, "<html><head><title>%s</title></head><body><table>", dirName);
  struct dirent** namelist;
  int num = scandir(dirName, &namelist, NULL, alphasort);
  for(int i = 0; i < num; ++i)
  {
    // 名字取出来 namelist是一个指针数组的
    char* name = namelist[i]->d_name; 
    struct stat st;
    char subPath[1024] = {0};
    sprintf(subPath, "%s/%s", dirName, name);
    stat(name, &st);
    if(S_ISDIR(st.st_mode))
    {
      sprintf(buf + strlen(buf), "<td><a href=\"%s/\">>%s</a></td><td>%ld</td></tr>", name, name, st.st_size);
    }
    else 
    {
      sprintf(buf + strlen(buf), "<td><a href=\"%s\">>%s</a></td><td>%ld</td></tr>", name, name, st.st_size);
    }
    send(cfd, buf, strlen(buf), 0);
    memset(buf, 0, sizeof buf);

    free(namelist[i]);
  }
  sprintf(buf, "</table></body></html>");
  send(cfd, buf, strlen(buf), 0);
  free(namelist);
  return 0;
}

