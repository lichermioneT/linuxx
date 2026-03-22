#pragma once 
#include <iostream>
#include <cstring>
#include <string>
using namespace std;

#define SEP " "
#define SEP_LEN strlen(SEP)

#define LINE_SEP "\r\n"
#define LINE_SEP_lEN strlen(LINE_SEP)

const string& enlength(const string& text)
{
  
}


const string& delength(const string& package)
{
  
}

class request 
{
public:
  request(int x, int y, char op)
    :_x(x)
    ,_y(y)
    ,_op(op)
  {}
  
//1.开始序列化。
  bool serialize(string* out)
  {
    *out = "";
    //1.结构化 "x op y \r\n"
    
    string x_string = to_string(_x);
    string y_string = to_string(_y);
    
    *out += x_string;
    *out += SEP;
    *out += _op;
    *out += SEP;
    *out += y_string;
    *out += LINE_SEP;
  }

//2.反序列化
//"x op y\r\n"
  bool deserialize(const string& in)
  {

  }

public:
  // x op y
  int _x;
  int _y;
  char _op;
};



class response
{
public:
  response()
    :exotcode(0)
    ,result(0)
  {}

  bool serialize(string* out){}
  bool deserialize(){}
public:
  int exotcode; // 0成功，!0失败。
  int result;   // 真正的计算的结果
};
