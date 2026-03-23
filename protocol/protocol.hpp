#pragma once 
#include <iostream>
#include <cstring>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
using namespace std;

#define SEP " "
#define SEP_LEN strlen(SEP)

#define LINE_SEP "\r\n"
#define LINE_SEP_LEN strlen(LINE_SEP)

//"exotcode result"  --> content_len "\r\n" exotcode result "\r\n"
// enlength:后的结果
// size 
// exotcode result 

// 
string enlength(const string& text)
{
    string send_string = to_string(text.size());

    send_string += LINE_SEP;
    send_string += text;
    send_string += LINE_SEP;

    return send_string;
}

// content_len "\r\n" exotcode result "\r\n"
// *text = exotcode result
bool delength(const string& package, string* text)
{
  auto pos = package.find(LINE_SEP); 
  if(pos == string::npos) return false;

  string text_len_string = package.substr(0, pos);
  size_t text_len = stoi(text_len_string);
  
  *text = package.substr(pos + LINE_SEP_LEN, text_len);
  return true; 
}

// 请求序列化和反序列化
class request 
{
public:
  request(int x = 0, int y = 0, char op = '+')
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
    
// x\r\nop\r\ny
// x
// op 
// y
    *out += x_string;
    *out += SEP;
    *out += _op;
    *out += SEP;
    *out += y_string;

    return true;
  }

//2.反序列化
// x\r\nop\r\ny
  bool deserialize(const string& in)
  {
    auto left = in.find(SEP);
    auto right = in.rfind(SEP);
    if(left == string::npos || right == string::npos)
      return false;
    if(left == right)
      return false;
    if(right - left - SEP_LEN != 1)
      return false;

    string x_string = in.substr(0,left);
    string y_string = in.substr(right + SEP_LEN);
    if(x_string.empty()) return false;
    if(y_string.empty()) return false;

    _x = stoi(x_string);
    _y = stoi(y_string);
    _op = in[left + SEP_LEN];
    
    return true;
  }

public:
  // x op y
  int _x;
  int _y;
  char _op;
};


// 响应的序列化
class response
{
public:
  response(int exotcode_ = 0, int result_ = 0)
    :exotcode(exotcode_)
    ,result(result_)
  {}

// exotcode\r\nresult
  bool serialize(string* out) 
  {
    *out = "";
    string ec_string = to_string(exotcode);
    string re_string = to_string(result); 

    *out += ec_string;
    *out += SEP;
    *out += re_string;

    return true;
  }

// exotcode\r\nresult
// exotcode 
// result
  bool deserialize(const string& in)
  {
    // exit result;
    auto mid = in.find(SEP);
    if(mid == string::npos) return false;
    
    string ec_string = in.substr(0, mid);
    string re_string = in.substr(mid + SEP_LEN);

    if(ec_string.empty() || re_string.empty()) return false;

    exotcode = stoi(ec_string);
    result = stoi(re_string);

    return true;
  }
public:
  int exotcode; // 0成功，!0失败。
  int result;   // 真正的计算的结果
};

bool recvRequset(int sock, string& inbuffer, string* text)
{
char buffer[1024];
while (true)
{
    ssize_t n = recv(sock, buffer, sizeof(buffer)-1, 0);
    if(n > 0)
    {
        buffer[n] = 0;
        inbuffer += buffer;
        // 分析处理
        auto pos = inbuffer.find(LINE_SEP);
        if(pos == std::string::npos) continue;

        std::string text_len_string = inbuffer.substr(0, pos);
        int text_len = std::stoi(text_len_string);

        size_t total_len = text_len_string.size() + 2*LINE_SEP_LEN + text_len;
        // text_len_string + "\r\n" + text + "\r\n" <= inbuffer.size();
        if(inbuffer.size() < total_len) continue;

        // 至少有一个完整的报文
        *text = inbuffer.substr(0, total_len);
        inbuffer.erase(0, total_len);
        break;
    }
    else if(n  == 0)
    {
      return false;
    }
    else 
    {
      return false;
    }
}

return true;
  
}

