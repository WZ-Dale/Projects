#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    friend class P2PSever;
    friend class P2PClient;
    void srv_start(Ui::MainWindow *ui);
    void cli_start(Ui::MainWindow *ui);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
