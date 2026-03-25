#pragma once
#include "protocol.hpp"
#include "log.hpp"
#include <string>
#include <iostream>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <unistd.h>
#include <cstring>
#include <stdio.h>
#include <iostream>
#include <string>
using namespace std;

class client
{
public:
  client(const string& serverip, const uint16_t& serverport)
    : _sock(-1)
    , _serverip(serverip)
    , _serverport(serverport)
  {}

  void init()
  {
    // 1. 创建套接字
    _sock = socket(AF_INET, SOCK_STREAM, 0);
    if(_sock < 0)
    {
      cout << "socket failed" << endl;
      exit(-1);
    }
  }
  
  // [修改1] 新增：保证把完整数据全部发送出去
  bool sendAll(const string& send_string)
  {
    size_t total = 0;
    while(total < send_string.size())
    {
      ssize_t n = send(_sock,
                       send_string.c_str() + total,
                       send_string.size() - total,
                       0);
      if(n > 0)
      {
        total += n;
      }
      else
      {
        return false;
      }
    }
    return true;
  }

  void start()
  {
    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(_serverip.c_str());
    server.sin_port = htons(_serverport);

    if(connect(_sock, (struct sockaddr*)&server, sizeof(server)) != 0)
    {
      cout << "connect failed" << endl;
      return; // [修改2] 连接失败直接返回
    }

    string msg;
    string inbuffer;
// contentSize" "text" "。 

    while(true)
    {
      cout << "mycal# ";
      if(!getline(cin, msg))  // [修改3] 处理标准输入结束
      {
        break;
      }

      if(msg == "quit" || msg == "exit") // [修改4] 支持退出
      {
        break;
      }

      // [修改5] 用用户输入来构造请求，而不是写死 10 10 +
// 输入格式要求：x op y
// 请求的反序列化就是 request成员属性可以拿到 x,op,y。
      request req;
      if(!req.deserialize(msg))
      {
        cout << "input format error, please input like: 10 + 20" << endl;
        continue;
      }
      cout<< "输入的数据:" << msg <<endl;

// 序列化x" "op" "y。

      string content;
      if(!req.serialize(&content))
      {
        cout << "request serialize failed" << endl;
        continue;
      }

// 开始发送文本
// contentlen \r\n text \r\n
      string send_string = enlength(content);
      cout<< "客户端输入进行序列化的结果和加长的结果\n" << send_string << endl;

      // [修改6] 发送时必须保证完整发送
      if(!sendAll(send_string))
      {
        cout << "send failed, server may be disconnected" << endl;
        break;
      }

      string package, text;

      // [修改7] 接收失败不能 continue，要退出循环
      // 这里如果你已经把 protocol.hpp 里的函数名改成 recvRequest，
      // 那这里也要同步改名
      if(!recvRequset(_sock, inbuffer, &package))
      {
        cout << "recv failed, server closed or error occurred" << endl;
        break;
      }

      cout<< "客户端接收服务器的响应的结果:" << package <<endl;
      if(!delength(package, &text))
      {
        cout << "package decode failed" << endl;
        continue;
      }
      cout<< "客户端接收服务器的响应的结果,然后剪短:" << text <<endl;

      response resp;
      // [修改8] 反序列化结果要检查
      if(!resp.deserialize(text))
      {
        cout << "response deserialize failed" << endl;
        continue;
      }

      cout << "exitcode: " << resp.exotcode << endl;
      cout << "result: " << resp.result << endl;
    }

    // [修改9] 客户端退出时关闭 socket
    if(_sock >= 0)
    {
      close(_sock);
      _sock = -1;
    }
  }
private:
  int _sock;
  string _serverip;
  uint16_t _serverport;
};
