#include "CLabelManager.h"
#include "CXAxis.h"
#include "CYAxis.h"
#include "CFind.h"
#include "MDJsonConvert.h"
#include <QJsonObject>
#include <QDebug>


CLabelManager::CLabelManager():
    m_selectedColor(Qt::red),
    m_lableVisible(false),
    m_movingValueVisible(true),
    m_isStudioMode(false),
    m_moursePressX(0),
    m_isCalculated(false)
{
}
CLabelManager::~CLabelManager()
{
}

double CLabelManager::height() const
{
    return m_height;
}

void CLabelManager::setHeight(double height)
{
    m_height = height;
}
QFont CLabelManager::font() const
{
    return m_font;
}

void CLabelManager::setFont(const QFont &font)
{
    m_font = font;
}

QColor CLabelManager::selectedColor() const
{
    return m_selectedColor;
}

void CLabelManager::setSelectedColor(const QColor &selectedColor)
{
    m_selectedColor = selectedColor;
}

bool CLabelManager::lableVisible() const
{
    return m_lableVisible;
}

void CLabelManager::setLableVisible(bool lableVisible)
{
    m_lableVisible = lableVisible;
}

bool CLabelManager::movingValueVisible() const
{
    return m_movingValueVisible;
}

void CLabelManager::setMovingValueVisible(bool movingValueVisible)
{
    m_movingValueVisible = movingValueVisible;
}

void CLabelManager::addCurve(CCurveObject *curve)
{
    m_curves.append(curve);
}

void CLabelManager::removeCurve(CCurveObject *curve)
{
//    m_curves.remove(curve);
}

QList<CCurveObject *> CLabelManager::curves() const
{
    return m_curves;
}

void CLabelManager::setCurves(const QList<CCurveObject *> &curves)
{
    m_curves = curves;
}

void CLabelManager::addReCurve(CReferenceCurve *reCurves)
{
    m_reCurves.append(reCurves);
}

QList<CReferenceCurve *> CLabelManager::reCurves() const
{
    return m_reCurves;
}

void CLabelManager::setReCurves(const QList<CReferenceCurve *> &reCurves)
{
    m_reCurves = reCurves;
}

/*!
\section
*/
bool CLabelManager::isStudioMode() const
{
    return m_isStudioMode;
}

void CLabelManager::setIsStudioMode(bool isStudioMode)
{
    m_isStudioMode = isStudioMode;
}

/*!
\section
*/
QRectF CLabelManager::drawArea() const
{
    return m_drawArea;
}

void CLabelManager::setDrawArea(const QRectF &drawArea)
{
    m_drawArea = drawArea;
    m_drawArea.setHeight(m_height);
}
double CLabelManager::moursePressX() const
{
    return m_moursePressX;
}

void CLabelManager::setMoursePressX(double moursePressX)
{
    m_moursePressX = moursePressX;
}

/*!
\section
*/
// when there are lots of items, the appearance would go wrong
void CLabelManager::calculate()
{
    if(!m_lableVisible || m_curves.isEmpty() || m_drawArea.isNull()) {
        m_isCalculated = false;
        m_height = 0;
        return;
    }

    QFontMetricsF metrics(m_font);
    double textHeight = metrics.ascent() + metrics.descent();
    m_height = textHeight;

    QRectF area = m_drawArea;
    QPointF firstPoint = area.topLeft();
    double valueWidth = metrics.width(QString("valueTest"));//变量值预留宽度
    double colorWidth = textHeight;

    // 清空链表,重新计算
    m_itemList.clear();

    for(int i = 0;i <m_curves.count();i++){
        CCurveObject *curve = m_curves.at(i);
        if(!curve) // curve maybe removed
            continue;
        QString text;
        if(curve->comment().isEmpty())
            text = curve->variantDepict();
        else
            text = curve->comment();

        if(!curve->curveVisible())
            continue;
        double textWidth = metrics.width(text);
        double areaWidth = valueWidth+colorWidth+textWidth+textHeight+2;

        if(firstPoint.x() + areaWidth > area.width()) {
            firstPoint.setX(area.x());
            firstPoint.setY(firstPoint.y()+textHeight);
            m_height += textHeight;
        }

        CLabelItem item;
        item.setTopLeft(firstPoint);
        item.calculate(valueWidth,textWidth,textHeight);
        firstPoint.setX(firstPoint.x()+areaWidth);

        if(m_isStudioMode)
        {
            item.setValueString("???");
        }
        else
        {
            // 值检索
            CXAxis *xAxis = curve->xAxis();
            double value = 0;
            int valuePrecision = curve->yAxis()->valuePrecision();

            if(CXAxis::ValueType::Time == xAxis->valueType())
                value = calculateTimeValue(curve);
            else if(CXAxis::ValueType::Value == xAxis->valueType())
                value = calculateDoubleValue(curve);

            item.setValueString(QString::number(value,'f',valuePrecision));
        }
        item.setColor(curve->curveColor());
        if(curve->comment().isEmpty())
            item.setText(curve->variantDepict());
        else
            item.setText(curve->comment());

        m_itemList.append(item);
    }
    m_isCalculated = true;
}

