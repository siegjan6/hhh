#include "CVernier.h"
#include "CLabelItem.h"
#include "CXAxis.h"
#include "CYAxis.h"
#include "CFind.h"
#include <QDataStream>
#include <QFontMetrics>
#include <QFont>
#include <QJsonObject>
#include "MDJsonConvert.h"
#include <QDebug>
#include <math.h>

#define OUTOFTIME       -4
#define INTIME          -5

CVernier::CVernier():
    m_display(false),
    m_timeFormat("hh:mm:ss"),
    m_isFixed(false),
    m_interval(5),
    m_isVertical(true),
    m_timeLength(3600),
    m_isChoseNearer(true),
    m_customString("???"),
    m_moursePressX(0),
    m_isStudioMode(false),
    m_isCalculated(false)
{
}

CVernier::~CVernier()
{
    clearItemList();
}

bool CVernier::display() const
{
    return m_display;
}

void CVernier::setDisplay(bool display)
{
    m_display = display;
}

QFont CVernier::font() const
{
    return m_font;
}

void CVernier::setFont(const QFont &font)
{
    m_font = font;
}

QString CVernier::timeFormat() const
{
    return m_timeFormat;
}

void CVernier::setTimeFormat(const QString &timeFormat)
{
    m_timeFormat = timeFormat;
}

/*!
\section
*/
bool CVernier::isFixed() const
{
    return m_isFixed;
}

void CVernier::setIsFixed(bool isFixed)
{
    m_isFixed = isFixed;
}
QPoint CVernier::fixedPoint() const
{
    return m_fixedPoint;
}

void CVernier::setFixedPoint(const QPoint &fixedPoint)
{
    m_fixedPoint = fixedPoint;
}

/*!
\section
*/
int CVernier::interval() const
{
    return m_interval;
}

void CVernier::setInterval(int interval)
{
    m_interval = interval;
}
bool CVernier::isVertical() const
{
    return m_isVertical;
}

void CVernier::setIsVertical(bool isVertical)
{
    m_isVertical = isVertical;
}

/*!
\section
*/
int CVernier::timeLength() const
{
    return m_timeLength;
}

void CVernier::setTimeLength(int timeLength)
{
    m_timeLength = timeLength;
}

QString CVernier::customString() const
{
    return m_customString;
}

void CVernier::setCustomString(const QString &customString)
{
    m_customString = customString;
}


bool CVernier::isChoseNearer() const
{
    return m_isChoseNearer;
}

void CVernier::setIsChoseNearer(bool isChoseNearer)
{
    m_isChoseNearer = isChoseNearer;
}

/*!
\section
*/

double CVernier::moursePressX() const
{
    return m_moursePressX;
}

void CVernier::setMoursePressX(double moursePressX)
{
    m_moursePressX = moursePressX;
}
bool CVernier::isStudioMode() const
{
    return m_isStudioMode;
}

void CVernier::setIsStudioMode(bool isStudioMode)
{
    m_isStudioMode = isStudioMode;
}
QList<CCurveObject *> CVernier::list() const
{
    return m_list;
}

void CVernier::setList(const QList<CCurveObject *> &list)
{
    m_list = list;
}

QList<CReferenceCurve *> CVernier::referenceList() const
{
    return m_referenceList;
}

void CVernier::setReferenceList(const QList<CReferenceCurve *> &reList)
{
    m_referenceList = reList;
}
QList<CLabelItem *> CVernier::labelList() const
{
    return m_labelList;
}

void CVernier::setLabelList(const QList<CLabelItem *> &labelList)
{
    m_labelList = labelList;
}


/*!
\section calculate
*/
QSizeF CVernier::widgetSize() const
{
    return m_widgetSize;
}

void CVernier::setWidgetSize(const QSizeF &widgetSize)
{
    m_widgetSize = widgetSize;
}

