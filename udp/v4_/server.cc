#include "server.hpp"
#include "user.hpp"
#include <stdio.h>
#include <signal.h>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <memory>
using namespace std;

static void usage(string proc)
{
  cout<< "usage:" << proc << "loacl_port" <<endl;
  exit(1);
}


//1.创建socket
//2.绑定socket
//3.接收数据
//4.返回信息

const string dictTxt = "./dict.txt";
unordered_map<string, string> dict;

static bool cutString(const string& target, string* key, string* value)
{
  string sep = ":";
  auto pos = target.find(sep);
  if(pos == string::npos)
  {
    return false;
  }

  *key = target.substr(0, pos);  // [0,  pos);
  *value  = target.substr(pos + sep.size());

  return true;
}

static void initDict()
{
  ifstream in(dictTxt, std::ios::binary);
  if(!in.is_open())
  {
    cout<< "open file:" << dictTxt << " error" <<endl;
    exit(3);
  }

  string line;
  string key,value;
  while(getline(in, line))
  {
    if(cutString(line, &key, &value))
    {
      dict.insert(make_pair(key, value));
    }
  }

  in.close();
  cout<< "dict load success" <<endl;
}

static void debug_print()
{
  for(auto &dt : dict)
  {
    cout<< dt.first << "#" << dt.second <<endl;
  }
}

void handerMessage(int socket, string clietip,  uint16_t clietport, string message)
{
    string reponse_message;
    auto iter  = dict.find(message);
    if(iter == dict.end()) reponse_message = "unkown";
    else reponse_message = iter->second;

    struct  sockaddr_in client;
    bzero(&client, sizeof(client));

    client.sin_family = AF_INET;
    client.sin_addr.s_addr = inet_addr(clietip.c_str());
    client.sin_port = htons(clietport);

    sendto(socket, reponse_message.c_str(), reponse_message.size(), 0, (struct sockaddr*)&client, sizeof(client));
}

void reload(int signo)
{
  (void)signo; 
  initDict();
}
void exe(int socket, string clietip,  uint16_t clietport, string cmd)
{
  //1.cmd:ls -a -l;
  
  string reponse_message;
  FILE* fp = popen(cmd.c_str(), "r");
  if(fp == nullptr) reponse_message = cmd + "exec failed";
  
  char line[1024] = {0};
  while(fgets(line, sizeof(line), fp))
  {
    reponse_message += line;
  }

  fclose(fp);

    struct  sockaddr_in client;
    bzero(&client, sizeof(client));

    client.sin_family = AF_INET;
    client.sin_addr.s_addr = inet_addr(clietip.c_str());
    client.sin_port = htons(clietport);

    sendto(socket, reponse_message.c_str(), reponse_message.size(), 0, (struct sockaddr*)&client, sizeof(client));
}

onlineUser onlineuser;

void routemessage(int socket, string clietip,  uint16_t clietport, string message)
{
  if(message == "online") onlineuser.addUser(clietip, clietport);
  if(message == "offline") onlineuser.delUser(clietip, clietport);
  if(onlineuser.isOline(clietip, clietport)) 
  {
    // 消息的路由：
    onlineuser.broadcast(socket, message, clietip, clietport);
  }
  else 
  {
    struct  sockaddr_in client;
    string reponse_message;
    bzero(&client, sizeof(client));

    client.sin_family = AF_INET;
    client.sin_addr.s_addr = inet_addr(clietip.c_str());
    client.sin_port = htons(clietport);

    reponse_message = "您还没有上线，请先上线：运行online";
    sendto(socket, reponse_message.c_str(), reponse_message.size(), 0, (struct sockaddr*)&client, sizeof(client));
  }
}


// .udpserver ip port
int main(int argc, char* argv[])
{
  if(argc != 2)
  {
    usage(argv[0]);
  }


  uint16_t port = atoi(argv[1]);

  unique_ptr<server> usvr(new server(routemessage,port));
  usvr->init(); // bind设置进内核里面去
  usvr->start();
  return 0;
}
