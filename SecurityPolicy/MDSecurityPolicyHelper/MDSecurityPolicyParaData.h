#ifndef MDSECURITYPOLICYPARADATA_H
#define MDSECURITYPOLICYPARADATA_H

#include <QString>
#include <QDateTime>
#include <QVariant>
#include <QDataStream>

class CMDSecurityPolicyParaData
{
public:
    CMDSecurityPolicyParaData();
     virtual ~CMDSecurityPolicyParaData();
    CMDSecurityPolicyParaData(const CMDSecurityPolicyParaData& obj);
    CMDSecurityPolicyParaData& operator = (const CMDSecurityPolicyParaData& obj);
public:
    bool m_enableSP;
    bool m_enableLoginBeforeRun;
public:
    bool copy(const CMDSecurityPolicyParaData* source);
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

#endif // MDSECURITYPOLICYPARADATA_H
