#include "client.hpp"

#include <QString>

P2PClient::P2PClient(uint16_t port, QObject *parent):_srv_port(port), QObject(parent){}
bool P2PClient::Start(){
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
        sleep(1);
    }
}

void P2PClient::client_read(int a)
{
    //QString aa = QString::number(a, 10);
    _choose = a;
}

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
    _str = "##################################################\n";
    _str += "1. 搜索附近主机\n";
    _str += "2. 显示在线主机\n";
    _str += "3. 显示在线列表\n";
    _str += "0. 退出\n";
    _str += "==============================\n";
    _str += "Please choose: ";
    emit client_emit(_str);
    sleep(3);
    _str += QString::number(_choose, 10);
    _str += "\n------------------------------\n";
    return _choose;
}
