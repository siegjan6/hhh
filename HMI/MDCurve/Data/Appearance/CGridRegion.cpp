#include "CGridRegion.h"
#include <QDebug>
#include <QJsonObject>

#define MINVALUE 1
#define MAXVALUE 50

CGridRegion::CGridRegion():
    m_xMainBisectrixWidth(0),
    m_xSecondaryBisectrixWidth(0),
    m_xMainBisectrixNum(3),
    m_xSecondaryBisectrixNum(1),
    m_xMainBisectrixLine(new CBisectrixLine),
    m_xSecondaryBisectrixLine(new CBisectrixLine),
    m_yMainBisectrixHeight(0),
    m_ySecondaryBisectrixHeight(0),
    m_yMainBisectrixNum(3),
    m_ySecondaryBisectrixNum(1),
    m_yMainBisectrixLine(new CBisectrixLine),
    m_ySecondaryBisectrixLine(new CBisectrixLine)
{
}
CGridRegion::~CGridRegion()
{
    if(m_xMainBisectrixLine)
    {
        delete m_xMainBisectrixLine;
        m_xMainBisectrixLine = NULL;
    }
    if(m_xSecondaryBisectrixLine)
    {
        delete m_xSecondaryBisectrixLine;
        m_xSecondaryBisectrixLine = NULL;
    }
    if(m_yMainBisectrixLine)
    {
        delete m_yMainBisectrixLine;
        m_yMainBisectrixLine = NULL;
    }
    if(m_ySecondaryBisectrixLine)
    {
        delete m_ySecondaryBisectrixLine;
        m_ySecondaryBisectrixLine = NULL;
    }
}

QColor CGridRegion::borderColor() const
{
    return m_borderColor;
}
void CGridRegion::setBorderColor(const QColor &borderColor)
{
    m_borderColor = borderColor;
}


QRectF CGridRegion::gridRegionArea() const
{
    return m_gridRegionArea;
}

void CGridRegion::setGridRegionArea(const QRectF &gridRegionArea)
{
    m_gridRegionArea = gridRegionArea;
}

/*!
\section x
*/
int CGridRegion::xMainBisectrixNum() const
{
    return m_xMainBisectrixNum;
}

void CGridRegion::setXMainBisectrixNum(int xMainBisectrixNum)
{
    if(xMainBisectrixNum < MINVALUE )
        m_xMainBisectrixNum = MINVALUE;
    else if(xMainBisectrixNum > MAXVALUE)
        m_xMainBisectrixNum = MAXVALUE;
    else
        m_xMainBisectrixNum = xMainBisectrixNum;
}
int CGridRegion::xSecondaryBisectrixNum() const
{
    return m_xSecondaryBisectrixNum;
}

void CGridRegion::setXSecondaryBisectrixNum(int xSubBisectrixNum)
{
    if(xSubBisectrixNum < MINVALUE )
        m_xSecondaryBisectrixNum = MINVALUE;
    else if(xSubBisectrixNum > MAXVALUE)
        m_xSecondaryBisectrixNum = MAXVALUE;
    else
        m_xSecondaryBisectrixNum = xSubBisectrixNum;
}

CBisectrixLine *CGridRegion::xMainBisectrixLine()
{
    return m_xMainBisectrixLine;
}
CBisectrixLine *CGridRegion::xSecondaryBisectrixLine()
{
    return m_xSecondaryBisectrixLine;
}

/*!
\section Y
*/
int CGridRegion::yMainBisectrixNum() const
{
    return m_yMainBisectrixNum;
}

void CGridRegion::setYMainBisectrixNum(int yMainBisectrixNum)
{
    if(yMainBisectrixNum < MINVALUE )
        m_yMainBisectrixNum = MINVALUE;
    else if(yMainBisectrixNum > MAXVALUE)
        m_yMainBisectrixNum = MAXVALUE;
    else
        m_yMainBisectrixNum = yMainBisectrixNum;
}

int CGridRegion::ySecondaryBisectrixNum() const
{
    return m_ySecondaryBisectrixNum;
}

void CGridRegion::setYSecondaryBisectrixNum(int ySubBisectrixNum)
{
    if(ySubBisectrixNum < MINVALUE )
        m_ySecondaryBisectrixNum = MINVALUE;
    else if(ySubBisectrixNum > MAXVALUE)
        m_ySecondaryBisectrixNum = MAXVALUE;
    else
        m_ySecondaryBisectrixNum = ySubBisectrixNum;
}

