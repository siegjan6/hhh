#include "DrawWidget.h"
#include "ui_DrawWidget.h"
#include "DesignForm.h"
#include "DrawAxis.h"
#include "DrawHistogram.h"
#include "DrawHistogram3D.h"
#include "DrawVernier.h"
#include "MDMultiLanguage.h"
#include "PrintForm.h"
#include "MDJsonConvert.h"
#include <QPainter>
#include <QMouseEvent>
#include <QFileDialog>
#include <QFileInfo>
#include <QtPrintSupport/QPrintDialog>
#include <QtPrintSupport/QPrintPreviewDialog>
#include <QtPrintSupport/QPrinter>
#include <QPageSetupDialog>
#include <QDateTime>
#include <QDebug>

CDrawWidget::CDrawWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CDrawWidget)
{
    ui->setupUi(this);
    m_rect = QRectF(50,70,400,300);
    m_paintRect = QRectF(50,70,400,300);

    m_printTop = 5;
    m_printBottom = 5;
    m_printLeft = 5;
    m_printRight = 5;
    m_printHeader = 8;
    m_printFooter = 8;
    m_printDateFormat = "yyyy-MM-dd";
    m_printTimeFormat = "hh:mm:ss";

    initialize();
    CMDMultiLanguage::instance()->translateWidget(this);
}

CDrawWidget::~CDrawWidget()
{
    if(m_drawData)
    {
        delete m_drawData;
        m_drawData = NULL;
    }
    delete ui;
}

void CDrawWidget::setRect(const QRectF &rect)
{
    m_rect = rect;
}

void CDrawWidget::setString1(QString string)
{
    m_stringList1.clear();
    QStringList strlist = string.split(",", QString::SkipEmptyParts);
    m_stringList1 = strlist;
    m_colorList.clear();
    for(qint32 i=0;i< m_stringList1.count();i++)
    {
        QColor c;
        c.setRgb(qrand()%255,qrand()%255,qrand()%255,255);
        m_colorList.append(c);
    }
    QRect rect = QRect(m_rect.x(),m_rect.y(),m_rect.width(),m_rect.height());
    update(rect);
}
void CDrawWidget::setString2(QString string)
{
    m_stringList2.clear();
    QStringList strlist = string.split(",", QString::SkipEmptyParts);
    m_stringList2 = strlist;
    m_colorList.clear();
    for(qint32 i=0;i< m_stringList2.count();i++)
    {
        QColor c;
        c.setRgb(qrand()%255,qrand()%255,qrand()%255,255);
        m_colorList.append(c);
    }
    QRect rect = QRect(m_rect.x(),m_rect.y(),m_rect.width(),m_rect.height());
    update(rect);
}

void CDrawWidget::setString3(QString string)
{
    m_stringList3.clear();
    QStringList strlist = string.split(",", QString::SkipEmptyParts);
    m_stringList3 = strlist;
    QRect rect = QRect(m_rect.x(),m_rect.y(),m_rect.width(),m_rect.height());
    update(rect);
}

void CDrawWidget::setValue(QString valueString)
{
    m_valueList.clear();
    QStringList strlist = valueString.split(",", QString::SkipEmptyParts);
    for(qint32 i = 0;i <strlist.count();i++)
    {
        qreal value = strlist.at(i).toDouble();
        m_valueList.append(value);
    }
    QRect rect = QRect(m_rect.x(),m_rect.y(),m_rect.width(),m_rect.height());
    update(rect);
}

void CDrawWidget::initialize()
{
    m_drawData = new CDrawData;
    this->setMouseTracking(true);
    m_vernier =new CDrawVernier(this);
    m_vernier->setVernierData(m_drawData);
    setString1(m_drawData->m_string1);
    setString2(m_drawData->m_string2);
    setString3(m_drawData->m_string3);
    setValue(m_drawData->m_valueString);

    qint32 count = m_stringList1.count();
    for(qint32 i=0;i< count;i++)
    {
        QColor c;
        c.setRgb(qrand()%255,qrand()%255,qrand()%255,255);
        m_colorList.append(c);
    }
}

void CDrawWidget::paintEvent(QPaintEvent* )
{
    QPainter *painter = new QPainter(this);
    painter->setRenderHint(QPainter::Antialiasing,true);
    painter->setClipRect(m_rect);

    drawToolBar(painter);
    drawDescribeString(painter,m_drawData->m_stringText,m_drawData->m_stringFont,
                       m_drawData->m_stringPosition,m_drawData->m_stringArrange);
    drawType(painter);
    drawAxis(painter);
    drawClicent(painter);
    drawItem(painter);

    delete painter;
}

void CDrawWidget::resizeEvent(QResizeEvent *e)
{
    int height  = e->size().height();
    int width = e->size().width();
    QRectF rect = QRectF(0,0,width,height);
    setRect(rect);
}

void CDrawWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(m_drawData->m_category == 0)
    {
        for(qint32 i = 0;i <m_pillarList.count();i++)
        {
            if(m_pillarList.at(i).contains(event->pos()) == true)
            {
                m_vernier->setNum(i);
                m_vernier->setValueList(m_valueList);
                m_vernier->setStringList1(m_stringList1);
                qint32 x = m_pillarList.at(i).x()+m_pillarList.at(i).width()/2-m_vernier->getWidth()/2;
                qint32 y = m_pillarList.at(i).y()+m_pillarList.at(i).height()/2-m_vernier->getheight()/2;
                m_vernier->move(x,y);
                m_vernier->show();
            }
        }
        m_vernier->close();
    }
    else if(m_drawData->m_category ==1)
    {
        if(m_itemRectList.count()>m_valueList.count())
            return;
        if(m_valueList.count() < m_stringList1.count()*m_stringList2.count())
            return;
        for(int i = 0;i <m_itemRectList.count();i++)
        {
            if(m_itemRectList.at(i).contains(event->pos()) == true)
            {
                m_vernier->setNum(i);
                m_vernier->setValueList(m_valueList);
                m_vernier->setStringList1(m_stringList1);
                m_vernier->setStringList2(m_stringList2);
                qint32 x = m_itemRectList.at(i).x()+m_itemRectList.at(i).width()/2-m_vernier->getWidth()/2;
                qint32 y = m_itemRectList.at(i).y()+m_itemRectList.at(i).height()/2-m_vernier->getheight()/2;
                m_vernier->move(x,y);
                m_vernier->show();
            }
        }
        m_vernier->close();
    }

}

void CDrawWidget::drawItem(QPainter *paint)
{
    switch(m_drawData->m_diagramStyle)
    {
    case 1:
    case 2:
    {
        CDrawHistogram histogram;
        histogram.setHistogramData(m_drawData);
        histogram.setHistogramRectList(m_itemRectList);
        histogram.setHistogramValueList(m_valueList);
        histogram.setHistogramColorList(m_colorList);
        histogram.setHistogramStringList2(m_stringList2);
        histogram.setHistogramStringList3(m_stringList3);
        histogram.drawHistogram(paint);
        m_pillarList.clear();
        m_pillarList = histogram.m_pillarRectList;
    }
        break;
    case 3:
    case 4:
    {
        CDrawHistogram3D histogram3D;
        histogram3D.setHistogramData3D(m_drawData);
        histogram3D.setDrawRect(m_drawRect);
        histogram3D.setInterRect(m_interRect);
        histogram3D.setColorList(m_colorList);
        histogram3D.setStringList1(m_stringList1);
        histogram3D.setStringList2(m_stringList2);
        histogram3D.setStringList3(m_stringList3);
        histogram3D.setValueList(m_valueList);
        histogram3D.setDrawRectList(m_drawRectList);
        histogram3D.setInterRectList(m_interRectList);
        histogram3D.drawHistogram3D(paint);
        m_pillarList.clear();
        m_pillarList = histogram3D.m_pillarRectList;
    }
        break;
    default:
        break;
    }
}

