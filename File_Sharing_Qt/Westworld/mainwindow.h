#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QObject>
#include <QThread>
#include <QDebug>
#include <QString>

#include "server.hpp"
#include "client.hpp"

namespace Ui {
class MainWindow;
}

class Thread;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void UI_emit(int a);
private slots:
    void UI_read(QString b);
    void on_pushButton_OK_clicked();

private:
    Ui::MainWindow *ui;

    QThread *QThread0;
    QThread *QThread1;
    Thread *thread0;
    Thread *thread1;
};

/*****************************************************************/

class Thread : public QObject
{
    Q_OBJECT
public:
    Thread();
    ~Thread();
public slots:
    void srv_start();
    void cli_start();
};


#endif // MAINWINDOW_H
