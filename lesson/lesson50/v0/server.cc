#include "server.hpp"
#include "protocal.hpp"
#include <memory>

bool gget(const httprequest& req, httpresponse& resp)
{
  cout<< "------------------------------" <<endl;
  cout<< req.inbuffer <<endl; 
  cout<< "------------------------------" <<endl;
  return true;
}




int main(int argc, char* argv[])
{
  
  uint16_t port = atoi(argv[1]);
  unique_ptr<server> ser(new server(gget,port));
  ser->init(); 
  ser->start();





  return 0;
}
