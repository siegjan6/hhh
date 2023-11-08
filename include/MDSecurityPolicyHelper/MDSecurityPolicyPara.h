/*!

\author dGFuZ3p3

\section 功能

安全策略运行时参数类。

*/

#ifndef MDSECURITYPOLICYPARA_H
#define MDSECURITYPOLICYPARA_H

#include <QString>
#include "MDSecurityPolicyHelper_Global.h"
#include <QJsonObject>

class CMDSecurityPolicyParaData;
class MDSECURITYPOLICYHELPERSHARED_EXPORT CMDSecurityPolicyPara
{
public:
    CMDSecurityPolicyPara();
    virtual ~CMDSecurityPolicyPara();
   CMDSecurityPolicyPara(const CMDSecurityPolicyPara& obj);
   CMDSecurityPolicyPara& operator = (const CMDSecurityPolicyPara& obj);
/*!
\section 属性
*/
public:
   //! 项目的安全策略是否被启用
   //! 若为 false，则安全策略全部被忽略
    bool isEnableSP() const;
    void setEnableSP(bool enable);

    //! 运行项目前是否要求登录
    bool isEnableLoginBeforeRun() const;
    void setEnableLoginBeforeRun(bool enable);
public:
    virtual bool copy(const CMDSecurityPolicyPara* source);

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
    CMDSecurityPolicyParaData* m_data;
};

#endif // MDSECURITYPOLICYPARA_H
