#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QString str = "Welcome to the Westworld !";
    ui->textBrowser->setText(str);

    QThread* thread0 = new QThread();
    QThread* thread1 = new QThread();
    P2PServer* server = new P2PServer(9000);
    P2PClient* client = new P2PClient(9000);

    connect(thread0,SIGNAL(started()),server,SLOT(Start()));
    connect(server,SIGNAL(complete()),server,SLOT(deleteLater()));
    connect(server,SIGNAL(destroyed(QObject*)),thread0,SLOT(quit()));
    connect(thread0,SIGNAL(finished()),thread0,SLOT(deleteLater()));

    connect(thread1,SIGNAL(started()),client,SLOT(Start()));
    connect(client,SIGNAL(complete()),client,SLOT(deleteLater()));
    connect(client,SIGNAL(destroyed(QObject*)),thread1,SLOT(quit()));
    connect(thread1,SIGNAL(finished()),thread1,SLOT(deleteLater()));

    connect(server,SIGNAL(server_emit(QString)),this,SLOT(UI_read(QString)));
    connect(client,SIGNAL(client_emit(QString)),this,SLOT(UI_read(QString)));
    connect(this,SIGNAL(UI_emit(int)),client,SLOT(client_read(int)),Qt::DirectConnection);

    server->moveToThread(thread0);
    client->moveToThread(thread1);
    thread0->start();
    thread1->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::UI_read(QString b)
{
    //将发送过来的参数，int型转成QString型，传入控件
//    QString aa = QString::number(a, 10);
//    ui->lineEdit->setText(aa);
    ui->textBrowser->append(b);
}

void MainWindow::on_pushButton_OK_clicked()
{
    QString str = ui->lineEdit->text();
    int choose = str.toInt();
    emit UI_emit(choose);
//    std::cout << choose << std::endl;
}

void MainWindow::on_textBrowser_textChanged()
{
    ui->textBrowser->moveCursor(QTextCursor::End);
}
