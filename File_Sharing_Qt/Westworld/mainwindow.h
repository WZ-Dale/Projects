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
    void on_textBrowser_textChanged();
private:
    Ui::MainWindow *ui;
};


#endif // MAINWINDOW_H
