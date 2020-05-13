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
      int64_t fsize = bf::file_size(name.str());
      if(req.method == "HEAD"){
        rsp.status = 200;
        std::string len = std::to_string(fsize);
        rsp.set_header("Content-Length", len.c_str());
        return;
      }
      else{ // 不是HEAD方法就是，GET方法
        if(!req.has_header("Range")){
          rsp.status = 400;
          return;
        }
        std::string range_val = req.get_header_value("Range");
        int64_t start, rlen;
        bool ret = RangeParse(range_val, start, rlen);
        if(ret == false){
          rsp.status = 400;
          return;
        }
        std::cerr << "range: " << range_val << std::endl;
        std::cerr << "body.resize: " << rlen << std::endl;
        rsp.body.resize(rlen);
        std::ifstream file(name.str(), std::ios::binary); 
        if(!file.is_open()){
          std::cerr << "openfile " << name.str() << " failed!" << std::endl;
          rsp.status = 404;// 访问资源不存在
          return;
        }
        file.seekg(start, std::ios::beg); // 指定位置读
        file.read(&rsp.body[0], rlen);
        if(!file.good()){
          std::cerr << "read file " << name.str() << " body error!" << std::endl;
          rsp.status = 500;// 服务器内部错误
          return;
        }
        file.close();
        rsp.status = 206;
        rsp.set_header("Content-Type", "application/octet-stream");
        std::cerr << "file range " << range_val;
        std::cerr << " download success" << std::endl;
      }
    }
    static bool RangeParse(std::string &range_val, int64_t &start, int64_t &len){
      // Range: bytes = start-end;
      size_t pos1 = range_val.find("=");
      size_t pos2 = range_val.find("-");
      if(pos1 == std::string::npos || pos2 == std::string::npos){
        std::cerr << "range " << range_val << " format error" << std::endl;
        return false;
      }
      int64_t end;
      std::string rstart = range_val.substr(pos1 + 1, pos2 - pos1 -1);
      std::string rend = range_val.substr(pos2 + 1);
      std::stringstream tmp;
      tmp << rstart;
      tmp >> start;
      tmp.clear();
      tmp << rend;
      tmp >> end;
      len = end - start + 1;
      return true;
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
