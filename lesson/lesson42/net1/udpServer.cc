#include "udpServer.hpp"
#include <unordered_map>
#include <memory>
#include <fstream>

using namespace std;
using namespace Serve;

static void Usage(string proc)
{
  cout<< "Usage:\n \t  "<< proc << "  local_port\n\n";
}

// ./udpServe port 
//  getopt()

unordered_map<string, string> dict;
const std::string dictTxt="./dict.txt";

static bool cutString(const string &target, string *s1, string *s2, const string &sep)
{
    //apple:苹果
    auto pos = target.find(sep);
    if(pos == string::npos) return false;
    *s1 = target.substr(0, pos); //[)
    *s2 = target.substr(pos + sep.size()); //[）
    return true;
}

static void initDict()
{
    ifstream in(dictTxt, std::ios::binary);
    if(!in.is_open())
    {
        cerr << "open file " << dictTxt << " error" << endl;
        exit(OPEN_ERR);
    }
    string line;
    string key, value;
    while(getline(in, line))
    {
        // cout << line << endl;
        if(cutString(line, &key, &value, ":"))
        {
            dict.insert(make_pair(key, value));
        }
    }
    in.close();
    cout << "load dict success" << endl;
}

static void debugPrint()
{
    for(auto &dt : dict)
    {
        cout << dt.first << " # " << dt.second<< endl;
    }
}

void handlerMessage(int sockfd, string clientip, uint16_t clientport, string message)
{
    // 就可以对message进行特定的业务处理，而不关心message怎么来的 ---- server通信和业务逻辑解耦！
    // 婴儿版的业务逻辑
    string response_message;
    auto iter = dict.find(message);
    if(iter == dict.end()) response_message = "unknown";
    else response_message = iter->second;

    // 开始返回
    struct sockaddr_in client;
    bzero(&client, sizeof(client));

    client.sin_family = AF_INET;
    client.sin_port = htons(clientport);
    client.sin_addr.s_addr = inet_addr(clientip.c_str());

    sendto(sockfd, response_message.c_str(), response_message.size(), 0, (struct sockaddr*)&client, sizeof(client)); // 
}


int main(int argc, char* argv[])
{
  if(argc != 2)
  {
    Usage(argv[0]);
    exit(USAGE_ERR);
  }

  uint16_t port = atoi(argv[1]);
  std::unique_ptr<udpServe> usvr(new udpServe(handlerMessage, port));
  initDict();

  usvr->initserve();
  usvr->start();

  return 0;
}
