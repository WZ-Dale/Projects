#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "server.hpp"
#include "client.hpp"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QString str = "Welcome to the FileSharing !";
    ui->textBrowser->setText(str);


    std::thread thr(&MainWindow::srv_start, this, ui);
    thr.detach();
    std::thread thr1(&MainWindow::cli_start, this, ui);
    thr1.detach();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::srv_start(Ui::MainWindow *ui){
    P2PServer server(ui);
    server.Start(9000);
}

void MainWindow::cli_start(Ui::MainWindow *ui){
    P2PClient client(9000, ui);
    client.Start();
}
