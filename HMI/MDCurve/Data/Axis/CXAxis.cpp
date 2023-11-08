#include "CXAxis.h"
#include "MDJsonConvert.h"
#include <QDebug>
#include <math.h>
#include <QFile>
#include <QJsonObject>

//const int  Internal = 2;
const int  ArrowLength = 10;

CXAxis::CXAxis():
    m_valueType(ValueType::Time),
    m_totalLength(600),
    m_originalLength(600),
    m_divNum(3),
    m_xHeight(24),
    m_timeMark(QDateTime::currentDateTime()),
    m_originalEndTime(QDateTime::currentDateTime()),
    m_endTime(QDateTime::currentDateTime()),
    m_timeFormat("hh:mm:ss"),
    m_maxValue(100),
    m_minValue(0),
    m_originMax(100),
    m_originMin(0)
{
    setAxisLineVisible(true);
    setComment("X");
}

CXAxis::~CXAxis()
{

}

/*!
\section
*/
CXAxis::ValueType CXAxis::valueType() const
{
    return m_valueType;
}

void CXAxis::setValueType(const ValueType &valueType)
{
    m_valueType = valueType;
}

/*!
\section
*/
qint64 CXAxis::valueLength() const
{
//    return m_totalLength;
    return (qint64)(m_totalLength * (endScale() - startScale()));
}
qint64 CXAxis::totalLength() const
{
    return m_totalLength;
}

void CXAxis::setTotalLength(const qint64 &totalLength)
{
    m_totalLength = totalLength;
}

QString CXAxis::unit() const
{
    return m_unit;
}

void CXAxis::setUnit(const QString &uint)
{
    m_unit = uint;
}
/*!
\section
*/
QRectF CXAxis::drawArea() const
{
    return m_drawArea;
}

void CXAxis::setDrawArea(const QRectF &drawArea)
{
    m_drawArea = drawArea;
}
int CXAxis::divNum() const
{
    return m_divNum;
}

void CXAxis::setDivNum(int divNum)
{
    m_divNum = divNum;
}
double CXAxis::xHeight() const
{
    return m_xHeight;
}

void CXAxis::setXHeight(double xHeight)
{
    m_xHeight = xHeight;
}
/*!
\section Time
*/
QString CXAxis::timeFormat() const
{
    return m_timeFormat;
}

void CXAxis::setTimeFormat(const QString &timeFormat)
{
    m_timeFormat = timeFormat;
}

QDateTime CXAxis::endTime() const
{
//    return m_originalEndTime;
    return {m_originalEndTime.addSecs(-m_totalLength*(1-endScale()))};
}

void CXAxis::setEndTime(const QDateTime &endTime)
{
    if(endTime > QDateTime::currentDateTime())
        return;
    m_originalEndTime = endTime;

}
QDateTime CXAxis::timeMark() const
{
    return m_timeMark;
}

void CXAxis::setTimeMark(const QDateTime &timeMark)
{
    m_timeMark = timeMark;
}

void CXAxis::setFinalEnd(const QDateTime &xEndTime,bool isStudioMode)
{
    if(xEndTime < m_originalEndTime && !isStudioMode)
        return;
    m_originalEndTime = xEndTime;

}

void CXAxis::setOriginalEndTime(const QDateTime &dateTime)
{
    m_originalEndTime = dateTime;
}

QDateTime CXAxis::originEndTime() const
{
    return m_originalEndTime;
}

void CXAxis::setOriginalLength(const int &length)
{
    m_originalLength = length;
}

int CXAxis::originLength() const
{
    return m_originalLength;
}
/*!
\section
*/
double CXAxis::minValue() const
{
//    return m_minValue;
    return m_minValue + (m_maxValue - m_minValue) * startScale();
}

void CXAxis::setMinValue(double minValue)
{
    m_minValue = minValue;
}

double CXAxis::maxValue() const
{
//    return m_maxValue;
    return m_minValue + (m_maxValue - m_minValue) * endScale();
}

