#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QStandardItemModel>
#include <QItemSelectionModel>
#include <QFile>
#include <QTextStream>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void iniModelFromStringList (QStringList&); //从 StringList 初始化数据模型

private:
    QLabel *LabNotice;    //注意事项
    QLabel *LabCellPos;    //当前单元格行列号
    QLabel *LabCellText;    //当前单元格内容
    QStandardItemModel * theModel; //数据模型
    QItemSelectionModel *theSelection; //选择模型


private slots:
    //当前选择单元格发生变化
    void on_currentChanged(const QModelIndex &current, const QModelIndex &previous);

    void on_actSort_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
