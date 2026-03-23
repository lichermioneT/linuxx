#pragma once 
#include "protocol.hpp"
#include "log.hpp"
#include <string>
#include <iostream>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <unistd.h>
#include <cstring>
#include <stdio.h>
#include <iostream>
#include <string>
using namespace std;


class client
{
public:
  client(const string& serverip, const uint16_t& serverport)
    :_sock(-1)
    ,_serverip(serverip)
    ,_serverport(serverport)
  {}

  void init()
  {
   //1.套接字创建
    _sock = socket(AF_INET, SOCK_STREAM, 0);
   if(_sock < 0)
   {
     cout<< "sock failend" << endl;
     exit(-1);
   }

   //2.需要bind，不需要显示的bind,同理udp.
  }
  
  void start()
  {
    // connect会帮你自动bind的。
    struct  sockaddr_in server;
    memset(&server, 0, sizeof(server));

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(_serverip.c_str());
    server.sin_port = htons(_serverport);

    if(connect(_sock, (struct sockaddr*)&server, sizeof(server)) != 0) 
    {
        cout<< "socket connet failed" << endl;
    }
    else 
    {
      string msg;
      string inbuffer;
      while(1)
      {
        cout<<"mycal:";  
        getline(cin, msg);

        request req(10, 10, '+');
        string content;
        req.serialize(&content);

        string send_string = enlength(content);

        send(_sock, send_string.c_str(), send_string.size(), 0); //bug不管的？
        string package, text;
        if(!recvRequset(_sock, inbuffer, &package)) continue;
        if(!delength(package, &text)) continue;

        response resp;
        resp.deserialize(text);
        
        cout<< "exitcode:" << resp.exotcode <<endl;
        cout<< "result:" << resp.result << endl;
      }
    }
  }







private:
  int _sock;
  string _serverip;
  uint16_t _serverport;
};
