#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QObject>
#include <QThread>
#include "thread.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

//signals:
//    void ToThread(); // 信号

private:
    Ui::MainWindow *ui;

    QThread *Thread_Test;
    Thread *thread_class;
};


#endif // MAINWINDOW_H
