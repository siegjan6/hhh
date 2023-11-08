#include "DrawHistogram3D.h"

CDrawHistogram3D::CDrawHistogram3D()
{
    m_histogramData3D = NULL;
}

CDrawHistogram3D::~CDrawHistogram3D()
{ 
}

void CDrawHistogram3D::drawHistogram3D(QPainter *painter)
{
    painter->setClipRect(m_drawRect);
    switch(m_histogramData3D->m_category)
    {
    case 0:
    {
        QList<qreal>valueList;
        for(qint32 i = 0;i<m_stringList1.count();i++)
        {
            valueList.append(m_valueList.at(i));
        }
        drawHistogram3D1(painter,valueList,m_drawRect,m_interRect);
        valueList.clear();
    }
        break;
    case 1:
    {
        drawHistogram3D2(painter);
    }
        break;
    case 2:
    {
        if(m_histogramData3D->m_diagramStyle == 3)
        {
            int countNegative = 0;
            for(int i = 0;i<m_valueList.count();i++)
            {
                if(m_valueList.at(i)<0)
                    countNegative++;
            }
            if(countNegative != 0)
                return;
           drawHistogram3D3(painter);
        }
        else
            drawHistogram3D3Negative(painter);

    }
        break;
    default:
        break;
    }

}

void CDrawHistogram3D::setHistogramData3D(CDrawData *data)
{
    m_histogramData3D = data;
}

void CDrawHistogram3D::setDrawRect(QRectF rect)
{
    m_drawRect = rect;
}

void CDrawHistogram3D::setInterRect(QRectF rect)
{
    m_interRect = rect;
}

void CDrawHistogram3D::setStringList1(QList<QString>stringList)
{
    m_stringList1 = stringList;
}

void CDrawHistogram3D::setStringList2(QList<QString> stringList)
{
    m_stringList2 = stringList;
}

void CDrawHistogram3D::setStringList3(QList<QString> stringList)
{
    m_stringList3 = stringList;
}

void CDrawHistogram3D::setColorList(QList<QColor> colorList)
{
    m_colorList = colorList;
}

void CDrawHistogram3D::setValueList(QList<qreal> valueList)
{
    m_valueList = valueList;
}

void CDrawHistogram3D::setDrawRectList(QList<QRectF> rectList)
{
    m_drawRectList = rectList;
}

void CDrawHistogram3D::setInterRectList(QList<QRectF> rectList)
{
    m_interRectList = rectList;
}

