#include "CCurveObject.h"
#include "CXAxis.h"
#include "CYAxis.h"
#include "CPoint.h"
#include "CFind.h"
#include "MDJsonConvert.h"
#include <QJsonObject>
#include <math.h>
#include <QRectF>
#include <QRegion>
#include <QDebug>
//#include <sys/time.h>
#include <time.h>

CCurveObject::CCurveObject():
    m_xAxis(NULL),
    m_yAxis(NULL),
    m_curveColor(Qt::black),
    m_curveWidth(1),
    m_curveStyle(Qt::SolidLine),
    m_curveVisible(true),
    m_curveVernierVisible(true),
    m_isReal(false),
    m_isCurveRunning(false),
    m_freshTime(1000),
    m_upWarningLine(new CWarningLine),
    m_belowWarningLine(new CWarningLine),
    m_normalPoint(new CPoint),
    m_warningPoint(new CPoint),
    m_isCalculated(false),
    m_picLength(0),
    m_isBoundedPreview(false)

{
}

CCurveObject::~CCurveObject()
{
    if(m_upWarningLine)
    {
        delete m_upWarningLine;
        m_upWarningLine = NULL;
    }
    if(m_belowWarningLine)
    {
        delete m_belowWarningLine;
        m_belowWarningLine = NULL;
    }

    if(m_normalPoint)
    {
        delete m_normalPoint;
        m_normalPoint = NULL;
    }
    if(m_warningPoint)
    {
        delete m_warningPoint;
        m_warningPoint = NULL;
    }
}

/*!
\section axis
*/
CXAxis *CCurveObject::xAxis() const
{
    return m_xAxis;
}

void CCurveObject::setXAxis(CXAxis *xAxis)
{
    if(xAxis)
    {
        m_xAxis = xAxis;
        m_xAxis->addCurve(this);
    }
}
QString CCurveObject::xName() const
{
    return m_xName;
}

void CCurveObject::setXName(const QString &xName)
{
    m_xName = xName;
}

CYAxis *CCurveObject::yAxis()
{
    return m_yAxis;
}

void CCurveObject::setYAxis(CYAxis *yAxis)
{
    if(yAxis){
        m_yAxis = yAxis;
        m_yAxis->addCurve(this);
    }
}

QString CCurveObject::yName() const
{
    return m_yName;
}

void CCurveObject::setYName(const QString &yName)
{
    m_yName = yName;
}

void CCurveObject::setProjectPath(const QString &path)
{
    m_projectPath = path;
}

QString CCurveObject::projectPath() const
{
    return m_projectPath;
}

CCurveData &CCurveObject::rawData()
{
    return m_rawData;
}

void CCurveObject::setRawData(const CCurveData &rawData)
{
    m_rawData = rawData;
}

void CCurveObject::setPreviewData(const CCurveData &rawData)
{
    m_rawPreviewData = rawData;
}

CCurveData &CCurveObject::previewData()
{
    return m_rawPreviewData;
}

void CCurveObject::setBoundedPreview(bool isBounded)
{
    m_isBoundedPreview = isBounded;
}

bool CCurveObject::isBoundedPreview() const
{
    return m_isBoundedPreview;
}

/*!
\section data
*/

CCurveVisible &CCurveObject::totalVisblePoint()
{
    return m_totalVPoint;
}

void CCurveObject::clearDataForZoom()
{
    QPixmap nullPic;
    m_pixmap = nullPic;
    m_prePixmap = nullPic;


    QPoint point;
    m_lastPoint = point;
    m_preEndTime = QDateTime();

    m_visiblePoint.clear();
    m_totalVPoint.clear();

}

/*!
\section properties
*/
int CCurveObject::curveWidth() const
{
    return m_curveWidth;
}

void CCurveObject::setCurveWidth(int curveWidth)
{
    enum { MINVALUE =1 , MAXVALUE = 10 };
    if(curveWidth < MINVALUE )
        m_curveWidth = MINVALUE;
    else if(curveWidth > MAXVALUE)
        m_curveWidth = MAXVALUE;
    else
        m_curveWidth = curveWidth;
}
QColor CCurveObject::curveColor() const
{
    return m_curveColor;
}

void CCurveObject::setCurveColor(const QColor &curveColor)
{
    m_curveColor = curveColor;
}

