#include "DrawHistogram.h"
#include <QPainter>

CDrawHistogram::CDrawHistogram()
{
    m_histogramData = NULL;
}

CDrawHistogram::~CDrawHistogram()
{
}

void CDrawHistogram::setHistogramData(CDrawData *data)
{
    m_histogramData = data;
}

void CDrawHistogram::setHistogramRectList(QList<QRectF> rectList)
{
    m_rectList = rectList;
}

void CDrawHistogram::setHistogramValueList(QList<qreal> valueList)
{
    m_valueList = valueList;
}

void CDrawHistogram::setHistogramColorList(QList<QColor> colorList)
{
    m_colorList = colorList;
}

void CDrawHistogram::setHistogramStringList2(QList<QString> stringList)
{
    m_stringList2 = stringList;
}

void CDrawHistogram::setHistogramStringList3(QList<QString> stringList)
{
   m_stringList3  = stringList;
}

void CDrawHistogram::drawHistogram(QPainter *painter)
{
    if (m_valueList.count() == 0 || m_colorList.count() ==0 ||m_rectList.count() ==0
            ||m_stringList2.count() ==0||m_stringList3.count() ==0)
        return;
    painter->setClipRect(m_rectList.at(0).x(),m_rectList.at(0).y(),
                         m_rectList.at(0).width()*m_rectList.count(),m_rectList.at(0).height());
    switch(m_histogramData->m_category)
    {
    case 0:
    {
        drawHistogram1(painter);
    }

        break;
    case 1:
    {
        drawHistogram2(painter);
    }

        break;
    case 2:
    {
        if(m_histogramData->m_diagramStyle ==1)
        {
            int countNegative = 0;
            for(int i = 0;i<m_valueList.count();i++)
            {
                if(m_valueList.at(i)<0)
                    countNegative++;
            }
            if(countNegative != 0)
                return;
            drawHistogram3(painter);
        }
        if(m_histogramData->m_diagramStyle == 2)
        {
           drawHistogram3Negative(painter);
        }
    }

        break;
    default:
        break;
    }
}

void CDrawHistogram::drawHistogram1(QPainter *painter)
{
    if(m_colorList.count() < m_rectList.count()-1 ||m_valueList.count()<m_rectList.count()-1)
        return;
    //柱子间空格的大小
    qreal blankWidth = m_rectList.at(0).width()/3;
    qreal pillarWidth = blankWidth*2;           // 柱子的宽
    qreal heightFact = (m_rectList.at(0).height()) / (m_histogramData->m_maxValue-m_histogramData->m_minValue);

    //绘制柱子
    if(m_histogramData->m_diagramStyle == 2 && m_histogramData->m_minValue <0)
    {
        qreal zeroLine = heightFact*abs(m_histogramData->m_minValue);

        qreal leftUpX;
        qreal leftUpY;
        for (qint32 i = 0; i < (m_rectList.size()-1); ++i)
        {
            qreal pillarHeight;
            pillarHeight =heightFact * abs(m_valueList.at(i));
            QRectF pillarRect;
            if(m_valueList.at(i) >0)
            {
                leftUpX = i * ( pillarWidth + blankWidth ) +m_rectList.at(0).x()+pillarWidth;
                leftUpY = m_rectList.at(0).height() - pillarHeight -zeroLine+ m_rectList.at(0).y();
                pillarRect = QRectF(leftUpX, leftUpY, pillarWidth, pillarHeight);
            }
            if(m_valueList.at(i) <0)
            {
                leftUpX =i * ( pillarWidth + blankWidth ) +m_rectList.at(0).x()+pillarWidth;
                leftUpY = m_rectList.at(0).height() -zeroLine+ m_rectList.at(0).y();
                pillarRect = QRectF(leftUpX, leftUpY, pillarWidth, pillarHeight);
            }

            m_pillarRectList.append(pillarRect);

            painter->setPen(Qt::NoPen);
            painter->setBrush( QBrush( m_colorList.at(i)));
            painter->drawRect(pillarRect);

        }
    }
    else
    {
        for (qint32 i = 0; i < (m_rectList.size()-1); ++i)
        {
            qreal pillarHeight;
            if(m_valueList.at(i) <0)
            {
                 pillarHeight = (m_valueList.at(i)-m_histogramData->m_minValue) * heightFact ;
            }
            else
                 pillarHeight = (m_valueList.at(i)-m_histogramData->m_minValue) * heightFact;
            qreal leftUpX = i * ( pillarWidth + blankWidth ) +m_rectList.at(0).x()+pillarWidth;
            qreal leftUpY = m_rectList.at(0).height() - pillarHeight + m_rectList.at(0).y();
            QRectF rect = QRectF( leftUpX, leftUpY, pillarWidth, pillarHeight );

            m_pillarRectList.append(rect);

            painter->setPen(Qt::NoPen);
            painter->setBrush( QBrush( m_colorList.at(i)));
            painter->drawRect(rect);
        }
    }
}

