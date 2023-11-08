#include <QRectF>
#include <QPainter>
#include "DrawAxis.h"

CDrawAxis::CDrawAxis()
{
    m_axisData = NULL;
}

CDrawAxis::~CDrawAxis()
{
}

void CDrawAxis::setAxisData(CDrawData *data)
{
    m_axisData = data;
}
void CDrawAxis::drawAxis(QPainter* painter,QRectF rectf)
{
    m_drawRect = getDiagramRect(rectf);
    m_interRect = QRectF(m_drawRect.x()+30,m_drawRect.y(),m_drawRect.width()-30,m_drawRect.height()-30);

    if(m_axisData->m_diagramStyle ==3||m_axisData->m_diagramStyle ==4)
    {
        QPainterPath path1;
        path1.moveTo(m_drawRect.x()+30,m_drawRect.y());
        path1.lineTo(m_interRect.x(),m_interRect.y()+m_interRect.height());
        path1.lineTo(m_interRect.x()+m_interRect.width(),m_interRect.y()+m_interRect.height());
        path1.lineTo(m_drawRect.x()+m_drawRect.width()-30,m_drawRect.y()+m_drawRect.height());
        path1.lineTo(m_drawRect.x(),m_drawRect.y()+m_drawRect.height());
        path1.lineTo(m_drawRect.x(),m_drawRect.y()+30);
        path1.lineTo(m_drawRect.x()+30,m_drawRect.y());

        QPainterPath path2;
        path2.moveTo(m_drawRect.x()+30,m_drawRect.y());
        path2.lineTo(m_drawRect.x()+m_drawRect.width(),m_drawRect.y());
        path2.lineTo(m_drawRect.x()+m_drawRect.width(),m_drawRect.y()+m_drawRect.height()-30);
        path2.lineTo(m_drawRect.x()+m_drawRect.width()-30,m_drawRect.y()+m_drawRect.height());
        path2.lineTo(m_drawRect.x(),m_drawRect.y()+m_drawRect.height());
        path2.lineTo(m_drawRect.x(),m_drawRect.y()+30);
        path2.lineTo(m_drawRect.x()+30,m_drawRect.y());
        painter->setPen(m_axisData->m_lineColor);
        painter->drawPath(path1);
//        painter->drawPath(path2);
        painter->setBrush(m_axisData->m_interColor);
        painter->drawPath(path2);
        if(m_axisData->m_interColor.red() == 255&&m_axisData->m_interColor.green() == 255&&m_axisData->m_interColor.blue() == 255)
        {
//            painter->setBrush(QColor(150,150,150,50));
            painter->fillPath(path1,QColor(150,150,150,80));
        }
    }
    drawYAxis(painter,rectf);
    if(m_axisData->m_diagramStyle ==1||m_axisData->m_diagramStyle ==2)
    {
        drawScale(painter,rectf);
    }
    else
    {
        QRectF rect = QRectF(rectf.x(),rectf.y()+30,rectf.width(),rectf.height()-30);
        drawScale(painter,rect);
    }
    drawXAxis(painter,rectf);
}

void CDrawAxis::setXString(QList<QString> stringList)
{
    m_stringList = stringList;
}

QList<QRectF> CDrawAxis::getRectList()
{
    return m_rectList;
}

