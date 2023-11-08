#include "CYAxis.h"
#include <math.h>
#include <QJsonObject>
#include <QDebug>
const int Interval = 4;
CYAxis::CYAxis():
    m_yWidth(50),
    m_isLeft(true),
    m_maxValue(100),
    m_minValue(0),
    m_maxPercent(100),
    m_minPercent(0),
    m_isNumeric(true),
    m_axisLineLength(0),
    m_originMax(100),
    m_originMin(0)
{
    setAxisLineVisible(true);
    setComment(("Y"));
}

CYAxis::~CYAxis()
{

}

double CYAxis::yWidth() const
{
    return m_yWidth;
}

void CYAxis::setYWidth(double yWidth)
{
    m_yWidth = yWidth;
}


bool CYAxis::isLeft() const
{
    return m_isLeft;
}

void CYAxis::setIsLeft(bool isLeft)
{
    m_isLeft = isLeft;
}
/*!
\section
*/
bool CYAxis::isNumeric() const
{
    return m_isNumeric;
}

void CYAxis::setIsNumeric(bool isNumeric)
{
    m_isNumeric = isNumeric;
}
double CYAxis::maxValue() const
{
    if(!m_isNumeric)
    {
        return 100 *endScale();
    }
    else
    {
        return {m_minValue + (m_maxValue - m_minValue) * endScale()};
    }

}

void CYAxis::setMaxValue(double maxValue)
{
    m_maxValue = maxValue;
}
double CYAxis::minValue() const
{
    if(!m_isNumeric)
    {
        return 100 * startScale();
    }
    else
    {
        return {m_minValue + (m_maxValue - m_minValue) * startScale()};
    }
}

void CYAxis::setMinValue(double minValue)
{
    m_minValue = minValue;
}

void CYAxis::setMaxPercent(const double &maxPercent)
{
    m_maxPercent = maxPercent;
}

double CYAxis::maxPercent() const
{
    return m_maxPercent;
}

void CYAxis::setMinPercent(const double &minPercent)
{
    m_minPercent = minPercent;
}

double CYAxis::minPercent() const
{
    return m_minPercent;
}

double CYAxis::originMax() const
{
    return m_originMax;
}

void CYAxis::setOriginMax(double originMax)
{
    m_originMax = originMax;
}

double CYAxis::originMin() const
{
    return m_originMin;
}

void CYAxis::setOriginMin(double originMin)
{
   m_originMin = originMin;
}

/*!
\section
*/
QRectF CYAxis::drawArea() const
{
    return m_drawArea;
}

void CYAxis::setDrawArea(const QRectF &rectF)
{
    m_drawArea = rectF;
    m_drawArea.setWidth(m_yWidth);
}
void CYAxis::draw(QPainter *p)
{

    if(!axisVisible())
        return;

    p->save();

    // 以TopLeft点为(0,0)点
    p->translate(m_drawArea.topLeft());
    p->setPen(fontColor());
    p->setFont(font());
    int count = m_valueRectF.count();
    for(int i = 0;i< count;i++)
        p->drawText(m_valueRectF.at(i),m_valueTextList.at(i));

    // 绘制comment
    p->drawText(m_commentRectF,comment());

    // 绘制轴线
    if(axisLineVisible())
        drawAxisLine(p);

    p->restore();
}
void CYAxis::drawAxisLine(QPainter *p)
{
    p->save();
    QPen pen;
    pen.setColor(Qt::black);
    pen.setWidth(axisLine()->lineWidth());
    pen.setStyle(Qt::SolidLine);
    p->setPen(pen);

    double width = 0;
    if(m_isLeft)
        width = m_drawArea.width();


    QPen linePen;
    linePen.setColor(axisLine()->lineColor());
    linePen.setStyle(axisLine()->lineStyle());
    linePen.setWidth(axisLine()->lineWidth());

    QBrush brush;
    brush.setColor(axisLine()->lineColor());
    brush.setStyle(Qt::SolidPattern);

    p->setPen(linePen);
    p->setBrush(brush);

    p->drawLine(width,0,width,m_axisLineLength);

    // 三角形边长为8
    double trH = 8* 0.5 * sqrt(3);// 高度

    QPointF topPoint = QPointF(width,0);
    QPointF leftPoint = QPointF(width-4,trH);
    QPointF rightPoint = QPointF(width+4,trH);

    QPainterPath path;
    path.moveTo(topPoint);
    path.lineTo(leftPoint);
    path.lineTo(rightPoint);
    path.closeSubpath();

    path.setFillRule(Qt::WindingFill);
    p->drawPath(path);

    p->restore();
}

