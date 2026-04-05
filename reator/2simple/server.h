#pragma once
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

int initListenFd(uint16_t port);
int epollRun(int lfd);
void* acceptClient(void* arg);
void* recvHttpRequest(void* arg);
int parseRequestLine(const char* line, int cfd);

// 发送文件                                                                                                                                                                                    
int sendFile(const char* fileName, int cfd);      
      
// 发送响应头 状态行 + 响应头      
int sendHeadMsg(int cfd, int status, const char* descr, const char* type, int length);      
      
// 文件类型      
const char* getFileType(const char* name);      
      
//  发送目录      
int sendDir(const char* dirName, int cfd);      
      
void formatSize(off_t size, char* out, int len);      
      
// 中文乱码的问题      
int hexToDec(char c);      
void decodeMsg(char* to, char* from);   
