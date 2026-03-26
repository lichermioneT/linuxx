#pragma once
#include <sstream>
#include <string>
#include <iostream>
#include <sys/types.h>    
#include <sys/stat.h>
#include <sys/socket.h>    
#include <netinet/in.h>    
#include <arpa/inet.h>    
#include "protocal.hpp"
    
#include <unistd.h>    
#include <cstring>    
#include <stdio.h>    
#include <iostream>    
#include <string>    
#include <functional>
using namespace std;

 const string sep = "\r\n";
 const string default_root = "./wwwroot";
 const string home_page = "index.html";
 const string html_404 = "./wwwroot/404.html";


class request
{
public:
  void parse()
  {
// 1.method url httpversion;
    string line = getoneline(inbuffer, "\r\n");
    if(line.empty()) return;
    stringstream ss(line);
    ss>> method >> url >> httpversion;
// 2. 添加路径 信息
   path = default_root;
   path += url;
    

       // 4.path资源的后缀。
      auto pos = path.rfind(".");
       if(pos == string::npos) suffix = ".html";
       else suffix = path.substr(pos);
  
       // 5.资源的大小
      struct stat st;
      int n = stat(path.c_str(), &st);
      if(n == 0) size = st.st_size;
      else size = -1;

  }
public:
  string inbuffer;
  string method;
  string url;
  string httpversion;

  string path;
  string suffix;
  int size;

private:
static string getoneline(string& buffer, const string& sep)
{
  auto pos = buffer.find(sep);
  if(pos == string::npos)
  {
    cout<< "请求行提取失败" <<endl; 
    exit(-1);
  }
  
  string sub = buffer.substr(0, pos);
  buffer.erase(0, sub.size() + sep.size());
  return sub;
}
};

class response
{
public:
  string outbuffer;
};

/*
 *POST /login HTTP/1.1\r\n
 *Host: 127.0.0.1\r\n
 *Content-Length: 11\r\n
 *\r\n
 *hello=world
 */

// 读取到Content-Length: 11\r\n;
bool parseContentLength(const string& header, size_t* content_length)
{
  *content_length = 0;

//1.pos找到了指向:Content-Length:的C的下标。
  size_t pos = header.find("Content-Length:");
  if(pos == string::npos)
  {
    cout<< "这个浏览器的请求方法没有(post)Content-Length" <<endl;
    return false;
  }
  
//2.pos + strlen(Content-Lenght:)的空格滴
//++pos,指向了数字
  pos += strlen("Content-Length:");
  if(pos < header.size() && header[pos] == ' ')
  {
    ++pos;
  }

//3.从pos开始找\r\n; end等于\r的下标位置的。
  size_t end = header.find("\r\n", pos);
  if(end == string::npos)
  {
    return false;
  }

//4.end - pos就是正文的长度信息的。
  string len_str = header.substr(pos,end - pos);
  *content_length = stoi(len_str);

  return true;
}

bool GetOneHttpRequest(string& inbuffer, string* request)
{

// 1.找到\r\n\r\n
  size_t header_end_pos = inbuffer.find("\r\n\r\n");
  if(header_end_pos == string::npos)
  {
    return false;
  }
  
//2.加上\r\n\r\n
  size_t header_end = header_end_pos + 4;
  string header = inbuffer.substr(0, header_end);

//3.把header整个传入进行，然后找到Content-Length;
  size_t content_length = 0;
  bool has_body = parseContentLength(header, &content_length);
  
//4.一个请求的长度
//header + content_length;
  size_t total_len = header_end;
  if(has_body)
  {
    total_len += content_length;
  }

  if(inbuffer.size() < total_len)
  {
    return false;
  }

  *request = inbuffer.substr(0, total_len);
  inbuffer.erase(0, total_len);
  return true;
}


bool ReadOneRequest(int sock, string& inbuffer, string* request)
{
    char buffer[4096];

    while (true)
    {
        // 先尝试看看 inbuffer 里是否已经有完整请求
        if (GetOneHttpRequest(inbuffer, request))
        {
            return true;
        }

        ssize_t n = recv(sock, buffer, sizeof(buffer), 0);
        if (n > 0)
        {
            inbuffer.append(buffer, n);
        }
        else if (n == 0)
        {
            // 对端关闭连接
            return false;
        }
        else
        {
            // recv 出错
            return false;
        }
    }
}


