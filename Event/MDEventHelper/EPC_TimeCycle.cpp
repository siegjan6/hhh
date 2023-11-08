#include "EPC_TimeCycle.h"
#include "MDEventPropertyData.h"

// 周期
CEPC_TimeCycle::CEPC_TimeCycle()
    : CMDEventProperty_Config(MDEventType::timer_Cycle)
{
    m_data1 = new CEPCData_TimeCycle;
}

CEPC_TimeCycle::~CEPC_TimeCycle()
{
    if(m_data1)
        delete m_data1;
}

CEPC_TimeCycle::CEPC_TimeCycle(const CEPC_TimeCycle& obj)
    : CMDEventProperty_Config(obj)
{
    m_data1 = new CEPCData_TimeCycle;

    copy(&obj); // 基类属性被重复拷贝1次
}

CEPC_TimeCycle& CEPC_TimeCycle::operator = (const CEPC_TimeCycle& obj)
{
    if( this != &obj )
    {
        copy(&obj);
    }
    return *this;
}

/*!
\section 属性
*/
quint32 CEPC_TimeCycle::timeCycle() const
{
    return (m_data1 ? m_data1->m_timeCycle : 1000);
}
void CEPC_TimeCycle::setTimeCycle(quint32 timeCycle)
{
    if(m_data1)
        m_data1->m_timeCycle = timeCycle;
}

QString CEPC_TimeCycle::condition() const
{
    quint32 minutes;
    quint32 seconds;
    quint32 millSeconds;
    to_M_S_Mill(minutes, seconds, millSeconds);
    QString str;
    if(minutes > 0)
        str = QString("%1 minute").arg(minutes);
    if(seconds > 0)
        str = str + QString("%1 sec").arg(seconds);
    if(millSeconds > 0)
        str = str + QString("%1 msec").arg(millSeconds);

    return str;
}

bool CEPC_TimeCycle::copy(const CMDEventProperty_Config* source)
{
    CMDEventProperty_Config::copy(source);

    return (m_data1 ? m_data1->copy(((CEPC_TimeCycle*)source)->m_data1) : false);
}

const quint32 _1S = 1000;
const quint32 _1M = 60*1000;
void CEPC_TimeCycle::to_M_S_Mill(quint32& minutes, quint32& seconds, quint32& millSeconds) const
{
    minutes	= timeCycle()/_1M;
    quint32 leftMill = timeCycle() - minutes * _1M;
    seconds = leftMill/_1S;
    millSeconds = leftMill - seconds * _1S;
}

void CEPC_TimeCycle::setTimeCycle(quint32 minutes, quint32 seconds, quint32 millSeconds)
{
    quint32 cycle = minutes *_1M +  seconds *_1S + millSeconds;
    setTimeCycle(cycle);
}

/*!
\section 序列化
*/
bool CEPC_TimeCycle::serialize(QDataStream& stream, bool useCurrentVersion)
{
    if(!CMDEventProperty_Config::serialize(stream, useCurrentVersion))
        return false;

    return (m_data1 ? m_data1->serialize(stream, useCurrentVersion) : false);
}

void CEPC_TimeCycle::deserialize(QDataStream& stream)
{
    CMDEventProperty_Config::deserialize(stream);

    if(m_data1)
        m_data1->deserialize(stream);
}

/*!
\section 序列化：JSON格式
*/
void CEPC_TimeCycle::serialize(QJsonObject& json)
{
    CMDEventProperty_Config::serialize(json);

    if(m_data1)
        m_data1->serialize(json);
}

void CEPC_TimeCycle::deserialize(const QJsonObject& json)
{
    CMDEventProperty_Config::deserialize(json);

    if(m_data1)
        m_data1->deserialize(json);
}
