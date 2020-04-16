#include "mainwindow.h"
#include "ui_mainwindow.h"

#define FixedColumnCount 6    //文件固定 6 列

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    theModel = new QStandardItemModel(2, FixedColumnCount, this) ; //数据模型
    theSelection = new QItemSelectionModel (theModel) ;//选择模型
    connect(theSelection,SIGNAL(currentChanged(QModelIndex,QModelIndex)), this,SLOT(on_currentChanged(QModelIndex,QModelIndex)));
    ui->tableView->setModel(theModel) ; //设置数据模型
    ui->tableView->setSelectionModel(theSelection) ; //设置选择模型
    ui->tableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectItems);
    //创建状态栏组件
    QStatusBar* bar = ui->statusBar; //获取状态栏
    LabNotice = new QLabel; //新建标签
    LabCellPos = new QLabel;
    LabCellText = new QLabel;
    LabNotice->setText("请注意：该软件根据您给出的分数筛选的大学仅供参考，是否报考决定权在您！");
    LabCellPos->setText("当前位置：");
    LabCellText->setText("当前内容：");
    bar->addWidget(LabNotice);
    LabNotice->setFrameShadow(QFrame::Sunken); //设置标签阴影
    bar->addWidget(LabCellPos);
    LabCellPos->setFrameShadow(QFrame::Sunken); //设置标签阴影
    bar->addWidget(LabCellText);
    LabCellPos->setFrameShadow(QFrame::Sunken); //设置标签阴影
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_currentChanged(const QModelIndex &current, const QModelIndex &previous)
{   //选择单元格变化时的响应
    if (current.isValid())
    {
        LabCellPos->setText(QString::asprintf("当前位置： %d 行， %d 列", current.row(),current.column()));
        QStandardItem* aItem=theModel->itemFromIndex(current);
        LabCellText->setText("当前内容："+aItem->text());
    }
}
void MainWindow::on_actSort_triggered()
{
    QString aFileName="Universities.txt";
    if (aFileName.isEmpty())
        return; //如果未选择文件，退出
    QStringList fFileContent;//文件内容字符串列表
    QFile aFile(aFileName);  //以文件方式读出
    if (aFile.open(QIODevice::ReadOnly | QIODevice::Text)) //以只读文本方式打开文件
    {
        QTextStream aStream(&aFile); //用文本流读取文件
        while (!aStream.atEnd())
        {
            QString str=aStream.readLine();//读取文件的一行
            fFileContent.append(str); //添加到 StringList
        }
        aFile.close();//关闭文件
        iniModelFromStringList(fFileContent);//从StringList的内容初始化数据模型
    }
}
void MainWindow::iniModelFromStringList(QStringList& aFileContent)
{   //从一个StringList 获取数据，初始化数据Model
    int rowCnt=aFileContent.count(); //文本行数，第1行是标题
    theModel->setRowCount(rowCnt-1); //实际数据行数
    //设置表头
    QString header=aFileContent.at(0);//第1行是表头
    //一个或多个空格、TAB等分隔符隔开的字符串， 分解为一个StringList
    QStringList headerList=header.split(QRegExp("\\s+"),QString::SkipEmptyParts);
    theModel->setHorizontalHeaderLabels(headerList); //设置表头文字
    //设置表格数据
    QString aLineText;
    QStringList tmpList;
    int j;
    QStandardItem *aItem;
    for (int i=1;i<rowCnt;i++)
    {
        aLineText=aFileContent.at(i); //获取数据区的一行
        //一个或多个空格、TAB等分隔符隔开的字符串， 分解为一个StringList
        tmpList=aLineText.split(QRegExp("\\s+"),QString::SkipEmptyParts);
        for (j=0;j<FixedColumnCount-3;j++) //tmpList的行数等于FixedColumnCount, 固定的
        { //不包含最后一列
            aItem=new QStandardItem(tmpList.at(j));//创建item
            theModel->setItem(i-1,j,aItem); //为模型的某个行列位置设置Item
        }
        for(;j<FixedColumnCount;j++){
            aItem=new QStandardItem(headerList.at(j));//最后一列是Checkable,需要设置
            //aItem=new QStandardItem();//最后一列是Checkable,设置
            aItem->setCheckable(false); //设置为Checkable
            //aItem->setTextAlignment(Qt::AlignHCenter);
            if (tmpList.at(j)=="0")
                aItem->setCheckState(Qt::Unchecked); //根据数据设置check状态
            else
                aItem->setCheckState(Qt::Checked);
            theModel->setItem(i-1,j,aItem); //为模型的某个行列位置设置Item
        }

    }
}