void CDrawWidget::drawToolBar(QPainter *paint)
{
    if(m_drawData->m_toolPosition == 2)
    {
        m_paintRect = m_rect;
        ui->histogramBtn->hide();
        ui->histogram2Btn->hide();
        ui->preprintBtn->hide();
        ui->printBtn->hide();
        ui->printSetBtn->hide();
        ui->histogram3DBtn->hide();
        ui->histogram3D2Btn->hide();
        paint->setBrush(m_drawData->m_backColor);
        paint->setPen(Qt::NoPen);
        paint->drawRect(m_paintRect);
    }
    else if(m_drawData->m_toolPosition == 0)
    {
        paint->setPen(Qt::NoPen);
        paint->setBrush(QColor(180,180,180,255));
        paint->drawRect(m_rect.x(),m_rect.y(),m_rect.width(),40);
        paint->setBrush(m_drawData->m_backColor);
        if(m_drawData->m_isSetEdge != 0)
        {
          paint->drawRect(m_rect.x()+5,m_rect.y() + 40,m_rect.width()-10,m_rect.height() -45);
        }
        else
            paint->drawRect(m_rect.x(),m_rect.y() + 40,m_rect.width(),m_rect.height() -40);

        m_paintRect = QRectF(m_rect.x(),m_rect.y() +40,m_rect.width(),m_rect.height()-40);
        ui->histogramBtn->move(m_rect.x() + 10, m_rect.y() + 5);
        ui->histogram2Btn->move(m_rect.x() + 50, m_rect.y() + 5);

        ui->histogram3DBtn->move(m_rect.x() + 90, m_rect.y() + 5);
        ui->histogram3D2Btn->move(m_rect.x() + 130, m_rect.y() + 5);
        ui->preprintBtn->move(m_rect.x() + 170, m_rect.y() + 5);
        ui->printBtn->move(m_rect.x() + 240, m_rect.y() + 5);
        ui->printSetBtn->move(m_rect.x() + 310, m_rect.y() + 5);
        ui->histogramBtn->show();
        ui->histogram2Btn->show();
        ui->histogram3DBtn->show();
        ui->histogram3D2Btn->show();
        ui->preprintBtn->show();
        ui->printBtn->show();
        ui->printSetBtn->show();
    }
    else
    {
        paint->setPen(Qt::NoPen);
        paint->setBrush(QColor(0,0,0,32));
        paint->drawRect(m_rect.x(),m_rect.y()+m_rect.height()-40,m_rect.width(),40);
        paint->setBrush(m_drawData->m_backColor);
        paint->drawRect(m_rect.x(),m_rect.y(),m_rect.width(),m_rect.height()-40);
        m_paintRect = QRectF(m_rect.x(),m_rect.y(),m_rect.width(),m_rect.height()-40);
        ui->histogramBtn->move(m_rect.x() + 10, m_rect.y()+ m_rect.height()-35);
        ui->histogram2Btn->move(m_rect.x() + 50, m_rect.y()+ m_rect.height()-35);

        ui->histogram3DBtn->move(m_rect.x() + 90, m_rect.y()+ m_rect.height()-35);
        ui->histogram3D2Btn->move(m_rect.x() + 130, m_rect.y()+ m_rect.height()-35);
        ui->preprintBtn->move(m_rect.x() + 170, m_rect.y()+ m_rect.height()-35);
        ui->printBtn->move(m_rect.x() + 240, m_rect.y()+ m_rect.height()-35);
        ui->printSetBtn->move(m_rect.x()+ 310, m_rect.y()+ m_rect.height()-35);
        ui->histogramBtn->show();
        ui->histogram2Btn->show();
        ui->histogram3DBtn->show();
        ui->histogram3D2Btn->show();
        ui->preprintBtn->show();
        ui->printBtn->show();
        ui->printSetBtn->show();
    }
}

void CDrawWidget::drawDescribeString(QPainter *p, QString str, QFont font, int position, int arrange)
{
    p->setPen(m_drawData->m_stringColor);
        p->setFont(font);
        QRectF r = m_paintRect;
        QFontMetrics fm(font);
        qreal fontwidth = fm.width(str);
        qreal fonthight = fm.height();
        QPointF pf;
        qreal fontPointSize = fm.ascent();
        if(fontPointSize <= 18)
        {
            fontPointSize += 3;
        }
        if(position == 1)
        {
            switch(arrange)
            {
            case 1:
                pf = QPointF(r.x()+5,r.y() + fontPointSize);
                break;
            case 2:
                pf = QPointF(r.x() + r.width() / 2 - fontwidth/2,r.y() + fontPointSize);
                break;
            case 3:
                pf = QPointF(r.x() +r.width() - fontwidth -5,r.y() + fontPointSize);
                break;
            default:
                break;
            }
        }
        if(position == 2)
        {
            switch(arrange)
            {
            case 1:
                pf = QPointF(r.x()+5,r.y() - fonthight/5 + r.height()-5);
                break;
            case 2:
                pf = QPointF(r.x() + r.width() / 2 - fontwidth/2 +5,r.y() - fonthight/5  + r.height()-5);
                break;
            case 3:
                pf = QPointF(r.x() +r.width() - fontwidth -5,r.y() - fonthight/5 + r.height()-5);
                break;
            default:
                break;
            }
        }
        p->drawText(pf,str);

}

void CDrawWidget::drawAxis(QPainter *paint)
{
    CDrawAxis drawAxis;
    drawAxis.setAxisData(m_drawData);
    drawAxis.setXString(m_stringList1);
    drawAxis.drawAxis(paint,drawAxisRect(m_paintRect));
    if(m_drawData->m_diagramStyle ==1 ||m_drawData->m_diagramStyle ==2)
    {
       m_itemRectList = drawAxis.getRectList();
    }
    if(m_drawData->m_diagramStyle ==3 ||m_drawData->m_diagramStyle ==4)
    {
       m_itemRectList = drawAxis.m_drawRectList;
    }
    m_drawRect = drawAxis.m_drawRect;
    m_interRect = drawAxis.m_interRect;
    m_drawRectList = drawAxis.m_drawRectList;
    m_interRectList = drawAxis.m_interRectList;
}

