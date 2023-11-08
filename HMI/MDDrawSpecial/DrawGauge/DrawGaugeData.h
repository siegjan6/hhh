#ifndef DRAWGGAUGEDATA_H
#define DRAWGGAUGEDATA_H

#include "MDGauge.h"

//报表控件,数据类
class CDrawGaugeData
{
public:
    CDrawGaugeData();
    ~CDrawGaugeData();
public:
    CMDGauge m_gauge;
//////////////////////////////////////////////////////////////////////////
//common
public:
    void operator=(const CDrawGaugeData& other);
public:
    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);
};

#endif // DRAWGGAUGEDATA_H
