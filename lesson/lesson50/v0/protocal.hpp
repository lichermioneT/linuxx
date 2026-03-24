#pragma once 
#include <iostream>
#include <string>
#include <vector>
using namespace std;

class httprequest
{
public:
   string inbuffer;
   string reqline;
   vector<string> reqheader;
   string body;

   string method;
   string url;
   string httpvsersion;
};



class httpresponse 
{
  public:
    string outbuffer;
};

