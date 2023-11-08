#include <QPainter>
#include <QMessageBox>
#include "RulerRangeDialog.h"
#include "ui_RulerRangeDialog.h"
#include "StyleDialog.h"
#include "ColorDialog.h"
#include "Ccommon.h"
#include "MDMultiLanguage.h"
#include "CMessage.h"

CRulerRangeDialog::CRulerRangeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CRulerRangeDialog)
{
    ui->setupUi(this);
    m_rRange = NULL;
    m_rRangeStyle = NULL;
    m_rRangeArrange = NULL;
    m_edgeStyle = NULL;
    m_edgeColor = NULL;
    m_gradient = NULL;
    m_color1 = NULL;
    m_color2 = NULL;
    this->initDialog();
    QStringList types;
    types << "QLineEdit";
    CMDMultiLanguage::instance()->translateWidget(this, &types);
}

CRulerRangeDialog::~CRulerRangeDialog()
{
    delete m_rRangeStyle;
    delete m_rRangeArrange;
    delete m_edgeStyle ;
    delete m_edgeColor ;
    delete m_gradient;
    delete m_color1;
    delete m_color2;
    delete ui;
}

void CRulerRangeDialog::initDialog()
{
    this->move(190,10);
    setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    hide();
    ui->rWidEdit->installEventFilter(this);
    ui->rOffsetEdit->installEventFilter(this);
    ui->rStartValueEdit->installEventFilter(this);
    ui->rRangeValueEdit->installEventFilter(this);
    ui->redgeWidEdit->installEventFilter(this);
    QObject::connect(ui->rWidSlider,SIGNAL(valueChanged(qint32)),this,SLOT(setRangeWid(qint32)));
    QObject::connect(ui->rOffsetSlider,SIGNAL(valueChanged(qint32)),this,SLOT(setOffset(qint32)));
    QObject::connect(ui->rStartSlider,SIGNAL(valueChanged(qint32)),this,SLOT(setStartValue(qint32)));
    QObject::connect(ui->rRangeValueSlider,SIGNAL(valueChanged(qint32)),this,SLOT(setRangeValue(qint32)));
    QObject::connect(ui->redgeWidSlider,SIGNAL(valueChanged(qint32)),this,SLOT(setEdgeWid(qint32)));

    QObject::connect(ui->rWidEdit,SIGNAL(editingFinished()),this,SLOT(widWarning()));
    QObject::connect(ui->rOffsetEdit,SIGNAL(editingFinished()),this,SLOT(offsetWarning()));
    QObject::connect(ui->rStartValueEdit,SIGNAL(editingFinished()),this,SLOT(startWarning()));
    QObject::connect(ui->rRangeValueEdit,SIGNAL(editingFinished()),this,SLOT(rangeWarning()));
    QObject::connect(ui->redgeWidEdit,SIGNAL(editingFinished()),this,SLOT(edgeWarning()));
}
/*!
\section 将各个属性的值显示在配置界面上。
*/
void CRulerRangeDialog::setData(CData *data)
{
    m_rRange = (CRulerRange*)data;
    m_rRangeStyle = new CPaintButton;
    m_rRangeStyle->m_button = ui->rRangeStyBtn;
    m_rRangeStyle->setButtonImage(ui->rRangeStyBtn,stRRangeStyle,
                                  m_rRange->m_style,ptImage);
    m_rRangeArrange = new CPaintButton;
    m_rRangeArrange->m_button = ui->rRangeArrangeBtn;
    m_rRangeArrange->setButtonImage(ui->rRangeArrangeBtn,stRRangeArrangement,
                                    m_rRange->m_arrangement,ptImage);
     m_edgeStyle = new CPaintButton;
     m_edgeStyle->m_button = ui->redgeStyleBtn;
     m_edgeStyle->setButtonImage(ui->redgeStyleBtn,stLine,
                                 m_rRange->m_edgeLine.m_lineStyle,ptImage);
     m_edgeColor = new CPaintButton;
     m_edgeColor->m_button = ui->redgeColorBtn;
     m_edgeColor->setButtonColor(ui->redgeColorBtn,m_rRange->m_edgeLine.m_color,ptColor);
     m_gradient = new CPaintButton;
     m_gradient->m_button = ui->rgradStyleBtn;
     m_gradient->setButtonImage(ui->rgradStyleBtn,stGradient,
                                m_rRange->m_backColor.m_gradientStyle,ptImage);
     m_color1 = new CPaintButton;
     m_color1->m_button = ui->rcolor1Btn;
     m_color1->setButtonColor(ui->rcolor1Btn,m_rRange->m_backColor.m_color1,ptColor);
     m_color2 = new CPaintButton;
     m_color2->m_button = ui->rcolor2Btn;
     m_color2->setButtonColor(ui->rcolor2Btn,m_rRange->m_backColor.m_color2,ptColor);
    Ccommon::setEdit(ui->rWidEdit,m_rRange->getWidth());
    Ccommon::setEdit(ui->rOffsetEdit,m_rRange->getOffset());
    Ccommon::setEdit(ui->rStartValueEdit,m_rRange->getMinPercent());
    Ccommon::setEdit(ui->rRangeValueEdit,m_rRange->getPercentRange());
    Ccommon::setEdit(ui->redgeWidEdit,m_rRange->m_edgeLine.getWidth());
    Ccommon::setSlider(ui->rWidSlider,m_rRange->getWidth()*10);
    Ccommon::setSlider(ui->rOffsetSlider,m_rRange->getOffset()*10);
    Ccommon::setSlider(ui->rStartSlider,m_rRange->getMinPercent()*10);
    Ccommon::setSlider(ui->rRangeValueSlider,m_rRange->getPercentRange()*10);
    Ccommon::setSlider(ui->redgeWidSlider,m_rRange->m_edgeLine.getWidth());

}
//!  绘制矩形边框
void CRulerRangeDialog::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPen pen;
    pen.setColor(QColor(0,0,0,100));
    pen.setWidth(1);
    painter.setPen(pen);
    painter.drawRect(ui->redgeColorBtn->x()+7,ui->redgeColorBtn->y()+ui->groupBox_2->y()-2,
                     ui->redgeColorBtn->width()+3,ui->redgeColorBtn->height()+3);
    painter.drawRect(ui->rcolor1Btn->x()+7,ui->rcolor1Btn->y()+ui->groupBox_3->y()-2,
                     ui->rcolor1Btn->width()+3,ui->rcolor1Btn->height()+3);
    painter.drawRect(ui->rcolor2Btn->x()+7,ui->rcolor2Btn->y()+ui->groupBox_3->y()-2,
                     ui->rcolor2Btn->width()+3,ui->rcolor2Btn->height()+3);
}
/*!
\section 向编辑框中输入数值对直表显示块的属性进行配置。
*/
bool CRulerRangeDialog::eventFilter(QObject *watched, QEvent *event)
{
    if (watched==ui->rWidEdit)
        {
             if (event->type()==QEvent::FocusOut)
             {
                 QString str = ui->rWidEdit->text();
                 qint32 wid =qint32(str.toDouble()* 10);
                 ui->rWidSlider->setValue(wid);
                 m_rRange->setWidth(str.toDouble());
             }
        }

    if (watched==ui->rOffsetEdit)
        {
             if (event->type()==QEvent::FocusOut)
             {
                 QString str = ui->rOffsetEdit->text();
                 qint32 offset =qint32(str.toDouble()* 10);
                 ui->rOffsetSlider->setValue(offset);
                 m_rRange->setOffset(str.toDouble());
             }
        }
    if (watched==ui->rStartValueEdit)
        {
             if (event->type()==QEvent::FocusOut)
             {
                 QString str = ui->rStartValueEdit->text();
                 qint32 start =qint32(str.toDouble()* 10);
                 ui->rStartSlider->setValue(start);
                 m_rRange->setMinPercent(str.toDouble());
             }
        }
    if (watched==ui->rRangeValueEdit)
        {
             if (event->type()==QEvent::FocusOut)
             {
                 QString str = ui->rRangeValueEdit->text();
                 qint32 range = qint32(str.toDouble()* 10);
                 ui->rRangeValueSlider->setValue(range);
                 m_rRange->setPercentRange(str.toDouble());
             }
        }
    if (watched==ui->redgeWidEdit)
        {
             if (event->type()==QEvent::FocusOut)
             {
                 QString str = ui->redgeWidEdit->text();
                 qint32 edge = str.toInt();
                 ui->redgeWidSlider->setValue(edge);
                 m_rRange->m_edgeLine.setWidth(edge);
             }
        }
     return QWidget::eventFilter(watched,event);
}