CBisectrixLine *CGridRegion::yMainBisectrixLine()
{
    return m_yMainBisectrixLine;
}
CBisectrixLine *CGridRegion::ySecondaryBisectrixLine()
{
    return m_ySecondaryBisectrixLine;
}

/*!
\section calculate ,draw
*/
void CGridRegion::draw(QPainter *p)
{
    p->save();
    // 绘制边框
    p->setPen(m_borderColor);
    p->drawRect(m_gridRegionArea);

    p->translate(m_gridRegionArea.bottomLeft());

    drawXBisectrix(p);// 绘制x方向等分线
    drawYBisectrix(p);// 绘制Y方向等分线
    p->restore();
}

void CGridRegion::calculate()
{
    // 垂直方向 等分线间距
    Q_ASSERT(m_xMainBisectrixNum >0 && m_xSecondaryBisectrixNum > 0);
    m_xMainBisectrixWidth = m_gridRegionArea.width() / m_xMainBisectrixNum;
    m_xSecondaryBisectrixWidth = m_xMainBisectrixWidth / m_xSecondaryBisectrixNum;

    // 水平方向
    Q_ASSERT(m_yMainBisectrixNum >0 && m_ySecondaryBisectrixNum >0);
    m_yMainBisectrixHeight = m_gridRegionArea.height() / m_yMainBisectrixNum;
    m_ySecondaryBisectrixHeight = m_yMainBisectrixHeight / m_ySecondaryBisectrixNum;
}

void CGridRegion::drawXBisectrix(QPainter *p)
{

    double yHeight = m_gridRegionArea.height();

    // 主等分线
    QPen xMain;
    xMain.setColor(m_xMainBisectrixLine->lineColor());
    xMain.setWidth(m_xMainBisectrixLine->lineWidth());
    xMain.setStyle(m_xMainBisectrixLine->lineStyle());
    p->setPen(xMain);

    for(int i = 1; i < m_xMainBisectrixNum;i++)
    {
        p->drawLine(m_xMainBisectrixWidth *i,0,m_xMainBisectrixWidth *i, -yHeight);
    }

    // 次等分线
    QPen subDiverPen;
    subDiverPen.setColor(m_xSecondaryBisectrixLine->lineColor());
    subDiverPen.setWidth(m_xSecondaryBisectrixLine->lineWidth());
    subDiverPen.setStyle(m_xSecondaryBisectrixLine->lineStyle());
    p->setPen(subDiverPen);
    if(m_xSecondaryBisectrixNum == 1)
        return;

    for(int i = 1; i<= (m_xSecondaryBisectrixNum * m_xMainBisectrixNum);i++){
        if(!(i % m_xSecondaryBisectrixNum))
            continue;
        p->drawLine(m_xSecondaryBisectrixWidth * i, 0, m_xSecondaryBisectrixWidth * i, -yHeight);
    }
}

void CGridRegion::drawYBisectrix(QPainter *p)
{

    double xWidth = m_gridRegionArea.width();

    // 绘制主等分线
    QPen mainDiverPen;
    mainDiverPen.setColor(m_yMainBisectrixLine->lineColor());
    mainDiverPen.setWidth(m_yMainBisectrixLine->lineWidth());
    mainDiverPen.setStyle(m_yMainBisectrixLine->lineStyle());
    p->setPen(mainDiverPen);

    for (int i = 1; i < m_yMainBisectrixNum; ++i)
        p->drawLine(0, -i * m_yMainBisectrixHeight, xWidth, -i * m_yMainBisectrixHeight);

    // 绘制次等分线
    QPen subDiverPen;
    subDiverPen.setColor(m_ySecondaryBisectrixLine->lineColor());
    subDiverPen.setWidth(m_ySecondaryBisectrixLine->lineWidth());
    subDiverPen.setStyle(m_ySecondaryBisectrixLine->lineStyle());
    p->setPen(subDiverPen);
    if(m_ySecondaryBisectrixNum == 1)
        return;
    for(int i = 1; i<= (m_ySecondaryBisectrixNum * m_yMainBisectrixNum);i++){
        if(i % m_ySecondaryBisectrixNum == 0)
            continue;
        p->drawLine(0, -i * m_ySecondaryBisectrixHeight, xWidth, -i * m_ySecondaryBisectrixHeight);
    }
}

