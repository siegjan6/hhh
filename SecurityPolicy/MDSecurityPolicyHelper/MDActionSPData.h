#ifndef MDACTIOMDPDATA_H
#define MDACTIOMDPDATA_H

#include <QDataStream>
#include "MDActionID.h"
#include "MDSecurityPolicyConstant.h"

class CMDActionSPData
{
public:
    CMDActionSPData();
    virtual ~CMDActionSPData();
   CMDActionSPData(const CMDActionSPData& obj);
   CMDActionSPData& operator = (const CMDActionSPData& obj);
public:
    CMDActionID m_actionID;
public:
    MDActionProtectMode m_protectMode;
    MDSigType m_sigType;
public:
    bool m_enableLog;
public:
    QStringList m_operatorGroupList;
    QStringList m_verifierGroupList;
public:
    bool copy(const CMDActionSPData* source);
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

public:
    // 运行时签名操作参数-临时存储签名信息，不序列化，需要拷贝
    bool m_needSignature;
};

#endif // MDACTIOMDPDATA_H
