#pragma once
#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>

// 初始化监听套接字
int initListenFd(unsigned short port);

// 启动epoll
int epollRun(int lfd);

// 和客户端建立链接
void* acceptClient(void* arg);

// 接收http请求
void* recvHttpRequest(void* arg);

// 请求解析
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


