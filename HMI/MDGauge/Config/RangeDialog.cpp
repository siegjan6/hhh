#include <QMessageBox>
#include <QColorDialog>
#include "RangeDialog.h"
#include "ui_RangeDialog.h"
#include "ColorDialog.h"
#include "StyleDialog.h"
#include "Ccommon.h"
#include "MDMultiLanguage.h"
#include "CMessage.h"

CRangeDialog::CRangeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CRangeDialog)
{
    ui->setupUi(this);
    m_rangeStyle = NULL;
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

CRangeDialog::~CRangeDialog()
{
    delete m_rangeStyle;
    delete m_edgeStyle;
    delete m_edgeColor;
    delete m_gradient;
    delete m_color1 ;
    delete m_color2 ;
    delete ui;
}

void CRangeDialog::initDialog()
{
    this->move(190,0);
    setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    hide();
   ui->radiusEdit->installEventFilter(this);
   ui->widEdit->installEventFilter(this);
   ui->widEdgeEdit->installEventFilter(this);
   QObject::connect(ui->radiusSlider,SIGNAL(valueChanged(qint32)),this,SLOT(setRadius(qint32)));
   QObject::connect(ui->widSlider,SIGNAL(valueChanged(qint32)),this,SLOT(setWidth(qint32)));
   QObject::connect(ui->widEdgeSlider,SIGNAL(valueChanged(qint32)),this,SLOT(setEdgeWidth(qint32)));
   QObject::connect(ui->radiusEdit,SIGNAL(editingFinished()),this,SLOT(radiusWarning()));
   QObject::connect(ui->widEdit,SIGNAL(editingFinished()),this,SLOT(widWarning()));
   QObject::connect(ui->widEdgeEdit,SIGNAL(editingFinished()),this,SLOT(edgeWarning()));
   QObject::connect(ui->startValueEdit,SIGNAL(textChanged(QString)),this,SLOT(setStartValue(QString)));
   QObject::connect(ui->rangeValueEdit,SIGNAL(textChanged(QString)),this,SLOT(setRangeValue(QString)));
}
/*!
\section 绘制矩形框。
*/
void CRangeDialog::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPen pen;
    pen.setColor(QColor(0,0,0,100));
    pen.setWidth(1);
    painter.setPen(pen);
    painter.drawRect(ui->edgeColorBtn->x()+7,ui->edgeColorBtn->y()+ui->groupBox_3->y()-2,
                     ui->edgeColorBtn->width()+3,ui->edgeColorBtn->height()+3);
    painter.drawRect(ui->color1Btn->x()+7,ui->color1Btn->y()+ui->groupBox_4->y()-2,
                     ui->color1Btn->width()+3,ui->color1Btn->height()+3);
    painter.drawRect(ui->color2Btn->x()+7,ui->color2Btn->y()+ui->groupBox_4->y()-2,
                     ui->color2Btn->width()+3,ui->color2Btn->height()+3);

}
/*!
\section 将各个属性的值显示在配置界面上。
*/
void CRangeDialog::setData(CData *data)
{
    m_range = (CDataRange*)data;
    m_rangeStyle = new CPaintButton;
    m_rangeStyle->m_button = ui->rangeStyleBtn;
    m_rangeStyle->setButtonImage(ui->rangeStyleBtn,stRange,m_range->m_arrangement,ptImage);
     m_edgeStyle = new CPaintButton;
     m_edgeStyle->m_button = ui->edgeStyleBtn;
     m_edgeStyle->setButtonImage(ui->edgeStyleBtn,stLine,
                                 m_range->m_edgeLine.m_lineStyle,ptImage);
     m_edgeColor = new CPaintButton;
     m_edgeColor->m_button = ui->edgeColorBtn;
     m_edgeColor->setButtonColor(ui->edgeColorBtn,m_range->m_edgeLine.m_color,ptColor);
     m_gradient = new CPaintButton;
     m_gradient->m_button = ui->gradStyleBtn;
     m_gradient->setButtonImage(ui->gradStyleBtn,stGradient,
                                m_range->m_backColor.m_gradientStyle,ptImage);
     m_color1 = new CPaintButton;
     m_color1->m_button = ui->color1Btn;
     m_color1->setButtonColor(ui->color1Btn,m_range->m_backColor.m_color1,ptColor);
     m_color2 = new CPaintButton;
     m_color2->m_button = ui->color2Btn;
     m_color2->setButtonColor(ui->color2Btn,m_range->m_backColor.m_color2,ptColor);
    Ccommon::setEdit(ui->radiusEdit,m_range->getRadius());
    Ccommon::setEdit(ui->widEdit,m_range->getWidth());
    Ccommon::setEdit(ui->widEdgeEdit,m_range->m_edgeLine.getWidth());
    Ccommon::setEdit(ui->startValueEdit,m_range->getMinValue());
    Ccommon::setEdit(ui->rangeValueEdit,m_range->getValueRange());
    Ccommon::setSlider(ui->radiusSlider,m_range->getRadius()*10);
    Ccommon::setSlider(ui->widSlider,m_range->getWidth()*10);
    Ccommon::setSlider(ui->widEdgeSlider,m_range->m_edgeLine.getWidth());

}

