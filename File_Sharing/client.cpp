#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <thread>
#include <stdio.h>
#include <ifaddrs.h>
#include <fcntl.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <boost/filesystem.hpp>
#include <boost/thread.hpp>
#include <boost/algorithm/string.hpp>

#include "httplib.h"

//#define RANGE_SIZE (1024*1024*100)
#define RANGE_SIZE (100<<20)
#define DOWNLOAD "Download"

using namespace httplib;
namespace bf = boost::filesystem;

class P2PClient
{
  private:
    uint16_t _srv_port;
    int _host_idx;
    int _file_idx;
    std::vector<std::string> _online_list;
    std::vector<std::string> _file_list;
  private:
    bool GetAllHost(std::vector<std::string> &list){
      struct ifaddrs* addrs = NULL;
      getifaddrs(&addrs);
      for(; addrs != NULL; addrs = addrs->ifa_next){
        struct sockaddr_in* ip = (struct sockaddr_in*)addrs->ifa_addr;
        struct sockaddr_in* mask = (struct sockaddr_in*)addrs->ifa_netmask;
        if(ip->sin_family != AF_INET){
          //addrs = addrs->ifa_next;
          continue;
        }
        if(ip->sin_addr.s_addr == inet_addr("127.0.0.1")){
          //addrs = addrs->ifa_next;
          continue;
        }
        uint32_t net = ntohl(ip->sin_addr.s_addr & mask->sin_addr.s_addr);
        uint32_t max_host = ~(ntohl(mask->sin_addr.s_addr));
        for(int i = 2; i < max_host - 1; ++i){
          struct in_addr ip;
          ip.s_addr = htonl(net + i);
          list.push_back(inet_ntoa(ip));
        }
      }
      freeifaddrs(addrs);
      return true;
    }
    void HostPair(std::string &i){
      Client client(i.c_str(), _srv_port); 
      auto rsp = client.Get("/hostpair");
      if(rsp && rsp->status == 200){
        std::cerr << "host " << i << " pair success" << std::endl;
        _online_list.push_back(i);
      }
      std::cerr << "host " << i << " pair failed" << std::endl;
      //fprintf(stderr, "%s\n", i.c_str());
      return;
    }
    bool GetOnlineHost(std::vector<std::string> &list){
      _online_list.clear();
      std::vector<std::thread> thr_list(list.size());
      for(int i = 0; i < list.size(); ++i){
        std::thread thr(&P2PClient::HostPair, this, std::ref(list[i]));
        thr_list[i] = std::move(thr);
      }
      for(int i = 0; i < thr_list.size(); ++i){
        thr_list[i].join();
      }
      return true;
    }
    bool ShowOnlineHost(){
      for(int i = 0; i < _online_list.size(); ++i){
        std::cout << i << ". " << _online_list[i] << std::endl;
      }
      std::cout << "==============================" << std::endl;
      std::cout << "Please choose: ";
      fflush(stdout);
      std::cin >> _host_idx;
      std::cout << "------------------------------" << std::endl;
      if(_host_idx < 0 || _host_idx > _online_list.size()){
        _host_idx = -1;
        std::cerr << "choose error" << std::endl;
        return false;
      }
      return true;
    }
    bool GetFileList(){
      Client client(_online_list[_host_idx].c_str(), _srv_port);
      auto rsp = client.Get("/list");
      if(rsp && rsp->status == 200){
        boost::split(_file_list, rsp->body, boost::is_any_of("\n"));
      }
      return true;
    }
    bool ShowFileList(std::string &name){
      for(int i = 0; i < _file_list.size(); ++i){
        std::cout << i << ". " << _file_list[i] << std::endl;
      }
      std::cout << "==============================" << std::endl;
      std::cout << "Please choose: ";
      fflush(stdout);
      std::cin >> _file_idx;
      std::cout << "------------------------------" << std::endl;
      if(_file_idx < 0 || _file_idx > _file_list.size()){
        _file_idx = -1;
        std::cerr << "choose error" << std::endl;
        return false;
      }
      name = _file_list[_file_idx];
      return true;
    }
    void RangeDownload(std::string host, std::string name, int64_t start, int64_t end, bool *res){
      std::string uri = "/list/" + name;
      std::string realpath = "Download/" + name;
      std::stringstream range_val;
      range_val << "bytes=" << start << "-" << end;
      std::cerr << "download range；" << range_val.str() << std::endl;
      *res = false;
      Client client(host.c_str(), _srv_port);
      // Range: bytes = start-end;
      Headers header;
      header.insert(std::make_pair("Range", range_val.str().c_str()));
      auto rsp = client.Get(uri.c_str(), header);
      //std::cout << rsp->status << std::endl;
      if(rsp && rsp->status == 206){
        int fd = open(realpath.c_str(), O_CREAT | O_WRONLY, 0664); // 创建和写入，并给权限
        if(fd < 0){
          std::cerr << "file " << realpath << " open error" << std::endl;
          return;
        }
        lseek(fd, start, SEEK_SET); // 跳转到指定位置
        int ret = write(fd, &rsp->body[0], rsp->body.size());
        if(ret < 0){
          std::cerr << "file " << realpath << " write error" << std::endl;
          close(fd);
          return;
        }
        close(fd);
        *res = true;
        std::cerr << realpath << " download range: ";
        std::cerr << range_val.str() << " success" << std::endl;
      }
      else{
        std::cerr << realpath << " download range: ";
        std::cerr << range_val.str() << " falied" << std::endl;
      }
      return;
    }
    int64_t GetFileSize(std::string &host, std::string &name){
      int64_t fsize = -1;
      std::string uri = "/list/" + name;
      Client client(host.c_str(), _srv_port);
      auto rsp = client.Head(uri.c_str());
      if(rsp && rsp->status == 200){
        if(!rsp->has_header("Content-Length")){
          return -1;
        }
        std::string len = rsp->get_header_value("Content-Length");
        std::stringstream tmp;
        tmp << len;
        tmp >> fsize;
      }
      return fsize;
    }
    bool DownloadFile(std::string &name){
      // 请求首行：GET /list/filename HTTP/1.1
      // 响应首行：HTTP/1.1 200 OK
      // 1. 获取文件总长度
      // 2. 根据文件总长度和分块大小分割线程的下载区域
      // 3. 创建线程下载指定文件的指定分块数据
      // 4. 同步等待所有线程结束，获取下载结果
      std::string host = _online_list[_host_idx];
      int64_t fsize = GetFileSize(host, name);
      if(fsize < 0){
        std::cerr << "download file " << name << " failed" << std::endl;
        return false;
      }
      int count = fsize / RANGE_SIZE;
      std::cerr << "file size: " << fsize << std::endl;
      std::cerr << "range count: " << count << std::endl;
      std::vector<boost::thread> thr_list(count + 1);
      std::vector<bool> res_list(count + 1);
      bool ret = true;
      for(int64_t i = 0; i <= count; ++i){
        int64_t start, end, rlen;
        start = i * RANGE_SIZE;
        end = start + RANGE_SIZE - 1;
        if(i == count){ // 是最后一块
          if(fsize % RANGE_SIZE == 0){ //最后一块为0
            break;
          }
          end = fsize - 1;  // 最后一块不满
        }
        std::cerr << "range: " << start << "-" << end << std::endl;
        rlen = end - start + 1;
        // Range: bytes = start-end;
        bool res;
        boost::thread thr(&P2PClient::RangeDownload, this, host, name, start, end, &res);
        //thr_list[i] = std::move(thr);
        //res_list[i] = std::move(res);
        thr.join();
        if(res == false){
          ret = false;
        }
      }
      /*
      for(int i = 0; i <= count; ++i){
        if(i == count && fsize % RANGE_SIZE == 0){ // 没有最后一个分块
          break;
        } 
        thr_list[i].join();
        if(res_list[i] == false){
          ret = false;
        }
      }
      */
      if(ret == true){
        std::cerr << "download file " << name << " success" << std::endl;
      }
      else{
        std::cerr << "download file " << name << " failed" << std::endl;
        return false;
      }
      return true;
    }
    int DoFile(){
      std::cout << "##################################################" << std::endl;
      std::cout << "1. 搜索附近主机" << std::endl;
      std::cout << "2. 显示在线主机" << std::endl;
      std::cout << "3. 显示在线列表" << std::endl;
      std::cout << "0. 退出" << std::endl;
      int choose;
      std::cout << "==============================" << std::endl;
      std::cout << "Please choose: ";
      fflush(stdout);
      std::cin >> choose;
      std::cout << "------------------------------" << std::endl;
      return choose;
    }
  public:
    P2PClient(uint16_t port):_srv_port(port){}
    bool Start(){
      while(1){
        std::vector<std::string> list;
        std::string filename;
        int choose = DoFile();
        switch(choose){
          case 1:
            GetAllHost(list);
            GetOnlineHost(list);
            break;
          case 2:
            if(!ShowOnlineHost()){
              break;
            }
            GetFileList();
            break;
          case 3:
            if(!ShowFileList(filename)){
              break;
            }
            DownloadFile(filename);
            break;
          case 0:
            exit(0);
          default:
            break;
        }
      }
    }
};

int main(){
  P2PClient client(9000);
  client.Start();
  return 0;
}
