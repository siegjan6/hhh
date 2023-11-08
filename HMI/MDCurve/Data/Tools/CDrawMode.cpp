#include "CDrawMode.h"
#include <QTimer>
#include <QJsonObject>

CDrawMode::CDrawMode():
    m_orietation(Orientation::RigthToLeft),
    m_freshTime(1000),
    m_timer(new QTimer(this))
{    
    connect(m_timer,SIGNAL(timeout()),this,SIGNAL(outOfTime()));
}

CDrawMode::~CDrawMode()
{

}
CDrawMode::Orientation CDrawMode::orietation() const
{
    return m_orietation;
}

void CDrawMode::setOrietation(const Orientation &orietation)
{
    m_orietation = orietation;
}

qint32 CDrawMode::freshTime() const
{
    return m_freshTime;
}

void CDrawMode::setFreshTime(const qint32 &freshTime)
{
    if(freshTime < 1000)
        m_freshTime = 1000;
    else
        m_freshTime = freshTime;
}

void CDrawMode::start()
{
    m_timer->start(m_freshTime);
}

void CDrawMode::stop()
{
    m_timer->stop();
}

CDrawMode *CDrawMode::clone()
{
    CDrawMode *drawMode = new CDrawMode;
    drawMode->m_freshTime = this->m_freshTime;
    drawMode->m_orietation = this->m_orietation;

    return drawMode;
}

void CDrawMode::serialize(QJsonObject &json)
{
    json.insert("Orietation",(int)m_orietation);
    json.insert("FreshTime",m_freshTime);
}

void CDrawMode::deserialize(const QJsonObject &json)
{
    int orietation = json.value("Orietation").toInt();
    m_orietation = (Orientation)orietation;
    m_freshTime = json.value("FreshTime").toInt();
    if(m_freshTime < 1000)
        m_freshTime = 1000;
}


