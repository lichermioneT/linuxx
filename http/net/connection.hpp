#pragma once
#include <iostream>
#include <string>
#include <unistd.h>
#include <cstring>
#include <sys/socket.h>

class Connection
{
public:
    Connection(int sock = -1);
    ~Connection();
    
    ssize_t  Read();
    ssize_t Write();

    void AppendOutBuffer(const std::string& data);

    std::string& InBuffer();
    std::string& OutBuffer();
    std::string& Header();
    std::string& Content();

    void ClearInBuffer();
    void ClearOutBuffer();

    int GetSock() const;
    bool IsClosed() const;
    void Close();

private:
    int _sock;
    std::string _inbuffer;
    std::string _header;
    std::string _content;
    std::string _outbuffer;
    bool _is_closed;
private:
    // 1.找到Content_length
    // 2.读到一个完整的请求
    bool parseContentLenght(const std::string& inbuffer, size_t* content_length);
    bool getOneHttpRequest(std::string& inbuffer);
};