void CCurveObject::setCurveStyle(Qt::PenStyle style)
{
    m_curveStyle = style;
}

Qt::PenStyle CCurveObject::curveStyle() const
{
    return m_curveStyle;
}

bool CCurveObject::curveVisible() const
{
    return m_curveVisible;
}

void CCurveObject::setCurveVisible(bool isShowCurve)
{
    m_curveVisible = isShowCurve;
}

bool CCurveObject::curveVernierVisible() const
{
    return m_curveVernierVisible;
}

void CCurveObject::setCurveVernierVisible(bool curveVernierVisible)
{
    m_curveVernierVisible = curveVernierVisible;
}

void CCurveObject::setType(bool isReal)
{
    m_isReal = isReal;
}

void CCurveObject::setFresh(qint32 ms)
{
    m_freshTime = ms;
}

void CCurveObject::setIsRun(bool isRun)
{
    m_isCurveRunning = isRun;
}

QString CCurveObject::name() const
{
    return m_name;
}

void CCurveObject::setName(const QString &name)
{
    m_name = name;
}

QString CCurveObject::comment() const
{
    return m_comment;
}

void CCurveObject::setComment(const QString &comment)
{
    m_comment = comment;
}

QString CCurveObject::variantDepict() const
{
    return m_variantDepict;
}

void CCurveObject::setVariantDepict(const QString &variantDepict)
{
    m_variantDepict = variantDepict;
}

QString CCurveObject::timeVariantDepict() const
{
    return m_timeVariantDepict;
}

void CCurveObject::setTimeVariantDepict(const QString &timeVariantDepict)
{
    m_timeVariantDepict = timeVariantDepict;
}

QString CCurveObject::viewName() const
{
    return m_viewName;
}

void CCurveObject::setViewName(const QString &str)
{
    m_viewName = str;
}
QRectF CCurveObject::curveArea() const
{
    return m_curveArea;
}

void CCurveObject::setCurveArea(const QRectF &curveArea)
{
    m_curveArea = curveArea;
}

/*!
\section warningLines and points
*/

CWarningLine *CCurveObject::upWarningLine()
{
    return m_upWarningLine;
}

CWarningLine *CCurveObject::belowWarningLine()
{
    return m_belowWarningLine;
}

CPoint *CCurveObject::normalPoint()
{
    return m_normalPoint;
}

CPoint *CCurveObject::warningPoint()
{
    return m_warningPoint;
}

/*!
\section calculate and draw
*/
void CCurveObject::draw(QPainter *p)
{
    if(!m_isCalculated)
        return;
    p->save();
    if(m_isCurveRunning)
    {
        p->drawPixmap(m_curveArea.topLeft(),m_pixmap);
    }
    else
    {
        p->drawPixmap(m_curveArea.topLeft(),m_prePixmap);
    }
    p->restore();
}

void CCurveObject::drawOnPixmap()
{
    QPixmap pixmap(m_curveArea.size().toSize());
    pixmap.fill(QColor(0,0,0,0));
    QPainter painter(&pixmap);
    if(!m_pixmap.isNull() /*&& m_picLength*/ && m_isCurveRunning)
    {
//        if(m_xAxis->endTime() > QDateTime::currentDateTime())  // the left to right mode
//            m_picLength = 0;
        QRectF rect;
        rect.setTopLeft(QPointF(m_picLength,0));
        rect.setSize(QSizeF(m_curveArea.width()- m_picLength,m_curveArea.height()));
        painter.drawPixmap(QPointF(0,0),m_pixmap,rect);
    }

    painter.translate(0,m_curveArea.height());
    // 设置曲线属性
    QPen pen;
    pen.setColor(m_curveColor);
    pen.setStyle(m_curveStyle);
    pen.setWidth(m_curveWidth);
    painter.setPen(pen);

    // 绘制曲线
    for(int i = 0;i < m_visiblePoint.count()-1;++i)
    {
        painter.drawLine(m_visiblePoint.at(i),m_visiblePoint.at(i+1));
    }

    m_normalPoint->draw(&painter);
    m_upWarningLine->draw(&painter);
    m_belowWarningLine->draw(&painter);
    m_warningPoint->draw(&painter);

    if(m_isCurveRunning)
    {
        m_pixmap = pixmap;
    }
    m_prePixmap = pixmap;
}

