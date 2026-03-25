#pragma once 

#include <iostream>
#include <string>
#include <fstream>
using namespace std;

class util 
{
public:
// 1.从一个完整的请求里面提取第一行的的新。
  static string getoneline(string& buffer, const string& sep)
  {
    auto pos = buffer.find(sep);
    if(pos == string::npos)  return "";

    string sub = buffer.substr(0,pos);
    buffer.erase(0,sub.size() + sep.size());

    return sub;
  }

// 2.读取整个文件.html文件的信息。
  static bool readFile(const string resource, string *out)
  {
    ifstream in(resource);
    if(!in.is_open()) return false;

    string line;
    while(getline(in, line))
    {
      *out += line;
    }
    in.close();

   return true;
  }

};