void CVernier::calculate()
{
    if(!m_display || m_isStudioMode){
        m_isCalculated = false;
        return;
    }

    if(m_list.isEmpty() && m_referenceList.isEmpty())
    {
        return;
    }
    enum {Internal = 2};

    QFontMetrics metrics(m_font);
    double textHeight = metrics.height() + interval();
    m_widgetSize.setHeight(textHeight);
    m_widgetSize.setWidth(0);

    QPointF firstPoint(0,0);
    double valueWidth = metrics.width(QString("valueTest"));//变量值预留宽度
    double colorWidth = textHeight;
    // 清空链表,重新计算
    clearItemList();

    for(int i = 0; i < m_list.count() + m_referenceList.count();i++){
        if(i < m_list.count())
        {
            CCurveObject *curve = m_list.at(i);

            if(!curve->curveVisible() || curve->rawData().isEmpty())
                continue;

            if(!curve->curveVernierVisible())
                continue;
            QString showText;
            if(curve->comment().isEmpty())
                showText = curve->variantDepict();
            else
                showText = curve->comment();
            double textWidth = metrics.width(showText); // change to comment

            // 垂直排列
            if(m_isVertical){
                firstPoint.setX(0);
                firstPoint.setY(m_widgetSize.height());
                m_widgetSize.setHeight(m_widgetSize.height() + textHeight);
            }

            CLabelItem *item = new CLabelItem;
            item->setTopLeft(firstPoint);

            // 值检索
            CXAxis *xAxis = curve->xAxis();
            double value = 0;
            // 当前时间值
            QString timeString;

            //是否超时标识
            int timeFlag = INTIME;
            double xPoint = m_moursePressX - curve->curveArea().left();
            // X 轴值为double型
            if(CXAxis::ValueType::Value == xAxis->valueType())
                value = calculateDoubleValue(xPoint,curve);
            // X轴值为时间型
            else if(CXAxis::ValueType::Time == xAxis->valueType())
                value = calculateTimeValue(xPoint,curve,timeFlag,timeString);

            int valuePrecision = curve->yAxis()->valuePrecision();
            if(OUTOFTIME == timeFlag)
                item->setValueString(m_customString);
            else
                item->setValueString(QString::number(value,'f',valuePrecision));

            double timeWidth = metrics.width(timeString);

            item->setTimeString(timeString);
            item->setColor(curve->curveColor());
            item->setText(showText); // change to comment
            item->calculate(valueWidth,textWidth,textHeight,timeWidth);
            m_labelList.append(item);
            double itemWidth = timeWidth + colorWidth + textWidth + valueWidth + 3*Internal + m_interval;

            m_widgetSize.setWidth(m_widgetSize.width()+itemWidth);
            firstPoint.setX(firstPoint.x()+itemWidth);
        }
        else
        {
            CReferenceCurve *reCurve = m_referenceList.at(i - m_list.count());

            if(!reCurve->curveVisible() || reCurve->rawData().isEmpty())
                continue;

            if(!reCurve->reCurveVernierVisible())
                continue;

            double textWidth = metrics.width(reCurve->comment()); // change to comment

            // 垂直排列
            if(m_isVertical)
            {
                firstPoint.setX(0);
                firstPoint.setY(m_widgetSize.height());
                m_widgetSize.setHeight(m_widgetSize.height() + textHeight);
            }

            CLabelItem *item = new CLabelItem;
            item->setTopLeft(firstPoint);

            // 值检索
            CXAxis *xAxis = reCurve->xAxis();
            double value = 0;
            // 当前时间值
            QString timeString;

            //是否超时标识
            int timeFlag = INTIME;
            double xPoint = m_moursePressX - reCurve->curveArea().left();
            // X 轴值为double型
            if(CXAxis::ValueType::Value == xAxis->valueType())
            {
                value = calculateDoubleValue(xPoint,reCurve);

            }
            // X轴值为时间型
            else if(CXAxis::ValueType::Time == xAxis->valueType())
            {
                value = calculateTimeValue(xPoint,reCurve,timeFlag,timeString);
            }

            int valuePrecision = reCurve->yAxis()->valuePrecision();
            if(OUTOFTIME == timeFlag)
                item->setValueString(m_customString);
            else
                item->setValueString(QString::number(value,'f',valuePrecision));

            double timeWidth = metrics.width(timeString);

            item->setTimeString(timeString);
            item->setColor(reCurve->reCurveColor());
            item->setText(reCurve->comment()); // change to comment
            item->calculate(valueWidth,textWidth,textHeight,timeWidth);
            m_labelList.append(item);
            double itemWidth = timeWidth + colorWidth + textWidth + valueWidth + 3*Internal + m_interval;


            m_widgetSize.setWidth(m_widgetSize.width()+itemWidth);
            firstPoint.setX(firstPoint.x()+itemWidth);
        }
    }
    m_isCalculated = true;
}

