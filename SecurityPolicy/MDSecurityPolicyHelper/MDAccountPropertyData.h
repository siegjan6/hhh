#ifndef MDACTIONPROPERTYDATA_H
#define MDACTIONPROPERTYDATA_H

#include <QString>
#include <QDateTime>
#include <QVariant>
#include <QDataStream>

#include "MDActionSP.h"

class CMDAccountPropertyData_Config
{
public:
    CMDAccountPropertyData_Config();
     virtual ~CMDAccountPropertyData_Config();
    CMDAccountPropertyData_Config(const CMDAccountPropertyData_Config& obj);
    CMDAccountPropertyData_Config& operator = (const CMDAccountPropertyData_Config& obj);
public:
    QString m_name;
    QString m_password;
    QString m_comment;
    qint8 m_type;
public:
    bool m_enable;
    bool m_locked;
    bool m_logined;
    bool m_visible;
public:
    CMDActionSPList m_spList;
public:
    bool copy(const CMDAccountPropertyData_Config* source);
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

class CMDAccountPropertyData_Runtime
{
public:
    CMDAccountPropertyData_Runtime();
    ~CMDAccountPropertyData_Runtime();
   CMDAccountPropertyData_Runtime(const CMDAccountPropertyData_Runtime& obj);
   CMDAccountPropertyData_Runtime& operator = (const CMDAccountPropertyData_Runtime& obj);
public:
    long m_loginCount;
    QDateTime m_lastLoginTime;
};

#endif // MDACTIONPROPERTYDATA_H
