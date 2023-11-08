/*!

\author dGFuZ3p3

\section 功能

项目参数数据类的实现类。

*/

#ifndef MDPROJECTPARADATA_H
#define MDPROJECTPARADATA_H

#include <QString>
#include <QDateTime>
#include <QVariant>
#include <QDataStream>
#include "MDActionSP.h"

enum class ConnectAuthMode {
    /// 匿名
    ANONYMOUS = 1,
    /// 用户密码
    USER_PASSWORD = 2,
    /// 授权
    CERTIFICATE = 4,
    /// 授权+私钥
    CERTIFICATE_PRIVATEKEY = 8,

    /// length
    MAXLENGTH
};

enum class SecurityPolicy {
    /// 无
    NONE,
    ///
    BASIC128RSA15,
    ///
    BASIC256,
    ///
    BASIC256SHA256,
    ///
    AES128SHA256RSAOAEP,
    ///
    AES256SHA256RSAPSS,

    /// length
    MAXLENGTH
};

enum class MessageSecurityMode {
    NONE = 1,
    SIGN = 2,
    SIGNANDENCRYPT = 3,
    /// length
    MAXLENGTH
};

class CMDProjectParaData
{
public:
    CMDProjectParaData();
     virtual ~CMDProjectParaData();
    CMDProjectParaData(const CMDProjectParaData& obj);
    CMDProjectParaData& operator = (const CMDProjectParaData& obj);
/*!
\section 属性
*/
public:
    CMDActionSPList m_spList;
    bool m_enableDeviceCloudClient;
    QString m_cloudClientID;

    // opc ua server config
    int m_port;
    /// ConnectAuthMode 组合
    quint16 m_caMode;
    SecurityPolicy m_securityPolicy;
    MessageSecurityMode m_messageSecurityMode;
    QString m_user;
    QString m_password;
    QString m_certificatePath;
    QString m_privateKeyPath;
public:
    bool copy(const CMDProjectParaData* source);

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
public:
    quint8 m_version ;// 0;
};

#endif // MDPROJECTPARADATA_H
