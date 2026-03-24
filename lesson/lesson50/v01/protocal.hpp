#pragma once 
#include <iostream>
#include "util.hpp"
#include <sstream>
#include <string>
#include <vector>
using namespace std;

const string sep = "\r\n";
const string default_root = "./wwwroot";
const string home_page = "index.html";


class httprequest
{
public:
    void parse()
    {
      // 1.从inbuffer提取第一行
      string line = util::getoneline(inbuffer, sep);  
      if(line.empty()) return;
      cout<< "first line:" << line <<endl;

      // 2.从请求行中提取method,url,httpvsersion
      stringstream ss(line);
      ss>>method>>url>>httpvsersion;
      
      // 3.添加默认的路径
     string path = default_root; // .wwwroot/
     path += url;                // .wwwroot/a/b/c.html;

     if(path[path.size() - 1] == '/') path += home_page;
    }

public:
   string inbuffer;

   string method;
   string url;
   string httpvsersion;
   string path;
};



class httpresponse 
{
  public:
    string outbuffer;
};

