#include "CReferenceCurve.h"

#include "CXAxis.h"
#include "CYAxis.h"
#include "CPoint.h"
#include <QDebug>
#include <QDateTime>
#include <QJsonObject>
#include "MDJsonConvert.h"

CReferenceCurve::CReferenceCurve():
    m_xAxis(NULL),
    m_yAxis(NULL),
    m_reCurveColor(Qt::red),
    m_reCurveWidth(1),
    m_reCurveStyle(1),
    m_reCurveVisible(true),
    m_reVernierVisible(true),
    m_normalPoint(new CPoint),
    m_startTime(QDateTime::currentDateTime().toMSecsSinceEpoch()),
    m_isCalculated(false),
    m_startValue(0)
{

}

CReferenceCurve::~CReferenceCurve()
{
    if(m_normalPoint){
        delete m_normalPoint;
        m_normalPoint = NULL;
    }
}

CXAxis *CReferenceCurve::xAxis() const
{
    return m_xAxis;
}

void CReferenceCurve::setXAxis(CXAxis *xAxis)
{
    m_xAxis = xAxis;
}

QString CReferenceCurve::xName() const
{
    return m_xName;
}

void CReferenceCurve::setXName(const QString &xName)
{
    m_xName = xName;
}

CYAxis *CReferenceCurve::yAxis()
{
    return m_yAxis;
}

void CReferenceCurve::setYAxis(CYAxis *yAxis)
{
    m_yAxis = yAxis;
}

QString CReferenceCurve::yName() const
{
    return m_yName;
}

void CReferenceCurve::setYName(const QString &yName)
{
    m_yName = yName;
}

CCurveData &CReferenceCurve::rawData()
{
    return m_rawData;
}

void CReferenceCurve::setRawData(const CCurveData &rawData)
{
    m_rawData = rawData;
}

CCurveVisible &CReferenceCurve::totalVisblePoint()
{
    return m_totalVPoint;
}

void CReferenceCurve::clearDataForZoom()
{
    QPoint point;
    m_lastPoint = point;
    m_preEndTime = QDateTime();
}

QString CReferenceCurve::name() const
{
    return m_reName;
}

void CReferenceCurve::setName(const QString &name)
{
    m_reName = name;
}

QString CReferenceCurve::comment() const
{
    return m_reComment;
}

void CReferenceCurve::setComment(const QString &comment)
{
    m_reComment = comment;
}

QString CReferenceCurve::variantDepict() const
{
    return m_variantDepict;
}

void CReferenceCurve::setVariantDepict(const QString &variantDepict)
{
    m_variantDepict = variantDepict;
}

int CReferenceCurve::curveWidth() const
{
    return m_reCurveWidth;
}

void CReferenceCurve::setCurveWidth(int curveWidth)
{
    m_reCurveWidth = curveWidth;
}

QColor CReferenceCurve::reCurveColor() const
{
    return m_reCurveColor;
}

void CReferenceCurve::setReCurveColor(const QColor &curveColor)
{
    m_reCurveColor = curveColor;
}

void CReferenceCurve::setCurveStyle(int style)
{
    m_reCurveStyle = style;
}

qint32 CReferenceCurve::curveStyle() const
{
    return m_reCurveStyle;
}

QRectF CReferenceCurve::curveArea() const
{
    return m_reCurveArea;
}

void CReferenceCurve::setCurveArea(const QRectF &curveArea)
{
    m_reCurveArea = curveArea;
}

bool CReferenceCurve::curveVisible() const
{
    return m_reCurveVisible;
}

void CReferenceCurve::setCurveVisible(bool curveVisible)
{
    m_reCurveVisible = curveVisible;
}

bool CReferenceCurve::reCurveVernierVisible() const
{
    return m_reVernierVisible;
}

void CReferenceCurve::setReCurveVernierVisible(bool vernierVisible)
{
    m_reVernierVisible = vernierVisible;
}

CPoint *CReferenceCurve::normalPoint()
{
    return m_normalPoint;
}