void CCurveObject::calculate()
{
    if(!calculateCheck())
        return;

    clearOldData();
    if(m_picLength < 0.5)
        clearDataForZoom();

    calculateWarningLine(m_upWarningLine,m_curveArea,m_yAxis);
    calculateWarningLine(m_belowWarningLine,m_curveArea,m_yAxis);

    if(!m_isCurveRunning)
    {
        if(m_prePixmap.isNull())
        {
            m_prePixmap = m_pixmap;
            QPixmap pixmap;
            m_pixmap = pixmap;
        }
    }

    switch (m_xAxis->valueType())
    {
    case CXAxis::ValueType::Time:
    {
        calculateTimeType();
    }
        break;
    case CXAxis::ValueType::Value:
        calculateValueType();
        break;
    default:
        break;
    }
    calVisiblePoint();
    drawOnPixmap();
    m_isCalculated = true;
}

void CCurveObject::calculatePoint(const QPointF &p)
{
    if(p.y() < m_upWarningLine->yPoint() || p.y() > m_belowWarningLine->yPoint())
    {
        if(m_warningPoint->visible())
            m_warningPoint->figurePoint(p);
        else if(m_normalPoint->visible())
            m_normalPoint->figurePoint(p);
    }
    else
    {
        if(m_normalPoint->visible())
            m_normalPoint->figurePoint(p);
    }
}

bool CCurveObject::calculateCheck()
{
    if(!m_xAxis || !m_yAxis || m_curveArea.isNull()
            || m_rawData.isEmpty()|| !m_curveVisible)
    {
        m_isCalculated = false;
        return m_isCalculated;
    }
    return true;
}

void CCurveObject::calVisiblePoint()
{
    if(!m_visiblePoint.isEmpty())
    {
        CFind::processData(m_visiblePoint);
        if(m_isReal)
        {
            if(m_isCurveRunning)
            {
//                int totalCount = m_totalVPoint.count()+ m_visiblePoint.count();
//                if(totalCount > m_xAxis->valueLength())
//                    m_totalVPoint = m_totalVPoint.mid(totalCount- m_xAxis->valueLength()-1);// maybe error

                for(int i = 0; i < m_totalVPoint.count();i++)
                {
                    QPoint point = m_totalVPoint.at(i);
                    m_totalVPoint[i].setX(point.x() - m_picLength);
                }
                m_totalVPoint.append(m_visiblePoint);
            }
            else
            {
                m_totalVPoint.clear();
            }
        }
        else
        {
            m_totalVPoint.clear();
            m_totalVPoint = m_visiblePoint;
        }

        for(int i = 0; i < m_visiblePoint.count();i++)
        {
            QPointF point = m_visiblePoint.at(i);
            calculatePoint(point);
        }
    }
}

void CCurveObject::clearOldData()
{
    m_visiblePoint.clear();
    m_normalPoint->clearPath();
    m_warningPoint->clearPath();
}

void CCurveObject::calculateWarningLine(CWarningLine *line, const QRectF &area, CYAxis *y)
{
    if(line->visible())
    {
        double yHeight = area.height();
        double xWidth = area.width();
        double yMaxValue;
        double yMinValue;
        if(!y->isNumeric() && m_isCurveRunning)
        {
            yMaxValue = y->maxPercent();
            yMinValue = y->minPercent();
        }
        else
        {
            yMaxValue = y->maxValue();
            yMinValue = y->minValue();
        }

        double yPoint = -yHeight/(yMaxValue - yMinValue)*(line->value() - yMinValue);
        line->setLinePoint(QPointF(0,yPoint),QPointF(xWidth,yPoint));
        line->setYPoint(yPoint);
    }
}

// 计算曲线区域