QRectF CDrawWidget::drawAxisRect(const QRectF &rect)
{
    QRectF r = drawTypeRect(rect);
    qreal x,y,width,hight;
    x = r.x();
    y = r.y();
    width = r.width();
    hight = r.height();
    switch(m_drawData->m_typePosition)
    {
    case 0:
        y = y + getTypeHight();
        hight = hight - getTypeHight();
        break;
    case 1:
        hight = hight - getTypeHight();
        break;
    case 2:
        x = x + getTypeWidth()-10;
        width = width - getTypeWidth()+10;
        break;
    case 3:
        width = width - getTypeWidth()+10;
        break;
    case 4:
        y = y-6;
        hight = hight +6;
        break;
    default:
        break;
    }

    r = QRectF(x,y,width,hight);
    return r;
}

void CDrawWidget::drawClicent(QPainter *paint)
{
    if(m_drawData->m_isSetEdge != 0)
    {
        QPen pen1;
        pen1.setWidth(3);
        pen1.setColor(QColor(128,128,128,76));
        paint->setPen(pen1);
        paint->setBrush(Qt::NoBrush);
        paint->drawRect(m_paintRect);
        QPen pen2;
        pen2.setWidth(2);
        pen2.setColor(QColor(58,58,58,108));
        paint->setPen(pen2);
        paint->drawRect(m_paintRect.x()+3,m_paintRect.y()+3,m_paintRect.width()-7,m_paintRect.height()-7);
    }
}

