#pragma once 
#include <netinet/in.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include <string>
#include <unordered_map>
using namespace std;

class user
{
public:
  user(const string& ip, const uint16_t& port)
    :_ip(ip)
    ,_port(port)
  {}
  ~user(){}
  string ip(){return _ip;}
  uint16_t port() {return _port;}

private: 
  string _ip;
  uint16_t _port;
};

class onlineUser 
{
public:
  onlineUser(){}
  ~onlineUser(){}
  
  void addUser(const string& ip, const uint16_t& port)
  {
    string id = ip + "-" + to_string(port);
    users.insert(make_pair(id,user(ip,port)));
  }

  void delUser(const string& ip, const uint16_t& port)
  {
    string id = ip + "-" + to_string(port);
    users.erase(id);
  }

  bool isOline(const string& ip, const uint16_t& port)
  {
    string id = ip + "-" + to_string(port);
    return users.find(id) == users.end() ? false : true;
  }
  
  void broadcast(int socketfd, const string& message, const string& ip, const uint16_t& port)
  {
    for(auto& u : users)
    {
    struct  sockaddr_in client;
    bzero(&client, sizeof(client));

    client.sin_family = AF_INET;
    client.sin_addr.s_addr = inet_addr(u.second.ip().c_str());
    client.sin_port = htons(u.second.port());
    
    string s = ip +  "-" + to_string(port) + "#";
    s += message;
    sendto(socketfd, s.c_str(), s.size(), 0, (struct sockaddr*)&client, sizeof(client));
    }
  }

private:
  unordered_map<string, user> users;
};

