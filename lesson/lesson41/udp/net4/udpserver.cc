#include "udpserver.hpp"
#include "userManager.hpp"
#include <unordered_map>
#include <memory>
#include <fstream>
#include <signal.h>

using namespace std;
using namespace server;

static void usage()
{
  cout<< "usage ：" << "./udpserver local_port" <<endl;
}

const std::string dicttxt = "./dict.txt";
unordered_map<string, string> dict;
// apple:苹果
static bool cutString(const string& target, string* s1, string* s2, const string& sep)
{
  auto pos = target.find(sep);
  if(pos == string::npos) return false;
  *s1 = target.substr(0, pos);  // apple:苹果
  *s2 = target.substr(pos + sep.size()); // 苹果
  return true;
}

static void initDict()
{
  ifstream in(dicttxt, std::ios::binary);
  if(!in.is_open())
  {
    std::cerr<< "open_cerr : " << dicttxt <<endl;
    exit(OPEN_ERR);
  }

  string line;
  string key, value;
  while(getline(in, line))
  {
    // cout<< line <<endl;
    if(cutString(line, &key, &value, ":"))
    {
      dict.insert(make_pair(key, value));
    }
  }
  cout<< "load dict success"<<endl;
  in.close();
}

static void debugPrint()
{
  for(const auto& dt : dict)
  {
    cout<< dt.first << " # " << dt.second <<endl;
  }
}

void reload(int sig)
{
  (void)sig;
  initDict();
}
void handlerMessage(int socket, string clientip, uint16_t clientport, string message)
{
  string respose_message;
  auto iter = dict.find(message);
  if(iter == dict.end()) respose_message = "ukonwn";
  else respose_message = iter->second;

  // 构建开始返回
  struct sockaddr_in client;
  bzero(&client, sizeof(client));
  client.sin_family = AF_INET;
  client.sin_port = htons(clientport);
  client.sin_addr.s_addr = inet_addr(clientip.c_str());

  sendto(socket, respose_message.c_str(), respose_message.size(), 0, (struct sockaddr*)&client, sizeof(client));
}


void exeCommand(int socket, string clientip, uint16_t clientport, string cmd)
{

  if(cmd.find("rm") != string::npos)
  {
    cout<<"this error execution" <<endl;
  }

   string respose;
   FILE* fp = popen(cmd.c_str(), "r");
   if(fp == nullptr)
   {
     respose = cmd + "exex failde";
   }
  
   char line[1024];
   while(fgets(line, sizeof(line), fp))
   {
      respose += line;
   }
   pclose(fp);
  // 构建开始返回
  struct sockaddr_in client;
  bzero(&client, sizeof(client));
  client.sin_family = AF_INET;
  client.sin_port = htons(clientport);
  client.sin_addr.s_addr = inet_addr(clientip.c_str());

  sendto(socket, respose.c_str(), respose.size(), 0, (struct sockaddr*)&client, sizeof(client));
}

OnlineUser onlineuser;  // 创建全局 OnlineUser 对象

void routeMessage(int sockfd, string clientip, uint16_t clientport, string message)
{
    if (message == "online") onlineuser.addUser(clientip, clientport);
    if (message == "offline") onlineuser.delUser(clientip, clientport);

    if (onlineuser.isOnline(clientip, clientport))
    {
        // 消息的路由
        onlineuser.broadcastMessage(sockfd, clientip, clientport, message);
    }
    else
    {
        struct sockaddr_in client;
        bzero(&client, sizeof(client));

        client.sin_family = AF_INET;
        client.sin_port = htons(clientport);
        client.sin_addr.s_addr = inet_addr(clientip.c_str()); // 注意：原图中为clientp，应为clientip

        string response = "你还没有上线，请先上线，运行: online";

        sendto(sockfd, response.c_str(), response.size(), 0, (struct sockaddr *)&client, sizeof(client));
    }
}



int main(int agrc, char* argv[])
{
  if(agrc != 2)
  {
    usage();
    exit(1);
  }

  // initDict();
  // debugPrint();
  // signal(2,reload);

  uint16_t port = atoi(argv[1]);

  /*
   *std::unique_ptr<udpServe> usvr(new udpServe(handlerMessage, port));
   */
  // std::unique_ptr<udpServe> usvr(new udpServe(exeCommand, port));
  std::unique_ptr<udpServe> usvr(new udpServe(routeMessage, port));

  usvr->init();
  usvr->start();

  return 0;
}
