#ifndef __M_CLI_H__
#define __M_CLI_H__

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
    /* 这里所说的主机在局域网中是被请求的，则相当于服务器端 */
    /* 获取局域网中所有主机地址 */
    bool GetAllHost(std::vector<std::string> &list){
      struct ifaddrs* addrs = NULL;
      getifaddrs(&addrs);     // 获取本机网卡信息
      for(; addrs != NULL; addrs = addrs->ifa_next){  // 有几个网卡就会循环几次
        struct sockaddr_in* ip = (struct sockaddr_in*)addrs->ifa_addr;  // 地址结构
        struct sockaddr_in* mask = (struct sockaddr_in*)addrs->ifa_netmask; // 子网掩码
        if(ip->sin_family != AF_INET){    // 不是IPv4，则跳过
          //addrs = addrs->ifa_next;
          continue;
        }
        if(ip->sin_addr.s_addr == inet_addr("127.0.0.1")){  // 是本地的回环网卡地址，则跳过
          //addrs = addrs->ifa_next;
          continue;
        }
        uint32_t net = ntohl(ip->sin_addr.s_addr & mask->sin_addr.s_addr);  // IP地址与子网掩码相与得到网络号，将网络字节序（大端字节序）转换为主机字节序（小端字节序）
        uint32_t max_host = ~(ntohl(mask->sin_addr.s_addr));  //子网掩码取反得到局域网中最大主机号
        for(int i = 2; i < max_host - 1; ++i){  // 循环最大主机号次，即局域网中所有主机号都会循环到，主机号全0（网络号）全1（UDP广播地址）的是特殊地址，主机号为1的地址是局域网网关地址
          struct in_addr ip;
          ip.s_addr = htonl(net + i); // 网络号 + 主机号 = 主机地址，再转为网络字节序
          list.push_back(inet_ntoa(ip));  // 将网络字节序的数字IP地址转换为点分十进制的字符串IP地址
        }
      }
      freeifaddrs(addrs);   // 释放网卡信息存储资源
      return true;
    }
    /* 局域网中所有主机向服务器发起请求配对，配对成功的就是在线主机 */
    void HostPair(std::string &i){
      Client client(i.c_str(), _srv_port); // 此处i是主机地址
      auto rsp = client.Get("/hostpair"); // 请求配对
      if(rsp && rsp->status == 200){    // 若配对成功，则将成功的主机地址放入在线主机列表中
        std::cerr << "host " << i << " pair success" << std::endl;
        _online_list.push_back(i);
      }
      std::cerr << "host " << i << " pair failed" << std::endl;
      //fprintf(stderr, "%s\n", i.c_str());
      return;
    }
    /* 获取在线主机列表（多线程配对），需要调用上面的函数HostPair */
    bool GetOnlineHost(std::vector<std::string> &list){
      _online_list.clear();
      // 有多少个主机，就开多少个线程，用于配对
      std::vector<std::thread> thr_list(list.size());
      for(int i = 0; i < list.size(); ++i){
        std::thread thr(&P2PClient::HostPair, this, std::ref(list[i])); // 线程运行类的成员函数需要传入this指针作为第一个参数，线程传递参数的形参为引用，传递实参需要使用ref
        thr_list[i] = std::move(thr); // 线程对象无法直接赋值，移动的是操作句柄
      }
      for(int i = 0; i < thr_list.size(); ++i){
        thr_list[i].join();
      }
      return true;
    }
    /* 打印在线主机列表 */
    bool ShowOnlineHost(){
      for(int i = 0; i < _online_list.size(); ++i){
        std::cout << i << ". " << _online_list[i] << std::endl;
      }
      std::cout << "==============================" << std::endl;
      std::cout << "Please choose: ";
      fflush(stdout);
      std::cin >> _host_idx;  // 输入要选择的主机序号
      std::cout << "------------------------------" << std::endl;
      // 用户容易输入错误
      if(_host_idx < 0 || _host_idx > _online_list.size()){
        _host_idx = -1;
        std::cerr << "choose error" << std::endl;
        return false;
      }
      return true;
    }
    /* 获取文件列表 */
    bool GetFileList(){
      Client client(_online_list[_host_idx].c_str(), _srv_port);
      auto rsp = client.Get("/list");
      if(rsp && rsp->status == 200){
        boost::split(_file_list, rsp->body, boost::is_any_of("\n"));  // 将body中的字符串按照'\n'进行切分，并将切分结果放入list
      }
      return true;
    }
    /* 打印文件列表 */
    bool ShowFileList(std::string &name){
      for(int i = 0; i < _file_list.size(); ++i){
        std::cout << i << ". " << _file_list[i] << std::endl;
      }
      std::cout << "==============================" << std::endl;
      std::cout << "Please choose: ";
      fflush(stdout);
      std::cin >> _file_idx;  // 输入要下载的文件序号
      std::cout << "------------------------------" << std::endl;
      // 用户容易输入错误
      if(_file_idx < 0 || _file_idx > _file_list.size()){
        _file_idx = -1;
        std::cerr << "choose error" << std::endl;
        return false;
      }
      name = _file_list[_file_idx];
      return true;
    }
    /* 分块下载 */
    void RangeDownload(std::string host, std::string name, int64_t start, int64_t end, int *res){
      std::string uri = "/list/" + name;
      std::string realpath = "Download/" + name;
      std::stringstream range_val;    // Range的值
      range_val << "bytes=" << start << "-" << end;
      std::cerr << "download range；" << range_val.str() << std::endl;
      *res = 0;
      Client client(host.c_str(), _srv_port);
      // Range: bytes = start-end;
      Headers header;   // 查看httplib中Headers是一个multimap
      header.insert(std::make_pair("Range", range_val.str().c_str()));    // 在头部插入Range的键值对
      auto rsp = client.Get(uri.c_str(), header);
      //std::cout << rsp->status << std::endl;
      if(rsp && rsp->status == 206){  // 分块处理成功
        int fd = open(realpath.c_str(), O_CREAT | O_WRONLY, 0664); // 创建和写入，并给权限
        if(fd < 0){
          std::cerr << "file " << realpath << " open error" << std::endl;
          return;
        }
        lseek(fd, start, SEEK_SET);   // 跳转到指定位置
        int ret = write(fd, &rsp->body[0], rsp->body.size());   // 将传输过来的分块的body写入到fd所指文件中
        if(ret < 0){
          std::cerr << "file " << realpath << " write error" << std::endl;
          close(fd);  // 任何可能退出的位置都要关闭文件
          return;
        }
        close(fd);
        *res = 1;     // 当前分块写入（下载）成功
        std::cerr << realpath << " download range: ";
        std::cerr << range_val.str() << " success" << std::endl;
      }
      else{                         // 分块下载失败
        std::cerr << realpath << " download range: ";
        std::cerr << range_val.str() << " falied" << std::endl;
      }
      return;
    }
    /* 获取文件总长度 */
    int64_t GetFileSize(std::string &host, std::string &name){
      int64_t fsize = -1;
      std::string uri = "/list/" + name;
      Client client(host.c_str(), _srv_port);
      auto rsp = client.Head(uri.c_str());  // HEAD方法只要响应头部信息，不要正文
      if(rsp && rsp->status == 200){
        if(!rsp->has_header("Content-Length")){ // 头部信息中无正文长度，则返回
          return -1;
        }
        std::string len = rsp->get_header_value("Content-Length");  // 获取响应头部信息中的正文长度
        std::stringstream tmp;  // 定义一个缓冲流，用于将string类型的len转换为string流类型的fsize
        tmp << len;
        tmp >> fsize;
      }
      return fsize;
    }
    /* 下载文件 */
    bool DownloadFile(std::string &name){
      // 请求首行：GET /list/filename HTTP/1.1
      // 响应首行：HTTP/1.1 200 OK
      // 1. 获取文件总长度
      // 2. 根据文件总长度和分块大小分割线程的下载区域
      // 3. 创建线程下载指定文件的指定分块数据
      // 4. 同步等待所有线程结束，获取下载结果
      std::string host = _online_list[_host_idx];
      int64_t fsize = GetFileSize(host, name);  // 获取文件总长度
      if(fsize < 0){
        std::cerr << "download file " << name << " failed" << std::endl;
        return false;
      }
      // 开始分块
      int count = fsize / RANGE_SIZE;   // 分块数
      std::cerr << "file size: " << fsize << std::endl;
      std::cerr << "range count: " << count << std::endl;
      // 有多少个分块，就开多少个线程
      std::vector<boost::thread> thr_list(count + 1);
      std::vector<int> res_list(count + 1);   // 用于统计每个每块是否下载成功
      bool ret = true;
      // 开始循环下载每一个分块
      for(int64_t i = 0; i <= count; ++i){
        int64_t start, end, rlen;
        start = i * RANGE_SIZE;         // 下载的分块开始位置
        end = start + RANGE_SIZE - 1;   // 下载的分块结束位置
        if(i == count){     // 如果是最后一块，需特别注意
          if(fsize % RANGE_SIZE == 0){  // 最后一块为0
            break;
          }
          end = fsize - 1;              // 最后一块不满一个RANGE_SIZE，则最后一个分块的结束位置需特别注意
        }
        std::cerr << "range: " << start << "-" << end << std::endl;
        rlen = end - start + 1;         // 当前分块的长度
        // Range: bytes = start-end;
        int* res = &res_list[i];        // 用于修改res_list的每个元素
        boost::thread thr(&P2PClient::RangeDownload, this, host, name, start, end, res);  // 创建线程开始下载该分块
        thr_list[i] = std::move(thr);   // 线程对象无法直接赋值，移动的是操作句柄
        //res_list[i] = std::move(res);
        //thr.join();
        //if(res == false){
        //  ret = false;
        //}
      }
      for(int i = 0; i <= count; ++i){
        if(i == count && fsize % RANGE_SIZE == 0){ // 没有最后一个分块，则不需要等待（最后一个分块为0在上面break出去了，并没有创建线程）
          break;
        } 
        thr_list[i].join();
        if(res_list[i] == 0){   // 如果有一个统计为0，则整个文下载失败
          std::cerr << "range " << i << " download falied" << std::endl; 
          ret = false;
        }
      }
      if(ret == true){    // 下载成功
        std::cerr << "download file " << name << " success" << std::endl;
      }
      else{               // 下载失败
        std::cerr << "download file " << name << " failed" << std::endl;
        return false;
      }
      return true;
    }
    /* 菜单，选择 */
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

#endif
