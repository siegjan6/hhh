/*!

\author dGFuZ3p3

\section 功能

报警条件数据的实现类。

*/

#ifndef MDALARMCONDITIONIMP_H
#define MDALARMCONDITIONIMP_H

#include <QString>
#include "MDAlarmLevel.h"
#include "MDEventHelper.h"
#include "MDActionSP.h"
#include <QDataStream>

class CMDAlarmConditionImp
{
public:
    CMDAlarmConditionImp();
     virtual ~CMDAlarmConditionImp();
    CMDAlarmConditionImp(const CMDAlarmConditionImp& obj);
    CMDAlarmConditionImp& operator = (const CMDAlarmConditionImp& obj);
/*!
\section 属性
*/
public:
    QString m_name;
    QString m_comment;
    QString m_levelName;
    QString m_expression;
    QString m_bindvar;
    bool m_enabled;
    QString m_customId;
    int m_serverity;
    bool m_delayEnabled;
    QString m_delayConditionExp;
    QString m_delayTimeExp;
    bool m_latched;
    bool m_acknowledgeRequired;
    QString m_enableAlarmExp;
public:
    CMDActionSPList m_spList;
public:
    bool copy(const CMDAlarmConditionImp* source);

/*!
\section 序列化
*/
public:
    bool serialize(QDataStream& stream, bool useCurrentVersion = true);
    void serializeOnVersion_1(QDataStream& stream, bool useCurrentVersion);
    void serializeOnVersion_2(QDataStream& stream, bool useCurrentVersion);
public:
    void deserialize(QDataStream& stream);
    void deserializeOnVersion_1(QDataStream& stream);
    void deserializeOnVersion_2(QDataStream& stream);

/*!
\section 序列化：JSON格式
*/
public:
    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);
public:
    quint8 m_version ;// 0;
};

#endif // MDALARMCONDITIONIMP_H
