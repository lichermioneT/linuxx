#include "udpclient.hpp"

udpclient::udpclient(const string& ip, uint16_t port)
  :_ip(ip)
  ,_port(port)
  ,_sock(-1)
{}

udpclient::~udpclient()
{
  if(_sock >= 0)
  {
    close(_sock);
  }

  _sock = -1;
}

bool udpclient::createSocket()
{

  int sock = socket(AF_INET, SOCK_DGRAM, 0);
  if(sock < 0)
  {
    perror("sock failed");
    return false;
  }

  return true;
}

void udpclient::Send()
{
  struct sockaddr_in server;
  bzero(&server, sizeof(server));

  server.sin_family = AF_INET;
  if(inet_pton(AF_INET, _ip.c_str(), &server.sin_addr) < 0)
  {
    perror("inet_pton failed");
    return;
  }
  server.sin_port = htons(_port);

  for(;;)
  {
    char buffer[1024] = {0};
    char inbuffer[1024] = {0};
    cout<< "请输入:";
    cin.getline(buffer, 1024);
    
    sendto(_sock, buffer, sizeof(buffer), 0, (struct sockaddr*)&server, sizeof(server));
    recvfrom(_sock, inbuffer, sizeof(inbuffer) - 1, 0, nullptr, 0);
    cout<< inbuffer <<endl;
  }
}

void udpclient::init()
{
  createSocket();
}

void udpclient::start()
{
  Send();
}

