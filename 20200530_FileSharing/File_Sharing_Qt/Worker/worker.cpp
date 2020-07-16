#include "worker.hpp"

#include <QDebug>

Worker::Worker(QObject *parent) : QObject(parent){}
void Worker::doLongTimeWork()
{
    qDebug()<<__LINE__<<__FUNCTION__<<" - enter";
    qint64 count = 100;
    while(count--){
        QThread::msleep(10);
        qDebug()<<__LINE__<<__FUNCTION__<<"count = "<<count;
    }
    emit complete();
    qDebug()<<__LINE__<<__FUNCTION__<<" - leave";
}