CGridRegion &CGridRegion::operator =(const CGridRegion &other)
{
    if(&other == this)
        return *this;

    this->m_gridRegionArea = other.m_gridRegionArea;
    this->m_borderColor = other.m_borderColor;
    this->m_xMainBisectrixWidth = other.m_xMainBisectrixWidth;
    this->m_xSecondaryBisectrixWidth = other.m_xSecondaryBisectrixWidth;
    this->m_xMainBisectrixNum = other.m_xMainBisectrixNum;
    *this->m_xMainBisectrixLine = *other.m_xMainBisectrixLine;
    this->m_xSecondaryBisectrixNum = this->m_xSecondaryBisectrixNum;
    *this->m_xSecondaryBisectrixLine = *this->m_xSecondaryBisectrixLine;

    this->m_yMainBisectrixHeight = other.m_yMainBisectrixHeight;
    this->m_ySecondaryBisectrixHeight = other.m_ySecondaryBisectrixHeight;
    this->m_yMainBisectrixNum = other.m_yMainBisectrixNum;
    *this->m_yMainBisectrixLine = *other.m_yMainBisectrixLine;
    this->m_ySecondaryBisectrixNum = other.m_ySecondaryBisectrixNum;
    *this->m_ySecondaryBisectrixLine = *other.m_ySecondaryBisectrixLine;

    return *this;
}
/*!
\section serialize
*/

void CGridRegion::serialize(QJsonObject &json)
{
    QJsonObject xMainJson;
    m_xMainBisectrixLine->serialize(xMainJson);
    json.insert("XMainBisectrixLine",xMainJson);
    QJsonObject xSecondJson;
    m_xSecondaryBisectrixLine->serialize(xSecondJson);
    json.insert("XSecondaryBisectrixLine",xSecondJson);
    QJsonObject yMainJson;
    m_yMainBisectrixLine->serialize(yMainJson);
    json.insert("YMainBisectrixLine",yMainJson);
    QJsonObject ySecondJson;
    m_ySecondaryBisectrixLine->serialize(ySecondJson);
    json.insert("YSecondaryBisectrixLine",ySecondJson);

    json.insert("BorderColor",(double)m_borderColor.rgba());
    json.insert("XMainBisectrixWidth",m_xMainBisectrixWidth);
    json.insert("XSecondBisectrixWidth",m_xSecondaryBisectrixWidth);
    json.insert("XMainBisectrixNum",m_xMainBisectrixNum);
    json.insert("XSecondaryBisectrixNum",m_xSecondaryBisectrixNum);
    json.insert("YMainBisectrixHeight",m_yMainBisectrixHeight);
    json.insert("YSecondaryBisectrixHeight",m_ySecondaryBisectrixHeight);
    json.insert("YSecondaryBisectrixNum",m_ySecondaryBisectrixNum);
    json.insert("YMainBisectrixNum",m_yMainBisectrixNum);
}

void CGridRegion::deserialize(const QJsonObject &json)
{
    QJsonObject xMainJson = json.value("XMainBisectrixLine").toObject();
    m_xMainBisectrixLine->deserialize(xMainJson);
    QJsonObject xSecondJson = json.value("XSecondaryBisectrixLine").toObject();
    m_xSecondaryBisectrixLine->deserialize(xSecondJson);
    QJsonObject yMainJson = json.value("YMainBisectrixLine").toObject();
    m_yMainBisectrixLine->deserialize(yMainJson);
    QJsonObject ySecondJson = json.value("YSecondaryBisectrixLine").toObject();
    m_ySecondaryBisectrixLine->deserialize(ySecondJson);

    m_borderColor = QColor::fromRgba(json.value("BorderColor").toDouble());
    m_xMainBisectrixWidth = json.value("XMainBisectrixWidth").toDouble();
    m_xSecondaryBisectrixWidth = json.value("XSecondBisectrixWidth").toDouble();
    m_xMainBisectrixNum = json.value("XMainBisectrixNum").toInt();
    m_xSecondaryBisectrixNum = json.value("XSecondaryBisectrixNum").toInt();
    m_yMainBisectrixHeight = json.value("YMainBisectrixHeight").toDouble();
    m_ySecondaryBisectrixHeight = json.value("YSecondaryBisectrixHeight").toDouble();
    m_ySecondaryBisectrixNum = json.value("YSecondaryBisectrixNum").toInt();
    m_yMainBisectrixNum  =json.value("YMainBisectrixNum").toInt();

}
