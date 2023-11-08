#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../../../include/MDExpression/MDExpression.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString string = ui->lineEdit->text();
    CMDExpression ex;
    bool isok = ex.compile(string);
    if(isok == false)
    {
         ui->label->setText("编译错误");
         return;
    }
    //ex
    int tmp_Year = 1980;
    int tmp_Month = 11;
    int tmp_Day = 25;
    int tmp_Hour = 17;
    int tmp_Minute = 59;
    QString v3 = "1";
    ex.setVariableValue("tmp_Year",tmp_Year);
    ex.setVariableValue("tmp_Month",tmp_Month);
    ex.setVariableValue("tmp_Day",tmp_Day);
    ex.setVariableValue("tmp_Hour",tmp_Hour);
    ex.setVariableValue("tmp_Minute",tmp_Minute);
    QList<QString> list = ex.referencedVarList();
    isok = true;
    QVariant v = ex.excute(&isok);
    if(isok)
        ui->label->setText(v.toString());
    else
        ui->label->setText("结果错误");
}