void CLabelManager::draw(QPainter *p)
{
    if(!m_isCalculated)
        return;
    p->save();

    p->setFont(m_font);
    for(int i = 0; i < m_itemList.count();i++)
        m_itemList[i].draw(p);

    p->restore();
}

double CLabelManager::calculateTimeValue(CCurveObject *curve)
{
    if(!curve->rawData().timeList().isEmpty()){
        QList<CTimeValuePoint> list = curve->rawData().timeList();

        if(!m_movingValueVisible) {
            return list.last().y();
        } else {
            double xPoint = m_moursePressX -curve->curveArea().left();
            CXAxis *xAxis = curve->xAxis();

            QRectF    curveArea = curve->curveArea();
            double    xWidth = curveArea.width();
            QDateTime timeMark  = xAxis->timeMark();
            QDateTime endTime = xAxis->endTime();
            qint64    valueLength = xAxis->valueLength();

            QDateTime rawX =timeMark.addSecs(xPoint/xWidth *valueLength -
                                             endTime.addSecs(-valueLength).secsTo(timeMark));

            int index = CFind::binarySearch(list,rawX);
            return list.at(index).y();
        }
    }
    return 0;
}

double CLabelManager::calculateDoubleValue(CCurveObject *curve)
{
    if(!curve->rawData().valueList().isEmpty()){
        QList<CPureValuePoint> list =curve->rawData().valueList();
        if(!m_movingValueVisible){
            return list.last().y();
        } else {
            double xPoint = m_moursePressX -curve->curveArea().left();
            CXAxis *xAxis = curve->xAxis();

            QRectF curveArea = curve->curveArea();
            double xWidth = curveArea.width();
            double maxValue = xAxis->maxValue();
            double minValue = xAxis->minValue();

            double rawX = xPoint*(maxValue-minValue)/xWidth +minValue;
            int index = CFind::binarySearch(list,rawX);
            return list.at(index).y();
        }
    }
    return 0;
}

/*!
\section
*/
void CLabelManager::serialize(QJsonObject &json)
{
    QJsonObject LabelJson;
    LabelJson.insert("DrawArea",CMDJsonConvert::toJson(m_drawArea));
    LabelJson.insert("Height",m_height);
    LabelJson.insert("Font",CMDJsonConvert::toJson(m_font));
    LabelJson.insert("SelectedColor",(double)m_selectedColor.rgba());
    LabelJson.insert("LabelVisible",m_lableVisible);
    LabelJson.insert("MovingValueVisible",m_movingValueVisible);
    json.insert("LabelManager",LabelJson);
}

void CLabelManager::deserialize(const QJsonObject &json)
{
    QJsonObject LabelJson = json.value("LabelManager").toObject();
    m_drawArea = CMDJsonConvert::toQRectF(LabelJson.value("DrawArea"));
    m_height = LabelJson.value("Height").toDouble();
    m_font =CMDJsonConvert::toQFont(LabelJson.value("Font"));
    m_selectedColor = QColor::fromRgba(LabelJson.value("selectedColor").toDouble());
    m_lableVisible = LabelJson.value("LabelVisible").toBool();
    m_movingValueVisible = LabelJson.value("MovingValueVisible").toBool();

}

CLabelManager &CLabelManager::operator =(const CLabelManager &other)
{
    if(&other == this)
        return *this;
    this->m_drawArea = other.m_drawArea;
    this->m_height = other.m_height;
    this->m_font = other.m_font;
    this->m_selectedColor = other.m_selectedColor;
    this->m_lableVisible = other.m_lableVisible;
    this->m_movingValueVisible = other.m_movingValueVisible;

    return *this;
}