void CRulerRangeDialog::setRangeWid(qint32 value)
{
    QString str = QString("%1").arg(value / 10);
    str.append(".");
    QString str1 = QString("%1").arg(value % 10);
    str.append(str1);
    ui->rWidEdit->setText(str);
    m_rRange->setWidth(ui->rWidEdit->text().toDouble());
    ((QWidget*)parent())->update();
}

void CRulerRangeDialog::setOffset(qint32 value)
{
    QString str = QString("%1").arg(value / 10);
    str.append(".");
    QString str1 = QString("%1").arg(abs(value % 10));
    str.append(str1);
    ui->rOffsetEdit->setText(str);
    m_rRange->setOffset(ui->rOffsetEdit->text().toDouble());
    ((QWidget*)parent())->update();
}

void CRulerRangeDialog::setStartValue(qint32 value)
{
    QString str = QString("%1").arg(value / 10);
    str.append(".");
    QString str1 = QString("%1").arg(abs(value % 10));
    str.append(str1);
    ui->rStartValueEdit->setText(str);
    m_rRange->setMinPercent(ui->rStartValueEdit->text().toDouble());
    ((QWidget*)parent())->update();
}

void CRulerRangeDialog::setRangeValue(qint32 value)
{
    QString str = QString("%1").arg(value / 10);
    str.append(".");
    QString str1 = QString("%1").arg(abs(value % 10));
    str.append(str1);
    ui->rRangeValueEdit->setText(str);
    m_rRange->setPercentRange(ui->rRangeValueEdit->text().toDouble());
    ((QWidget*)parent())->update();
}