void CDrawHistogram3D::drawHistogram3D1(QPainter *painter,QList<qreal>valueList,QRectF drawRect,QRectF interRect)
{
    qint32 count;
    if(m_histogramData3D->m_category == 0)
    {
        if(m_colorList.count() < m_stringList1.count() ||valueList.count()<m_stringList1.count())
            return;
        count = m_stringList1.count();
    }
    if(m_histogramData3D->m_category == 1)
    {
        if(m_colorList.count() < m_stringList2.count()||valueList.count()<m_stringList2.count())
            return;
        count = m_stringList2.count();
    }

    qint32 pillarWidth1 = interRect.width()/(count+1)/3*2;
    qint32 pillarWidth2 = (drawRect.width()-30) /(count+1)/3*2;
    if(m_histogramData3D->m_minValue < 0 && m_histogramData3D->m_diagramStyle ==4)
    {
        //!绘制矩形
        for(qint32 i = 0; i < count; i++)
        {
            painter->setPen(QColor(0,0,0,100));
            painter->setBrush(m_colorList.at(i));
            if(valueList.at(i) >= 0)
            {
                //前后矩形
                qint32 x1 = interRect.x()+interRect.width() /(count+1)*(i+1) -pillarWidth1/2;
                qint32 rectHight1 = interRect.height()/(m_histogramData3D->m_maxValue - m_histogramData3D->m_minValue)*valueList.at(i);
                qint32 y1 = interRect.y()+interRect.height()/(m_histogramData3D->m_maxValue - m_histogramData3D->m_minValue)*m_histogramData3D->m_maxValue-rectHight1;

                qint32 x2 = drawRect.x()+ (drawRect.width()-30) /(count+1)*(i+1) -pillarWidth2/2;
                qint32 rectHight2 = (drawRect.height()-30)/(m_histogramData3D->m_maxValue - m_histogramData3D->m_minValue)*valueList.at(i);
                qint32 y2 = drawRect.y()+30+(drawRect.height()-30)/(m_histogramData3D->m_maxValue - m_histogramData3D->m_minValue)*m_histogramData3D->m_maxValue-rectHight2;
                m_pillarRectList.append(QRect(x2,y2,pillarWidth2,rectHight2));

                //绘制上部矩形
                qint32 x_drawRect = drawRect.x()+ (drawRect.width()-30) /(count+1)*(i+1) -pillarWidth2/2;
                qint32 upRectHight2 = (drawRect.height()-30)/(m_histogramData3D->m_maxValue - m_histogramData3D->m_minValue)*valueList.at(i);
                qint32 y_drawRect = drawRect.y()+30+(drawRect.height()-30)/(m_histogramData3D->m_maxValue - m_histogramData3D->m_minValue)*m_histogramData3D->m_maxValue-upRectHight2;
                QPainterPath path;
                path.moveTo(x_drawRect,y_drawRect);
                qint32 x_interRect = interRect.x()+interRect.width() /(count+1)*(i+1) -pillarWidth1/2;
                qint32 upRectHight1 = interRect.height()/(m_histogramData3D->m_maxValue - m_histogramData3D->m_minValue)*valueList.at(i);
                qint32 y_interRect = interRect.y()+interRect.height()/(m_histogramData3D->m_maxValue - m_histogramData3D->m_minValue)*m_histogramData3D->m_maxValue-upRectHight1;

                path.lineTo(x_interRect,y_interRect);
                path.lineTo(x_interRect+pillarWidth1,y_interRect);
                path.lineTo(x_drawRect+pillarWidth2,y_drawRect);
                path.lineTo(x_drawRect,y_drawRect);

                //绘制右侧矩形
                qint32 x_draw =  drawRect.x()+ (drawRect.width()-30) /(count+1)*(i+1) +pillarWidth2/2;
                qint32 x_inter = interRect.x()+interRect.width() /(count+1)*(i+1) +pillarWidth1/2;
                qint32 hight1 = interRect.height()/(m_histogramData3D->m_maxValue - m_histogramData3D->m_minValue)*valueList.at(i);
                QPainterPath path1;
                path1.moveTo(x_draw,y2);
                path1.lineTo(x_inter,y1);
                path1.lineTo(x_inter,y1+hight1);
                path1.lineTo(x_draw,y2+hight1);
                path1.lineTo(x_draw,y2);

                painter->drawRect(x1,y1,pillarWidth1,rectHight1);
                painter->drawRect(x2,y2,pillarWidth2,rectHight2);
                painter->drawPath(path);
                painter->drawPath(path1);
            }
            if(valueList.at(i) <0)
            {
                //前后矩形
                qint32 zeroLine = (interRect.height())/(m_histogramData3D->m_maxValue - m_histogramData3D->m_minValue)*m_histogramData3D->m_maxValue;
                qint32 x1 = interRect.x()+interRect.width() /(count+1)*(i+1) -pillarWidth1/2;
                qint32 rectHight1 = interRect.height()/(m_histogramData3D->m_maxValue - m_histogramData3D->m_minValue)*valueList.at(i);
                qint32 y1 = interRect.y()+zeroLine-rectHight1;

                qint32 x2 = drawRect.x()+ (drawRect.width()-30) /(count+1)*(i+1) -pillarWidth2/2;
                qint32 rectHight2 = (drawRect.height()-30)/(m_histogramData3D->m_maxValue - m_histogramData3D->m_minValue)*valueList.at(i);
                qint32 y2 = drawRect.y()+30+zeroLine-rectHight1;
                m_pillarRectList.append(QRect(x2,y2,pillarWidth2,rectHight2));

                //绘制上部矩形

                qint32 upRectHight2 = (drawRect.height()-30)/(m_histogramData3D->m_maxValue - m_histogramData3D->m_minValue)*valueList.at(i);
                QPainterPath path;
                path.moveTo(x2,y2+upRectHight2);
                qint32 upRectHight1 = interRect.height()/(m_histogramData3D->m_maxValue - m_histogramData3D->m_minValue)*valueList.at(i);
                path.lineTo(x1,y1+upRectHight1);
                path.lineTo(x1 +pillarWidth1,y1+upRectHight1);
                path.lineTo(x2+pillarWidth2,y2+upRectHight2);
                path.lineTo(x2,y2+upRectHight2);

                //绘制右侧矩形
                QPainterPath path1;
                path1.moveTo(x2+pillarWidth2,y2+upRectHight2);
                path1.lineTo(x1 +pillarWidth1,y1+upRectHight1);
                path1.lineTo(x1 +pillarWidth1,y1);
                path1.lineTo(x2+pillarWidth2,y2);
                path1.lineTo(x2+pillarWidth2,y2+upRectHight2);

                painter->drawRect(x1,y1,pillarWidth1,rectHight1);
                painter->drawRect(x2,y2,pillarWidth2,rectHight2);
                painter->drawPath(path);
                painter->drawPath(path1);
            }
        }
    }
    else
    {
        m_pillarRectList.clear();
        //！绘制矩形
        for(qint32 i = 0; i < count; i++)
        {
            painter->setBrush(m_colorList.at(i));
            qreal x_back = interRect.x()+interRect.width() /(count+1)*(i+1) -pillarWidth1/2;
            qreal backHight = interRect.height()/(m_histogramData3D->m_maxValue - m_histogramData3D->m_minValue)*(valueList.at(i)-m_histogramData3D->m_minValue);
            qreal y_back = interRect.y()+interRect.height()-backHight;
            painter->drawRect(x_back,y_back,pillarWidth1,backHight);

            qreal x_front = drawRect.x()+ (drawRect.width()-30) /(count+1)*(i+1) -pillarWidth2/2;
            qreal frontHight = (drawRect.height()-30)/(m_histogramData3D->m_maxValue - m_histogramData3D->m_minValue)*(valueList.at(i)-m_histogramData3D->m_minValue);
            qreal y_front = drawRect.y()+drawRect.height()-frontHight;
            painter->drawRect(x_front,y_front,pillarWidth2,frontHight);
            m_pillarRectList.append(QRectF(x_front,y_front,pillarWidth2,frontHight));

            //！绘制右侧矩形

            qreal x_drawRright =  drawRect.x()+ (drawRect.width()-30) /(count+1)*(i+1) +pillarWidth2/2;
            qreal y_drawRright = drawRect.y()+drawRect.height();
            QPainterPath path;
            path.moveTo(x_drawRright,y_drawRright);

            path.lineTo(x_drawRright,y_drawRright -m_pillarRectList.at(i).height() );
            qint32 x_interRight = interRect.x()+interRect.width() /(count+1)*(i+1) +pillarWidth1/2;
            qint32 rightHight = interRect.height()/(m_histogramData3D->m_maxValue - m_histogramData3D->m_minValue)*(valueList.at(i)-m_histogramData3D->m_minValue);
            qint32 y_interRight = interRect.y()+interRect.height()-rightHight;
            path.lineTo(x_interRight,y_interRight);
            path.lineTo(x_interRight,interRect.y()+interRect.height());
            path.lineTo(x_drawRright,y_drawRright);
            painter->drawPath(path);

            //！绘制上部矩形

            qint32 x_drawUp = drawRect.x()+ (drawRect.width()-30)/(count+1)*(i+1) -pillarWidth2/2;
            qint32 drawUpHight = (drawRect.height()-30)/(m_histogramData3D->m_maxValue - m_histogramData3D->m_minValue)*(valueList.at(i)-m_histogramData3D->m_minValue);
            qint32 y_drawUp = drawRect.y()+drawRect.height()-drawUpHight;
            QPainterPath pathUp;
            pathUp.moveTo(x_drawUp,y_drawUp);
            qint32 x_interUp = interRect.x()+interRect.width() /(count+1)*(i+1) -pillarWidth1/2;
            qint32 interUpHight = interRect.height()/(m_histogramData3D->m_maxValue - m_histogramData3D->m_minValue)*(valueList.at(i)-m_histogramData3D->m_minValue);
            qint32 y_interUp = interRect.y()+interRect.height()-interUpHight;
            pathUp.lineTo(x_interUp,y_interUp);
            pathUp.lineTo(x_interUp+pillarWidth1,y_interUp);
            pathUp.lineTo(x_drawUp+pillarWidth2,y_drawUp);
            pathUp.lineTo(x_drawUp,y_drawUp);
            painter->drawPath(pathUp);
        }

        //！再次绘制前部矩形（立体）
        for(qint32 i = 0; i < count; i++)
        {
            painter->setBrush(m_colorList.at(i));
            qint32 x_front1 = drawRect.x()+ (drawRect.width()-30) /(count+1)*(i+1) -pillarWidth2/2;
            qint32 hight = (drawRect.height()-30)/(m_histogramData3D->m_maxValue - m_histogramData3D->m_minValue)*(valueList.at(i)-m_histogramData3D->m_minValue);
            qint32 y_front1 = drawRect.y()+drawRect.height()-hight;
            painter->drawRect(x_front1,y_front1,pillarWidth2,hight);
        }
    }
}

