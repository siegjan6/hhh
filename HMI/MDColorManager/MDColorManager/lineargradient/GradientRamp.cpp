#include "../lineargradient/GradientRamp.h"

GradientRamp::GradientRamp()
{
    m_select = false;
    this->setWidth(LINEARCHILD_WIDTH);
    this->setHeight(LINEARCHILD_HEIGHT);
}

GradientRamp::GradientRamp(const QColor &clr, const qreal &scale)
{
    this->setWidth(LINEARCHILD_WIDTH);
    this->setHeight(LINEARCHILD_HEIGHT);
    m_gradientStop.first = scale;
    m_gradientStop.second = clr;
}

GradientRamp::GradientRamp(const QGradientStop &colorfloat)
{
    this->setWidth(LINEARCHILD_WIDTH);
    this->setHeight(LINEARCHILD_HEIGHT);
    m_gradientStop = colorfloat;
}

GradientRamp::~GradientRamp()
{
}

void GradientRamp::Draw(QPainter* p, const QRectF &rt)
{
    qreal centerX = rt.x()+m_gradientStop.first*rt.width();//笔的中心点
    qreal topY = rt.bottom() - 1;

    qreal frameLeftX = centerX- width()/2;
    qreal frameRightX = centerX + width()/2;
    qreal frameTopY =  topY+ height()/3;
    qreal frameBottomY = topY+ height();

    qreal innerLeftX = frameLeftX+3;
    qreal innerRightX = frameRightX - 3;
    qreal innerTopY = frameTopY + 3;
    qreal innerBottomY = frameBottomY - 3;

    QPainterPath path1;
    QVector<QLineF> frameLines;
    frameLines.append(QLineF(centerX,topY,frameRightX,frameTopY));
    frameLines.append(QLineF(frameRightX,frameTopY,frameRightX,frameBottomY));
    frameLines.append(QLineF(frameRightX,frameBottomY,frameLeftX,frameBottomY));
    frameLines.append(QLineF(frameLeftX,frameBottomY,frameLeftX,frameTopY));
    frameLines.append(QLineF(frameLeftX,frameTopY,centerX,topY));
    path1.moveTo(centerX,topY);
    path1.lineTo(frameRightX,frameTopY);
    path1.lineTo(frameRightX,frameBottomY);
    path1.lineTo(frameLeftX,frameBottomY);
    path1.lineTo( frameLeftX,frameTopY);
    path1.lineTo(centerX,topY);
    //p->setBrush();
   p->fillPath(path1,QBrush(color()));
   if(selected())
   {
       QPen pen(Qt::black,2);
       p->setPen(pen);
       p->drawPath(path1);
   }
}

void GradientRamp::setSelected(bool bSelect)
{
    if(m_select != bSelect)
    {
        m_select = bSelect;
    }
}

bool GradientRamp::selected() const
{
    return m_select;
}

void GradientRamp::setColor(const QColor &clr)
{
    if(m_gradientStop.second != clr)
    {
        m_gradientStop.second = clr;
    }
}

QColor GradientRamp::color()
{
    return m_gradientStop.second;
}

void GradientRamp::setScale(qreal f)
{
    m_gradientStop.first = f;
}

qreal GradientRamp::scale() const
{
    return m_gradientStop.first;
}

