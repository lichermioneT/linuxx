#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "Util.hpp"

const std::string sep = "\r\n";
const std::string default_root = "./wwwroot";
const std::string home_page = "index.html";

class HttpRequest
{
public:
    HttpRequest(){}
    ~HttpRequest(){}
public:
    std::string inbuffer;

    std::string method;
    std::string url;
    std::string httpversion;
    std::string path;
};


class HttpResponse
{
public:
    std::string outbuffer;
};