#include <QTest>
#include <QMessageBox>
#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->lineEditFullName->setText(m_formPreviewTest.fullName());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButtonFormPreview_clicked()
{
    m_formPreviewTest.setFullName(ui->lineEditFullName->text());

    int result = QTest::qExec(&m_formPreviewTest);
    if(!result)
        QMessageBox::about(this, "布局预览模块", "测试通过");
    else
        QMessageBox::about(this, "布局预览模块", "测试失败");
}