//void CVernier::calculate()
//{
//    if(!m_display || m_list.isEmpty() || m_isStudioMode){
//        m_isCalculated = false;
//        return;
//    }
//    enum {Internal = 2};
//// error
//    QFontMetrics metrics(m_font);

//    double textHeight = metrics.height();
//    m_widgetSize.setHeight(textHeight);
//    m_widgetSize.setWidth(0);

//    QPointF firstPoint(0,0);
//    double valueWidth = metrics.width(QString("valueTest"));//变量值预留宽度
//    double colorWidth = textHeight;
//    // 清空链表,重新计算
//    clearItemList();

//    for(int i = 0; i < m_list.count();i++){
//        CCurveObject *curve = m_list.at(i);

//        if(!curve->curveVisible() || curve->rawData().isEmpty())
//            continue;
//        //!test
//        if(!curve->curveVernierVisible())
//            continue;

////        double textWidth = metrics.width(curve->variantDepict());
//        double textWidth = metrics.width(curve->comment()); // change to comment
////        double itemWidth = valueWidth+colorWidth+textWidth+textHeight+m_interval;
////        m_widgetSize.setWidth(m_widgetSize.width()+itemWidth);

//        // 垂直排列
//        if(m_isVertical){
//            firstPoint.setX(0);
//            firstPoint.setY(m_widgetSize.height());
//            m_widgetSize.setHeight(m_widgetSize.height() + textHeight);
//        }

//        CLabelItem *item = new CLabelItem;
//        item->setTopLeft(firstPoint);

//        // 值检索
//        CXAxis *xAxis = curve->xAxis();
//        double value = 0;
//        // 当前时间值
//        QString timeString;

//        //是否超时标识
//        int timeFlag = INTIME;
//        double xPoint = m_moursePressX - curve->curveArea().left();
//        // X 轴值为double型
//        if(CXAxis::ValueType::Value == xAxis->valueType())
//            value = calculateDoubleValue(xPoint,curve);
//        // X轴值为时间型
//        else if(CXAxis::ValueType::Time == xAxis->valueType())
//            value = calculateTimeValue(xPoint,curve,timeFlag,timeString);

//        if(OUTOFTIME == timeFlag)
//            item->setValueString(m_customString);
//        else
//            item->setValueString(QString::number(value,'f',2));

//        double timeWidth = metrics.width(timeString);

//        item->setTimeString(timeString);
//        item->setColor(curve->curveColor());
//        item->setText(curve->comment()); // change to comment
//        item->calculate(valueWidth,textWidth,textHeight,timeWidth);
//        m_labelList.append(item);
//        double itemWidth = timeWidth + colorWidth + textWidth + valueWidth + 3*Internal + m_interval;

//        m_widgetSize.setWidth(m_widgetSize.width()+itemWidth);
//        firstPoint.setX(firstPoint.x()+itemWidth);
//    }

//    m_isCalculated = true;
//}