void CReferenceCurve::draw(QPainter *p)
{
    if(!m_isCalculated)
        return;
    p->save();
    p->setClipRect(m_reCurveArea);
    p->translate(m_reCurveArea.x(),m_reCurveArea.y()+m_reCurveArea.height());
    // 设置曲线属性
    QPen pen;
    pen.setColor(m_reCurveColor);
    pen.setStyle((Qt::PenStyle)m_reCurveStyle);
    pen.setWidth(m_reCurveWidth);
    p->setPen(pen);
    // 绘制线
    for(int i = 0;i < m_visiblePoint.count()-1;++i)
    {
        p->drawLine(m_visiblePoint.at(i),m_visiblePoint.at(i+1));
    }

    m_normalPoint->draw(p);
    p->restore();

}

void CReferenceCurve::calculate()
{
    if(!calculateCheck())
        return;

    clearOldData();
    switch (m_xAxis->valueType())
    {
    case CXAxis::ValueType::Time:
        calculateTimeType();
        break;
    case CXAxis::ValueType::Value:
        calculateValueType();
        break;
    default:
        break;
    }
    calVisiblePoint();
    m_isCalculated = true;

}

void CReferenceCurve::calculatePoint(const QPointF &p)
{
    if(m_normalPoint->visible())
    {
        m_normalPoint->figurePoint(p);
    }

}

bool CReferenceCurve::calculateCheck()
{
    if(!m_xAxis || !m_yAxis || m_reCurveArea.isNull()
            || m_rawData.isEmpty()|| !m_reCurveVisible){
        m_isCalculated = false;
        return m_isCalculated;
    }
    return true;
}

void CReferenceCurve::calVisiblePoint()
{
    if(!m_visiblePoint.isEmpty()){
//        CFind::processData(m_visiblePoint);
//        if(m_isReal){
//            if(m_isCurveRunning){
//                int totalCount = m_totalVPoint.count()+ m_visiblePoint.count();
//                if(totalCount > m_xAxis->valueLength())
//                    m_totalVPoint = m_totalVPoint.mid(totalCount- m_xAxis->valueLength()-1);// maybe error
//                for(int i = 0; i < m_totalVPoint.count();i++){
//                    QPoint point = m_totalVPoint.at(i);
////                    m_totalVPoint[i].setX(point.x()-ceil(m_picLength));
//                }
//                m_totalVPoint.append(m_visiblePoint);
//            }else{
//                m_totalVPoint.clear();
//            }
//        }
//        else{
            m_totalVPoint.clear();
            m_totalVPoint = m_visiblePoint;
//        }
        for(int i = 0; i < m_visiblePoint.count();i++){
            QPointF point = m_visiblePoint.at(i);
            calculatePoint(point);
        }
    }
}

void CReferenceCurve::clearOldData()
{
    m_visiblePoint.clear();
    m_normalPoint->clearPath();
}

void CReferenceCurve::calculateTimeType()
{
    if(m_rawData.timeList().isEmpty())
        return;

    double xWidth = m_reCurveArea.width();
    qint64 valueLength = m_xAxis->valueLength();

    double yHeight = m_reCurveArea.height();
    double yValueRange = m_yAxis->maxValue() - m_yAxis->minValue();

    QDateTime timeMark = m_xAxis->timeMark();
    QDateTime endTime = m_xAxis->endTime();
    QDateTime startTime = endTime.addSecs(-valueLength);
    qint64 time = startTime.secsTo(timeMark);

    if(m_preEndTime.isValid()){
        startTime = m_preEndTime;
    }

    for(int i = 0; i < m_rawData.timeList().count();i++){
        CTimeValuePoint timePoint = m_rawData.timeList().at(i);
        qint64 timeConst = timeMark.secsTo(timePoint.xTime());

        double tx = xWidth *timeConst / valueLength;
        int cx = xWidth*time/valueLength;
        tx += cx;

        double value = timePoint.y() - m_yAxis->minValue();
        double cy =  -yHeight/yValueRange*value;

        m_visiblePoint.append(QPointF(tx,cy).toPoint());

    }

    m_preEndTime = m_xAxis->endTime();
}

