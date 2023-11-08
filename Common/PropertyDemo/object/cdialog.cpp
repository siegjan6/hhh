#include "cdialog.h"
#include "ui_cdialog.h"
#include <QDebug>

CDialog::CDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CDialog)
{
    ui->setupUi(this);
}

CDialog::~CDialog()
{
    delete ui;
}

void CDialog::closeEvent(QCloseEvent *)
{
   emit dialogClosed();
}


void CDialog::on_pushButton_2_clicked()
{
    QImage img(16, 16, QImage::Format_ARGB32_Premultiplied);
    QPainter painter(&img);
    painter.setCompositionMode(QPainter::CompositionMode_Source);

    QLinearGradient line(0,0,img.width(),img.height());
    line.setColorAt(0.2,Qt::white);
    line.setColorAt(0.6,Qt::green);
    line.setColorAt(1.0,Qt::black);

    painter.fillRect(0,0,img.width(),img.height(),QBrush(line));
    painter.end();

    emit changeValue(ui->lineEdit->text());
    emit changeIcon(QPixmap::fromImage(img));
    this->close();
}