/*!
\section 向编辑框中输入数值对临界区属性进行配置。
*/
bool CRangeDialog::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->widEdit)
        {
             if (event->type()==QEvent::FocusOut)
             {
                 QString str = ui->widEdit->text();
                 qint32 wid = (qint32)(str.toDouble()*10);
                 ui->widSlider->setValue(wid);
                 m_range->setWidth(str.toDouble());
             }
        }
    if (watched == ui->radiusEdit)
        {
             if (event->type()==QEvent::FocusOut)
             {
                 QString str = ui->radiusEdit->text();
                 qint32 radius =qint32(str.toDouble()* 10);
                 ui->radiusSlider->setValue(radius);
                 m_range->setRadius(str.toDouble());
             }
        }
    if (watched == ui->widEdgeEdit)
        {
             if (event->type()==QEvent::FocusOut)
             {
                 QString str = ui->widEdgeEdit->text();
                 qint32 edge = str.toInt();
                 ui->widEdgeSlider->setValue(edge);
                 m_range->m_edgeLine.setWidth(edge);
             }
        }

    return QWidget::eventFilter(watched,event);
}

void CRangeDialog::setRadius(qint32 value)
{
    QString ret = QString("%1").arg(value / 10);
    ret.append(".");
    QString ret1 = QString("%1").arg(value % 10);
    ret.append(ret1);
    ui->radiusEdit->setText(ret);
    m_range->setRadius(ui->radiusEdit->text().toDouble());
    ((QWidget*)parent())->update();
}

void CRangeDialog::setWidth(qint32 value)
{
    QString ret = QString("%1").arg(value / 10);
    ret.append(".");
    QString ret1 = QString("%1").arg(value % 10);
    ret.append(ret1);
    ui->widEdit->setText(ret);
    m_range->setWidth(ui->widEdit->text().toDouble());
    ((QWidget*)parent())->update();
}

void CRangeDialog::setEdgeWidth(qint32 value)
{
    QString ret = QString::number(value);
    ui->widEdgeEdit->setText(ret);
    m_range->m_edgeLine.setWidth(ui->widEdgeEdit->text().toInt());
    ((QWidget*)parent())->update();
}

void CRangeDialog::setStartValue(QString str)
{
    qreal min = str.toDouble();
    m_range->setMinValue(min);
    ((QWidget*)parent())->update();
}

void CRangeDialog::setRangeValue(QString str)
{
    qreal range = str.toDouble();
    m_range->setValueRange(range);
    ((QWidget*)parent())->update();
}
/*!
\section 输入数值超限给出警告信息。
*/
void CRangeDialog::radiusWarning()
{
    Ccommon::editDoubleWarning(ui->radiusEdit,WARN_1_50F,50,1);
}

void CRangeDialog::widWarning()
{
    Ccommon::editDoubleWarning(ui->widEdit,WARN_1_25F,25,1);
}

void CRangeDialog::edgeWarning()
{
    Ccommon::editIntWarning(ui->widEdgeEdit,WARN_1_10I,10,1);
}

void CRangeDialog::on_rangeStyleBtn_clicked()
{
    m_rangeStyle->buttonClicked();
    m_range->m_arrangement = m_rangeStyle->m_index;
     ((QWidget*)parent())->update();
}

void CRangeDialog::on_edgeColorBtn_clicked()
{
    m_edgeColor->setParentDialog(this);
    m_edgeColor->buttonClicked();
    m_range->m_edgeLine.m_color = m_edgeColor->m_backColor;
    ((QWidget*)parent())->update();
}

void CRangeDialog::on_edgeStyleBtn_clicked()
{
    m_edgeStyle->buttonClicked();
    m_range->m_edgeLine.m_lineStyle = (lineStyle)m_edgeStyle->getIndex();
     ((QWidget*)parent())->update();
}

void CRangeDialog::on_color1Btn_clicked()
{
    m_color1->setParentDialog(this);
    m_color1->buttonClicked();
    m_range->m_backColor.m_color1 = m_color1->m_backColor;
    ((QWidget*)parent())->update();
}

void CRangeDialog::on_color2Btn_clicked()
{
    m_color2->setParentDialog(this);
    m_color2->buttonClicked();
    m_range->m_backColor.m_color2 = m_color2->m_backColor;
    ((QWidget*)parent())->update();
}

void CRangeDialog::on_gradStyleBtn_clicked()
{
    m_gradient->buttonClicked();
    m_range->m_backColor.m_gradientStyle = (gradientStyle)m_gradient->getIndex();
     ((QWidget*)parent())->update();
}