void CDrawHistogram3D::drawHistogram3D2(QPainter *painter)
{
    QList<qreal> valueList;
    if(m_valueList.count() < m_stringList1.count()*m_stringList2.count())
        return;
    qint32 count = m_stringList2.count();//！每组数据的个数
    for(qint32 i = 0;i<m_stringList1.count();i++)
    {
        for(qint32 j =i*count;j<count*(i+1);j++)
        {
            valueList.append(m_valueList.at(j));
        }
        drawHistogram3D1(painter,valueList,m_drawRectList.at(i),m_interRectList.at(i));
        valueList.clear();
    }
}

void CDrawHistogram3D::drawHistogram3D3(QPainter *painter)
{
    qint32 count = (m_stringList3.count())*(m_drawRectList.count());//！总的直方图个数
    if(m_valueList.count() < m_stringList2.count()*count)
        return;
    QList<qreal>valueList;
    QList<qreal>valueSumList;
    qreal value = 0;
    //!柱子间空格的大小
    qreal blankWidth = (m_drawRectList.at(0).width()-30)/(m_stringList3.count()+1)/3;
    qreal pillarWidth = blankWidth*2;           //！柱子的宽
    qreal heightFact = (m_drawRectList.at(0).height()) / (m_histogramData3D->m_maxValue-m_histogramData3D->m_minValue);

    //！绘制柱子
    qint32 i = 0;
    while(i <count)
    {
        for(qint32 k =i*m_colorList.count();k<m_colorList.count()*(i+1);k++)
        {
            valueList.append(m_valueList.at(k));
        }
        qint32 num = valueList.count();
        for(qint32 j = 0;j<valueList.count();j++)
        {
            for(qint32 t = 0;t<num;t++)
            {
                value += valueList.at(t);
            }
            valueSumList.append(value);
            value = 0;
            if(num>0)
                num--;

        }
        //!绘制后部、前部矩形
        for(qint32 m = 0;m<valueSumList.count();m++)
        {
            qreal pillarHeight;
            pillarHeight = (valueSumList.at(m)-m_histogramData3D->m_minValue) * heightFact;
            qint32 t = i/m_stringList3.count();
            //！后部矩形
            qint32 backX = i * ( pillarWidth + blankWidth ) +m_interRectList.at(0).x()+pillarWidth+3*t*blankWidth;
            qint32 backY = m_interRectList.at(0).height() - pillarHeight + m_interRectList.at(0).y();
            QRectF rectBack;
            rectBack = QRectF( backX, backY, pillarWidth, pillarHeight);
            painter->setPen(Qt::NoPen);
            painter->setBrush(Qt::NoBrush);
            painter->drawRect(rectBack);
            //！前部矩形
            QPen pen;
            pen.setColor(QColor(200,200,200,255));
            painter->setPen(0.1);
            painter->setBrush(m_colorList.at(m_colorList.count()-1- m));
            qint32 frontX = i * ( pillarWidth + blankWidth ) +m_drawRectList.at(0).x()+pillarWidth+3*t*blankWidth;
            qint32 frontY = m_drawRectList.at(0).height() - pillarHeight + m_drawRectList.at(0).y();
            QRectF rectFront;
            rectFront = QRectF( frontX, frontY, pillarWidth, pillarHeight);
            painter->drawRect(rectFront);

//            QPainterPath pathUp;
//            pathUp.moveTo(rectFront.x(),rectFront.y());
//            pathUp.lineTo(rectBack.x(),rectBack.y());
//            pathUp.lineTo(rectBack.x()+rectBack.width(),rectBack.y());
//            pathUp.lineTo(rectFront.x()+rectFront.width(),rectFront.y());
//            pathUp.lineTo(rectFront.x(),rectFront.y());
//            painter->drawPath(pathUp);

            //!绘制最上部矩形（只绘制一次）
            if(m == 0)
            {
                QPainterPath pathUp;
                pathUp.moveTo(rectFront.x(),rectFront.y());
                pathUp.lineTo(rectBack.x(),rectBack.y());
                pathUp.lineTo(rectBack.x()+rectBack.width(),rectBack.y());
                pathUp.lineTo(rectFront.x()+rectFront.width(),rectFront.y());
                pathUp.lineTo(rectFront.x(),rectFront.y());
                painter->drawPath(pathUp);
            }
            //!绘制右侧矩形
            QPainterPath pathRight;
            pathRight.moveTo(rectFront.x()+rectFront.width(),rectFront.y());
            pathRight.lineTo(rectBack.x()+rectBack.width(),rectBack.y());
            pathRight.lineTo(rectBack.x()+rectBack.width(),rectBack.y()+rectBack.height());
            pathRight.lineTo(rectFront.x()+rectFront.width(),rectFront.y()+rectFront.height());
            pathRight.lineTo(rectFront.x()+rectFront.width(),rectFront.y());
            painter->drawPath(pathRight);

        }
        //!再次绘制前部矩形

//        for(int m = 0;m<valueSumList.count();m++)
//        {
//            qreal pillarHeight;
//            pillarHeight = (valueSumList.at(m)-m_histogramData3D->m_minValue) * heightFact;
//            int t = i/m_stringList3.count();

//            int frontX = i * ( pillarWidth + blankWidth ) +m_drawRectList.at(0).x()+pillarWidth+3*t*blankWidth;
//            int frontY = m_drawRectList.at(0).height() - pillarHeight + m_drawRectList.at(0).y();
//            QRectF rectFront;
//            if(m == valueSumList.count()-1)
//            {
//                rectFront = QRect( frontX, frontY, pillarWidth, pillarHeight+1);
//            }
//            else
//                rectFront = QRect( frontX, frontY, pillarWidth, pillarHeight);
//            painter->setBrush( QBrush( m_colorList.at(m_colorList.count()-1- m)));
//            painter->drawRect(rectFront);

//        }
        valueList.clear();
        valueSumList.clear();
        i++;
    }
}