double CVernier::calculateTimeValue(double xVisble, CCurveObject *curve, int &timeFlag, QString &timeValue)
{
    CXAxis *xAxis = curve->xAxis();
    QList<CTimeValuePoint> list = curve->rawData().timeList();

    if(list.isEmpty())
        return 0;

    QRectF    curveArea = curve->curveArea();
    double    xWidth = curveArea.width();
    QDateTime timeMark  = xAxis->timeMark();

    qint64    valueLength = xAxis->valueLength();
    QDateTime startTime = xAxis->endTime().addSecs(-valueLength);

    /*
        (xVisble - xWidth *(startTime.secsTo(timeMark))/valueLength) = tx;
        tx*valueLength /xWidth = timeConst;
        xTime = timeMark.addsecsTo(timeConst);
    */
    double tx = xVisble - xWidth*(startTime.secsTo(timeMark))/valueLength;
    qint64 timeConst = tx *valueLength/xWidth;
    QDateTime rawX = timeMark.addSecs(timeConst);
    timeValue = rawX.toString(m_timeFormat);

//    const int maxIndex = list.count()-1;
    int index = CFind::binarySearch(list,rawX);
    CTimeValuePoint point = list.at(index);
    if(rawX.secsTo(point.xTime()) >= m_timeLength && !m_isChoseNearer){
        timeFlag = OUTOFTIME;
        return 0;
    }else {
        return point.y();
    }
#if 0
    if(index == SMALLTHANFISRT)
    {

        if(rawX.secsTo(list.first().xTime()) >= m_timeLength && !m_isChoseNearer)
        {
            timeFlag = OUTOFTIME;
            return 0;
        } else {
            return list.first().y();
        }
    }
    else if(index == BIGTHANLAST)
    {
        if(rawX.secsTo(list.last().xTime()) >= m_timeLength && !m_isChoseNearer)
        {
            timeFlag = OUTOFTIME;
            return 0;
        } else {
            return list.last().y();
        }
    } else if(index == ERRORFINDING) {
        return list.first().y();
    } else if(index == maxIndex) {
        if(rawX.secsTo(list.last().xTime()) >= m_timeLength && !m_isChoseNearer)
        {
            timeFlag = OUTOFTIME;
            return 0;
        } else {
            return list.last().y();
        }
    } else {
        QDateTime xMin = list.at(index).xTime();
        QDateTime xMax = list.at(index+1).xTime();
        double yMin = list.at(index).y();
        double yMax = list.at(index+1).y();

        if(xMin.secsTo(rawX) >= m_timeLength && rawX.secsTo(xMax) >= m_timeLength && !m_isChoseNearer)
        {
            timeFlag = OUTOFTIME;
            return 0;
        }
        return (xMin.secsTo(rawX) - rawX.secsTo(xMax) > 0 ? yMin:yMax);
    }
#endif
}

double CVernier::calculateDoubleValue(double xVisble,CCurveObject *curve)
{
    CXAxis *xAxis = curve->xAxis();
    QList<CPureValuePoint> list = curve->rawData().valueList();
    if(list.isEmpty())
        return 0;

    QRectF curveArea = curve->curveArea();
    double xWidth = curveArea.width();
    double maxValue = xAxis->maxValue();
    double minValue = xAxis->minValue();

    double rawX = xVisble*(maxValue-minValue)/xWidth +minValue;
    int index = CFind::binarySearch(list,rawX);

    return index == -1 ? 0: list.at(index).y();

//        //Key小于第一个值
//        if(index == SMALLTHANFISRT)
//            return list.first().y();
//        //Key大于最后一个值，或Key等于最后一个值
//        else if(index == BIGTHANLAST || index == list.count()-1)
//            return list.last().y();
//        else if(index == ERRORFINDING)
//            return 0;
//        else {
//            double xMin = list.at(index).xDouble();
//            double xMax = list.at(index+1).xDouble();
//            double yMin = list.at(index).y();
//            double yMax = list.at(index+1).y();

//            return (rawX - xMin - (xMax - rawX) > 0 ? yMin : yMax);
//        }
//    }

}