void CDrawAxis::drawYAxis(QPainter* painter,QRectF rectf)
{
    QRectF rect = getDiagramRect(rectf);
    if(m_axisData->m_subScaleNum < 1 )
    {
        painter->setPen(m_axisData->m_lineColor);
         if(m_axisData->m_diagramStyle == 1||m_axisData->m_diagramStyle ==2)
         {
             painter->drawLine(rect.x(),rect.y(),rect.x()+rect.width(),rect.y());
             painter->drawLine(rect.x(),rect.y()+rect.height(),rect.x()+rect.width(),rect.y()+rect.height());
         }
         else
         {
             painter->drawLine(rect.x()+30,rect.y(),rect.x()+rect.width()-30,rect.y());
             painter->drawLine(rect.x(),rect.y()+rect.height(),rect.x()+rect.width()-30,rect.y()+rect.height());
             painter->drawLine(rect.x()+30,rect.y()+rect.height()-30,rect.x()+rect.width(),rect.y()+rect.height()-30);
         }

    }
    //！二维直方图Y轴
    if(m_axisData->m_diagramStyle == 1||m_axisData->m_diagramStyle ==2)
    {
        painter->setBrush(m_axisData->m_interColor);
        painter->setPen(Qt::NoPen);
        painter->drawRect(rect);
        QPen pen;
        pen.setColor(m_axisData->m_lineColor);
        painter->setPen(pen);
        painter->drawLine( rect.x(), rect.y(), rect.x(), rect.y()+rect.height());
        painter->drawLine(rect.x()+rect.width(),rect.y(),rect.x()+rect.width(),rect.y()+rect.height());
        //！画主刻度线
        for(qint32 i = 0; i<= m_axisData->m_mainScaleNum; i++)
        {

            painter->drawLine(rect.x()-5, rect.y()+rect.height()/m_axisData->m_mainScaleNum*i,
                              rect.x(), rect.y()+rect.height()/m_axisData->m_mainScaleNum*i);
        }
        //！画子刻度线
        for(qint32 i = 0;i <=m_axisData->m_mainScaleNum*m_axisData->m_subScaleNum;i++)
        {

            painter->drawLine(rect.x(), rect.y()+(rect.height())/(m_axisData->m_mainScaleNum*m_axisData->m_subScaleNum)*i,
                              rect.x()+rect.width( ), rect.y()+(rect.height())/(m_axisData->m_mainScaleNum*m_axisData->m_subScaleNum)*i);
        }

        //！画零刻度线
        if(m_axisData->m_minValue <0)
        {
            qreal zeroLine = rect.height()/(m_axisData->m_maxValue-m_axisData->m_minValue)*abs(m_axisData->m_minValue);
            if(m_axisData->m_diagramStyle ==2)
            {
                painter->drawLine(rect.x(), rect.y()+rect.height()-zeroLine,
                                  rect.x()+rect.width(),rect.y()+rect.height()-zeroLine);
            }
        }
    }
    //！三维直方图Y轴
    if(m_axisData->m_diagramStyle ==3||m_axisData->m_diagramStyle==4)
    {
        painter->setPen(m_axisData->m_lineColor);
        //！画刻度线
        for(qint32 i = 0; i<= m_axisData->m_mainScaleNum; i++)
        {
            //！绘制刻度短线
            painter->drawLine(m_drawRect.x()-5, m_drawRect.y()+30+(m_drawRect.height()-30)/m_axisData->m_mainScaleNum*i,
                              m_drawRect.x(), m_drawRect.y()+30+(m_drawRect.height()-30)/m_axisData->m_mainScaleNum*i);

        }

        //！画子刻度线
        for(qint32 i = 0;i <= m_axisData->m_mainScaleNum*m_axisData->m_subScaleNum;i++)
        {

                painter->drawLine(m_drawRect.x(), m_drawRect.y()+30+(m_drawRect.height()-30)/(m_axisData->m_mainScaleNum*m_axisData->m_subScaleNum)*i,
                                  m_interRect.x(), m_interRect.y()+m_interRect.height()/(m_axisData->m_mainScaleNum*m_axisData->m_subScaleNum)*i);
                painter->drawLine(m_interRect.x(), m_interRect.y()+m_interRect.height()/(m_axisData->m_mainScaleNum*m_axisData->m_subScaleNum)*i,
                                  m_interRect.x()+m_interRect.width(),
                                  m_interRect.y()+(m_interRect.height())/(m_axisData->m_mainScaleNum*m_axisData->m_subScaleNum)*i);
        }
        if(m_axisData->m_diagramStyle ==4)
        {
            //！画零刻度线
            qreal zeroLine1 = (m_drawRect.height()-30)/(m_axisData->m_maxValue - m_axisData->m_minValue)*abs(m_axisData->m_minValue);
            qreal zeroLine2 = m_interRect.height()/(m_axisData->m_maxValue-m_axisData->m_minValue)*abs(m_axisData->m_minValue);
            painter->setPen(m_axisData->m_lineColor);
            painter->drawLine(m_drawRect.x(), m_drawRect.y()+m_drawRect.height()-zeroLine1,
                               m_interRect.x(), m_interRect.y()+m_interRect.height()-zeroLine2);
            painter->drawLine(m_interRect.x(), m_interRect.y()+m_interRect.height()-zeroLine2,
                              m_interRect.x()+m_interRect.width(),m_interRect.y()+m_interRect.height()-zeroLine2);
        }
    }
}