void CDrawHistogram::drawHistogram2(QPainter *painter)
{
    QList<qreal> valueList;
    if(m_valueList.count() < m_rectList.count()*m_stringList2.count())
        return;
    if(m_colorList.count()<m_stringList2.count())
        return;
    qint32 count = m_stringList2.count();//每组数据的个数
    for(qint32 i = 0;i<m_rectList.count();i++)
    {
        for(qint32 j =i*count;j<count*(i+1);j++)
        {
            valueList.append(m_valueList.at(j));
        }
        drawPillar2(painter,valueList,m_colorList,m_rectList.at(i));
        valueList.clear();
    }
}

void CDrawHistogram::drawPillar2(QPainter* painter,QList<qreal> valueList, QList<QColor> colorList, QRectF rect)
{
    //柱子间空格的大小
    qreal blankWidth = rect.width()/(valueList.count()+1)/3;
    qreal pillarWidth = blankWidth*2;           // 柱子的宽
    qreal heightFact = (rect.height()) / (m_histogramData->m_maxValue-m_histogramData->m_minValue);

    //绘制柱子
    if(m_histogramData->m_diagramStyle == 2 && m_histogramData->m_minValue <0)
    {
        qreal zeroLine = heightFact*abs(m_histogramData->m_minValue);

        qreal leftUpX;
        qreal leftUpY;
        for (qint32 i = 0; i < valueList.size(); ++i)
        {
            qreal pillarHeight;
            pillarHeight =heightFact * abs(valueList.at(i));
            QRectF pillarRect;
            if(valueList.at(i) >0)
            {
                leftUpX = i * ( pillarWidth + blankWidth ) +rect.x()+pillarWidth;
                leftUpY = rect.height() - pillarHeight -zeroLine+ rect.y();
                pillarRect = QRectF(leftUpX, leftUpY, pillarWidth, pillarHeight);
            }
            if(valueList.at(i) <0)
            {
                leftUpX =i * ( pillarWidth + blankWidth ) +rect.x()+pillarWidth;
                leftUpY = rect.height() -zeroLine+ rect.y();
                pillarRect = QRectF(leftUpX, leftUpY, pillarWidth, pillarHeight);
            }

            painter->setPen(Qt::NoPen);
            painter->setBrush( QBrush( colorList.at(i)));

//            if(valueList.at(i) >= m_histogramData->m_minValue && m_valueList.at(i) <= m_histogramData->m_maxValue)
//            {
                painter->drawRect(pillarRect);
//            }
        }

    }
    else
    {
        for (qint32 i = 0; i < valueList.size(); ++i)
        {
            qreal pillarHeight;
            if(valueList.at(i) <0)
            {
                 pillarHeight = (valueList.at(i)-m_histogramData->m_minValue) * heightFact ;
            }
            else
                 pillarHeight = (valueList.at(i)-m_histogramData->m_minValue) * heightFact;
            qreal leftUpX = i * ( pillarWidth + blankWidth ) +rect.x()+pillarWidth;
            qreal leftUpY = rect.height() - pillarHeight + rect.y();
            QRectF rect = QRectF( leftUpX, leftUpY, pillarWidth, pillarHeight);
            painter->setPen(Qt::NoPen);
            painter->setBrush( QBrush( colorList.at(i)));

//            if(valueList.at(i) >= m_histogramData->m_minValue && m_valueList.at(i) <= m_histogramData->m_maxValue)
//            {
                 painter->drawRect(rect);

//            }
        }
    }
}

