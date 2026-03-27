#include "http_parser.hpp"
#include <iostream>
#include <cstring>

bool http_parser::parseContentLenght(const std::string& inbuffer, size_t* content_length)
{
  *content_length = 0;
  size_t pos = inbuffer.find("Content-Length:");
  if(pos == std::string::npos)
  {
    std::cout<< "这是个请求没有Content-Length:" <<std::endl;
    return false;
  }
  
  pos += strlen("Content-Length:");
  if(pos < inbuffer.size() && inbuffer[pos] == ' ')
  {
    ++pos;
  }
  
  size_t end = inbuffer.find("\r\n", pos);
  std::string str_len = inbuffer.substr(pos, end);
  
  *content_length = stoi(str_len);

  return true;
}                                                                                                                                                                                            

bool http_parser::getOneHttpRequest(std::string& inbuffer)
{
  size_t hear_end_pos = inbuffer.find("\r\n\r\n");
  if(hear_end_pos == std::string::npos)
  {                                                                                                                                                                                          
    return false;
  }
  
  size_t hear_end = hear_end_pos + 4;
  _header = inbuffer.substr(0, hear_end);

  size_t  content_length = 0;
  bool body = parseContentLenght(inbuffer, &content_length);

  size_t total_len = hear_end;
  if(body)
  {
    total_len += content_length;
  }

  if(inbuffer.size() < total_len)
  {
    return false;
  }

  _inbuffer = inbuffer.substr(0, total_len);

  return true;
}

/*
 *bool Connection::Read()
 *{
 *  char buffer[4096] = {0};
 *  std::string inbuffer = "";
 *  while(true)
 *  {
 *    if(getOneHttpRequest(inbuffer))
 *    {
 *      return true;
 *    }
 *
 *    size_t n = recv(_sock, buffer, sizeof(buffer), 0);
 *    if(n > 0)
 *    {
 *      inbuffer.append(buffer, n);
 *    }
 *    else if (n == 0)
 *    {
 *      return false;
 *    }
 *    else 
 *    {
 *      return false;
 *    }
 *  }
 *  
 *  return true;
 *}
 *
 */