void CDrawWidget::drawType(QPainter *p)
{
    if(m_drawData->m_category == 0)
    {
        for(qint32 i=0;i< m_stringList1.count();i++)
        {
            QColor c;
            c.setRgb(qrand()%255,qrand()%255,qrand()%255,255);
            m_colorList.append(c);
        }
    }

    QRectF typeRect = drawTypeRect(m_paintRect);
    QFontMetrics fm(m_drawData->m_typeValueFont);
    qint32 stringHight = fm.height()+6;
    qint32 stringWidth = getTypeWidth();
    qint32 count;
    QList<QString>stringList;
    switch (m_drawData->m_category)
    {
    case 0:
        count = m_stringList1.count();
        stringList = m_stringList1;
        break;
    case 1:
    case 2:
        count = m_stringList2.count();
        stringList = m_stringList2;
        break;
    default:
        break;
    }

    if(/*m_drawData->m_category ==1 ||*/ m_drawData->m_category ==2)
    {
        m_colorList.clear();
        for(qint32 i=0;i< count;i++)
        {
            QColor c;
            c.setRgb(qrand()%255,qrand()%255,qrand()%255,255);
            m_colorList.append(c);
        }
    }
    p->setFont(m_drawData->m_typeValueFont);
    p->setPen(QColor(0,0,0,255));

    qint32 typeNum = typeRect.width()/stringWidth;
    if(typeNum <1)
        return;
    qint32 rowNum = count / typeNum;
    qint32 leftNum = count % typeNum;
    if(!m_drawData->m_stringText.isEmpty())
    {
        if(rowNum == 0 || (rowNum ==1 && leftNum == 0))
        {
            typeRect = QRectF(typeRect.x(),typeRect.y() -stringHight/4,typeRect.width(),typeRect.height() +stringHight/4);
        }
        else
        {
             typeRect = QRectF(typeRect.x(),typeRect.y() +stringHight/4,typeRect.width(),typeRect.height() -stringHight/4);
        }
    }

    qint32 j = 0;
    qint32 t = 0;
    qint32 ibottom = 0;
    for(qint32 i = 0;i<count;i++)
    {
        //绘制图例文本
        switch(m_drawData->m_typePosition)
        {
        case 0:
        {
            if(rowNum == 0 && leftNum != 0)
            {

                p->drawText(typeRect.x()+typeRect.width()/2 - count*stringWidth/2 +stringHight+i*stringWidth,
                            typeRect.y()+fm.height(),stringList.at(i));

            }
            else if(rowNum !=0 && leftNum == 0)
            {
                if(rowNum ==1 && leftNum == 0)
                {
                    p->drawText(typeRect.x()+typeRect.width()/2 - count*stringWidth/2 +stringHight+i*stringWidth,
                                typeRect.y()+fm.height(),stringList.at(i));
                }
                else
                {
                    if(j < rowNum)
                        if(t < typeNum)
                        {
                            {
                                p->drawText(typeRect.x()+typeRect.width()/2 - typeNum*stringWidth/2+stringHight+t*stringWidth,
                                            typeRect.y()+stringHight*(j+0.5),stringList.at(i));
                                t++;
                            }
                        }
                    if(t == typeNum)
                    {
                        j++;
                        t =0;
                    }
                }

            }
            else if(rowNum != 0 && leftNum != 0)
            {
                if(j < rowNum+1)
                    if(t < typeNum)
                    {

                        {
                            p->drawText(typeRect.x()+typeRect.width()/2 - typeNum*stringWidth/2 +stringHight+t*stringWidth,
                                        typeRect.y()+stringHight*(j+0.5),stringList.at(i));
                            t++;
                        }
                    }
                if(t == typeNum)
                {
                    j++;
                    t =0;
                }

            }

        }

            break;
        case 1:
        {
            if(rowNum == 0&& leftNum != 0)
            {
                {
                    p->drawText(typeRect.x()+typeRect.width()/2 - count*stringWidth/2 +stringHight+i*stringWidth,
                                typeRect.y()+typeRect.height()-5,stringList.at(i));
                }

            }
            else if(rowNum !=0 && leftNum == 0)
            {
                if(count == typeNum)
                {
                    rowNum -= ibottom;
                    if(rowNum>0)
                        if(t < typeNum)
                        {
                            {
                                p->drawText(typeRect.x()+typeRect.width()/2 - typeNum*stringWidth/2+stringHight +t*stringWidth,
                                            typeRect.y()+typeRect.height()-stringHight*(rowNum-1)-5,stringList.at(i));
                                t++;
                            }

                        }
                    if(t == typeNum)
                    {
                        t =0;
                        ibottom++;
                    }
                }
                else
                {
                    if(j < rowNum)
                        if(t < typeNum)
                        {
                            {
                                p->drawText(typeRect.x()+typeRect.width()/2 - typeNum*stringWidth/2+stringHight+t*stringWidth,
                                            typeRect.y()+ typeRect.height() - stringHight*j,stringList.at(i));
                                t++;
                            }
                        }
                    if(t == typeNum)
                    {
                        j++;
                        t =0;
                    }
                }
            }
            else if(rowNum != 0 && leftNum != 0)
            {
                qint32 num = rowNum;
                num -= ibottom;
                if(rowNum>-1)
                    if(t < typeNum)
                    {

                        {
                            p->drawText(typeRect.x()+typeRect.width()/2 - typeNum*stringWidth/2 +stringHight+t*stringWidth,
                                        typeRect.y()+typeRect.height()-stringHight*num-5,stringList.at(i));
                            t++;
                        }

                    }
                if(t == typeNum)
                {
                    t =0;
                    ibottom++;
                }
            }
        }

            break;
        case 2:
            p->drawText(typeRect.x()+15+fm.height()/2,
                        typeRect.y()+typeRect.height()/2-(count-1)*stringHight/2+i*stringHight+fm.height()/2,
                        stringList.at(i));
            break;
        case 3:
            p->drawText(typeRect.x()+typeRect.width()-getTypeWidth()+fm.height()+15,
                        typeRect.y()+typeRect.height()/2-(count-1)*stringHight/2+i*stringHight+fm.height()/2,
                        stringList.at(i));
            break;
        case 4:

            break;
        }
    }
    //绘制图例显示颜色框
    j = 0;
    t = 0;
    ibottom = 0;
    for(qint32 i = 0;i<count;i++)
    {
        p->setBrush(m_colorList.at(i));
        switch(m_drawData->m_typePosition)
        {
        case 0:
        {
            if(rowNum == 0&& leftNum != 0)
            {

                p->drawRect(typeRect.x()+typeRect.width()/2 - count*stringWidth/2 +stringHight+i*stringWidth-fm.height()/2-5,
                            typeRect.y()+fm.height()-fm.height()/2,fm.height()/2,fm.height()/2);

            }
            else if(rowNum !=0 && leftNum == 0)
            {
              if(rowNum ==1 && leftNum == 0)
              {
                  p->drawRect(typeRect.x()+typeRect.width()/2 - count*stringWidth/2 +stringHight+i*stringWidth-fm.height()/2-5,
                              typeRect.y()+fm.height()-fm.height()/2,fm.height()/2,fm.height()/2);
              }
              else
              {
                  if(j < rowNum)
                      if(t < typeNum)
                      {
                          {
                              p->drawRect(typeRect.x()+typeRect.width()/2 - typeNum*stringWidth/2+stringHight +t*stringWidth-fm.height()/2-5,
                                          typeRect.y()+stringHight*(j+0.5)-fm.height()/2,fm.height()/2,fm.height()/2);
                              t++;
                          }

                      }
                  if(t == typeNum)
                  {
                      j++;
                      t =0;
                  }
              }
            }
            else if(rowNum != 0 && leftNum != 0)
            {
                if(j < rowNum+1)
                    if(t < typeNum)
                    {

                        {
                            p->drawRect(typeRect.x()+typeRect.width()/2 - typeNum*stringWidth/2 +stringHight+t*stringWidth-fm.height()/2-5,
                                        typeRect.y()+stringHight*(j+0.5)-fm.height()/2,fm.height()/2,fm.height()/2);
                            t++;
                        }

                    }
                if(t == typeNum)
                {
                    j++;
                    t =0;
                }

            }

        }
            break;
        case 1:
        {
            if(rowNum == 0&& leftNum != 0)
            {
                {
                    p->drawRect(typeRect.x()+typeRect.width()/2 - count*stringWidth/2 +stringHight+i*stringWidth-fm.height()/2-5,
                                typeRect.y()+typeRect.height()-5-fm.height()/2,fm.height()/2,fm.height()/2);
                }

            }
            else if(rowNum !=0 && leftNum == 0)
            {
                if(count == typeNum)
                {
                    rowNum -= ibottom;
                    if(rowNum>0)
                        if(t < typeNum)
                        {
                            {
                                p->drawRect(typeRect.x()+typeRect.width()/2 - typeNum*stringWidth/2+stringHight +t*stringWidth-fm.height()/2-5,
                                            typeRect.y()+typeRect.height()-stringHight*(rowNum-1)-5-fm.height()/2,fm.height()/2,fm.height()/2);
                                t++;
                            }

                        }
                    if(t == typeNum)
                    {
                        t =0;
                        ibottom++;
                    }
                }
                else
                {
                    if(j < rowNum)
                        if(t < typeNum)
                        {
                            {
                                p->drawRect(typeRect.x()+typeRect.width()/2 - typeNum*stringWidth/2+stringHight+t*stringWidth-fm.height()/2-5,
                                            typeRect.y()+ typeRect.height() - stringHight*j-fm.height()/2,fm.height()/2,fm.height()/2);
                                t++;
                            }
                        }
                    if(t == typeNum)
                    {
                        j++;
                        t =0;
                    }
                }
            }
            else if(rowNum != 0 && leftNum != 0)
            {
                qint32 num = rowNum;
                num -= ibottom;
                if(rowNum>-1)
                    if(t < typeNum)
                    {

                        {
                            p->drawRect(typeRect.x()+typeRect.width()/2 - typeNum*stringWidth/2 +stringHight+t*stringWidth-fm.height()/2-5,
                                        typeRect.y()+typeRect.height()-stringHight*num-5-fm.height()/2,fm.height()/2,fm.height()/2);
                            t++;
                        }

                    }
                if(t == typeNum)
                {
                    t =0;
                    ibottom++;
                }
            }
        }


            break;
        case 2:
            p->drawRect(typeRect.x()+10,
                        typeRect.y()+typeRect.height()/2-(count-1)*stringHight/2+(i*stringHight),
                        fm.height()/2,fm.height()/2);
            break;
        case 3:
            p->drawRect(typeRect.x()+typeRect.width()-getTypeWidth()+fm.height()/2+10,
                        typeRect.y()+typeRect.height()/2-(count-1)*stringHight/2+(i*stringHight),
                        fm.height()/2,fm.height()/2);
            break;
        case 4:

            break;
        }
    }

}