void CRulerRangeDialog::setEdgeWid(qint32 value)
{
    QString str = QString::number(value);
    ui->redgeWidEdit->setText(str);
    m_rRange->m_edgeLine.setWidth(ui->redgeWidEdit->text().toInt());
    ((QWidget*)parent())->update();
}
/*!
\section 输入数值超限给出警告信息。
*/
void CRulerRangeDialog::widWarning()
{
    Ccommon::editDoubleWarning(ui->rWidEdit,WARN_1_25F,25,1);
}

void CRulerRangeDialog::offsetWarning()
{
    Ccommon::editDoubleWarning(ui->rOffsetEdit,WARN__55_55F,55,-55);
}

void CRulerRangeDialog::startWarning()
{
    Ccommon::editDoubleWarning(ui->rStartValueEdit,WARN_0_100F,100,0);
}

void CRulerRangeDialog::rangeWarning()
{
    Ccommon::editDoubleWarning(ui->rRangeValueEdit,WARN_0_100F,100,0);
}

void CRulerRangeDialog::edgeWarning()
{
    Ccommon::editIntWarning(ui->redgeWidEdit,WARN_1_10I,10,1);
}

void CRulerRangeDialog::on_rRangeStyBtn_clicked()
{
    m_rRangeStyle->buttonClicked();
    m_rRange->m_style = m_rRangeStyle->getIndex();
     ((QWidget*)parent())->update();
}

void CRulerRangeDialog::on_rRangeArrangeBtn_clicked()
{
    m_rRangeArrange->buttonClicked();
    m_rRange->m_arrangement = m_rRangeStyle->getIndex();
     ((QWidget*)parent())->update();
}

void CRulerRangeDialog::on_redgeStyleBtn_clicked()
{
    m_edgeStyle->buttonClicked();
    m_rRange->m_edgeLine.m_lineStyle = (lineStyle)m_edgeStyle->getIndex();
     ((QWidget*)parent())->update();
}

void CRulerRangeDialog::on_rgradStyleBtn_clicked()
{
    m_gradient->buttonClicked();
    m_rRange->m_backColor.m_gradientStyle = (gradientStyle)m_gradient->getIndex();
     ((QWidget*)parent())->update();
}

void CRulerRangeDialog::on_redgeColorBtn_clicked()
{
    m_edgeColor->setParentDialog(this);
    m_edgeColor->buttonClicked();
    m_rRange->m_edgeLine.m_color = m_edgeColor->m_backColor;
    ((QWidget*)parent())->update();
}

void CRulerRangeDialog::on_rcolor1Btn_clicked()
{
    m_color1->setParentDialog(this);
    m_color1->buttonClicked();
    m_rRange->m_backColor.m_color1 = m_color1->m_backColor;
    ((QWidget*)parent())->update();
}

void CRulerRangeDialog::on_rcolor2Btn_clicked()
{
    m_color2->setParentDialog(this);
    m_color2->buttonClicked();
    m_rRange->m_backColor.m_color2 = m_color2->m_backColor;
    ((QWidget*)parent())->update();
}

