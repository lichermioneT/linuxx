#pragma once 

#include <iostream>
#include <string>
using namespace std;

class util 
{
public:
  static string getoneline(string& buffer, const string& sep)
  {
    auto pos = buffer.find(sep);
    if(pos == string::npos)  return "";

    string sub = buffer.substr(0,pos);
    buffer.erase(0,sub.size() + sep.size());

    return sub;
  }

};