void CYAxis::calculate(double labelHeight,int divNum)
{
    if(!axisVisible())
        return;

    QFontMetrics metrics(font());
    double textHeight = metrics.ascent()+ metrics.descent();
    QRectF drawArea = m_drawArea;
    double width = drawArea.width();
    m_axisLineLength = drawArea.height();// 轴线总高度
    double height = drawArea.height() - labelHeight;// 绘制字符高度(轴线说明高度固定)
    double maxValue = m_maxValue;
    double minValue = m_minValue;

    if(!m_isNumeric){ // 百分比
        maxValue = 100 * endScale();
        minValue = 100 * startScale();
    }else{
        maxValue = m_minValue + (m_maxValue - m_minValue) * endScale();
        minValue = m_minValue + (m_maxValue - m_minValue) * startScale();
    }

    m_valueRectF.clear();
    m_valueTextList.clear();

    // 计算绘制的字符位置
    double firstWid = metrics.width(QString::number(minValue,'f',valuePrecision()));
    double firstTextX = 0;

    //第一个字符的位置x
    if(m_isLeft)
        firstTextX = width -(firstWid + axisLine()->lineWidth());
    else
        firstTextX = Interval;
    Q_ASSERT(divNum > 0);
    double deltaValue = (maxValue - minValue) / divNum;
    double deltaHeight = height/ divNum;
    QString first =QString::number(minValue,'f',valuePrecision());
    m_valueTextList.append(first);
    QRectF firstRectF = QRectF(firstTextX,m_axisLineLength-textHeight,firstWid,textHeight);
    m_valueRectF.append(firstRectF);

    int count  = divNum +1;
    for(int i = 1;i< count;i++)
    {
       QString valueString= QString::number(minValue+deltaValue*i,'f',valuePrecision());
       m_valueTextList.append(valueString);

       double stringWidth = metrics.width(valueString);
       QRectF textRect = QRectF((m_isLeft)*(width - Interval - stringWidth)+(!m_isLeft)*(Interval),
                                m_axisLineLength -(deltaHeight*i + textHeight /2),
                                stringWidth,textHeight);
       m_valueRectF.append(textRect);
    }

    // 计算comment 区域
    double    commentWid = metrics.width(comment());
    if(m_isLeft)
        m_commentRectF.setTopLeft(QPointF(width-commentWid-Interval,0));
    else
        m_commentRectF.setTopLeft(QPointF(Interval, 0));
    m_commentRectF.setSize(QSizeF(commentWid,textHeight));
}


/*!
\section
*/
void CYAxis::serialize(QJsonObject &json)
{
    CAbstractAxis::serialize(json);
    QJsonObject jsonObject;
    jsonObject.insert("Ywidth",m_yWidth);
    jsonObject.insert("IsLeft",m_isLeft);
    jsonObject.insert("MaxValue",m_maxValue);
    jsonObject.insert("MinValue",m_minValue);
    jsonObject.insert("IsNumeric",m_isNumeric);
    jsonObject.insert("AxisLineLength",m_axisLineLength);
    jsonObject.insert("MaxPercent",m_maxPercent);
    jsonObject.insert("MinPercent",m_minPercent);
    jsonObject.insert("OriginMax",m_originMax);
    jsonObject.insert("OriginMin",m_originMin);
    json.insert("YAxis",jsonObject);
}

void CYAxis::deserialize(const QJsonObject &json)
{
    CAbstractAxis::deserialize(json);
    QJsonObject jsonObject = json.value("YAxis").toObject();
    m_yWidth = jsonObject.value("Ywidth").toDouble();
    m_isLeft = jsonObject.value("IsLeft").toBool();
    m_maxValue = jsonObject.value("MaxValue").toDouble();
    m_minValue = jsonObject.value("MinValue").toDouble();
    m_isNumeric = jsonObject.value("IsNumeric").toBool();
    m_axisLineLength = jsonObject.value("AxisLineLength").toDouble();
    m_maxPercent = jsonObject.value("MaxPercent").toDouble();
    m_minPercent = jsonObject.value("MinPercent").toDouble();
    m_originMax = jsonObject.value("OriginMax").toDouble();
    m_originMin = jsonObject.value("OriginMin").toDouble();

}

/*!
\section
*/
CYAxis &CYAxis::operator =(const CYAxis &other)
{
    if(&other == this)
        return *this;
    this->m_drawArea = other.m_drawArea;
    this->m_yWidth = other.m_yWidth;
    this->m_isLeft = other.m_isLeft;
    this->m_maxValue = other.m_maxValue;
    this->m_minValue = other.m_minValue;
    this->m_isNumeric = other.m_isNumeric;
    this->m_axisLineLength = other.m_axisLineLength;

    this->m_maxPercent = other.m_maxPercent;
    this->m_minPercent = other.m_minPercent;
    this->m_originMax = other.m_originMax;
    this->m_originMin = other.m_originMin;

    CAbstractAxis::operator =(other);
    return *this;
}




