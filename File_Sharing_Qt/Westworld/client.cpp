#include "client.hpp"

P2PClient::P2PClient(uint16_t port, QObject *parent):_srv_port(port), QObject(parent){
    qDebug() << "client构造函数ID:" << QThread::currentThreadId();
}
bool P2PClient::Start(){
    while(1){
        std::vector<std::string> list;
        std::string filename;
        _choose = -1;
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
    _choose = a;
}
/* 这里所说的主机在局域网中是被请求的，则相当于服务器端 */
/* 获取局域网中所有主机地址 */
bool P2PClient::GetAllHost(std::vector<std::string> &list){
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
    freeifaddrs(addrs);   // 释放网卡信息 存储资源
    return true;
}
/* 局域网中所有主机向服务器发起请求配对，配对成功的就是在线主机 */
void P2PClient::HostPair(std::string &i){
    Client client(i.c_str(), _srv_port); // 此处i是主机地址
    auto rsp = client.Get("/hostpair"); // 请求配对
    _str = "host ";
    _str += QString::fromStdString(i);
    if(rsp && rsp->status == 200){    // 若配对成功，则将成功的主机地址放入在线主机列表中
        _str += " pair success\n";
        _online_list.push_back(i);
    }
    else{
        _str += " pair failed\n";
    }
    emit client_emit(_str);
    return;
}
/* 获取在线主机列表（多线程配对），需要调用上面的函数HostPair */
bool P2PClient::GetOnlineHost(std::vector<std::string> &list){
    _str = "该主机所在的局域网中最多有 ";
    _str += QString::number(list.size(), 10);
    _str += " 台主机，开始配对：\n";
    emit client_emit(_str);
    _online_list.clear();
//    // 有多少个主机，就开多少个线程，用于配对
//    std::vector<std::thread> thr_list(list.size());
//    for(int i = 0; i < list.size(); ++i){
//        std::thread thr(&P2PClient::HostPair, this, std::ref(list[i])); // 线程运行类的成员函数需要传入this指针作为第一个参数，线程传递参数的形参为引用，传递实参需要使用ref
//        thr_list[i] = std::move(thr); // 线程对象无法直接赋值，移动的是操作句柄
//    }
//    for(int i = 0; i < thr_list.size(); ++i){
//        thr_list[i].join();
//    }
    for(auto& i : list){
        HostPair(i);
    }
    return true;
}
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
    _str += "Please choose:\n";
    emit client_emit(_str);
    while(_choose == -1){
        _str = "Please choose:\n";
        emit client_emit(_str);
        sleep(5);
    }
    _str = QString::number(_choose, 10);
    _str += "\n------------------------------\n";
    emit client_emit(_str);
    return _choose;
}

