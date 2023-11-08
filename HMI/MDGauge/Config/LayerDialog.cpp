#include <QPainter>
#include <QMouseEvent>
#include <cmath>
#include "LayerDialog.h"
#include "ui_LayerDialog.h"
#include "Ccommon.h"
#include "MDMultiLanguage.h"

CLayerDialog::CLayerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CLayerDialog)
{
    ui->setupUi(this);
    m_xChange = 0;
    m_yChange = 0;
    m_widthChange = 0;
    m_hightChange = 0;
    m_setLeft = 3;
    m_setTop = 3;
    m_setWid = 94;
    m_setHgt = 94;
    m_rectLayer = QRectF(85,45,230,220);
    m_rectHight= QRectF(170,260,10,10);
    m_rectWidth = QRectF(310,125,10,10);
    m_rectX = QRectF(80,175,10,10);
    m_rectY = QRectF(220,40,10,10);
    m_rectMove= QRectF(90,50,220,210);
    m_rectL = QRectF(85,45,230,220);
    m_rectH= QRectF(220,310,10,10);
    m_rectW= QRectF(360,175,10,10);
    m_recX = QRectF(80,175,10,10);
    m_recY = QRectF(220,40,10,10);
    m_rectM= QRectF(90,50,220,210);
    m_isHover = false;
    m_ScaleMove= 0;
    this->initDialog();
    QStringList types;
    types << "QLineEdit";
    CMDMultiLanguage::instance()->translateWidget(this, &types);
}

CLayerDialog::~CLayerDialog()
{
    delete ui;
}

