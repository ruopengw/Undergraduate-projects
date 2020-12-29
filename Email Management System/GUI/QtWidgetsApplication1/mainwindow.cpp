#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // connect函数
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(start()));
}

void MainWindow::start()
{
    // 清除结果区域前面缓存的结果
    ui->textBrowser->clear();

    //  获取输入框的内容
    QString content = ui->textEdit->toPlainText();
    char* content_char = content.toLatin1().data(); // 将QString转化为char字符数组

   //中间处理过程。。。。。。。。

   //  将获取的内容用append输出到TextBrowser
    QString result;
    // 处理result
    ui->textBrowser->append(result);    // 将结果输出到结果区域
}

MainWindow::~MainWindow()
{
    delete ui;
}
