#include "cdialog_1.h"
#include "ui_cdialog_1.h"

CDialog_1::CDialog_1(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CDialog_1)
{
    ui->setupUi(this);
}

CDialog_1::~CDialog_1()
{
    delete ui;
}

void CDialog_1::on_pushButton_clicked()
{
    emit changeIcon(QPixmap());
    emit changeValue(ui->lineEdit->text());
    this->setVisible(false);
}
