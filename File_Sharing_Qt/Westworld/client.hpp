#ifndef CLIRNT_HPP
#define CLIRNT_HPP


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

#include <QObject>
#include <QString>

class Thread1;

class P2PClient : public QObject
{
    Q_OBJECT
//public:
//    P2PClient(uint16_t port);
//    bool Start();

public:
    explicit P2PClient(uint16_t port, QObject *parent = 0);
    bool Start();

signals:
    void client_emit(QString);
private slots:
    void client_read(int a);

private:
    /* 这里所说的主机在局域网中是被请求的，则相当于服务器端 */
    /* 获取局域网中所有主机地址 */
    bool GetAllHost(std::vector<std::string> &list);
    /* 局域网中所有主机向服务器发起请求配对，配对成功的就是在线主机 */
    void HostPair(std::string &i);
    /* 获取在线主机列表（多线程配对），需要调用上面的函数HostPair */
    bool GetOnlineHost(std::vector<std::string> &list);
    /* 打印在线主机列表 */
    bool ShowOnlineHost();
    /* 获取文件列表 */
    bool GetFileList();
    /* 打印文件列表 */
    bool ShowFileList(std::string &name);
    /* 分块下载 */
    void RangeDownload(std::string host, std::string name, int64_t start, int64_t end, int *res);
    /* 获取文件总长度 */
    int64_t GetFileSize(std::string &host, std::string &name);
    /* 下载文件 */
    bool DownloadFile(std::string &name);
    /* 菜单，选择 */
    int DoFile();
private:
    uint16_t _srv_port;
    int _host_idx;
    int _file_idx;
    std::vector<std::string> _online_list;
    std::vector<std::string> _file_list;

    QString _str;
    int _choose = -1;
};

/*****************************************************************/

class Thread1 : public QObject
{
    Q_OBJECT
public:
    Thread1();
    ~Thread1();
public slots:
    void srv_start();
    void cli_start();
};

#endif // CLIRNT_HPP
