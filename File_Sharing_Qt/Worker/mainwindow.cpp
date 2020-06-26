#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButtonDoWork_clicked()
{
    Worker* worker = new Worker();
    QThread* thread = new QThread();
    //当线程启动时，执行Worker类的耗时函数doLongTimeWork()
    connect(thread,SIGNAL(started()),worker,SLOT(doLongTimeWork()));
    //当耗时函数执行完毕，发出complete()信号时，删除worker实例
    connect(worker,SIGNAL(complete()),worker,SLOT(deleteLater()));
    //当worker对象实例销毁时，退出线程
    connect(worker,SIGNAL(destroyed(QObject*)),thread,SLOT(quit()));
    //当线程结束时，销毁线程对象实例
    connect(thread,SIGNAL(finished()),thread,SLOT(deleteLater()));
    //移动worker对象实例到线程中
    worker->moveToThread(thread);
    //启动线程
    thread->start();
}