void CLayerDialog::initDialog()
{
    this->move(190,0);
    setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    hide();

    QObject::connect(ui->leftEdit,SIGNAL(textChanged(QString)),this,SLOT(setLeftValue(QString)));
    QObject::connect(ui->topEdit,SIGNAL(textChanged(QString)),this,SLOT(setTopValue(QString)));
    QObject::connect(ui->widEdit,SIGNAL(textChanged(QString)),this,SLOT(setWidthValue(QString)));
    QObject::connect(ui->heightEdit,SIGNAL(textChanged(QString)),this,SLOT(setHightValue(QString)));
    QObject::connect(ui->startValueEdit,SIGNAL(textChanged(QString)),this,SLOT(setStartValue(QString)));
    QObject::connect(ui->valueRangeEdit,SIGNAL(textChanged(QString)),this,SLOT(setValueRange(QString)));
    QObject::connect(ui->scountEdit,SIGNAL(textChanged(QString)),this,SLOT(setScout(QString)));
    QObject::connect(ui->startAngleEdit,SIGNAL(textChanged(QString)),this,SLOT(setStartAngle(QString)));
    QObject::connect(ui->AngleRangeEdit,SIGNAL(textChanged(QString)),this,SLOT(setAngleRange(QString)));
    connect(ui->leftEdit,SIGNAL(editingFinished()),this,SLOT(setLayer()));
    connect(ui->topEdit,SIGNAL(editingFinished()),this,SLOT(setLayer()));
    connect(ui->widEdit,SIGNAL(editingFinished()),this,SLOT(setLayer()));
    connect(ui->heightEdit,SIGNAL(editingFinished()),this,SLOT(setLayer()));


}
/*!
\section 将各个属性的值显示在配置界面上。
*/
void CLayerDialog::setData(CData *data)
{
    m_layer = (CDataLayer*)data;
    QRectF rectf = m_layer->getRectF();

    m_rectLayer = m_rectL = QRectF((85 + (rectf.x()-3)*230/100),(45 + (rectf.y()-3)*220/100),rectf.width()*230/94,rectf.height()*220/94);
    m_rectHight = m_rectH = QRectF(m_rectLayer.width()/2+m_rectLayer.x(),m_rectLayer.height()+m_rectLayer.y()-5,10,10);

    m_rectWidth = m_rectW= QRectF(m_rectLayer.width()+m_rectLayer.x()-5,(m_rectLayer.height())/2 +m_rectLayer.y(),10,10);
    m_rectX = m_recX = QRectF(m_rectLayer.x()-5,(m_rectLayer.height())/2 +m_rectLayer.y(),10,10);
    m_rectY = m_recY = QRectF(m_rectLayer.width()/2+m_rectLayer.x(),m_rectLayer.y()-5,10,10);
    m_rectMove = m_rectM = QRectF((90+rectf.x()-3),(50+rectf.y() - 3),176+rectf.width(),rectf.height() +166);

    Ccommon::setEdit(ui->leftEdit,rectf.left());
    Ccommon::setEdit(ui->topEdit,rectf.top());
    Ccommon::setEdit(ui->widEdit,rectf.width());
    Ccommon::setEdit(ui->heightEdit,rectf.height());
    Ccommon::setEdit(ui->startValueEdit,m_layer->getMinValue());
    Ccommon::setEdit(ui->valueRangeEdit,m_layer->getValueRange());
    Ccommon::setEdit(ui->scountEdit,m_layer->getScaleCount());
    Ccommon::setEdit(ui->startAngleEdit,m_layer->getStartAngle());
    Ccommon::setEdit(ui->AngleRangeEdit,m_layer->getAngleRange());

    ui->horRadioBtn->setChecked(m_layer->m_isHori);
    ui->vertRadioBtn->setChecked(!m_layer->m_isHori);
}
//!  控制层移动缩放时对控制层进行重画。
void CLayerDialog::paintEvent(QPaintEvent *)
{
    QPainter* painter = new QPainter(this);
    painter->setPen(QPen(Qt::NoPen));
    QRect layer1(10,14,370,280);
    QRect layer2(80,39,240,230);
    QPainterPath path;
    path.addRect(layer1);
    path.addRect(layer2);
    painter->setClipRect(10,14,370,280);
    painter->setBrush(QColor(0,0,0,120));
    painter->drawPath(path);
    painter->setBrush(QColor(255,255,255));
    painter->drawRect(layer2);
    QPen pen;
    pen.setStyle(Qt::DotLine);
    pen.setWidth(1);
    painter->setPen(pen);
    painter->setBrush(QColor(0, 130, 255,150));
    painter->drawRect(m_rectLayer);
    painter->setBrush(QColor(255,255,255,255));
    painter->setPen(Qt::SolidLine);
    painter->drawRect(m_rectHight);
    painter->drawRect(m_rectWidth);
    painter->drawRect(m_rectX);
    painter->drawRect(m_rectY);
    delete painter;
}
/*!
\section 控制层的实现。
*/
void CLayerDialog::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_rectL =m_rectLayer;
        m_rectH = m_rectHight;
        m_rectW = m_rectWidth;
        m_recX = m_rectX;
        m_recY = m_rectY;
        m_rectM = m_rectMove;

        m_lastPnt = event->pos();
        /*!
        \section 鼠标左键按下且鼠标在控制层拖动的矩形内，则可移动控制层。
        */
        if(m_rectMove.contains(event->pos()) == true)
        {
            m_isHover = true;
            setCursor(Qt::SizeAllCursor);
            m_ScaleMove = 0;
        }
        /*!
        \section 鼠标左键按下且鼠标在控制层右边线的矩形内，
                       则可从右向左边拖动控制层使其变小。
        */
        if(m_rectWidth.contains(event->pos()) == true)
        {
            m_isHover = false;
            setCursor(Qt::SizeHorCursor);
            m_ScaleMove = 3;
        }
        /*!
        \section 鼠标左键按下且鼠标在控制层下边线的矩形内，
                       则可从下边向上拖动控制层使其变小。
        */
        if(m_rectHight.contains(event->pos()) == true)
        {
            m_isHover = false;
            setCursor(Qt::SizeVerCursor);
            m_ScaleMove = 4;
        }
        /*!
        \section 鼠标左键按下且鼠标在控制层左边线的矩形内，
                       则可从左边向右拖动控制层使其变小。
        */
        if(m_rectX.contains(event->pos()) == true)
        {
            m_isHover = false;
            setCursor(Qt::SizeHorCursor);
            m_ScaleMove = 1;
        }
        /*!
        \section 鼠标左键按下且鼠标在控制层上边线的矩形内，
                       则可从上边向下拖动控制层使其变小。
        */
        if(m_rectY.contains(event->pos()) == true)
        {
            m_isHover = false;
            setCursor(Qt::SizeVerCursor);
            m_ScaleMove = 2;
        }
    }
}
/*!
\section 对控制进行横向或纵向移动从而改变绘图位置。
*/
void CLayerDialog::mouseMoveEvent(QMouseEvent *event)
{
      if(m_isHover == true)
      {
       qreal dx = event->pos().x() - m_lastPnt.x();
       qreal dy=event->pos().y()-m_lastPnt.y();
       qreal xL = m_rectL.x()+dx;
       qreal yL = m_rectL.y()+dy;
       m_rectLayer.moveTo(xL,yL);
       qreal xH = m_rectH.x()+dx;
       qreal yH = m_rectH.y()+dy;
       m_rectHight.moveTo(xH,yH);
       qreal xW = m_rectW.x()+dx;
       qreal yW = m_rectW.y()+dy;
       m_rectWidth.moveTo(xW,yW);
       qreal x_x = m_recX.x()+dx;
       qreal x_y = m_recX.y()+dy;
       m_rectX.moveTo(x_x,x_y);
       qreal y_x = m_recY.x()+dx;
       qreal y_y = m_recY.y()+dy;
       m_rectY.moveTo(y_x,y_y);
       qreal xM = m_rectM.x()+dx;
       qreal yM = m_rectM.y()+dy;
       m_rectMove.moveTo(xM,yM);

       m_xChange = (3 + m_rectLayer.x() - 85)/230*100;
       m_yChange = (3 + m_rectLayer.y() - 45)/220*100;

       QString retx;
       retx = QString::number(m_xChange,'f',1);
       ui->leftEdit->setText(retx);

       QString rety;
       rety = QString::number(m_yChange,'f',1);
       ui->topEdit->setText(rety);
     }
    if(m_ScaleMove == 4)
    {
        m_isHover = false;
        m_hightChange = (220- m_rectLayer.height())/220*94;

        qreal dy=event->pos().y()-m_lastPnt.y();
        m_rectLayer = QRectF(m_rectL.x(),m_rectL.y(),m_rectL.width(),m_rectL.height() + dy);
        m_rectMove = QRectF(m_rectM.x(),m_rectM.y(),m_rectM.width(),m_rectM.height() + dy);

        qreal yH = m_rectH.y()+dy;
        m_rectHight.moveTo(m_rectH.x(),yH);

        qreal yW = m_rectLayer.height()/2 + m_rectLayer.y();
        m_rectWidth.moveTo(m_rectWidth.x(),yW);

        qreal x_y = m_rectLayer.height()/2 + m_rectLayer.y();
        m_rectX.moveTo(m_rectX.x(),x_y);

        m_rectY.moveTo(m_recY.x(),m_recY.y());

        QString retx;
        if(94-m_hightChange > 0)
        {
            retx = QString::number((94-m_hightChange),'f',1);
        }
        else
            retx = QString::number((m_hightChange-94),'f',1);
        ui->heightEdit->setText(retx);
    }
    if(m_ScaleMove ==3)
    {

        m_isHover = false;
        m_widthChange = (230 - m_rectLayer.width())*94/230;

        qreal dx=event->pos().x()-m_lastPnt.x();
        m_rectLayer = QRect(m_rectL.x(),m_rectL.y(),m_rectL.width() + dx,m_rectL.height());

        qreal xH = m_rectLayer.width() / 2 +m_rectLayer.x();
        m_rectHight.moveTo(xH,m_rectHight.y());

        qreal xW = m_rectW.x()+dx;
        m_rectWidth.moveTo(xW,m_rectW.y());

        qreal y_x = m_rectLayer.width()/2 + m_rectLayer.x();
        m_rectY.moveTo(y_x,m_rectY.y());

        m_rectX.moveTo(m_recX.x(),m_recX.y());

        QString retx;
        if(94-m_widthChange > 0)
        {
            retx = QString::number((94-m_widthChange),'f',1);
        }
        else
            retx = QString::number((m_widthChange-94),'f',1);
        ui->widEdit->setText(retx);
    }
    if(m_ScaleMove == 1)
    {
        m_isHover = false;
        m_widthChange = (230 - m_rectLayer.width())*94/230;
        qreal dx=event->pos().x()-m_lastPnt.x();
        m_rectLayer = QRect(m_rectL.x()+dx,m_rectL.y(),m_rectL.width() - dx,m_rectL.height());

        qreal xH = m_rectLayer.width() / 2 +m_rectLayer.x();
        m_rectHight.moveTo(xH,m_rectHight.y());


        m_rectWidth.moveTo(m_rectW.x(),m_rectW.y());

        qreal y_x = m_rectLayer.width()/2 + m_rectLayer.x();
        m_rectY.moveTo(y_x,m_rectY.y());

        m_rectX.moveTo(m_recX.x()+dx,m_recX.y());

        QString retx;
        if(94-m_widthChange > 0)
        {
            retx = QString::number((94-m_widthChange),'f',1);
        }
        else
            retx = QString::number((m_widthChange-94),'f',1);
        ui->widEdit->setText(retx);

        m_xChange = (3 + m_rectLayer.x() - 85)/100*85;
        QString xCh;
        xCh = QString::number(m_xChange,'f',1);
        ui->leftEdit->setText(xCh);
    }
    if(m_ScaleMove == 2)
    {
        m_isHover = false;
        m_hightChange = (220- m_rectLayer.height())/220*94;

        qreal dy=event->pos().y()-m_lastPnt.y();
        m_rectLayer = QRectF(m_rectL.x(),m_rectL.y()+dy,m_rectL.width(),m_rectL.height() - dy);
        m_rectMove = QRectF(m_rectM.x(),m_rectM.y()+dy,m_rectM.width(),m_rectM.height() - dy);
        m_rectHight.moveTo(m_rectH.x(),m_rectH.y());

        qreal yW = m_rectLayer.height()/2 + m_rectLayer.y();
        m_rectWidth.moveTo(m_rectWidth.x(),yW);

        qreal x_y = m_rectLayer.height()/2 + m_rectLayer.y();
        m_rectX.moveTo(m_rectX.x(),x_y);
        m_rectY.moveTo(m_recY.x(),m_recY.y()+dy);

        QString retx;
        if(94-m_hightChange > 0)
        {
            retx = QString::number((94-m_hightChange),'f',1);
        }
        else
            retx = QString::number((m_hightChange-94),'f',1);
        ui->heightEdit->setText(retx);

        m_yChange = (3 + m_rectLayer.y() - 45)/100*45;
        QString rety;
        rety = QString::number(m_yChange,'f',1);
        ui->topEdit->setText(rety);

    }

    this->update();
}

