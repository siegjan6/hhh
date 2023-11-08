#ifndef MDPOWERGROUPDATA_H
#define MDPOWERGROUPDATA_H

#include <QString>
#include <QDateTime>
#include <QVariant>
#include <QDataStream>
#include "MDActionSP.h"

class CMDPowerGroupData
{
public:
    CMDPowerGroupData();
     virtual ~CMDPowerGroupData();
    CMDPowerGroupData(const CMDPowerGroupData& obj);
    CMDPowerGroupData& operator = (const CMDPowerGroupData& obj);
public:
    QString m_name;
    QString m_comment;
    int m_level;
    QStringList m_accountList;
public:
    bool m_enable;
    bool m_visible;
public:
    CMDActionSPList m_spList;
public:
    bool copy(const CMDPowerGroupData* source);
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
    quint8 m_version;
};

#endif // MDPOWERGROUPDATA_H
