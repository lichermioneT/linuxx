#include "server.hpp"
#include "protocal.hpp"
#include <memory>
#include <fstream>
#include <sstream>

// 2.读取整个文件.html文件的信息。    
  static bool readFile(const string resource, string *out)    
  {    
    ifstream in(resource);    
    if(!in.is_open()) return false;    
    
    string line;    
    while(getline(in, line))    
    {    
      *out += line;    
    }    
    in.close();    
    
   return true;    
  }    
    

string suffixToDesc(const string& suffix)
{
    string ct = "Content-Type: ";

    if(suffix == ".html")
        ct += "text/html; charset=UTF-8";
    else if(suffix == ".png")
        ct += "image/png";
    else
        ct += "application/octet-stream";

    ct += "\r\n";
    return ct;
}

bool gett(const request& req, response& resp)
{
  /*
  cout << "客户端的请求报文如下:" << endl;
  cout << req.inbuffer << endl;
  */

  cout << "-----------------------" << endl;
  cout << "method: " << req.method << endl;
  cout << "url: " << req.url << endl;
  cout << "httpversion: " << req.httpversion << endl;
  cout << "-----------------------" << endl;

 string respline = "HTTP/1.1 200 OK \r\n";    
  string respheader = suffixToDesc(req.suffix);    
    
  if(req.size > 0)    
  {    
    respheader += "Content-Length: ";    
    respheader += to_string(req.size);    
    respheader += "\r\n";    
  }    
    
  string respblank = "\r\n";    
  string body;    
  if(!readFile(req.path, &body))                                                                                                                                                         
  {    
    readFile(html_404, &body);  // 失败就去默认路径读取文件的    
  }    
    
  resp.outbuffer += respline;    
  resp.outbuffer += respheader;    
  resp.outbuffer += respblank;    
      
  cout<< "response start------------------"<<endl;    
  cout<< resp.outbuffer <<endl;    
  cout<< "response end  ------------------"<<endl;    
    
  resp.outbuffer += body;    
  return true;  

  return true;
}


int main(int argc, char* argv[])
{
  (void)argc;
  uint16_t port = atoi(argv[1]);
  unique_ptr<server> ser(new server(port, gett));
  ser->init();
  ser->start();

  return 0;
}