void CLayerDialog::mouseReleaseEvent(QMouseEvent*)
{
    setCursor(Qt::ArrowCursor);
    m_isHover = false;
    m_ScaleMove = 5;
}
/*!
\section 设置控制层所在位置以及大小。
*/
void CLayerDialog::setLeftValue(QString str)
{
    double val = str.toDouble();
    m_setLeft = val;
    QRectF rect(m_setLeft,m_setTop,m_setWid,m_setHgt);
    m_layer->setRectF(rect);
    ((QWidget*)parent())->update();
}

void CLayerDialog::setTopValue(QString str)
{
    double val = str.toDouble();
    m_setTop = val;
    QRectF rect(m_setLeft,m_setTop,m_setWid,m_setHgt);
    m_layer->setRectF(rect);
    ((QWidget*)parent())->update();
}

void CLayerDialog::setWidthValue(QString str)
{
    double val = str.toDouble();
    m_setWid = val;
    QRectF rect(m_setLeft,m_setTop,m_setWid,m_setHgt);
    m_layer->setRectF(rect);
    ((QWidget*)parent())->update();
}

void CLayerDialog::setHightValue(QString str)
{
    double val = str.toDouble();
    m_setHgt = val;
    QRectF rect(m_setLeft,m_setTop,m_setWid,m_setHgt);
    m_layer->setRectF(rect);
    ((QWidget*)parent())->update();
}
/*!
\section 设置仪表的刻度的开始值以及值范围。
*/
void CLayerDialog::setStartValue(QString str)
{
    double val = str.toDouble();
    m_layer->setMinValue(val);
    ((QWidget*)parent())->update();
}

