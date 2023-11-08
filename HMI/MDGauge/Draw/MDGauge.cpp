#include "MDGauge.h"
#include "DrawGauge.h"
#include "DesignDialog.h"

CMDGauge::CMDGauge()
{
    m_gauge = NULL;
    m_gauge = new CDrawGauge;
}

CMDGauge::~CMDGauge()
{
    if(m_gauge)
    {
        delete m_gauge;
        m_gauge = NULL;
    }
}

CMDGauge &CMDGauge::operator =(const CMDGauge &other)
{
    if(this == &other)
    {
        return *this;
    }
    m_gauge = other.m_gauge->clone();
    return *this;
}

void CMDGauge::setDrawEnvrionment(IDrawEnvrionment *value)
{
    m_gauge->setDrawEnvrionment(value);
}

void CMDGauge::setParent(IHMIForm *parent)
{
    m_gauge->setParent(parent);
}

void CMDGauge::serialize(QJsonObject &json)
{
    m_gauge->serialize(json);
}

void CMDGauge::deserialize(const QJsonObject &json)
{
    m_gauge->deserialize(json);
}

void CMDGauge::setRect(const QRect &value)
{
    CDrawGauge *drawGauge = (CDrawGauge*)m_gauge;
    if(drawGauge)
    {
        drawGauge->setClientRect(value);
    }
}

void CMDGauge::paint(QPainter *p)
{
    CDrawGauge *drawGauge = (CDrawGauge*)m_gauge;
    if(drawGauge)
    {
        drawGauge->draw(p);
    }
}

bool CMDGauge::openDesign()
{
    return m_gauge->openDesign();
}

void CMDGauge::setValue(qint32 index, qreal value)
{
    m_gauge->setValue(index, value);
}
void CMDGauge::setInitialIndex(qint32 value)
{
    m_gauge->setInitialIndex(value);
}
void CMDGauge::getExpressionList(QList<qint32> &indexs, QList<QString> &expressions)
{
    m_gauge->getExpressionList(indexs, expressions);
}

void CMDGauge::onDoubleClick()
{
    m_gauge->openDesign();
}

void CMDGauge::changeLanguage()
{
    m_gauge->changeLanguage();
}

void CMDGauge::collectWords()
{
    m_gauge->collectWords();
}
