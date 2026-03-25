#include "server.hpp"
#include "protocal.hpp"
#include <memory>


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

// 1.服务器和网络的分类， html 
// 2.url->/ web:根目录
// 3.正确给客户端放回资源类型的。首先我们自己要知道的。所以的资源都有后缀的。
bool gget(const httprequest& req, httpresponse& resp)
{
  cout<< "------------------------------" <<endl;
  cout<< req.inbuffer       <<endl; 
  cout<< "method:         " << req.method << endl;
  cout<< "url:            " << req.url <<endl;
  cout<< "httpvsersion:   " << req.httpvsersion <<endl;
  cout<< "paht            " << req.path <<endl;
  cout<< "suffix          " << req.suffix << endl;
  cout<< "size            " << req.size <<endl;
  cout<< "------------------------------" <<endl;

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
  if(!util::readFile(req.path, &body))
  {
    util::readFile(html_404, &body);  // 失败就去默认路径读取文件的
  }

  resp.outbuffer += respline;
  resp.outbuffer += respheader;
  resp.outbuffer += respblank;
  
  cout<< "response start------------------"<<endl;
  cout<< resp.outbuffer <<endl;
  cout<< "response end  ------------------"<<endl;

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