void CDrawAxis::drawScale(QPainter *painter,QRectF rect)
{
    QFontMetricsF fm(m_axisData->m_diagramFont);
    qreal scaleHight = fm.height();
    painter->setFont(m_axisData->m_diagramFont);
    painter->setPen(QColor(0,0,0,255));
    //！二维直方图刻度值

    const qreal valueInterval = (m_axisData->m_maxValue - m_axisData->m_minValue)/m_axisData->m_mainScaleNum;
    const qreal rectInterval = getDiagramRect(rect).height() / m_axisData->m_mainScaleNum;
    for (qint32 i = 0; i <= m_axisData->m_mainScaleNum; i++)
    {
        QString str = QString::number((m_axisData->m_minValue + i*valueInterval) ,'d',m_axisData->m_decimals);
        qreal scaleWidth = fm.width(str);
        QRectF textRect = QRectF(getDiagramRect(rect).x()-7-scaleWidth,
                                 getDiagramRect(rect).y()+getDiagramRect(rect).height() -(rectInterval*i + scaleHight /2),
                                 scaleWidth,scaleHight);
        painter->drawText(textRect,str);

    }
}

QRectF CDrawAxis::getDiagramRect(QRectF rect)
{
    QRectF r = rect;
    QFontMetrics fm(m_axisData->m_diagramFont);
    QString str1 = QString::number(m_axisData->m_maxValue ,'d',m_axisData->m_decimals);
    QString str2 = QString::number(m_axisData->m_minValue,'d',m_axisData->m_decimals);
    qreal scaleWidth1 = fm.width(str1);
    qreal scaleWidth2 = fm.width(str2);
    qreal scaleWidth = (scaleWidth1>scaleWidth2 ? scaleWidth1:scaleWidth2)+10;
    qreal scaleHight = fm.height()+10;

    qreal x,y,w,h;
    x = r.x() + scaleWidth;
    y = r.y()+8;
    w = r.width() - scaleWidth-10;
    h = r.height() - scaleHight;

    r = QRectF(x,y,w,h);
    return r;
}

qreal CDrawAxis::scaleCoord(qreal data,  QRectF rect)
{
    qreal valuePerPixel = (qreal)((m_axisData->m_maxValue-m_axisData->m_minValue) / (rect.height() - 3));
    qreal finalCoord = (qreal)((data-m_axisData->m_minValue) / valuePerPixel);
    finalCoord = (rect.y()+rect.height() - 3) - finalCoord;
    return finalCoord;
}