QRectF CDrawWidget::drawTypeRect(const QRectF &rect)
{
    qint32 textHight;
    if(!m_drawData->m_stringText.isEmpty())
    {
        QFontMetrics fm(m_drawData->m_stringFont);
        textHight = fm.height();
    }
    else
        textHight = 0;

    QRectF r = rect;
    qreal x,y,width,height;
    if(m_drawData->m_stringPosition == 1)
    {
        x = r.x();
        y = r.y() + textHight;
        width = r.width();
        height = r.height() - textHight;

    }
    else
    {
        x = r.x();
        y = r.y();
        width = r.width();
        if(!m_drawData->m_stringText.isEmpty())
        {
            height = r.height()- textHight+textHight/5;
        }
        else
            height = r.height()- textHight;

    }
    if(m_drawData->m_isSetEdge != 0)
    {
        x = x+5;
        y = y+5;
        width = width -12;
        height = height -13;
    }

    r = QRectF(x,y,width,height);
    return r;
}

qint32 CDrawWidget::getTypeWidth()
{
    qint32 count;
        QList<QString>stringList;
        switch (m_drawData->m_category) {
        case 0:
            count = m_stringList1.count();
            stringList = m_stringList1;
            break;
        case 1:
        case 2:
            count = m_stringList2.count();
            stringList = m_stringList2;
            break;
        default:
            break;
        }
        qint32 *stringWidth = new qint32[count];
//        qint32 stringWidth[count];
        QFontMetrics fm(m_drawData->m_typeValueFont);
        for (qint32 i = 0; i < count; i++)
        {
            qint32 width = fm.width(stringList.at(i));
            stringWidth[i] = width;
        }
        qint32 maxStringWidth = stringWidth[0];
        for (qint32 i=0;i<count;i++)
        {
            if(maxStringWidth <= stringWidth[i+1])
            {
                maxStringWidth = stringWidth[i+1];
            }
        }

        qint32 valueHight = fm.height();
        maxStringWidth = valueHight+maxStringWidth+20;
        return maxStringWidth;
}

