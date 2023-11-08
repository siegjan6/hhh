/*!

\author dGFuZ3p3

\section 功能

帐户配置时属性数据类。

*/

#ifndef MDACCOUNTPROPERTY_CONFIG_H
#define MDACCOUNTPROPERTY_CONFIG_H

#include <QString>
#include "MDSecurityPolicyHelper_Global.h"
#include <QJsonObject>

class CMDAccountPropertyData_Config;
class CMDActionSPList;
class MDSECURITYPOLICYHELPERSHARED_EXPORT CMDAccountProperty_Config
{
public:
    CMDAccountProperty_Config();
     virtual ~CMDAccountProperty_Config();
    CMDAccountProperty_Config(const CMDAccountProperty_Config& obj);
    CMDAccountProperty_Config& operator = (const CMDAccountProperty_Config& obj);
/*!
\section 属性
*/
public:
    QString name() const;
    void setName(const QString& name);

    QString password() const;
    void setPassword(const QString& password);

    QString comment() const;
    void setComment(const QString& comment);

    qint8 type() const;
    void setType(qint8 type);
public:
    bool isEnable() const;
    void setEnable(bool enable);

    bool isLocked() const;
    void setLocked(bool locked);

    bool isLogin();
    void setLogined(bool logined);

    bool isVisible() const;
    void setVisible(bool visible);
public:
    CMDActionSPList* spList();
    bool setSPList(const CMDActionSPList* spList);
public:
    virtual bool copy(const CMDAccountProperty_Config* source);

/*!
\section 序列化
*/
public:
    virtual bool serialize(QDataStream& stream, bool useCurrentVersion = true);
    virtual void deserialize(QDataStream& stream);
/*!
\section 序列化：JSON格式
*/
public:
    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);
protected:
    CMDAccountPropertyData_Config* m_data;
};

#endif // MDACCOUNTPROPERTY_CONFIG_H
