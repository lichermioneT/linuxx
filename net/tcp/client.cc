#include "client.hpp"


int main(int argc, char* argv[])
{
  (void)argc;
  string ip = argv[1];
  uint16_t port = atoi(argv[2]);

  client cli(ip, port);
  cli.CreateSocket();
  cli.Connect();



  return 0;
}
