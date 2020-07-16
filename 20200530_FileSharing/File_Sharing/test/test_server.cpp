// 使用httplib实现一个最简单的HTTP服务器
// 并了解httplib的最基本使用
#include "httplib.h"

using namespace httplib;

void Excellent(const Request &req, Response &rsp){
  rsp.status = 302;
  rsp.set_header("Location", "http://www.baidu.com");
  rsp.body = "<html><h1>Excellent!!</h4></html>";
  return;
}


int main(){
  Server server;
  server.Get("/", Excellent);
  server.listen("0.0.0.0", 9000);

  return 0;
}
