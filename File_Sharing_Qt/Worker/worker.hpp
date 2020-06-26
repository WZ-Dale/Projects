#ifndef WORKER_HPP
#define WORKER_HPP

#include <QObject>
#include <QThread>

class Worker : public QObject
{
    Q_OBJECT
public:
    explicit Worker(QObject *parent = 0);
signals:
    void complete();
public slots:
    void doLongTimeWork();//耗时操作
};

#endif // WORKER_HPP
