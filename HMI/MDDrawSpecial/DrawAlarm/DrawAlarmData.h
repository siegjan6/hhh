/*!
\author Y2hhaWhq
\section 报警控件数据类
*/
#ifndef DRAWALARMDATA_H
#define DRAWALARMDATA_H

#include <QJsonObject>
#include "MDAlarmGrid.h"

/*!
\section  报警控件，数据类
*/
class CDrawAlarmData
{
public:
    CDrawAlarmData();
    ~CDrawAlarmData(){}
public:
    CMDAlarmGrid m_alarm;

/*!
\section  序列化
*/
public:
    void operator = (/*const */CDrawAlarmData& other);


    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);
};

#endif // DRAWALARMDATA_H
