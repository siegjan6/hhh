#include "DrawProcDockerData.h"

/*!
\section 拷贝进程主窗体停靠控件
*/
CDrawProcDockerData::CDrawProcDockerData()
{
}

CDrawProcDockerData::~CDrawProcDockerData()
{

}

CDrawProcDockerData& CDrawProcDockerData::operator=(const CDrawProcDockerData &other)
{
    if(this == &other)
    {
        return *this;
    }

    this->m_cmd = other.m_cmd;

    return *this;
}


void CDrawProcDockerData::serialize(QJsonObject &json)
{
    json["cmd"] = m_cmd;
}

void CDrawProcDockerData::deserialize(const QJsonObject &json)
{
    m_cmd = json["cmd"].toString();
}
