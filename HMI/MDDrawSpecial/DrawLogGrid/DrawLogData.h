/*!
\author Y2hhaWhq
\section 日志控件数据类
*/
#ifndef DRAWLOGDATA_H
#define DRAWLOGDATA_H

#include <QJsonObject>
#include "MDLogGrid.h"

/*!
\section  日志控件，数据类
*/
class CDrawLogData
{
public:
    CDrawLogData();
    ~CDrawLogData(){}

public:
    CMDLogGrid m_log;

/*!
\section  序列化
*/
public:
    CDrawLogData &operator =(const CDrawLogData& other);

    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);
};

#endif // DRAWLOGDATA_H