// Warning:: the Point transform is based on the Cooradinate has been translated
// Point: x = xWidth/ timeLength * (time-startTime);
// add the third point Mark to avoid the loss.
// calculate the raw point to the Mark as the first part
// the startPoint to the Mark as the second part
// Point: y =  - yHeight/(maxValue-minValue)* (value-minValue);
void CCurveObject::calculateTimeType()
{
    if(m_rawData.timeList().isEmpty())
        return;

    double yMaxValue;
    double yMinValue;
    if(!m_yAxis->isNumeric() && m_isCurveRunning)
    {
        yMaxValue = m_yAxis->maxPercent();
        yMinValue = m_yAxis->minPercent();
    }
    else
    {
        yMaxValue = m_yAxis->maxValue();
        yMinValue = m_yAxis->minValue();
    }

    double xWidth = m_curveArea.width();
    qint64 valueLength = m_xAxis->valueLength();

    double yHeight = m_curveArea.height();
    double yValueRange = yMaxValue - yMinValue;

    QDateTime timeMark = m_xAxis->timeMark();
    QDateTime endTime = m_xAxis->endTime();

    QDateTime startTime = endTime.addSecs(-valueLength);
    qint64 time = startTime.secsTo(timeMark); // 优化数据的固定值

    if(m_isReal && m_freshTime && m_isCurveRunning && !m_pixmap.isNull())
    {
        if(m_xAxis->endTime() > QDateTime::currentDateTime())  // the left to right mode
            m_picLength = 0;
        else
        {
            m_picLength = (xWidth/valueLength)*(m_freshTime/1000);
        }

        if(m_preEndTime.isValid())
        {
            startTime = m_preEndTime;
        }
    }

    // 数据缓冲
    QList<CTimeValuePoint> list;
    list.clear();
    int firstIndex = CFind::binarySearch(m_rawData.timeList(),startTime);
    int lastIndex = CFind::binarySearch(m_rawData.timeList(),endTime);

    if(lastIndex != -1)
    {
        if(firstIndex != lastIndex)
        {
            qint64 timeRange = valueLength/xWidth;
            CFind::proData(&m_rawData.timeList(),timeRange, firstIndex,lastIndex,&list);
        }
        else
        {
            list.append(m_rawData.timeList().at(firstIndex));
        }
    }

    for(int i = 0; i < list.count();i++)
    {
        CTimeValuePoint timePoint = list.at(i);
        qint64 timeConst = timeMark.secsTo(timePoint.xTime());

        // tX 为数据点到Mark点的长度
        double tx = xWidth *timeConst / valueLength;
        double cx = xWidth*time/valueLength;
        tx += cx;

        double value = timePoint.y() - yMinValue;
        double cy =  -yHeight/yValueRange*value;
        m_visiblePoint.append(QPointF(tx,cy).toPoint());
//                calculatePoint(QPointF(tx,cy));
    }

    if(m_isReal && !m_lastPoint.isNull() && m_isCurveRunning)
    {
            m_visiblePoint.push_front(m_lastPoint);
    }

    if(!m_visiblePoint.isEmpty() && m_isReal && m_isCurveRunning)
    {
        m_lastPoint = m_visiblePoint.last();
        double x = m_lastPoint.x();
        if(m_xAxis->endTime() <= QDateTime::currentDateTime()) // the left to right mode
            m_lastPoint.setX(x - m_picLength);
    }

// set the last Point to Null,when the curve running again,the last point would not effect the points.
    if(!m_isCurveRunning)
    {
        QPoint point;
        m_lastPoint = point;
    }
    m_preEndTime = m_xAxis->endTime();
}

void CCurveObject::calculateValueType()
{
    if(m_rawData.valueList().isEmpty())
        return;

    double xWidth = m_curveArea.width();
    double yHeight = m_curveArea.height();

    double yMaxValue;
    double yMinValue;
    if(!m_yAxis->isNumeric() && m_isCurveRunning)
    {
        yMaxValue = m_yAxis->maxPercent();
        yMinValue = m_yAxis->minPercent();
    }
    else
    {
        yMaxValue = m_yAxis->maxValue();
        yMinValue = m_yAxis->minValue();
    }

    double yValueRange = yMaxValue - yMinValue;
    double xMaxValue = m_xAxis->maxValue();
    double xMinValue = m_xAxis->minValue();

    for(int i = 0; i < m_rawData.valueList().count();i++){
        CPureValuePoint valuePoint = m_rawData.valueList().at(i);
        double rawDataY = valuePoint.y();
        double rawDataX = valuePoint.xDouble();
        double y =  -yHeight/yValueRange*(rawDataY - yMinValue);
        double x =  xWidth/(xMaxValue-xMinValue) *(rawDataX- xMinValue) + ceil(m_picLength);

        m_visiblePoint.append(QPointF(x,y).toPoint());
//        m_totalVPoint.append(m_visiblePoint);
//        calculatePoint(QPointF(x,y));
    }
}

