#include "server.hpp"

/* 响应配对请求 */
void P2PServer::GetHostPair(const Request &req, Response &rsp){}
/* 响应文件列表请求 */
void P2PServer::GetFileList(const Request &req, Response &rsp){}
/* 响应文件下载请求 */
void P2PServer::GetFileData(const Request &req, Response &rsp){}
/* 分块分析 */
bool P2PServer::RangeParse(std::string &range_val, int64_t &start, int64_t &len){}