qint32 CDrawWidget::getTypeHight()
{
    qint32 uphight = 0;
    QRectF typeRect = drawTypeRect(m_paintRect);
    QFontMetrics fm(m_drawData->m_typeValueFont);
    qint32 valueHight = fm.height()+6;
    qint32 stringWidth = getTypeWidth();
    qint32 count;
    switch (m_drawData->m_category)
    {
    case 0:
        count = m_stringList1.count();
        break;
    case 1:
    case 2:
        count = m_stringList2.count();
        break;
    default:
        break;
    }
    qint32 typeNum = typeRect.width()/stringWidth;
    if(typeNum <1)
        return 0;
    qint32 rowNum = count / typeNum;
    qint32 leftNum = count % typeNum;
    if(rowNum == 0 && leftNum != 0)
    {
        uphight = valueHight;
    }
    else if(rowNum != 0 && leftNum == 0)
    {
        uphight = valueHight*rowNum;
    }
    else if(rowNum != 0 && leftNum != 0)
    {
        uphight = valueHight*(rowNum+1);
    }
    return uphight;
}

bool CDrawWidget::openDesign()
{
    CDesignForm design(this) ;
    design.m_drawWidget = this;
    design.setDesignData(m_drawData);
    design.inintDesign();

    if(design.exec() == QDialog::Accepted)
    {
        *m_drawData = *design.getData();
        setValue(m_drawData->m_valueString);
        setString1(m_drawData->m_string1);
        setString2(m_drawData->m_string2);
        setString3(m_drawData->m_string3);

        return true;
    }
    return false;
}

void CDrawWidget::on_histogramBtn_clicked()
{
    m_drawData->m_diagramStyle = 1;
    update();
}

void CDrawWidget::on_histogram2Btn_clicked()
{
   m_drawData->m_diagramStyle = 2;
   update();
}

void CDrawWidget::on_histogram3DBtn_clicked()
{
    m_drawData->m_diagramStyle = 3;
    update();
}

void CDrawWidget::on_histogram3D2Btn_clicked()
{
    m_drawData->m_diagramStyle = 4;
    update();
}

void CDrawWidget::on_preprintBtn_clicked()
{
    doPrintPreview();
}

void CDrawWidget::on_printBtn_clicked()
{
    doPrint();
}

void CDrawWidget::on_printSetBtn_clicked()
{
    printConfig();
}

void CDrawWidget::doPrint()
{
    QPrinter printer;
    setPrintMargins(&printer);
    QPrintDialog dlg(&printer, this);
    if (dlg.exec() == QDialog::Accepted)
        print(&printer);
}

void CDrawWidget::doPrintPreview()
{
    QPrinter printer;
    setPrintMargins(&printer);
    QPrintPreviewDialog preview(&printer, this);
    connect(&preview, SIGNAL(paintRequested(QPrinter*)),
                  this,SLOT(print(QPrinter*)));
    preview.exec();
}

void CDrawWidget::setUpPage()
{
    QPrinter printer;
    QPageSetupDialog pageSetUpdlg(&printer, this);
    if (pageSetUpdlg.exec() == QDialog::Accepted)
    {
        printer.setOrientation(QPrinter::Landscape);
    }
    else
    {
        printer.setOrientation(QPrinter::Portrait);
    }
}

void CDrawWidget::createPdf()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("导出PDF文件"), QString(), "*.pdf");
    if (!fileName.isEmpty()) {
        // 如果文件后缀为空，则默认使用.pdf
        if (QFileInfo(fileName).suffix().isEmpty())
            fileName.append(".pdf");
        QPrinter printer;
        // 指定输出格式为pdf
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setOutputFileName(fileName);
        print(&printer);
    }
}

CDrawWidget &CDrawWidget::operator =(const CDrawWidget &other)
{
    if(this == &other)
    {
        return *this;
    }
    CDrawWidget* d = (CDrawWidget*)&other;
    this->m_rect = d->m_rect;

    this->m_stringList1 = d->m_stringList1;
    this->m_stringList2 = d->m_stringList2;
    this->m_stringList3 = d->m_stringList3;
    this->m_valueList = d->m_valueList;
    * this->m_drawData = *d->m_drawData;
    return *this;
}

void CDrawWidget::serialize(QJsonObject &json)
{
    json.insert("Rect",CMDJsonConvert::toJson(m_rect));
    m_drawData->serialize(json);
}

void CDrawWidget::deserialize(const QJsonObject &json)
{
    m_rect =CMDJsonConvert::toQRectF(json.value("Rect"));
    m_drawData->deserialize(json);
}

void CDrawWidget::print(QPrinter *printer)
{
    QRect rect(QPoint(0,0),size());
    printFun(printer, rect);
}