void CLayerDialog::setValueRange(QString str)
{
    double val = str.toDouble();
    m_layer->setValueRange(val);
    ((QWidget*)parent())->update();
}
/*!
\section 设置仪表刻度数。
*/
void CLayerDialog::setScout(QString str)
{
    qint32 num = str.toInt();
    if(num>16)
    {
        num = 16;
    }
    if(num <2)
    {
        num = 2;
    }
    m_layer->setScaleCount(num);
    ((QWidget*)parent())->update();
}
/*!
\section 设置仪表指针的开始角度以及移动的角度范围。
*/
void CLayerDialog::setStartAngle(QString str)
{
    qint32 angle = str.toInt();
    m_layer->setStartAngle(angle);
    ((QWidget*)parent())->update();
}

void CLayerDialog::setAngleRange(QString str)
{
    qint32 angle = str.toInt();
    m_layer->setAngleRange(angle);
    ((QWidget*)parent())->update();
}

/*!
\section 对直表进行水平或竖直放置的设定。
*/
void CLayerDialog::on_horRadioBtn_clicked()
{
    ui->horRadioBtn->setChecked(true);
    ui->vertRadioBtn->setChecked(false);
    m_layer->m_isHori = true;
    ((QWidget*)parent())->update();
}

void CLayerDialog::on_vertRadioBtn_clicked()
{
    ui->horRadioBtn->setChecked(false);
    ui->vertRadioBtn->setChecked(true);
    m_layer->m_isHori = false;
    ((QWidget*)parent())->update();
}

void CLayerDialog::setLayer()
{
    QRectF rect = m_layer->getRectF();
    m_rectLayer.moveTo((85 + (rect.x()-3)*230/100),(45 + (rect.y()-3)*220/100));
    m_rectMove.moveTo((90+rect.x()-3),(50+rect.y() - 3));
    m_rectLayer = QRectF((85 + (rect.x()-3)*230/100),(45 + (rect.y()-3)*220/100),rect.width()*230/94,rect.height()*220/94);
    m_rectHight.moveTo(m_rectLayer.width()/2+m_rectLayer.x(),m_rectLayer.height()+m_rectLayer.y()-5);
    m_rectWidth.moveTo(m_rectLayer.width()+m_rectLayer.x()-5,(m_rectLayer.height())/2 +m_rectLayer.y());
    m_rectX.moveTo(m_rectLayer.x()-5,(m_rectLayer.height())/2 +m_rectLayer.y());
    m_rectY.moveTo(m_rectLayer.width()/2+m_rectLayer.x(),m_rectLayer.y()-5);
    ((QWidget*)parent())->update();
}
