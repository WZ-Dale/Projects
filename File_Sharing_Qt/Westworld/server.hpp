#ifndef SERVER_HPP
#define SERVER_HPP


#include <iostream>
#include <fstream>
#include <stdio.h>
#include <boost/filesystem.hpp>

#include "httplib.h"

using namespace httplib;
namespace bf = boost::filesystem;

#define SHAREDFILE "Shared"
#define LOG(fmt, ...) fprintf(stderr, fmt, __VA_ARGS__)

#include <QObject>
#include <QThread>
#include <QString>

class P2PServer : public QObject
{
    Q_OBJECT
public:
    explicit P2PServer(QObject *parent = 0);
signals:
    void complete();
    void server_emit(QString);
public slots:
    bool Start(uint16_t port);

private:
    /* httplib库中会将请求信息放入req对象中，并将响应信息放入rsp中 */
    /* 响应配对请求 */
    static void GetHostPair(const Request &req, Response &rsp);
    /* 响应文件列表请求 */
    static void GetFileList(const Request &req, Response &rsp);
    /* 响应文件下载请求 */
    static void GetFileData(const Request &req, Response &rsp);
    /* 分块分析 */
    static bool RangeParse(std::string &range_val, int64_t &start, int64_t &len);
private:
    Server _server;

    QString _str;
};


#endif // SERVER_HPP
