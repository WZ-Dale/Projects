#include "server.hpp"

P2PServer::P2PServer(){
    // 判断共享目录是否存在，若不存在则创建
    if(!bf::exists(SHAREDFILE)){
    bf::create_directory(SHAREDFILE);
    }
}
bool P2PServer::Start(uint16_t port){
    _server.Get("/hostpair", GetHostPair);
    _server.Get("/list", GetFileList);
    _server.Get("/list/(.*)", GetFileData);
    _server.listen("0.0.0.0", port);
    return true;
}

/* 响应配对请求 */
void P2PServer::GetHostPair(const Request &req, Response &rsp){}
/* 响应文件列表请求 */
void P2PServer::GetFileList(const Request &req, Response &rsp){}
/* 响应文件下载请求 */
void P2PServer::GetFileData(const Request &req, Response &rsp){}
/* 分块分析 */
bool P2PServer::RangeParse(std::string &range_val, int64_t &start, int64_t &len){}
