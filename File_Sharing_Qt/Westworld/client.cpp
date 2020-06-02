#include "client.hpp"

#include <QString>

/* 获取局域网中所有主机地址 */
bool P2PClient::GetAllHost(std::vector<std::string> &list){}
/* 局域网中所有主机向服务器发起请求配对，配对成功的就是在线主机 */
void P2PClient::HostPair(std::string &i){}
/* 获取在线主机列表（多线程配对），需要调用上面的函数HostPair */
bool P2PClient::GetOnlineHost(std::vector<std::string> &list){}
/* 打印在线主机列表 */
bool P2PClient::ShowOnlineHost(){}
/* 获取文件列表 */
bool P2PClient::GetFileList(){}
/* 打印文件列表 */
bool P2PClient::ShowFileList(std::string &name){}
/* 分块下载 */
void P2PClient::RangeDownload(std::string host, std::string name, int64_t start, int64_t end, int *res){}
/* 获取文件总长度 */
int64_t P2PClient::GetFileSize(std::string &host, std::string &name){}
/* 下载文件 */
bool P2PClient::DownloadFile(std::string &name){}
/* 菜单，选择 */
int P2PClient::DoFile(){
    QString str = "##################################################";
}
