#include <iostream>
#include "udpclient.hpp"
#include <memory>

using namespace std;
using namespace client;

static void usage()
{
  cout<< "usage ：" << "./udpserver ip port" <<endl;
}

// ./udpclient ip port

int main(int argc, char* argv[])
{
  if(argc != 3)
  {
    usage();
    exit(1);
  }

  string serverip =  argv[1];
  uint16_t serverport = atoi(argv[2]);

  unique_ptr<udpclient> ucli(new udpclient(serverip, serverport));

  ucli->init();
  ucli->run();



  return 0;
}