void CDrawWidget::setPrintMargins(QPrinter *printer)
{
    printer->setPageMargins(m_printLeft, m_printTop+m_printHeader,
        m_printRight, m_printBottom+m_printFooter, QPrinter::Millimeter);
}

void CDrawWidget::printConfig()
{
    CPrintForm dlg(window());
    dlg.init(this);
    if(dlg.exec() == QDialog::Accepted)
    {
        collectWords();
        changeLanguage();
    }
}

void CDrawWidget::printFun(QPrinter *printer, const QRect &rect)
{
    QPainter p(printer);
    p.setRenderHint(QPainter::Antialiasing, true);
    QRect pageRect = printer->pageRect();
    QSize size = rect.size();

    //calculate rect
    int w = pageRect.width();
    int h = pageRect.height();
    int row = size.height() / h + 1;
    int col = size.width() / w + 1;
    int pageCount = row * col;

    for (int i = 0; i < printer->numCopies(); i++)
    {
        for (int j = 0; j < pageCount; j++)
        {
            if (j != 0)
                printer->newPage();

            int x = (j % col ) * w;
            int y = (j / col) * h;

            p.resetTransform();

            //header & footer
            p.setClipRect(QRect(), Qt::NoClip);
            QRectF rectHeader = QRectF(0, -m_printHeader, w, m_printHeader);
            QRectF rectFooter = QRectF(0, h, w, m_printFooter);

            QTextOption option;
            option.setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
            p.drawText(rectHeader, replacePrintString(m_showPrintLeftHeader, pageCount, j+1), option);
            p.drawText(rectFooter, replacePrintString(m_showPrintLeftFooter, pageCount, j+1), option);
            option.setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            p.drawText(rectHeader, replacePrintString(m_showPrintMiddleHeader, pageCount, j+1), option);
            p.drawText(rectFooter, replacePrintString(m_showPrintMiddleFooter, pageCount, j+1), option);
            option.setAlignment(Qt::AlignRight | Qt::AlignVCenter);
            p.drawText(rectHeader, replacePrintString(m_showPrintRightHeader, pageCount, j+1), option);
            p.drawText(rectFooter, replacePrintString(m_showPrintRightFooter, pageCount, j+1), option);

            //page
            p.translate(- x - rect.x(), - y - rect.y());
            QRect r = QRect(x + rect.x(), y + rect.y() , w, h);
            p.setClipRect(rect.intersected(r));

            drawToolBar(&p);
            drawDescribeString(&p,m_drawData->m_stringText,m_drawData->m_stringFont,
                               m_drawData->m_stringPosition,m_drawData->m_stringArrange);
            drawAxis(&p);
            drawType(&p);
            drawClicent(&p);
            drawItem(&p);
        }
    }
}

QString CDrawWidget::replacePrintString(const QString &source, int count, int page)
{
    const QString sDate = "&Date";
    const QString sTime = "&Time";
    const QString sPage = "&Page";
    const QString sPageCount = "&Count";

    QDateTime current = QDateTime::currentDateTime();
    QString d = current.toString(m_showPrintDateFormat);
    QString t = current.toString(m_showPrintTimeFormat);
    QString p = QString::number(page);
    QString c = QString::number(count);

    QString r = "@";
    QString src = source;
    src.remove(r);

    src.replace(sDate, r, Qt::CaseSensitive);
    src.replace(r, d);
    src.replace(sTime, r, Qt::CaseSensitive);
    src.replace(r, t);
    src.replace(sPage, r, Qt::CaseSensitive);
    src.replace(r, p);
    src.replace(sPageCount, r, Qt::CaseSensitive);
    src.replace(r, c);

    return src;
}

void CDrawWidget::changeLanguage()
{
    CMDMultiLanguage* language = CMDMultiLanguage::instance();
    m_showPrintLeftHeader = language->value(m_printLeftHeader, false);
    m_showPrintMiddleHeader = language->value(m_printMiddleHeader, false);
    m_showPrintRightHeader = language->value(m_printRightHeader, false);
    m_showPrintLeftFooter = language->value(m_printLeftFooter, false);
    m_showPrintMiddleFooter = language->value(m_printMiddleFooter, false);
    m_showPrintRightFooter = language->value(m_printRightFooter, false);
    m_showPrintDateFormat = language->value(m_printDateFormat, false);
    m_showPrintTimeFormat = language->value(m_printTimeFormat, false);
}

void CDrawWidget::collectWords()
{
    CMDMultiLanguage* language = CMDMultiLanguage::instance();
    language->addWord(m_printLeftHeader, false);
    language->addWord(m_printMiddleHeader, false);
    language->addWord(m_printRightHeader, false);
    language->addWord(m_printLeftFooter, false);
    language->addWord(m_printMiddleFooter, false);
    language->addWord(m_printRightFooter, false);
    language->addWord(m_printDateFormat, false);
    language->addWord(m_printTimeFormat, false);
}
