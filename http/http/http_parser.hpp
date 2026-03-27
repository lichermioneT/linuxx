#pragma once
#include <string>


class http_parser
{
public:
bool parseContentLenght(const std::string& inbuffer, size_t* content_length);
bool getOneHttpRequest(std::string& inbuffer); 

public:
    std::string _inbuffer;
    std::string _header;
};
