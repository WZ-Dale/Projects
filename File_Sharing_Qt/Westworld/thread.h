#ifndef THREAD_H
#define THREAD_H


#include <QObject>
#include <QThread>

class Thread :public QObject
{
    Q_OBJECT
public:
    Thread();
    ~Thread();
signals:
    void finished();//完成信号
public slots:
    void Thread_Fun(void);
    void Thread_Fun1(void);
};


#endif // THREAD_H
