#pragma once

#include <iostream>
#include <string>
#include <unordered_map>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <unistd.h>
#include <cstdlib>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <functional>

using namespace std;

class User
{
public:
    User(const string &ip, const uint16_t &port):_ip(ip), _port(port)
    {
    }

    string ip() {return _ip;}
    uint16_t  port() {return _port;}
    ~User(){}
private:
    string _ip;
    uint16_t _port;
};


class OnlineUser
{
public:
    OnlineUser(){}
    ~OnlineUser(){}

    void addUser(const string &ip, const uint16_t &port)
    {
        string id = ip + "-" + to_string(port);
        users.insert(make_pair(id, User(ip, port)));
    }

    void delUser(const string &ip, const uint16_t &port)
    {
        string id = ip + "-" + to_string(port);
        users.erase(id);
    } // 注意：原图中此处缺少函数结束括号

    bool isOnline(const string &ip, const uint16_t &port) // 原图中此处缺少 public: 修饰符
    {
        string id = ip + "-" + to_string(port);
        return users.find(id) == users.end() ? false : true;
    }
    void broadcastMessage(int sockfd, const string &ip, const uint16_t &port, const string &message)
    {
        for (auto &user : users)
        {
            struct sockaddr_in client;
            bzero(&client, sizeof(client));

            client.sin_family = AF_INET;
            client.sin_port = htons(user.second.port());
            client.sin_addr.s_addr = inet_addr(user.second.ip().c_str());
            
            string s = ip + "-" + to_string(port) + "#";
            s += message;
            
            sendto(sockfd, s.c_str(), s.size(), 0, (struct sockaddr *)&client, sizeof(client));
        }
    }

private:
    unordered_map<string, User> users;
}; // 类定义结束