/*!

\author dGFuZ3p3

\section 功能

报警参数数据的实现类。

*/

#ifndef MDALARMPARAIMP_H
#define MDALARMPARAIMP_H

#include <QString>
#include "MDAlarmConstant.h"
#include <QJsonObject>
#include <QDataStream>

class CMDAlarmParaImp
{
public:
    CMDAlarmParaImp();
     virtual ~CMDAlarmParaImp();
    CMDAlarmParaImp(const CMDAlarmParaImp& obj);
    CMDAlarmParaImp& operator = (const CMDAlarmParaImp& obj);
/*!
\section 属性
*/
public:
    MDAlarmMode m_mode ;// MDAlarmMode::common;
public:
    bool copy(const CMDAlarmParaImp* source);

/*!
\section 序列化
*/
public:
    bool serialize(QDataStream& stream, bool useCurrentVersion = true);
    void serializeOnVersion_1(QDataStream& stream);
public:
    void deserialize(QDataStream& stream);
    void deserializeOnVersion_1(QDataStream& stream);
/*!
\section 序列化：JSON格式
*/
public:
    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);
public:
    quint8 m_version ;// 0;
};

#endif // MDALARMPARAIMP_H
