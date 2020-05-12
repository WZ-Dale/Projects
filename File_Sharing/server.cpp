#include <iostream>
#include <fstream>
#include <stdio.h>
#include <boost/filesystem.hpp>

#include "httplib.h"

using namespace httplib;
namespace bf = boost::filesystem;

#define SHAREDFILE "Shared"
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
      std::stringstream body;
      //body << "<html><body>";
      for(; item_begin != item_end; ++item_begin){
        // 判断是否是目录，如果是目录递归进入
        if(bf::is_directory(item_begin->status())){
          continue;
        }
        //std::string path = item_begin->path().string();
        //std::string name = item_begin->path().filename().string();
        bf::path path = item_begin->path();
        std::string name = path.filename().string();
        //std::cerr << path << std::endl;
        rsp.body += name + "\n";
        //body << "<h3><a href='/list/" << name << "'>";
        //body << name;
        //body << "</a></h3>";
      }
      //body << "</body></html>";
      //rsp.body = body.str();
      rsp.set_header("Content-Type", "text/html");
      rsp.status = 200;
      //rsp.set_content(&body[0], body.size(), "text/plain");
    }
    static void GetFileData(const Request &req, Response &rsp){
      // /list/a.txt -> /Shared/a.txt
      bf::path path(req.path);
      std::stringstream name;
      name << SHAREDFILE << "/" + path.filename().string();// 常量字符串相加会出问题
      if(!bf::exists(name.str())){
        rsp.status = 404;
        return;
      }
      if(bf::is_directory(name.str())){
        rsp.status = 403;
        return;
      }
      std::ifstream file(name.str(), std::ios::binary); 
      if(!file.is_open()){
        std::cerr << "openfile " << name.str() << " failed!" << std::endl;
        rsp.status = 404;// 访问资源不存在
        return;
      }
      int64_t fsize = bf::file_size(name.str());
      rsp.body.resize(fsize);
      file.read(&rsp.body[0], fsize);
      if(!file.good()){
        std::cerr << "read file " << name.str() << " body error!" << std::endl;
        rsp.status = 500;// 服务器内部错误
        return;
      }
      file.close();
      rsp.status = 200;
      rsp.set_header("Content-Type", "application/octet-stream");
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
