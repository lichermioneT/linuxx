#include "server.hpp"
#include "protocal.hpp"
#include <memory>

bool gget(const httprequest& req, httpresponse& resp)
{
  cout<< "------------------------------" <<endl;
  cout<< req.inbuffer <<endl; 
  cout<< "method:         " << req.method << endl;
  cout<< "url:            " <<req.url <<endl;
  cout<< "httpvsersion:   " << req.httpvsersion <<endl;
  cout<< "paht            " << req.path <<endl;
  cout<< "------------------------------" <<endl;

  string respline = "HTTP/1.1 200 OK \r\n";
  string respheader = "Content-Type: text/html\r\n";
  string respblank = "\r\n";
  string body = "<html><body><h1>Hello World</h1></body></html>";

  resp.outbuffer += respline;
  resp.outbuffer += respheader;
  resp.outbuffer += respblank;
  resp.outbuffer += body;

  return true;
}

int main(int argc, char* agrv[])
{
  (void)argc;
  uint16_t port = atoi(agrv[1]);
  unique_ptr<server> ser(new server(gget, port));
  ser->init(); 
  ser->start();
  return 0;

}
