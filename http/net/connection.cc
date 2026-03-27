/*
 *#include "connection.hpp"
 *#include <cstring>
 *
 *Connection::Connection(int sock)
 *  :_sock(sock)
 *  ,_inbuffer("")
 *  ,_header("")
 *  ,_content("")
 *  ,_outbuffer("")
 *  ,_is_closed(false)
 *{}
 *
 *Connection::~Connection()
 *{
 *  _sock = -1;
 *  _is_closed = true;
 *}
 *
 *bool Connection::parseContentLenght(const std::string& inbuffer, size_t* content_length)
 *{
 *  *content_length = 0;
 *  size_t pos = inbuffer.find("Content-Length:");
 *  if(pos == std::string::npos)
 *  {
 *    std::cout<< "这是个请求没有Content-Length:" <<std::endl;
 *    return false;
 *  }
 *  
 *  pos += strlen("Content-Length:");
 *  if(pos < inbuffer.size() && inbuffer[pos] == ' ')
 *  {
 *    ++pos;
 *  }
 *  
 *  size_t end = inbuffer.find("\r\n", pos);
 *  std::string str_len = inbuffer.substr(pos, end);
 *  
 *  *content_length = stoi(str_len);
 *
 *  return true;
 *}
 *
 *bool Connection::getOneHttpRequest(std::string& inbuffer)
 *{
 *  size_t hear_end_pos = inbuffer.find("\r\n\r\n");
 *  if(hear_end_pos == std::string::npos)
 *  {
 *    return false;
 *  }
 *  
 *  size_t hear_end = hear_end_pos + 4;
 *  _header = inbuffer.substr(0, hear_end);
 *
 *  size_t  content_length = 0;
 *  bool body = parseContentLenght(inbuffer, &content_length);
 *
 *  size_t total_len = hear_end;
 *  if(body)
 *  {
 *    total_len += content_length;
 *  }
 *
 *  if(inbuffer.size() < total_len)
 *  {
 *    return false;
 *  }
 *
 *  _inbuffer = inbuffer.substr(0, total_len);
 *
 *  return true;
 *}
 *
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
 *
 *std::string& Connection::InBuffer()
 *{
 *  return _inbuffer;
 *}
 *
 *std::string& Connection::Header()
 *{
 *  return _header;
 *}
 *
 *std::string& Connection::Content()
 *{
 *    return _content;
 *}
 */

 #include "connection.hpp"

Connection::Connection(int sock)
    : _sock(sock)
    , _inbuffer()
    , _outbuffer()
    , _is_closed(false)
{}

Connection::~Connection()
{
    Close();
}

ssize_t Connection::Read()
{
    if (_is_closed || _sock < 0)
    {
        return -1;
    }

    char buffer[4096];
    ssize_t n = recv(_sock, buffer, sizeof(buffer), 0);

    if (n > 0)
    {
        _inbuffer.append(buffer, n);
    }
    else if (n == 0)
    {
        // 对端关闭连接
        Close();
    }
    else
    {
        // 出错
        std::cerr << "recv failed: " << strerror(errno) << std::endl;
        Close();
    }

    return n;
}

ssize_t Connection::Write()
{
    if (_is_closed || _sock < 0)
    {
        return -1;
    }

    if (_outbuffer.empty())
    {
        return 0;
    }

    ssize_t n = send(_sock, _outbuffer.c_str(), _outbuffer.size(), 0);

    if (n > 0)
    {
        // 已发送的数据从缓冲区移除
        _outbuffer.erase(0, n);
    }
    else if (n == 0)
    {
        // 一般 send 返回 0 很少见，这里按关闭处理
        Close();
    }
    else
    {
        std::cerr << "send failed: " << strerror(errno) << std::endl;
        Close();
    }

    return n;
}

void Connection::AppendOutBuffer(const std::string& data)
{
    _outbuffer += data;
}

std::string& Connection::InBuffer()
{
    return _inbuffer;
}

std::string& Connection::OutBuffer()
{
    return _outbuffer;
}

void Connection::ClearInBuffer()
{
    _inbuffer.clear();
}

void Connection::ClearOutBuffer()
{
    _outbuffer.clear();
}

int Connection::GetSock() const
{
    return _sock;
}

bool Connection::IsClosed() const
{
    return _is_closed;
}

void Connection::Close()
{
    if (!_is_closed && _sock >= 0)
    {
        close(_sock);
        _sock = -1;
        _is_closed = true;
    }
}
