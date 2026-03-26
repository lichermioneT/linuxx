#pragma once 
#include <iostream>
#include "util.hpp"
#include <sstream>
#include <string>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
using namespace std;

const string sep = "\r\n";
const string default_root = "./wwwroot";
const string home_page = "index.html";
const string html_404 = "./wwwroot/404.html";


class httprequest
{
public:
    void parse()
    {
// 1.从inbuffer提取第一行
      string line = util::getoneline(inbuffer, sep);  
      if(line.empty()) return;

// 2.从请求行中提取method,url,httpvsersion
      stringstream ss(line);
      ss>>method>>url>>httpvsersion;
      
      // 3.添加默认的路径
     path = default_root; // .wwwroot/
     path += url;                // .wwwroot/a/b/c.html;

     if(path[path.size() - 1] == '/') path += home_page;

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
   string httpvsersion;
   string path;
   string suffix;
   int size;
};



class httpresponse 
{
  public:
    string outbuffer;
};


/*
 *class httprequest
 *{
 *public:
 *    void parse()
 *    {
 *        // 1. 从 inbuffer 提取第一行
 *        string line = util::getoneline(inbuffer, sep);
 *        if(line.empty()) return;
 *
 *        cout << "first line: " << line << endl;
 *
 *        // 2. 从请求行中提取 method、url、httpversion
 *        stringstream ss(line);
 *        if(!(ss >> method >> url >> httpversion))
 *            return;
 *
 *        // 3. 去掉 URL 中的查询参数
 *        size_t pos = url.find('?');
 *        if(pos != string::npos)
 *        {
 *            url = url.substr(0, pos);
 *        }
 *
 *        // 4. 拼接默认路径
 *        path = default_root;
 *        path += url;
 *
 *        // 5. 如果访问的是目录，则补默认首页
 *        if(!path.empty() && path.back() == '/')
 *        {
 *            path += home_page;
 *        }
 *    }
 *
 *public:
 *    string inbuffer;
 *
 *    string method;
 *    string url;
 *    string httpversion;
 *    string path;
 *};
 */