void CDrawHistogram3D::drawHistogram3D3Negative(QPainter *painter)
{
    if(m_histogramData3D->m_minValue >= 0)
    {
        drawHistogram3D3(painter);
    }
    qint32 count = (m_stringList3.count())*(m_drawRectList.count());
    if(m_valueList.count() < m_colorList.count()*count)
        return;
    QList<qreal>valueList;
    QList<qreal>valueSumList;
    qreal value = 0;
    qreal blankWidth = (m_drawRectList.at(0).width()-30)/(m_stringList3.count()+1)/3;//柱子间空格的大小
    qreal pillarWidth = blankWidth*2;           //！柱子的宽
    qreal heightFact = (m_drawRectList.at(0).height()) / (m_histogramData3D->m_maxValue-m_histogramData3D->m_minValue);

    if(m_histogramData3D->m_minValue <0)
    {
        qreal zeroValue = heightFact*abs(m_histogramData3D->m_minValue);
        //绘制柱子
        qint32 i = 0;
        while(i <count)
        {
            for(qint32 j =i*m_colorList.count();j<m_colorList.count()*(i+1);j++)
            {
                valueList.append(m_valueList.at(j));
            }
            qint32 num = valueList.count();
            QList<qreal>positiveList;
            QList<qreal>negetiveList;
            for(qint32 k=0; k<num; k++)
            {
                if(valueList.at(k) >= 0)
                {
                    positiveList.append(valueList.at(k));
                    negetiveList.append(0);
                }
                else
                {
                    positiveList.append(0);
                    negetiveList.append(valueList.at(k));
                }
            }
            //绘制数据为正数直方图累积

            for(qint32 j = 0;j<positiveList.count();j++)
            {
                for(qint32 t = 0;t<num;t++)
                {
                    value += positiveList.at(t);
                }
                valueSumList.append(value);
                value = 0;
                if(num>0)
                    num--;
            }
            qint32 samePositiveNum = 0;
            for(qint32 same = 1;same<valueSumList.count();same++)
            {
                if(valueSumList.at(0) ==valueSumList.at(same))
                    samePositiveNum++;
            }
            for(qint32 m = 0;m<valueSumList.count();m++)
            {
                qreal pillarHeight;
                pillarHeight = valueSumList.at(m) * heightFact;
                qint32 t = i/m_stringList3.count();
                //!back rect
                qint32 backX = i * ( pillarWidth + blankWidth ) +m_interRectList.at(0).x()+pillarWidth+3*t*blankWidth;
                qint32 backY = m_interRectList.at(0).height() - pillarHeight + m_interRectList.at(0).y() - zeroValue;
                QRect rectBack = QRect( backX, backY, pillarWidth, pillarHeight);
                painter->setPen(Qt::NoPen);
                painter->setBrush(Qt::NoBrush);
                painter->drawRect(rectBack);

                //!front rect
                qint32 frontX = i * ( pillarWidth + blankWidth ) +m_drawRectList.at(0).x()+pillarWidth+3*t*blankWidth;
                qint32 frontY = m_drawRectList.at(0).height() - pillarHeight + m_drawRectList.at(0).y() - zeroValue;
                QRect rectFront = QRect( frontX, frontY, pillarWidth, pillarHeight);
                QPen pen;
                pen.setColor(QColor(200,200,200,255));
                painter->setPen(0.1);
                painter->setBrush( QBrush( m_colorList.at(m_colorList.count()-1- m)));
                painter->drawRect(rectFront);

                //!绘制最上部矩形（只绘制一次）

                if(m == 0)
                {
                    painter->setBrush( QBrush( m_colorList.at(m_colorList.count()-1- samePositiveNum)));
                    QPainterPath pathUp;
                    pathUp.moveTo(rectFront.x(),rectFront.y());
                    pathUp.lineTo(rectBack.x(),rectBack.y());
                    pathUp.lineTo(rectBack.x()+rectBack.width(),rectBack.y());
                    pathUp.lineTo(rectFront.x()+rectFront.width(),rectFront.y());
                    pathUp.lineTo(rectFront.x(),rectFront.y());
                    painter->drawPath(pathUp);

                }
                //!绘制右侧矩形
                QPainterPath pathRight;
                pathRight.moveTo(rectFront.x()+rectFront.width(),rectFront.y());
                pathRight.lineTo(rectBack.x()+rectBack.width(),rectBack.y());
                pathRight.lineTo(rectBack.x()+rectBack.width(),rectBack.y()+rectBack.height());
                pathRight.lineTo(rectFront.x()+rectFront.width(),rectFront.y()+rectFront.height());
                pathRight.lineTo(rectFront.x()+rectFront.width(),rectFront.y());
                painter->setBrush( QBrush( m_colorList.at(m_colorList.count()-1- m)));
                painter->drawPath(pathRight);
            }

            valueSumList.clear();
            //绘制数据为负数直方图累积
            qint32 negetiveNum = negetiveList.count();
            for(qint32 j = 0;j<negetiveList.count();j++)
            {
                for(qint32 t = 0;t<negetiveNum;t++)
                {
                    value += negetiveList.at(t);
                }

                valueSumList.append(abs(value));
                value = 0;
                if(negetiveNum>0)
                    negetiveNum--;
            }

            for(qint32 m = 0;m<valueSumList.count();m++)
            {
                qreal pillarHeight;
                pillarHeight = valueSumList.at(m) * heightFact;
                qint32 t = i/m_stringList3.count();
                qint32 backX = i * ( pillarWidth + blankWidth ) +m_interRectList.at(0).x()+pillarWidth+3*t*blankWidth;
                qint32 backY = m_interRectList.at(0).height() + m_interRectList.at(0).y() - zeroValue;
                QRect rectBack = QRect( backX, backY, pillarWidth, pillarHeight);
                painter->setPen(Qt::NoPen);
                painter->setBrush(Qt::NoBrush);
                painter->drawRect(rectBack);

                qint32 frontX = i * ( pillarWidth + blankWidth ) +m_drawRectList.at(0).x()+pillarWidth+3*t*blankWidth;
                qint32 frontY = m_drawRectList.at(0).height() + m_drawRectList.at(0).y() - zeroValue;
                QRect rectFront = QRect( frontX, frontY, pillarWidth, pillarHeight);
                QPen pen;
                pen.setColor(QColor(200,200,200,255));
                painter->setPen(0.1);
                painter->setBrush( QBrush( m_colorList.at(m_colorList.count()-1- m)));
                painter->drawRect(rectFront);

                //!绘制最上部矩形（只绘制一次）
//                if(m == 0)
//                {
//                    painter->setBrush( QBrush( m_colorList.at(m_colorList.count()-1- sameNegativeNum)));
//                    QPainterPath pathUp;
//                    pathUp.moveTo(rectFront.x(),rectFront.y());
//                    pathUp.lineTo(rectBack.x(),rectBack.y());
//                    pathUp.lineTo(rectBack.x()+rectBack.width(),rectBack.y());
//                    pathUp.lineTo(rectFront.x()+rectFront.width(),rectFront.y());
//                    pathUp.lineTo(rectFront.x(),rectFront.y());
//                    painter->drawPath(pathUp);
//                }
                //!绘制右侧矩形
                QPainterPath pathRight;
                pathRight.moveTo(rectFront.x()+rectFront.width(),rectFront.y());
                pathRight.lineTo(rectBack.x()+rectBack.width(),rectBack.y());
                pathRight.lineTo(rectBack.x()+rectBack.width(),rectBack.y()+rectBack.height());
                pathRight.lineTo(rectFront.x()+rectFront.width(),rectFront.y()+rectFront.height());
                pathRight.lineTo(rectFront.x()+rectFront.width(),rectFront.y());
                painter->setBrush( QBrush( m_colorList.at(m_colorList.count()-1- m)));
                painter->drawPath(pathRight);

            }
            valueList.clear();
            valueSumList.clear();
            i++;
        }
    }
}