void CDrawAxis::drawXAxis(QPainter *painter, QRectF rectf)
{
   QRectF rect = getDiagramRect(rectf);
   qreal averageLength = 0;
   qint32 count = m_stringList.count();
   QList<QPointF>m_pointList;
   painter->setPen(m_axisData->m_lineColor);
   if(m_axisData->m_category == 0)
   {
       if(m_axisData->m_diagramStyle == 1||m_axisData->m_diagramStyle ==2)
       {
           averageLength = rect.width()/(count+1);
           for(qint32 i = 1;i<(count+1);i++)
           {
               painter->drawLine(rect.x()+averageLength*i,rect.y(),
                                 rect.x()+averageLength*i,rect.y()+rect.height()-2);//！绘制竖直刻度线
               m_pointList.append(QPointF(rect.x()+averageLength*i,rect.y()+rect.height()));
           }
           //！获取每一个小的矩形区域
           for(qint32 i =0; i<= count; i++)
           {
               QRectF rectf = QRectF(rect.x()+averageLength*i,rect.y(),averageLength,rect.height());
               m_rectList.append(rectf);

           }

       }
       if(m_axisData->m_diagramStyle == 3||m_axisData->m_diagramStyle ==4)
       {
           m_pointList.clear();
           averageLength = m_interRect.width()/(count+1);
           for(qint32 i = 0;i<(count+1);i++)
           {
               painter->drawLine(m_interRect.x()+averageLength*i,m_interRect.y(),
                                 m_interRect.x()+averageLength*i,m_interRect.y()+m_interRect.height());//！绘制竖直刻度线
           }
           for(qint32 i = 1;i<(count+1);i++)
           {
               averageLength = (m_drawRect.width()-30)/(count+1);
               m_pointList.append(QPointF(rect.x()+averageLength*i,rect.y()+rect.height()));
           }
           //！获取每一个小的矩形区域
           for(qint32 i =0; i<= count; i++)
           {
               qreal inter = m_interRect.width()/(count+1);
               QRectF interRect = QRectF(m_interRect.x()+inter*i,m_interRect.y(),inter,m_interRect.height());
               m_interRectList.append(interRect);
               qreal draw = (m_drawRect.width()-30)/(count+1);
               QRectF drawRect = QRectF(m_drawRect.x()+draw*i,m_drawRect.y(),draw,m_drawRect.height());
               m_drawRectList.append(drawRect);
           }

           for(qint32 i = 0; i <=m_stringList.count();i++)
           {
               //画刻度竖斜线
               painter->drawLine(m_interRect.x()+m_interRect.width()/(m_stringList.count()+1)*i,
                                 m_interRect.y()+m_interRect.height(),

                                 m_drawRect.x()+(m_drawRect.width()-30)/(m_stringList.count()+1)*i,
                                 m_drawRect.y()+m_drawRect.height());
           }
       }
       //！绘制X轴刻度文本
       painter->setPen(QColor(0,0,0,255));
       for(qint32 i = 0;i<count;i++)
       {

           QFontMetrics fm(m_axisData->m_diagramFont);
           qreal stringWidth = fm.width(m_stringList.at(i));
           qreal stringHight = fm.height();
           painter->drawText(m_pointList.at(i).x()-stringWidth/2,
                             m_pointList.at(i).y()+stringHight,m_stringList.at(i));
       }

   }
   else
   {
       if(m_axisData->m_diagramStyle == 1||m_axisData->m_diagramStyle == 2)
       {
           averageLength = rect.width()/count;
           for(qint32 i = 1;i<count;i++)
           {
               painter->drawLine(rect.x()+averageLength*i,rect.y(),
                                 rect.x()+averageLength*i,rect.y()+rect.height());

           }
           //！绘制X轴刻度文本
           painter->setPen(QColor(0,0,0,255));
           for(qint32 i =0;i<count;i++)
           {
               QFontMetrics fm(m_axisData->m_diagramFont);
               qreal stringWidth = fm.width(m_stringList.at(i));
               qreal stringHight = fm.height();
               painter->drawText(rect.x()+averageLength*i+averageLength/2-stringWidth/2,
                                 rect.y()+rect.height()+stringHight, m_stringList.at(i));

               QRectF rectf = QRectF(rect.x()+averageLength*i,rect.y(),averageLength,rect.height());
               m_rectList.append(rectf);
           }
       }
       if(m_axisData->m_diagramStyle == 3||m_axisData->m_diagramStyle == 4)
       {
           averageLength = m_interRect.width()/count;
           for(qint32 i = 0;i<count;i++)
           {
               painter->drawLine(m_interRect.x()+averageLength*i,m_interRect.y(),
                                 m_interRect.x()+averageLength*i,m_interRect.y()+m_interRect.height());

           }
           for(qint32 i =0;i<count;i++)
           {
               //！绘制X轴刻度文本
               painter->setPen(QColor(0,0,0,255));
               averageLength = (m_drawRect.width()-30)/count;
               QFontMetrics fm(m_axisData->m_diagramFont);
               qreal stringWidth = fm.width(m_stringList.at(i));
               qreal stringHight = fm.height();
               painter->drawText(m_drawRect.x()+averageLength*i+averageLength/2-stringWidth/2,
                                 m_drawRect.y()+m_drawRect.height()+stringHight, m_stringList.at(i));

               QRectF rect = QRectF(m_drawRect.x()+averageLength*i,m_drawRect.y(),averageLength+30,m_drawRect.height());
               m_drawRectList.append(rect);
               qreal inter = m_interRect.width()/count;
               QRectF rectf = QRectF(m_interRect.x()+inter*i,m_interRect.y(),inter,m_interRect.height());
               m_interRectList.append(rectf);
           }
           for(qint32 i = 0; i <=m_stringList.count();i++)
           {
               //画刻度竖斜线
               painter->setPen(m_axisData->m_lineColor);
               painter->drawLine(m_interRect.x()+m_interRect.width()/m_stringList.count()*i,
                                 m_interRect.y()+m_interRect.height(),

                                 m_drawRect.x()+(m_drawRect.width()-30)/m_stringList.count()*i,
                                 m_drawRect.y()+m_drawRect.height());
           }
       }
   }
}