void CDrawHistogram::drawHistogram3(QPainter *painter)
{
    qint32 count = (m_stringList3.count())*(m_rectList.count());//总的直方图个数
    if(m_valueList.count() < m_stringList2.count()*count)
        return;
    QList<qreal>valueList;
    QList<qreal>valueSumList;
    qreal value = 0;
    //柱子间空格的大小
    qreal blankWidth = m_rectList.at(0).width()/(m_stringList3.count()+1)/3;
    qreal pillarWidth = blankWidth*2;           // 柱子的宽
    qreal heightFact = (m_rectList.at(0).height()) / (m_histogramData->m_maxValue-m_histogramData->m_minValue);

    //绘制柱子
    qint32 i = 0;
    while(i <count)
    {
        for(int k =i*m_colorList.count();k<m_colorList.count()*(i+1);k++)
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
        for(qint32 m = 0;m<valueSumList.count();m++)
        {
            qreal pillarHeight;
            pillarHeight = (valueSumList.at(m)-m_histogramData->m_minValue) * heightFact;

            qint32 t = i/m_stringList3.count();
            qreal leftUpX = i * ( pillarWidth + blankWidth ) +m_rectList.at(0).x()+pillarWidth+3*t*blankWidth;
            qreal leftUpY = m_rectList.at(0).height() - pillarHeight + m_rectList.at(0).y();
            QRectF rect;
            rect = QRectF( leftUpX, leftUpY, pillarWidth, pillarHeight);
            painter->setPen(Qt::NoPen);
            painter->setBrush( QBrush( m_colorList.at(m_colorList.count()-1- m)));
            painter->drawRect(rect);
        }

        valueList.clear();
        valueSumList.clear();
        i++;
    }

}

void CDrawHistogram::drawHistogram3Negative(QPainter *painter)
{
    if(m_histogramData->m_minValue >= 0)
    {
        drawHistogram3(painter);
    }
    qint32 count = (m_stringList3.count())*(m_rectList.count());
    if(m_valueList.count() < m_colorList.count()*count)
        return;
    QList<qreal>valueList;
    QList<qreal>valueSumList;
    qreal value = 0;
    qreal blankWidth = m_rectList.at(0).width()/(m_stringList3.count()+1)/3;//柱子间空格的大小
    qreal pillarWidth = blankWidth*2;           // 柱子的宽
    qreal heightFact = (m_rectList.at(0).height()) / (m_histogramData->m_maxValue-m_histogramData->m_minValue);

    if(m_histogramData->m_minValue <0)
    {
        qreal zeroValue = heightFact*abs(m_histogramData->m_minValue);
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
            for(qint32 m = 0;m<valueSumList.count();m++)
            {
                qreal pillarHeight;
                pillarHeight = valueSumList.at(m) * heightFact;
                qint32 t = i/m_stringList3.count();
                qreal leftUpX = i * ( pillarWidth + blankWidth ) +m_rectList.at(0).x()+pillarWidth+3*t*blankWidth;
                qreal leftUpY = m_rectList.at(0).height() - pillarHeight + m_rectList.at(0).y() - zeroValue;
                QRectF rect = QRectF( leftUpX, leftUpY, pillarWidth, pillarHeight);
                painter->setPen(Qt::NoPen);
                painter->setBrush( QBrush( m_colorList.at(m_colorList.count()-1- m)));
                painter->drawRect(rect);
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
                qreal leftUpX = i * ( pillarWidth + blankWidth ) +m_rectList.at(0).x()+pillarWidth+3*t*blankWidth;
                qreal leftUpY = m_rectList.at(0).height() + m_rectList.at(0).y() - zeroValue;

                QRectF rect = QRectF(leftUpX, leftUpY, pillarWidth, pillarHeight);
                painter->setPen(Qt::NoPen);
                painter->setBrush( QBrush( m_colorList.at(m_colorList.count()-1- m)));
                painter->drawRect(rect);
            }
            valueList.clear();
            valueSumList.clear();
            i++;
        }
    }
}






