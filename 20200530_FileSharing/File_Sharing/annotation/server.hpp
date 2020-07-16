#ifndef __M_SRV_H__
#define __M_SRV_H__

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
    /* httplib库中会将请求信息放入req对象中，并将响应信息放入rsp中 */
    /* 响应配对请求 */
    static void GetHostPair(const Request &req, Response &rsp){
      rsp.status = 200; // 剩余的信息不用管，httplib库中会进行响应填充操作到rsp
    }
    /* 响应文件列表请求 */
    static void GetFileList(const Request &req, Response &rsp){
      bf::directory_iterator item_begin(SHAREDFILE);  // 文件目录迭代器
      bf::directory_iterator item_end;
      //std::stringstream body;   // string流类型的正文body
      //body << "<html><body>";
      // 获该目录下的所有文件名，并添加到响应正文body中
      for(; item_begin != item_end; ++item_begin){
        if(bf::is_directory(item_begin->status())){ // 判断是否是目录，如果是目录递归进入
          continue;
        }
        //std::string path = item_begin->path().string();
        //std::string name = item_begin->path().filename().string();
        bf::path path = item_begin->path();           // 得到文件路径
        std::string name = path.filename().string();  // 从文件路径中提取出文件名
        //std::cerr << path << std::endl;
        rsp.body += name + "\n";                      // 文件名放入正文body中
        //body << "<h3><a href='/list/" << name << "'>";
        //body << name;
        //body << "</a></h3>";
      }
      //body << "</body></html>";
      //rsp.body = body.str();
      rsp.set_header("Content-Type", "text/html");  // 设置头部信息（键值对）
      rsp.status = 200;
      //rsp.set_content(&body[0], body.size(), "text/plain");
    }
    /* 响应文件下载请求 */
    static void GetFileData(const Request &req, Response &rsp){
      // /list/a.txt -> /Shared/a.txt
      bf::path path(req.path);  // 请求路径
      std::stringstream name;
      // 取出文件名并将路径组合成 /Shared/a.txt
      name << SHAREDFILE << "/" + path.filename().string(); // 常量字符串相加会出问题
      if(!bf::exists(name.str())){      // 如果该路径文件不存在，则返回404（请求的资源不存在）
        rsp.status = 404;
        return;
      }
      if(bf::is_directory(name.str())){ // 如果该路径是一个目录，则返回403（服务器理解请求客户端的请求，但是拒绝执行此请求）
        rsp.status = 403;
        return;
      }
      int64_t fsize = bf::file_size(name.str());  // 文件总长度
      if(req.method == "HEAD"){ // 如果请求方法是HEAD方法，就将文件总长度组织成键值对，并放入响应头信息中
        rsp.status = 200;
        std::string len = std::to_string(fsize);
        rsp.set_header("Content-Length", len.c_str());
        return;
      }
      else{                     // 不是HEAD方法就是，GET方法，将分块数据放入响应正文body中
        if(!req.has_header("Range")){ // 如果请求头信息中没有Range分块信息，则返回400（客户端请求的语法错误，服务器无法理解）
          rsp.status = 400;
          return;
        }
        std::string range_val = req.get_header_value("Range");  // 执行到此说明存在Range信息，则从头信息中获取该信息
        int64_t start, rlen;
        bool ret = RangeParse(range_val, start, rlen);  // 分块分析，从Range的值中提取出分块的开始位置和结束，便可得分块长度
        if(ret == false){   // 分块分析失败，则返回400（客户端请求的语法错误，服务器无法理解）
          rsp.status = 400;
          return;
        }
        std::cerr << "range: " << range_val << std::endl;
        std::cerr << "body.resize: " << rlen << std::endl;
        rsp.body.resize(rlen);  // resize响应正文body
        std::ifstream file(name.str(), std::ios::binary); // 二进制读方式打开该文件
        if(!file.is_open()){    // 若文件打开失败，则返回404（请求的资源不存在）
          std::cerr << "openfile " << name.str() << " failed!" << std::endl;
          rsp.status = 404;
          return;
        }
        file.seekg(start, std::ios::beg); // 跳转到指定位置读（把文件的读指针从文件开头向后移start个字节）
        file.read(&rsp.body[0], rlen);  // 读分块数据到body中
        if(!file.good()){   // 判断是否文件读完，这是文件流的一个函数，若没读完返回500（服务器内部错误）
          std::cerr << "read file " << name.str() << " body error!" << std::endl;
          rsp.status = 500;
          return;
        }
        file.close();
        rsp.status = 206;
        rsp.set_header("Content-Type", "application/octet-stream"); // application/octet-stream，只能提交二进制（https://tool.oschina.net/commons/）
        std::cerr << "file range " << range_val;
        std::cerr << " download success" << std::endl;
      }
    }
    /* 分块分析 */
    static bool RangeParse(std::string &range_val, int64_t &start, int64_t &len){
      // Range: bytes = start-end;
      size_t pos1 = range_val.find("=");
      size_t pos2 = range_val.find("-");
      if(pos1 == std::string::npos || pos2 == std::string::npos){   // 没有找到任何一个则报错
        std::cerr << "range " << range_val << " format error" << std::endl;
        return false;
      }
      int64_t end;
      std::string rstart = range_val.substr(pos1 + 1, pos2 - pos1 -1);  // 开始位置
      std::string rend = range_val.substr(pos2 + 1);                    // 结束位置
      std::stringstream tmp;  // 缓存string流
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

#endif