void CReferenceCurve::calculateValueType()
{
    if(m_rawData.valueList().isEmpty())
        return;

    double xWidth = m_reCurveArea.width();
    double yHeight = m_reCurveArea.height();
    double yMaxValue = m_yAxis->maxValue();
    double yMinValue = m_yAxis->minValue();
    double yValueRange = yMaxValue - yMinValue;
//    double xMaxValue = m_xAxis->maxValue()-(m_xAxis->maxValue()-m_xAxis->minValue())*(1-m_xAxis->endScale());
//    double xMinValue = m_xAxis->maxValue()-(m_xAxis->maxValue()-m_xAxis->minValue())*(1-m_xAxis->startScale());
    double xMaxValue = m_xAxis->maxValue();
    double xMinValue = m_xAxis->minValue();

    for(int i = 0; i < m_rawData.valueList().count();i++){
        CPureValuePoint valuePoint = m_rawData.valueList().at(i);
        double rawDataY = valuePoint.y();
        double rawDataX = valuePoint.xDouble();
        double y =  -yHeight/yValueRange*(rawDataY - yMinValue);
        double x =  xWidth/(xMaxValue-xMinValue) *(rawDataX- xMinValue);
        m_visiblePoint.append(QPointF(x,y).toPoint());
    }
}

void CReferenceCurve::serialize(QJsonObject &json)
{
    json.insert("ReName",m_reName);
    json.insert("ReComment",m_reComment);
    json.insert("ReCurveColor",(double)m_reCurveColor.rgba());
    json.insert("ReCurveStyle",m_reCurveStyle);
    json.insert("ReCurveWidth",m_reCurveWidth);
    json.insert("ReCurveVisible",m_reCurveVisible);
    json.insert("ReCurveArea",CMDJsonConvert::toJson(m_reCurveArea));
    json.insert("XName",m_xName);
    json.insert("YName",m_yName);
    json.insert("VariantDepict",m_variantDepict);
    json.insert("ReVernierVisible",m_reVernierVisible);
    QJsonObject normalJson;
    m_normalPoint->serialize(normalJson);
    json.insert("NormalPoint",normalJson);

}

void CReferenceCurve::deserialize(const QJsonObject &json)
{
    m_reName = json.value("ReName").toString();
    m_reComment = json.value("ReComment").toString();
    m_reCurveColor = QColor::fromRgba(json.value("ReCurveColor").toDouble());
    m_reCurveStyle = json.value("ReCurveStyle").toInt();
    m_reCurveWidth = json.value("ReCurveWidth").toInt();
    m_reCurveVisible = json.value("ReCurveVisible").toBool();
    m_reCurveArea = CMDJsonConvert::toQRectF(json.value("ReCurveArea"));
    m_xName = json.value("XName").toString();
    m_yName = json.value("YName").toString();
    m_variantDepict = json.value("VariantDepict").toString();
    m_reVernierVisible = json.value("ReVernierVisible").toBool();
    QJsonObject normalJson = json.value("NormalPoint").toObject();
    m_normalPoint->deserialize(normalJson);
}

CReferenceCurve &CReferenceCurve::operator =(const CReferenceCurve &other)
{
    if(&other == this)
        return *this;
    this->m_reName        = other.m_reName;
    this->m_reComment     = other.m_reComment;
    this->m_reCurveColor = other.m_reCurveColor;
    this->m_reCurveStyle = other.m_reCurveStyle;
    this->m_reCurveWidth = other.m_reCurveWidth;
    this->m_reCurveVisible = other.m_reCurveVisible;
    this->m_reVernierVisible = other.m_reVernierVisible;
    this->m_reCurveArea = other.m_reCurveArea;
    this->m_xName = other.m_xName;
    this->m_yName = other.m_yName;
    this->m_variantDepict = other.m_variantDepict;
    *this->m_normalPoint = *other.m_normalPoint;

    return *this;
}

void CReferenceCurve::setStartValue(const QVariant &value)
{
    m_startValue = value;
}

QVariant CReferenceCurve::startValue() const
{
    return m_startValue;
}

void CReferenceCurve::setStartTime(const QVariant &startTime)
{
    m_startTime = startTime;
}

QVariant CReferenceCurve::startTime() const
{
    return m_startTime;
}