double CVernier::calculateTimeValue(double xVisble,CReferenceCurve *reCurve, int &timeFlag, QString &timeValue)
{
    CXAxis *xAxis = reCurve->xAxis();
    QList<CTimeValuePoint> list = reCurve->rawData().timeList();

    if(list.isEmpty())
        return 0;

    QRectF    curveArea = reCurve->curveArea();
    double    xWidth = curveArea.width();
    QDateTime timeMark  = xAxis->timeMark();

    qint64    valueLength = xAxis->valueLength();
    QDateTime startTime = xAxis->endTime().addSecs(-valueLength);


    double tx = xVisble - xWidth*(startTime.secsTo(timeMark))/valueLength;
    qint64 timeConst = tx *valueLength/xWidth;
    QDateTime rawX = timeMark.addSecs(timeConst);
    timeValue = rawX.toString(m_timeFormat);

    int index = CFind::binarySearch(list,rawX);
    CTimeValuePoint point = list.at(index);
    if(rawX.secsTo(point.xTime()) >= m_timeLength && !m_isChoseNearer){
        timeFlag = OUTOFTIME;
        return 0;
    }else {
        return point.y();
    }
}

double CVernier::calculateDoubleValue(double xVisble, CReferenceCurve *reCurve)
{
    CXAxis *xAxis = reCurve->xAxis();
    QList<CPureValuePoint> list = reCurve->rawData().valueList();
    if(list.isEmpty())
        return 0;

    QRectF curveArea = reCurve->curveArea();
    double xWidth = curveArea.width();
    double maxValue = xAxis->maxValue();
    double minValue = xAxis->minValue();

    double rawX = xVisble*(maxValue-minValue)/xWidth +minValue;
    int index = CFind::binarySearch(list,rawX);

    return index == -1 ? 0: list.at(index).y();

}

void CVernier::clearItemList()
{
    if(!m_labelList.isEmpty())
        qDeleteAll(m_labelList.begin(),m_labelList.end());
    m_labelList.clear();
}

/*!
\section
*/

void CVernier::serialize(QJsonObject &json)
{
    json.insert("Display",m_display);
    json.insert("TimeLength",m_timeLength);
    json.insert("WidgetSizeWidth",m_widgetSize.width());
    json.insert("WidgetSizeHeight",m_widgetSize.height());
    json.insert("Font",CMDJsonConvert::toJson(m_font));
    json.insert("TimeFormat",m_timeFormat);
    json.insert("IsFixed",m_isFixed);
    json.insert("FixedPoint",CMDJsonConvert::toJson(m_fixedPoint));
    json.insert("Interval",m_interval);
    json.insert("IsVertical",m_isVertical);
    json.insert("IsChoseNearer",m_isChoseNearer);
    json.insert("CustomString",m_customString);

}

void CVernier::deserialize(const QJsonObject &json)
{
    m_display = json.value("Display").toBool();
    m_timeLength = json.value("TimeLength").toInt();
    int width = json.value("WidgetSizeWidth").toDouble();
    int height = json.value("WidgetSizeHeight").toDouble();
    m_widgetSize.setWidth(width);
    m_widgetSize.setHeight(height);
    m_font = CMDJsonConvert::toQFont(json.value("Font"));
    m_timeFormat = json.value("TimeFormat").toString();
    m_isFixed = json.value("IsFixed").toBool();
    m_fixedPoint = CMDJsonConvert::toQPoint(json.value("FixedPoint"));
    m_interval = json.value("Interval").toInt();
    m_isVertical = json.value("IsVertical").toBool();
    m_isChoseNearer = json.value("IsChoseNearer").toBool();
    m_customString = json.value("CustomString").toString();
}

CVernier &CVernier::operator =(const CVernier &other)
{
    if(&other == this)
        return *this;

    this->m_display = other.m_display;
    this->m_timeLength = other.m_timeLength;
    this->m_widgetSize = other.m_widgetSize;
    this->m_font = other.m_font;
    this->m_timeFormat = other.m_timeFormat;
    this->m_isFixed = other.m_isFixed;
    this->m_fixedPoint = other.m_fixedPoint;
    this->m_interval = other.m_interval;
    this->m_isVertical = other.m_isVertical;
    this->m_isChoseNearer = other.m_isChoseNearer;
    this->m_customString = other.m_customString;

    return *this;
}
