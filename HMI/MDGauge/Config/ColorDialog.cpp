#include <QPainter>
#include <QMessageBox>
#include <QMouseEvent>
#include <QColorDialog>
#include "ColorDialog.h"
#include "ui_ColorDialog.h"
#include "Ccommon.h"
#include "CMessage.h"
#include "MDMultiLanguage.h"

CColorDialog::CColorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CColorDialog)
{
    ui->setupUi(this);
    m_color = QColor(0,0,0,0);
    m_R = 0;
    m_G = 0;
    m_B = 0;
    m_A = 0;
    m_rect = QRect(10,150,320,50);
    initDialog();

    QStringList types;
    types << "QLineEdit";
    CMDMultiLanguage::instance()->translateWidget(this, &types);
}

CColorDialog::~CColorDialog()
{
    delete ui;
}

void CColorDialog::initDialog()
{
    this->setWindowTitle(tr("颜色"));
    this->setModal(true);
    ui->AColorEdit->installEventFilter(this);
    ui->RColorEdit->installEventFilter(this);
    ui->GColorEdit->installEventFilter(this);
    ui->BColorEdit->installEventFilter(this);
    QObject::connect(ui->AColorSlider,SIGNAL(valueChanged(qint32)),this,SLOT(setAColor(qint32)));
    QObject::connect(ui->RColorSlider,SIGNAL(valueChanged(qint32)),this,SLOT(setRColor(qint32)));
    QObject::connect(ui->GColorSlider,SIGNAL(valueChanged(qint32)),this,SLOT(setGColor(qint32)));
    QObject::connect(ui->BColorSlider,SIGNAL(valueChanged(qint32)),this,SLOT(setBColor(qint32)));
}

void CColorDialog::paintEvent(QPaintEvent *)
{
    QPainter *painter = new QPainter(this);
    painter->setPen(QPen(QColor(0,0,0,80),1));
    QRect rectFrame = QRect(8,148,324,54);
    painter->drawRect(rectFrame);
    painter->setPen(QPen(QColor(192,192,192),1));
    painter->setBrush(QColor(m_R,m_G,m_B,m_A));
    painter->drawRect(m_rect);
    delete painter;
}

void CColorDialog::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        if(m_rect.contains(event->pos()) == true)
        {
            QColorDialog colorDialog(this);
            colorDialog.setWindowTitle(tr("颜色"));
            colorDialog.setCurrentColor(QColor(m_R,m_G,m_B,m_A));
            if(colorDialog.exec() != QDialog::Accepted)
               return;
            QColor color = colorDialog.currentColor();
            ui->RColorSlider->setValue(color.red());
            ui->GColorSlider->setValue(color.green());
            ui->BColorSlider->setValue(color.blue());
        }
    }
    this->update();
}

bool CColorDialog::eventFilter(QObject *watched, QEvent *event)
{
    if (watched==ui->AColorEdit)
        {
             if (event->type()==QEvent::FocusOut)
             {
                 QString str = ui->AColorEdit->text();
                 qint32 A_color = str.toLongLong();
                 ui->AColorSlider->setValue(A_color);
             }
        }
    if (watched==ui->RColorEdit)
        {
             if (event->type()==QEvent::FocusOut)
             {
                 QString str = ui->RColorEdit->text();
                 qint32 R_color =str.toLongLong();
                 ui->RColorSlider->setValue(R_color);
             }
        }
    if (watched==ui->GColorEdit)
        {
             if (event->type()==QEvent::FocusOut)
             {
                 QString str = ui->GColorEdit->text();
                 qint32 G_color = str.toLongLong();
                 ui->GColorSlider->setValue(G_color);
             }
        }
    if (watched==ui->BColorEdit)
        {
             if (event->type()==QEvent::FocusOut)
             {
                 QString str = ui->BColorEdit->text();
                 qint32 B_color = str.toLongLong();
                 ui->BColorSlider->setValue(B_color);
             }
        }

     return QWidget::eventFilter(watched,event);
     this->update();
}

void CColorDialog::setAColor(qint32 value)
{
    QString ret = QString::number(value);
    ui->AColorEdit->setText(ret);
    m_A = ui->AColorEdit->text().toLongLong();
    this->update();
}

void CColorDialog::setRColor(qint32 value)
{
    QString ret = QString::number(value);
    ui->RColorEdit->setText(ret);
    m_R = ui->RColorEdit->text().toLongLong();
    this->update();
}

void CColorDialog::setGColor(qint32 value)
{
    QString ret = QString::number(value);
    ui->GColorEdit->setText(ret);
    m_G = ui->GColorEdit->text().toLongLong();
    this->update();
}

void CColorDialog::setBColor(qint32 value)
{
    QString ret = QString::number(value);
    ui->BColorEdit->setText(ret);
    m_B = ui->BColorEdit->text().toLongLong();
    this->update();
}

void CColorDialog::setColor(QColor c)
{
    m_color = c;
    ui->AColorSlider->setValue(m_color.alpha());
    ui->RColorSlider->setValue(m_color.red());
    ui->GColorSlider->setValue(m_color.green());
    ui->BColorSlider->setValue(m_color.blue());
    QString str;
    str.setNum(m_color.alpha());
    ui->AColorEdit->setText(str);
    str.setNum(m_color.red());
    ui->RColorEdit->setText(str);
    str.setNum(m_color.green());
    ui->GColorEdit->setText(str);
    str.setNum(m_color.blue());
    ui->BColorEdit->setText(str);
}
/*!
\section 点击确定按钮后检查设定的各颜色分量是否在0-255间，否则给出警告信息；
将确定的颜色值给m_color供调用。
*/
void CColorDialog::on_OKBtn_clicked()
{
    Ccommon::editIntWarning(ui->AColorEdit,WARN_0_255I,255,0);
    Ccommon::editIntWarning(ui->RColorEdit,WARN_0_255I,255,0);
    Ccommon::editIntWarning(ui->GColorEdit,WARN_0_255I,255,0);
    Ccommon::editIntWarning(ui->BColorEdit,WARN_0_255I,255,0);
    m_color = QColor(ui->RColorEdit->text().toLongLong(), ui->GColorEdit->text().toLongLong(),
                     ui->BColorEdit->text().toLongLong(),ui->AColorEdit->text().toLongLong());

    this->accept();
}

void CColorDialog::on_CancelBtn_clicked()
{
    this->close();
}

