#include <QMessageBox>
#include "LabelDialog.h"
#include "ui_LabelDialog.h"
#include "ColorDialog.h"
#include "Ccommon.h"
#include "MDMultiLanguage.h"
#include "CMessage.h"

CLabelDialog::CLabelDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CLabelDialog)
{
    ui->setupUi(this);
    m_color = NULL;
    m_x  = 50;
    m_y = 40;
    m_label = NULL;
    this->initDialog();
    CMDMultiLanguage::instance()->translateWidget(this);

}

CLabelDialog::~CLabelDialog()
{
    delete m_color;
    delete ui;
}

void CLabelDialog::initDialog()
{
    this->move(190,0);
    setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    hide();
    ui->fontComboBox->setMaxVisibleItems(10);
    ui->leftEdit->installEventFilter(this);
    ui->topEdit->installEventFilter(this);
    ui->sizeEdit->installEventFilter(this);
    QObject::connect(ui->leftSlider,SIGNAL(valueChanged(qint32)),this,SLOT(setLeft(qint32)));
    QObject::connect(ui->topSlider,SIGNAL(valueChanged(qint32)),this,SLOT(setTop(qint32)));
    QObject::connect(ui->sizeSlider,SIGNAL(valueChanged(qint32)),this,SLOT(setSize(qint32)));
    QObject::connect(ui->fontComboBox,SIGNAL(currentTextChanged(QString)),this,SLOT(setFontName(QString)));
    QObject::connect(ui->stringEdit,SIGNAL(textChanged(QString)),this,SLOT(setString(QString)));
    QObject::connect(ui->boldCheckBox,SIGNAL(stateChanged(qint32)),this,SLOT(setTextBold(qint32)));

    QObject::connect(ui->leftEdit,SIGNAL(editingFinished()),this,SLOT(leftWarning()));
    QObject::connect(ui->topEdit,SIGNAL(editingFinished()),this,SLOT(topWarning()));
    QObject::connect(ui->sizeEdit,SIGNAL(editingFinished()),this,SLOT(sizeWarning()));
}

void CLabelDialog::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPen pen;
    pen.setColor(QColor(0,0,0,100));
    pen.setWidth(1);
    painter.setPen(pen);
    painter.drawRect(ui->colorBtn->x()+7,ui->colorBtn->y()+ui->groupBox_2->y()-2,
                     ui->colorBtn->width()+3,ui->colorBtn->height()+3);

}
/*!
\section 将各个属性的值显示在配置界面上。
*/
void CLabelDialog::setData(CData *data)
{
    m_label = (CDataLabel*)data;
    m_color = new CPaintButton;
    m_color->m_button = ui->colorBtn;
    m_color->setButtonColor(ui->colorBtn,m_label->m_color,ptColor);
    QPointF point = m_label->m_pos;
    Ccommon::setSlider(ui->topSlider,point.y()*10);
    Ccommon::setEdit(ui->topEdit,point.y());
    Ccommon::setSlider(ui->leftSlider,point.x()*10);
    Ccommon::setEdit(ui->leftEdit,point.x());
    Ccommon::setEdit(ui->sizeEdit,m_label->getSize());
    Ccommon::setSlider(ui->sizeSlider,m_label->getSize()*10);
    ui->stringEdit->setText(m_label->m_text);
    ui->fontComboBox->setCurrentText(m_label->m_name);
    if(m_label->m_isBold == false)
    {
        ui->boldCheckBox->setChecked(false);
    }
    else
        ui->boldCheckBox->setChecked(true);
}
/*!
\section 向编辑框中输入数值对标签进行配置。
*/
bool CLabelDialog::eventFilter(QObject *watched, QEvent *event)
{
    if (watched==ui->leftEdit)
        {
             if (event->type()==QEvent::FocusOut)
             {
                 QString str = ui->leftEdit->text();
                 qint32 left = qint32(str.toDouble()*10);
                 ui->leftSlider->setValue(left);
                 m_label->m_pos = QPointF(str.toDouble(),m_y);
             }
        }
    if (watched==ui->topEdit)
        {
             if (event->type()==QEvent::FocusOut)
             {
                 QString str = ui->topEdit->text();
                 qint32 top =qint32(str.toDouble()* 10);
                 ui->topSlider->setValue(top);
                 m_label->m_pos = QPointF(m_x,str.toDouble());
             }
        }
    if (watched==ui->sizeEdit)
        {
             if (event->type()==QEvent::FocusOut)
             {
                 QString str = ui->sizeEdit->text();
                 qint32 size =qint32(str.toDouble()* 10);
                 ui->sizeSlider->setValue(size);
                 m_label->setSize(str.toDouble());
             }
        }
     return QWidget::eventFilter(watched,event);

}
/*!
\section 设置标签的位置。
*/
void CLabelDialog::setLeft(qint32 value)
{
    QString str = QString("%1").arg(value / 10);
    str.append(".");
    QString str1 = QString("%1").arg(value % 10);
    str.append(str1);
    ui->leftEdit->setText(str);
    m_x = ui->leftEdit->text().toDouble();
    m_label->m_pos = QPointF(m_x,m_y);
    ((QWidget*)parent())->update();
}

void CLabelDialog::setTop(qint32 value)
{
    QString str = QString("%1").arg(value / 10);
    str.append(".");
    QString str1 = QString("%1").arg(value % 10);
    str.append(str1);
    ui->topEdit->setText(str);
    m_y = ui->topEdit->text().toDouble();
    m_label->m_pos = QPointF(m_x,m_y);
    ((QWidget*)parent())->update();
}
/*!
\section 设置标签的大小。
*/
void CLabelDialog::setSize(qint32 value)
{
    QString str = QString("%1").arg(value / 10);
    str.append(".");
    QString str1 = QString("%1").arg(value % 10);
    str.append(str1);
    ui->sizeEdit->setText(str);
    m_label->setSize(ui->sizeEdit->text().toDouble());
    ((QWidget*)parent())->update();
}
/*!
\section 设置标签的字体，文本以及文本的是否加粗。
*/
void CLabelDialog::setFontName(QString str)
{
    m_label->m_name = str;
    ((QWidget*)parent())->update();
}

void CLabelDialog::setString(QString str)
{
    m_label->m_text = str;
    m_label->m_showText = CMDMultiLanguage::instance()->value(m_label->m_text, false);
    ((QWidget*)parent())->update();
}

void CLabelDialog::setTextBold(qint32 state)
{
    if(state == 0)
        m_label->m_isBold = false;
    else
        m_label->m_isBold = true;
    ((QWidget*)parent())->update();
}
/*!
\section 超出限定值给出警告。
*/
void CLabelDialog::leftWarning()
{
    Ccommon::editDoubleWarning(ui->leftEdit,WARN_0_100F,100,0);
}

void CLabelDialog::topWarning()
{
    Ccommon::editDoubleWarning(ui->topEdit,WARN_0_100F,100,0);
}

void CLabelDialog::sizeWarning()
{
    Ccommon::editDoubleWarning(ui->sizeEdit,WARN_5_20F,20,5);
}
/*!
\section 设置标签的字体颜色。
*/
void CLabelDialog::on_colorBtn_clicked()
{
    m_color->setParentDialog(this);
    m_color->buttonClicked();
    m_label->m_color = m_color->getColor();
    ((QWidget*)parent())->update();
}


