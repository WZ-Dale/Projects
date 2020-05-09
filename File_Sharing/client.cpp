#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <stdio.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

#include "httplib.h"

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
          addrs = addrs->ifa_next;
          continue;
        }
        if(ip->sin_addr.s_addr == inet_addr("127.0.0.1")){
          addrs = addrs->ifa_next;
          continue;
        }
        uint32_t net = ntohl(ip->sin_addr.s_addr & mask->sin_addr.s_addr);
        uint32_t max_host = ~(ntohl(mask->sin_addr.s_addr));
        for(int i = 128; i < max_host; ++i){
          struct in_addr ip;
          ip.s_addr = htonl(net + i);
          list.push_back(inet_ntoa(ip));
        }
      }
      freeifaddrs(addrs);
      return true;
    }
    bool GetOnlineHost(std::vector<std::string> &list){
      for(const auto& i : list){
        Client client(&i[0], _srv_port); 
        auto rsp =  client.Get("/hostpair");
        if(rsp && rsp->status == 200){
          std::cerr << "host " << i << " pair success" << std::endl;
          _online_list.push_back(i);
          break;
          continue;
        }
        std::cerr << "host " << i << " pair failed" << std::endl;
      }
      return true;
    }
    bool ShowOnlineHost(){
      for(int i = 0; i < _online_list.size(); ++i){
        std::cout << i << ". " << _online_list[i] << std::endl;
      }
      std::cout << "Please choose: ";
      fflush(stdout);
      std::cin >> _host_idx;
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
      std::cout << "Please choose: ";
      fflush(stdout);
      std::cin >> _file_idx;
      if(_file_idx < 0 || _file_idx > _file_list.size()){
        _file_idx = -1;
        std::cerr << "choose error" << std::endl;
        return false;
      }
      name = _file_list[_file_idx];
      return true;
    }
    bool DownloadFile(std::string &name){
      return true;
    }
    int DoFile(){
      std::cout << "##################################################" << std::endl;
      std::cout << "1. 搜索附近主机" << std::endl;
      std::cout << "2. 显示在线主机" << std::endl;
      std::cout << "3. 显示在线列表" << std::endl;
      std::cout << "0. 退出" << std::endl;
      int choose;
      std::cout << "Please choose: ";
      fflush(stdout);
      std::cin >> choose;
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
