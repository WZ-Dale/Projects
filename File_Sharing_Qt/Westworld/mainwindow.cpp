#include "mainwindow.h"
#include "ui_mainwindow.h"

P2PServer server;
P2PClient client(9000);

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QString str = "Welcome to the Westworld !";
    ui->textBrowser->setText(str);

    QThread0 = new QThread;
    QThread1 = new QThread;
    thread0 = new Thread;
    thread1 = new Thread;
    connect(QThread0,SIGNAL(started()),thread0,SLOT(srv_start()));
    connect(QThread0,SIGNAL(finished()),thread0,SLOT(deleteLater()));
    //connect(thread_class,SIGNAL(clear()),thread0,SLOT(deleteLater()));
    connect(QThread1,SIGNAL(started()),thread1,SLOT(cli_start()));
    connect(QThread1,SIGNAL(finished()),thread1,SLOT(deleteLater()));
    thread0->moveToThread(QThread0);
    thread1->moveToThread(QThread1);
    QThread0->start();
    QThread1->start();

    connect(&client,SIGNAL(client_emit(QString)),this,SLOT(UI_read(QString)));
    //connect(&client,SIGNAL(server_emit(int,QString)),this,SLOT(UI_read(int,QString)));
    connect(this,SIGNAL(UI_emit(int)),&client,SLOT(client_read(int)));
}

MainWindow::~MainWindow()
{
    delete ui;
    QThread0->quit();
    QThread0->wait();
    QThread1->quit();
    QThread1->wait();
}

void MainWindow::UI_read(QString b)
{
    //将发送过来的参数，int型转成QString型，传入控件
//    QString aa = QString::number(a, 10);
//    ui->lineEdit->setText(aa);
    ui->textBrowser->setText(b);
}

void MainWindow::on_pushButton_OK_clicked()
{
    QString str = ui->lineEdit->text();
    int choose = str.toInt();
    UI_emit(choose);
}

/*****************************************************************/

Thread::Thread()
{
    qDebug()<<"Thread构造函数ID:"<<QThread::currentThreadId();
}

Thread::~Thread()
{
    qDebug()<<"Thread析构函数ID:"<<QThread::currentThreadId();
}

void Thread::srv_start()
{
    qDebug()<<"子线程srv_start函数ID:"<<QThread::currentThreadId();
    //P2PServer server;
    server.Start(9000);
}

void Thread::cli_start()
{
    qDebug()<<"子线程cli_start函数ID:"<<QThread::currentThreadId();
    //P2PClient client(9000);
    client.Start();
}
