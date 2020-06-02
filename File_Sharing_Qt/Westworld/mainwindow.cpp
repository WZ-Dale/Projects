#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QString str = "Welcome to the Westworld !";
    ui->textBrowser->setText(str);


    Thread_Test = new QThread;
    thread_class = new Thread;
    //connect(this,&MainWindow::ToThread,thread_class,&Thread::Thread_Fun);
    connect(Thread_Test,SIGNAL(started()),thread_class,SLOT(Thread_Fun()));
    connect(Thread_Test,SIGNAL(started()),thread_class,SLOT(Thread_Fun1()));
    //connect(Thread_Test,SIGNAL(finished()),thread_class,SLOT(deleteLater()));
    connect(thread_class,SIGNAL(finished()),thread_class,SLOT(deleteLater()));
    thread_class->moveToThread(Thread_Test);
    Thread_Test->start();
    //emit ToThread();
}

MainWindow::~MainWindow()
{
    delete ui;
    Thread_Test->quit();
    Thread_Test->wait();
}
