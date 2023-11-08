#include <QMessageBox>
#include "CapDialog.h"
#include "ui_CapDialog.h"
#include "ColorDialog.h"
#include "StyleDialog.h"
#include "Ccommon.h"
#include "CMessage.h"
#include "MDMultiLanguage.h"

CCapDialog::CCapDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CCapDialog)
{
    ui->setupUi(this);
    m_cap = NULL;
    m_capStyle = NULL;
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

CCapDialog::~CCapDialog()
{
    delete m_capStyle;
    delete m_edgeStyle;
    delete m_edgeColor;
    delete m_gradient;
    delete m_color1;
    delete m_color2;
    delete ui;
}

void CCapDialog::initDialog()
{
    this->move(190,0);
    setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    hide();
    ui->edgeWidEdit->installEventFilter(this);
    ui->radiusEdit->installEventFilter(this);
    QObject::connect(ui->edgeWidSlider,SIGNAL(valueChanged(int)),this,SLOT(setEdgeWidth(int)));
    QObject::connect(ui->radiusSlider,SIGNAL(valueChanged(int)),this,SLOT(setRadius(int)));
    QObject::connect(ui->edgeWidEdit,SIGNAL(editingFinished()),this,SLOT(edgeWarning()));
    QObject::connect(ui->radiusEdit,SIGNAL(editingFinished()),this,SLOT(radiusWarning()));
}

void CCapDialog::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPen pen;
    pen.setColor(QColor(0,0,0,100));
    pen.setWidth(1);
    painter.setPen(pen);
    painter.drawRect(ui->edgeColorBtn->x()+7,ui->edgeColorBtn->y()+ui->groupBox_2->y()-2,
                     ui->edgeColorBtn->width()+3,ui->edgeColorBtn->height()+3);
    painter.drawRect(ui->bkColor1Btn->x()+7,ui->bkColor1Btn->y()+ui->groupBox_3->y()-2,
                     ui->bkColor1Btn->width()+3,ui->bkColor1Btn->height()+3);
    painter.drawRect(ui->bkColor2Btn->x()+7,ui->bkColor2Btn->y()+ui->groupBox_3->y()-2,
                     ui->bkColor2Btn->width()+3,ui->bkColor2Btn->height()+3);

}

void CCapDialog::setData(CData *data)
{
    m_cap = (CDataCap*)data;
    m_capStyle = new CPaintButton;
    m_capStyle->m_button = ui->capStyleBtn;
    m_capStyle->setButtonImage(ui->capStyleBtn,stCap,m_cap->m_style,ptImage);
    m_edgeStyle = new CPaintButton;
    m_edgeStyle->m_button = ui->edgeStyleBtn;
    m_edgeStyle->setButtonImage(ui->edgeStyleBtn,stLine,
                                m_cap->m_edgeLine.m_lineStyle,ptImage);
    m_gradient = new CPaintButton;
    m_gradient->m_button= ui->backGradientBtn;
    m_gradient->setButtonImage(ui->backGradientBtn,stGradient,
                               m_cap->m_backColor.m_gradientStyle,ptImage);
    m_edgeColor = new CPaintButton;
    m_edgeColor->m_button = ui->edgeColorBtn;
    m_edgeColor->setButtonColor(ui->edgeColorBtn,m_cap->m_edgeLine.m_color,ptColor);
    m_color1 = new CPaintButton;
    m_color1->m_button = ui->bkColor1Btn;
    m_color1->setButtonColor(ui->bkColor1Btn,m_cap->m_backColor.m_color1,ptColor);
    m_color2 = new CPaintButton;
    m_color2->m_button = ui->bkColor2Btn;
    m_color2->setButtonColor(ui->bkColor2Btn,m_cap->m_backColor.m_color2,ptColor);
    Ccommon::setSlider(ui->radiusSlider,m_cap->getRadius()*10);
    Ccommon::setEdit(ui->radiusEdit,m_cap->getRadius());
    Ccommon::setSlider(ui->edgeWidSlider,m_cap->m_edgeLine.getWidth());
    Ccommon::setEdit(ui->edgeWidEdit,m_cap->m_edgeLine.getWidth());
}

bool CCapDialog::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->edgeWidEdit)
        {
             if (event->type()==QEvent::FocusOut)
             {
                 QString str = ui->edgeWidEdit->text();
                 qint32 edge = str.toInt();
                 ui->edgeWidSlider->setValue(edge);
                 m_cap->m_edgeLine.setWidth(edge);
             }
        }
    if (watched == ui->radiusEdit)
        {
             if (event->type()==QEvent::FocusOut)
             {
                 QString str = ui->radiusEdit->text();
                 qint32 radius =qint32(str.toDouble()* 10);
                 ui->radiusSlider->setValue(radius);
                 m_cap->setRadius(str.toDouble());
             }
        }

    return QWidget::eventFilter(watched,event);
}

void CCapDialog::setEdgeWidth(qint32 value)
{
    QString ret = QString::number(value);
    ui->edgeWidEdit->setText(ret);
    m_cap->m_edgeLine.setWidth(ui->edgeWidEdit->text().toInt());
    ((QWidget*)parent())->update();
}

void CCapDialog::setRadius(qint32 value)
{
    QString ret = QString("%1").arg(value / 10);
    ret.append(".");
    QString ret1 = QString("%1").arg(value % 10);
    ret.append(ret1);
    ui->radiusEdit->setText(ret);
    m_cap->setRadius(ui->radiusEdit->text().toDouble());
    ((QWidget*)parent())->update();
}
/*!
\section 输入数值超过限定范围给出警告
*/
void CCapDialog::edgeWarning()
{
    Ccommon::editIntWarning(ui->edgeWidEdit,WARN_1_10I,10,1);
}

void CCapDialog::radiusWarning()
{
    Ccommon::editDoubleWarning(ui->radiusEdit,WARN_1_36F,36,1);
}

void CCapDialog::on_edgeColorBtn_clicked()
{
    m_edgeColor->setParentDialog(this);
    m_edgeColor->buttonClicked();
    m_cap->m_edgeLine.m_color = m_edgeColor->m_backColor;
    ((QWidget*)parent())->update();
}

void CCapDialog::on_bkColor1Btn_clicked()
{
    m_color1->setParentDialog(this);
    m_color1->buttonClicked();
    m_cap->m_backColor.m_color1 = m_color1->m_backColor;
    ((QWidget*)parent())->update();
}

void CCapDialog::on_bkColor2Btn_clicked()
{
    m_color2->setParentDialog(this);
    m_color2->buttonClicked();
    m_cap->m_backColor.m_color2 = m_color2->m_backColor;
    ((QWidget*)parent())->update();
}

void CCapDialog::on_edgeStyleBtn_clicked()
{
    m_edgeStyle->buttonClicked();
    m_cap->m_edgeLine.m_lineStyle = (lineStyle)m_edgeStyle->getIndex();
     ((QWidget*)parent())->update();
}

void CCapDialog::on_backGradientBtn_clicked()
{
    m_gradient->buttonClicked();
    m_cap->m_backColor.m_gradientStyle = (gradientStyle)m_gradient->getIndex();
     ((QWidget*)parent())->update();
}

void CCapDialog::on_capStyleBtn_clicked()
{
      m_capStyle->buttonClicked();
      m_cap->m_style = m_capStyle->getIndex();
      ((QWidget*)parent())->update();
}

