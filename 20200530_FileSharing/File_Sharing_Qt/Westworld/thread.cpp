#include "thread.h"
#include <QDebug>

Thread::Thread()
{
    qDebug()<<"Thread构造函数ID:"<<QThread::currentThreadId();
}

Thread::~Thread()
{
    qDebug()<<"Thread析构函数ID:"<<QThread::currentThreadId();
}

void Thread::Thread_Fun()
{
    qDebug()<<"子线程功能函数ID:"<<QThread::currentThreadId();
    QThread::sleep(3);
    emit finished();
}

void Thread::Thread_Fun1()
{
    qDebug()<<"子线程功能函数ID1:"<<QThread::currentThreadId();
    QThread::sleep(3);
    emit finished();
}
