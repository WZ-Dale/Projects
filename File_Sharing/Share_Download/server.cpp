#include <iostream>
#include <stdio.h>
#include <boost/filesystem.hpp>

#include "httplib.h"

using namespace httplib;
namespace bf = boost::filesystem;

#define SHAREDFILE "Share_Download"
#define LOG(fmt, ...) fprintf(stderr, fmt, __VA_ARGS__)

class P2PServer
{
  private:
    Server _server;
  private:
    static void GetHostPair(const Request &req, Response &rsp){
      rsp.status = 200;
    }
    static void GetFileList(const Request &req, Response &rsp){
      bf::directory_iterator item_begin(SHAREDFILE);
      bf::directory_iterator item_end;
      //std::string body;
      for(; item_begin != item_end; ++item_begin){
        // 判断是否是目录，如果是目录递归进入
        if(bf::is_directory(item_begin->status())){
          continue;
        }
        std::string path = item_begin->path().string();
        std::cerr << path << std::endl;
        rsp.body += path + "\n";
      }
      rsp.status = 200;
      //rsp.set_content(&body[0], body.size(), "text/plain");
    }
    static void GetFileData(const Request &req, Response &rsp){
    
    }
  public:
    P2PServer(){
      // 判断共享目录是否存在，若不存在则创建
      if(!bf::exists(SHAREDFILE)){
        bf::create_directory(SHAREDFILE);
      }
    }
    bool Start(uint16_t port){
      _server.Get("/hostpair", GetHostPair);
      _server.Get("/list", GetFileList);
      _server.Get("/list/(.*)", GetFileData);
      _server.listen("0.0.0.0", port);
      return true;
    }
};

int main(){
  P2PServer srv;
  srv.Start(9000);
  return 0;
}
