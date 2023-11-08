/*!

\author dGFuZ3p3

\section 功能

报警等级与报警颜色数据的实现类。

*/

#ifndef MDALARMLEVELIMP_H
#define MDALARMLEVELIMP_H

#include <QVariant>
#include <QString>
#include <QtGui/QRgb>
#include "MDAlarmLevel.h"
#include <QJsonObject>
#include <QDataStream>

class CMDAlarmColorImp
{
public:
    CMDAlarmColorImp();
    CMDAlarmColorImp(QRgb f, QRgb b);
     virtual ~CMDAlarmColorImp();
    CMDAlarmColorImp(const CMDAlarmColorImp& obj);
    CMDAlarmColorImp& operator = (const CMDAlarmColorImp& obj);
/*!
\section 属性
*/
public:
    QRgb m_f ;// qRgb(255, 0, 0);
    QRgb m_b ;// qRgb(0, 0, 0);
public:
    bool copy(const CMDAlarmColorImp* source);

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

class CMDAlarmLevelImp
{
public:
    CMDAlarmLevelImp();
     virtual ~CMDAlarmLevelImp();
    CMDAlarmLevelImp(const CMDAlarmLevelImp& obj);
    CMDAlarmLevelImp& operator = (const CMDAlarmLevelImp& obj);
/*!
\section 属性
*/
public:
    QString m_name;
    quint8 m_level ;// 0;
    QString m_comment;
    quint32 m_delayLength ;// 0;
public:
    CMDAlarmColor m_color_occur;
    CMDAlarmColor m_color_confirm_not_end;
    CMDAlarmColor m_color_confirm_end;
    CMDAlarmColor m_color_end_not_confirm;
    CMDAlarmColor m_color_end_confirm;
public:
    bool copy(const CMDAlarmLevelImp* source);

/*!
\section 序列化
*/
public:
    bool serialize(QDataStream& stream, bool useCurrentVersion = true);
    void serializeOnVersion_1(QDataStream& stream, bool useCurrentVersion);
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

#endif // MDALARMLEVELIMP_H