/*!
\section serialize
*/

void CCurveObject::serialize(QJsonObject &json)
{
    json.insert("CurveStyle",(int)m_curveStyle);
    json.insert("Name",m_name);
    json.insert("Comment",m_comment);
    json.insert("VariantDepict",m_variantDepict);
    json.insert("CurveColor",(double)m_curveColor.rgba());
    json.insert("CurveWidth",m_curveWidth);
    json.insert("CurveVisible",m_curveVisible);
    json.insert("CurveArea",CMDJsonConvert::toJson(m_curveArea));
    json.insert("XName",m_xName);
    json.insert("YName",m_yName);
    json.insert("CurveVernierVisible",m_curveVernierVisible);
    json.insert("IsBoundedPreview",m_isBoundedPreview);
    json.insert("ProjectPath",m_projectPath);
    json.insert("ViewName",m_viewName);
    json.insert("TimeVariantDepict",m_timeVariantDepict);
    QJsonObject upJson;
    m_upWarningLine->serialize(upJson);
    json.insert("UpWaringLine",upJson);
    QJsonObject belowJson;
    m_belowWarningLine->serialize(belowJson);
    json.insert("BelowWarningLine",belowJson);
    QJsonObject normalJson;
    m_normalPoint->serialize(normalJson);
    json.insert("NormalPoint",normalJson);
    QJsonObject warningJson;
    m_warningPoint->serialize(warningJson);
    json.insert("WarningPoint",warningJson);
}

void CCurveObject::deserialize(const QJsonObject &json)
{
    int style = json.value("CurveStyle").toInt();
    m_curveStyle = (Qt::PenStyle)style;
    m_name = json.value("Name").toString();
    m_comment = json.value("Comment").toString();
    m_variantDepict = json.value("VariantDepict").toString();
    m_curveColor = QColor::fromRgba(json.value("CurveColor").toDouble());
    m_curveWidth = json.value("CurveWidth").toInt();
    m_curveVisible = json.value("CurveVisible").toBool();
    m_curveArea = CMDJsonConvert::toQRectF(json.value("CurveArea"));
    m_xName = json.value("XName").toString();
    m_yName = json.value("YName").toString();
    m_curveVernierVisible = json.value("CurveVernierVisible").toBool();
    m_isBoundedPreview = json.value("IsBoundedPreview").toBool();
    m_projectPath = json.value("ProjectPath").toString();
    m_viewName = json.value("ViewName").toString();
    m_timeVariantDepict = json.value("TimeVariantDepict").toString();
    QJsonObject upJson = json.value("UpWaringLine").toObject();
    m_upWarningLine->deserialize(upJson);
    QJsonObject belowJson = json.value("BelowWarningLine").toObject();
    m_belowWarningLine->deserialize(belowJson);
    QJsonObject normalJson = json.value("NormalPoint").toObject();
    m_normalPoint->deserialize(normalJson);
    QJsonObject warningJson = json.value("WarningPoint").toObject();
    m_warningPoint->deserialize(warningJson);
}

CCurveObject &CCurveObject::operator =(const CCurveObject &other)
{
    if(&other == this)
        return *this;
    this->m_name        = other.m_name;
    this->m_comment     = other.m_comment;
    this->m_variantDepict = other.m_variantDepict;
    this->m_curveColor = other.m_curveColor;
    this->m_curveWidth = other.m_curveWidth;
    this->m_curveVisible = other.m_curveVisible;
    this->m_curveArea = other.m_curveArea;
    this->m_curveStyle = other.m_curveStyle;
    this->m_xName = other.m_xName;
    this->m_yName = other.m_yName;
    this->m_curveVernierVisible = other.m_curveVernierVisible;
    this->m_isBoundedPreview = other.m_isBoundedPreview;
    this->m_projectPath = other.m_projectPath;
    this->m_viewName = other.m_viewName;
    this->m_timeVariantDepict = other.m_timeVariantDepict;
    *this->m_upWarningLine = *other.m_upWarningLine;
    *this->m_belowWarningLine = *other.m_belowWarningLine;
    *this->m_normalPoint = *other.m_normalPoint;
    *this->m_warningPoint = *other.m_warningPoint;

    return *this;
}