void CXAxis::setMaxValue(double maxValue)
{
    m_maxValue = maxValue;
}
double CXAxis::originMin() const
{
    return m_originMin;
}

void CXAxis::setOriginMin(double originMin)
{
    m_originMin = originMin;
}

double CXAxis::originMax() const
{
    return m_originMax;
}

void CXAxis::setOriginMax(double originMax)
{
    m_originMax = originMax;
}

/*!
\section
*/
void CXAxis::calculate()
{
    if(!axisVisible())
        return;
    switch (m_valueType) {
    case ValueType::Time:
        calculateTime();
        break;
    case ValueType::Value:
//        m_totalLength = m_originMax - m_originMin;
        calculateValue();
        break;
    default:
        break;
    }
}

void CXAxis::drawAxisLine(QPainter *p)
{
    p->save();

    double lineLength = m_drawArea.width();
    QPen linePen;
    linePen.setColor(axisLine()->lineColor());
    linePen.setStyle(axisLine()->lineStyle());
    linePen.setWidth(axisLine()->lineWidth());

    QBrush brush;
    brush.setColor(axisLine()->lineColor());
    brush.setStyle(Qt::SolidPattern);

    p->setPen(linePen);
    p->setBrush(brush);
    p->drawLine(0,0,lineLength,0);

    // 三角形边长为8
    double trW = 8* 0.5 * sqrt(3);
    double tH = -4;
    double tX = lineLength - trW;
    double tL = 4;

    QPainterPath path;
    path.moveTo(tX,tH);
    path.lineTo(lineLength,0);
    path.lineTo(tX,tL);
    path.closeSubpath();
    path.setFillRule(Qt::WindingFill);
    p->drawPath(path);

    p->restore();
}

void CXAxis::draw(QPainter *p)
{
    if(!axisVisible())
        return;

    p->save();

    // 以绘制区域的左上点为(0,0)点
    p->translate(m_drawArea.topLeft());
    p->setPen(fontColor());
    p->setFont(font());

    QFontMetricsF metrics(font());
    double textHeight = metrics.ascent() + metrics.descent();

    for (int i = 0 ;i < m_textXPointList.count();i++)
    {
        int xPoint = m_textXPointList.at(i);
        QString value = m_valueStringList.at(i);
        p->drawText(QPointF(xPoint,textHeight),value);
    }

    //绘制x轴线
    if(axisLineVisible())
        drawAxisLine(p);

    p->restore();
}
void CXAxis::calculateValue()
{
    QRectF area = m_drawArea;
    QFontMetricsF metrics(font());
//    double valueLength = m_totalLength * (endScale() - startScale());
    double valueLength = (m_maxValue - m_minValue) * (endScale() - startScale());

    double axisLineLength = area.width() - ArrowLength;
    double deltaSec= valueLength/m_divNum;

    double maxValue = m_maxValue -(m_maxValue - m_minValue)*(1-endScale());
    double minValue = m_maxValue -(m_maxValue - m_minValue)*(1-startScale());

    Q_ASSERT(m_divNum > 0);

    double deltaWid = axisLineLength / m_divNum;

    m_textXPointList.clear();
    m_valueStringList.clear();


    m_valueStringList.append(QString::number(minValue,'f',valuePrecision()));
    m_textXPointList.append(0);

    for(int i = 1;i< m_divNum;i++) {
        QString value = QString::number(minValue+deltaSec*i,'f',valuePrecision());
        //值
        m_valueStringList.append(value);
        //字体的宽度
        double stringWidth = metrics.width(value);

        //每个值绘制的x坐标
        double valueX = deltaWid *i -stringWidth/2;
        m_textXPointList.append(valueX);
    }


    QString maxString = QString::number(maxValue,'f',valuePrecision()) +m_unit+" " +comment();//m_maxValue

    m_valueStringList.append(maxString);
    double maxLength = axisLineLength - metrics.width(maxString);
    m_textXPointList.append(maxLength);
}

void CXAxis::calculateTime()
{
    QRectF area = m_drawArea;
    QFontMetricsF metrics(font());

    double axisLineLength = area.width() - ArrowLength;
    QDateTime endTime = m_originalEndTime.addSecs(-m_totalLength*(1-endScale()));
    QDateTime startTime = m_originalEndTime.addSecs(-m_totalLength*(1-startScale()));
    Q_ASSERT(m_divNum > 0);
    double deltaSec = m_totalLength*(endScale() - startScale()) / m_divNum;

    double deltaWid = axisLineLength / m_divNum;

    m_textXPointList.clear();
    m_valueStringList.clear();

    m_valueStringList.append(startTime.toString(timeFormat()));
    m_textXPointList.append(0);

    for(int i = 1;i< m_divNum;i++) {
        QString value = startTime.addSecs(deltaSec*i).toString(timeFormat());
        //值
        m_valueStringList.append(value);
        //字体的宽度
        double stringWidth = metrics.width(value);

        //每个值绘制的x坐标
        double valueX = deltaWid *i-stringWidth/2;
        m_textXPointList.append(valueX);
    }

    //endTime
    QString maxString = endTime.toString(timeFormat()) +m_unit+  " " +comment();

    m_valueStringList.append(maxString);
    double maxLength = axisLineLength - metrics.width(maxString);
    m_textXPointList.append(maxLength);
}

/*!
\section
*/
void CXAxis::serialize(QJsonObject &json)
{
    CAbstractAxis::serialize(json);
    QJsonObject xAxisJson;
    xAxisJson.insert("DrawArea",CMDJsonConvert::toJson(m_drawArea));
    xAxisJson.insert("Xheight",m_xHeight);
    xAxisJson.insert("ValueType",(int)m_valueType);
    xAxisJson.insert("TotalLength",m_totalLength);
    xAxisJson.insert("MaxValue",m_maxValue);
    xAxisJson.insert("MinValue",m_minValue);
    xAxisJson.insert("OriginMax",m_originMax);
    xAxisJson.insert("OriginMin",m_originMin);
    xAxisJson.insert("TimeFormat",m_timeFormat);
    xAxisJson.insert("Unit",m_unit);
    xAxisJson.insert("OriginalLength",m_originalLength);
    json.insert("XAxis",xAxisJson);

}

void CXAxis::deserialize(const QJsonObject &json)
{
    CAbstractAxis::deserialize(json);
    QJsonObject xAxisJson = json.value("XAxis").toObject();
    m_drawArea =CMDJsonConvert::toQRectF(xAxisJson.value("DrawArea"));
    m_xHeight= xAxisJson.value("Xheight").toDouble();
    int type = xAxisJson.value("ValueType").toInt();
    m_valueType = (ValueType)type;
    m_totalLength = xAxisJson.value("TotalLength").toDouble();
    m_maxValue = xAxisJson.value("MaxValue").toDouble();
    m_minValue = xAxisJson.value("MinValue").toDouble();
    m_originMax = xAxisJson.value("OriginMax").toDouble();
    m_originMin = xAxisJson.value("OriginMin").toDouble();
    m_timeFormat = xAxisJson.value("TimeFormat").toString();
    m_unit = xAxisJson.value("Unit").toString();
    m_originalLength = xAxisJson.value("OriginalLength").toInt();

}

/*!
\section
*/
CXAxis &CXAxis::operator =(const CXAxis &other)
{
    if(&other == this)
        return *this;

    this->m_drawArea = other.m_drawArea;
    this->m_xHeight = other.m_xHeight;
    this->m_valueType = other.m_valueType;
    this->m_maxValue = other.m_maxValue;
    this->m_minValue = other.m_minValue;
    this->m_originMax = other.m_originMax;
    this->m_originMin = other.m_originMin;
    this->m_originalLength = other.m_originalLength;
    this->m_timeFormat = other.m_timeFormat;
    this->m_unit = other.m_unit;
    CAbstractAxis::operator =(other);

    return *this;
}




